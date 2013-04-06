import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import json
import collections
from jsonencode import MyEncoder


def storeFeedback(deviceID, feedback):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()

    #TODO: Need to associate with a vectorID - how?

    for fbTuple in feedback:
        cursor.execute("INSERT INTO feedback (vectorID, deviceID, created, value) VALUES (%s, %s, %s, %S)", (vectorID, deviceID, fbTuple.created, fbTuple.value))

    db.commit()
    return 0

class aFeedback:
    def GET(self):
        feedback_data = web.input()
        return 0

    def POST(self):
        feedback_data = web.input()
        feedbackArray = json.loads(feedback_data.jsonArray)

        storeFeedback(deviceID, feedbackArray)
        return 0
