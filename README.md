# Moviedata Editor
A tool to edit moveiedata and moviesound files in New Super Mario bros. Wii.

## Special Thanks
- [CLF78](https://github.com/CLF78) for his documentation
- [LiQ](https://github.com/realLiQ) for programming assistance
- App icon designed by [Freepik](https://www.freepik.com/icon/birthday-cake_3159518#fromView=keyword&term=Birthday+Cake&page=1&position=7)

## Compiling
### Windows
Note: these are the steps that I used.

Requirements:
- Microsoft Visual Studio
- wxWidgets Libraries

A video I used to set it up can be found [here](https://youtu.be/ONYW3hBbk-8).

Once you've done that, click on the .sln file, set it to `Release` config and your desired platform (`x86` or `x64`). Then, click on `Build`, and click `Build Solution`.

The program will then compile. If compiling for x86, it will be found in the **Release/** folder. If compiling for x64, if will be found in the **x64/Release/** folder.

Note that you'll want to still include the **res** folder in the same directory as your executable to load the icon.

### MacOS and Linux
(nothing here...yet)
