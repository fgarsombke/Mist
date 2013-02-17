import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db/')
from db import DBConfig
import json
import collections

class WeatherData:
    def __init__(self, startTemp, endTemp, windVelocity, avgHumidity, avgPressure, avgDew, timestamp):
        self.startTemp = startTemp
        self.endTemp = endTemp
        self.windVelocty = windVelocity
        self.avgHumidity = avgHumidity
        self.avgPressure = avgPressure
        self.avgDew = avgDew
        self.timestamp = timestamp

def findNearestClimateStation(latitude, longitude):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT *, 3956 * 2 * ASIN(SQRT(POWER(SIN((%s - latitude) *  pi()/180 / 2), 2) + COS(%s * pi()/180) * COS(%s * pi()/180) * POWER(SIN((%s - longitude) * pi()/180 / 2), 2) )) as distance FROM climateStations ORDER BY distance limit 1", (latitude, latitude, latitude, longitude)) # THIS FINDS THE NEAREST WEATHER STATION
    nearestStation = cursor.fetchone()
    return nearestStation[0]

def getWeatherData(station, time):
	
    conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()														 
	
    sqlString = """SELECT * FROM hourlyClimateData WHERE climateStationID = '%s' AND hourlyClimateData.date > DATE_SUB(now(), INTERVAL 1 DAY)""" % (station)
	cursor.execute(sqlString)
	results = cursor.fetchall()	
	cursor.close()
	
    #compute WeatherData object
	avgDailyTemp = 0
	avgDailyWind = 0
	avgDailyHumidity = 0
	avgDailyPressure = 0
	avgDailyDew = 0
	maxTemp = -1000
	minTemp = 1000

	humidCount = 0
	tempCount = 0
	pressureCount = 0
	dewCount = 0
	windCount = 0

	#iterate through the last 24 hours of weather readings
	if len(results) > 12:				
		# average each of the last 21 readings
		for result in results:
			
			#verify that all results exist (NOT NULL)
			if result[2] is not None:
				tempCount += 1
				#update min/max temps
                if result[2] >= maxTemp:
					maxTemp = result[2]
				if result[2] <= minTemp:
					minTemp = result[2]
				
				avgDailyTemp += result[2]
	
            if result[3] is not None:
				humidCount += 1
				avgDailyHumidity += result[3]

            if result[4] is not None:
				pressureCount += 1
    			avgDailyPressure += result[4]

            if result[5] is not None:
				dewCount += 1
				avgDailyDew += result[5]

            if result[6] is not None:
				windCount += 1
				avgDailyWind += result[6]
	
		#compute averages
		if humidCount != 0 and tempCount != 0 and pressureCount != 0 and dewCount != 0 and windCount != 0:	
			avgDailyTemp = avgDailyTemp / tempCount
			avgDailyWind = avgDailyWind / windCount
			avgDailyHumidity = avgDailyHumidity / humidCount
			avgDailyPressure = avgDailyPressure / pressureCount
			avgDailyDew = avgDailyDew / dewCount
			isoTime = results[len(results) - 1][8] #Most recent time reading

        data = WeatherData(startTemp, endTemp, avgDailyTemp, avgDailyWind, avgDailyHumidity, avgDailyPressure, avgDailyDew)

        return data


class aWeather:
    #GET API FOR WEATHER
    #RETURN weather data object: expetected parameter: latitude, longitude, time
    def GET(self):
        weather_data = web.input()
        devices = 0

        if weather_data:
            #get nearest station
            station = findNearestClimateStation(weather_data.latitude, weather_data.longitude)
            #compute necessary data from that station
            weatherData = getWeatherData(station, time)
            #JSONize
            objects_list = []
            d = collections.OrderedDict()
            d['startTemp'] = weatherData.startTemp
            d['endTemp'] = weatherData.endTemp
            d['avgTemp'] = weatherData.avgTemp
            d['windVelocity'] = weatherData.windVelocity
            d['avgHumidity'] = weatherData.avgHumidity
            d['avgPressure'] = weatherData.avgPressure
            d['avgDew'] = weatherData.avgDew
            d['timeStamp'] = weatherData.timestamp
            objects_list.append(d)
            j = json.dumps(objects_list)
            #return JSONized data
            return j
        else:
            return 0

    #POST API for WeatherData
    def POST(self):
        return deviceId
