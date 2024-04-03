// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "SBAbility.h"
#include "UObject/Object.h"
#include "SBAbility_Jump.generated.h"

/**
 * Only used by ASBCharacters
 */
UCLASS()
class SMULTRONBARNA_API USBAbility_Jump : public USBAbility
{
	GENERATED_BODY()

	virtual bool CanActivateAbility	( ASBCharacter& Character ) override;
	virtual void ActivateAbility	( AActor* Instigator ) override;
	virtual void EndAbility			( AActor* Instigator ) override;
	
	UFUNCTION( BlueprintCallable )
	void AddJump( int Num );

protected:
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Ability|Jump" )	float FirstJumpHeight;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Ability|Jump" )	float MultipleJumpHeight;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Ability|Jump" )	int MaxJumps;
	UPROPERTY( BlueprintReadWrite, Category = "Ability|Jump")					int NumJumps;
};
