// Copyright (c) 2020 Smultron Barna AB


#include "SBAbility_Glide.h"
#include "Character/SBCharacter.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

bool USBAbility_Glide::CanActivateAbility( ASBCharacter& Character )
{
	return Character.GetMovementComponent()->IsFalling();
}

void USBAbility_Glide::ActivateAbility( AActor* Instigator )
{
	if ( !IsValid( Instigator ) ) return;

	ASBCharacter* Character = Cast< ASBCharacter >( Instigator );

	if ( !IsValid( Character ) )				return;
	if ( !CanActivateAbility( *Character ) ) return;

	USBCharacterMovementComponent* MovementComponent = Character->GetCastedMovementComponent();

	if( MovementComponent->Velocity.Z != DescentRate * -1 && GlideTime <= GlideMaxTime )
	{
		Character->LaunchCharacter(FVector(0.f, 0.f, UKismetMathLibrary::FInterpTo(MovementComponent->Velocity.Z, DescentRate, GetWorld()->GetDeltaSeconds(), 3.f)), false, true);
	}
	
	MovementComponent->AirControl = GlideControl;

	TimerHandle.Invalidate();
	GlideTime += GetWorld()->GetDeltaSeconds();

	Super::ActivateAbility( Instigator );
}

void USBAbility_Glide::EndAbility( AActor* Instigator )
{
	if ( !IsValid( Instigator ) ) return;

	ASBCharacter* Character = Cast< ASBCharacter >( Instigator );

	if ( !IsValid( Character ) ) return;

	USBCharacterMovementComponent* MovementComponent = Character->GetCastedMovementComponent();
	MovementComponent->AirControl = JumpControl;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USBAbility_Glide::Timer, 1.f, false, 0.5f);

	Super::EndAbility(Instigator);
}

void USBAbility_Glide::Timer()
{
	if(GlideTime > 0.f && TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USBAbility_Glide::Timer, 1.f, false, 0.001f);
		GlideTime -= 0.01f;
	}
	else if( GlideTime < 0.f )
	{
		GlideTime = 0.f;	
	}
}
