# Survival Shouter

Survival Shouter is a modification of the popular Survival Shooter game demo by Unity Technologies. 

## Setup

In addition to the entire Unity project and all assets, this distribution includes pre-built executables
for 32-bit and 64-bit Windows in the bin folder. To build the game for other platforms, you will have to first
build the splistener plugin for that platform and add it to the Unity project. For details on how to
build splistener, see the readme file in the `splistener` folder. For instructions on building a Unity 
project, please consult the Unity Engine documentation at http://docs.unity3d.com/Manual/index.html.

## How to play

Survival "Shouter" adds "shouting" (ie, a speech recognition element) to the top-down shooting gameplay.
Most of the controls are the same as the original: you move with WASD, use the cursor to aim, and click
to shoot (this version does not currently support mobile platforms). In addition, you can use the following
voice commands to set a bomb:

* "Bombs away!"
* "Set bomb"
* "Blow them up!"
* "Boom!"
* (or really anything with the word "bomb" or "blow" or "boom")

Note: bombs hurt (a lot). We recommend you run away after setting one.
You will also note that there is a cooldown between bombs.

## Technology

This game uses the speech recognition library pocketsphinx, which is a part of CMU Sphinx. `splistener` is the C++
plugin that manages the pocketsphinx backend and allows Unity to access the recognized speech data. The C#
scripts that have been added for this version of the game are found in Assets/Scripts/Shouting. For more technical
details, see the file `specs.pdf`.

## Acknowledgments

The original Survival Shooter demo is Copyright (c) 2015 Unity Technologies.
PocketSphinx is Copyright (c) 1999-2008 Carnegie Mellon University.
