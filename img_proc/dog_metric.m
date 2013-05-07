function [metric] = dog_metric(image, hsz, sigma)
    [M, N] = size(image);
    red = double(image(:, :, 1));
    green = image(:, :, 2);

    brown_scale = 80;
    green_scale = 80;
    shift = 0;
    dogf = fspecial('log', hsz, sigma);
    
    %lap = [-1,-1,-1;-1,8,-1;-1,-1,-1];

    brown_filtered = imfilter(uint8(round(double(red)/2+double(green)/2)), dogf);
    green_filtered = imfilter(green, dogf);
    
    sum_nongreen = log(1 + sum(sum(brown_filtered))/(size(image,1)*size(image,2)));   
    sum_green = log(1 + sum(sum(green_filtered))/(size(image,1)*size(image,2)));  
    
    % Blob magnitude is inverted
    metric = brown_scale*sum_nongreen - green_scale*sum_green + shift;
    
    
end