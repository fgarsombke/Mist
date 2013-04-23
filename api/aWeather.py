import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import json
import collections
from jsonencode import MyEncoder
import datetime
from datetime import date
from datetime import timedelta
import urllib2
import urllib
import time
from collections import defaultdict

class WeatherData:
    def __init__(self, startTemp, endTemp, avgTemp, avgWind, avgPressure, minRH, maxRH, minTemp, maxTemp):
        self.startTemp = startTemp
        self.endTemp = endTemp
        self.avgTemp = avgTemp
        self.avgWind = avgWind
        self.avgPressure = avgPressure
        self.minRH = minRH
        self.maxRH = maxRH
        self.minTemp = minTemp
        self.maxTemp = maxTemp

    def printWD(self):
        if self.avgTemp:
            print "Average Temperature: %s" % self.avgTemp
        if self.avgPressure:
            print "Average Pressure: %s" % self.avgPressure
        if self.avgWind:
            print "Average WindSpeed: %s" % self.avgWind
        if self.minRH:
            print "Minimum Relative Humidity: %s" % self.minRH
        if self.maxRH:
            print "Maximum Relative Humidity: %s" % self.maxRH
        if self.minTemp:
            print "Minimum Temp: %s" % self.minTemp
        if self.maxTemp:
            print "Maximum Temp: %s" % self.maxTemp
        if self.startTemp:
            print "Start Temp: %s" % self.startTemp
        if self.endTemp:
            print "End Temp: %s" % self.endTemp

def findNearestClimateStation(latitude, longitude):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT *, 3956 * 2 * ASIN(SQRT(POWER(SIN((%s - latitude) *  pi()/180 / 2), 2) + COS(%s * pi()/180) * COS(%s * pi()/180) * POWER(SIN((%s - longitude) * pi()/180 / 2), 2) )) as distance FROM climateStations ORDER BY distance limit 1", (latitude, latitude, latitude, longitude)) # THIS FINDS THE NEAREST WEATHER STATION
    nearestStation = cursor.fetchone()
    return nearestStation[0]

def forecastAPI(latitude, longitude, thetime, interval):
   apiKey = "6d838d5df77d3f8650aee0d173fcad4b"
   posix = time.mktime(thetime.timetuple())
   url = "http://api.forecast.io/forecast/%s/%s,%s,%s" % (apiKey, latitude, longitude, int(posix))
   req = urllib2.Request(url)
   response = ""
   try:
       response = json.loads(urllib2.urlopen(req).read())
   except urllib2.URLError, e:
       print e
   return response[interval]

def getWeatherData(latitude, longitude, begintime, endtime):
    #count # of minutes til next hour
    numMinutes = 60 - begintime.minute
    #count # of hours til midnight
    numHours = 24 - begintime.hour
    #count # of days til end day
    numDays = endtime.day - begintime.day #what if new yaer?
    #count # of hours til hour of end day
    numEndHours = endtime.hour
    #count # of minutes til minute of end day
    numEndMinutes = endtime.minute
    dataDict = {'beginMinutes': [], 'beginHours': [], 'days': [], 'endHours': [], 'endMinutes': []}
    #this will create an array of [4, 10, 15, 10 ,4] which is # of calls of each kind to API

    #then use correct logic to compute the correct values for Michael Graczyk
    i = begintime.minute
    #while i < 60:
        #call API, minute resolution
    #    aTime = begintime + timedelta(minutes=i)
    #    results = forecastAPI(latitude, longitude, aTime, 'minutely')
    #    dataRes = results['data']   
    #    dataDict['beginMinutes'].append(dataRes[i])
    #    i+=1
    i = begintime.hour
    while i < 24:
        #call API, hour resolution
        aTime = begintime + timedelta(hours=i)
        results = forecastAPI(latitude, longitude, aTime, 'hourly')
        dataRes = results['data']
        dataDict['beginHours'].append(dataRes[i])
        i+=1
    i = 0
    while i < numDays:
        #call API, day resolution
        aTime = begintime + timedelta(days=i)
        results = forecastAPI(latitude, longitude, aTime, 'daily')
        dataRes = results['data']
        dataDict['days'].append(dataRes[0])
        i+=1
    i = 0
    while i < numEndHours:
        #call API, hours resolution
        aTime = begintime - timedelta(hours=i)
        results = forecastAPI(latitude, longitude, aTime, 'hourly')
        dataRes = results['data']
        dataDict['endHours'].append(dataRes[i])
        i+=1
    i = 0
    #while i < numEndMinutes:
    #    #call API, days resolution
    #    aTime = endTime - timedelta(minutes=i)
    #    results = forecastAPI(latitude, longitude, aTime, 'minutely')
    #    dataDict['endMinutes'].append(results)
    #    i+=1

    data = createWeatherDataObjectFromDictionary(dataDict)
    return data

