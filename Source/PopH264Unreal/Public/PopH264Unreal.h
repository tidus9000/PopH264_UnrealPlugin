// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/UniquePtr.h"
#include "ImportPopH264dll.h"

POPH264UNREAL_API DECLARE_LOG_CATEGORY_EXTERN(PopH264, Log, All);


class FPopH264DecoderInstance;


class FPopH264UnrealModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	ImportPopH264dll instance;

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle = nullptr;
};


class PopH264FrameMeta_t
{
public:
	size_t	FrameNumber = 0;
	int32_t ImageHeight = 0;
	int32_t ImageWidth = 0;
	int32_t ImageRect[4] = {0,0,0,0};
};

class POPH264UNREAL_API FPopH264DecoderInstance
{
public:
	//	gr: this is mostly just a helper for implementation reference
	static TSharedPtr<FPopH264DecoderInstance>	AllocDecoder();

public:
	FPopH264DecoderInstance();
	~FPopH264DecoderInstance();

	void					PushH264Data(const TArray<uint8_t>& H264Data,size_t FrameNumber);
	bool					PushTestData(const char* TestDataName,size_t FrameNumber);	//	returns false if invalid test data, or failed to push


	//	gr: must be called on game thread
	//	gr: Not sure if it's wise to just pass raw UTexture2D* pointers around, apparently if they're assigned to
	//		UPROPERTY members, they wont get garbage collected, but... when MIGHT they get garbage collected?
	//		should this return shared_ptr's ?
	void					PeekFrame(PopH264FrameMeta_t& Meta);
	TArray<UTexture2D*>		PopFrame(PopH264FrameMeta_t& Meta);
	TArray<UTexture2D*>		PopFrame(PopH264FrameMeta_t& Meta, TArray<UTexture2D*>& ExistingTextures);	
	static void				FillTextures(TArray<UTexture2D*>& ExistingTextures, uint8_t Value = 0xff);

private:
	uint32_t		mInstanceHandle = 0;	//	allocated by dll
};
