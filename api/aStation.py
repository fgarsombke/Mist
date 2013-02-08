import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db')
from db import DBConfig

def insertStationInDatabase(stationID, desc, latitude, longitude):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("INSERT INTO climateStations (climateStationID, description, latitude, longitude) VALUES (%s, %s, %s, %s)", (stationID, desc, latitude, longitude))
	results = cursor.lastrowid
	db.commit()
	return results

def getAllStations():
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM climateStations")
	results = cursor.fetchall()
	return results

def getStation(stationID):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM climateStations WHERE climateStations.climateStationID = (%s)", stationID)
	results = cursor.fetchone()
	return results	

class aStation:
	#GET API FOR STATION
	#reutrn one station: expected parameter: stationid (long)
	#return all stations: expected parameter: null
	def GET(self):
		station_data = web.input()
	
		#parameters
		if station_data:
			if station_data.stationID:	
				station = getStation(station_data.stationID) # SQL
				return station 
		else:
			stations = getAllStations()
			return stations

	#POST API FOR STATION CREATION
	#expeceted parameters: stationid, description, latitude, longitude
	#return STATION ID? 
	def POST(self):
		station_data = web.input()	
		
		#verify parameters

		#if verified, put in database
		stationId = insertStationInDatabase(station_data.stationid, station_data.description, station_data.latitude, station_data.longitude) #SQL
		return stationId
