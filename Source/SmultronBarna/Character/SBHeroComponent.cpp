// Copyright (c) 2020 Smultron Barna AB


#include "SBHeroComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SBPawnExtensionComponent.h"

USBHeroComponent::USBHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void USBHeroComponent::AddAdditionalInputConfig(const USBInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn< APawn >();
	if ( !Pawn )
	{
		return;
	}
	
	const APlayerController* PC = GetController< APlayerController >();
	check( PC );

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check( LP );

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem< UEnhancedInputLocalPlayerSubsystem >();
	check( Subsystem );

	if ( const USBPawnExtensionComponent* PawnExtComp = USBPawnExtensionComponent::FindPawnExtensionComponent( Pawn ) )
	{
		USBInputComponent* SBIC = Pawn->FindComponentByClass< USBInputComponent >();
		if ( ensureMsgf( SBIC, TEXT( "Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULyraInputComponent or a subclass of it." ) ) )
		{
			//@TODO Bind abilties;
			// SBIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void USBHeroComponent::RemoveAdditionalInputConfig(const USBInputConfig* InputConfig)
{
}

bool USBHeroComponent::IsReadyToBindInputs() const
{
	return false;
}

void USBHeroComponent::OnRegister()
{
	Super::OnRegister();
}

void USBHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USBHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void USBHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{ }

void USBHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{ }

void USBHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{ }

void USBHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{ }

void USBHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{ }
