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

	if dateObject.year % 400 == 0:
		leapYear = True
	elif dateObject.year % 100 == 0:
		leapYear = False
	elif dateObject.year % 4 == 0:
		leapYear = True
	else:
		leapYear = False

	daysAccumulatedNotLeapYear = [0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365]
	daysAccumulatedLeapYear = [0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366]

	if leapYear == False:
		days = daysAccumulatedNotLeapYear[dateObject.month-1]
	else:
		days = daysAccumulatedLeapYear[dateObject.month-1]

	days = days + dateObject.day
	
	return days	