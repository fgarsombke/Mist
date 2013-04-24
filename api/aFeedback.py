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


def storeFeedback(deviceID, zoneNumber, created, value):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("INSERT INTO feedback (deviceID, zoneNumber, created, value) VALUES (%s, %s, %s, %S)", (deviceID, zoneNumber, created, value))
    db.commit()
    return 0

class aFeedback:
    def GET(self):
        feedback_data = web.input()
        return 0

    def POST(self):
        data = web.data()
        print data
        deviceID = data['deviceID']
        zoneNumber = data['zoneNumber']
        created = data['created']
        value = data['value']
        storeFeedback(deviceID, zoneNumber, created, value)
        
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
