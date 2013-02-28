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
from jsonencode import MyEncoder

class WeatherData:
    def __init__(self, startTemp, endTemp, avgTemp, avgWind, avgHumidity, avgPressure, avgDew, timestamp):
        self.startTemp = startTemp
        self.endTemp = endTemp
        self.avgTemp = avgTemp
        self.avgWind = avgWind
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

    #PARAMETERIZE TIME INTERVAL!!!

    sqlString = """SELECT * FROM hourlyClimateData WHERE climateStationID = '%s' AND hourlyClimateData.date > DATE_SUB(now(), INTERVAL 1 DAY)""" % (station)
    cursor.execute(sqlString)
    results = cursor.fetchall() 
    cursor.close()

    #compute WeatherData object
    avgTemp = 0
    avgWind = 0
    avgHumidity = 0
    avgPressure = 0
    avgDew = 0
    maxTemp = -1000
    minTemp = 1000

    humidCount = 0
    tempCount = 0
    pressureCount = 0
    dewCount = 0
    windCount = 0

    for result in results:  
        #verify that all results exist (NOT NULL)
        if result[2] is not None:
            tempCount += 1
            #update min/max temps
            if result[2] >= maxTemp:
                maxTemp = result[2]
            if result[2] <= minTemp:
                minTemp = result[2] 
            avgTemp += result[2]

        if result[3] is not None:
            humidCount += 1
            avgHumidity += result[3]

        if result[4] is not None:
            pressureCount += 1
            avgPressure += result[4]

        if result[5] is not None:
            dewCount += 1
            avgDew += result[5]

        if result[6] is not None:
            windCount += 1
            avgWind += result[6]

    #compute averages
    isoTime = ""
    if humidCount != 0 and tempCount != 0 and pressureCount != 0 and dewCount != 0 and windCount != 0:  
        avgTemp = avgTemp / tempCount
        avgWind = avgWind / windCount
        avgHumidity = avgHumidity / humidCount
        avgPressure = avgPressure / pressureCount
        avgDew = avgDew / dewCount #NOT USING
        isoTime = results[len(results) - 1][8] #Most recent time reading

    data = WeatherData(minTemp, maxTemp, avgTemp, avgWind, avgHumidity, avgPressure, avgDew, isoTime)
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
            weatherData = getWeatherData(station, weather_data.time)
            #JSONize
            objects_list = []
            d = collections.OrderedDict()
            d['startTemp'] = weatherData.startTemp
            d['endTemp'] = weatherData.endTemp
            d['avgTemp'] = weatherData.avgTemp
            d['avgWind'] = weatherData.avgWind
            d['avgHumidity'] = weatherData.avgHumidity
            d['avgPressure'] = weatherData.avgPressure
            d['avgDew'] = weatherData.avgDew
            d['timestamp'] = weatherData.timestamp
            objects_list.append(d)
            j = json.dumps(objects_list, cls=MyEncoder)
            #return JSONized data
            return j
        else:
            return 0

    #POST API for WeatherData
    def POST(self):
        return deviceId
