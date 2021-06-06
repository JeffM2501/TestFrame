# Raylib/ImGui TestFrame
This is a test framework that uses Raylib and ImGui together to help test and develop concepts.
It is made using C++ because it uses classes for windows and views.

The idea is you add code to places in the test frame to get something showing quickly with access to common features such as resources, cameras, and other tools.

![image](https://user-images.githubusercontent.com/322174/120928519-d02f7880-c699-11eb-9bb7-5a47564ca80d.png)

## Building

1. Make sure you have the submodules
2. Make sure that you have ImGui in raylibExtras/imgui
    1. Be sure to use the docking branch of ImGui
    2. Download from https://github.com/ocornut/imgui/tree/docking
3. Get the premake5 for your platform
    1. Download https://premake.github.io/download
    2. Don't get Premake4
5. Run premake
    1. Batch file for visual studio 2019 is included
    2. Other targets will need the command line (premake5 gmake)
6. Build


# What this does
The testframe has two main concepts Views and Windows. Each exist to display data and work with each other.

# Viws
Views are shown in the background window in the area that is not covered by docked windows. There is only ever one view up at a time. Each View uses a render texture so you can draw to it as if it was a little screen. Views are based on the MainView class and have an OnShow virtual method where you can put drawing code. You can have as many view types as you want and setup the View menu to swap between them.

## SceneView
This is the default view and is 3d. It sets up a first person camera, skybox and ground grid by default.

## SpriteView
This is a 2d View used for sprite-like drawing.

# Windows
Windows are ImGui windows that are docked around the view. They are used to display information about the view. They are based on the UIWindow base class and have a virtual OnShow method that is passed the view. You can use this to get data from the view and display it. There are two default Windows, but you can add as many as you want. The Window menu will display the status of all known windows.

## LogWindow
This shows the raylib log output. Has filters for type and by text to search for specific issues in the log.

## Inspector
This shows data from the view. It is meant to be exteneed with more info.


# Tools
The tools menu has several things to help debug ImGui.

## Item Picker
This will start an ImGui Item Picker, the next gui element you click on will trigger a breakpoint in the debugger so you can see where in the callstack the code that makes the GUI is.

## Style Editor
This is a realtime style editor

## Metrics
This shows debug info about ImGui

## Demo
This is the ImGui Demo window

## About
The ImGui version
