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
            if event:
                identifier = event[0]
                #grab the learning vector for that irrigation event
                vector = getLearningVector(identifier)

                #assimilate all feedback that has happened since the creation of that learning vector (or since downloading of the schedule directly attributed to that vector?)
                feedback = getFeedbackForLearningVector(vector)

                #score the learning vector... and store it in the database.
                scoreVector(feedback, vector)

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
    #given an ET value, generate a schedule.
    #the first question to answer is what time period is this ET value calculated for?
    #let's assume the ET value is just for today.
    #well, we need a function that translates ET into minutes of watering.


def predictCorrectET(ETo, vector):
    #need to decide on the length of the vector and which coefficients are part of the equation
    correctET = 0
    count = 0
    for each in vector:
        correctET += math.pow(ETo, count)*each
        count += 1
    return correctET

def createNewLearningVector():
    print "create new learning vector"

    #get the last X learning vectors
    last = getLastXLearningVectors(3)
    lv1 = last[0]
    lv2 = last[1]
    lv3 = last[2]

    #~~~NEWTON'S METHOD~~~
    diffVector = np.subtract(lv1.vector, lv2.vector)

    #get slope of score
    scoreChange = lv1.score - lv2.score
    previousScoreChange = lv2.score - lv3.score
    #jacobian
    jacobianApprox = scoreChange/diffVector
    inverseJacobian = 1/(jacobianApprox)
    updateJacob = inverseJacobian*lv1.score

    #compute time constant TODO
    timeConstant = 1.5*scheduleDuration

    #next vector
    newVector = lv1.vector + (updateJacob)*timeConstant

    #add randomness to each part of the vector gaussian style
    for each in newVector:
        gauss = random.gauss(0, chooseStdDev(each, scoreChange, prevScoreChange))
        each += gauss

    #create the LearningVector Object
    new = LearningVector()
    new.vector = newVector

    #store it in the database.  #TODO: ETo?
    sqlString = "INSERT INTO learning COLUMNS (deviceID, zoneNumber, ETo) VALUES (%s, %s, %s)" % (new, new, new)
    vectorID = result

    #also store the vectors
    for i in range(len(new.vector)):
        sqlString = "INSERT INTO vectors COLUMNS (vectorID, columnNumber, value) VALUES (%s, %s, %s)" % (vectorID, i, new.vector[i - 1])

    #populate it and return it
    return new

def chooseStdDev(vectorComponentLength, scoreDelta1, scoreDelta2):
   #choose standard deviation based on the change in score and the length of the component of the difference vector 
   return 1/10*vectorComponentLength*math.pow(scoreDelta1 - scoreDelta2, 2)

def getLastXLearningVectors(number):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "SELECT * FROM learning ORDER BY vectorID DESC LIMIT %s" % number
    cursor.execute(sqlString)
    last = cursor.fetchall()
    cursor.close()

    count = 0
    lastVectors = []
    while count < number:
        lastVectors[count] = getLearningVector(last[count][0])
        count += 1

    return lastVectors

def scoreVector(feedback, vector):
    print "scoring the vector with feedback"

    #scoring function
    summation = 0
    bSummation = 0
    for each in feedback:
       summation += math.pow(each.time, 1/2)*abs(each.value) 
       bSummation += math.pow(each.time, 1/2)
    score = summation/bSummation

    #store in database
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "UPDATE learning SET score = '%s' WHERE vectorID = '%s' """ % (score, vector.vectorID)
    cursor.execute(sqlString)
    cursor.close()

def getFeedbackForLearningVector(vector):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = """SELECT * FROM executedIrrigations WHERE vectorID = '%s' ORDER BY startTime ASC""" % vector.vectorID
    cursor.execute(sqlString)
    earliest = cursor.fetchone()
    sqlString = """SELECT * FROM feedback WHERE deviceID = '%s' AND created > '%s'""" % earlist[1], earliest[3] #DATETIME
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
    sqlString = "SELECT * FROM queuedIrrigations WHERE productID = '%s' ORDER BY startTime DESC" % deviceID
    cursor.execute(sqlString)
    event = cursor.fetchone()
    cursor.close()
    return event

if __name__ == '__main__':
    main()
