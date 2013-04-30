import sqlite3 as sql
import sys
import os

class ImgDB:
    """Wrapper around the image processing database."""
    
    ImgTableName = 'images'
    ImgIDColName = 'img_id'
    FileNameColName = 'filename'
    AlgoColName = 'algo_'
    NumAlgoCols = 1
    AlgoScoreColName = 'algo_score'
    
    ImgTableCols = ImgIDColName + ' INTEGER, PRIMARY KEY' + \
                    FileNameColName + ' TEXT, ' + \
                    ''.join([AlgoColName + str(x) + ' REAL, ' for x in range(NumAlgoCols)]) + \
                    AlgoScoreColName + ' REAL '
    
    UserTableName = 'users'
    UserIdColName = 'user_id'
    ScoreAvgColName = 'score_avg'
    ScoreVarColName = 'score_var'
    LastMsgColName = 'last_msg'
     
    UserTableCols = UserIdColName + ' INTEGER PRIMARY KEY, ' + \
                    ScoreAvgColName + ' REAL, ' + \
                    ScoreVarColName + ' REAL, ' + \
                    LastMsgColName + ' TEXT'         
    
    ScoresTableName = 'user_scores'
    PhoneNumColName = 'phone_num'
    ScoreColName = 'score'
    
    ScoresTableCols = UserIdColName + ' INTEGER, ' + \
                        PhoneNumColName + ' TEXT, ' + \
                        ImgIDColName + ' INTEGER, ' + \
                        ScoreColName + ' REAL, ' + \
        'FOREIGN KEY(' + UserIdColName + ') REFERENCES ' + UserTableName + '(' + UserIdColName + ')' + \
        'FOREIGN KEY(' + ImgIDColName + ') REFERENCES ' + ImgTableName + '(' + ImgIDColName + ')'
    
    ImageExtensions = ['jpg', 'png', 'bmp', 'jpeg', 'gif']
    
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
        
        # Create scores table
        crScrTbl = 'CREATE TABLE IF NOT EXISTS ' + self.ScoresTableName + '(' + self.ScoresTableCols + ')'
        cur.execute(crScrTbl)
        
    def add_image(filename):
        if filename:
            
        else:
            raise ValueError('filename cannot be None or empty')
        
        
    def load_images(image_dir):
        numLoaded = 0
        for filename in os.listdir(image_dir):
            name, ext = os.path.splitext(filename)
            if any(ext in s for s in ImageExtensions):
                add_image(os.path.join(image_dir, filename))
                numLoaded += 1
                
        return numLoaded
        
    def rebuild_db(self):
        """Repopulates the DB using the gvoice account and our csv scores."""
        
    def __enter__(self):
        return self
        
    def __exit__(self, type, value, traceback):
        if self.con:
            self.con.close()
          
    def __init__(self, image_dir, repopulate=True):
        """Creates the db if it does not exist, and populates it with all the images in img_dir"""
        self.con = None

        dbPath = os.path.join(image_dir, 'data.db')
        
        try:
            self.con = sql.connect(dbPath)  
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
        
        # Add all the images to the table if they aren't already there
        if repopulate:
            n = load_images(image_dir)
            print('%d Images loaded from %s.' % (n,image_dir))
        
        self.con.commit()
               
        except sql.Error, e:
            print "Error Opening SQLite DB." 
            raise
            
        