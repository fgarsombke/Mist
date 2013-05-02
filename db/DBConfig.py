import sys
import os
import MySQLdb
class DBConfig:

    def __init__(self):
        abspath = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname("local.config")))
        config = open(abspath + "/local.config")
        self.host = config.readline().rstrip()
        self.user = config.readline().rstrip()
        self.password = config.readline().rstrip()
        self.database = config.readline().rstrip()

    def connectToLocalConfigDatabase(self):
        abspath = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname("local.config")))
        config = open(abspath + "/local.config")
        host = config.readline().rstrip()
        user = config.readline().rstrip()
        password = config.readline().rstrip()
        database = config.readline().rstrip()
        db = MySQLdb.connect(host=host, user=user, passwd=password, db=database)    
        return db

    def executeLastRowID(self, query):
        db = self.connectToLocalConfigDatabase()
        cursor = db.cursor()
        cursor.execute(query)
        db.commit()
        return cursor.lastrowid

    def executeFetchOne(self, query):
        db = self.connectToLocalConfigDatabase()
        cursor = db.cursor()
        cursor.execute(query)
        db.commit()
        return cursor.fetchone()

    def executeFetchAll(self, query):
        db = self.connectToLocalConfigDatabase()
        cursor = db.cursor()
        cursor.execute(query)
        db.commit()
        return cursor.fetchall()
