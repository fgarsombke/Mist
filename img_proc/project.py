#!/usr/bin/env python

import BeautifulSoup
from googlevoice import Voice
import argparse
import re
import sqlite3
import sys
import img_db
import logging

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

    
def main():
    parser = argparse.ArgumentParser(description='Gather image processing information.')
    parser.add_argument('img_dir', type=str, metavar='IMG_DIR',
                        help='Directory where images to be rated are stored.')
    parser.add_argument('image', type=str, metavar='IMAGE',
                      help='File name of current Image to be rated.')
    parser.add_argument('-d', '--debug', dest='do_dbg', action='store_true', default=False)
                    
    args = parser.parse_args()
    
    if args.do_dbg:
        print('Debugging enabled.')
        logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)
    
    db = img_db.ImgDB(args.img_dir, repopulate=True)
   
   
    #poll(opts.image, int(opts.score)) # Polls gvoice and stores scores

if __name__ == '__main__':
    main()

