// Copyright (c) 2020 Smultron Barna AB


#include "SBCheckpoint.h"


ASBCheckpoint::ASBCheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void ASBCheckpoint::SetPlayerOwner(AActor* NewOwner)
{
	// if( !IsValid(Player) ) return;
	
	Player = NewOwner;
	
	SetActorLocation(Player->GetActorLocation());
	SetActorRotation(Player->GetActorRotation());
}

void ASBCheckpoint::TeleportOwner()
{
	if( !IsValid(Player) ) return;

	Player->TeleportTo( GetActorLocation(), GetActorRotation() );
}
