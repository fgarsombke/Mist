
close all
db_name = 'img_procDataSource';
db = ImgDB(db_name);



[usr_d, usr_std] = db.getImageData();
usr_d(:,3) = usr_std;


usr_d = sortrows(usr_d,2);
img_ids = usr_d(:,1);

figure;
plot(usr_d(:,2));

algo_results = zeros(numel(img_ids), 1);

parfor i = 1:numel(img_ids)
   image = db.getImageById(img_ids(i));
   
   for i = 30:50
       
       params = [50, 20];
       algo_results(i) = compute_metric(image, params);
   end
   
end

figure;
plot(algo_results);

save('res', 'algo_results_pre')




clear db