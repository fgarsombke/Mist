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

def getToggleStatus(productID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT * FROM toggle WHERE productID = (%s)", productID)
    results = cursor.fetchone()
    db.commit()
    return results

def doToggle(productID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("UPDATE toggle SET toggleStatus = !toggleStatus WHERE productID = (%s)", productID)
    results = cursor.lastrowid
    db.commit()
    return results

class aToggle:
    def GET(self):
        toggle_data = web.input()
        return getToggleStatus(toggle_data.productID)

    #POST API for WeatherData
    def POST(self):
        toggle_data = web.input()
        doToggle(toggle_data.productID)
        j = getToggleStatus(toggle_data.productID)
        j2 = str(j).replace('(', '{')
        j2 = j2.replace(')', '}')
        return j2
