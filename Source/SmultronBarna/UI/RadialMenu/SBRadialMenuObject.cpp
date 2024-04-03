// Fill out your copyright notice in the Description page of Project Settings.


#include "SBRadialMenuObject.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
USBRadialMenuObject::USBRadialMenuObject()
	:	bIsActive(false)
	,	NameOfObject("")
{

}

void USBRadialMenuObject::Select()
{
	if(bIsLocked)
		return;
	
	bIsActive = true;
	OnSelected(GetWorld());
}

void USBRadialMenuObject::Unselect()
{
	bIsActive = false;
}

void USBRadialMenuObject::Unlock()
{
	bIsLocked = false;
	OnUnlocked(GetWorld());
}

UMaterialInstance* USBRadialMenuObject::GetIcon()
{
	return Icon;
}

UMaterialInstance* USBRadialMenuObject::GetHoveredIcon()
{
	return HoveredIcon;
}

UMaterialInstance* USBRadialMenuObject::GetActiveIcon()
{
	return ActiveIcon;
}

UMaterialInstance* USBRadialMenuObject::GetLockedIcon()
{
	return LockedIcon;
}

FString USBRadialMenuObject::GetName()
{
	return NameOfObject;
}

bool USBRadialMenuObject::IsActive()
{
	return bIsActive;
}

bool USBRadialMenuObject::IsLocked()
{
	return bIsLocked;
}




