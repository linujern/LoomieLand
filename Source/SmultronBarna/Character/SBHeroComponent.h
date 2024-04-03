// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Input/SBInputComponent.h"
#include "SBHeroComponent.generated.h"

struct FInputActionValue;
class USBInputConfig;

UCLASS()
class SMULTRONBARNA_API USBHeroComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	USBHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Hero")
	static USBHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USBHeroComponent>() : nullptr); }

	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const USBInputConfig* InputConfig);

	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const USBInputConfig* InputConfig);

	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	bool IsReadyToBindInputs() const;

protected:

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UInputMappingContext> DefaultInputMappings;

	/** True when player input bindings have been applied, will never be true for non - players */
	bool bReadyToBindInputs;
};
