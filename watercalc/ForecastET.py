import pymetar
import sys
import datetime
import time
import math
import MySQLdb
import ComputeET
import urllib
import urllib2
import json
import ctypes
import os

def getForecastForLocationAndDate(latitude, longitude, begin, end):
    url = "http://0.0.0.0:8080/api/weather?latitude=%s&longitude=%s&beginTime=%s&endTime=%s" % (latitude, longitude, begin, end)

    req = urllib2.Request(url)
    try:
        response = json.loads(urllib2.urlopen(req).read())
    except urllib2.URLError, e:
        print e

    print response

    #structure and return the data for ET computation
    return response

def forecastETo(latitude, longitude, begin, end):
    wdObject = getForecastForLocationAndDate(latitude, longitude, begin, end)
    
    #USE ctypes to turn this WeatherData object into an ETo value: Graczyk's extern C function.
    #ETCalc = ctypeslib.load_library('ETCalc.so', '.')

    #then return ETo
    return 7.0

def main():
    forecastETo(30, -100, "1366768754", "1366779536")

if __name__ == '__main__':
    main()  
