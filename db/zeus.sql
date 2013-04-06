CREATE TABLE IF NOT EXISTS users (
					userID INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
					email VARCHAR(50) NOT NULL,
					password CHAR(128) NOT NULL,
					salt CHAR(40) NOT NULL,
					created DATETIME,
					updated DATETIME
					);
					
CREATE TABLE IF NOT EXISTS devices (
					productID INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
					userID INTEGER NOT NULL,
					latitude FLOAT,
					longitude FLOAT,
					wifiNetwork VARCHAR(100),
					wifiPassword VARCHAR(100),
					numZones INTEGER,
					climateStationID VARCHAR(10)
					);

CREATE TABLE IF NOT EXISTS zones (
					productID INTEGER NOT NULL,
					zoneNumber INTEGER NOT NULL,
					description VARCHAR(50),
					plantSpecies VARCHAR(50),
					microClimateConstant FLOAT,
					speciesConstant FLOAT,
					densityConstant FLOAT
					);

CREATE TABLE IF NOT EXISTS climateStations (
						climateStationID VARCHAR(10) NOT NULL PRIMARY KEY,
						description VARCHAR(100),
						city VARCHAR(30),
						county VARCHAR(30),
						zipCode INTEGER,
						state VARCHAR(20),
						country VARCHAR(30),
						latitude FLOAT,
						longitude FLOAT,
						altitude FLOAT,
						active BOOLEAN	
						);

CREATE TABLE IF NOT EXISTS hourlyClimateData (
						climateRecordID INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY, 
						climateStationID VARCHAR(10), 
						airTemp FLOAT, 
						humidity TINYINT, 
						pressure SMALLINT, 
						dewpointTemp FLOAT, 
						windspeed FLOAT, 
						cycle TINYINT(2), 
						date DATETIME
						);

CREATE TABLE IF NOT EXISTS dailyETo (
					climateStationID VARCHAR(10) NOT NULL,
					ETo FLOAT NOT NULL,
					dayOfYear INTEGER,
					date DATETIME NOT NULL
					);

CREATE TABLE IF NOT EXISTS dailyETLperZone (
						productID INTEGER NOT NULL,
						zoneNumber INTEGER NOT NULL,
						dailyETL FLOAT NOT NULL,
						dayOfYear INTEGER,
						date DATETIME NOT NULL
						);

CREATE TABLE IF NOT EXISTS queuedIrrigations (
						    vectorID INTEGER NOT NULL,
                            productID INTEGER NOT NULL,
						    zoneNumber INTEGER NOT NULL,
						    startTime DATETIME NOT NULL,
						    duration INTEGER NOT NULL,
						    created DATETIME NOT NULL
						);

CREATE TABLE IF NOT EXISTS executedIrrigations (
                            vectorID INTEGER NOT NULL,
                            productID INTEGER NOT NULL,
						    zoneNumber INTEGER NOT NULL,
						    startTime DATETIME NOT NULL,
						    duration INTEGER NOT NULL,
						    created DATETIME NOT NULL
						);

CREATE TABLE IF NOT EXISTS sessions (
					session_id VARCHAR(128) UNIQUE NOT NULL,
    					atime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    					data text
					);

CREATE TABLE IF NOT EXISTS toggle (
                    productID INTEGER NOT NULL,
                    toggleStatus BOOLEAN DEFAULT TRUE
                    );

CREATE TABLE IF NOT EXISTS learning (
                            vectorID UNIQUE INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
                            deviceID INTEGER NOT NULL,
                            zoneNumber INTEGER NOT NULL,
                            ETo FLOAT NOT NULL,
                            score INTEGER NOT NULL
                            created DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
                            );

CREATE TABLE IF NOT EXISTS vectors (
                            vectorID INTEGER NOT NULL,
                            columnNumber INTEGER NOT NULL,
                            value FLOAT NOT NULL
                            );

CREATE TABLE IF NOT EXISTS feedback (
                            deviceID INTEGER NOT NULL,
                            created TIMESTAMP,
                            value INTEGER NOT NULL
                            );
