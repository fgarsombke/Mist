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
from ctypes import c_double
from ctypes import c_ulonglong
import os

def getForecastForLocationAndDate(latitude, longitude, begin, end):
    url = "http://quattrotap.com:8080/api/weather?latitude=%s&longitude=%s&begin=%s&end=%s" % (latitude, longitude, begin, end)
    print url
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
    wd = wdObject[0]
    print wd['MaxRH']
    #CALL C++ ET CODE
    ET = ctypes.cdll.LoadLibrary('/Users/makilian/Mist/lib/libETCalc.so')
    ETCalc = ET.CalculateET_0
    ETCalc.argtypes = [c_double, c_double, c_ulonglong, c_ulonglong, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double]
    eto = ETCalc(latitude, longitude, begin, end, 30.0, 40.0, 35.0, .2, .8, 32.0, 39.0, 100.0, 25.0, .7)

    print eto

    return eto

def main():
    forecastETo(40, -90, "1366768754", "1366779536")

if __name__ == '__main__':
    main()  
