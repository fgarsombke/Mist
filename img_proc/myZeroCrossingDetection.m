function [ result ] = myZeroCrossingDetection ( img )

img_shifted_horz = [ img(:,2:end) , img(:,end) ];
img_shifted_vert = [ img(2:end,:) ; img(end,:) ];

result = zeros ( size ( img ) );
result(((img_shifted_horz.*img)<0)|((img_shifted_vert.*img)<0)) = 1;
result = logical ( result );
