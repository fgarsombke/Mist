import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('/Users/makilian/Mist/db')
import DBConfig

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

def getUser(userid):
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()	
	cursor = db.cursor()
	cursor.execute("SELECT * FROM users WHERE users.userID = (%s)", userid)
	results = cursor.fetchone()
	return results	

class user:
	#GET API FOR USER
	#expected parameter: userid (long), password
	#RETURN: user data TODO
	def GET(self):
		user_data = web.input()
		render = web.template.render('templates/')
	
		#parameters	
		userid = user_data.userid
		suppliedPass = user_data.password

		user = getUserWithEmail(userid) # SQL
		print user
		salt = user[3] #salt is the 4th column of users table

		encodedPass = hashlib.sha512(suppliedPass + salt).hexdigest()
		print encodedPass
		print user[2]

		if encodedPass == user[2]: #password is the 2nd column of users table
			return render.user("PASSWORD VERIFIED.  RETURN USER.")
		else:
			return render.user("INCORRECT PASSWORD.")

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
			print salt
			print len(salt)
			encodedPass = hashlib.sha512(user_data.password + salt).hexdigest()
			#TODO -  ORM (sqlalchemy?)
			userID = insertUserInDatabase(user_data.email, salt, encodedPass) #SQL
			return render.user(userID)
