function [ algo_scores ] = getAlgoScore( db_con )
%	GETALGOSCORE Gets all the algo scores for all images.
%
%	algo_scores = GETALGOSCORE( DB_NAME, DB_USER, DB_PASS )   
%		Gets all the algo scores for all images.

	% constants
    ImgTableName = 'images';
	AlgoScoreColName = 'score';

    AlgoScoreQuery = ['SELECT ' AlgoScoreColName ' FROM ' ImgTableName];

    % connect to db
	cur = exec(db_con, AlgoScoreQuery);
	cur = fetch(cur);

	algo_scores = cur.data;