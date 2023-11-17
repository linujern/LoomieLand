// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "SBAbility.generated.h"

class ASBCharacter;

/**
 * This class should only be used as a base for abilities
 */
UCLASS(Blueprintable)
class SMULTRONBARNA_API USBAbility : public UObject
{
	GENERATED_BODY()

public:
	USBAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* Ability Interface */
	virtual bool CanActivateAbility	( ASBCharacter& Character ) { return true; }

	UFUNCTION( BlueprintCallable )
	virtual void ActivateAbility	( AActor* Instigator );

	UFUNCTION( BlueprintCallable )
	virtual void EndAbility			( AActor* Instigator );
	/* End Of Ability Interface */

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivateAbility	( AActor* Instigator );
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndAbility		( AActor* Instigator );

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Ability" );
	FName AbilityName;

	UPROPERTY( BlueprintReadOnly, Category = "Ability" )
	bool bActive;
};
