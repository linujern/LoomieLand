// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "SBAbility.h"
#include "Engine/EngineTypes.h"
#include "SBAbility_Glide.generated.h"

class ASBCharacter;
/**
 * Only used by 
 */
UCLASS()
class SMULTRONBARNA_API USBAbility_Glide : public USBAbility
{
	GENERATED_BODY()

public:

	virtual bool CanActivateAbility	( ASBCharacter& Character ) override;

	virtual void ActivateAbility	( AActor* Instigator ) override;
	virtual void EndAbility			( AActor* Instigator ) override;

	void Timer();

protected:

	// Air control properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterMovement")
	float GlideControl;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterMovement")
	float JumpControl;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterMovement" )
	float DescentRate;
	
	UPROPERTY(BlueprintReadWrite, Category = "CharacterMovement")
	float GlideTime;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterMovement" )
	float GlideMaxTime;

	FTimerHandle TimerHandle;
};
