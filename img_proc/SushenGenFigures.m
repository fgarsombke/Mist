close all
clear all

image1 = imread('Nathan_29.jpg');
image2 = imread('Sushen_-95.jpg');

[metric1, thresh1] = color_analyzer(image1, .95, 1.1, 0.4);
[metric2, thresh2] = color_analyzer(image2, .95, 1.1, 0.4);


imwrite ( image1, 'nathan_29.jpg' , 'jpg' );
imwrite ( thresh1, 'nathan_29_thresh.jpg' , 'jpg' );

imwrite ( image2 , 'sushen_-95.jpg' , 'jpg' );
imwrite ( thresh2 , 'sushen_-95_thresh.jpg' , 'jpg' );

%Run on first 62 images

[ratings fileNames] = xlsread('lawn_ratings_sushen.csv');

startImg = 2; 
endImg = 63;

%ratings = ratings(startImg: endImg);
ratings = (mean(ratings'));
fileNames = fileNames(startImg:endImg);

color_metric = zeros ( numel(ratings), 1 ) ;

for i = 1 : numel(ratings) ;
    
    img = imread(strcat(fileNames{i}));
    color_metric(i) = color_analyzer(img, 0.95, 1.1, 0.4);
end

[V, indx] = sort(ratings);

h= figure(i+1);
plot(1:numel(ratings),V, 'b');
set(gca, 'XTick', 1:numel(ratings))
title('Sorted Quality Metric');
xlabel('Image');
ylabel('Rating');

hold on;
plot(color_metric(indx), 'b');
saveas ( gcf , 'sorted_all' , 'eps' );

figure;
plot(1:numel(ratings), ratings, 'r');
set(gca, 'XTick', 1:numel(ratings))
title('Unsorted Quality Metric');
xlabel('Image');
ylabel('Rating');

hold on;
plot(color_metric, 'b');
saveas(gcf, 'unsorted_all', 'eps');

