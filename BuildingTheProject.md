First of all, be sure to meet the following requirements:
  * Visual C++ 2010 (Express and above; no other project file supported, but it shouldn't be hard to port anyway, as long as C++ 0x lambdas are supported)
  * CEGUI development version, more info on the [CEGUIVersion](CEGUIVersion.md) page
  * boost
  * SFML 2

Now tweak the VC10 PropertySheets provided to match your libraries' paths: you can simply open the .props in your favorite text editor and modify the following XML fields :
  * **AdditionalIncludeDirectories**
  * **AdditionalLibraryDirectories**

Open the VC10 solution, compile and enjoy :)