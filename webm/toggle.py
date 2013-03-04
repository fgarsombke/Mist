import web
import sys

class toggle:
	#GET API FOR HOME page
	def GET(self):
		data = web.input()
		render = web.template.render('templates/', base='layout')
		return render.toggle()
