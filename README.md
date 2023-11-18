<h1 align="center">ALSXT - ALS-Refactored XT</h1>

<p align="center">
<img src="ALSXT.png" alt="ALSXT">
</p>

<p align="center">
    <a href="https://github.com/Voidware-Prohibited/ALSXT/commits/master">
    <img src="https://img.shields.io/github/last-commit/Voidware-Prohibited/ALSXT.svg?style=flat-square&logo=github&logoColor=white"
         alt="GitHub last commit">
     </a>
    <a href="https://github.com/Voidware-Prohibited/ALSXT/issues">
    <img src="https://img.shields.io/github/issues-raw/Voidware-Prohibited/ALSXT.svg?style=flat-square&logo=github&logoColor=white"
         alt="GitHub issues">
    </a>
    <a href="https://github.com/Voidware-Prohibited/ALSXT/pulls">
    <img src="https://img.shields.io/github/issues-pr-raw/Voidware-Prohibited/ALSXT.svg?style=flat-square&logo=github&logoColor=white"
         alt="GitHub pull requests">
    </a>
    <a href="https://github.com/Voidware-Prohibited/ALSXT/blob/master/LICENSE">
    <img src="https://img.shields.io/github/license/Voidware-Prohibited/ALSXT.svg?style=flat-square&logo=github&logoColor=white"
         alt="MIT License">
    </a>
    <a href="https://github.com/sponsors/colorindarkness">
    <img src="https://img.shields.io/github/sponsors/colorindarkness.svg?style=flat-square&logo=github&logoColor=white"
         alt="Become a Sponsor">
    </a>
    <a href="https://www.patreon.com/colorindarkness">
    <img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Dcolorindarkness%26type%3Dpatrons&style=flat"
         alt="Become a Patron">
    </a>
</p>

