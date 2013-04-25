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
    wd = getForecastForLocationAndDate(latitude, longitude, begin, end)

    #CALL C++ ET CODE
    ET = ctypes.cdll.LoadLibrary('/Users/makilian/Mist/lib/libETCalc.so')
    ETCalc = ET.CalculateET_0

    ETCalc.argtypes = [c_double, c_double, c_ulonglong, c_ulonglong, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double]
    ETCalc.restype = c_double

    eto = ETCalc(latitude, longitude, long(begin), long(end), wd['minTemp'], wd['maxTemp'], wd['AvgTemp'], wd['MinRH'], wd['MaxRH'], wd['StartTemp'], wd['EndTemp'], wd['AvgPressure'], wd['AvgWindSpeed'], .6)

    return eto

def main():
    ETo = forecastETo(40.0, -90.0, "1366768754", "1366779536")
    print ETo

if __name__ == '__main__':
    main()  
