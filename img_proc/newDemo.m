
close all
db_name = 'img_procDataSource';
db = ImgDB(db_name);


image = db.getImageByName('Nathan_0.jpg');

figure;
imshow(image);
title('Example Lawn Image - Nathan_0.jpg');


[usr_d, usr_std] = db.getImageData();
usr_d(:,3) = usr_std;


usr_d = sortrows(usr_d,2);
img_ids = usr_d(:,1);

figure;
plot(usr_d(:,2));



