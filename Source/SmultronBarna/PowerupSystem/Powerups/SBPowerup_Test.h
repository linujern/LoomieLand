// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "SBPowerup.h"
#include "SBPowerup_Test.generated.h"

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBPowerup_Test : public USBPowerup
{
	GENERATED_BODY()

public:
	USBPowerup_Test(const FObjectInitializer& ObjectInitializer);

	virtual void ActivatePowerup(AActor* Instigator) override;
	virtual void EndPowerup(AActor* Instigator) override;
};
