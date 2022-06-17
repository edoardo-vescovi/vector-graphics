# Vector graphics

Proofs of concept based on OpenGL (nehe.gamedev.net/) and SDL (www.lazyfoo.net/) tutorials. They rely on the header windows.h and therefore can be compiled and run only on Windows. I worked on Windows 7 with the latest releases of the relevant software in 2010.

## How to set up virtual machine
If your machine doesn't run Windows 7, you can set up a virtual machine.  
1) Download Windows 7 (e.g. archive.org/details/Windows7SP1ENGAIOx86x64, which comes with blank product key)  
2) Install Oracle VM VirtualBox (www.virtualbox.org/wiki/Downloads)  
3) Open VirtualBox > New > follow instructions to set up machine (RAM 2048 MB, dynamically-allocated HD 32 GB)  
4) Settings > Storage > Empty > click disk image > Choose a disk file... > select ISO > OK  
5) Start > follow instructions to install Windows 7 (e.g. Home Basic x64, Custom installation, click Next when prompted to input product key)

## How to import repository
You can download this repository from GitHub. If you run a virtual machine, you also need to send a copy to it.
1) In VirtualBox top bar: Devices > Shared Folders > Shared Folder Settings... > click add image > choose Folder Path and select Auto-mount 
4) Devices > Add Guest Additions CD image...  
5) In Windows 7, run D:\VBoxWindowsAdditions to mount a shared folder in Z:\  
6) Move the downloaded files from the shared folder to the virtual machine (e.g. desktop)

## How to compile
The compiler's instructions are written for Dev-C++.  
1) Install Dev-C++ 5.0 (www.bloodshed.net/)  
2) Check C:\Dev-Cpp\include\GL contains gl.h, glaux.h, glext.h, glu.h  
3) Check C:\Dev-Cpp\lib contains glaux.lib  
4) If not, copy/paste any missing file from the folder “OpenGL files”  
5) Each repository's folder brings along glaux.dll, but you can alternatively place it in C:\Windows
6) Other headers and libraries may be missing upon compiling - please download and place them in the correct folders

## Commands
The programs are designed to support none or some  
- Directional arrows: move point of view  
- F: toggle fog/normal view  
- M: change rendering (solid, wireframe, points)  
- S: screenshot  
- F1: toggle fullscreen/windowed mode
- ESC: terminate  

## Notes
- I compiled the sources as above. I also uploaded the 64-bit programs (for Windows users) and screen recordings (for all users).  
- Movements may appear slow/fast depending on the rendered objects and the machine. For the latter, you can change the hard-coded values of linear and rotational speed and recompile the source.  
