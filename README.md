# Unreal NPC Companion Asset

## Introduction
This plugin enables developers to quickly integrate intelligent, GPT-powered NPCs with advanced features like dynamic memory, patrolling behaviors, and companion NPCs into Unreal Engine C++ projects.

### Features:
- Modular NPC blueprint.
- GPT-powered NPC brain.
- Text-to-Speech and Speech-to-Text features.
- Dynamic in-game reactions and memory.
- Companion NPC with follow mechanics.
- Patrolling NPCs.
- Locally saved individual NPC save files.

## Installation

### Prerequisites:
- Unreal Engine version 5.4 (with C++ project setup).
- OpenAI and Azure subscriptions.
- HttpGPT Plugin from Unreal Marketplace.
- Python installed on the system.

### Steps:

1. Ensure you have a C++ project with the **ThirdPersonTemplate** content. If not, resolve the blueprint dependencies.
2. Create a `Plugins` folder in your project directory (if not already present).
3. Paste the asset files into the folder.
4. Download and add the **HttpGPT plugin** files from the Unreal Marketplace.
5. Add the following modules to your `[ProjectName].Build.cs` file:
    ```csharp
    "AIModule",
    "GameplayTasks",
    "NavigationSystem",
    "HTTP",
    "AudioCapture",
    "Json",
    "JsonUtilities"
    ```
6. Compile your solution or regenerate project files.
7. Open the project and build the modules if prompted.
8. In the Unreal Editor, enable the custom plugin, HttpGPT, AudioCapture, and Python Editor Script Plugin.
9. Restart the Unreal Engine.

## Required Subscriptions

1. **OpenAI**: A paid subscription is required for GPT integration. Find your API key and paste it in the `BP_NPCManager` OpenAI API Key field.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/1.png)

2. **Azure**: Sign up for Azure Speech Studio (free plan offers 5 hours per month). Retrieve the API key and region, then add them to the `BPAzureTTS` blueprint.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/2.png)

3. **Azure Setup**: Edit the `transcribe_audio.py` script found in the plugin's Python folder to include your API key and region.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/3.png)

Ensure you're using `GM_Base` as the game mode, or derive your own from this class.

## How to Use

A demo level named `Demo` is included to showcase the plugin.

### Overview:
- There are two types of NPCs: **World** NPCs and **Companion** NPCs.
- Only one companion can exist per level. Additional NPCs act as **WorldNPCs**.
- NPCs are controlled via prompts sent to ChatGPT, which can be modified in the `BP_NPC_Base` blueprint.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/4.png)

### Main Blueprints:
- Use either `WorldNPC` or `CompanionNPC` blueprints in your level. Simply drag and drop them into the scene.
- AI details can be modified via the details panel.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/5.png)

- Both NPCs support patrolling behaviours. Set patrol points after placing them in the level.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/6.png)

- The default Pawn Class is `BP_ThirdPersonCharacterNPC`. If you're using a custom class, either inherit from this one or copy the necessary functions and events.
  
### Saving Data:
- All NPC and World data is saved locally in the project’s saved directory.
- To delete save files, use the `EUWBP_NPCManager` editor blueprint.

## Visuals

- To modify the NPC's appearance, change the skeletal mesh. Ensure compatibility with the Unreal animations used in the `ABP_Manny_NPC` animation blueprint.
-  It uses the ABP_Manny_NPC animation blueprint, with an additional boolean
 “bIsRotating” and the following logic, should you need to add it to a new one.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/7.png)

- **Idle Animations**: The plugin includes 3 idle animations, which are managed by the behaviour trees.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/8.png)

- Customize behaviour trees as needed, using the **Base_AIController** to control them.

## Dynamic Events

- Dynamically update NPC knowledge in real-time by calling the **Dynamic Event** function.
- Example usage:
    ```csharp
    GetGameMode()
    CastToGM_Base()
    GetNPCManager()
    CallDynamicEvent("Event Description")
    ```
- This allows NPCs to respond to in-game events dynamically.

![](https://github.com/Afterlife1707/NPC-Plugin-Project/blob/main/Documentation%20Pics/9.png)

