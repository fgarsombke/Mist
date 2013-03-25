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

class aFeedback:
    def GET(self):
        feedback_data = web.input()
        return 0

    #POST API for WeatherData
    def POST(self):
        feedback_data = web.input()
        return 0
