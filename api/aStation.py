import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import collections
import json

def insertStationInDatabase(climateStationID, description, city, county, zipCode, state, country, latitude, longitude, altitude):
    conf = DBConfig.DBConfig()
    results = conf.executeLastRowID("INSERT INTO climateStations (climateStationID, description, city, county, zipCode, state, country, latitude, longitude, altitude) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s)" % (climateStationID, description, city, county, zipCode, state, country, latitude, longitude, altitude))
    return results

def getAllStations():
    conf = DBConfig.DBConfig()
    results = conf.executeFetchAll("SELECT * FROM climateStations")
    return results

def getStation(stationID):
    conf = DBConfig.DBConfig()
    results = conf.executeFetchOne("SELECT * FROM climateStations WHERE climateStations.climateStationID = (%s)" % stationID)
    return results

class aStation:
    #GET API FOR STATION
    #reutrn one station: expected parameter: stationid (long)
    #return all stations: expected parameter: null
    def GET(self):
        station_data = web.input()

        #parameters
        stations = ""
        if station_data:
            if station_data.stationID:
                stations = getStation(station_data.stationID) # SQL
        else:
            stations = getAllStations()
        objects_list = []
        for row in stations:
            d = collections.OrderedDict()
            d['climateStationID'] = row[0]
            d['description'] = row[1]
            d['city'] = row[2]
            d['county'] = row[3]
            d['zipCode'] = row[4]
            d['state'] = row[5]
            d['country'] = row[6]
            d['latitude'] = row[7]
            d['longitude'] = row[8]
            d['altitude'] = row[9]
            objects_list.append(d)

        j = json.dumps(objects_list)
        return j

    #POST API FOR STATION CREATION
    #expeceted parameters: stationid, description, latitude, longitude, altitude
    #return STATION ID
    def POST(self):
        station_data = web.input()

        #verify parameters
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
       
        #if verified, put in database
        stationId = insertStationInDatabase(climateStationID, description, city, county, zipCode, state, country, latitude, longitude, altitude)
        
        return stationId
