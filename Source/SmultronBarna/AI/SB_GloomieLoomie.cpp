// Copyright (c) 2020 Smultron Barna AB


#include "SB_GloomieLoomie.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/SBCharacterMovementComponent.h"
#include "AbilitySystem/SBAbilitySystemComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"

ASB_GloomieLoomie::ASB_GloomieLoomie()
	:m_DetectionRadius( 700.0f )
	, m_VisionConeAngle( 70.0f )
	, m_VisionConeLength( 700.0f )
	, m_AttackRange( 100.0f )
	, m_ShoveDistance( 100.0f )
	, m_StunDuration( 3.0f )
	,m_IsBeingDestroyed(false)
	,m_MaterialParameter(0.0f)
	,m_Attacking(false)
{

}

void ASB_GloomieLoomie::Hunting( float DeltaTime )
{
	m_MoveRequestResult = m_Aicontroller->MoveTo( m_MoveRequest );

	m_TargetPlayer = IsPlayerInVisibleRange();

	if ( m_TargetPlayer )
	{
		m_MoveRequest.UpdateGoalLocation( m_TargetPlayer->GetActorLocation() );
		m_MovementSpeed = m_ChaseSpeed;
	}
	else if ( m_MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal )
	{
		if ( ( m_Idletimer -= DeltaTime ) <= 0 && m_StunDuration <= 0)
		{
			m_Idletimer = m_InitialIdleTimer;
			m_CurrentState = m_MovingPattern;
			m_TargetPlayer = nullptr;
			m_MovementSpeed = m_InitialMovementSpeed;
			GetNewPatrolPoint();
			return;
		}
	}
	UpdateRotation( DeltaTime );
}

ASBCharacter* ASB_GloomieLoomie::IsPlayerInVisibleRange()
{
	if ( m_Players.IsEmpty() )
	{
		return nullptr;
	}

	//DrawDebugCone	( GetWorld(), GetActorLocation(), GetActorForwardVector(), m_VisionConeLength, FMath::DegreesToRadians( m_VisionConeAngle ), FMath::DegreesToRadians( m_VisionConeAngle ) / 2, 10, FColor::Magenta );
	//DrawDebugSphere	( GetWorld(), GetActorLocation(), m_DetectionRadius, 6, FColor::Red );

	ASBCharacter* ClosestPlayer = nullptr;
	float			ClosestDistance = 1000000;

	for ( ASBCharacter* Player : m_Players )
	{
		FVector PlayerPosition = FVector( Player->GetActorLocation().X, Player->GetActorLocation().Y, GetActorLocation().Z );
		FVector EnemyToPlayer = PlayerPosition - GetActorLocation();

		float angle = FMath::Acos( EnemyToPlayer.Dot( GetActorForwardVector() ) / ( EnemyToPlayer.Length() * GetActorForwardVector().Length() ) );
		angle *= 180 / 3.1415;



		if ( ( angle < m_VisionConeAngle && EnemyToPlayer.Length() < m_VisionConeLength ) || EnemyToPlayer.Length() < m_DetectionRadius )
		{

			if ( EnemyToPlayer.Length() > ClosestDistance )
				continue;

			ClosestPlayer = Player;
			ClosestDistance = EnemyToPlayer.Length();
		}
	}
	

	return ClosestPlayer;
}

void ASB_GloomieLoomie::Attack()
{
	USBCharacterMovementComponent* CharactherMovement = m_TargetPlayer->GetComponentByClass<USBCharacterMovementComponent>();
	if (CharactherMovement)
	{
		m_MoveRequest.UpdateGoalLocation(GetActorLocation());
		m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
		m_Attacking = true;
		FVector OutVelocity;
		FVector ShoveLocation = GetActorLocation() + GetActorForwardVector() * m_ShoveDistance;
		if ( UGameplayStatics::SuggestProjectileVelocity_CustomArc( GetWorld(), OutVelocity, m_TargetPlayer->GetActorLocation(), ShoveLocation, GetWorld()->GetGravityZ(), 0.6f ) )
		{
			CharactherMovement->BrakingFrictionFactor = 0.0f;
			CharactherMovement->SetIsLaunched(true);

			m_StunDuration = InitialStunDuration;
			AttackingPlayer( OutVelocity, m_TargetPlayer );

			
		}
	}
}

void ASB_GloomieLoomie::Tick(float DeltaTime)
{
	if ( !HasAuthority() ) return;

	Super::Tick( DeltaTime );

	m_StunDuration -= DeltaTime;

	if(m_StunDuration <= 0)
	{
		StateMachine(DeltaTime);
	}

	if(m_IsBeingDestroyed)
	{
		m_Aicontroller->StopMovement();
		GetParameterValue();
		
		if(m_MaterialParameter >= 1 && m_LoomieManager)
		{
			m_LoomieManager->SpawnLoomie(FTransform(GetActorTransform()));
			m_LoomieManager->DestroyLoomie(this);
		}
	}
	
}

void ASB_GloomieLoomie::BeginPlay()
{
	Super::BeginPlay();

	InitialStunDuration = m_StunDuration;
	m_StunDuration = 0;

	//AddActorWorldOffset(FVector(0.0f, 0.0f, m_CapsuleCollider->GetUnscaledBoxExtent().Z));

	m_SphereCollider = GetComponentByClass<USphereComponent>();

	if(m_SphereCollider)
	{
		m_SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ASB_GloomieLoomie::PlayerCollision);
		m_SphereCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	}
	
}

void ASB_GloomieLoomie::PlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( !HasAuthority() ) return;

	if( OtherComp->GetCollisionObjectType() == ECC_Pawn && m_StunDuration <= 0.0f)
	{
		Attack();
	}

}

