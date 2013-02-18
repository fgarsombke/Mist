import pymetar
import sys
import datetime
import time
import math
import MySQLdb
sys.path.append('../db')
import DBConfig

def main():	
	#get list of all devices 
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM devices");
	devices = cursor.fetchall()

	#for each devices 
	for device in devices:
		deviceId = device[0]
		climateStationId = device[7] 
	
		sqlString = """SELECT * FROM dailyETo WHERE climateStationID = '%s' AND dailyETo.date > DATE_SUB(now(), INTERVAL 1 DAY)""" % (climateStationId)  #change from CURDATE to most recent download.  also need to verify all cycles included
		cursor = db.cursor()														 
		cursor.execute(sqlString)
		results = cursor.fetchall()	
		cursor.close()		

		cursor = db.cursor()
		cursor.execute("""SELECT * FROM zones WHERE productId = (%s)""", deviceId)
		zones = cursor.fetchall()
		cursor.close()

		#iterate through each DailyETo reading that hasn't been converted yet TODO: ADD CONVERTED COLUMN TO DailyETo table so we can select the ones that only need to be converted
		for result in results:	
			#verify that all results exist (NOT NULL)
			print result				
			#convert the DailyETo value to DailyETL value for each zone in the device			
			for zone in zones:
				dailyETL = computeETLforZoneFromDailyETo(zone, result)												

				productID = zone[0]
				zoneNumber = zone[1]
				dayOfYear = result[2]
				date =  result[3]
				cursor = db.cursor()
				
				cursor.execute("""INSERT INTO dailyETLperZone (productID, zoneNumber, dailyETL, dayOfYear, date) VALUES (%s, %s, %s, %s, '%s')""" % (productID, zoneNumber, dailyETL, dayOfYear, date))
				resulter = cursor.fetchone()
				cursor.close()
				db.commit()

def computeETLforZoneFromDailyETo(zone, result):	
	dailyET = result[1]
	densityFactor = zone[4]
	speciesFactor = zone[5]
	microClimateFactor = zone[6]
	return float(dailyET) * float(densityFactor) * float(speciesFactor) * float(microClimateFactor)
	

if __name__ == '__main__':
	main()	
