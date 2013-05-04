class Score:
    """Object Model for user scores"""
    
    username=''
    score = 0
    score_id = ''
    
    def __init__(self, username, score=0, score_id=''):
        if not username:
            raise ValueError('Username cannot be empty.')
        
        try:
            float(score)
        except:
            raise ValueError("Score must be a number: '%s' given"%(score))
    
        self.username = username
        self.score = score
        self.score_id = score_id
