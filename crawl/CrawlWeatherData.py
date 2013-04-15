#!/usr/bin/env python
import pymetar
import sys
import MySQLdb
import datetime
import time
from db import DBConfig

def main():
    conf = DBConfig.DBConfig()
    db = conf.connectToLocalConfigDatabase()
    cursor = db.cursor()
    cursor.execute("SELECT climateStationID FROM climateStations")
    stations = cursor.fetchall()    
    cursor.close()
    
    for station in stations:
        print station[0]
        try:
            rFetch = pymetar.ReportFetcher(station[0])
            rep = rFetch.FetchReport()
            rParse = pymetar.ReportParser()
            pr = rParse.ParseReport(rep)        
            insertParsedReportIntoDatabase(db, station[0], pr)
        except Exception, e:
            sys.stderr.write("Something went wrong when fetching the report \n")
            sys.stderr.write("These usually are transient problems if the station  ")
            sys.stderr.write("ID is valid. \nThe error encountered was:\n")
            sys.stderr.write(str(e)+"\n")


def insertParsedReportIntoDatabase(db, station, parsedReport):
    cursor = db.cursor()
    dateObject = datetime.datetime(*(time.strptime(parsedReport.getISOTime(), "%Y-%m-%d %H:%M:%SZ")[0:6]))  
    cursor.execute("""INSERT INTO hourlyClimateData (climateStationID, airTemp, humidity, pressure, dewpointTemp, windspeed, cycle, date) VALUES (%s, %s, %s, %s, %s, %s, %s, %s)""", (station, parsedReport.getTemperatureCelsius(), parsedReport.getHumidity(), parsedReport.getPressure(), parsedReport.getDewPointCelsius(), parsedReport.getWindSpeed(), parsedReport.getCycle(), dateObject))
    results = cursor.fetchone()
    db.commit()

if __name__ == '__main__':
    main()
