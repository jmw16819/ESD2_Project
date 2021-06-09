# WeFlow - A Water Leak Detection System
The code in this repository was used for the WeFlow, a water leak detection system developed for my senior design project at Northwestern University. This system monitors the water pressure within different sections of a piping structure and detects drops in the pressure that may be caused by a leak. If the pressure drops below a certain level, the system can automatically close off that section of piping to prevent excessive water loss. Information about the state of the system is also displayed on a webpage for the user to interact with. More information about the WeFlow can be found on its [Hackaday project page](https://hackaday.io/project/180239-weflow-a-water-leak-detection-system).

Everything has been uploaded under the [MIT license](LICENSE). Below are details about each file, and what they're used for.

### [monitor.py](monitor.py)
This Python file was used to create the server used for the WeFlow, which was developed using [Tornado Web Server](https://www.tornadoweb.org/en/stable/). It contains the seven WebSocket handles for the different clients, and the code is configured so that the sending of messages to and from the server are done asynchronously to prevent blocking.

### [monitor.js](templates/static/monitor.js)
This Javascript file provides functionality for the monitor webpage of the system, which is one of the clients connected to the server. Displays pressure data from the system, controls state of solenoid valves, and shows system state messages to the user. Webpage also made with index.html and monitor.css.

### [sensor_client.ino](Arduino/sensor_client.ino)
This Arduino file serves to connect the ESP32 client for each pressure sensor to the server, as well as read in the pressure data from the sensors and control the indicator LEDs that signify how high the pressure is. Currently the bounds are set as if the sensors are reading air pressure, though ideally they would need to be raised so that water pressure can be measured. Adjustments to the file include adding the Wifi ssid and password the ESP32s will be connecting to, adding the localhost of the computer the server is running on, and entering the [MAC addresses](https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/) of the three development boards used for the pressure sensors.

### [valve_client.ino](Arduino/valve_client.ino)
This Arduino file serves to connect the ESP32 client for each solenoid valve to the server, as well as change the state of the valves as needed and control the indicator LEDs that signify the current state of the valve. Adjustments to the file include adding the Wifi ssid and password the ESP32s will be connecting to, adding the localhost of the computer the server is running on, and entering the [MAC addresses](https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/) of the three development boards used for the solenoid valves.
