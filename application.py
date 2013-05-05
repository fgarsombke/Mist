#!/usr/bin/env python
import web
import sys

from webm.user import user
from webm.user import addUser
from webm.user import list
from webm.user import logout
from webm.device import device
from webm.device import addDevice
from webm.station import station
from webm.station import addStation
from webm.zone import zone
from webm.index import index
from webm.about import about
from webm.schedule import addSchedule
from webm.schedule import schedule
from webm.toggle import toggle
from webm.weather import weather

from api.aUser import aUser
from api.aStation import aStation
from api.aDevice import aDevice
from api.aSchedule import aSchedule
from api.aWeather import aWeather
from api.aToggle import aToggle
from api.aFeedback import aFeedback

from db import DBConfig


if __name__ == '__main__':

    web.config.debug = False

    render = web.template.render('templates/')

    urls = (
        #WEB APP URLs
        '/', 'index',
        '/about', 'about',
        '/user', 'user',
        '/user/add', 'addUser',
        '/user/list', 'list',
        '/user/logout', 'logout',
        '/station/add', 'addStation',
        '/station/list', 'station',
        '/device', 'device',
        '/device/add', 'addDevice',
        '/schedule', 'schedule',
        '/schedule/add', 'addSchedule', 
        '/toggle', 'toggle',
        '/weather', 'weather',

        #API URLs
        '/api/user', 'aUser', 
        '/api/station', 'aStation',
        '/api/device', 'aDevice',
        '/api/schedule', 'aSchedule',
        '/api/weather', 'aWeather',
        '/api/toggle', 'aToggle',
        '/api/feedback', 'aFeedback'
        )

    config = DBConfig.DBConfig()
    app = web.application(urls, globals())
    db = web.database(dbn='mysql', db=config.database, user=config.user, pw=config.password)
    store = web.session.DBStore(db, 'sessions')

    #IS THIS THE RIGHT PLACE TO CREATE THE SESSION? OR SHOULD IT BE IN USER?
    if web.config.get('_session') is None:
        session = web.session.Session(app, store, initializer={'count': 0, 'userID':0, 'deviceID':0})
        session.count = 4
        web.config._session = session
    else:
        session = web.config._session
        
    app.run()
