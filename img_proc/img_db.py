import sqlite3 as sql
import os
import getpass
import logging
import user_score

ImgTableName = 'images'
AlgoColName = 'algo_'
NumAlgoCols = 1
AlgoScoreColName = 'algo_score'

ImgTableCols = 'id INTEGER PRIMARY KEY AUTOINCREMENT,' + \
                'filename TEXT NOT NULL UNIQUE, ' + \
                ''.join([AlgoColName + str(x) + ' REAL, ' for x in range(NumAlgoCols)]) + \
                AlgoScoreColName + ' REAL '

UserTableName = 'users'
UsernameColName = 'username'
LastScoreIDColName = 'last_score_id'
 
UserTableCols = 'id INTEGER PRIMARY KEY AUTOINCREMENT, ' + \
                UsernameColName + ' Text NOT NULL UNIQUE, ' + \
                LastScoreIDColName + ' TEXT'         

ScoresTableName = 'user_scores'

ScoresTableCols =   'id INTEGER PRIMARY KEY AUTOINCREMENT,' + \
                    'user_id INTEGER, ' + \
                    'img_id INTEGER, ' + \
                    'score REAL, ' + \
    'FOREIGN KEY(user_id) REFERENCES ' + UserTableName + '(id)' + \
    'FOREIGN KEY(img_id) REFERENCES ' + ImgTableName + '(id)'

ImageExtensions = ['.jpg', '.png', '.bmp', '.jpeg', '.gif']

class ImgDB:
    """Wrapper around the image processing database."""
   
    
    def get_cursor(self):    
        return self.con.cursor()
    
    def end_transaction(self):
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
           
            cur.execute("INSERT OR IGNORE INTO %s (filename) VALUES (?)" %(ImgTableName), [filename])
            
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
                                                                   
    def add_score(self, img_filename, user_score, overwrite=False):
        """Adds a score to the db at the specified message id. Uses image file name.
            If overwrite=False, then the score is added to the db only if no current score for that user has id score_id
            If overwrite=True, then the score is added to the db regardless of the value of score_id.
        """       
        username = user_score.username
        score = user_score.score
        score_id = user_score.score_id
        
        cur = self.get_cursor() 
        
        # First ensure the user is in the db
        # This will give him a NULL last score if just added
        cur.execute("INSERT or IGNORE INTO %s (%s) VALUES (?)"%(UserTableName, UsernameColName), [username])
        
        if overwrite:
            ins_action = 'REPLACE'
        else:
            ins_action = 'ABORT'
        
        qargs = [];
        action_q = "INSERT or %s INTO %s (user_id, img_id, score) "%(ins_action, ScoresTableName)
        
        select_usrID = "SELECT id FROM %s WHERE username=?"%(UserTableName);
        qargs.append(username)
        
        select_imgID = "SELECT id FROM %s WHERE filename=?"%(ImgTableName);
        qargs.append(img_filename)
        
        value_q = "VALUES ((%s),(%s), ?)"%(select_usrID, select_imgID)
        qargs.append(score)
        
        query = action_q + value_q;
        
        print("query:")
        print(query)
        print('\n\n')
        cur.execute(query, qargs)
               
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
           
        dbPath = os.path.join(self.image_dir, 'data_%s.db'%(getpass.getuser()))
        
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
             
        self.con.commit()