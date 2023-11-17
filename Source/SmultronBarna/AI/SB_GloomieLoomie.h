// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "SB_BaseLoomieAI.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "SB_GloomieLoomie.generated.h"

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API ASB_GloomieLoomie : public ASB_BaseLoomieAI
{
	GENERATED_BODY()

public:
	ASB_GloomieLoomie();

	virtual void			Hunting					( float DeltaTime ) override;

	virtual ASBCharacter*	IsPlayerInVisibleRange	( void ) override;

	virtual void			Attack					( void ) override;

	void Tick(float DeltaTime) override;

	void BeginPlay() override;

	UFUNCTION()
	void PlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyGloomie();

	void SetIsbeingDestroyed() { m_IsBeingDestroyed = true; }

	UFUNCTION(BlueprintImplementableEvent)
	void GetParameterValue();

	UFUNCTION(BlueprintImplementableEvent)
		void SetParameterValue();

protected:

	UPROPERTY( EditDefaultsOnly )
	USphereComponent* m_SphereCollider;

	UFUNCTION( BlueprintImplementableEvent )
	void AttackingPlayer( FVector Velocity, ASBCharacter* Player );

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hunting")
		float m_StunDuration;

	UPROPERTY(BlueprintReadWrite)
	float m_MaterialParameter;

	UPROPERTY(BlueprintReadWrite)
	bool m_Attacking;

private:

	UPROPERTY(EditAnywhere, Category = "Hunting")
	float m_DetectionRadius;

	UPROPERTY(EditAnywhere, Category = "Hunting")
	float m_VisionConeAngle;

	UPROPERTY(EditAnywhere, Category = "Hunting")
	float m_VisionConeLength;
	
	UPROPERTY(EditAnywhere, Category = "Hunting")
	float m_AttackRange;

	UPROPERTY(EditAnywhere, Category = "Hunting")
	int m_AttackDamage;

	UPROPERTY(EditAnywhere, Category = "Hunting")
	float m_ShoveDistance;

	float InitialStunDuration;

	bool m_IsBeingDestroyed;


};
