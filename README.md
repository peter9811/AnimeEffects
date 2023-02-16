# AnimeEffects

🇯🇵 [日本語](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README-jp.md)

A 2D animation tool which doesn't require a carefully thought-out plan, it simplifies animation by providing various functions based on the deformation of polygon meshes.<br>
Originally developed by hidefuku, it is now being developed and maintained by its community.

* Official Website:<br>
  * https://animeeffectsdevs.github.io/

* Official socials:<br>
  * Discord: <a href='https://discord.gg/sKp8Srm'>AnimeEffects Community Server</a> (courtesy of @Jose-Moreno)<br>
  * Twitter: <a href='https://twitter.com/anime_effects'>AnimeEffects</a> (maintained by @p_yukusai)<br>

Note: For the present there may be incompatible changes made, these will be made known in the release affected should they occur.<br>
***If you have any issues or wish to suggest new features, feel reach out to us on our socials!***

## Downloads
* The latest AnimeEffects release is available [here](https://github.com/AnimeEffectsDevs/AnimeEffects/releases), to use it just extract its contents to a folder of your choosing, and then simply run the executable.<br>

## Requirements
* Windows/Linux/Mac
  * See compatible versions bellow.
* OpenGl 3.3 or higher
  * On linux, you can check whether your graphics card supports OpenGL3.3 CoreProfile or not, run `glxinfo | grep "OpenGL core profile version"` on your terminal.
* [FFmpeg](https://ffmpeg.org/download.html) (Necessary for video exporting, you can place it on your path or copy it to the "/tools" folder.)

## OS Targets
#### This is what we are compiling and testing the software on, it may work on older versions but this is discouraged.
* Windows 10 or newer.
* Ubuntu LTS or newer.
  * The provided AppImage will *not* work on older versions due to glibc.
* macOS Big Sur or newer.
  * None of our contributors actually has a Mac to test AnimeEffects on, but we do keep track of compilation errors and artifacts.

## Development requirements
* Qt 5.14 or later.
* MSVC2015/MinGW/GCC/Clang (32-bit or 64-bit)

## Linux
### Installing Dependencies
#### Debian / Ubuntu

* First update and install dependencies:

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git gcc libglib2.0-0 qt5-default make
```

#### Arch / Manjaro
* First update and install dependencies:  

```
sudo pacman -Syu
sudo pacman -S git gcc glib2 qt5 make
```

### Clone / Building
* Clone AnimeEffects git repo and go to the "src" folder:  

```
git clone https://github.com/herace/AnimeEffects  
cd AnimeEffects/src
qmake AnimeEffects.pro
make
```
* When building is done, run AnimeEffects:
```
./AnimeEffects  
```

## Windows
* It is recommended that you use QtCreator for compilation:
```
If you're not using QtCreator, it is adviced that you add the bin folder of your compiler of choice and of its tooling to your path, and then check out the powershell scripts available for building and deploying (MinGW is recommended) 
Clone the project and open "AnimeEffects.pro" using QtCreator
Compile the project on the release profile 
Open your console of preference
Run "windeployqt.exe --release "path_to_the_executable""
```

* When deployment is done, you may just run AnimeEffects.exe
