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

class list:
	def GET(self):
		user_data = web.input()
		render = web.template.render('templates/', base='layout')
		users = getAllUsers()
		return render.userlist(users)
	def POST(self):
		print "hi"

class user:
	#GET API FOR USER
	#return one user: expected parameter: useremail, password
	def GET(self):
		user_data = web.input()
		render = web.template.render('templates/', base='layout')

		if user_data:
			userid = user_data.userid
			suppliedPass = user_data.password
			user = getUserWithEmail(userid) # SQL
			salt = user[3] #salt is the 4th column of users table
			encodedPass = hashlib.sha512(suppliedPass + salt).hexdigest()
			
			if encodedPass == user[2]: #password is the 2nd column of users table
				session = web.config._session
				session.count = session.count + 1 
				session.userID = user[0]
				session.deviceID = getDeviceIDForUser(user[0])
				return render.user("Verified", user[1])
			else:
				#back to home page
				return render.index2()


	#POST API FOR USER CREATION
	#expeceted parameters: email (string) and password (string)
	#TODO:add e-mail confirmation and token generation.
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
			#TODO -  ORM (sqlalchemy?)
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
