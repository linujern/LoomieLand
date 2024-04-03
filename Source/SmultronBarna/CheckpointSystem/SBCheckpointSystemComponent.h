// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SBCheckpointSystemComponent.generated.h"

class ASBCheckpoint;
/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBCheckpointSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USBCheckpointSystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void SpawnCheckpoint();
	UFUNCTION(BlueprintCallable)
	void TeleportToCheckpoint();
	UFUNCTION(BlueprintCallable)
	bool HasCheckpoint();

private:
	UPROPERTY(EditDefaultsOnly, meta = ( AllowPrivateAccess = "true" ))
	TSubclassOf< ASBCheckpoint > CheckpointTemplate;

	TObjectPtr< ASBCheckpoint > Checkpoint;
};
