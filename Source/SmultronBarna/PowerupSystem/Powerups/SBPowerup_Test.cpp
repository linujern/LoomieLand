// Copyright (c) 2020 Smultron Barna AB


#include "SBPowerup_Test.h"

#include "Character/SBCharacter.h"

USBPowerup_Test::USBPowerup_Test(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PowerupName = TEXT("Test");
}

void USBPowerup_Test::ActivatePowerup(AActor* Instigator)
{
	Super::ActivatePowerup(Instigator);

	ASBCharacter* Character = Cast<ASBCharacter>(Instigator);
	Character->GetMovementComponent()->MaxWalkSpeed = Character->GetMovementComponent()->MaxWalkSpeed * 10;
}

void USBPowerup_Test::EndPowerup(AActor* Instigator)
{
	Super::EndPowerup(Instigator);
}
