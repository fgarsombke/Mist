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
        #TODO MAKE THIS API WORK WITH GRACZYKS SIMULATOR TO CALL THE LEARNING CODE
        data = web.data()
        data = eval(data)
        deviceID = str(data['deviceID'])
        zoneNumber = str(data['zoneNumber'])
        created = str(data['created'])
        value = str(data['value'])
        storeFeedback(deviceID, zoneNumber, created, value)
        
        learn.doLearning(deviceID, zoneNumber, currentTime)
        #print web.input()
        #print FBdata.deviceID
        #if FBData.json:
        #    jsonFeedback = FBdata.json
        #    fb = json.loads(jsonFeedback)
        #    for zone in fb:
        #        for item in zone["feedback"]:
        #            storeFeedback(FBdata.deviceID, zone["zoneNumber"], item["time"], item["value"])
        #else:
        #    storeFeedback(FBdata.deviceID, FBdata.zoneNumber, FBdata.create, FBData.value)
        #return 0
