 ## LED Matrix Display IOT Clock
 
 ### Details:
 
 - Using an old ESP32 and 3 *very* old HT1632 8x32 LED matrix panels
 	- Originally created ~2011 for a [Make Magazine project](https://makezine.com/projects/ps2you-led-sign/), been [updated over the years to current form
 - Features:
	 + Spotify Integration: 
		 * Start playing a song and the sign automatically switches to act as a  "now playing" display
		 * Displays song information and live duration
		 * Used to scrape lyrics as well but haven't ported that feature to this version yet
	 + Headlines:
		 * Using [NewsAPI](https://newsapi.org/) to find and scrape headlines
			 - Configurable sources and topics for these headlines
			 - Day to day, I have it pull a bunch of headlines every hour, and display a few every 10-15 minutes
			 - Free for under 1000/requests a day
	 + Weather
		 * Determines city and rough geographical coordinates from IP via [ipwhois.io](https://ipwhois.io) API at startup
		 * Uses [OpenMeteo](https://open-meteo.com) for Weather information
		 * Automatic updates every 15 minutes
		 * WIP: custom icons for different forecasts (cloudy, snow, etc)
	 + Clock
		 * NTP based timekeeping
		 * It's an overengineered digital clock, what else can I say
		 * WIP: fun clockfaces like Conway's Game of Life, animations, etc.
	 
	### Gallery:  
	
	![Image]()
	
