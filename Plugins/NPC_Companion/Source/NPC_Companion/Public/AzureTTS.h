// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "AzureEnums.h"
#include "Components/AudioComponent.h"
#include "AzureTTS.generated.h"

UCLASS()
class NPC_COMPANION_API AAzureTTS : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAzureTTS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AzureTTS")
	void ConvertTextToSpeech(FString Text, EAzureVoiceGender VoiceGender, EAzureVoiceName VoiceName, EAzureVoiceLanguage VoiceLanguage);

	UFUNCTION(BlueprintCallable, Category="AzureTTS")
	void StopPlayback();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AzureTTS", meta=(AllowPrivateAccess))
	FString AzureAPIKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AzureTTS", meta=(AllowPrivateAccess))
	FString Region; //"example : uksouth, useast
	FString SSMLTemplate = TEXT("<speak version='1.0' xml:lang='%s'><voice xml:lang='%s' xml:gender='%s' name='%s'><mstts:express-as style='%s'>%s</mstts:express-as></voice></speak>");

	FString CachedSSML;
	void OnAccessTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnTTSResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UPROPERTY()
	UAudioComponent* CurrentAudioComponent;
	
	const TCHAR* GetVoiceName(EAzureVoiceName VoiceName);
	FString DetermineSpeakingStyle(FString& Text);
	void StripActionsFromText(FString& Text);
};
