// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SmultronBarna/UI/RadialMenu/SBRadialMenuObject.h"
#include "Components/Button.h"
#include "SBRadialMenuIcon.generated.h"
/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBRadialMenuIcon : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetMenuObject(USBRadialMenuObject* MenuObject);
	
	UFUNCTION()
	USBRadialMenuObject* GetMenuObject();

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Unlock();

	UFUNCTION()
	void Unselect();
		
	UFUNCTION()
	void SetHovered();
	
	UFUNCTION()
	void SetUnhovered();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHoveredBP();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta=(BindWidget))
	UImage* IconImage;
	
	UPROPERTY(BlueprintReadOnly)
	USBRadialMenuObject* RadialMenuObject = nullptr;
};

	
