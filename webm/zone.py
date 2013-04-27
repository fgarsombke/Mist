import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig

def insertZoneInDatabase(deviceID, zoneNumber, description, plantSpecies, microClimateConstant, speciesConstant, densityConstant):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("INSERT INTO zones (deviceID, zoneNumber, description, plantSpecies, microClimateConstant, speciesConstant, densityConstant) VALUES (%s, %s, %s, %s, %s, %s, %s)", (deviceID, zoneNumber, description, plantSpecies, microClimateConstant, speciesConstant, densityConstant))
	db.commit()

def getOneZone(deviceID, zoneNumber):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM zones WHERE zones.deviceID = (%s) AND zones.zoneNumber = (%s)", deviceID, zoneNumber)
	results = cursor.fetchone()
	return results	

def getAllZones(deviceID):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM zones WHERE zones.deviceID = (%s)", deviceID)
	results = cursor.fetchall()
	return results	

class zone:
	#GET API FOR ALL ZONES
	#expected parameter: deviceID (long), zoneNumber (optional)
	#RETURN: one zone or all zones based on input
	def GET(self):
		zone_data = web.input()
		render = web.template.render('templates/')
	
		#parameters	
		deviceID = zone_data.deviceID

		if zone_data.zoneNumber is None:
			zones = getAllZones(deviceID)
			return render.zone("ZONE " + str(zones))
		else:
			zone = getOneZone(deviceID, zone_data.zoneNumber)	
			return render.zone("ZONE " + str(zone[0]) + " " + str(zone[1]) + " " + str(zone[2]) + " " + str(zone[3]))

	#POST API FOR ZONE CREATION
	#expeceted parameters: deviceID, zoneNumber, description, plantSpecies, microClimateConstant, speciesConstant, densityConstant 
	#return SUCCESS/FAIL 
	def POST(self):
		zone_data = web.input()	
		render = web.template.render('templates/')
		
		#verify parameters

		#if verified, put in database
		userId = insertZoneInDatabase(zone_data.deviceID, zone_data.zoneNumber, zone_data.description, zone_data.plantSpecies, zone_data.microClimateConstant, zone_data.speciesConstant, zone_data.densityConstant) #SQL
		return render.zone("SUCCESS")
