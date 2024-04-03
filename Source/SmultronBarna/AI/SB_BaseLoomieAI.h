// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include <Navigation/PathFollowingComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include "SB_LoomieManager.h"
#include "Character/SBCharacter.h"
#include "AITypes.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "SB_BaseLoomieAI.generated.h"

UENUM()
enum class State
{
	NONE,
	PATROL,
	HUNTING,
	ROAM,
	IDLE
};

UCLASS()
class SMULTRONBARNA_API ASB_BaseLoomieAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASB_BaseLoomieAI();

	virtual ASBCharacter*	IsPlayerInVisibleRange();

	void					Patrol				( float DeltaTime );

	virtual void			Hunting				( float DeltaTime ) {};

	void					StateMachine		( float DeltaTime );

	virtual void			Attack				();

	FVector					GetNewPatrolPoint();

	void					UpdateRotation		( float DeltaTime );

	void					Roam				( float DeltaTime );

	virtual void			Idle				( float DeltaTime );

	void					OffsetLocation		();

	void SetLoomieManager(ASB_LoomieManager* Value) { m_LoomieManager = Value; }

	ASB_LoomieManager* GetLoomieManager() { return m_LoomieManager; }

	void IncreaseMovementSpeed(float DeltaTime);

	bool Walking(float DeltaTime, bool Increase);

	bool Sprinting(float DeltaTime, bool Increase);

	void GetNewRoamPoint();

	float GetMovementSpeed() { return m_MovementSpeed; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Editanywhere, Category = "Idle")
		float m_Idletimer;

	UPROPERTY(EditDefaultsOnly)
		UFloatingPawnMovement* m_FloatingPawnMovement;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* m_CapsuleCollider;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> m_PatrolPoints;

	UPROPERTY()
		USkeletalMeshComponent* m_Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		State m_CurrentState;

	UPROPERTY(Editanywhere, Category = "Movement")
		float m_MovementSpeed;

	UPROPERTY(Editanywhere, Category = "Movement")
		float m_ChaseSpeed;

	UPROPERTY(Editanywhere, Category = "Movement")
		float m_RotationSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Patrol")
		int m_PatrolIndex;

	UPROPERTY(EditAnywhere, Category = "Hunting")
		float m_VisibleRange;

	UPROPERTY(EditAnywhere, Category = "Roam")
		float m_Radius;


	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool m_Ordered;

	UPROPERTY(EditAnywhere, Category = "Movement")
		State m_MovingPattern;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float m_DashSpeed;

	FVector						m_LastPosition;

	FAIMoveRequest				m_MoveRequest;
	UPROPERTY()
	AAIController*				m_Aicontroller;
	
	UPROPERTY()
	UNavigationSystemV1*		m_NavSystem;

	FPathFollowingRequestResult m_MoveRequestResult;

	UPROPERTY()
	TArray< ASBCharacter* >		m_Players;

	UPROPERTY()
	ASBCharacter*				m_TargetPlayer;

	float						m_InitialIdleTimer;

	UPROPERTY()
	ASB_LoomieManager* m_LoomieManager;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_MovementSpeedIncreaseSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_DistanceToWalkSpeed;

	float m_WalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_MovementSpeedDecreaseSpeed;

	float m_InitialMovementSpeed;

	UPROPERTY( EditAnywhere, meta = ( ToolTip = "How often should the loomie look for players." ) )
	float m_CheckStep;

	float m_CheckTimer;

public:	
	// Called every frame
	virtual void Tick( float DeltaTime ) override;

private:

};
