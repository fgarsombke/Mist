import pymetar
import sys
import datetime
import time
import math
import MySQLdb
sys.path.append('/Users/makilian/Dropbox/Mist/Mist/Software/Backend/ZeusET/db')
import DBConfig

def main():
	
	#get list of all weather stations zeus database
	conf = DBConfig.DBConfig()
	db = conf.connectToLocalConfigDatabase()
	cursor = db.cursor()
	cursor.execute("SELECT climateStationID FROM climateStations");
	stations = cursor.fetchall()

	#for each weather station use the data from the last 24 hours to compute the Daily ETo, then store in DailyETo database
	for station in stations:
		print "Computing DailyETo for %s " % station[0]
		sqlString = """SELECT * FROM hourlyClimateData WHERE climateStationID = '%s' AND hourlyClimateData.date > DATE_SUB(now(), INTERVAL 1 DAY)""" % (station[0])  #change from CURDATE to most recent download.  also need to verify all cycles included
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
			# average each of the last 21 readings
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
		
				#function that computes the daily EvapoTranspiration from the information in the METAR report
				ETresults = computeDailyETo(avgDailyTemp, maxTemp, minTemp, avgDailyWind, avgDailyHumidity, avgDailyPressure, avgDailyDew, isoTime, 30.183, 172) #latitude and altitude for ATX KAUS
			
				cursor = db.cursor()

				cursor.execute("INSERT INTO dailyETo (climateStationID, ETo, dayOfYear, date) VALUES (%s, %s, %s, %s)", (station[0], ETresults[0], ETresults[1], isoTime))
				cursor.close()	
				db.commit()
			else:
				print humidCount
				print tempCount
				print pressureCount
				print dewCount
				print windCount	
				print "MISSING DATA FOR STATION:  %s " % station[0]
		else:
			print "DON'T HAVE 12 hours of data in the last 24 hours for %s " % station[0]
			print "# Results: %s" % len(results)

#this is the alternative way of finding solar radiation in the absence of sunshine hours data
#it estimates the solar radiation using the difference between the maximum and minimum daily temperature
def computeSolarRadiationFromTemp(extraTerrest, adjCoeff, TMax, TMin):
	return adjCoeff*math.sqrt(TMax - TMin)*extraTerrest

#this function computes solar radiation using the ratio of actual sunshine hours to potential sunshine hours
#this data is not easy to get
def computeSolarRadiationFromSunshineHours(extraTerrest, n, N):
	return (.25 + .5*n/N)*extraTerrest

def computeCloudlessSolarRadiation(extraTerrestRad, elevation):
	return (.75 + 2 * (elevation*(10**(-5)))) * extraTerrestRad

#this function computes the extra terrestrial radiation
def computeExtraTerrestRad(dr, omega, latitude, delta):
	solarConst = .0820
	return (24*60/math.pi) * solarConst  * dr * (omega * math.sin(latitude) * math.sin(delta) + math.cos(latitude) * math.cos(delta) * math.sin(omega))

#computes net longwave radaition
def computeNetLongRadiation(TMax, TMin, actVap, solarRad, solarRadCloudless):
	stefBoltz = 4.903 * 10**(-9)
	TMaxK4 = (TMax + 273.16)**4
	TMinK4 = (TMin + 273.16)**4
	return stefBoltz * ((TMaxK4 + TMinK4)/2) * (.34 - .14*math.sqrt(actVap)) * (1.35 * solarRad/solarRadCloudless - .35)

#computes psychrometric constant from atmospheric pressure
def computePsychrometricConstant(atmospheric):
	return (0.665 * 10**-3)*atmospheric

#computes atmospheric pressure given the elvation of the site
def computeAtmosphericPressure(elevation):
	return 101.3 * ((293 - (.0065 * elevation)) / 293)**5.26

#computes actual vapour pressure (ea) using dewpoint
def computeActualVapourPressure(dewpoint):
	return .6108 * math.exp((17.27 * dewpoint) / (dewpoint + 237.3)) #average daily dew temp?	

#compute es
def computeSaturationVapourPressure(TMax, TMin):
	satVapMax = .6108 * math.exp((17.27 * TMax)/(TMax + 237.3))
	satVapMin = .6108 * math.exp((17.27 * TMin)/(TMin + 237.3)) 
	return (satVapMax + satVapMin / 2) 

def computeSolarDeclination(dayOfYear):
	return .409 * math.sin(2*math.pi*dayOfYear/365 - 1.39) #solar declination 

