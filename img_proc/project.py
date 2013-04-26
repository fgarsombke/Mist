#!/usr/bin/env python

import BeautifulSoup
from googlevoice import Voice
from optparse import OptionParser
import re
import sqlite3
import sys

# Extracts SMS messages and returns them as a list of dictionaries, one per message
# From the pygooglevoice examples
# John Nagle, nagle@animats.com
def extractsms(htmlsms) :
    msgitems = []
    #   Extract all conversations by searching for a DIV with an ID at top level.
    tree = BeautifulSoup.BeautifulSoup(htmlsms)                 # parse HTML into tree
    conversations = tree.findAll("div",attrs={"id" : True},recursive=False)
    for conversation in conversations :
        #       For each conversation, extract each row, which is one SMS message.
        rows = conversation.findAll(attrs={"class" : "gc-message-sms-row"})
        #       Additonally get the phone number
        type = conversation.findAll("span", attrs={"class" : "gc-message-type"})
        for row in rows :                                                               # for a
            #   For each row, which is one message, extract all the fields.
            msgitem = {"id" : conversation["id"]}               # tag this message with convers
            spans = row.findAll("span",attrs={"class" : True}, recursive=False)
            for span in spans :                                                 # for all spans
                cl = span["class"].replace('gc-message-sms-', '')
                msgitem[cl] = (" ".join(span.findAll(text=True))).strip()       # put text in d
            msgitems.append(msgitem)                                    # add msg dictionary to

    return msgitems

def poll(img_name, score_range):
    score_re = re.compile("-?[0-9]+")
    num_re = re.compile("[0-9]{9,12}")

    # login to gvoice
    voice = Voice();
    voice.login()

    raw_input("Press ENTER to end poll...\n") # waits until we are ready

    hist = [0]*(score_range*2+1) # initialize histogram

    print "Results for '" + img_name + "':"
    print "SENDER\t\tSCORE"
    voice.sms()
    for ballot in extractsms(voice.sms.html):
        # remove blank matches
        phone_num = filter(None, num_re.findall(ballot['from']))
        score = filter(None, score_re.findall(ballot['text']))

        # checks if the first number is a valid score
        if len(score) != 0 and -score_range <= int(score[0]) <= score_range:
            resp = "Thank you!\nYou rated '" + img_name + "' as a " + score[0] + "."
            hist[(int(score[0])+score_range)] = hist[(int(score[0])+score_range)] + 1; # Histogram
            print phone_num[0] + "\t" + score[0]

        else:
            resp = "You submitted an invalid rating for '" + img_name + "'!\n"
            resp = resp + "Valid scores are " + str(-score_range) + " to " + str(score_range) + ".\n"
            resp = resp + "Please try again."
            print phone_num[0] + "\t" + "invalid"

        # send responses
        # voice.send_sms(phone_num[0], resp);

    # print histogram
    print ""
    print "HISTOGRAM"
    for idx in range(-score_range,score_range+1):
        print '%+03d' % idx,
    print ""
    for val in hist:
        print '%3d' % val,
    print ""
    print ""

    # removes read messages
    #for message in voice.sms().messages:
        #if message.isRead:
        #    message.delete() # moves to trash

    voice.logout()

    store(img_name, hist, score_range)

def store(img_name, hist, score_range):
    # connect to the db
    conn = sqlite3.connect("image_scores.db")
    cursor = conn.cursor()

    print "TABLE " + img_name
    try:
        create_table = "CREATE TABLE "+ img_name + "(score integer, votes integer)"
        cursor.execute(create_table);

        for idx in range(-score_range, score_range+1):
            entry = "INSERT INTO " + img_name + " VALUES (" + str(idx) +", " + str(hist[idx+score_range]) + ")"
            cursor.execute(entry)

        print "  CREATED"

    except sqlite3.OperationalError:
        for idx in range(-score_range, score_range+1):
            update = "UPDATE " + img_name + " "  
            set = "SET votes=" + str(hist[idx+score_range]) + " " 
            where = "WHERE score=" + str(idx) + " " 
            cursor.execute(update + set + where);

        print "  UPDATED"

    conn.commit()

def main():
    parser = OptionParser(usage = "Usage: %prog -f <filename> -s <score>")
    parser.add_option("-f", "--filename", 
            dest="filename", 
            metavar="FILENAME",
            help="filename of the image to be scored",)
    parser.add_option("-s", "--score", 
            dest="score", 
            default="10",
            metavar="SCORE",
            help="defines scoring range [-SCORE SCORE], defaults to 10",)
    (opts, args) = parser.parse_args()

    if opts.filename is None:
        print "ERROR: no image specified\n"
        parser.print_help()
        exit(-1)

    poll(opts.filename, int(opts.score))

if __name__ == '__main__':
    main()

