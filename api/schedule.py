import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from datetime import datetime
import time
sys.path.append('../db')
from db import DBConfig

def queueIrrigation(productID, zoneNumber, startTime, duration, date):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()	
	cursor = db.cursor()
	startTime = date + " " + startTime #NEED TO FIX THIS FORMATTING
	timestamp = datetime.now() 
	cursor.execute("INSERT INTO queuedIrrigations (productID, zoneNumber, startTime, duration, created) VALUES (%s, %s, %s, %s, %s)", (productID, zoneNumber, startTime, duration, timestamp))
	results = cursor.lastrowid
	db.commit()
	return results

def getScheduleForDeviceAndZone(productID, zoneNumber):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()	
	cursor = db.cursor()
	cursor.execute("SELECT * FROM queuedIrrigations WHERE (queuedIrrigations.productID = (%s) AND queuedIrrigations.zoneNumber = (%s))", (productID, zoneNumber))
	results = cursor.fetchall()
	return results	

class schedule:
	#GET API FOR SCHEDULE
	#Return all queued irrigations for given device: expected parameter: productID (integer), zoneNumber (integer)
	def GET(self):
		schedule_data = web.input()
		render = web.template.render('templates/', base='layout')

		if schedule_data:	
			productID = schedule_data.productID
			zoneNumber = schedule_data.zoneNumber
			schedule = getScheduleForDeviceAndZone(productID, zoneNumber) # SQL
			return render.schedulelist(schedule)
		else:
			return render.schedule()

	#POST API - queue an irrigation event
	#parameters productID, zoneNumber, startTime, duration
	def POST(self):
		schedule_data = web.input()	
		render = web.template.render('templates/', base='layout')
	
		if schedule_data:	
			#productID = schedule_data.productID
			productID = 3460
			zoneNumber = schedule_data.zoneNumber
			startTime = schedule_data.startTime
			date = schedule_data.date
			duration = schedule_data.duration
			queueIrrigation(productID, zoneNumber, startTime, duration, date) 
			return render.schedule()
