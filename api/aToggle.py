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
    results = cursor.executeFetchOne("SELECT * FROM toggle WHERE deviceID = (%s)", deviceID)
    return results

def doToggle(deviceID):
    conf = DBConfig.DBConfig()
    results = conf.executeLastRowID("UPDATE toggle SET toggleStatus = !toggleStatus WHERE deviceID = (%s)", deviceID)
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
