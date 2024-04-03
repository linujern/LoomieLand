// Fill out your copyright notice in the Description page of Project Settings.


#include "SBRadialMenu.h"

#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void USBRadialMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void USBRadialMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	MenuIcons.Empty();
	GenerateMenuIcons();
	PreviousHoveredItem = 0;
	UpdateTimer = 1;
}

void USBRadialMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTimer += InDeltaTime;

	if(UpdateTimer < UpdateDelay)
		return;

	UpdateTimer = 0;
		
	UpdateMouseAngle();

	if(IsValid(MousePointer))
	{
		MousePointer->SetRenderTransformAngle(MouseAngle);
	}

	if(MouseAngle >= 0)
	{
		MouseAngle360 = MouseAngle;
	}
	else
	{
		MouseAngle360 = 360 + MouseAngle;
	}
	
	PickHoveredItem();
}

void USBRadialMenu::Open(bool RegenerateIcons)
{

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if(RegenerateIcons)
	{
		for(int i = MenuIcons.Num() - 1; i >= 0; i-- )
		{
			Panel->RemoveChild(MenuIcons[i]);
		}

		MenuIcons.Empty();
		GenerateMenuIcons();
	}
	
	
}

bool USBRadialMenu::Close(bool PickItemOnClose, bool CloseOnLocked)
{
	if(!PickItemOnClose)
		return false;

	if(MenuIcons[PreviousHoveredItem]->GetMenuObject()->IsLocked() && !CloseOnLocked)
		return false;

	if(MenuIcons[PreviousHoveredItem]->GetMenuObject()->IsLocked() && CloseOnLocked)
	{
		OnClose(GetWorld(), GetOwningPlayer());
		return true;
	}

	
	for(int i = 0; i < MenuObjects.Num(); i++)
	{
		if(i == PreviousHoveredItem)
		{
			if(IsValid(MenuIcons[PreviousHoveredItem]))
				MenuIcons[PreviousHoveredItem]->Select();
		}
		else
		{
			MenuIcons[i]->Unselect();
		}
	}
	
	OnClose(GetWorld(), GetOwningPlayer());
	return true;
}

void USBRadialMenu::UnlockIconOfType(TSubclassOf<USBRadialMenuObject> ObjectType)
{
	for(USBRadialMenuIcon* MenuIcon : MenuIcons)
	{
		if(MenuIcon->GetMenuObject()->GetClass() == ObjectType)
		{
			MenuIcon->Unlock();
		}
	}
}

void USBRadialMenu::PickHoveredItem()
{
	if(MenuIcons.IsEmpty())
		return;
	
	int HoveredItem = UKismetMathLibrary::FFloor((MouseAngle360 - (180/MenuIcons.Num())) / (360/MenuIcons.Num())) + 1;
	
	if(HoveredItem >= MenuIcons.Num() || HoveredItem < 0 )
		HoveredItem = 0;

	if(HoveredItem == PreviousHoveredItem)
		return;


	
	MenuIcons[HoveredItem]->SetHovered();
	MenuIcons[PreviousHoveredItem]->SetUnhovered();

	PreviousHoveredItem = HoveredItem;
	
	OnNewItemPicked(PreviousHoveredItem);
}

void USBRadialMenu::GenerateMenuIcons()
{
	if(MenuObjects.IsEmpty())
		return;
	
	float IconAngle = 360/MenuObjects.Num();

	FVector2d ViewportSize;
	GEngine->GameViewport->GetViewportSize( ViewportSize);
	FVector2d CenterOfScreen(ViewportSize.X/2 ,ViewportSize.Y/2);
	
	if(!IsValid(Panel) && !IsValid(IconClass))
	{
		return;
	}
	
	for(int i = 0; i < MenuObjects.Num(); i++)
	{
		UUserWidget* NewIcon = CreateWidget(this, IconClass);
		
		if(!IsValid(NewIcon))
			continue;
		
		USBRadialMenuIcon* NewMenuIcon = Cast<USBRadialMenuIcon>(NewIcon);
		if(!IsValid(NewMenuIcon))
			continue;
			
		if(!IsValid( MenuObjects[i]))
			continue;
		
		USBRadialMenuObject* MenuObject = NewObject<USBRadialMenuObject>(this, MenuObjects[i]);
		
		if(IsValid(MenuObject))
			NewMenuIcon->SetMenuObject(MenuObject);
		
		FVector2d IconVector  = FVector2d(ViewportSize.X/2 ,0) - CenterOfScreen;
		IconVector = IconVector.GetRotated(IconAngle*i);
		IconVector.Normalize();

		FVector2d IconPos = (IconVector * Radius);
	
		Panel->AddChild(NewMenuIcon);
		Cast<UCanvasPanelSlot>(NewMenuIcon->Slot)->SetAnchors(  FAnchors((CenterOfScreen.X/ViewportSize.X), (CenterOfScreen.Y/ViewportSize.Y)) );
		Cast<UCanvasPanelSlot>(NewMenuIcon->Slot)->SetAlignment( FVector2d(0.5,0.5) );
		Cast<UCanvasPanelSlot>(NewMenuIcon->Slot)->SetPosition( IconPos );
		Cast<UCanvasPanelSlot>(NewMenuIcon->Slot)->SetSize(FVector2D( Size, Size));

		MenuIcons.Add(NewMenuIcon);
	}
}

void USBRadialMenu::UpdateMouseAngle()
{
	FIntVector2 ViewportSize;
	FVector2d MousePosition;
	
	GetOwningPlayer()->GetMousePosition(MousePosition.X,MousePosition.Y);
	GetOwningPlayer()->GetViewportSize(  ViewportSize.X, ViewportSize.Y);
	
	FVector2d CenterOfScreen(ViewportSize.X/2 ,ViewportSize.Y/2);
	FVector2d MiddleTop(ViewportSize.X/2, 0);
	
	FVector2d UpVector = MiddleTop - CenterOfScreen;
	FVector2d MouseVector = MousePosition - CenterOfScreen;
	
	UpVector.Normalize();
	MouseVector.Normalize();
	
	if(MousePosition.X < ViewportSize.X/2)
	{
		MouseAngle = -1 * UKismetMathLibrary::DegAcos(MouseVector.Dot(UpVector));
	} else
	{
		MouseAngle = 1 * UKismetMathLibrary::DegAcos(MouseVector.Dot(UpVector));
	}
	
}
