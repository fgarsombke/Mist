function [ result ] = myfscs( image )
%MYFSCS Contrast Stretching

image = double(image);

%Compute the min and max values in the histogram]
grayMax = 255;
minVal = double(min(reshape(image, 1, numel(image))));
maxVal = double(max(reshape(image, 1, numel(image))));

%Compute pointwise linear transform
result = uint8(round((grayMax/(maxVal-minVal))*(double(image) - minVal)));

end

