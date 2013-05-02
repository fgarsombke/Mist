function [ metric ] = color_analyzer( image, ideal_green_energy, minimum_green)

ideal_green_energy = 0.40;
minimum_green = 30;

red = image(:, :, 1);
green = image(:, :, 2);
blue = image(:, :, 3);
sum = double(red)+double(green)+double(blue);

average_green = mean(reshape(green, 1, numel(green)));
percent_green = double(green)./double(sum);
percent_green2 = uint32(percent_green*10000);

vector = reshape(percent_green, 1, numel(percent_green));
average_green_percent = mean(uint32(vector*10000))/10000;

stdev = std(std(double(percent_green2)))/10000;


metric = (average_green_percent - ideal_green_energy)/(stdev);

if(average_green < minimum_green)
    metric = -1000                  %image is too dark to analyze or has too little green
end

%warning - if green value is too low, even if the relative energy of the
%green is high the picture is probably very dark and this might not
%accurately assess the quality of the lawn color, so i am setting the
%metric to -1000 to indicate the photo is no good.
end

