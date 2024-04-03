// Copyright (c) 2020 Smultron Barna AB


#include "SBPowerup.h"

#include "PowerupSystem/SBPowerupSystemComponent.h"

USBPowerup::USBPowerup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USBPowerup::ActivatePowerup(AActor* Instigator)
{
	if( Count != -1 )
	{
		Count -= 1;
	}
}

void USBPowerup::EndPowerup(AActor* Instigator)
{
	if( Count != -1, Count <= 0 )
	{
		if( USBPowerupSystemComponent* PowerupSystemComponent = Instigator->GetComponentByClass< USBPowerupSystemComponent >() )
		{
			PowerupSystemComponent->RemovePowerup( PowerupName );
		}
	}
}
