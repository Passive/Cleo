# Cleo
![image](https://images-ext-2.discordapp.net/external/E6TLOZylKls5WKxD6XijIAAnh7yiVgRN3fax3Vo7_f0/https/repository-images.githubusercontent.com/365388923/06101b80-b060-11eb-9196-5b89343294ab?width=500&height=375)                                                                                                                                                                             
Written specifically to target windows machines running Discord.                                                                                               
Version: 5.0

We are not responsible for what you do with this code.                                                                                                           
If you encounter an error or issue, make an issue [here](https://github.com/dehoisted/Cleo/issues).                                                                   
Lastly, if code here is used for something else, **give credit where credit is due**.

# Features
Note: All info grabbed will be sent to the given webhook.

Feature  | Description
-------- | -----------
Silence | Process is hidden when ran.
Info Grab | Grabs ALL important PC information & discord account information.
Persistence | Runs at startup & infects discord installation.
Anti VM | Checks system manufacturers & names that are virtual machines (exits if detected)
Anti Debug | Uses methods to check if program is being debugged. (exits if detected).
Anti Analysis | Checks if an analyzing process is open. (exits if detected).
Watch Dog | Runs anti analysis in a new thread, kills blacklisted process if open.
Obfuscation | Automatic compile-time string obfuscation.

# Build

Steps for building/compiling Cleo yourself.

## Dependencies
All dependencies listed can be installed automatically by running the powershell script [here](https://github.com/dehoisted/Cleo/blob/main/install_dependencies.ps1).
+ [LibCurl](https://curl.se/libcurl/c/) - Used by CPR behind the scenes.
+ [CPR](https://github.com/whoshuu/cpr) - Simply a wrapper over LibCurl, used for all HTTP requests in Cleo.
+ [JSON Parser](https://github.com/nlohmann/json) - For parsing discord account information.
+ [String Obfuscator](https://github.com/adamyaxley/Obfuscate) - Used for hiding important strings. (already in Cleo source as "AY_Obfuscate.h")

After installing dependencies, make sure it's all statically linked.                                                                                            
Then, make sure you compile on Release Mode. (x86)                                                                                                              
Make an issue [here](https://github.com/dehoisted/Cleo) if you get any compilation errors.

# Credits                                                                                                                                               
+ [Dehoisted](https://github.com/dehoisted) - Making all of Cleo V5.                                                                                   
+ [Passive](https://github.com/Passive) - Making other versions & coming up with the idea for Cleo itself.                                                               
