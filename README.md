# GPIO-Controller
Controls GPIO


To run:
  1. Download ZIP File
  2. Extract ZIP File
  3. Open GPIO-Controller-master/GPIO-Controller-master/SusiGPIO.sln in visual studio
  4. Run Local Windows Debugger on the top of the screen
  5. Script should be running. 

To config:
  1. Open gpiomain.c file under SusiGPIO
  2. DRYRUN Variable depends on hardware
  
  	0- Runs normal script
	1- Bypasses SUSI initialization if hardware is missing
  
  3. On line 42: Define the pin modes for each pin
  
  
	PININPUT- sets pin to input mode
	PINOUTPUT- sets pin to output mode
	PINOFF- sets pin to output mode, defaulted level 0
    
While running:
  1. The program should display the changes of levels on the Input pins.
  2. In order to change output level (pin 2), either click 1 or 2 on the keyboard. 
  
  
	1 - turns GPIO Output level to low				
	2 - turns GPIO Output level to high 
