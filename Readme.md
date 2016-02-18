# Survival Shouter

Survival Shouter is a modification of the popular Survival Shooter game demo by Unity Technologies
(https://www.assetstore.unity3d.com/en/#!/content/40756).

## How to play

You will have to download and install Unity 5 (https://unity3d.com/get-unity) and then open this folder 
as a Unity project. Then open `Assets\_CompletedAssets\Scenes\Level 01 5.x.unity` and hit the play button
at the top of the Unity editor. Only Windows (both 32-bit and 64-bit) is currently supported.

You have to play within the Unity editor because the Unity player for standalone builds has problems
with plugins that depend on other plugins. Therefore, prebuilt executables for this game are not provided.

Survival "Shouter" adds "shouting" (ie, a speech recognition element) to the top-down shooting 
gameplay of the original. Most of the controls are the same as the original: you move with WASD, 
use the cursor to aim, and click to shoot (this version does not currently support mobile platforms).
In addition, you can use the following voice commands to set a bomb:

* "Bombs away!"
* "Set bomb"
* "Blow them up!"
* "Boom!"

Note: bombs hurt (a lot). We recommend you run away after setting one.
You will also note that there is a cooldown between bombs.

## Technology

This game uses the speech recognition library `pocketsphinx`, which is a part of `CMU Sphinx`. `splistener` is our C++
plugin that manages the pocketsphinx backend and allows Unity to access the recognized speech data. The C#
scripts that have been added for this version of the game are found in `Assets/Scripts/Shouting`. For more functional
details, see the file `specs.pdf`.

Note: for this application, it makes sense to use keyword spotting mode (kws) instead of the full default language
model (kws would try to recognize only the specified voice commands instead of full english grammar,
making it more efficient for our purposes). A file `keywords` is in the Assets folder that can be used for kws.
However, kws has been temporarily disabled because it has been found to cause splistener to crash.
Until this issue is fixed, the default build uses the full english language model. If you want to try kws,
replace the null in the second argument of `spInitListener` in the Awake() function of 
`Assets/Scripts/Shouting/BombShout.cs` with `Application.dataPath + "/keywords"` and then build the project.

## Acknowledgments

The original Survival Shooter demo is Copyright (c) 2015 Unity Technologies.  
PocketSphinx is Copyright (c) 1999-2008 Carnegie Mellon University.

This game also uses assets from Ball Pack, Copyright (c) 2011 YounGen Tech,  
and Detonator Explosion Framework, Copyright (c) 2014 Ben Throop.
