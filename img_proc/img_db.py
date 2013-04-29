import sqlite3 as sql
import sys


class ImgDB:
    ImgTableName = 'images'
    ImgIDColName = 'img_id'
    FileNameColName = 'filename'
    AlgoColName = 'algo_'
    NumAlgoCols = 1
    AlgoScoreColName = 'algo_score'
    
    ImgTableCols = ImgIDColName + ' INTEGER, ' + FileNameColName + \
        ' TEXT, ' + ''.join([AlgoColName + str(x) + ' REAL, ' for x in range(NumAlgoCols)]) + AlgoScoreColName + ' REAL '
    
    UserTableName = 'users'
    UserIdColName = 'user_id'
    PhoneNumColName = 'phone_num'
    ScoreColName = 'user_score'
    
    UserTableCols = UserIdColName + ' INTEGER, ' + PhoneNumColName + \
        ' TEXT, ' + ScoreColName + ' REAL, ' + ImgIDColName + ' INTEGER, ' + \
        'FOREIGN KEY(' + ImgIDColName + ') REFERENCES ' + ImgTableName + '(' + ImgIDColName + ')'
    
    def get_cursor():
        return self.con.cursor()
    
    def create_db(self):
        """Creates the project databases."""
        cur = self.con.cursor()
        # Create images table
        crImgTbl = 'CREATE TABLE IF NOT EXISTS ' + self.ImgTableName + '(' + self.ImgTableCols + ')'
        cur.execute(crImgTbl)
        
        # Create users table
        crUsrTbl = 'CREATE TABLE IF NOT EXISTS ' + self.UserTableName + '(' + self.UserTableCols + ')'
        cur.execute(crUsrTbl)
        
    def rebuild_db(self):
        """Repopulates the DB using the ."""
        
    def __enter__(self):
        return self
        
    def __exit__(self, type, value, traceback):
        if self.con:
            self.con.close()
        
    """Wrapper around the image processing database."""
    def __init__(self):
        """Creates the db if it does not exist"""
        self.con = None

        try:
            self.con = sql.connect('data.db')  
            cur = self.con.cursor()    
            cur.execute('PRAGMA foreign_keys = ON')
            
            # Check if the table exists
            try:
                cur.execute('SELECT * FROM ' + ImgTableName)
            except:
                # Create the table
                print('Creating Database')
                self.create_db()
                self.rebuild_db()
                self.con.commit()
                
        except sql.Error, e:
            print "Error %s:" % e.args[0]
            sys.exit(1)
            
        