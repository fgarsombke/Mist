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
from api import aWeather as aWeatherAPI

def getForecastForLocationAndDate(latitude, longitude, begin, end):
    result = aWeatherAPI.aWeather().getHelper(latitude, longitude, begin, end)

    #structure and return the data for ET computation
    return result

def forecastETo(latitude, longitude, begin, end):
    wd = getForecastForLocationAndDate(latitude, longitude, begin, end)

    #CALL C++ ET CODE
    ET = ctypes.cdll.LoadLibrary('/Users/makilian/Mist/lib/libETCalc.so')
    ETCalc = ET.CalculateET_0

    ETCalc.argtypes = [c_double, c_double, c_ulonglong, c_ulonglong, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double]
    ETCalc.restype = c_double

    if isinstance(wd, str):
        wd = json.loads(wd) 

    eto = ETCalc(latitude, longitude, long(begin), long(end), wd['MinTemp'], wd['MaxTemp'], wd['AvgTemp'], wd['MinRH'], wd['MaxRH'], wd['StartTemp'], wd['EndTemp'], wd['AvgPressure'], wd['AvgWindSpeed'], .6)

    return eto

def main():
    ETo = forecastETo(40.0, -90.0, "1366768754", "1366779536")
    print ETo

if __name__ == '__main__':
    main()  
