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
        
        function [ids, avgs] = getAverageScores(this)
            cur = exec(this.conn, 'SELECT img_id, AVG(score) FROM user_scores GROUP BY img_id');
            cur = fetch(cur);
            values = cell2mat(cur.data);
            ids = values(:,1);
            avgs = values(:,2);
        end
        
        function [ids, avg, std] = getImageData(this)
            cur = exec(this.conn, 'SELECT img_id, AVG(score) FROM user_scores GROUP BY img_id');
            cur = fetch(cur);
            avg = cell2mat(cur.data);
            
            cur = exec(this.conn, 'SELECT img_id, score FROM user_scores');
            cur = fetch(cur);
            std = cur.data;
        end
        
        function [image_id] = getImageIDs(this)
            cur = exec(this.conn, 'SELECT id FROM images');
            cur = fetch(cur);
            image_id = cell2mat(cur.data);
        end
        
        function [image] = getImageByName(this, img_name)
            cur = exec(this.conn, 'SELECT * FROM metadata WHERE key=''img_dir''');
            cur = fetch(cur);
            fileDir = cur.data(2);
            
            imname = strcat(fileDir, '/', img_name);
            image = imread(cell2mat(imname));
        end
        
        function [image] = getImageById(this, img_id)
            cur = exec(this.conn, sprintf('SELECT filename FROM images WHERE id=%d',img_id));
            cur = fetch(cur);
            fileName = cur.data;
            
            cur = exec(this.conn, 'SELECT * FROM metadata WHERE key=''img_dir''');
            cur = fetch(cur);
            fileDir = cur.data(2);

            imname = strcat(fileDir, '/', fileName);
            image = imread(cell2mat(imname));
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

    

