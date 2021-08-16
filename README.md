# Ubisoft NEXT 2021 Submission - Tower Defense Game

## Introduction
This is my submission to the Ubisoft Toronto NEXT Programming Challenge 2021 (https://toronto.ubisoft.com/next/). The organizers distributed a simple C++ API that handled rendering, input, and so on. The API was released ahead of the competition so participants could prepare. When the competition began, we were given 48 hours to make a game within the framework to fit a theme. The theme was "tower defense". A demonstration of the game with my commentary can be found here: https://www.youtube.com/watch?v=vcNgfW8QUAI

This repository contains the orignial API, the ECS (entity-component system) framework that I built on top of it, and then my actual tower defense game itself.
Some basic documentation detailing more of the game's design can be found in 'Tower Defense Game Documentation.pdf' in the root folder.

## Code Overview
The files listed below can be found by navigating to NEXTAPI2020/GameTest/

### The ECS
I built an ECS to serve as a solid but flexible foundation for the game logic. It is split into a few important files
- Entity.h/.cpp -> basically an identifier for the object
- Component.h/.cpp -> abstract class, used as the base for all component types in the game
    - All component subclasses are named with the 'C' prefix (ie: CBoxCollider, CHealth, etc)
- Registry.h/.cpp -> the central list of all entities and the components that they relate to

### Scene Management
I wrote a scene management system that allows for smooth and efficient swapping between the main menu and the different levels
- Scene.h/.cpp -> abstract class, used as the base for all scenes in the game
     - All scene subclasses are named with the 'S' prefix (ie: SMenu, SGame, etc)
- SceneManager.h/.cpp -> responsible for swapping between scenes and cleaning up memory as needed when doing so
- SGame.h/.cpp -> contains much of the game's logic
- LevelLoader.h/.cpp -> parses the different level layouts and uses it to generate them in-game

### Enemies
- WaveSpawner.h/.cpp -> Where enemy waves are defined
- EnemyFactory.h/.cpp -> Enemies are constructed and given unique behaviours by assembling various components together

## How To Run
- There is an executable for the game contained in NEXTAPI2020/x64/Executable. Double click 'GameTest.exe' to run it.
- Alternatively, navigate to NEXTAPI2020/ and run the GameTest.sln to open the project in Visual Studio
