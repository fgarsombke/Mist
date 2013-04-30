function [ score ] = getScore( db_con )
%	GETIMGSCORES Gets the average, normalized score for all images from all users.
%
%	score = GETIMGSCORES( DB_CON )   
%		Gets the average, normalized score for all images from all users.
%
% 	DB_CON ( required )
%		Program will perform queries usings the connection DB_CON.

	% constants
    UserTableName = 'users';
    ScoreColName = 'user_score';

    ScoreQuery = ['SELECT ' ScoreColName ' FROM ' UserTableName];

    % connect to db
	cur = exec(db_con, ScoreQuery);
	cur = fetch(cur);

	% normalize
	score = mean((cur.data - mean(cur.data) ) / ( std(cur.data) ^ 2 ));