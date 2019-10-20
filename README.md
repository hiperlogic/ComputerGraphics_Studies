# ComputerGraphics_Studies

Repository to gather code related to OpenGL and Vulkan Studies and Explorations

The process will be constructed considering extending the previous tutorial within a new branch. So, the master branch contains all the tutorial folders empty.

Each branch will be named considering the number of the tutorial and the technology used. All the folders with number greater than the indicated in the branch will contain the same code as in the branch. All the folders with number lesser than the indicated in the branch will contain its own code.
The branches have a conceptual hierarchy. This means that branch number XX may have _chapters_, which will be indicated as _YY.
Thus, the branch structure, for clarification, is XX_[YY_]Branch_Name. Where XX, YY and Branch_Name stands for:

XX is the branch order.
YY is the order of that branch improvement and can be ommitted.
Branch_Name is self-explanatory.

With this process it is possible for the student to use the diff tool to easily identify what was added or removed.

This, so far, is not created with intention to educate other than maintain the author current knowledge (and improve it). It can, however, become an educational tool. If you have suggestions about how better to educate others, please inform the author and your suggestion will be taken into account and mentioned in future endeavors.

## Configuring The environment to work with [CMAKE](https://cmake.org/)

I really needed to recall the multiplatform programming style with C/C++, since it has been quite a while from the last time I coded things to be easily compiled in Linux and Windows (and C/C++, with Python it is a piece of cake! But Python lacks some useful things to work with OpenGL, like GLEW). And it was done with [CMake](https://cmake.org/), in a time when CMake for Windows was barelly functional (`I'm talking about finding tools, including headers and libraries to be linked... toolchaining was another serious issue that I preferred working with branch commands and imperative algorithmic logic`). By “a while” I really mean about *7 years*!

So, CMake was the first choice to use, since it can generate builds for Windows, Linux, MacOS (and pretty much anything else commercially available).

### A little about CMake

  CMake is not a compiler, it does not generate a software. What it does is prepare all your code, along with the dependencies needed by the source files, to be compiled for the target system.
  The target system is the system you run the CMake to generate the *make* files, unless you specify a toolchain so it generates the *make* files for another system. (The cross compiling/toolchain creation subject won't be addressed in this project, since the codes developed here are pretty much straightforward to be compiled in Linux, Windows or Mac).
  So, if you are in Windows and have Visual Studio installed, CMake generates the `Visual Studio Solution` and `Project(s)` files. In Linux, it generates the files `configure` and `Makefile`, it is no wonder if on Mac it would generate `XCode files` that resemble the `Visual Studio Solution and Project(s)` files.

So, the issue was: How is CMake handling with Windows? (Because within *nix systems it is great!)
Luckily (and with some years in delay... at least 7) Microsoft finally is providing a good tool support for developers (that endeavor into other systems!). I'm talking about [`vcpack`](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019). A package manager for Windows... yes, a package manager for windows, but it deals only with development libraries and... integrate seamlessly with CMake. If you haven't installed yet, go on and do it! It will save you some time (after you struggle to make things work well! But that's why I'm writing this... so... keep on, I'll tell you how to do it soon!)

The actions you need to do for this to work is:

* Install [CMake](https://cmake.org/)
* Install [vcpack](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019) if you are in Windows System.

## The Editor

Code is text, so any text editor would work to get the job done, but we, developers, are lazy. We do not want to switch the application to the command line just to compile the code and get frustrated with the warnings and errors. We want the warnings and errors easily to get!... (maybe we are not lazy at all... just psychologically disturbed... like everyone else!).
IDEs like Visual Studio, XCode, Netbeans, Eclipse or QtDevelop (are there others?) are heavy, they are somewhat sluggish, sometimes take forever for us to start typing!... I want to produce warnings and errors and feel the thrill to get frustrated as soon as I click the editor! (Maybe I should have been a writer!). Also, choosing an IDE would force my lazyness to switch from CMake to the IDE self building structure and this could kill my original intention of writing code as multiplatform as possible.
That's why the choice was [Visual Studio Code](https://code.visualstudio.com/). Aside it being the editor I use professionally, it is fast, it has several features provided by extensions and seamless Git integration, a good fit to use in this project.
The tools and languages features are available via extensions. If you are trully following this project, using it for learning, updating, remembering or just to see how others do Computer Graphics with OpenGL (and Vulkan), these are the extensions you must have:

* C/C++ (By Microsoft) to integrate with Microsoft C/C++ compiler. You should need Visual Studio, at least community edition. On linux systems this will be updated in the future.
* CMake (By twxs) Core features for CMake manipulation in VSCode
* CMake Tools (by vector-of-bool) Extra tools for CMake manipulation in VSCode

## Configuring Visual Studio Code to Compile C/C++ code

This is the main action if you want to use VSCode to write C/C++ coded software. And the VSCode team couldn't be more helpful in providing their documentation about how to do it. Seriously, it is simple, it works and all you need to do is follo the instructions at [VSCode configuration for C/C++ Languages](https://code.visualstudio.com/docs/languages/cpp).
Follow there, test compiling the program and then you ar already set for the very first project in branch 00.

## Other Editors

Although this project is aimed to use Visual Studio Code, you can use other editors, or even IDEs, if you are more prone to or more experienced with them.
Atom works somewhat like Visual Studio Code, VIm and EMacs have extensions as well. The main issue in using CMake is to let the process as system independent as possible. So, basically, within each project conclusion all you need to do is:

1. Generate the build files via CMake.
2. Configure the options (when available), also using CMake.
3. Open the Solution (Visual Studio) or the shell with current working directory where the Makefiles are.
4. Compile the solution (either clicking a button on Visual Studio or calling make in the shell).
5. Install the generated project (We will not really cover this one!).

Or, just click the build button (or calling the build command) in one of the mentioned editors (VSCode, Atom, VIm, EMacs)

Next Branch 00: The initial application framework: Configuring and instancing a Window

