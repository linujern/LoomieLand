// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBCheckpoint.generated.h"

UCLASS()
class SMULTRONBARNA_API ASBCheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ASBCheckpoint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void SetPlayerOwner(AActor* NewOwner);
	void TeleportOwner();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Checkpoint")
	TObjectPtr< UStaticMeshComponent > MeshComponent;

	UPROPERTY()
	AActor* Player;
};
