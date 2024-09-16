// PythonScriptActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PythonScriptActor.generated.h"

UCLASS()
class NPC_COMPANION_API APythonScriptActor : public AActor
{
	GENERATED_BODY()

public:	
	APythonScriptActor();

	UFUNCTION(BlueprintCallable, Category = "Python")
	void ExecutePythonScript();
	UFUNCTION(BlueprintCallable, Category = "Python")
	FString GetPythonScriptOutput() const;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Python", meta = (AllowPrivateAccess = "true"))
	FString PythonScriptOutput;
};
