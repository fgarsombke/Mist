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
import urllib2
import urllib

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

def getScheduleForDevice(productID):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()	
	cursor = db.cursor()
	cursor.execute("SELECT * FROM queuedIrrigations WHERE (queuedIrrigations.productID = (%s))", (productID))
	results = cursor.fetchall()
	return results	

class schedule:
	def GET(self):
		schedule_data = web.input()
		render = web.template.render('templates/', base='layout')
		#get user session
		session = web.config._session	
		#if we've got the deviceID for the user...
		if session.deviceID:
			#use deviceID to get schedule from api/aSchedule.
			query_args = {'deviceID':session.deviceID}
			encoded_args = urllib.urlencode(query_args)
			url = "http://0.0.0.0:8080/api/schedule?" + encoded_args
			schedule = ""
			try:
			  	schedule = urllib2.urlopen(url)
			except urllib2.URLError, e:
				print e
			return render.schedulelist(schedule)
		else:
			return render.schedule()

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
