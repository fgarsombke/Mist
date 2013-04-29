function [ algo_scores ] = getAlgoScore( varargin )
%	GETALGOSCORE Gets all the algo scores for all images.
%
%	algo_scores = GETALGOSCORE( DB_NAME, DB_USER, DB_PASS )   
%		Gets all the algo scores for all images.
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
    ImgTableName = 'images'
	AlgoScoreColName = 'algo_score'

    AlgoScoreQuery = ['SELECT' AlgoScoreColName 'FROM' ImgTableName]

    % connect to db
	con = database( db_name, db_user, db_pass );
	cur = exec(con, AlgoScoreQuery);
	cur = fetch(cur);

	algo_scores = cur.data;

	% close out
	close(cur);
	close(con);