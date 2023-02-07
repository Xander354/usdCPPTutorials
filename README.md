### USD Tutorial project in C++
Very barebones, source and vs2022 project for doing the [USD tutorials](https://graphics.pixar.com/usd/release/tut_usd_tutorials.html) and other experiments in C++ with OpenGL. 

### Setting up the environment

<details>
<summary>Building USD</summary>

First step is building USD and all of its dependenices. This is mostly easy and can be done by following the instructions on [their repo](https://github.com/PixarAnimationStudios/USD#readme). A couple helpful tips though:
- Before starting make sure your CMake version is >=3.24
- It lists several dependencies that are needed however if you already have VS installed then the only ones you should need to install yourself are the Python ones (afaik). Before running their build script you can do
  - `pip install PySide6`
  - `pip install PyOpenGL`
- <b>This ones important:</b> If you're on Windows and using Visual Studio, you'll want to run the build script command that they provide in a VS command prompt. i.e `x64 Native Tools Command Prompt for VS 2022`. Ideally you can just search `x64` in the start menu and it will be listed. Run as administrator. This is needed because those command prompts use an environment that includes the msvc compiler. If you get an error saying `No C++ compiler found` check that you're in the right prompt. 

From there the build command should work fine and install everything you'll need, then just add the path items that the script says at the end and you should be good to go for USD
</details>

<details>
<summary>Setting up the project</summary>
No build scripts or anything nice right now, maybe later. For now this is just a VS2022 project.

If everything's setup correctly you should be able to open the project in VS2022 and change a few settings, then build. 
The settings that need changing are include and library directories. Everything else <i>should</i> be setup. 
- In Properties (Make sure the configuation is all versions and platform is `x64`)
  - C/C++
    - General
      - In Additional Include Directories you'll want to add the following
        - The path to your USD include folder (i.e <drivename>:\usr\local\USD\include)
        - The path to the boost folder in the USD include folder (i.e <drivename>:\usr\local\USD\include\boost-1_78)
        - The path to your Python installation's include. To see which version run `python --version`. To find the directory locate your `Python.exe` file and that directory should have an include directory
  - In Linker
    - General
      - We'll need to add to Additional Library Directories
        - The path to the USD installations libs folder (i.e <drivename>:\usr\local\USD\lib)
        - The path to your Python installation's libs folder. Can be found in the same place as include. Specifically `libs` and not `Lib`

With that you should be able to build and run <b>in Release mode</b>(There's a bug in TBB for windows debug mode). If you can't, sorry, I'm not in devops. 

</details>

### Using the project

The project is meant to be able to do the USD tutorials linked above. To do this there is a `Tutorials.cpp` file with functions for most of the tutorials defined (some were skipped because they didn't include code or they include using other editors). If you're only doing the tutorials then this should be the only file you'll need to look at. The first `HelloWorld` tutorial is given and runs when you start the project. 

The existing code in the tutorial methods should not be modified (unless you want to) and the tutorials should go where the comments are.

To change which tutorial is running just change the method called at the bottom of the file in `Tutorial::runTutorial`

Doing the tutorials in C++ will require some intuiting of translating the Python to C++. A lot of it is simple because its already just binding to C++, however the typing can be a bit confusing. Don't forget to check the [documentation](https://graphics.pixar.com/usd/release/api/index.html). 

A set of the completed tutorials is included in `CompletedTutorials.cpp` if you want to see how one is done. You can also run these without looking at the code by calling `CompletedTutorial::<name>` in `Tutorial::runTutorial`

A bit about controls:
- Press R to restart the scene
- Camera controls are a bit janky but since they're not super relevant I didn't put a whole lot in. 
  - WASD to move
  - Arrow keys to rotate (pitch and yaw)
  - Hold ctrl to strafe
  - Space and Shift go Up and Down respectivley
