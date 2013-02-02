import sys
import MySQLdb
sys.path.append('/Users/makilian/Mist/')
class DBConfig:

	def connectToLocalConfigDatabase(self):
		config = open("/Users/makilian/Mist/local.config")
		host = config.readline().rstrip()
		user = config.readline().rstrip()
		password = config.readline().rstrip()
		database = config.readline().rstrip()
		db = MySQLdb.connect(host=host, user=user, passwd=password, db=database)	
		return db
