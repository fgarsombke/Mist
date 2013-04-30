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
from watercalc import learn

def storeFeedback(deviceID, zoneNumber, created, value):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("INSERT INTO feedback (deviceID, zoneNumber, created, value) VALUES (%s, %s, FROM_UNIXTIME(%s), %s)", (deviceID, zoneNumber, created, value))
    db.commit()
    return 0

class aFeedback:
    def GET(self):
        feedback_data = web.input()
        return 0

    def POST(self):
        data = web.data()
        data = eval(data)
        print data['deviceID']
        if 'json' in data: #then this is coming from the simulator
            json = data['json'] 
            fb = json.loads(json)
            for zone in fb['array']:
                for item in zone["feedback"]:
                    storeFeedback(fb['deviceID'], zone["zoneNumber"], item["time"], item["value"])
            #run the learning code
            learn.doLearning(deviceID, zoneNumber, currentTime)
        else:#this is coming from the iPhone app
            deviceID = str(data['deviceID'])
            zoneNumber = str(data['zoneNumber'])
            created = str(data['created'])
            value = str(data['value'])
            storeFeedback(deviceID, zoneNumber, created, value)

