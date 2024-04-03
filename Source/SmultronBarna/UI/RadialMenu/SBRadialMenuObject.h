// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBRadialMenuObject.generated.h"

UCLASS(Blueprintable)
class SMULTRONBARNA_API USBRadialMenuObject : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	USBRadialMenuObject();
	
	UFUNCTION()
	void Select();

	UFUNCTION()
	void Unselect();

	UFUNCTION(BlueprintCallable)
	void Unlock();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnlocked(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelected(UObject* WorldContextObject);

	UFUNCTION()
	UMaterialInstance* GetIcon();

	UFUNCTION()
	UMaterialInstance* GetHoveredIcon();

	UFUNCTION()
	UMaterialInstance* GetActiveIcon();

	UFUNCTION()
	UMaterialInstance* GetLockedIcon();

	UFUNCTION()
	FString GetName();

	UFUNCTION()
	bool IsActive();

	UFUNCTION()
	bool IsLocked();
	
protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	bool bIsActive;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	bool bIsLocked;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	FString NameOfObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	UMaterialInstance* Icon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	UMaterialInstance* HoveredIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	UMaterialInstance* ActiveIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	UMaterialInstance* LockedIcon;
	
};
