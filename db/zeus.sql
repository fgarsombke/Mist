CREATE TABLE IF NOT EXISTS users (
					userID INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
					email VARCHAR(50) NOT NULL,
					password CHAR(128) NOT NULL,
					salt CHAR(40) NOT NULL,
					created DATETIME,
					updated DATETIME
					);
					
CREATE TABLE IF NOT EXISTS devices (
					deviceID INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
					userID INTEGER NOT NULL,
					latitude FLOAT,
					longitude FLOAT,
					wifiNetwork VARCHAR(100),
					wifiPassword VARCHAR(100),
					numZones INTEGER,
					climateStationID VARCHAR(10)
					);

CREATE TABLE IF NOT EXISTS zones (
					deviceID INTEGER NOT NULL,
					zoneNumber INTEGER NOT NULL,
					description VARCHAR(50),
					);

CREATE TABLE IF NOT EXISTS dailyETo (
					climateStationID VARCHAR(10) NOT NULL,
					ETo FLOAT NOT NULL,
					dayOfYear INTEGER,
					date DATETIME NOT NULL
					);

CREATE TABLE IF NOT EXISTS queuedIrrigations (
						    vectorID INTEGER NOT NULL,
                            deviceID INTEGER NOT NULL,
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
                    deviceID INTEGER NOT NULL,
                    toggleStatus BOOLEAN DEFAULT TRUE
                    );

CREATE TABLE IF NOT EXISTS learning (
                            vectorID INTEGER UNIQUE NOT NULL AUTO_INCREMENT PRIMARY KEY,
                            deviceID INTEGER NOT NULL,
                            zoneNumber INTEGER NOT NULL,
                            ETo FLOAT NOT NULL,
                            score FLOAT NOT NULL,
                            created TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
                            );

CREATE TABLE IF NOT EXISTS vectors (
                            vectorID INTEGER NOT NULL,
                            columnNumber INTEGER NOT NULL,
                            value FLOAT NOT NULL
                            );

CREATE TABLE IF NOT EXISTS feedback (
                            deviceID INTEGER NOT NULL,
                            zoneNumber INTEGER NOT NULL,
                            created TIMESTAMP,
                            value INTEGER NOT NULL
                            );

CREATE TABLE IF NOT EXISTS weatherCache (
                            latitude FLOAT NOT NULL,
                            longitude FLOAT NOT NULL,
                            beginTime INTEGER NOT NULL,
                            endTime INTEGER NOT NULL,
                            json TEXT NOT NULL
                            );
                          
