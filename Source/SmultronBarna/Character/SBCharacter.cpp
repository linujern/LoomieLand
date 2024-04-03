// Copyright (c) 2020 Smultron Barna AB


#include "SBCharacter.h"

#include "SBCharacterMovementComponent.h"
#include "SBPawnExtensionComponent.h"
#include "AbilitySystem/SBAbilitySystemComponent.h"
#include "PowerupSystem/SBPowerupSystemComponent.h"
#include "CheckpointSystem/SBCheckpointSystemComponent.h"
#include "Camera/SBSpringArmComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ASBCharacter::ASBCharacter( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer.SetDefaultSubobjectClass< USBCharacterMovementComponent >( ACharacter::CharacterMovementComponentName ) )
{
	PrimaryActorTick.bCanEverTick			= false;
	PrimaryActorTick.bStartWithTickEnabled	= false;

	PawnExtComponent           = ObjectInitializer.CreateDefaultSubobject< USBPawnExtensionComponent >    ( this, L"PawnExtensionComponent"     );
	AbilitySystemComponent     = ObjectInitializer.CreateDefaultSubobject< USBAbilitySystemComponent >    ( this, L"AbilitySystemComponent"     );
	PowerupSystemComponent     = ObjectInitializer.CreateDefaultSubobject< USBPowerupSystemComponent >    ( this, L"PowerupSystemComponent"     );
	CheckpointSystemComponent  = ObjectInitializer.CreateDefaultSubobject< USBCheckpointSystemComponent > ( this, L"CheckpointSystemComponent"  );
	//CameraArm                  = ObjectInitializer.CreateDefaultSubobject< USBSpringArmComponent >          ( this, L"CameraBoom"                 );
	//FollowCamera               = ObjectInitializer.CreateDefaultSubobject< UCameraComponent >             ( this, L"FollowCamera"               );

	//CameraArm->SetupAttachment( RootComponent );
	//SCameraArm->bUsePawnControlRotation = true;                                   // Rotate the arm based on the controller

	//FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false;                                // Camera does not rotate relative to arm


}

void ASBCharacter::BeginPlay()
{
	for( TSubclassOf< USBAbility >& AbilityTemplate : DefaultAbilities )
		AbilitySystemComponent->AddAbility( AbilityTemplate );

	Super::BeginPlay();
}

void ASBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//FVector RPosStart = CameraBoom->GetUnfixedCameraPosition();
	//RPosStart.X += 30.f;
//
	//FVector LPosStart = RPosStart;
	//LPosStart.X -= 60.f;
//
	//FHitResult RCamTraceRes;
	//GetWorld()->LineTraceSingleByChannel(RCamTraceRes, FollowCamera->GetRelativeLocation(), GetActorLocation(), ECC_Camera);
//
	//DrawDebugLine(GetWorld(), RPosStart, GetActorLocation(), FColor::Red);
	//DrawDebugLine(GetWorld(), LPosStart, GetActorLocation(), FColor::Red);
}

void ASBCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	
	PawnExtComponent->SetupPlayerInputComponent();
}

void ASBCharacter::Landed( const FHitResult& Hit )
{ Super::Landed( Hit ); }
