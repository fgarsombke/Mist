#!/usr/bin/env python
import web
import sys
from webm.user import user
from webm.user import list
from webm.user import logout
from webm.device import device
from webm.station import station
from webm.zone import zone
from webm.index import index
from webm.schedule import schedule
from api.aUser import aUser
from api.aStation import aStation
from api.aDevice import aDevice
from api.aSchedule import aSchedule
sys.path.append('./db')
from db import DBConfig


if __name__ == '__main__':

	web.config.debug = False

	render = web.template.render('templates/')

	urls = (
		'/', 'index',
		'/user', 'user',
		'/user/list', 'list',
		'/user/logout', 'logout',
		'/station/list', 'station',
		'/device', 'device',
		'/zone', 'zone',
		'/schedule', 'schedule',
		'/api/user', 'aUser', 
		'/api/station', 'aStation',
		'/api/device', 'aDevice',
		'/api/schedule', 'aSchedule'
		)

	config = open("/Users/makilian/Mist/local.config")
	host = config.readline().rstrip()
	username = config.readline().rstrip()
	password = config.readline().rstrip()
	database = config.readline().rstrip()
	
	app = web.application(urls, globals())
	
	db = web.database(dbn='mysql', db=database, user=username, pw=password)
	store = web.session.DBStore(db, 'sessions')
	
	#IS THIS THE RIGHT PLACE TO CREATE THE SESSION? OR SHOULD IT BE IN USER?
	if web.config.get('_session') is None:
		session = web.session.Session(app, store, initializer={'count': 0, 'userID':0, 'deviceID':0})
		session.count = 4
		web.config._session = session
	else:
		session = web.config._session
		
	app.run()  

