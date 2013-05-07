close all
clear all

%db = ImgDB('img_procDB');

[ratings, filenames] = xlsread('./lawn_ratings.csv');

%ratings = ratings(startImg: endImg);
ratings = mean(ratings,2);

window_scale_min = 5;
window_scale_max = 100;

maxIter = 10;
tol = 0.1;

numSearchImages = 45;

ratings=ratings(1:numSearchImages);
color_metric = zeros ( numel(ratings), 1 ) ;

a = window_scale_min;
b = window_scale_max;
for sc = 1:maxIter 

    for i = 1:maxIter
        c = (a + b)/2;

        for i = 2:numSearchImages ;    
            img = imread(['./lawns/', filenames{i}]);
            img = imresize(img, 0.4);
            hsz = round(sqrt(numel(img)/3)/c);
            sig = hsz/10;
            [color_metric(i)] = compute_metric(img, [ hsz, sig]);
            fprintf(1, 'Done processing image %d.\n', i);
        end
        f_c = sum(color_metric - ratings); 
        
        if( ( f_c == 0 ) || ( (b - a)/2 < tol ) )
            return;
        end       
        
        fprintf(1, 'Computed metric error =%g\n', f_c);
        fprintf(1, 'Computed height scaling c=%g\n', c);
        java.lang.System.gc();
        
        for i = 2:numSearchImages ;    
            img = imread(['./lawns/', filenames{i}]);
            img = imresize(img, 0.4);
            hsz = round(sqrt(numel(img)/3)/a);
            sig = hsz/10;
            [color_metric(i)] = compute_metric( img, [ hsz, sig]);
            fprintf(1, 'Done processing image %d.\n', i);
        end
        f_a = sum(color_metric - ratings); 
         
        if( f_c*f_a > 0 )
            a = c;
        else
            b = c;
        end
    end
    
    disp('Iterations exceeded nmax');
end

    
hsz_best = c;


for sc = 1:numSearchImages 
    for i = 2:numImages ;    
        img = imread(['./lawns/', filenames{i}]);
        img = imresize(img, 0.3);
        hsz = round(sqrt(numel(img)/3)/hsz_best);
        sig = hsz/10;
        [color_metric(i)] = compute_metric( img, [ hsz, sig]);
        fprintf(1, 'Done processing image %d.\n', i);
    end
end


[V, indx] = sort(ratings);
color_metric = color_metric(indx);

cc = corr(color_metric, ratings);
fprintf(1, 'Correlation: %g', corr(color_metric, ratings));

h= figure(i+1);
plot(1:numel(ratings),V);
set(gca, 'XTick', 1:2:numel(ratings))
title(sprintf('Quality Metric.  Corr=%g', cc));
xlabel('Image');
ylabel('Rating');

hold on;
plot(color_metric(indx));