import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db')
from db import DBConfig
import urllib2
import urllib
import json

def insertStationInDatabase(stationID, desc, latitude, longitude):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("INSERT INTO climateStations (climateStationID, description, latitude, longitude) VALUES (%s, %s, %s, %s)", (stationID, desc, latitude, longitude))
    results = cursor.lastrowid
    db.commit()
    return results

class station:
    #GET the station page.
    #check for stationID parameter.  if exists, get specific station, otherwise get all.
    def GET(self):
        station_data = web.input()
        render = web.template.render('templates/', base = 'layout')

        #Construct API request  
        if station_data:
            if station_data.stationID:  
                #stationID query, get one station
                query_args = {'stationID':station_data.stationID} 
        else:
            #empty query, get all stations
            query_args = {}

        encoded_args = urllib.urlencode(query_args)
        url = "http://0.0.0.0:8080/api/station?" + encoded_args
        stations = ""

        #Execute API request
        try:
            stations = json.loads(urllib2.urlopen(url).read()) 
        except urllib2.URLError, e:
            print e

        #Render results
        return render.station(stations)

    #POST API FOR STATION CREATION
    #expeceted parameters: stationid, description, latitude, longitude
    #return STATION ID? 
    def POST(self):
        station_data = web.input()  
        render = web.template.render('templates/')
        
        #verify parameters
        print "TEST"

        #if verified, put in database
        stationId = insertStationInDatabase(station_data.stationid, station_data.description, station_data.latitude, station_data.longitude) #SQL
        return render.station(stationId)
