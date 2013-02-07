import web
import sys
import hashlib
import uuid
import base64
import MySQLdb
sys.path.append('../db')
from db import DBConfig

def getAllUsers():
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT * FROM users")
	results = cursor.fetchone()
	return results

class list:
	#return all users: expected parameter: null
	def GET(self):
		user_data = web.input()
		render = web.template.render('templates/')	
		users = getAllUsers()
		return render.userlist(users)


	def POST(self):
