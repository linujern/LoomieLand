// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Character/SBCharacter.h"
#include "SB_FastTransport.generated.h"

UCLASS()
class SMULTRONBARNA_API ASB_FastTransport : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_FastTransport();

	UFUNCTION()
	void PlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void LaunchPlayer(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* m_BoxCollider;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AActor* m_EndPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASBCharacter* m_Player;

	UFUNCTION(BlueprintImplementableEvent)
		void LaunchPlayer(FVector Velocity, ASBCharacter* Player);
private:
	
	FVector m_StartPos;

	bool m_Launching;

	float m_BlendTime;

	FVector m_StartBlendPos;

	FVector m_MiddlePos;

	FVector m_LerpTarget;

};
