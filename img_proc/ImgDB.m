classdef ImgDB < handle
    %IMGDB Represents a connection to 
    
    properties (Hidden, SetAccess = private)
        conn;
    end
    
    methods 
        function this = ImgDB(db_name)
            this.conn = database(db_name, '', '');
        end       
        
        function scores = getImageScores(this, img_name)
            cur = exec(this.conn, ['SELECT score FROM user_scores WHERE img_id=(SELECT img_id FROM images WHERE filename=''', img_name, ''')']);
            cur = fetch(cur);
            scores = cell2mat(cur.data);
        end
        
        function [avg, std] = getImageData(this)
            cur = exec(this.conn, 'SELECT img_id, AVG(score) FROM user_scores GROUP BY img_id');
            cur = fetch(cur);
            avg = cur.data;
            
            cur = exec(this.conn, 'SELECT (img_id, score) FROM user_scores');
            cur = fetch(cur);
            std = cur.data;
        end
        
        function [] = storeResultsById(this, img_id, score, params)
            cur = exec(this.conn, ['UPDATE or FAIL images SET algo_score=',num2str(score),' WHERE img_id=,' num2str(img_id)]);
            
            for i = 0:(numel(params)-1)
                cur = exec(this.conn, ['UPDATE or FAIL images SET algo', num2str(i), '=',num2str(params(i+1)),' WHERE img_id=,' num2str(img_id)]);
            end
        end
        
        function [] = storeResultsByName(this, img_name, score, params)
            cur = exec(this.conn, ['UPDATE or FAIL images SET algo_score=',num2str(score),' WHERE filename=,' img_name]);
            
            for i = 0:(numel(params)-1)
                cur = exec(this.conn, ['UPDATE or FAIL images SET algo', num2str(i), '=',num2str(params(i+1)),' WHERE filename=,' img_name]);
            end
        end
        
        function [] = commitChanges(obj)
           commit(obj.conn) 
        end
    end
    
end

