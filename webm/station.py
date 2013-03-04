import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import urllib2
import urllib
import json

class addStation:
    def GET(self):
        render = web.template.render('templates/', base = 'layout')
        return render.addStation()

    def POST(self):
        render = web.template.render('templates/', base = 'layout')
        station_data = web.input()
        
        climateStationID = station_data.climateStationID
        description = station_data.description
        city = station_data.city
        county = station_data.county
        zipCode = station_data.zipCode
        state = station_data.state
        country = station_data.country
        latitude = station_data.latitude
        longitude = station_data.longitude
        altitude = station_data.altitude

        #Construct API request
        query_args = {'climateStationID':climateStationID, 'description':description, 'city':city, 'county':county, 'zipCode':zipCode, 'state':state, 'country':country, 'latitude':latitude, 'longitude':longitude, 'altitude':altitude}
        encoded_args = urllib.urlencode(query_args)
        url = "http://0.0.0.0:8080/api/station?"
        req = urllib2.Request(url, encoded_args)
        stationID = ""
        #Execute API request
        try:
            stationID = json.loads(urllib2.urlopen(req).read())
        except urllib2.URLError, e:
            print e

        return render.addStation()

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

    def POST(self):
        render = web.template.render('templates/')
        return render.station()
