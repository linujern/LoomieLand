// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SBPowerupSystemComponent.generated.h"

class USBPowerup;

UCLASS()
class SMULTRONBARNA_API USBPowerupSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USBPowerupSystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	USBPowerup* GetPowerup( FName PowerupName );
	
	UFUNCTION( BlueprintCallable )
	void ActivatePowerup( FName PowerupName );

	// Do count = -1 for infinite uses
	UFUNCTION( BlueprintCallable )
	void AddPowerup( TSubclassOf< USBPowerup > PowerupTemplate, int Count = -1 );
	UFUNCTION( BlueprintCallable )
	void RemovePowerup( FName PowerupName );
	
private:
	UPROPERTY()
	TArray< USBPowerup* > Powerups;
};
