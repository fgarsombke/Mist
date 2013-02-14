import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
import collections
sys.path.append('../db/')
from db import DBConfig
import json

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

def getUserWithEmail(userEmail):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT * FROM users WHERE users.email= (%s)", userEmail)
    results = cursor.fetchone()
    return results

def getAllUsers():
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT * FROM users")
    results = cursor.fetchall()
    return results

def getUser(userid):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM users WHERE users.userID = (%s)", userid)
    results = cursor.fetchone()
    return results
    
def getDeviceIDForUser(userid):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()    
    cursor = db.cursor()
    cursor.execute("SELECT * FROM devices WHERE devices.userID = (%s)", userid)
    results = cursor.fetchone()
    return results[0]   


class aUser:
    #GET API FOR USER
    #return one user: expected parameter: userID
    #return all users: expected parameter: password
    def GET(self):
        user_data = web.input()
        #check for parameters
        if user_data:
            email = user_data.userID
            suppliedPass = user_data.password
            user = getUserWithEmail(email) # SQL
            salt = user[3] 
            encodedPass = hashlib.sha512(suppliedPass + salt).hexdigest()
            if encodedPass == user[2]: #password is the 2nd column of users table
            #valid, so return user object
                d = collections.OrderedDict()
                d['userID'] = user[0]
                d['email'] = user[1]
                d['password'] = user[2]
                d['salt'] = user[3]
                d['created'] = user[4]
                d['updated'] = user[5]
                j = json.dumps(d)
                return j
            else: #password incorrect
                return 0
        else: #no parameters supplied, return all users.
            users = getAllUsers()
            objects_list = []
            for row in users:
                d = collections.OrderedDict()
                d['userID'] = row[0]
                d['email'] = row[1]
                d['password'] = row[2]
                d['salt'] = row[3]
                d['created'] = row[4]
                d['updated'] = row[5]
                objects_list.append(d)
            j = json.dumps(objects_list)
            return j

    #POST API FOR USER CREATION
    #expeceted parameters: email (string) and password (string)
    #return USER ID
    def POST(self):
        user_data = web.input() 

        #hash+salt password
        salt = uuid.uuid4().hex
        encodedPass = hashlib.sha512(user_data.password + salt).hexdigest()

        #store in DB
        userID = insertUserInDatabase(user_data.email, salt, encodedPass) #SQL

        #return ID of new user to requester
        return userID
