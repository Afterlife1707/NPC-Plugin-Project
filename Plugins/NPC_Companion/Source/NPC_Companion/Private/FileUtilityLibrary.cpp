// Fill out your copyright notice in the Description page of Project Settings.


#include "FileUtilityLibrary.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

bool UFileUtilityLibrary::DeleteFilesInFolder(const FString& RelativeFolderPath, const FString& FileExtension)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Resolve the relative path to an absolute path
	FString FullFolderPath = FPaths::ProjectDir() + RelativeFolderPath;

	// Ensure the folder path ends with a forward slash
	if (!FullFolderPath.EndsWith("/"))
	{
		FullFolderPath.Append("/");
	}

	UE_LOG(LogTemp, Warning, TEXT("Resolved full folder path: %s"), *FullFolderPath);

	FString CorrectedFileExtension = FileExtension.StartsWith(".") ? FileExtension : "." + FileExtension;

	UE_LOG(LogTemp, Warning, TEXT("File extension: %s"), *CorrectedFileExtension);

	// Find files in the folder using the full path
	TArray<FString> FoundFiles;
	FString SearchPattern = FullFolderPath + "*" + CorrectedFileExtension;
	IFileManager::Get().FindFiles(FoundFiles, *SearchPattern, true, false);

	UE_LOG(LogTemp, Warning, TEXT("Found %d files to delete"), FoundFiles.Num());

	bool bAllFilesDeleted = true;
	for (const FString& File : FoundFiles)
	{
		FString FullFilePath = FullFolderPath + File;
		UE_LOG(LogTemp, Warning, TEXT("Attempting to delete file: %s"), *FullFilePath);

		if (!PlatformFile.DeleteFile(*FullFilePath))
		{
			bAllFilesDeleted = false;
			UE_LOG(LogTemp, Warning, TEXT("Failed to delete file: %s"), *FullFilePath);
		}
	}

	return bAllFilesDeleted;
}
