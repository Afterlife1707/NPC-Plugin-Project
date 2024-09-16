// Fill out your copyright notice in the Description page of Project Settings.


#include "AzureTTS.h"
#include "HttpModule.h"
#include "Engine/World.h"
#include "Interfaces/IHttpResponse.h"
#include "Sound/SoundWaveProcedural.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAzureTTS::AAzureTTS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAzureTTS::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAzureTTS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAzureTTS::ConvertTextToSpeech(FString Text, EAzureVoiceGender VoiceGender, EAzureVoiceName VoiceName, EAzureVoiceLanguage VoiceLanguage)
{
    StripActionsFromText(Text);
    // Determine speaking style based on text content
    FString StyleStr = DetermineSpeakingStyle(Text);

    // Convert enums to strings for SSML
    FString GenderStr = (VoiceGender == EAzureVoiceGender::Male) ? TEXT("Male") : TEXT("Female");
    FString VoiceNameStr = GetVoiceName(VoiceName);
    FString LanguageStr = (VoiceLanguage == EAzureVoiceLanguage::EN_US) ? TEXT("en-US") : TEXT("en-GB");

    // Construct SSML string using enum values
    CachedSSML = FString::Printf(TEXT("<speak version='1.0' xmlns:mstts='http://www.w3.org/2001/mstts' xml:lang='%s'>"
       "<voice xml:lang='%s' xml:gender='%s' name='%s'>"
       "<mstts:express-as style='%s'>%s</mstts:express-as>"
       "</voice></speak>"), *LanguageStr, *LanguageStr, *GenderStr, *VoiceNameStr, *StyleStr, *Text);
    
    // Set up the HTTP request for access token
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    FString URL = FString::Printf(TEXT("https://%s.api.cognitive.microsoft.com/sts/v1.0/issuetoken"), *Region);
    Request->SetURL(URL);
    Request->SetVerb("POST");
    Request->SetHeader("Ocp-Apim-Subscription-Key", AzureAPIKey);

    UE_LOG(LogTemp, Warning, TEXT("Request Sent"));
    Request->OnProcessRequestComplete().BindUObject(this, &AAzureTTS::OnAccessTokenResponse);
    Request->ProcessRequest();
}

void AAzureTTS::StopPlayback()
{
    if (CurrentAudioComponent && CurrentAudioComponent->IsPlaying())
    {
        CurrentAudioComponent->Stop();
        UE_LOG(LogTemp, Warning, TEXT("Stopped previous sound"));
    }
}

void AAzureTTS::OnAccessTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Access Token Response received"));
        FString AccessToken = Response->GetContentAsString();
        FString SynthesizeURL = FString::Printf(TEXT("https://%s.tts.speech.microsoft.com/cognitiveservices/v1"),*Region);

        // Set up the HTTP request for TTS
        TSharedRef<IHttpRequest> TTSRequest = FHttpModule::Get().CreateRequest();
        TTSRequest->SetURL(SynthesizeURL);
        TTSRequest->SetVerb("POST");
        TTSRequest->SetHeader("Content-Type", "application/ssml+xml");
        TTSRequest->SetHeader("Authorization", "Bearer " + AccessToken);
        TTSRequest->SetHeader("X-Microsoft-OutputFormat", "riff-16khz-16bit-mono-pcm");

        TTSRequest->SetContentAsString(CachedSSML);

        UE_LOG(LogTemp, Warning, TEXT("TTS Request Sent"));
        TTSRequest->OnProcessRequestComplete().BindUObject(this, &AAzureTTS::OnTTSResponse);
        TTSRequest->ProcessRequest();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get access token from Azure"));
        if (Response.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("Error: %s"), *Response->GetContentAsString());
        }
    }
}

void AAzureTTS::OnTTSResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("TTS Response valid"));

        // Create a SoundWave from the audio data
        TArray<uint8> AudioData = Response->GetContent();
        UE_LOG(LogTemp, Warning, TEXT("AudioData size: %d"), AudioData.Num());

        if (AudioData.Num() > 0)
        {
            USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
            SoundWave->SetSampleRate(16000);
            SoundWave->NumChannels = 1;
            SoundWave->Duration = static_cast<float>(AudioData.Num()) / (16000.0f * sizeof(int16));

            // Check SoundWave properties before queuing audio
            UE_LOG(LogTemp, Warning, TEXT("SoundWave properties - SampleRate: %f, NumChannels: %d, Duration: %f"), 
                SoundWave->GetSampleRateForCurrentPlatform(), SoundWave->NumChannels, SoundWave->Duration);

            // Queue the audio data
            SoundWave->QueueAudio(AudioData.GetData(), AudioData.Num());
            UE_LOG(LogTemp, Warning, TEXT("Audio queued"));

            // Stop the previous sound if it is still playing
            if (CurrentAudioComponent && CurrentAudioComponent->IsPlaying())
            {
                CurrentAudioComponent->Stop();
                UE_LOG(LogTemp, Warning, TEXT("Stopped previous sound"));
            }

            // Play the new sound
            CurrentAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundWave);
            UE_LOG(LogTemp, Warning, TEXT("Sound played"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AudioData is empty"));
        }
    }
    else
    {
        FString ErrorMessage = Response.IsValid() ? Response->GetContentAsString() : TEXT("Invalid response");
        UE_LOG(LogTemp, Error, TEXT("Failed to convert text to speech. Error: %s"), *ErrorMessage);
    }
}


const TCHAR* AAzureTTS::GetVoiceName(EAzureVoiceName VoiceName)
{
    switch (VoiceName)
    {
    case EAzureVoiceName::AriaNeural:
        return TEXT("en-US-AriaNeural");
    case EAzureVoiceName::GuyNeural:
        return TEXT("en-US-GuyNeural");
    case EAzureVoiceName::JennyNeural:
        return TEXT("en-US-JennyNeural");
    case EAzureVoiceName::DavisNeural:
        return TEXT("en-US-DavisNeural");
    default:
        return TEXT("en-US-AriaNeural");
    }
}

FString AAzureTTS::DetermineSpeakingStyle(FString& Text)
{
    int32 OpenBracketIndex = Text.Find(TEXT("("), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
    int32 CloseBracketIndex = Text.Find(TEXT(")"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

    if (OpenBracketIndex != INDEX_NONE && CloseBracketIndex != INDEX_NONE && CloseBracketIndex > OpenBracketIndex)
    {
        FString EmotionKeyword = Text.Mid(OpenBracketIndex + 1, CloseBracketIndex - OpenBracketIndex - 1);
        Text.RemoveAt(OpenBracketIndex, CloseBracketIndex - OpenBracketIndex + 1);

        return EmotionKeyword;
    }

    return TEXT("default");
}

void AAzureTTS::StripActionsFromText(FString& Text)
{
    int32 StartIndex = 0;
    while ((StartIndex = Text.Find(TEXT("*"), ESearchCase::IgnoreCase, ESearchDir::FromStart)) != INDEX_NONE)
    {
        int32 EndIndex = Text.Find(TEXT("*"), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex + 1);
        if (EndIndex != INDEX_NONE)
            Text.RemoveAt(StartIndex, EndIndex - StartIndex + 1);
        else
            break;
    }
}



