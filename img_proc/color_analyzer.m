function [ metric ] = color_analyzer( image, rg_ratio, rb_ratio, bg_ratio)

[M, N, K] = size(image);
red = image(:, :, 1);
green = image(:, :, 2);
blue = image(:, :, 3);

thresh = zeros(M,N);
thresh(red < rg_ratio*green & (blue < rb_ratio*red | blue < bg_ratio*green)) = 1;

thresh = reshape(thresh, M, N);
%filt_thresh = myerosion(mydilate(thresh, 3) , 3);

accept = nnz(thresh)/numel(thresh);

metric = round(accept*200 - 100);

average_green = mean(reshape(green, 1, numel(green)));
if(average_green < 40)
    metric = -100  ;                %image is too dark to analyze
end

end

