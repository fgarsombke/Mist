#!/usr/bin/env python
import web
import sys
from api.user import user
from api.user import list
from api.user import logout
from api.device import device
from api.station import station
from api.zone import zone
from api.index import index
from api.schedule import schedule
sys.path.append('./db')
from db import DBConfig

web.config.debug = False

render = web.template.render('templates/')

urls = (
	'/', 'index',
	'/user', 'user',
	'/user/list', 'list',
	'/user/logout', 'logout',
	'/station', 'station',
	'/device', 'device',
	'/zone', 'zone',
	'/schedule', 'schedule'
)

app = web.application(urls, globals())

config = open("/Users/makilian/Mist/local.config")
host = config.readline().rstrip()
username = config.readline().rstrip()
password = config.readline().rstrip()
database = config.readline().rstrip()

db = web.database(dbn='mysql', db=database, user=username, pw=password)
store = web.session.DBStore(db, 'sessions')

if web.config.get('_session') is None:
	session = web.session.Session(app, store, initializer={'count': 3})
	session.count = 4
	web.config._session = session
	print session
else:
	session = web.config._session
	print session

if __name__ == '__main__':
	app.run()  
