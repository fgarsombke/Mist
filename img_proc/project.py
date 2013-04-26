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

    # connect to the db
    conn = sqlite3.connect("temp.db")
    cursor = conn.cursor()

    # login to gvoice and get sms
    voice = Voice();
    voice.login()
    voice.sms()

    raw_input("Press ENTER to end poll...\n") # waits until we are ready

    print "Results for '" + img_name + "':"
    print "SENDER\t\tSCORE"

    # add the image if it doesn't exist
    try:
        add_image = "ALTER TABLE images ADD COLUMN " + img_name +" integer" 
        cursor.execute(add_image)
    except:
        pass

    for ballot in extractsms(voice.sms.html):
        # remove blank matches
        phone_num = filter(None, num_re.findall(ballot['from']))
        score = filter(None, score_re.findall(ballot['text']))

        # checks if the first number is a valid score
        if len(score) != 0 and -score_range <= int(score[0]) <= score_range:
            resp = "Thank you!\nYou rated '" + img_name + "' as a " + score[0] + "."

            # add the user if they do not exist
            try:
                add_user = "ALTER TABLE users ADD COLUMN `" + phone_num[0] +"` integer" 
                cursor.execute(add_user)
            except:
                pass

            # insert values
            add_usr_score = "INSERT INTO users (`" + phone_num[0] + "`) VALUES (" + score[0] +")"
            cursor.execute(add_usr_score)
            add_img_score = "INSERT INTO images (" + img_name + ") VALUES (" + score[0] + ")"
            cursor.execute(add_img_score)

            print phone_num[0] + "\t" + score[0]

        else:
            resp = "You submitted an invalid rating for '" + img_name + "'!\n"
            resp = resp + "Valid scores are " + str(-score_range) + " to " + str(score_range) + ".\n"
            resp = resp + "Please try again."
            print phone_num[0] + "\t" + "invalid"

        # send responses
        # voice.send_sms(phone_num[0], resp);

    # removes read messages
    #for message in voice.sms().messages:
        #if message.isRead:
        #    message.delete() # moves to trash

    # close sessions
    voice.logout()
    conn.commit()
    conn.close()

def init_db():
    # connect to the db
    conn = sqlite3.connect("temp.db")
    cursor = conn.cursor()

    # create tables, we have to create temporary columns 
    user_score_table = "CREATE TABLE IF NOT EXISTS users(tempUser integer)"
    cursor.execute(user_score_table)

    image_score_table = "CREATE TABLE IF NOT EXISTS images(tempImage integer)"
    cursor.execute(image_score_table)

    # commit and close
    conn.commit()
    conn.close()

def clean_db():
    # connect to the db
    conn = sqlite3.connect("temp.db")
    cursor = conn.cursor()

    # drop temporary columns
    try:
        cursor.execute("ALTER TABLE users DROP COLUMN tempUser")
    except:
        pass
    try:
        cursor.execute("ALTER TABLE images DROP COLUMN tempImage")
    except:
        pass

    # commit changes and close the db
    conn.commit()
    conn.close()

def main():
    parser = OptionParser(usage = "Usage: %prog -i IMAGE -s SCORE")
    parser.add_option("-i", "--image", 
            dest="image", 
            metavar="IMAGE",
            help="IMAGE is the name of the image to be scored",)
    parser.add_option("-s", "--score", 
            dest="score", 
            default="10",
            metavar="SCORE",
            help="defines scoring range [-SCORE SCORE], defaults to 10",)
    (opts, args) = parser.parse_args()

    if opts.image is None:
        print "ERROR: no image name specified\n"
        parser.print_help()
        exit(-1)

    init_db() # Initializes the db
    poll(opts.image, int(opts.score)) # Polls gvoice and stores scores
    clean_db() # Cleans the db

if __name__ == '__main__':
    main()

