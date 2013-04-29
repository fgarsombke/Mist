function [ score ] = getScore( varargin )
%	GETIMGSCORES Gets the average, normalized score for all images from all users.
%
%	score = GETIMGSCORES( DB_NAME, DB_USER, DB_PASS )   
%		Gets the average, normalized score for all images from all users.
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
	if( numargs ) &gt; 3
		error('Requires at most 3 optional arguments');
	end

	% option parsing
	optargs = { 'data.db', '', '' };
	optargs( 1:numargs ) = varargin;
	[ db_name, db_user, db_pass ] = optargs{:};

	% constants
    UserTableName = 'users'
    ScoreColName = 'user_score'

    ScoreQuery = ['SELECT' ScoreColName 'FROM' UserTableName]

    % connect to db
	con = database( db_name, db_user, db_pass );
	cur = exec(con, ScoreQuery);
	cur = fetch(cur);

	% normalize
	score = mean((cur.data - mean(cur.data) ) / ( std(cur.data) ^ 2 ));

	% close out
	close(cur);
	close(con);