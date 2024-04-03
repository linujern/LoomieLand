// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SBCharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FSBCharacterGroundInfo
{
	GENERATED_BODY()

	FSBCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USBCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SimulateMovement(float DeltaTime) override;

	virtual bool CanAttemptJump() const override;
	
	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
	const FSBCharacterGroundInfo& GetGroundInfo();

	float GetDefaultGravityScale() const { return DefaultGravityScale; }

	void SetReplicatedAcceleration(const FVector& InAcceleration);

	//~UMovementComponent interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;
	//~End of UMovementComponent interface

	UFUNCTION(BlueprintCallable)
		void ResetBrakingFrictionFactor();

	void SetIsLaunched(bool Value) { m_IsLaunched = Value; }
	bool GetIsLaunched() { return m_IsLaunched; }
protected:
	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FSBCharacterGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;

	float DefaultGravityScale;

	float InitialBrakingFrictionFactor;

	bool m_IsLaunched;
};
