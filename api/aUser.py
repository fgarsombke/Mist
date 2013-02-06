import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db/')
from db import DBConfig

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
	#return one user: expected parameter: userId
	#return all users: expected parameter: null
	def GET(self):
		user_data = web.input()

		if user_data:
			userid = user_data.userid
			user = getUser(userid)	
			return user
		else:
			users = getAllUsers()
			return users

	#POST API FOR USER CREATION
	#expeceted parameters: email (string) and password (string)
	#return USER ID
	def POST(self):
		user_data = web.input()	
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
			return userID
