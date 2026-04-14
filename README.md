<h1 align="center">ALSXT</h1>
<p align="center">
	<b>Seize the Means of Animation</b>
</p>

<p align="center">
<img src="ALSXT.png" alt="ALSXT">
</p>

<p align="center">
     <a href="https://github.com/Voidware-Prohibited/ALSXT/commits/master"><img src="https://img.shields.io/github/last-commit/Voidware-Prohibited/ALSXT.svg?logo=forgejo&logoColor=white" alt="Last commit"></a>&nbsp;
     <a href="https://github.com/Voidware-Prohibited/ALSXT/commits/master"><img src="https://img.shields.io/github/check-runs/Voidware-Prohibited/ALSXT/stable?logo=forgejo&logoColor=white&label=CI" alt="CI"></a>&nbsp;
     <a href="https://github.com/Voidware-Prohibited/ALSXT/commits/master"><img src="https://img.shields.io/github/checks-status/Voidware-Prohibited/ALSXT/stable?logo=forgejo&logoColor=white" alt="Checks Status"></a>&nbsp;
     <a href="https://github.com/Voidware-Prohibited/ALSXT/commits/master"><img src="https://img.shields.io/codecov/c/github/Voidware-Prohibited/ALSXT/stable?logo=codecov&logoColor=white" alt="Coverage"></a>&nbsp;
</p>
<p align="center">
     <a href="https://github.com/Voidware-Prohibited/ALSXT/issues"><img src="https://img.shields.io/github/issues-raw/Voidware-Prohibited/ALSXT.svg?logo=forgejo&logoColor=white" alt="Issues"></a>&nbsp;
     <a href="https://github.com/Voidware-Prohibited/ALSXT/pulls"><img src="https://img.shields.io/github/issues-pr-raw/Voidware-Prohibited/ALSXT.svg?logo=forgejo&logoColor=white" alt="Pull requests"></a>&nbsp;
     <a href="https://github.com/Voidware-Prohibited/ALSXT/blob/master/LICENSE"><img src="https://img.shields.io/badge/VOIDWARE%20Dual%20License-silver?style=flat&logo=unlicense&logoColor=white&label=License&color=white" alt="VOIDWARE Dual License"></a>
</p>
<p align="center">
     <a href="https://github.com/sponsors/colorindarkness"><img src="https://img.shields.io/github/sponsors/colorindarkness.svg?logo=github&logoColor=white" alt="Become a Sponsor"></a>&nbsp;
     <a href="https://www.patreon.com/colorindarkness"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Dcolorindarkness%26type%3Dpatrons" alt="Become a Patron"></a>&nbsp;
     <a href="https://ko-fi.com/colorindarkness"><img alt="Support me on Ko-fi" src="https://img.shields.io/badge/support_me_on-Ko--fi-red?link=https%3A%2F%2Fko-fi.com%2Fcolorindarkness"></a>&nbsp;
     <a href="https://liberapay.com/colorindarkness"><img alt="Support me on Liberapay" src="https://img.shields.io/badge/support_me_on-liberapay-yellow?link=https%3A%2F%2Fliberapay.com%2Fcolorindarkness%2F"></a>
</p>

