function storeScores( db_con, img_id, varargin )
%	STORESCORES Stores parameters and scores back into the image table of the database.
%
%	storeScores( DB_CON, IMG_ID, ALGO_0, ..., ALGO_N, ALGO_SCORE )   
%		Get all scores for an image with the option to normalize. 
%
% 	DB_CON ( required )
%		Program will perform queries in DB_NAME.
%
% 	IMG_ID ( required )
%		Get all scores for this IMG_ID.
%
% 	Algo ( required )
%		Get all scores for this IMG_ID.


	% option checking
	numargs = length(varargin);  

	% option parsing
	algo_n( 1:numargs ) = varargin;

	ImgTableName = 'images';
    ImgIDColName = 'img_id';
    AlgoColName = 'algo_';
    AlgoScoreColName = 'algo_score';

	AlgoStoreQuery = ['UPDATE ' ImgTableName ' SET'];
    for i = 1:numargs-1
        AlgoStoreQuery = [ AlgoStoreQuery ' ' AlgoColName num2str(i-1) '=' num2str(algo_n{i}) ', '];
   	end
   	AlgoStoreQuery = [ AlgoStoreQuery ' ' AlgoScoreColName '=' num2str(algo_n{numargs}) ' WHERE ' ImgIDColName '=' num2str(img_id)];
    
    cur = exec(db_con, AlgoStoreQuery);