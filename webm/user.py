import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
from db import DBConfig
import urllib
import json
import urllib2

def validateEmail(email):
    return True

def validatePassword(password):
    return True

def getDeviceIDForUser(userid):
    conf = DBConfig.DBConfig()
    results = conf.executeFetchOne("SELECT * FROM devices WHERE devices.userID = (%s)", userid)
    return results[0]

class addUser:
    def GET(self):
        render = web.template.render('templates/', base='layout')
        return render.addUser()

    def POST(self):
        render = web.template.render('templates/', base='layout')
        user_data = web.input()

        if user_data.email:
            email = user_data.email
            validate = validateEmail(user_data.email)
        else:
            print "ERROR: Email parameter is empty."

        if user_data.password:
            password = user_data.password
            validate = validatePassword(user_data.password)
        else:
            print "ERROR: Password parameter is empty."

        if validate:
            #Construct API request
            query_args = {'email':email, 'password':password}
            encoded_args = urllib.urlencode(query_args)
            url = "http://0.0.0.0:8080/api/user?"
            req = urllib2.Request(url, encoded_args)
            userID = ""
            #Execute API request
            try:
                userID = json.loads(urllib2.urlopen(req).read())
            except urllib2.URLError, e:
                print e

        return render.addUser()

class list:
    def GET(self):
        user_data = web.input()
        render = web.template.render('templates/', base='layout')
        url = "http://0.0.0.0:8080/api/user?" #+ encoded_args
        users = ""
        try:
            users = json.loads(urllib2.urlopen(url).read())
        except urllib2.URLError, e:
            print e
        return render.userlist(users)
    
    def POST(self):
        print "hi"

class user:
    #USER login, expecting email & password.
    #return user page & set up session variable if successful
    def GET(self):
        user_data = web.input()
        render = web.template.render('templates/', base='layout')
        #user clicked login form... check for data
        if user_data:
            userID = user_data.userID
            suppliedPass = user_data.password
            #TODO: switch to POST   
            #Construct API Request
            query_args = {'userID':userID, 'password':suppliedPass}
            encoded_args = urllib.urlencode(query_args)
            url = "http://0.0.0.0:8080/api/user?" + encoded_args
            user = ""
            #Execute API Request
            try:
                user = json.loads(urllib2.urlopen(url).read())
            except urllib2.URLError, e:
                print e
            if user:
                #API says it is valid, so set up session and return user page
                session = web.config._session
                session.count = session.count + 1
                session.userID = user['userID']
                #TODO: switch this to an API request
                session.deviceID = getDeviceIDForUser(user['userID']) 
                return render.user(user)
            else:
                #back to home page.
                return render.index2()
        else:
            return render.index2()

    def POST(self):
        render = web.template.render('templates/')
        return render.user()

class logout:
    def GET(self):
        render = web.template.render('templates/', base='layout')
        session = web.config._session
        session.kill()
        return render.index2()

    def POST(self):
        print "POST"
