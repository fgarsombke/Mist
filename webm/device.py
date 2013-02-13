import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db/')
from db import DBConfig
import urllib2
import urllib
import json

def insertDeviceInDatabase(productId, userId, latitude, longitude, stationID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("INSERT INTO devices (productID, userID, latitude, longitude, climateStationID) VALUES (%s, %s, %s, %s, %s)", (productId, userId, latitude, longitude, stationID))
    results = cursor.lastrowid
    db.commit()
    return results

def findNearestClimateStation(latitude, longitude):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT *, 3956 * 2 * ASIN(SQRT(POWER(SIN((%s - latitude) *  pi()/180 / 2), 2) + COS(%s * pi()/180) * COS(%s * pi()/180) * POWER(SIN((%s - longitude) * pi()/180 / 2), 2) )) as distance FROM climateStations ORDER BY distance limit 1", (latitude, latitude, latitude, longitude)) # THIS FINDS THE NEAREST WEATHER STATION
    nearestStation = cursor.fetchone()
    return nearestStation[0]

class addDevice:
    def GET(self):
        render = web.template.render('templates/', base='layout')
        return render.addDevice()


    def POST(self):
        render = web.template.render('templates/', base='layout')
        device_data = web.input()
        # get parameters and do stuff
        userID = device_data.userID
        latitude = device_data.latitude
        longitude = device_data.longitude
        wifiNetwork = device_data.wifiNetwork
        wifiPassword = device_data.wifiPassword
        numZones = device_data.numZones

        query_args = {'userID':userID, 'latitude':latitude, 'longitude':longitude, 'wifiNetwork':wifiNetwork, 'wifiPassword':wifiPassword, 'numZones':numZones}
        encoded_args = urllib.urlencode(query_args)
        url = "http://0.0.0.0:8080/api/device?"
        req = urllib2.Request(url, encoded_args)
        devices = ""

        #Execute API request
        try:
            devices = json.loads(urllib2.urlopen(req).read())
        except urllib2.URLError, e:
            print e

        return render.addDevice() 

class device:
    #GET the device page.  
    #check for sessionID (if logged in return users device, otherwise return all devices)
    def GET(self):
        device_data = web.input()
        render = web.template.render('templates/', base='layout')   
        session = web.config._session

        #Construct API request
        if session.deviceID:
            query_args = {'deviceID':session.deviceID}
        else:
            query_args = {}

        encoded_args = urllib.urlencode(query_args)
        url = "http://0.0.0.0:8080/api/device?" + encoded_args
        devices = ""

        #Execute API request
        try:
            devices = json.loads(urllib2.urlopen(url).read())
        except urllib2.URLError, e:
            print e

        #Render results
        return render.device(devices)

    #POST API FOR DEVICE CREATION
    #expeceted parameters: userid, latitude, longitude, wifi network, wifi password, stationID 
    #return DEVICE ID?  or actuallt maybe this is supplied by the device it self???
    def POST(self):
        device_data = web.input()   
        render = web.template.render('templates/')
        
        #verify parameters

        #find nearest climate station!
        assignedStationID = findNearestClimateStation(device_data.latitude, device_data.longitude)

        #if verified, put in database
        userId = insertDeviceInDatabase(device_data.productid, device_data.userid, device_data.latitude, device_data.longitude, assignedStationID) #SQL
        return render.user(userId)
