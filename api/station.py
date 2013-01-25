import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('~/Mist/db')
import DBConfig

def insertStationInDatabase(stationID, desc, latitude, longitude):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("INSERT INTO climateStations (climateStationID, description, latitude, longitude) VALUES (%s, %s, %s, %s)", (stationID, desc, latitude, longitude))
	results = cursor.lastrowid
	db.commit()
	return results

def getStation(stationID):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM climateStations WHERE climateStations.climateStationID = (%s)", stationID)
	results = cursor.fetchone()
	return results	

class station:
	#GET API FOR STATION
	#expected parameter: stationid (long)
	#RETURN: ??? TODO
	def GET(self):
		station_data = web.input()
		render = web.template.render('templates/')
	
		#parameters	
		stationid = station_data.stationid

		station = getStation(stationid) # SQL
		print station
		return render.station("STATION " + str(station[0]) + " " + str(station[1]) + " " + str(station[7]) + " " + str(station[8]))

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