def computeEarthSunDistance(dayOfYear):
	return 1 +.033 * math.cos(2*math.pi*dayOfYear / 365) #inverse relative earth-sun distance

def computeSunsetHourAngle(latitude, solarDeclination):	
	return math.acos(-math.tan(latitude)*math.tan(solarDeclination)) #sunset hour angle

def computeDailyETo(temperature, TMax, TMin, windspeed, humidity, pressure, dewpoint, date, lat, altitude):
	print "Computing Daily ETo"
	
	#VAPOUR PRESSURE
	#ea
	actVap = computeActualVapourPressure(dewpoint)

	#es	
	satVap = computeSaturationVapourPressure(TMax, TMin)
	
	#delta	
	vapDelta = 4098 * (.6108 * math.exp((17.27 * temperature)/(temperature + 237.3)))/(temperature + 237.3)**2 #average daily

	#RADIATION

	#data needed for computing radiation terms	
	dayOfYear = dayFromISOTime(date) #day of year (1-366)
	earthSunDist = computeEarthSunDistance(dayOfYear)
	latitude = degreesToRadians(lat) #latitude in radians
	solarDeclination = computeSolarDeclination(dayOfYear)
	sunsetHourAngle = computeSunsetHourAngle(latitude, solarDeclination)	
	
	#Ra
	extraTerrestRad = computeExtraTerrestRad(earthSunDist, sunsetHourAngle, latitude, solarDeclination)

	#Rn
	#n = don't have this data at the moment, so must estimate from temperature
	#N = (24/math.pi) * sunsetHourAngle
	#SolarRad = computeSolarRadiationFromSunshineHours(extraTerrestRad, a, b, n, N)
	SolarRad = computeSolarRadiationFromTemp(extraTerrestRad, .17, TMax, TMin)
	
	#Rns
	albedo = .23
	NetSolar = (1 - albedo)*SolarRad 

	#Rso	
	elevation = altitude
	SolarRadCloudless = computeCloudlessSolarRadiation(extraTerrestRad, elevation)
		
	#Rnl
	NetLong = computeNetLongRadiation(TMax, TMin, actVap, SolarRad, SolarRadCloudless)
	
	#Rn
	NetRad = NetSolar - NetLong
	
	#negligible for now
	SoilFlux = 0 
	
	#psychro constant
	height = 2 #hmmmm - why not use station altitude?
	atmosPressure = computeAtmosphericPressure(height)
	psychrometricConstant = computePsychrometricConstant(atmosPressure)
	
	#AIR TEMPERATURE
	temp = temperature #this should probably be average daily temperature.

	#WIND SPEED
	windSpeed = windspeed #this should be average wind speed, and we need a way of knowing at what height it was measured. 

	#ETo
	evapNumPart1 = (0.408 * vapDelta * (NetRad - SoilFlux))
	evapNumPart2 = (psychrometricConstant * 900 * windSpeed * (satVap - actVap))/(temp + 273)
	evapNum = evapNumPart1 + evapNumPart2
	evapDen = (vapDelta + psychrometricConstant*(1 + .34 * windSpeed))
	evapoTranDaily = evapNum/evapDen	

	print "Daily ETo: %0.2f" % (evapoTranDaily)
	returnArray = [evapoTranDaily, dayOfYear]
	return returnArray

#converts the latitude from degrees to radians so it is useful for other computations
def degreesToRadians(latitudeDegrees):
	return latitudeDegrees * (math.pi/180.0)

#converts ISOTime from METAR report to the current number day of the year
def dayFromISOTime(ISOtime):
	
	#datetime.datetime.strptime only work in Python 2.5 + (this is work around)
	#dateObject = datetime.datetime(*(time.strptime(ISOtime, "%Y-%m-%d %H:%M:%SZ")[0:6]))
	dateObject = ISOtime
	leapYear = False

	#January - 31
	#February - 28 (29 leap year)
	#March - 31
	#April - 30
	#May - 31
	#June - 30
	#July - 31
	#August - 31
	#September - 30
	#October - 31
	#November - 30
	#December - 31

	if dateObject.year % 4 == 0:
		leapYear = True

	daysAccumulatedNotLeapYear = [0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365]
	daysAccumulatedLeapYear = [0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366]

	if leapYear == False:
		days = daysAccumulatedNotLeapYear[dateObject.month-1]
	else:
		days = daysAccumulatedLeapYear[dateObject.month-1]

	days = days + dateObject.day
	
	return days	

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
