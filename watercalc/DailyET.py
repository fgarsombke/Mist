import pymetar
import sys
import datetime
import time
import math
import MySQLdb
from db import DBConfig
import ComputeET

def computeEToForStation(station):
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    #change from CURDATE to most recent download.  also need to verify all cycles included
    sqlString = """SELECT * FROM hourlyClimateData WHERE climateStationID = '%s' AND hourlyClimateData.date > DATE_SUB(now(), INTERVAL 1 DAY)""" % (station)  
    cursor = db.cursor()
    cursor.execute(sqlString)
    results = cursor.fetchall() 
    cursor.close()

    avgDailyTemp = 0
    avgDailyWind = 0
    avgDailyHumidity = 0
    avgDailyPressure = 0
    avgDailyDew = 0
    maxTemp = -1000
    minTemp = 1000

    humidCount = 0
    tempCount = 0
    pressureCount = 0
    dewCount = 0
    windCount = 0

    #iterate through the last 24 hours of weather readings
    if len(results) > 12:               
        #average each of the last 12 readings
        for result in results:
            #verify that all results exist (NOT NULL)
            if result[2] is not None:
                tempCount += 1
                #update min/max temps
                if result[2] >= maxTemp:
                    maxTemp = result[2]
                if result[2] <= minTemp:
                    minTemp = result[2]
                avgDailyTemp += result[2]
            if result[3] is not None:
                humidCount += 1
                avgDailyHumidity += result[3]

            if result[4] is not None:
                pressureCount += 1
                avgDailyPressure += result[4]

            if result[5] is not None:
                dewCount += 1
                avgDailyDew += result[5]

            if result[6] is not None:
                windCount += 1
                avgDailyWind += result[6]

        #compute averages and then ET (only if we have enough data)
        if humidCount != 0 and tempCount != 0 and pressureCount != 0 and dewCount != 0 and windCount != 0:  
            avgDailyTemp = avgDailyTemp / tempCount
            avgDailyWind = avgDailyWind / windCount
            avgDailyHumidity = avgDailyHumidity / humidCount
            avgDailyPressure = avgDailyPressure / pressureCount
            avgDailyDew = avgDailyDew / dewCount
            isoTime = results[len(results) - 1][8] #Most recent time reading

            #function that computes the ET from the information in the METAR report
            #latitude and altitude for ATX KAUS
            ETresults = ComputeET.computeDailyETo(avgDailyTemp, maxTemp, minTemp, avgDailyWind, avgDailyHumidity, avgDailyPressure, avgDailyDew, isoTime, 30.183, 172) 
            ETresults.append(isoTime)
            return ETresults
        else:
            return (-1, 0)
    else:
        return (-1, 0)

def main():
    #get list of all weather stations zeus database
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT climateStationID FROM climateStations");
    stations = cursor.fetchall()

    #for each weather station 24 hours to compute the Daily ETo, then store in DailyETo database
    for station in stations:
        ETresults = computeEToForStation(station)    
        if ETresults[0] > 0:
            cursor = db.cursor()
            cursor.execute("INSERT INTO dailyETo (climateStationID, ETo, dayOfYear, date) VALUES (%s, %s, %s, %s)", (station[0], ETresults[0], ETresults[1], ETresults[2]))
            cursor.close()  
            db.commit()
        else:
            print "insuffiecnet data to compute ETo"

#Function that prints out useful data from the METAR report using the PyMetar object
def printParsedReport(pr, station):
    
    print "~~~METAR report for %s (%s)~~~" % (pr.getStationName(), station)
    print "Time Data Acqired: %s" % (pr.getISOTime())
    print "Temperature: %s C / %s F" % (pr.getTemperatureCelsius(), pr.getTemperatureFahrenheit())
    print "Rel. Humidity: %s%%" % (pr.getHumidity())

    if pr.getWindSpeed() is not None:
        print "Wind speed: %0.2f m/s (%i Bft)" % (pr.getWindSpeed(), pr.getWindSpeedBeaufort())
    else:
        print "Wind speed: None"
        
    print "Wind direction: %s deg (%s)" % (pr.getWindDirection(), pr.getWindCompass())

    if pr.getPressure() is not None:
        print "Pressure: %s hPa" % (int(pr.getPressure()))
    else:
        print "Pressure: None"
    
    print "Dew Point: %s C / %s F" % (pr.getDewPointCelsius(), pr.getDewPointFahrenheit())
    print "Weather:",pr.getWeather()
    print "Sky Conditions:",pr.getSkyConditions()

if __name__ == '__main__':
    main()  
