// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SBAbilitySystemComponent.generated.h"

class USBAbility;

UCLASS()
class SMULTRONBARNA_API USBAbilitySystemComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	USBAbilitySystemComponent( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	virtual void BeginPlay() override;

	UFUNCTION( BlueprintCallable )
	USBAbility* GetAbility( FName AbilityName );

	UFUNCTION( BlueprintCallable )
	void ActivateAbility( FName AbilityName );

	UFUNCTION( BlueprintCallable )
	void EndAbility( FName AbilityName );
	
	UFUNCTION( BlueprintCallable )
	void AddAbility( TSubclassOf< USBAbility > AbilityTemplate );

	void SetIsStunned(bool Value) { m_IsStunned = Value; }

	UPROPERTY(BlueprintReadOnly)
		bool m_IsStunned;

private:

	UPROPERTY()
	TArray< USBAbility* > Abilities;

};
