import random
import sys
import datetime
from datetime import date
import time
import math
import numpy as np
import MySQLdb
from db import DBConfig
import ForecastET

class LearningVector:
    def __init__(self, vectorID=0, deviceID=0, zoneNumber=0, ETo=0, score=0, created=0, feedback=[], vector=[]):
        self.vectorID  = vectorID
        self.deviceID  = deviceID
        self.zoneNumber = zoneNumber
        self.ETo = ETo
        self.score = score
        self.created = created
        self.feedback = []
        self.vector = []

    def printLV(self):
        print "VectorID: %s" % self.vectorID
        print "Created: %s" % self.created
        print "DeviceID: %s" % self.deviceID
        print "Zone: %s" % self.zoneNumber
        print "ETo: %s" % self.ETo
        print "Score: %s" % self.score
        print "Feedback: %s" % self.feedback
        print "Vector: %s" % self.vector

#~~~~~~~~~
#CONSTANTS
#~~~~~~~~~
forecastLength = 7

def main():
    #RANDOM DATA FOR TESTING
    deviceID = 1
    zone = 1
    simulatedTime = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    simulatedTime = datetime.datetime.strptime(simulatedTime, "%Y-%m-%d %H:%M:%S")
    print simulatedTime
    doLearning(deviceID, zone, simulatedTime)

def doLearning(deviceID, zone, simulatedTime):
    #determine state
    numIterations = getNumIterations(deviceID, zone)
    if numIterations > 0:
        event = getMostRecentIrrigationEventForDevice(deviceID, zone, simulatedTime)
        vectorID = event[5]
        print "LearningID: %s (For most recent irrigation event for DeviceID: %s)" % (vectorID, deviceID)

        #grab the learning vector for that irrigation event
        vector = getLearningVector(vectorID)
        print "~~Learning Vector~~"
        vector.printLV()

        #assimilate all feedback since the creation of that learning vector (or since downloading of the schedule directly attributed to that vector?)
        vector.feedback = getFeedbackForLearningVector(vector)
        print "Feedback: %s" % str(vector.feedback)

        #update score for the learning vector
        score = scoreVector(vector)
        print "Vector Score: %s" % score

    #create a new learning vector
    #pick new parameters via hill climbing and store in database
    newVector = createNewLearningVector(numIterations, deviceID, zone, simulatedTime)
    print "~~New Generated Vector~~"
    newVector.printLV()

    #predict the new ET prime for the created vector
    #TODO: should this be added to the vector metadata?
    ETp = predictCorrectET(newVector)
    print "Computed ETprime: %s" % ETp

    #generate a new schedule for the device
    #and store it in the database
    #time = current or simulated current time
    generateNewSchedule(deviceID, zone, ETp, forecastLength, simulatedTime, newVector.vectorID)
    print "Generated Schedule"

def getNumIterations(deviceID, zone):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "SELECT COUNT(*) FROM learning WHERE deviceID = %s AND zoneNumber = %s" % (deviceID, zone)
    cursor.execute(sqlString)
    result = cursor.fetchone()[0]
    return result

