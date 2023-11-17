// Copyright (c) 2020 Smultron Barna AB


#include "SBCheckpointSystemComponent.h"

#include "GeometryTypes.h"
#include "SBCheckpoint.h"

USBCheckpointSystemComponent::USBCheckpointSystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USBCheckpointSystemComponent::SpawnCheckpoint()
{
	if( IsValid(Checkpoint) )
	{
		Checkpoint->Destroy();
	}
	
	if( Checkpoint = Cast<ASBCheckpoint>(GetWorld()->SpawnActor(CheckpointTemplate)))
	{
		AActor* Instigator = GetOwner();
		if( !IsValid( Instigator ) ) return;
		
		Checkpoint->SetPlayerOwner( Instigator );
	}
}

void USBCheckpointSystemComponent::TeleportToCheckpoint()
{
	Checkpoint->TeleportOwner();
}

bool USBCheckpointSystemComponent::HasCheckpoint()
{
	return IsValid(Checkpoint) ? true : false;
}
