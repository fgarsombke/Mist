function [ metric ] = compute_metric(image, params)
%COMPUTE_METRIC Summary of this function goes here
%   Detailed explanation goes here

image = double(image);
metric = sum(abs(image(:,1)-image(:,2))./params(2) > params(1)) + sum(abs(image(:,3)-image(:,2)) < params(2));


%metric = color_analyzer(image, params(1), params(2));
end