An Unreal Engine 5 plugin that extends ALS-Refactored with a set of popular replicated gameplay features, to demonstrate the extendability of ALS-Refactored in a Modular, Data-Driven, UE5 Way. The feature-set is informed by First and Third Person Tactical Shooters, Action Shooters and the parent project [Target Vector](https://github.com/Voidware-Prohibited/TargetVector).

_ALSXT is under heavy development, many features may not be finished or production-ready_.

_[Contibutors](#Contributions) and PRs are welcome_.

# Introduction

**Features**

- Expanded Character States (Combat Stance, Sex, Variant, Weapon Ready Position)
- Sliding Locomotion Action (with Sound, Decal and Particle Support)
- Expanded Footprints System (Independent foot states, Saturation, Material Transfer, Vertex Paint Support(Static Mesh only, In Progress))
- Combat Component _In Progress_
- Impact Reactions (Blending reaction animations with Physical Animation) _In Progress_
- Character Sound Component_In Progress_
- Idle Animation Component _In Progress_
- Paintable Skeletal Mesh Component _In Progress_
- Vaulting _In Progress_
- Camera FX Component _In Progress_
- Acrobatic Actions (Flipping, Wallrunning etc) _In Progress_
- Basic Stats Component (Stamina and Health to demonstate the functionality of various components)

**Planned Features**
- New Skeletal Mesh and Clothing set with Morph Targets/Shape Keys
- Clothing Component (Clothing States, Paintable)
- Gestures
- Emotes
- Stationary Modes (Sitting etc)

**Dependencies**

- [Advanced Locomotion System Refactored](https://github.com/Sixze/ALS-Refactored/)

**Important Note**

ALSXT is not automatically synced with the main ALS-Refactored repository, but is instead maintained through [this fork](https://github.com/Voidware-Prohibited/ALS-Refactored). If you find changes in ALS-Refactored have broken ALSXT you may use this fork to maintain more stability in your projects.

# Installation

- Begin with an empty C++ Project
- Install and setup Enhanced Input Plugin
- Install and setup MetaSound Plugin
- Install the latest version of ALS-Refactored via https://github.com/Sixze/ALS-Refactored#quick-start
- Clone/download ALSXT into your projects Plugin folder (You should have both ALS-Refactored and ALSXT side by side)
- Build your Solution in Visual Studio again
- Once compilation is successful you can now begin using ALSXT in your project

# Settings

Most Settings are set in the ALSXT Character Settings Data Assets in the Character Blueprint. This is where you would customize animations, sounds, parameters and toggle features. Components are configured similarly. Components may have some general settings that are editable when clicking on the Component inside the Character Blueprint.

# Contributions

I'm looking for help with implementing Root Motion for Vaulting, Wallrunning, Walljumping!
Any advice regarding making actions more GAS-friendly would be welcome!

If you wish to contribute, please ensure you are in sync with ALSXT, message me and I'll set up permissions for you. While I try to stay in sync with ALS-Refactored, at times it may be temporarily out of sync and features may break or need refactoring.
Please submit an issue for any Compile error fixes. Most are usually the result of ALSXT not being synced to ALS-R.

# License

Original content and code in this repository is under the MIT license. Any git submodules are covered by their respective licenses. Content listed in the Attributions are covered by their respective licenses.

# Attributions

- [Palm Trees](https://sketchfab.com/3d-models/palm-trees-55690379305145488e20afb05fc687e6) by [Erroratten](https://sketchfab.com/erroratten) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to be seperate meshes

- [LOOP surf on 100m sandbeach 01 170508_1135.wav](https://freesound.org/people/klankbeeld/sounds/392886/) by [klankbeeld](https://freesound.org/people/klankbeeld/) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to be shorter

- [Female Grunts For Games](https://freesound.org/people/SkyRaeVoicing/sounds/368843/) by [Sky Rae Voicing](https://freesound.org/people/SkyRaeVoicing/) [License: CC BY 3.0](https://creativecommons.org/licenses/by/3.0/) Modified to be seperate sounds

- [Tactical Knife](https://sketchfab.com/3d-models/tactical-knife-db381f4766cf453aa9b80822b0b95361) by [slimecent](https://sketchfab.com/slimecent) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to fix and bake materials

- [Tactical Axe 2](https://sketchfab.com/3d-models/tactical-axe2-e4fa55bab1d1433aaa8e8f563fd7ac05) by [TORI106](https://sketchfab.com/TORI106) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to optimize textures

- [Bench 02](https://sketchfab.com/3d-models/bench-02-0fc91c47c6b04717bb4400c247de573e) by [Ricardo Sanchez](https://sketchfab.com/380660711785) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/)

- [swipes and whooshes tshirt fast and short swings stereo ORTF 8040.wav](https://freesound.org/people/Soundholder/sounds/425856/) by [Soundholder](https://freesound.org/people/Soundholder/) [License: CC BY 3.0](https://creativecommons.org/licenses/by/3.0/) Modified to be seperate sounds

- [BACKPACK](https://sketchfab.com/3d-models/backpack-9ccadc2945c34826a0ce1c37de13675e) by [Idmental](https://sketchfab.com/idmental.id) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to optimize textures

- Vertical Foregrip from [Low-Poly Mk18 Mod0](https://sketchfab.com/3d-models/backpack-9ccadc2945c34826a0ce1c37de13675e) by [notcplkerry](https://sketchfab.com/notcplkerry) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to isolate and retexture model

- [Textured Suppressor - Free Download](https://sketchfab.com/3d-models/textured-suppressor-free-download-d176b5b1b03f4a78983698c6f7675023) by [RMT](https://sketchfab.com/rmt) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to optimize textures

- Angled Foregrip from [M4 - Stealth and Grenadier Configurations](https://sketchfab.com/3d-models/m4-stealth-and-grenadier-configurations-e530c3225f094a04ba33273c6ebb3d1c) by [LunarEclipse](https://sketchfab.com/e.hackett.03) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to isolate and retexture model

- Bushnell 1x22x33 from [low-poly Sight set](https://sketchfab.com/3d-models/low-poly-sight-set-2cdf0f52f1254ad3bc387bbd7e8d6223) by [D.U.](https://sketchfab.com/D.U.) [License: CC Attribution](https://creativecommons.org/licenses/by/4.0/) Modified to isolate and retexture model

# Special Thanks

[Sixze](https://github.com/Sixze)

[Freesound](https://freesound.org/)

[SketchFab](https://sketchfab.com/)

[Mixamo](https://www.mixamo.com)
