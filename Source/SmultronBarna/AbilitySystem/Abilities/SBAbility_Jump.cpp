// Copyright (c) 2020 Smultron Barna AB


#include "SBAbility_Jump.h"

#include "Character/SBCharacter.h"

bool
USBAbility_Jump::CanActivateAbility( ASBCharacter& Character )
{ return ( NumJumps < MaxJumps ); }

void
USBAbility_Jump::ActivateAbility( AActor* Instigator )
{
	ASBCharacter* Character = Cast< ASBCharacter >( Instigator );

	if ( !IsValid( Character ) )				return;
	if ( !CanActivateAbility( *Character ) )	return;

	NumJumps += 1;

	if ( NumJumps == 1 )	Character->LaunchCharacter( FVector( 0.f, 0.f, FirstJumpHeight * 10 ),		false, true );
	else					Character->LaunchCharacter( FVector( 0.f, 0.f, MultipleJumpHeight * 10 ),	false, true );
	
	Super::ActivateAbility( Instigator );
}

void
USBAbility_Jump::EndAbility( AActor* Instigator )
{
	NumJumps = 0;
	
	Super::EndAbility( Instigator );
}

void
USBAbility_Jump::AddJump( int Num )
{ NumJumps = FMath::Clamp( MaxJumps - Num, 0, MaxJumps ); }