> [!NOTE]
> Following the acquisition of GitHub by Micro$lop, we have taken principled action and have begun primarily hosting code and content on [Codeberg](https://codeberg.org/Voidware-Prohibited/). Mirrors will still be maintained on [GitHub](https://github.com/Voidware-Prohibited/ALSXT/) and [GitLab](https://gitlab.com/Voidware-Prohibited/ALSXT/).

ALSXT is a modular, GAS-based Unreal Engine 5 plugin that extends ALS-Refactored into a cutting-edge Character System. ALSXT empowers developers with AAA animation and features like Character Customization, runtime Mesh Painting, Gameplay Camera System, Combat, Impact Reactions, and advanced Firearms mechanics with Procedural Aim and Recoil, all streamlined into a rapid, data-driven workflow, ensuring stability without sacrificing flexibility. A part of the [Target Vector](https://github.com/Voidware-Prohibited/TargetVector/) framework.

[📚Wiki](../../wiki/) • [📘Documentation](https://alsxt.notion.site/2be59e501e7a4bf583437d1636bc7f2f?v=ac361eb5a6a14081892ff9a68e3e7a44/) • [📑ALSXT Notion](https://alsxt.notion.site/2be59e501e7a4bf583437d1636bc7f2f?v=ac361eb5a6a14081892ff9a68e3e7a44/) • [🐤Twitter](https://x.com/ALSXT/) • [🗨️Discord](discord://colorindarkness/)

# Introduction
- Generate truly unlimited, unique and immersive Characters with extensive States, deep Character Customization, Southpaw and support for blending multiple Locomotion and Overlay Layers.
- Create smooth third Person and True First Person Gameplay featuring Input Actions converted to Gameplay Abilities, governed by Attribute Sets and Gameplay Effects.
- Developer and Artist friendly, Data-oriented Locomotion framework with Real-world tested tech such as ALS-Refactored, GAS, Chooser Tables, Smart Objects and Modular Gamplay Features.

## Available Modular Game Features
- [Alsxt Dash Ability](https://codeberg.org/Voidware-Prohibited/AlsxtDashAbility/)

## Implementation Goals
- Empower Individuals and small independent Studios with AAA Feature Parity or better.
- Promote and Advance the Free and Open Source Software Movement.
- Utilize the latest Technologies and Methodologies.
- Maintain a Modular, Configurable and Data-oriented Design.
- Adhere to, promote and facilitate adherance to Coding Standards and Style Guides.
- Encourage and Facilitate the use of Versioning Systems and CI/CD.
- Performance and Optimization by Design.
- Do Cool Procedural Stuff.

# Features

> [!WARNING]
> _ALSXT is under heavy development, including a rewrite to utilze GAS, Modular features and new technologies. Many features may not be finished or production-ready. Use at your own risk._

## Improvements
- Alsxt Character Movement Component - Expanded Movement Featurees and GAS Integration
- Expanded Character States - Orthodox/Southpaw Stances, Locmotion Variants, Readiness, Weapon Ready Position, etc
- Expanded Footprints System - Independent foot states, Saturation, Material Transfer, Vertex Paint Support(Static Mesh only, In Progress)

## New Features
- Locomotion Variants - Default, Feminine, Masculine
- Locomotion Layer Blending - Injuries, Overlay-defined Locomotion
- Overlay Layer  Blending - Right, Left and Both Hands Layers
- Gameplay Camera System - Replaces ALS-Camera Component
	- Gameplay Camera System Director
	- Third and First Person Camera Rigs
- GAS Gameplay Abilities - With Cost Gameplay Effects
	- Nearly all existing Input Actions
	- Movement Ability that can modify Regen Magnitudes
	- Extended State Input Actions: Combat Stance, Readiness, Weapon Ready Position, Canting, Leaning, Hold Breath, Focus, Freelook
	- New Locomotion Actions: Slide, Flip
	- Plus more
- GAS Gameplay Effects
	- Staus Effects, such as Injuries, that modify Movement Speed Magnitudes and Locomotion animations
	- Stamina Regen with Magnitude
	- Hold Breath Regen with Magnitude
- GAS Gameplay Attribute Sets
	- Stamina
	- Breath
	- Plus more
- Advanced Customizable and Paintable Master Materials for Characters and Objects

### Firearm Features
- ADS (Aim Down Sight)
- Attachments
- Optic Effects
- Procedural Aim
- Procedural Recoil
- Firearm Actions
- Trigger Finger Layer

## Features In Development
- Smart Object based Generic Interaction System
- Contextual Animation
- Combat System with Targeting
- Impact Reactions System
- Physical Animation
- Character Customization
- Paintable Mesh Components
- Character Sounds
- Vaulting
- Idle Animation Component

## Bonus Features
- Advanced Landscape Auto-Material
- Procedural Radial/Pie Menu
- Workflow YAML files for Codeberg/Forgejo, GitHub, and GitLab

## Planned Features
- Skeletal Mesh and Clothing set with Morph Targets/Shape Keys
- Stationary Modes (Sitting, Vehicles etc)
- Wallrunning and Walljumping
- Prone Stance
- Swimming Custom Movement Mode
- Ladder Climbing Custom Movement Mode
- Gestures
- Emotes


# Requirements
- Unreal Engine 5.7 or later built from github Source Code
- A working UE5 C++ Integrated Developement Environment(IDE)
     - Tested: Jetbrains Rider, VS Code, Visual Studio (Windows)
     - Untested: XCode (MacOS), Jetbrains Rider, VS Code, Visual Studio (Linux)
- clang -20 13.0.1

## Engine Plugins
The following are built-in Engine plugins that are enabled in the Plugins window. Warning: some plugins may be Experimental or Beta software.
- [Gameplay Abilities](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/GameplayAbilities/)
- GameplayCameras
- GameplayTasks
- SmartObjectsModule
- GameplayBehaviorsModule
- GameplayBehaviorSmartObjectsModule
- AbilitySystemGameFeatureActions
- AIModule
- GameFeatures
- ModularGameplay
- ModularGameplayActors
- Chooser
- ContextualAnimation
- PhysicsControl
- DataRegistry
- TargetingSystem

## Third-Party Plugins
The following plugins require cloning or downloading into your projects `Plugins` folder.
- [Advanced Locomotion System Refactored](https://github.com/Sixze/ALS-Refactored/) ![GitHub Tag](https://img.shields.io/github/v/tag/Voidware-Prohibited/ALS-Refactored)

## Vertex Detection
To enable vertex detection in packaged builds (required for line tracing, procedural mesh, or runtime vertex inspection), you must enable Allow CPU Access on each mesh. This setting ensures vertex data is not stripped during cooking, allowing CPU-based functions to read vertex buffers at runtime.

For each Static Mesh:
- Open the Static Mesh in the Content Browser.
- In the Details panel, check "Allow CPU Access". This can also be performed in bulk with the Bulk Edit via Property Matrix command.
- Rebuild/Repackage: Save the meshes and repackage the project for the changes to take effect in the build.

## CI System Requirements
Systems performing Continous Intergration Workflows will have additional requirements depending on the CI solution used.
- Git
- OpenCppCoverage
- Domain Name or ngrok (For Self Hosted CI)
- Workflow Actions services and Runners configured for Forgejo/Codeberg, GitHub or GitLab
- Jenkins and Java 11 (For Jenkins)

# Installation
[Complete Installation Instructions](../..//wiki/Installation/)

**Quick Install**

1. Install and setup (if required) all dependencies
2. Clone/download ALSXT into your projects Plugin folder
3. Integrate Config\DefaultEngine.ini into your projects DefaultEngine.ini
	1. Note: Various `SurfaceTypes` and a new `CollisionChannel` named `Transferrable` are required for the Footprints and Impact Effects to work. SurfaceTypes can be renamed and/or re-assigned in the respective effects Settings
4. (Re)Build Project with the `BuildPlugin.bat` Script or your IDE
5. Once compilation is successful you can now begin using ALSXT in your project

## Available Scripts
- `./BuildPlugin.bat` - Build Plugin Batch Script
- `./BuildPlugin.sh` - Build Plugin Shell Script

# Getting Started
[Getting Started](../..//wiki/GettingStarted/)
Five(5) C++ Character classes and four(4) example Blueprint classes are provided, each with their respective feature-sets, defined by `AlsxtCharacterSettings`, `GameplaySystemInitalizationDataAsset` and any additional Actor Components Settings.

```
AlsxtCharacter						  # Abstract Base Character Class. No Input Actions. Not Blueprintable
├──AlsxtCharacterPlayer/				# Uses Player State. Ability System Component (ASC) is located on the Player State
    ├── AlsxtCharacterPlayerAdvanced/   # Uses Player State. Ability System Component (ASC) is located on the Player State
 ├──AlsxtCharacterNPC/				  # No Input Actions. Ability System Component (ASC) is located on the Character Class
    ├── AlsxtCharacterNPCAdvanced/	  # No Input Actions. Ability System Component (ASC) is located on the Character Class
```

Create a child C++ class or blueprint of any `AlsxtCharacter` subclass to create your own custom Character, or simply Copy and existing Blueprint class derived from any of these classes.

# Settings
ALSXT is designed to be extremely configurable by with Data Assets. Most [Settings](../..//wiki/Settings/) are centralized in the [ALSXTCharacter Settings](../..//wiki/ALSXTCharacterSettings/) Data Asset.

## Gameplay Ability System
Default [Character Abilities](/wiki/Settings/GameplayAbilities/) are defined by [Gameplay System Initialization Data](../..//wiki/Settings/GameplayAbilities/).

# CI
CI workflow YAML files are included for Forgejo/Codeberg, GitHub and GitLab. 

`ue-plugin-ci` Build, Test, Code Coverage, Upload to Codecov, Update Changelog, Update Status. Activated with any push.
`ue-plugin-ci-release` Build, Cook, Package, Test, Code Coverage, Upload to Codecov, Update Changelog, Update Status. Activated the a `v*` tag in the commit.
`changelog` Generates and commits CHANGELOG.md. Activated the a `v*` tag in the commit.


## Automated Testing
The provided tests can be run in an Action Workflow or run manually with RunUAT. Learn more about Unreal Engine Automated Testing in the [Run Automated Tests](https://dev.epicgames.com/documentation/unreal-engine/run-automation-tests-in-unreal-engine?lang=en-US) documentation.

- `AlsxtTest_BasicMovement`

## Code Coverage
OpenCodeCoverage will perform Code Coverage analysis and publish and XML file as a Commit Artifact, and can optionally be uploaded to a configured Codecov account. You will need to first generate an Access Token from your Codecov account and add it your Forgejo/Codeberg, GitHub or GitLab Secrets as `CODECOV_TOKEN`.

# Asset Source Files

- [ALSXT-Assets-Cascaduer](https://github.com/Voidware-Prohibited/ALSXT-Assets-Cascaduer/): Source Assets Cascaduer files
- [ALSXT-Assets-Blender](https://github.com/Voidware-Prohibited/ALSXT-Assets-Blender/): Source Assets Blender files
- [ALSXT-Assets-Photoshop](https://github.com/Voidware-Prohibited/ALSXT-Assets-Photoshop/): Source Assets Photoshop files
- [ALSXT-Assets-Textures](https://github.com/Voidware-Prohibited/ALSXT-Assets-Textures/): Source Assets Texture files
- [ALSXT-Assets-Audio](https://github.com/Voidware-Prohibited/ALSXT-Assets-Audio/): Source Assets Audio files

# Contributions

Contibutors and PRs are welcome! If you wish to contribute, please ensure you are synced with the latest ALSXT commit, message me and I'll set up permissions for you.

# License

This project is dual-licensed. It is available for absoultely free under the [MIT License](LICENSE.md) for most Individuals and Organizations. If your organization, industry or application is listed in the [Commercial License](LICENSE-Commercial.md), please [Contact Us](https://voidwarex.com/contact/).

Any git submodules are covered by their respective licenses. Content listed in the Attributions are covered by their respective licenses.

# Attributions
[ATTRIBUTIONS](ATTRIBUTIONS.md)

# Special Thanks

- [Sixze](https://github.com/Sixze/)
- [Freesound](https://freesound.org/)
- [SketchFab](https://sketchfab.com/)
- [Mixamo](https://www.mixamo.com/)
- [Joseph Thigpen, Narxim](https://github.com/Narxim/) for [Narxim-GAS-Example](https://github.com/Narxim/Narxim-GAS-Example/)
- [tranek](https://github.com/tranek/) for [GASDocumentation](https://github.com/tranek/GASDocumentation/)