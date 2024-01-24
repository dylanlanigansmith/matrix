 ## LED Matrix Display IOT Clock
 
 ### Details:
 
 - Using an old ESP32 and 3 *very* old HT1632 8x32 LED matrix panels
 	- Originally created ~2011 for a [Make Magazine project](https://makezine.com/projects/ps2you-led-sign/), and has been [updated over the years to its current form
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
		 * In the end, it's an overengineered digital clock, what else can I say?
		 * WIP: fun clockfaces like Conway's Game of Life, animations, etc.
 - Currently for the last 10-12 years has been enclosed in an old set of blinds,
	 + One of these days I'll finish the enclosure... right?
		 * Would love to add one more panel but you can't get these matrix displays w/ the same driver IC anymore :-( 
	 
	
	### Gallery:  
	
	![Image](https://raw.githubusercontent.com/dylanlanigansmith/matrix/main/img/clock.jpeg "Clock mode caught mid-refresh hence funny pixels")
	
	![Image](https://raw.githubusercontent.com/dylanlanigansmith/matrix/main/img/headline.jpeg "A Headline Comes In")
	
	![Image](https://raw.githubusercontent.com/dylanlanigansmith/matrix/main/img/spotify.jpeg "Clock mode caught mid-refresh hence funny pixels")
	
	#### Let me assure you it looks better in person 
	
* The panels altogether are roughly a foot and a half long. 
	- Quite well suited for viewing from across the room.. 
	


### Code

- Should really be rewritten from scratch
	+ Tech debt from using the same modded HT1632 lib for 10+ years
		* Originally for an older Arduino, could really be faster/more efficient
	+ Ideally should use RTOS / task based approach and better queuing for different tasks / states accessing the display
- WebUI needs a finished settings page
- WiFI credentials need to be saved to flash, and a setup mode where the ESP hosts a network so one can join it and provide wifi-creds 
	+ Would be cool for a party mode as the "now playing" feature would be nice next to a bluetooth speaker 
- Boy do I love ESP OTA updates, makes development fun not fiddling with USB->FTDI cables and reset buttons. 
	+ Noticing a bug, quickly fixing it, and having Platform.IO uploading a patch automatically post-compilation makes for fantastic workflow
- One must configure [api_config.hpp](https://github.com/dylanlanigansmith/matrix/blob/main/matrix/include/api_config.hpp.example) with their own credentials 

		