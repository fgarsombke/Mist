import csv
import img_db
from user_score import Score

with open ('./lawn_ratings.csv', 'rb') as f:
    reader = csv.reader(f)
    names = next(reader)[1:]
    
    with img_db.ImgDB('./lawns') as DB:     
        for row in reader:
            img_name = row[0]
            scores = row[1:]
            for i in range(len(scores)):
                DB.add_score_by_name(img_name, names[i], scores[i], '0', True)