def createWeatherDataObjectFromDictionary(dictionary):
    beginMinutes = dictionary['beginMinutes']
    beginHours = dictionary['beginHours']
    days = dictionary['days']
    endHours = dictionary['endHours']
    endMinutes = dictionary['endMinutes']

    #min temp
    #max temp
    #min humidity
    #max humidity
    #startTemp
    #endTemp

    #average temp over time period
    #average pressure over time period
    #average wind speed over time period
    #sunrise time on start day
    #sunrise time on end day
    #sunset time on start day
    #sunset time on end day
    #rainfall in MM
    
    startTemp = 0
    endTemp = 0
    minTemp = 200
    maxTemp = -100
    minRH = 1
    maxRH = 0
    count = 0
    avgTempSum = 0
    avgPressureSum = 0
    avgWindSum = 0
    #srStart
    #srEnd
    #ssStart
    #ssEnd
    #rainfall

    for minute in beginMinutes:
        if count == 0:
            startTemp = minute['temperature']
        if minute['temperature'] < minTemp:
            minTemp = minute['temperature']
        if minute['temperature'] > maxTemp:
            maxTemp = minute['temperature']
        if minute['humidity'] > maxRH:
            maxRH = minute['humidity']
        if minute['humidity'] < minRH:
            minRH = minute['humidity']
        avgTempSum += minute['temperature']
        avgPressureSum += minute['pressure']
        avgWindSum += minute['windSpeed']
        count += 1.0

    for hour in beginHours:
        if hour['temperature'] < minTemp:
            minTemp = hour['temperature']
        if hour['temperature'] > maxTemp:
            maxTemp = hour['temperature']
        if hour['humidity'] > maxRH:
            maxRH = hour['humidity']
        if hour['humidity'] < minRH:
            minRH = hour['humidity']
        avgTempSum += hour['temperature']*60.0
        avgPressureSum += hour['pressure']*60.0
        avgWindSum += hour['windSpeed']*60.0
        count += 60.0

    for day in days:
        if day['temperatureMin'] < minTemp:
            minTemp = day['temperatureMin']
        if day['temperatureMax'] > maxTemp:
            maxTemp = day['temperatureMax']
        if day['humidity'] > maxRH:
            maxRH = day['humidity']
        if day['humidity'] < minRH:
            minRH = day['humidity']
        avgTempSum += (day['temperatureMax'] + day['temperatureMin'])/2.0*60.0*24.0
        avgPressureSum += day['pressure']*60.0*24.0
        avgWindSum += day['windSpeed']*60.0*24.0
        count += 60.0*24.0

    for hour in endHours:
        if hour['temperature'] < minTemp:
            minTemp = hour['temperature']
        if hour['temperature'] > maxTemp:
            maxTemp = hour['temperature']
        if hour['humidity'] > maxRH:
            maxRH = hour['humidity']
        if hour['humidity'] < minRH:
            minRH = hour['humidity']
        avgTempSum += hour['temperature']*60.0
        avgPressureSum += hour['pressure']*60.0
        avgWindSum += hour['windSpeed']*60.0
        count += 60.0

    for minute in endMinutes:
        if minute['temperature'] < minTemp:
            minTemp = minute['temperature']
        if minute['temperature'] > maxTemp:
            maxTemp = minute['temperature']
        if minute['humidity'] > maxRH:
            maxRH = minute['humidity']
        if minute['humidity'] < minRH:
            minRH = minute['humidity']
        avgTempSum += minute['temperature']
        avgPressureSum += minute['pressure']
        avgWindSum += minute['windSpeed']
        count += 1

    avgTemp = avgTempSum/count
    avgPressure = avgPressureSum/count
    avgWind = avgWindSum/count
    
    wd = WeatherData(startTemp, endTemp, avgTemp, avgWind, avgPressure, minRH, maxRH, minTemp, maxTemp)
    return wd


def main():
    #huh
    result = getWeatherData(30, -100, datetime.datetime.now() - datetime.timedelta(days=5), datetime.datetime.now())
    result.printWD()

if __name__ == '__main__':
    main()

class aWeather:
    #GET API FOR WEATHER
    #reroute to Forecast.io API
    #RETURN weather data object: expetected parameter: latitude, longitude, POSIX time begin and end
    def GET(self):
        weather_data = web.input()
        devices = 0

        if weather_data:
            latitude = weather_data.latitude
            longitude = weather_data.longitude
            beginTime = weather_data.begin
            endTime = weather_data.end
            weatherData = getWeatherData(latitude, longitude, beginTime, endTime)

            #JSONize WeatherData object
            objects_list = []
            d = collections.OrderedDict()
            d['StartTemp'] = weatherData.startTemp
            d['EndTemp'] = weatherData.endTemp
            d['AvgTemp'] = weatherData.avgTemp
            d['AvgWind'] = weatherData.avgWind
            d['AvgPressure'] = weatherData.avgPressure
            d['MinRH'] = weatherData.minRH
            d['MaxRH'] = weatherData.maxRH
            objects_list.append(d)
            j = json.dumps(objects_list, cls=MyEncoder)
            #return JSONized data
            return j
        else:
            return 0

    #POST API for WeatherData
    def POST(self):
        return deviceId
