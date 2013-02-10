import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db/')
from db import DBConfig
import urllib
import json
import urllib2

def validateEmail(email):
    return True

def validatePassword(password):
    return True

def insertUserInDatabase(username, salt, password):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("INSERT INTO users (email, password, salt) VALUES (%s, %s, %s)", (username, password, salt))
    results = cursor.lastrowid
    db.commit()
    return results
    
def getDeviceIDForUser(userid):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM devices WHERE devices.userID = (%s)", userid)
    results = cursor.fetchone()
    return results[0]   

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

    #POST API FOR USER CREATION
    #expeceted parameters: email (string) and password (string)
    #return USER ID
    def POST(self):
        user_data = web.input() 
        render = web.template.render('templates/')
        validate = True

        if user_data.email:
            validate = validateEmail(user_data.email)
        else:
            print "ERROR: Email parameter is empty."

        if user_data.password:
            validate = validatePassword(user_data.password)
        else:
            print "ERROR: Password parameter is empty."

        if validate:
            salt = uuid.uuid4().hex 
            encodedPass = hashlib.sha512(user_data.password + salt).hexdigest()
            userID = insertUserInDatabase(user_data.email, salt, encodedPass) #SQL
            return render.user(userID)

class logout:
    def GET(self):
        render = web.template.render('templates/', base='layout')
        session = web.config._session
        session.kill()
        return render.index2()

    def POST(self):
        print "POST"
