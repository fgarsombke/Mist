function [ metric] = compute_metric(image, params)
%COMPUTE_METRIC Summary of this function goes here
%   Detailed explanation goes here

[metric] = dog_metric(image, params(1), params(2));
end

