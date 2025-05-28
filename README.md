# RPG Dialog System for Unreal Engine  
Dialog system with branching, conditions and events for single-player games  

## Table of Contents  
- [Features](#features)  
- [Requirements](#requirements)  
- [Installation](#installation)  
- [Quick Start](#quick-start)  
- [Documentation](#documentation)  
- [Debugging](#debugging)  

## Features  
- Flexible dialog trees with node types: Start, Work, Transfer, Finish  
- Conditions and events for NPCs and player responses  
- UMG widget integration for dialog UI  
- Helper functions for player management  

## Requirements  
- Unreal Engine 5.0 or later  
- UMG module enabled  

## Installation  
1. Copy the RPG_DialogSystem folder to YourProject/Source/  
2. Rebuild your project  
3. Add the dependency to YourProject.Build.cs  

## Quick Start  

### 1. Creating Dialog Object  
Create a child class of URPG_DialogObjectBase and add nodes using the editor.  

### 2. Configuring Nodes  
In Work nodes:  
- Add NPC dialog text  
- Add player response options  
- Set up conditions and events  

### 3. Actor Integration  
1. Add URPG_DialogComponentBase to your NPC  
2. Call RunDialog() to start conversation  

## Documentation  

### Key Classes  
| Class | Description |  
|-------|-------------|  
| URPG_DialogObjectBase | Base dialog object |  
| URPG_DialogNodeWork | Node with dialog options |  
| URPG_DialogWidget | UI widget implementation |  
| URPG_DialogFunctionLibrary | Helper utilities |  

### Editor
  
- Creating a dialog asset in the editor

![image](https://github.com/user-attachments/assets/75dc8e22-2841-469e-9e42-1ebaf6b3d0a6)  

- Example of a dialog object's viewport editor

![image](https://github.com/user-attachments/assets/50b52a0b-25f6-4243-a014-a3991cb21a9b)


## Debugging  
Enable logging with these console commands:  
- DialogSystem.ShowLog 1  
- DialogSystem.ShowDialogData 1  

## 📫 Other <a name="Other"></a>
:bangbang: Attention: If you can improve my trash code then make a pull request.
