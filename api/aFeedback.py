import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import json
import collections
import datetime
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
        sim = web.input()
        if sim.simulator:  #then this is coming from the simulator
            for zone in data["zones"]:
                fbItems = zone["fbItems"]
                for fb in fbItems:
                    storeFeedback(data['deviceID'], zone["zoneNumber"], fb["time"], fb["value"])
            endTime = datetime.datetime.fromtimestamp(data['endTime'])
            learn.doLearning(data['deviceID'], zone["zoneNumber"], endTime)
        else:#this is coming from the iPhone app
            deviceID = str(data['deviceID'])
            zoneNumber = str(data['zoneNumber'])
            created = str(data['created'])
            value = str(data['value'])
            storeFeedback(deviceID, zoneNumber, created, value)

