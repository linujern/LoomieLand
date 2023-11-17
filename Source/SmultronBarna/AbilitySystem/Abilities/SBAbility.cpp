// Copyright (c) 2020 Smultron Barna AB

#include "SBAbility.h"

USBAbility::USBAbility( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{ }

void USBAbility::ActivateAbility( AActor * Instigator )
{
	bActive = true;
	OnActivateAbility	( Instigator );
}
void USBAbility::EndAbility( AActor * Instigator )
{
	bActive = false;
	OnEndAbility		( Instigator );
}
