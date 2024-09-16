#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAzureVoiceGender : uint8
{
	Male    UMETA(DisplayName = "Male"),
	Female  UMETA(DisplayName = "Female"),
};
UENUM(BlueprintType)
enum class EAzureVoiceName : uint8
{
	AriaNeural     UMETA(DisplayName = "AriaNeural"),
	GuyNeural      UMETA(DisplayName = "GuyNeural"),
	JennyNeural    UMETA(DisplayName = "JennyNeural"),
	DavisNeural    UMETA(DisplayName = "DavisNeural"),
};
UENUM(BlueprintType)
enum class EAzureVoiceLanguage : uint8
{
	EN_US  UMETA(DisplayName = "en-US"),
	EN_GB  UMETA(DisplayName = "en-GB"),
};

UENUM(BlueprintType)
enum class EAzureSpeakingStyle : uint8
{
	Default     UMETA(DisplayName = "Default"),
	Cheerful    UMETA(DisplayName = "Cheerful"),
	Empathetic  UMETA(DisplayName = "Empathetic"),
	Sad         UMETA(DisplayName = "Sad"),
	Excited     UMETA(DisplayName = "Excited"),
	Angry       UMETA(DisplayName = "Angry"),
};
