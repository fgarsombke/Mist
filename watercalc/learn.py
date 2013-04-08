import random
import sys
import datetime
import time
import math
import numpy as np
import MySQLdb
from db import DBConfig

class LearningVector:
    def __init__(self, vectorID=0, deviceID=0, zoneNumber=0, ETo=0, score=0, feedback=[], vector=[]): #the default array values don't work b/c of how python figures these out at run time and only does so once....
        self.vectorID  = vectorID
        self.deviceID  = deviceID
        self.zoneNumber = zoneNumber
        self.ETo = ETo
        self.score = score
        self.feedback = [] 
        self.vector = []

    def printLV(self):
        print "VectorID: %s" % self.vectorID
        print "DeviceID: %s" % self.deviceID
        print "Zone: %s" % self.zoneNumber
        print "ETo: %s" % self.ETo
        print "score: %s" % self.score
        print "feedback: %s" % self.feedback
        print "vector: %s" % self.vector

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
    #given an ET value, generate a schedule.
    #the first question to answer is what time period is this ET value calculated for?
    #let's assume the ET value is just for today.
    #well, we need a function that translates ET into minutes of watering.
    return 0    

def predictCorrectET(ETo, vector):
    #need to decide on the length of the vector and which coefficients are part of the equation
    correctET = 0
    count = 0
    for each in vector:
        correctET += math.pow(ETo, count)*each
        count += 1
    return correctET

def createNewLearningVector():
    #get the last X learning vectors
    last = getLastXLearningVectors(3)
    lv1 = last[0]
    lv2 = last[1]
    lv3 = last[2]

    #~~~NEWTON'S METHOD~~~
    diffVector = np.subtract(lv1.vector, lv2.vector)
    print diffVector
    #diff vector is currently 0's

    #get slope of score
    scoreChange = lv1.score - lv2.score
    prevScoreChange = lv2.score - lv3.score
    print scoreChange
    print prevScoreChange

    #jacobian
    #scoreChange is 0 so the next two lines cause a divide by zero error!!!!! Almost fixed.  Data problem.
    jacobianApprox = scoreChange/diffVector
    print jacobianApprox
    inverseJacobian = 1/(jacobianApprox)
    print inverseJacobian
    updateJacob = inverseJacobian*lv1.score
    print updateJacob

    #compute time constant TODO
    scheduleDuration = 5
    timeConstant = 1.5*scheduleDuration

    #next vector
    newVector = lv1.vector + (updateJacob)*timeConstant

    #add randomness to each part of the vector gaussian style
    for each in newVector:
        gauss = random.gauss(0, chooseStdDev(each, scoreChange, prevScoreChange))
        each += gauss

    #the guess for the next set of parameters.  print it

    #create the LearningVector Object
    new = LearningVector()
    new.vector = newVector

    #store it in the database.  #TODO: ETo?
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "INSERT INTO learning (deviceID, zoneNumber, ETo) VALUES (%s, %s, %s)" % (lv1.deviceID, lv1.zoneNumber, 6.0)
    cursor.execute(sqlString)
    result = cursor.lastrowid
    db.commit()
    vectorID = result

    #also store the vectors
    for i in range(len(new.vector)):
        sqlString = "INSERT INTO vectors (vectorID, columnNumber, value) VALUES (%s, %s, %s)" % (vectorID, i, new.vector[i - 1])
        cursor.execute(sqlString)

    db.commit()

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
    #populate each vector?
    while count < number:
        vector = last[count]
        vID = vector[0]
        lastVectors.append(getLearningVector(vID))
        count += 1

    return lastVectors

def dateToValue(date):
    #turn a date into a value to go into the scoring function
    return 2.0

def scoreVector(feedback, vector):
    vector.printLV()
    #scoring function
    summation = 0
    bSummation = 0
    for each in feedback:
       timeValue = dateToValue(each[1])
       summation += math.pow(timeValue, 1/2)*abs(each[2]) 
       bSummation += math.pow(timeValue, 1/2)
    score = summation/bSummation
    #store in database
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    #WHY ISNT THIS LINE WORKING???
    cursor.execute("UPDATE learning SET score = (%s) WHERE vectorID = (%s)", (score, vector.vectorID))
    db.commit()
    cursor.close()

def getFeedbackForLearningVector(vector):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = """SELECT * FROM queuedIrrigations WHERE vectorID = '%s' ORDER BY startTime ASC""" % vector.vectorID
    cursor.execute(sqlString)
    earliest = cursor.fetchone()
    sqlString = """SELECT * FROM feedback WHERE deviceID = '%s' AND created > '%s'""" % (earliest[1], earliest[2])
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

    #create the Learning Vector object (no feedback or vectors yet)
    print v
    lv = LearningVector(v[0], v[1], v[2], v[3], v[4])
    
    # WHY IS THIS lv VECTOR POPULATED WITH a vector with vector ID = 1?? initially 
    lv.printLV()
    #assimilate the vectors from the vectors table to create the list of parameters
    for vector in vectors:
        lv.vector.append(vector[2])
    lv.printLV()
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
