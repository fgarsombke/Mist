import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import json
import collections

def insertDeviceInDatabase(userId, latitude, longitude, wifiNetwork, wifiPassword, numZones):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("INSERT INTO devices (userID, latitude, longitude, wifiNetwork, wifiPassword, numZones) VALUES (%s, %s, %s, %s, %s, %s)", (userId, latitude, longitude, wifiNetwork, wifiPassword, numZones))
    results = cursor.lastrowid
    db.commit()
    return results

def getAllDevices():
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM devices")
    results = cursor.fetchall()
    return results  

def getDevicesForUser(userid):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM devices WHERE devices.userID = (%s)", userid)
    results = cursor.fetchall()
    return results  

def getDevice(deviceid):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM devices WHERE devices.deviceID = (%s)", deviceid)
    results = cursor.fetchall()
    return results  

def findNearestClimateStation(latitude, longitude):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT *, 3956 * 2 * ASIN(SQRT(POWER(SIN((%s - latitude) *  pi()/180 / 2), 2) + COS(%s * pi()/180) * COS(%s * pi()/180) * POWER(SIN((%s - longitude) * pi()/180 / 2), 2) )) as distance FROM climateStations ORDER BY distance limit 1", (latitude, latitude, latitude, longitude)) # THIS FINDS THE NEAREST WEATHER STATION
    nearestStation = cursor.fetchone()
    return nearestStation[0]


class aDevice:
    #GET API FOR DEVICE
    #RRETURN one device: expected parameter: deviceID (long)
    #RETURN all devices: expetected parameter: none?
    def GET(self):
        device_data = web.input()   
        devices = 0     
    
        if device_data:
            if device_data.deviceID:
                devices = getDevice(device_data.deviceID)
        else:
            devices = getAllDevices()

        objects_list = []
        for row in devices:
                d = collections.OrderedDict()
                d['deviceID'] = row[0]
                d['userID'] = row[1]
                d['latitude'] = row[2]
                d['longitude'] = row[3]
                d['wifiNetwork'] = row[4]
                d['wifiPassword'] = row[5]
                d['numZones'] = row[6]
                d['climateStationID'] = row[7]
                objects_list.append(d)
 
        j = json.dumps(objects_list)    
        return j

    #POST API FOR DEVICE CREATION
    #expeceted parameters: userid, latitude, longitude, wifi network, wifi password, numZones 
    #return DEVICE ID?  or actually maybe this is supplied by the device it self???
    def POST(self):
        device_data = web.input()
        deviceId = 0

        if 'userID' in device_data:
            uid = device_data.userID
        else:
            uid = 1
        if 'latitude' in device_data:
            lat = device_data.latitude
        else:
            lat = 0
        if 'longitude' in device_data:
            longi = device_data.longitude
        else:
            longi = 0
        if 'wifiNetwork' in device_data:
            wifiN = device_data.wifiNetwork
        else:
            wifiN = ""
        if 'wifiPassword' in device_data:
            wifiP = device_data.wifiPassword
        else:
            wifiP = ""
        if 'numZones' in device_data:
            numZ = device_data.numZones
        else:
            numZ = 8
        deviceId = insertDeviceInDatabase(uid, lat, longi, wifiN, wifiP, numZ)
        print deviceId
        return deviceId
