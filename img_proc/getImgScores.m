function [ img_scores ] = getImgScores(img_id, db_con, varargin )
%	GETIMGSCORES Gets all the scores for an particular img_id with the option to normalize.
%
%	img_scores = GETIMGSCORES( IMG_ID, NORMALIZE, DB_NAME, DB_USER, DB_PASS )   
%		Get all scores for an image with the option to normalize. 
%   
% 	IMG_ID ( required )
%		Get all scores for this IMG_ID.
%
% 	DB_CON ( required )
%		Program will perform queries in DB_NAME.
%
% 	NORMALIZE ( optional )
%		Normalizes the data if true.
%		DEFAULT false

	% option checking
	numargs = length(varargin);  
	if( numargs > 1)
		error('Requires at most 1 optional arguments');
	end

	% option parsing
	optargs = { false }; % default
	optargs( 1:numargs ) = varargin;
	[ normalize ] = optargs{:};

	% constants
	UserTableName = 'users';
    ImgIDColName = 'id';
    ScoreColName = 'score';
    ImgScoreQuery = ['SELECT ' ScoreColName ' FROM ' UserTableName ' WHERE ' ImgIDColName ' = ' num2str(img_id)];

    % connect to db
	cur = exec(db_con, ImgScoreQuery);
	cur = fetch(cur);

	% normalizing option
	if normalize == true
		img_scores = ( cur.data - mean(cur.data) ) / ( std(cur.data) ^ 2 );
    else
		img_scores = cur.data;
    end