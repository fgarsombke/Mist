import sqlite3 as sql
import sys
import os
import logging
import user_score

class ImgDB:
    """Wrapper around the image processing database."""
    
    ImgTableName = 'images'
    ImgIDColName = 'img_id'
    FileNameColName = 'filename'
    AlgoColName = 'algo_'
    NumAlgoCols = 1
    AlgoScoreColName = 'algo_score'
    
    ImgTableCols = ImgIDColName + ' INTEGER PRIMARY KEY AUTOINCREMENT,' + \
                    FileNameColName + ' TEXT NOT NULL UNIQUE, ' + \
                    ''.join([AlgoColName + str(x) + ' REAL, ' for x in range(NumAlgoCols)]) + \
                    AlgoScoreColName + ' REAL '
    
    UserTableName = 'users'
    UserIdColName = 'user_id'
    UsernameColName = 'username'
    ScoreAvgColName = 'score_avg'
    ScoreVarColName = 'score_var'
    LastMsgColName = 'last_msg'
     
    UserTableCols = UserIdColName + ' INTEGER PRIMARY KEY AUTOINCREMENT, ' + \
                    UsernameColName + ' Text NOT NULL UNIQUE, ' + \
                    ScoreAvgColName + ' REAL, ' + \
                    ScoreVarColName + ' REAL, ' + \
                    LastMsgColName + ' TEXT'         
    
    ScoresTableName = 'user_scores'
    ScoreColName = 'score'
    
    ScoresTableCols = UserIdColName + ' INTEGER, ' + \
                        ImgIDColName + ' INTEGER, ' + \
                        ScoreColName + ' REAL, ' + \
        'FOREIGN KEY(' + UserIdColName + ') REFERENCES ' + UserTableName + '(' + UserIdColName + ')' + \
        'FOREIGN KEY(' + ImgIDColName + ') REFERENCES ' + ImgTableName + '(' + ImgIDColName + ')'
    
    ImageExtensions = ['.jpg', '.png', '.bmp', '.jpeg', '.gif']
    
    def get_cursor():    
        return self.con.cursor()
    
    def end_transaction():
        self.con.commit()
    
    def _create_db(self):
        """Creates the project databases."""
        cur = self.con.cursor()
        # Create images table
        crImgTbl = 'CREATE TABLE IF NOT EXISTS ' + self.ImgTableName + '(' + self.ImgTableCols + ')'
        cur.execute(crImgTbl)
        
        # Create users table
        crUsrTbl = 'CREATE TABLE IF NOT EXISTS ' + self.UserTableName + '(' + self.UserTableCols + ')'
        cur.execute(crUsrTbl)
        
        # Create scores table
        crScrTbl = 'CREATE TABLE IF NOT EXISTS ' + self.ScoresTableName + '(' + self.ScoresTableCols + ')'
        cur.execute(crScrTbl)
        
        self.con.commit()
        
    def _add_image(self, filename):
        if filename:
            name, ext = os.path.splitext(filename)
            cur = self.con.cursor()
           
            addImgStr = "INSERT OR IGNORE INTO %s (%s) VALUES ('%s')" % (self.ImgTableName, self.FileNameColName, filename)
            cur.execute(addImgStr)
            
            logging.debug('Added image file %s to DB' %(filename))
        else:
            raise ValueError('filename must be a nonempty string: %s suppled' % (filename))
        
    def load_images(self):
        logging.debug('Loading Images...')
        numLoaded = 0
        for filename in os.listdir(self.image_dir):
            name, ext = os.path.splitext(filename)
            if any(ext in s for s in self.ImageExtensions):
                self._add_image(filename)
                numLoaded += 1
                
        self.con.commit()
        return numLoaded
        
    def add_score(username, score, msg_id):
        """Adds a list of Scores to the db"""
        # Update the score if the message is new
        for s in scores:
            pass # TODO: Implement
                   
    def __enter__(self):
        return self
        
    def __exit__(self, type, value, traceback):
        if self.con:
            self.con.close()
          
    def __init__(self, image_dir, repopulate=True):
        """Creates the db if it does not exist, and populates it with all the images in img_dir"""
        if not image_dir:
            raise ValueError('image_dir cannot be None or empty.')
        self.image_dir = image_dir
        self.con = None
           
        dbPath = os.path.join(self.image_dir, 'data.db')
        
        try:
            self.con = sql.connect(dbPath)  
            cur = self.con.cursor()    
            cur.execute('PRAGMA foreign_keys = ON')
            
        except sql.Error, e:
            print "Error Opening SQLite DB." 
            raise
            
        # Check if the table exists
        try:
            cur.execute('SELECT * FROM ' + ImgTableName)
        except:
            # Create the table
            print('Creating Database')
            self._create_db()
    
        # Add all the images to the table if they aren't already there
        if repopulate:
            n = self.load_images()
            print('%d Images loaded from %s.' % (n,self.image_dir))
             
        