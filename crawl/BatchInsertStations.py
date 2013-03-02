import sys
import re
import MySQLdb
import csv
from db import DBConfig

def insertStationInDatabase(db, stationID, desc, latitude, longitude, elevation):
	cursor = db.cursor()
	cursor.execute("INSERT IGNORE climateStations (climateStationID, description, latitude, longitude, altitude) VALUES (%s, %s, %s, %s, %s)", (stationID, desc, latitude, longitude, elevation))
	cursor.close()
	db.commit()

def main():

  #	stationReader = csv.reader(open('TexasWeatherStations.csv', 'rb'), delimiter=' ', quotechar='|')

	stationReader = open('TexasWeatherStations.rtf').read()
	lines = stationReader.splitlines()
	
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()

	for row in lines:
		state = row[0:2]
		desc = row[3:20]
		stationID = row[20:24]
		latitude = row[34:40]
		longitude = row[42:49]
		elevation = row[50:54]
		metar = row[57:58]
	
		if metar == 'X':

			latMult = 1
			longMult = 1

			latitude1 = latitude.split()[0]
			latitude2 = latitude.split()[1]

			longitude1 = longitude.split()[0]
			longitude2 = longitude.split()[1]

			if latitude2.find('N') == -1:
				latitude2 = latitude2.split('S')[0]
				latMult = -1
			else:
				latitude2 = latitude2.split('N')[0]
		
			if longitude2.find('W') == -1:
				longitude2 = longitude2.split('E')[0]	
			else:
				longitude2 = longitude2.split('W')[0]
				longMult = -1

			longitude = float(longitude1) + float(longitude2)/60.0
			latitude = float(latitude1) + float(latitude2)/60.0
	
			longitude = longitude * longMult
			latitude = latitude * latMult
	
			print stationID + " " + desc + " " + str(latitude) + " " + str(longitude) + " " + elevation

			insertStationInDatabase(db, stationID, desc, latitude, longitude, elevation)
		
if __name__ == '__main__':
	main()
