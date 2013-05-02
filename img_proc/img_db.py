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
                'algo_score REAL '

UserTableName = 'users'
 
UserTableCols = 'id INTEGER PRIMARY KEY AUTOINCREMENT, ' + \
                'username Text NOT NULL UNIQUE'       

ScoresTableName = 'user_scores'

ScoresTableCols =   'id INTEGER PRIMARY KEY AUTOINCREMENT, ' + \
                    'score_id TEXT, ' +\
                    'user_id INTEGER, ' + \
                    'img_id INTEGER, ' + \
                    'score REAL, ' + \
    'FOREIGN KEY(user_id) REFERENCES ' + UserTableName + '(id)' + \
    'FOREIGN KEY(img_id) REFERENCES ' + ImgTableName + '(id)'

ImageExtensions = ['.jpg', '.png', '.bmp', '.jpeg', '.gif']

class ImgDB:
    """Wrapper around the image processing database."""
   
    def end_transaction(self):
        self.con.commit()
    
    def _create_db(self, minscore, maxscore):
        """Creates the project databases."""

        # Create the table
        if (minscore is None or maxscore is None):
            raise ValueError('minscore and maxscore must be provided for first time DB creation')
        
        logging.debug('Creating Database')
        # http://stackoverflow.com/questions/525512/drop-all-tables-command
        self.con.execute("PRAGMA writable_schema = 1")
        self.con.execute("delete from sqlite_master where type = 'table'")
        self.con.execute("PRAGMA writable_schema = 0")
        self.con.execute("VACUUM")
        
        self.minscore = float(minscore)
        self.maxscore = float(maxscore)
        
        # Create images table
        crImgTbl = 'CREATE TABLE IF NOT EXISTS ' + ImgTableName + '(' + ImgTableCols + ')'
        self.con.execute(crImgTbl)
        
        # Create metadate table
        self.con.execute('CREATE TABLE metadata (key TEXT NOT NULL UNIQUE, val TEXT)')
        self.con.execute("INSERT INTO metadata (key, val) VALUES ('minscore', %s)"%(str(self.minscore)))
        self.con.execute("INSERT INTO metadata (key, val) VALUES ('maxscore', %s)"%(str(self.maxscore)))
        self.con.execute("REPLACE INTO metadata (key, val) VALUES ('img_dir', '%s')"%(self.image_dir))
        
        # Create users table
        crUsrTbl = 'CREATE TABLE IF NOT EXISTS ' + UserTableName + '(' + UserTableCols + ')'
        self.con.execute(crUsrTbl)
        
        # Create scores table
        crScrTbl = 'CREATE TABLE ' + ScoresTableName + '(' + ScoresTableCols + ')'
        self.con.execute(crScrTbl)  
        logging.debug('Database Created Successfully')
        
    def _add_image(self, filename):
        if filename:
            name, ext = os.path.splitext(filename)
           
            self.con.execute("INSERT OR IGNORE INTO %s (filename) VALUES (?)" %(ImgTableName), [filename])
            
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
              
    def contains_image(self, filename):
        try:
            self.con.execute("SELECT id FROM images WHERE filename=?", [filename])
            return True
        except:
            return False
              
    def add_score(self, img_filename, user_score, ignore_id=False):
        """Adds a score to the db at the specified message id. Uses image file name.
            If overwrite=False, then the score is added to the db only if no current score for that user has id score_id
            If overwrite=True, then the score is added to the db regardless of the val of score_id.
        """       
        username = user_score.username
        score = user_score.score
        score_id = user_score.score_id
                
        logging.debug('Adding score=%s, img=%s, user=%s to DB'%(score, img_filename, username))
                
        # First ensure the user is in the db
        self.con.execute("INSERT or IGNORE INTO %s (username) VALUES (?)"%(UserTableName), [username])
        self.end_transaction()
            
        select_usrID = "SELECT id FROM %s WHERE username=?"%(UserTableName);
        select_imgID = "SELECT id FROM %s WHERE filename=?"%(ImgTableName);

        
        # Next we update the score
        if not ignore_id:
            # If we're not overwriting, then we need add update the score only when the score is new
            cur = self.con.cursor()
            cur.execute("SELECT id FROM %s WHERE score_id=? AND user_id=(%s) AND img_id=(%s)"%(ScoresTableName, select_usrID, select_imgID), [score_id, username, img_filename])
            val = cur.fetchone()
                        
            if val is not None:
                # This score has already been applied
                # do nothing
                logging.debug('Score already in the database.  Doing nothing.')
                return False

        qargs = [];
        action_q = "INSERT INTO %s (user_id, img_id, score_id, score) "%(ScoresTableName)
        
        
        qargs.append(username)
        qargs.append(img_filename)
        
        value_q = "VALUES ((%s),(%s), ?, ?)"%(select_usrID, select_imgID)
        qargs.append(score_id)
        qargs.append(score)
        
        query = action_q + value_q;
        self.con.execute(query, qargs)
        
        logging.debug('Score with ID %s added to the database.'%(score_id))
        return True
               
    def __enter__(self):
        return self
        
    def __exit__(self, type, val, traceback):
        if self.con:
            self.con.close()
          
    def __init__(self, image_dir, minscore=None, maxscore=None, repopulate=True):
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
            
            # Turn of synchronous storage.  This is a short running script, not
            #   an always-on server.
            cur.execute('PRAGMA synchronous=OFF')
            self.con.commit()
            
        except sql.Error, e:
            print "Error Opening SQLite DB." 
            raise
            
        # Check if the table exists
        try:
            cur.execute("SELECT * FROM metadata WHERE key='minscore'")
            self.minscore = float(cur.fetchone()[1])
            cur.execute("SELECT * FROM metadata WHERE key='maxscore'")
            self.maxscore = float(cur.fetchone()[1])
            
            if (self.minscore is None or self.maxscore is None):
                self._create_db(minscore, maxscore)
            else:
                if (not (minscore is None) or not (maxscore is None)):
                    print('WARNING: minscore and maxscore values ignored.  Remove the DB to reset parameters.')
        except:
            logging.debug("Couldn't read parameters from db.  Creating new.")
            self._create_db(minscore, maxscore)
    
        # Add all the images to the table if they aren't already there
        if repopulate:
            n = self._load_images()
            print('%d Images loaded from %s.\n' % (n,self.image_dir))
             
        self.end_transaction()