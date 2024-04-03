// Copyright (c) 2020 Smultron Barna AB


#include "SBPowerupSystemComponent.h"

#include "Powerups/SBPowerup.h"

USBPowerupSystemComponent::USBPowerupSystemComponent(const FObjectInitializer& ObjectInitializer)
{
}

USBPowerup* USBPowerupSystemComponent::GetPowerup(FName PowerupName)
{
	for ( USBPowerup* Powerup : Powerups )
	{
		if ( Powerup->PowerupName != PowerupName )
			continue;
		
		return Powerup;
	}
	
	return nullptr;
}

void USBPowerupSystemComponent::ActivatePowerup(FName PowerupName)
{
	if( USBPowerup* Powerup = GetPowerup(PowerupName) )
	{
		AActor* Instigator = GetOwner();
		if( !IsValid( Instigator ) ) return;
		
		Powerup->ActivatePowerup( Instigator );
	}
}

void USBPowerupSystemComponent::AddPowerup(TSubclassOf<USBPowerup> PowerupTemplate, int Count)
{
	if( USBPowerup* Powerup = GetPowerup(PowerupTemplate.GetDefaultObject()->PowerupName) )
	{
		if( Powerup->Count == -1 ) return;
		Powerup->Count += 1;
		return;
	}
	
	USBPowerup* Powerup = NewObject< USBPowerup >( this, PowerupTemplate );
	Powerups.Add( Powerup );
}

void USBPowerupSystemComponent::RemovePowerup(FName PowerupName)
{
	if( USBPowerup* Powerup = GetPowerup( PowerupName ) )
	{
		Powerups.Remove( Powerup );
		Powerup->BeginDestroy();
	}
}
