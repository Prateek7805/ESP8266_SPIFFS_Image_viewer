# ESP8266_SPIFFS_Image_viewer
This is an Image Viewer that dynamically loads images in SPIFFS and displays them in the HTTP Response
This is an easy to use Image viewer or a reference or starter code for implementing upload in a webserver.
This code allows clients to view delete and upload images to an ESP8266 Flash memory
The amount of SPIFFS memory depends on the flash chip present on the ESP8266 module as displayed below:
Module                            Size
ESP12E/ESP12F                     3MB
ESP-01                            512KB
ESP-01F                           512KB
NodeMCU/LUA                       3MB
You need SPIFFS implemented on ESP8266 inluding the (FS Uploader tool)[https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/]
Dependencies
(WiFiManager)[https://github.com/tzapu/WiFiManager]
