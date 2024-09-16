#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileUtilityLibrary.generated.h"

/**
 * A utility class to handle file operations from Blueprints.
 */
UCLASS()
class NPC_COMPANION_API UFileUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Deletes files with a specific extension in a given folder.
	 * 
	 * @param FolderPath The path to the folder where files should be deleted.
	 * @param FileExtension The extension of the files to delete (e.g., ".txt").
	 * @return Returns true if the operation was successful, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, Category="File Operations")
	static bool DeleteFilesInFolder(const FString& FolderPath, const FString& FileExtension);
	static bool LogAllFilesInFolder(const FString& FolderPath);
};
