#!/usr/bin/env python

import BeautifulSoup
from googlevoice import Voice
import argparse
import re
import sqlite3
import sys
import img_db
import logging
import user_score
import datetime as dt

MaxTimePass = dt.timedelta(seconds=5*60)

def extractsms(htmlsms):
    """
    extractsms  --  extract SMS messages from BeautifulSoup tree of Google Voice SMS HTML.

    Output is a list of dictionaries, one per message.
    """
    msgitems = []										# accum message items here
    #	Extract all conversations by searching for a DIV with an ID at top level.
    tree = BeautifulSoup.BeautifulSoup(htmlsms)			# parse HTML into tree
    conversations = tree.findAll("div",attrs={"id" : True},recursive=False)
    for conversation in conversations :
        #	For each conversation, extract each row, which is one SMS message.
        rows = conversation.findAll(attrs={"class" : "gc-message-sms-row"})
        for row in rows :								# for all rows
            #	For each row, which is one message, extract all the fields.
            msgitem = {"id" : conversation["id"]}		# tag this message with conversation ID
            spans = row.findAll("span",attrs={"class" : True}, recursive=False)
            for span in spans :							# for all spans in row
                cl = span["class"].replace('gc-message-sms-', '')
                msgitem[cl] = (" ".join(span.findAll(text=True))).strip()	# put text in dict
            msgitems.append(msgitem)					# add msg dictionary to list
    return msgitems
   

def get_thanks_reply(img_name, score):
    return "Thank you!  You rated %s with a score of %s."%(img_name,score)
    
def get_nothanks_reply(img_name, minscore, maxscore):
    return "Thank you for your reponse, but we couldn't understand your score!  "+\
            "Valid scores are numbers between %s and %s."%(minscore, maxscore) +\
            "Please try again."
            
def do_respond(voice, sender, resp, id):
    voice.send_sms(sender, resp)
    logging.debug('Replied with message:\n"%s"\n'%(resp))
    return

def process_scores(img_name, db):
    score_re = re.compile("-?\d+\.?\d*")
    
    # login to gvoice and get sms
    voice = Voice();
    logging.debug("Logging into Google Voice...")
    voice.login()
    logging.debug("Logged in successfully.\n")
    logging.debug("Retrieving SMS messages...")
    folder = voice.sms()
    logging.debug("SMS messages retrieved successfully.\n")
    print_bar = '----------------------------------------------'

    for msg in extractsms(voice.sms.html):
        text = msg['text']
        sender = msg['from']
        id = msg['id']
        sendTimeStr = msg['time']
        
        sendResponse = False
        
        # Check if the message is trash, or ours
        if (not text.startswith("Thank you")):      
            logging.debug(print_bar)
            logging.debug('Processing message %s from %s'%(text, sender))
            try:
                okayScore = False
                score = 0
                
                try:
                    score = float(score_re.search(text).group(0))
                    logging.debug('Read score: %s'%(score)) 
                    okayScore = (score is not None) and score >= db.minscore and score <= db.maxscore
                except:
                    logging.debug("Couldn't parse score.")
                    pass
                    
                if okayScore:                 
                    if db.add_score(img_name, user_score.Score(sender, score, id)):
                        try:
                            resp = get_thanks_reply(img_name, score)
                            sendResponse = True
                        except:
                            print("Error while sending reply to %s for message '%s'.\n"%(sender, text))                          
                else:
                    resp = get_nothanks_reply(img_name, db.minscore, db.maxscore);               
                    sendResponse = True
                
                tFormat = '%I:%M %p'
                msgTime = dt.datetime.strptime(sendTimeStr, tFormat)
                nowT = dt.datetime.now()
                msgTime = dt.datetime(nowT.year, nowT.month, nowT.day, msgTime.hour, msgTime.minute, msgTime.second)
                
                tDiff = (nowT - msgTime)

                if sendResponse:
                    if tDiff is not None and tDiff < MaxTimePass:
                        do_respond(voice, sender, resp, id)
                    else:
                        logging.debug('Message too old.  Not responding.')
                        
                # Delete the message so we don't respond to it again
                logging.debug('Deleting message')
                voice._Voice__messages_post('delete', id, trash=1)
            except:
                print('Error processing message. "%s" from %s'%(text, sender))
                raise
            logging.debug(print_bar)
                
    voice.logout()
    db.end_transaction()
    
def main():
    parser = argparse.ArgumentParser(description='Gather image processing information.')
    parser.add_argument('img_dir', type=str, metavar='IMG_DIR',
                        help='Directory where images to be rated are stored.')
    parser.add_argument('image', type=str, metavar='IMAGE',
                        help='File name of current Image to be rated.')
    parser.add_argument('-min', '--minimum_score', dest='minscore', type=float, nargs=1,
                        help='Minium acceptible score.')
    parser.add_argument('-max', '--maximum_score',  dest='maxscore', type=float, nargs=1,
                        help='Maxium acceptible score.')
    parser.add_argument('-d', '--debug', dest='do_dbg', action='store_true', default=False)
                    
    args = parser.parse_args()
    
    if args.do_dbg:
        print('Debugging enabled.')
        logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)
    
    with img_db.ImgDB(args.img_dir, (args.minscore or [0])[0], (args.maxscore or [10])[0], repopulate=True) as db:
        if db.contains_image(args.image):
            process_scores(args.image, db)
        else:
            print("Could not find image %s in the directory."%args.image)
            
if __name__ == '__main__':
    main()

