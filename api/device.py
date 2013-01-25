import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('~/Mist/db')
import DBConfig

def insertDeviceInDatabase(productId, userId, latitude, longitude, stationID):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()	
	cursor = db.cursor()
	cursor.execute("INSERT INTO devices (productID, userID, latitude, longitude, climateStationID) VALUES (%s, %s, %s, %s, %s)", (productId, userId, latitude, longitude, stationID))
	results = cursor.lastrowid
	db.commit()
	return results

def getDevice(deviceid):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()	
	cursor = db.cursor()
	cursor.execute("SELECT * FROM devices WHERE devices.productID = (%s)", deviceid)
	results = cursor.fetchone()
	return results	

def findNearestClimateStation(latitude, longitude):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT *, 3956 * 2 * ASIN(SQRT(POWER(SIN((%s - latitude) *  pi()/180 / 2), 2) + COS(%s * pi()/180) * COS(%s * pi()/180) * POWER(SIN((%s - longitude) * pi()/180 / 2), 2) )) as distance FROM climateStations ORDER BY distance limit 1", (latitude, latitude, latitude, longitude)) # THIS FINDS THE NEAREST WEATHER STATION
	nearestStation = cursor.fetchone()
	return nearestStation[0]

class device:
	#GET API FOR DEVICE
	#expected parameter: deviceid (long)
	#RETURN: ??? TODO
	def GET(self):
		device_data = web.input()
		render = web.template.render('templates/')
	
		#parameters	
		deviceid = device_data.deviceid

		device = getDevice(deviceid) # SQL
		print device
		return render.device("DEVICE " + str(device[0]) + " " + str(device[1]) + " " + str(device[2]) + " " + str(device[3]))

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
