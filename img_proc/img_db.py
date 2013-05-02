import sqlite3 as sql
import sys
import os
import logging
import user_score

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
LastScoreIDColName = 'score_id'
 
UserTableCols = UserIdColName + ' INTEGER PRIMARY KEY AUTOINCREMENT, ' + \
                UsernameColName + ' Text NOT NULL UNIQUE, ' + \
                LastScoreIDColName + ' TEXT'         

ScoresTableName = 'user_scores'
ScoreColName = 'score'

ScoresTableCols = UserIdColName + ' INTEGER, ' + \
                    ImgIDColName + ' INTEGER, ' + \
                    ScoreColName + ' REAL, ' + \
    'FOREIGN KEY(' + UserIdColName + ') REFERENCES ' + UserTableName + '(' + UserIdColName + ')' + \
    'FOREIGN KEY(' + ImgIDColName + ') REFERENCES ' + ImgTableName + '(' + ImgIDColName + ')'

ImageExtensions = ['.jpg', '.png', '.bmp', '.jpeg', '.gif']

class ImgDB:
    """Wrapper around the image processing database."""
   
    
    def get_cursor(self):    
        return self.con.cursor()
    
    def end_transaction():
        self.con.commit()
    
    def _create_db(self):
        """Creates the project databases."""
        cur = self.get_cursor()
        # Create images table
        crImgTbl = 'CREATE TABLE IF NOT EXISTS ' + ImgTableName + '(' + ImgTableCols + ')'
        cur.execute(crImgTbl)
        
        # Create users table
        crUsrTbl = 'CREATE TABLE IF NOT EXISTS ' + UserTableName + '(' + UserTableCols + ')'
        cur.execute(crUsrTbl)
        
        # Create scores table
        crScrTbl = 'CREATE TABLE IF NOT EXISTS ' + ScoresTableName + '(' + ScoresTableCols + ')'
        cur.execute(crScrTbl)  
        
    def _add_image(self, filename):
        if filename:
            name, ext = os.path.splitext(filename)
            cur = self.get_cursor()
           
            cur.execute("INSERT OR IGNORE INTO %s (%s) VALUES (?)" % (ImgTableName, FileNameColName), [filename])
            
            logging.debug('Added image file %s to DB' %(filename))
        else:
            raise ValueError('filename must be a nonempty string: %s suppled' % (filename))
        
    def _load_images(self):
        logging.debug('Loading Images...')
        numLoaded = 0
        for filename in os.listdir(self.image_dir):
            name, ext = os.path.splitext(filename)
            if any(ext in s for s in ImageExtensions):
                self._add_image(filename)
                numLoaded += 1
                
        return numLoaded
    
    def _get_img_id(self, filename):
        """Gets the image id of the image with the specified filename"""
        
    def add_score_by_id(self, img_id, username, score_id=None, overwrite=False):
        """Adds a score to the db at the specified score id. Uses image file name.
            If overwrite=False, then the score is added to the db only if no current score for that user has id score_id
            If overwrite=True, then the score is added to the db regardless of the value of score_id.
        """    
        if not username:
            raise ValueError('username must be a nonempty string: %s suppled' % (username)) 
        
                                                            
    def add_score_by_name(self, img_filename, username, score, score_id=None, overwrite=False):
        """Adds a score to the db at the specified message id. Uses image file name.
            If overwrite=False, then the score is added to the db only if no current score for that user has id score_id
            If overwrite=True, then the score is added to the db regardless of the value of score_id.
        """       
        cur = self.get_cursor()
        cur.execute("SELECT %s FROM %s WHERE %s=?"%(ImgIDColName,ImgTableName, FileNameColName), [img_filename])
        r = cur.fetchone()
        return self.add_score_by_id(r, username, score, score_id)
               
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
            n = self._load_images()
            print('%d Images loaded from %s.' % (n,self.image_dir))
             
        