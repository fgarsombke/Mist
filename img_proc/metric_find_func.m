function [ v ] = metric_find_func( d_in, c )
%METRIC_FIND_FUNC Summary of this function goes here
%   Detailed explanation goes here
numSearchImages = 60;

    for i = 2:numSearchImages ;    
        img = imread(['./lawns/', d_in{1}]);
        img = imresize(img, 0.3);
        hsz = round(sqrt(numel(img)/3)/c);
        sig = hsz/10;
        [v, proc_data] = compute_metric(proc_data, img, [ hsz, sig]);
        fprintf(1, 'Done processing image %d.\n', i);
    end

end

