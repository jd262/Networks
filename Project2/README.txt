// This repository contains project 2 for CS 475 for Fall 2017.

The purpose of this project is to extract data from an external server, then perform a useful action with the data.

This application obtains a weather forecast for the upcoming week. First a local client connects to a local server, and the user sends the desired lookup city on the client to the server. The local server then contacts the Wunderground server and requests weather information for the supplied city. The Wunderground server then relays the information back to the local server. The server processes this information and makes it readable for the user and sends it back to the client. 

This project is implemented through C++.