// Fill out your copyright notice in the Description page of Project Settings.


#include "SBRadialMenuIcon.h"

void USBRadialMenuIcon::SetMenuObject(USBRadialMenuObject* MenuObject)
{
	if(!MenuObject)
	{
		RadialMenuObject = nullptr;
		return;
	}
	
	RadialMenuObject = Cast<USBRadialMenuObject>(MenuObject);

	if(RadialMenuObject->IsLocked())
		IconImage->SetBrushFromMaterial(RadialMenuObject->GetLockedIcon());
	
	else if(RadialMenuObject->IsActive())
		IconImage->SetBrushFromMaterial(RadialMenuObject->GetActiveIcon());
	
	else
		IconImage->SetBrushFromMaterial(RadialMenuObject->GetIcon());
	
}

USBRadialMenuObject* USBRadialMenuIcon::GetMenuObject()
{
	return RadialMenuObject;
}

void USBRadialMenuIcon::Select()
{
	IconImage->SetBrushFromMaterial(RadialMenuObject->GetActiveIcon());
	RadialMenuObject->Select();
}

void USBRadialMenuIcon::Unlock()
{
	IconImage->SetBrushFromMaterial(RadialMenuObject->GetIcon());
	RadialMenuObject->Unlock();
}

void USBRadialMenuIcon::Unselect()
{
	RadialMenuObject->Unselect();

	if(RadialMenuObject->IsLocked())
		return;
	
	IconImage->SetBrushFromMaterial(RadialMenuObject->GetIcon());
}

void USBRadialMenuIcon::SetHovered()
{
	if(RadialMenuObject->IsLocked())
		return;

	
	IconImage->SetBrushFromMaterial(RadialMenuObject->GetHoveredIcon());
	OnHoveredBP();
}

void USBRadialMenuIcon::SetUnhovered()
{
	if(RadialMenuObject->IsLocked())
		return;
	
	if(RadialMenuObject->IsActive())
		IconImage->SetBrushFromMaterial(RadialMenuObject->GetActiveIcon());
	else
		IconImage->SetBrushFromMaterial(RadialMenuObject->GetIcon());
}
