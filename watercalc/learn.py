import pybrain
import sys
import datetime
import time
import math
import numpy as np
import MySQLdb
from db import DBConfig

class LearningVector:
    def __init__(self, vectorID, deviceID, zoneNumber, ETo, score, feedback, vector):
        if vectorID:
            self.vectorID  = vectorID
        if deviceID:
            self.deviceID  = deviceID
        if zoneNumber:
            self.zoneNumber = zoneNumber
        if ETo:
            self.ETo = ETo
        if score:
            self.score = score
        if feedback:
            self.feedback = feedback
        if vector:
            self.vector = vector

def main():
    while(1):
        #if certain amount of feedback gotten 
        if(1):
            #check the most recently executed irrigation for a given deviceID.
            event = getMostRecentIrrigationEventForDevice(1)

            #grab the learning vector for that irrigation event
            vector = getLearningVector(event[0])

            #assimilate all feedback that has happened since the creation of that learning vector (or since downloading of the schedule directly attributed to that vector?)
            feedback = getFeedbackForLearningVector(vector)

            #score the learning vector... and store it in the database.
            scoreVector(feedback)

            #create a new learning vetctor... pick new parameters via hill climbing
            #store in database
            newVector = createNewLearningVector()

            #predict the new ET prime
            ETp = predictCorrectET(newVector)

            #generate a new schedule for the device
            #and store it in the database
            generateNewSchedule(ETp)

    def generateNewSchedule(ETp):
        print "generate new schedule"

    def predictCorrectET(ETo, alpha, beta, gamma, delta):
        correctET = math.pow(ETo, 2)*alpha + math.pow(ETo, 1)*beta + math.pow(ETo, 0)*gamma + math.pow(ETo, -1)*delta
        return correctET

    def createNewLearningVector():
        print "create new learning vector"

        #get last two vectors
        lastTwo = getLastTwoLearningVectors()
        lv1 = lastTwo[0]
        lv2 = lastTwo[1]

        #compute gradient ascent
        diffVector = np.subtract(lv1.vector, lv2.vector)

        #compute if slope should be positive or negative
        scoreChange = abs(lv1.score) - abs(lv2.score)

        #choose next parameters for learned function
        multiplier = generateGaussianRandom(chooseStdDev(scoreChange, lv1.duration, lv2.duration))

        for i in diffVector:
            gaussDiffVector[i] = diffVector[i]*multiplier

        if scoreChange > 0: #go in direction of whichever score is closer to 0
            newVector = lv1 + gaussDiffVector
        else
            newVector = lv1 - gaussDiffVector

        #create the LearningVector Object
        new = LearningVector()
        new.vector = newVector

        #store it in the database.  when/where do I get ETo?
        sqlString = "INSERT INTO learning COLUMNS (deviceID, zoneNumber, ETo) VALUES (%s, %s, %s)" % (new, new, new)
        vectorID = result

        #also the vectors
        for i in range(0:len(new.vector)):
            sqlString = "INSERT INTO vectors COLUMNS (vectorID, columnNumber, value) VALUES (%s, %s, %s)" % (vectorID, i, new.vector[i - 1])

        #populate it and return it
        return new

    def chooseStdDev(scoreDelta, duration1, duration2):
        #generated std dev from previous scores and durations
        if scoreDelta < 0:





    def generateGaussianRandom(float stdDev):
        #pick a random point from a gaussian with mean 1 and the given standard deviation
        temp = 0
        for i in range(0,12):
            temp += ((2.0 * (float(rand()) / RAND_MAX) - 1.0) * sqrt(stdDev))
        return temp / 2.0

    def getLastTwoLearningVectors():
        conf = DBConfig.DBConfig()
        db = conf.connectToLocalConfigDatabase()
        cursor = db.cursor()
        sqlString = "SELECT * FROM learning ORDER BY vectorID DESC LIMIT 2" % vector.vectorID
        cursor.execute(sqlString)
        lastTwo = cursor.fetchall()
        cursor.close()

        lv1 = getLearningVector(lastTwo[0][0])
        lv2 = getLearningVector(lastTwo[1][0])

        return {lv1, lv2}

    def scoreVector(feedback):
        print "scoring the vector with feedback"

        #scoring function
        summation = 0
        count = len(feedback)
        for value in feedback:
           summation += (1/math.pow(2, count)*abs(value) 
           count -= 1
        score = 20 - summation

        #store in database
        conf = DBConfig.DBConfig()
        db = conf.connectToLocalConfigDatabase()
        cursor = db.cursor()
        sqlString = "UPDATE learning SET score = '%s' WHERE vectorID = '%s' """ % (score, vector.vectorID)
        cursor.execute(sqlString)
        cursor.close()

    def getFeedbackForLearningVector(vector)
        conf = DBConfig.DBConfig()
        db = conf.connectToLocalConfigDatabase()
        cursor = db.cursor()
        sqlString = """SELECT * FROM executedIrrigations WHERE vectorID = '%s' ORDER BY startTime ASC""" % vector.vectorID
        cursor.execute(sqlString)
        earliest = cursor.fetchone()
        sqlString = """SELECT * FROM feedback WHERE created > '%s'""" % earliest[3] #DATETIME
        cursor.execute(sqlString)
        feedback = cursor.fetchall()
        cursor.close()
        return feedback

    def getLearningVector(vectorID):
        conf = DBConfig.DBConfig()
        db = conf.connectToLocalConfigDatabase()
        cursor = db.cursor()
        sqlString = "SELECT * FROM learning WHERE vectorID = '%s' " % vectorID
        cursor.execute(sqlString)
        v = cursor.fetchone()
        sqlString = "SELECT * FROM vectors WHERE vectorID = '%s' ORDER BY columnNumber ASC" % vectorID
        cursor.execute(sqlString)
        vectors = cursor.fetchall()
        cursor.close()

        #create the Learning Vector object
        lv = LearningVector(v[0], v[1], v[2], v[3], v[4])

        #assimilate the vectors from the vectors table to create the list of parameters
        for vector in vectors:
            lv.vector.append(vector[2])
        return lv

    def getMostRecentIrrigationEventForDevice(deviceID):
        conf = DBConfig.DBConfig()
        db = conf.connectToLocalConfigDatabase()
        cursor = db.cursor()
        sqlString = "SELECT * FROM executedIrrigations WHERE deviceID = '%s' ORDER BY startTime DESC" % deviceID
        cursor.execute(sqlString)
        event = cursor.fetchone()
        cursor.close()
        return event

if __name__ == '__main__':
    main()
