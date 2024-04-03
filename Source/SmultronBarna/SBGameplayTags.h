#pragma once

#include "NativeGameplayTags.h"

namespace SBGameplayTags
{
	SMULTRONBARNA_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags
	SMULTRONBARNA_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SMULTRONBARNA_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	SMULTRONBARNA_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	SMULTRONBARNA_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	SMULTRONBARNA_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	SMULTRONBARNA_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);
}
