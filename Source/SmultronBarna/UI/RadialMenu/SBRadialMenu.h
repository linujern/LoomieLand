// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBRadialMenuObject.h"
#include "SBRadialMenuIcon.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "SBRadialMenu.generated.h"

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBRadialMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Open(bool RegenerateIcons = false);

	UFUNCTION(BlueprintCallable)
	bool Close(bool PickItemOnClose = true, bool CloseOnLocked = true);

	UFUNCTION(BlueprintCallable)
	void UnlockIconOfType(TSubclassOf<USBRadialMenuObject> ObjectType);
	
protected:
	
	virtual void SynchronizeProperties() override;

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Radial Menu")
	double UpdateDelay;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Radial Menu")
	TArray<TSubclassOf<USBRadialMenuObject>> MenuObjects;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Icons")
	TSubclassOf<USBRadialMenuIcon> IconClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Icons")
	float Radius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Icons")
	float Size;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UCanvasPanel* Panel;

	UPROPERTY(meta=(BindWidgetOptional), BlueprintReadWrite)
	UImage* MousePointer;

	UFUNCTION(BlueprintImplementableEvent)
	void OnClose( UObject* WorldContextObject, AActor* Owner);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewItemPicked(int HoveredItemIndex);

private:

	UPROPERTY()
	int PreviousHoveredItem = 0;
	UPROPERTY()
	double UpdateTimer = 0;
	UPROPERTY()
	double MouseAngle = 0;
	UPROPERTY()
	double MouseAngle360 = 0;
	
	UPROPERTY(BlueprintReadOnly, Category="Icons", meta=(AllowPrivateAccess))
	TArray<USBRadialMenuIcon*> MenuIcons;
	
	void PickHoveredItem();
	void GenerateMenuIcons();
	void UpdateMouseAngle();

};
