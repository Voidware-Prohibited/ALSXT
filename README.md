<h1 align="center">ALSXT</h1>

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
</p>

<p align="center">
    <a href="https://github.com/sponsors/colorindarkness">
    <img src="https://img.shields.io/github/sponsors/colorindarkness.svg?style=flat-square&logo=github&logoColor=white"
         alt="Become a Sponsor">
    </a>
    <a href="https://www.patreon.com/colorindarkness">
    <img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Dcolorindarkness%26type%3Dpatrons&style=flat"
         alt="Become a Patron">
    </a>
    <a href="https://ko-fi.com/colorindarkness">
     <img alt="Support me on Ko-fi" src="https://img.shields.io/badge/support_me_on-Ko--fi-red?link=https%3A%2F%2Fko-fi.com%2Fcolorindarkness">
    </a>
    <a href="https://ko-fi.com/colorindarkness">
     <img alt="Support me on Liberapay" src="https://img.shields.io/badge/support_me_on-liberapay-yellow?link=https%3A%2F%2Fliberapay.com%2Fcolorindarkness%2F">
    </a>
</p>

An Unreal Engine 5 plugin that extends ALS-Refactored with a set of popular replicated gameplay features, to demonstrate the extendability of ALS-Refactored in a Modular, Data-Driven, UE5 Way. The feature-set is informed by First and Third Person Tactical Shooters, Action Shooters and the parent project [Target Vector](https://github.com/Voidware-Prohibited/TargetVector).

_ALSXT is under heavy development, many features may not be finished or production-ready_.

_[Contibutors](#Contributions) and PRs are welcome_.

Source Assets of the Meshes, Textures, Sounds and Cascaduer files.

# Introduction

Two example Character classes are provided `ALSXTCharacter` and `ALSXTCharacterAdvanced`, each with their respective feature-sets. You can create a sub-class or blueprint of either class or simply use `ALSXTCharacterAdvanced` as an example for building your own custom class from the base `ALSXTCharacter` class.

**Features**

ALSXT

- Expanded Character States (Combat Stance, Sex, Variant, Weapon Ready Position)
- Sliding Locomotion Action (with Sound, Decal and Particle Support)
- Expanded Footprints System (Independent foot states, Saturation, Material Transfer, Vertex Paint Support(Static Mesh only, In Progress))
- Customization Component _In Progress_
- Paintable Skeletal Mesh Component _In Progress_
- Combat Component _In Progress_
- Impact Reactions (Blending reaction animations with Physical Animation) _In Progress_
- Character Sound Component_In Progress_
- Idle Animation Component _In Progress_
- Vaulting _In Progress_
- Physical Animation _In Progress_
- Sound Component _In Progress_
- Idle Animation Component _In Progress_
- Gesture _In Progress_
- Emotes _In Progress_
- Basic Stats Component (Stamina and Health to demonstate the functionality of various components)

ALSXT Advanced

- Combat
- ADS
- Procedural Recoil _In Progress_
- Reloading _In Progress_
- Camera FX Component _In Progress_
- Acrobatic Actions (Flipping, Wallrunning etc) _In Progress_


**Planned Features**
- New Skeletal Mesh and Clothing set with Morph Targets/Shape Keys
- Stationary Modes (Sitting, Vehilcles etc)
- Prone Stance
- Swimming Custom Movement Mode
- Ladder Climbing Custom Movement Mode

**Dependencies**

- [Advanced Locomotion System Refactored](https://github.com/Sixze/ALS-Refactored/)

**Integrations**

- [Procedural Recoil Animation System](https://www.fab.com/listings/c664053f-900b-4f33-aa26-4bdb46959e4c)

**Important Note**

ALSXT is not automatically synced with the main ALS-Refactored repository, but is instead maintained through [this fork](https://github.com/Voidware-Prohibited/ALS-Refactored). If you find changes in ALS-Refactored have broken ALSXT you may use this fork to maintain more stability in your projects.

# Installation

- Begin with an empty C++ Project
- Install and setup Enhanced Input Plugin
- Install and setup MetaSound Plugin
- Install the latest version of ALS-Refactored via https://github.com/Sixze/ALS-Refactored#quick-start
- Clone/download ALSXT into your projects Plugin folder (You should have both ALS-Refactored and ALSXT side by side)
- Integrate Config\DefaultEngine.ini into your projects DefaultEngine.ini
     - Note: Various `SurfaceTypes` and a new `CollisionChannel` named `Transferrable` are required for the Footprints and Impact Effects to work. SurfaceTypes can be renamed and/or re-assigned in the respective effects Settings.
- Build your Solution in Visual Studio or Rider again
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

# Special Thanks

[Sixze](https://github.com/Sixze)

[Freesound](https://freesound.org/)

[SketchFab](https://sketchfab.com/)

[Mixamo](https://www.mixamo.com)
