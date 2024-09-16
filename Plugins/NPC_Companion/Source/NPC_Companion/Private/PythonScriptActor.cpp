#include "PythonScriptActor.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"
#include "Misc/Paths.h"

APythonScriptActor::APythonScriptActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APythonScriptActor::ExecutePythonScript()
{
    FString PluginFolderName = TEXT("NPC_Companion");
    
    UE_LOG(LogTemp, Error, TEXT("Plugin location: %s"), *FPaths::ProjectPluginsDir());
    FString PythonExecutable = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("NPC_Companion/Content/Python3/Win64/python.exe"));
    FString PythonScriptPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("NPC_Companion/Content/NPC/Python/transcribe_audio.py"));

    FString InputFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("BouncedWavFiles/PlayerRecording.wav"));
    FString OutputFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("BouncedWavFiles/PlayerRecording_converted.wav"));

    // Construct the command line arguments for executing the Python script
    FString CommandLineArguments = FString::Printf(TEXT("\"%s\" \"%s\" \"%s\""), 
                                                   *PythonScriptPath, 
                                                   *InputFilePath, 
                                                   *OutputFilePath);

    int32 ReturnCode;
    FString StdOut;
    FString StdErr;
    FPlatformProcess::ExecProcess(*PythonExecutable, *CommandLineArguments, &ReturnCode, &StdOut, &StdErr);

    if (ReturnCode == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Python Output: %s"), *StdOut);
        PythonScriptOutput = StdOut;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Python Errors: %s"), *StdErr);
        PythonScriptOutput = TEXT("Error occurred while running the Python script.");
    }
}

// Function to return Python script output
FString APythonScriptActor::GetPythonScriptOutput() const
{
    return PythonScriptOutput;
}
