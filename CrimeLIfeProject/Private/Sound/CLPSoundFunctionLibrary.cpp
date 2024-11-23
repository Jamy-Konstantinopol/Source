// Crime Life Project, All Rights NOT Reserved.

#include "Sound/CLPSoundFunctionLibrary.h"

#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPSoundFunctionLibrary, All, All);

void UCLPSoundFunctionLibrary::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (SoundClass)
	{
		SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.f, 1.f);
		UE_LOG(LogCLPSoundFunctionLibrary, Display, TEXT("Sound volume %s: %f"), *SoundClass->GetName(), SoundClass->Properties.Volume);
	}
}

void UCLPSoundFunctionLibrary::ToggleSoundClassVolume(USoundClass* SoundClass)
{

	const bool NextVolume = SoundClass->Properties.Volume > 0.f ? 0.f : 1.f;
	SetSoundClassVolume(SoundClass, NextVolume);
}
