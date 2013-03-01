#This script reads in the SQL from the ./zeus.sql file and creates the tables described in the file in the zeus database.
import re
import sys
import MySQLdb
import DBConfig

def main():
	sql = open("zeus.sql").read()
	sql = re.split('\n\s*\n', sql)	
	print len(sql)

	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	
	for command in sql:
		print command
		cursor = db.cursor()	
		cursor.execute(command)
		cursor.close()


if __name__ == '__main__':
	main()
