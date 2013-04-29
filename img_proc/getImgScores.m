function [ img_scores ] = getImgScores(img_id, varargin )
%	GETIMGSCORES Gets all the scores for an particular img_id with the option to normalize.
%
%	img_scores = GETIMGSCORES( IMG_ID, NORMALIZE, DB_NAME, DB_USER, DB_PASS )   
%		Get all scores for an image with the option to normalize. 
%   
% 	IMG_ID ( required )
%		Get all scores for this IMG_ID.
%
% 	NORMALIZE ( optional )
%		Normalizes the data if true.
%		DEFAULT false
%
% 	DB_NAME ( optional )
%		Program will perform queries in DB_NAME.
%		DEFAULT 'data.db'
%
% 	DB_USER ( optional )
%		Program will perform queries in DB_NAME.
%		DEFAULT ''
%
% 	DB_PASS ( optional )
%		Program will perform queries in DB_NAME.
%		DEFAULT ''

	% option checking
	numargs = length(varargin);  
	if( numargs ) &gt; 4
		error('Requires at most 4 optional arguments');
	end

	% option parsing
	optargs = { false, 'data.db', '', '' };
	optargs( 1:numargs ) = varargin;
	[ normalize, db_name, db_user, db_pass ] = optargs{:};

	% constants
	UserTableName = 'users';
    ImgIDColName = 'img_id'
    ScoreColName = 'user_score';
    ImgScoreQuery = ['SELECT' ScoreColName 'FROM' UserTableName 'WHERE' ImgIDColName '=' img_id]

    % connect to db
	con = database( db_name, db_user, db_pass );
	cur = exec(con, ImgScoreQuery);
	cur = fetch(cur);

	% normalizing option
	if normalize == true
		img_scores = ( cur.data - mean(cur.data) ) / ( std(cur.data) ^ 2 );
	else
		img_scores = cur.data;

	% close out
	close(cur);
	close(con);