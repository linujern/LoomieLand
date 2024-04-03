// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Character/SBCharacter.h"
#include "SB_MovingPlatform.generated.h"

UCLASS()
class SMULTRONBARNA_API ASB_MovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_MovingPlatform();

	void Moving(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_BoxCollider;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;

	UFUNCTION()
	void PlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> m_MoveToLocations;

	UPROPERTY(EditAnywhere)
	bool m_CollisionBeforeMove;

	UPROPERTY(EditAnywhere)
	bool m_MovingDuringCollision;

	UPROPERTY(EditAnywhere)
	bool m_ShouldReturn;

	UPROPERTY(EditAnywhere)
	bool m_ShouldMove;

	UPROPERTY(VisibleAnywhere)
	bool m_DisableMovement;

	FVector m_StartPos;

	 FVector m_EndGoal;

	UPROPERTY(EditAnywhere)
	float m_MovementSpeed;

	UPROPERTY()
	TArray< ASBCharacter* > m_TouchingPlayers;

	UPROPERTY(EditAnywhere)
	float m_DistanceTillReturn;

	UPROPERTY(EditAnywhere)
		bool m_ShouldReturnInstantly;

	UPROPERTY(EditAnywhere)
		float m_DelayUntilMove;

	float m_InitialDelayUntilMove;

	int m_MoveToIndex;
};
