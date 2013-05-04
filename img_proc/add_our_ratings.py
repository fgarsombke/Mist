import csv
import img_db
from user_score import Score

with open ('./lawn_ratings.csv', 'rb') as f:
    reader = csv.reader(f)
    names = next(reader)[1:]
    
    with img_db.ImgDB('./lawns', -100, 100) as DB:     
        for row in reader:
            img_name = row[0]
            scores = row[1:]
            for i in range(len(scores)):
                if DB.add_score(img_name, Score(names[i], scores[i], 0)):
                    print('Added score %s for image %s'%(scores[i], img_name))
                else:
                    print('Skipped score... Already in DB')
        DB.end_transaction()
