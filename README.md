Unreal NPC Companion Asset
 Introduction
 This plugin enables developers to quickly integrate intelligent, GPT-powered NPCs with
 advanced features like dynamic memory, patrolling behaviours, and companion NPCs into
 Unreal Engine C++ projects.
 Features :
 ● Modular NPCblueprint.
 ● GPT-powered NPC brain.
 ● Text-to-Speech and Speech-to-Text features.
 ● Dynamic in-game reactions and memory.
 ● Companion NPCwith follow mechanics.
 ● Patrolling NPCs.
 ● Locally saved individual NPC save files
 Installation
 Prerequisites:
 ● Unreal Engine version 5.4 (with C++ project setup).
 ● OpenAIandAzure subscriptions
 ● HttpGPT Plugin from Unreal Marketplace.
 ● Python is installed on the system
 Steps:
 Setting up the project:
 ● Makesureyou have a C++ project, with the ThirdPersonTemplate content. If not you’ll
 have to resolve some blueprint dependencies.
 ● Makeaplugins folder, if not already present, named “Plugins” in your project folder.
● Pastethe asset files in the folder.
 ● Download and paste the HttpGPT plugin files as well, from the marketplace.
 ● Addthefollowing modules to your [ProjectName].Build.cs file :
 "AIModule",
 "GameplayTasks",
 "NavigationSystem",
 "HTTP",
 "AudioCapture",
 "Json",
 “JsonUtilities"
 ● Compile your solution, or regenerate project files.
 ● Opentheproject, if it asks to build modules, select yes.
 ● Opentheplugins window, and enable the custom plugin, HttpGPT, AudioCapture and
 Python Editor Script Plugin.
 ● Restart the engine.
 Required Subscriptions
 ● AnOpenAIpaid subscription is required for the GPT part. Once you have a membership,
 you’ll find your API Key. Copy and paste that key, in the BP_NPCManager’s OpenAI API
 Key field. You can also change the Game World and Common Knowledge details
 here.
 ● Next is an Azure speech studio membership, which can be free, and limited to 5 free
 hours a month. You will find an Azure API key as well once you have signed up and
 created a speech service.
 ● Pastethat key in the BPAzureTTS blueprint’s Azure API key, and also type the region
 name(this can also be found in the Azure Dashboard, for example, uksouth).
● Onemorestep is required in the Azure setup. Go to the plugins folder, which would be
 something like “YourProject\Plugins\NPC_Companion\Content\NPC\Python”, in the
 Python folder, you’ll find a script called “transcribe_audio.py”.
 ● Openthescript, and pase your APIKey and Region again
 ● Always use the GM_Base as the game mode, or you can derive your own game mode
 from this class.
 All done!
Howto Use
 A demo level is available demonstrating the plugin called “Demo” for reference.
 Overview:
 ● Thereare 2 types of NPCs available: World and Companion.
 ● Therecan be only 1 companion in the level.
 ● If there are more than 1, only the first one is registered as the companion, and others act
 as WorldNPCs.
 ● TheNPCsaresetup using prompts to the AI, which are sent to ChatGPT.
 ● Theseprompts can be modified and can be found in the BP_NPC_Base blueprint.
 ● Youcanchange these prompts according to your needs:
The Main Blueprints
 ● Youcanuseeither the WorldNPC or the CompanionNPC blueprints in your level,
 based on your needs. Simply drag and drop any of these in the level.
 ● TheAIdetails can be changed in the details panel, and searching for “AI”
 ● Theyboth have patrolling behaviours, if required, and can be enabled by adding patrol
 points after placing them in the level.
 ● Bydefault, the GM_Base game mode uses the BP_ThirdPersonCharacterNPC as the
 default Pawn Class.
 ● ThisBPhascertain functionalities which are essential for the plugin, so if you have your
 own character class, make a child of this one, or copy all the functions and events to
 your character BP.
 ● All the NPC and World data is saved locally, in the project saved directory.
 ● Thefiles can be deleted from the editor by using an editor blueprint,
 “EUWBP_NPCManager”. Right-click on it and select run.
Visuals
 ● TheNPC’sappearance can be changed by changing the skeletal mesh, but this might
 break some of the logic(like looking at the NPC, or the NPC looking at the player, when
 in range) if the new mesh is not supported by Unreal.
 ● Whenchanging the skeletal mesh, ensure the new mesh supports the default Unreal
 animations used in the NPC’s animation blueprint (ABP_Manny_NPC). If you encounter
 compatibility issues, consider re-creating animations for your custom mesh.
 ● Ituses the ABP_Manny_NPC animation blueprint, with an additional boolean
 “bIsRotating” and the following logic, should you need to add it to a new one.
 ● Itcomes with 3 idle animations, which are played by the behaviour trees.
 ● Youcancustomize the behaviour trees if you want as well.
 ● Theyusethe AI Controller: Base_AIController which sets up the behaviour trees and
 runs them.
Dynamic Events
 ● Oneof thekey features of this plugin.
 ● Ifyou want to dynamically update the NPC knowledge, and inform them of something
 that happened in the game, while the game is running, and give responses with that in
 mind, you can use dynamic events.
 ● Simply get the game mode, cast it to GM_Base, get the NPCManager from it, and call
 Dynamic Event.
 ● Addtheevent description and that's it. All the NPCs in the game will be aware of this
 event.
