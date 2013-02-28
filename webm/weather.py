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
import json


class weather:
    def GET(self):
        weather_data = web.input()
        render = web.template.render('templates/', base='layout')
        #if user submitted deviceID via a form...
        if weather_data:
            query_args = {'latitude':weather_data.latitude, 'longitude':weather_data.longitude, 'time':weather_data.time}
            encoded_args = urllib.urlencode(query_args)
            url = "http://0.0.0.0:8080/api/weather?" + encoded_args
            print url
            weatherdata = ""
            try:
                weatherdata = json.loads(urllib2.urlopen(url).read())
            except urllib2.URLError, e:
                print e
            return render.weatherlist(weatherdata)
        else:
            return render.getWeatherData()

    def POST(self):
        render = web.template.render('templates/', base='layout')
        return render.schedule()
