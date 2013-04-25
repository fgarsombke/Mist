#!/usr/bin/env python

import BeautifulSoup
from googlevoice import Voice
from optparse import OptionParser
import re
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

def store_results(img_name):
    qual_re = re.compile("^(-?10|-?[0-9])$")
    num_re = re.compile("[0-9]{9,12}")

    # login to gvoice
    voice = Voice();
    voice.login()

    raw_input("Press ENTER to process sms...\n") # waits until we are ready

    voice.sms()
    print "SENDER\t\tQUALITY"
    hist = [0]*21
    for ballot in extractsms(voice.sms.html):
        num = filter(None, num_re.findall(ballot['from']))
        qual = filter(None, qual_re.findall(ballot['text']))

        # valid
        if len(qual) != 0:
            resp = "Thank you!\nYou rated '" + img_name + "' as a " + qual[0] + "."
            hist[(int(qual[0])+10)] = hist[(int(qual[0])+10)] + 1; # Histogram

            print num[0] + "\t" + qual[0]

        # invalid
        else:
            resp = "You submitted an invalid rating for '" + img_name + "'!\nPlease try again."

            print num[0] + "\t" + "invalid"

        voice.send_sms(num[0], resp);

    # shows a histogram of the results
    print ""
    print "HISTOGRAM"
    for idx in range(-10,11):
        print '%+03d' % idx,
    print ""
    for val in hist:
        print '%3d' % val,
    print ""

    # deletes all texts
    for message in voice.sms().messages:
        if message.isRead:
            message.delete() # moves read messages to trash

    voice.logout()

def main():
    parser = OptionParser(usage = "Usage: %prog -i <image>")
    parser.add_option("-i", "--image", 
            dest="filename", 
            metavar="IMAGE",
            help="image to display",)
    (opts, args) = parser.parse_args()

    if opts.filename is None:
        print "ERROR: no image specified\n"
        parser.print_help()
        exit(-1)

    store_results(opts.filename)

if __name__ == '__main__':
    main()
