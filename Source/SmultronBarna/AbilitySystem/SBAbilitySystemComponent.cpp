// Copyright (c) 2020 Smultron Barna AB


#include "SBAbilitySystemComponent.h"

#include "Abilities/SBAbility.h"

USBAbilitySystemComponent::USBAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	,m_IsStunned(false)
{
}

void USBAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

USBAbility* USBAbilitySystemComponent::GetAbility( FName AbilityName )
{
	for ( USBAbility* Ability : Abilities )
	{
		if ( Ability->AbilityName != AbilityName )
			continue;

		//GEngine->AddOnScreenDebugMessage( -1, 10, FColor::Red, FString::Printf( L"kekw: %p", Ability ) );
		return Ability;
	}

	return nullptr;
}

void USBAbilitySystemComponent::ActivateAbility(FName AbilityName)
{
	if (!m_IsStunned)
	{
		if (USBAbility* Ability = GetAbility(AbilityName))
		{

			AActor* Instigator = GetOwner();
			if (!IsValid(Instigator)) return;

			Ability->ActivateAbility(Instigator);

		}
	}
}

void USBAbilitySystemComponent::EndAbility(FName AbilityName)
{
	if( USBAbility* Ability = GetAbility(AbilityName) )
	{
		AActor* Instigator = GetOwner();
		if( !IsValid( Instigator ) ) return;
	
		Ability->EndAbility( Instigator );
	}
}

void USBAbilitySystemComponent::AddAbility( TSubclassOf< USBAbility > AbilityTemplate )
{
	USBAbility* Ability = NewObject< USBAbility >( this, AbilityTemplate );
	Abilities.Add( Ability );

	GEngine->AddOnScreenDebugMessage( -1, 10, FColor::Red, FString::Printf(L"POG %s", *Ability->AbilityName.ToString() ) );
}
