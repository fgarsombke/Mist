#!/usr/bin/env python
import web
import sys
from api.user import user
from api.user import list
from api.device import device
from api.station import station
from api.zone import zone
from api.index import index
from api.schedule import schedule
sys.path.append('/Users/makilian/Mist/db')

#web.config.debug = False

render = web.template.render('templates/')

urls = (
	'/', 'index',
	'/user', 'user',
	'/user/list', 'list',
	'/station', 'station',
	'/device', 'device',
	'/zone', 'zone',
	'/schedule', 'schedule'
)

config = open("/Users/makilian/Mist/local.config")
host = config.readline().rstrip()
user = config.readline().rstrip()
password = config.readline().rstrip()
database = config.readline().rstrip()


app = web.application(urls, globals())

db = web.database(dbn='mysql', db=database, user=user, pw=password)
store = web.session.DBStore(db, 'sessions')
session = web.session.Session(app, store, initializer={'count': 0})

if __name__ == '__main__':
	app.run()  
