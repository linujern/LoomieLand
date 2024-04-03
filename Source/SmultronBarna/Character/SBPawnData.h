// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SBPawnData.generated.h"

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USBPawnData(const FObjectInitializer& ObjectInitializer);

public:
	// Class to instantiate for this pawn (should usually derive from APawn or ASBCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	TSubclassOf<class APawn> PawnClass;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class USBInputConfig> InputConfig;
};
