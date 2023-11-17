// Copyright (c) 2020 Smultron Barna AB


#include "SBPawnExtensionComponent.h"

#include "AbilitySystem/SBAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

USBPawnExtensionComponent::USBPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
}

void USBPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USBPawnExtensionComponent, PawnData);
}

void USBPawnExtensionComponent::SetPawnData(const USBPawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		// UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();
}

void USBPawnExtensionComponent::HandleControllerChanged()
{
}

void USBPawnExtensionComponent::HandlePlayerStateReplicated()
{
}

void USBPawnExtensionComponent::SetupPlayerInputComponent()
{
}

void USBPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("SBPawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(USBPawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one SBPawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));
}

void USBPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USBPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void USBPawnExtensionComponent::OnRep_PawnData()
{
}
