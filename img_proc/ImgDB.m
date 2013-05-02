classdef ImgDB
    %IMGDB Represents a connection to 
    
    properties (Hidden, SetAccess = private)
        conn;
        ImgTbl = 'images'
        UserTbl= 'users';
        ScoresTbl = 'user_scores'
    end
    
    methods 
        function db_obj = ImgDB(db_name)
            this.conn = database(db_name, '', '');
        end
        
        
        function scores = getImageScores(obj, img_name)
            obj.conn
            cur = exec(obj.conn, ['SELECT score FROM user_scores INNER JOIN images USING (img_id) WHERE filename=', genvarname(img_name)]);
            cur = fetch(cur);
            scores = cur.data;
        end
    end
    
end

