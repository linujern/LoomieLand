// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SBPawnExtensionComponent.generated.h"


class USBAbilitySystemComponent;
class USBPawnData;

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBPawnExtensionComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	USBPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Pawn")
	static USBPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USBPawnExtensionComponent>() : nullptr); }

	/** Gets the pawn data, which is used to specify pawn properties in data */
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	/** Sets the current pawn data */
	void SetPawnData(const USBPawnData* InPawnData);

	/** Should be called by the owning pawn when the pawn's controller changes. */
	void HandleControllerChanged();

	/** Should be called by the owning pawn when the player state has been replicated. */
	void HandlePlayerStateReplicated();

	/** Should be called by the owning pawn when the input component is setup. */
	void SetupPlayerInputComponent();

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnRep_PawnData();
	
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "Pawn")
	TObjectPtr<const USBPawnData> PawnData;
};
