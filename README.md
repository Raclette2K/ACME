# ACME
For more informations : https://acme.raclette-it.com
## What is AMCE ?
ACME is an "autoclick", it can generate "clicks" like you'd do with your mouse but it can do it with a defined delay.
<br>Furthermore, you can define an interval for the delays like between 100ms and 200ms and it will randomize the delay every new click.
## How to use it ? 
It's pretty easy, select how you want to trigger ACME, how you want your delay (if it is constant or randomized) and finally select a key and apply changes !
<br>Now you just have to use the key you selected to trigger it.
<br>You can also select if it is a right or left click and even do a mouse wheel **scroll** (not button).
<br>More informations on how to use it at https://acme.raclette-it.com/htu.html
## Is it a cheat ? 
**IT IS NOT INTENDED TO BE A CHEAT !**
<br>You have no warranty whatsoever while using ACME
## Ok so what's the point of this ?
Well no point at all. Just a project I've made because I was bored and to improve myself in C++ because I'm just a High Schooler 
(and I want to precise that, of course, the code is not really the best you can have for this kind of project and I'm pretty sure 
there is a lot of problems in it)
## What does this repo contain ? 
This repository contains all the source code including the Visual Studio project file.
<br>If you want to modify the project be sure to use VS2017 and the Windows 10 SDK.
## So where do I download it ? 
Oh pretty simple, go to https://acme.raclette-it.com/download.html or get it from the folder Release in the repo.
<br>You may need the VCRedist 2017 x86/x64 which you can download from the official Microsoft website.
## Known issues :/
If you find issues, feel free to report them, I'll gladly look into them !
### App not launching (msvcp140.dll missing or something like that)
To solve this issue, download the VCRedist 2017 in x86 or x64 (depends of the version of ACME you downoaded) from Microsoft website : https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads
### App not receiving the clicks
Sometimes, some app will not work with ACME. Try to launch ACME in administrator because the *SendInput()* function from the Windows API doesn't work on app that are more privilegied than ACME. And if it still doesn't work and the app you are using is in fullscreen, try to set it in windowed mode. 
### Can't change the Trigger Key
Issue reported on Windows 7 : When you want to change the trigger key, it doesn't wait for your input.
<br>If you still want to use ACME, you can open the .ini file and change the VK to the value in DECIMAL not HEX of the key you want to use using https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx as reference. (e.g. : 0x10 is shift, so you put 16 in VK because 0x10(HEX) = 16(DECIMAL))
<br>**No known solution for the moment, a change in the code may be required**
### Random bugs
- When in swith mode and changing the trigger key, the swith mode can be trigger by itself. So be wary when you are in switch mode. It is recommended to set ACME in push mode before changing the key to avoid this tiny but annoying bug.
