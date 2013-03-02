import web
import sys

class about:
	#GET API FOR ABOUT page
	def GET(self):
		data = web.input()
		render = web.template.render('templates/', base='layout')
		return render.about()
