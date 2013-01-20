#!/usr/bin/env python
import web
from api.user import user
from api.device import device
from api.station import station
from api.zone import zone
from api.index import index

#web.config.debug = False

render = web.template.render('templates/')

urls = (
	'/', 'index',
	'/user', 'user',
	'/station', 'station',
	'/device', 'device',
	'/zone', 'zone'
)

app = web.application(urls, globals())

if __name__ == '__main__':
	app.run()  