def getLatLongforDevice(deviceID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "SELECT latitude, longitude FROM devices WHERE deviceID = %s" % deviceID
    cursor.execute(sqlString)
    latlong = cursor.fetchone()
    return latlong

def getEToForecast(deviceID, createdTime):
    latLong = getLatLongforDevice(deviceID)
    print createdTime
    begin = time.mktime(createdTime.timetuple())
    end = time.mktime((createdTime + datetime.timedelta(days=forecastLength)).timetuple())
    value = ForecastET.forecastETo(latLong[0], latLong[1], int(begin), int(end))
    return value

def generateNewSchedule(deviceID, zone, ETp, numDays, timer, vectorID):
    #given an ET value and time period, generate a schedule.
    ETperDay = ETp/numDays
    minutesPerMMofWater = 20 
    minutesPerDay = ETperDay*minutesPerMMofWater
    count = 0
    for i in range(numDays):
        date = timer + datetime.timedelta(i, 0)
        createIrrigationEvent(deviceID, zone, date, minutesPerDay, vectorID)

def createIrrigationEvent(deviceID, zone, timer, duration, vectorID):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "INSERT INTO queuedIrrigations (deviceID, zoneNumber, startTime, duration, vectorID) VALUES (%s, %s, '%s', %s, %s)" % (deviceID, zone, timer, duration, vectorID)
    cursor.execute(sqlString)
    db.commit()

def predictCorrectET(vector):
    #need to decide on the length of the vector and which coefficients are part of the equation
    correctET = 0
    count = 0
    for each in vector.vector:
        correctET += math.pow(vector.ETo, count)*each
        count += 1
    return correctET

def getWeightForScheduleDuration(vector):
    #this is currently totally, linear.  could be improved.
    numberOfDays = getNumDays(vector)
    if numberOfDays < 28:
        return numberOfDays/14.0
    else:
        return 2.0

def computeDescent(vector1, vector2):
    #~~~NEWTON'S METHOD~~~
    diffVector = np.subtract(lv1.vector, lv2.vector)

    #get slope of score
    scoreChange = lv1.score - lv2.score

    #jacobian
    if scoreChange == 0:
       #this can definitely be improved!!!
       newVector = lv1.vector + diffVector/2
    else:
        jacobianApprox = scoreChange/diffVector
        inverseJacobian = 1.0/(jacobianApprox)
        updateJacob = inverseJacobian*lv1.score

        #compute time weighting
        timeConstant = getWeightForScheduleDuration(vector1)

        #next vector
        newVector = lv1.vector + (updateJacob)*timeConstant
        return newVector

#case 0: 1st iteration of algorithm
#case 1: 2nd iteration of algorithm
#case 2: 3rd or greater iteration of algorithm
def createNewLearningVector(iteration, deviceID, zoneNumber, created):
    newVector = []
    #pick parameters for new vector based on what iteration of algorithm it is
    if iteration == 0:
        #initially ETp = 0*ETo^2 + 1*ETo + 0
        newVector = [0.0, 1.0, 0.0]

    #update based on sign of score only
    if iteration == 1:
        last = getLastXLearningVectors(deviceID, 1)
        if last[0].score > 0: #too little water, we are underpredicing ETp
            newVector = [0.1, 1.2, 0.1]
        else: #too much water, we are overpredicting ETp
            newVector = [-.05, .94, -.05]

    #do a descent, no randomness
    if iteration == 2:
        last = getLastXLearningVectors(deviceID, 2)
        lv1 = last[0]
        lv2 = last[1]
        newVector = computeDescent(lv1, lv2)

    #do a descent, with randomness
    if iteration >= 3:
        last = getLastXLearningVectors(deviceID, 3)
        lv1 = last[0]
        lv2 = last[1]
        lv3 = last[2]
        newVector = computeDescent(lv1, lv2)
        scoreChange = lv1.score - lv2.score
        prevScoreChange = lv2.score - lv3.score

        #add randomness to each part of the vector gaussian style
        gaussArray = []
        for each in newVector:
            stdDev = chooseStdDev(each, scoreChange, prevScoreChange)
            gauss = random.gauss(0, stdDev)
            gaussArray.append(gauss)

        newVector = np.add(newVector, gaussArray)

    #create the LearningVector Object
    new = LearningVector()
    new.vector = newVector
    new.ETo = getEToForecast(deviceID, created)
    new.deviceID = deviceID
    new.zoneNumber = zoneNumber
    new.created = created

    #store in database
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "INSERT INTO learning (deviceID, zoneNumber, ETo) VALUES (%s, %s, %s)" % (new.deviceID, new.zoneNumber, new.ETo)
    cursor.execute(sqlString)

    #get the ID
    new.vectorID = cursor.lastrowid
    db.commit()

    #now store the vector in vectors table
    for i in range(len(new.vector)):
        sqlString = "INSERT INTO vectors (vectorID, columnNumber, value) VALUES (%s, %s, %s)" % (new.vectorID, i, new.vector[i - 1])
        cursor.execute(sqlString)
    db.commit()

    #Return the vector
    return new

def chooseStdDev(vectorComponentLength, scoreDelta1, scoreDelta2):
   #choose standard deviation based on the change in score and the length of the component of the difference vector 
   return (1.0/10.0)*vectorComponentLength*math.pow(scoreDelta1 - scoreDelta2, 2)

def getLastXLearningVectors(deviceID, number):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "SELECT * FROM learning WHERE deviceID = %s ORDER BY vectorID DESC LIMIT %s" % (deviceID, number)
    cursor.execute(sqlString)
    last = cursor.fetchall()
    cursor.close()

    count = 0
    lastVectors = []

    #populate each learning vector
    while count < number:
        vector = last[count]
        vID = vector[0]
        lastVectors.append(getLearningVector(vID))
        count += 1

    return lastVectors

def dateToValue(date):
    #TODO: turn a date into a value to go into the scoring function
    return 2.0

def scoreVector(vector):
    #scoring function
    summation = 0
    bSummation = 0
    for each in vector.feedback:
       timeValue = dateToValue(each[1])
       summation += math.pow(timeValue, 1/2)*abs(each[2]) 
       bSummation += math.pow(timeValue, 1/2)
    score = summation/bSummation

    #store score in database
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("UPDATE learning SET score = (%s) WHERE vectorID = (%s)", (score, vector.vectorID))
    cursor.close()
    db.commit()
    return score

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
    lv = LearningVector(v[0], v[1], v[2], v[3], v[4], v[5])

    #assimilate the parameter vector from the vectors table
    for vector in vectors:
        lv.vector.append(vector[2])
    return lv

def getMostRecentIrrigationEventForDevice(deviceID, zone, time):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    sqlString = "SELECT * FROM queuedIrrigations WHERE deviceID = '%s' AND zoneNumber = '%s' AND startTime < '%s' ORDER BY startTime DESC" % (deviceID, zone, time)
    cursor.execute(sqlString)
    event = cursor.fetchone()
    cursor.close()
    return event

if __name__ == '__main__':
    main()
