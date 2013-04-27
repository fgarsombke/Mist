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

def getToggleStatus(deviceID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT * FROM toggle WHERE deviceID = (%s)", deviceID)
    results = cursor.fetchone()
    db.commit()
    return results

def doToggle(deviceID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("UPDATE toggle SET toggleStatus = !toggleStatus WHERE deviceID = (%s)", deviceID)
    results = cursor.lastrowid
    db.commit()
    return results

class aToggle:
    def GET(self):
        toggle_data = web.input()
        return getToggleStatus(toggle_data.deviceID)

    #POST API for WeatherData
    def POST(self):
        toggle_data = web.input()
        doToggle(toggle_data.deviceID)
        j = getToggleStatus(toggle_data.deviceID)
        j2 = str(j).replace('(', '{')
        j2 = j2.replace(')', '}')
        return j2
