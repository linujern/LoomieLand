// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SBPowerup.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SMULTRONBARNA_API USBPowerup : public UObject
{
	GENERATED_BODY()

public:
	USBPowerup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	virtual void ActivatePowerup(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	virtual void EndPowerup(AActor* Instigator);
	
public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Powerup" );
	FName PowerupName;

	int Count;
};
