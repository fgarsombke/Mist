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
        FBdata = web.input()
        if FBData.json:
            jsonFeedback = FBdata.json
            string = json.dump(jsonFeedback)
            storeFeedback(FBdata.deviceID, FBdata.zoneNumber, FBdata.create, FBData.value)
        else:
            storeFeedback(FBdata.deviceID, FBdata.zoneNumber, FBdata.create, FBData.value)
        return 0
