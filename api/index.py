import web
import sys

class index:
	#GET API FOR HOME page
	def GET(self):
		data = web.input()
		render = web.template.render('templates/', base='layout')
		return render.index2()
