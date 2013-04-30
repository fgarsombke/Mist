class Score:
    """Object Model for user scores"""
    
    username = ''
    score = 0
    msg_id = ''
    
    def __init__(self, username='', score=0, msg_id=''):
        self.username = username
        self.score = score
        self.msg_id = msg_id
