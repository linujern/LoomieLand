// Copyright (c) 2020 Smultron Barna AB


#include "SB_BaseLoomieAI.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASB_BaseLoomieAI::ASB_BaseLoomieAI()
: m_Idletimer					( 1.0f )
, m_CurrentState				( State::PATROL )
, m_MovementSpeed				( 100.0f )
, m_ChaseSpeed					( 150.0f )
, m_RotationSpeed				( 60.0f )
, m_PatrolIndex					( 0 )
, m_LastPosition				( 0, 0, 0 )
, m_MoveRequest					( FAIMoveRequest() )
, m_MoveRequestResult			( { } )
, m_VisibleRange				( 2000.0f )
, m_TargetPlayer				( nullptr )
, m_Radius						( 100.0f )
, m_Ordered						( true )
, m_MovingPattern				( State::NONE )
, m_DashSpeed					( 70.0f )
, m_LoomieManager				( nullptr )
, m_MovementSpeedIncreaseSpeed	( 20.0f )
, m_DistanceToWalkSpeed			( 150.0f )
, m_MovementSpeedDecreaseSpeed	( 50.0f )
, m_InitialMovementSpeed		( 0.0f )
, m_CheckTimer					( 0.0f )
, m_CheckStep					( 0.3f )
{
	PrimaryActorTick.bCanEverTick = true;

	m_MoveRequest.SetUsePathfinding			( true );
	m_MoveRequest.SetProjectGoalLocation	( true );
	m_MoveRequest.SetAllowPartialPath		( true );
}

ASBCharacter* ASB_BaseLoomieAI::IsPlayerInVisibleRange()
{ return nullptr; }

void ASB_BaseLoomieAI::Patrol( float DeltaTime )
{
	if (!HasAuthority())
		return;

	m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);

	m_TargetPlayer = IsPlayerInVisibleRange();

	if( m_TargetPlayer )
	{
		m_CurrentState = State::HUNTING;
		m_Idletimer = m_InitialIdleTimer;
		return;
	}
	
	if(m_MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if((m_Idletimer -= DeltaTime) <= 0)
		{
			m_MoveRequest.UpdateGoalLocation(GetNewPatrolPoint());
			m_Idletimer = m_InitialIdleTimer;
		}
	}
	UpdateRotation(DeltaTime);
}

void ASB_BaseLoomieAI::StateMachine( float DeltaTime )
{
	switch ( m_CurrentState )
	{
		default: break;
		case State::PATROL:		Patrol	( DeltaTime ); break;
		case State::HUNTING:	Hunting	( DeltaTime ); break;
		case State::ROAM:		Roam	( DeltaTime ); break;
		case State::IDLE:		Idle	( DeltaTime ); break;
	}
}

void ASB_BaseLoomieAI::Attack( )
{
	
}

FVector ASB_BaseLoomieAI::GetNewPatrolPoint()
{
	if (m_PatrolPoints.IsEmpty() || m_PatrolPoints.Num() == 1)
		return GetActorLocation();

	if (m_Ordered)
	{
		if (m_PatrolIndex >= m_PatrolPoints.Num() - 1)
		{
			m_PatrolIndex = 0;
		}
		else
		{
			m_PatrolIndex++;
		}
	}
	else
	{
		int NewPatrolIndex = 0;
		do
		{
			NewPatrolIndex = FMath::RandRange(0, m_PatrolPoints.Num() - 1);
			
		} while (NewPatrolIndex == m_PatrolIndex);
		
		m_PatrolIndex = NewPatrolIndex;
	}
	if (m_PatrolPoints[m_PatrolIndex] != nullptr)
		return m_PatrolPoints[m_PatrolIndex]->GetActorLocation();
	else
	{
		return GetActorLocation();
	}
}

void ASB_BaseLoomieAI::UpdateRotation( float DeltaTime )
{
	if (GetActorLocation() == m_LastPosition)
		return;

	m_LastPosition.Z = GetActorLocation().Z;
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(m_LastPosition, GetActorLocation());
	Rotator = FMath::RInterpTo(GetActorRotation(), Rotator, DeltaTime, m_RotationSpeed);
	SetActorRotation(Rotator);
	m_LastPosition = GetActorLocation();
}

void ASB_BaseLoomieAI::Roam( float DeltaTime )
{
	m_TargetPlayer = IsPlayerInVisibleRange();

	if ( m_TargetPlayer )
	{
		m_CurrentState = State::HUNTING;
		m_Idletimer = m_InitialIdleTimer;
		return;
	}

	if(m_Radius <= 0)
	{
		return;
	}

	m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
	
	if (m_MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{

		if ((m_Idletimer -= DeltaTime) <= 0)
		{
			m_Idletimer = m_InitialIdleTimer;
			GetNewRoamPoint();
		}
		
	}
	UpdateRotation(DeltaTime);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), m_Radius, 10, FColor::Green);
}

void ASB_BaseLoomieAI::Idle(float DeltaTime)
{

}

void ASB_BaseLoomieAI::OffsetLocation()
{
	FHitResult Hit;
	FVector Offset(0, 0, 1000);
	FCollisionObjectQueryParams params;
	params.AddObjectTypesToQuery(ECC_WorldStatic);
	GetWorld()->LineTraceSingleByObjectType(Hit, GetActorLocation(), GetActorLocation() - Offset, params);

	if (m_CapsuleCollider)
	{

		Offset.Z = m_CapsuleCollider->GetUnscaledBoxExtent().Z + 4.0f;
		if ((Hit.Location - GetActorLocation()).Length() < 100.0f)
			SetActorLocation(Hit.Location + Offset);

	}
}

void ASB_BaseLoomieAI::IncreaseMovementSpeed(float DeltaTime)
{
	float DistanceToGoal = FVector::Dist(GetActorLocation(), m_MoveRequest.GetDestination());

	//Decrease
	if( DistanceToGoal < m_DistanceToWalkSpeed)
	{

		if (m_LastPosition == GetActorLocation() && m_MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			m_FloatingPawnMovement->MaxSpeed = FMath::FInterpConstantTo(m_FloatingPawnMovement->MaxSpeed, 0.0f, DeltaTime, m_MovementSpeedDecreaseSpeed);
		}
		else
		{
			m_FloatingPawnMovement->MaxSpeed = FMath::FInterpConstantTo(m_FloatingPawnMovement->MaxSpeed, m_WalkSpeed, DeltaTime, m_MovementSpeedDecreaseSpeed);
		}
		
	}
	//Increase
	else
	{
		if(!Walking(DeltaTime, true))
		{
			m_FloatingPawnMovement->MaxSpeed = FMath::FInterpConstantTo(m_FloatingPawnMovement->MaxSpeed, m_WalkSpeed, DeltaTime, m_MovementSpeedIncreaseSpeed);
		}
		else if(!Sprinting(DeltaTime,true))
		{
			m_FloatingPawnMovement->MaxSpeed = FMath::FInterpConstantTo(m_FloatingPawnMovement->MaxSpeed, m_MovementSpeed, DeltaTime, m_MovementSpeedIncreaseSpeed);
		}
	}
}

bool ASB_BaseLoomieAI::Walking(float DeltaTime, bool Increase)
{

	if (!Increase)
	{
		if (m_FloatingPawnMovement->MaxSpeed >= m_WalkSpeed + 1.0f)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		if (m_FloatingPawnMovement->MaxSpeed <= m_WalkSpeed - 1.0f)
		{
			
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool ASB_BaseLoomieAI::Sprinting(float DeltaTime, bool Increase)
{
	if (m_FloatingPawnMovement->MaxSpeed <= m_MovementSpeed)
	{
		
		return false;
	}
	else
	{
		return true;
	}

	
}

void ASB_BaseLoomieAI::GetNewRoamPoint()
{
	if (!HasAuthority())
	{
		return;
	}

	FNavLocation Temp;
	bool LocationFound = false;
	do
	{
		LocationFound = m_NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), m_Radius, Temp);
	} while (!LocationFound);



	m_MoveRequest.UpdateGoalLocation(Temp);
}

void ASB_BaseLoomieAI::BeginPlay()
{
	Super::BeginPlay();
	
	if(!HasAuthority())
	{
		return;
	}

	m_Aicontroller = Cast<AAIController>(GetController());

	m_InitialIdleTimer = m_Idletimer;

	m_CapsuleCollider = GetComponentByClass<UBoxComponent>();

	m_FloatingPawnMovement = GetComponentByClass<UFloatingPawnMovement>();

	m_FloatingPawnMovement->MaxSpeed = 0.0f;

	m_WalkSpeed = m_MovementSpeed / 2;

	m_InitialMovementSpeed = m_MovementSpeed;

	UNavigationSystemV1* Isvalid = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	m_MoveRequest.SetGoalLocation(GetActorLocation());
	if(Isvalid)
	{
		m_NavSystem = Isvalid;
	}
	if (m_MovementSpeed > 0)
	{
		if (m_MovingPattern != State::NONE)
		{
			m_CurrentState = m_MovingPattern;
		}
		else
		{
			if (!m_PatrolPoints.IsEmpty())
			{
				m_CurrentState = State::PATROL;
				m_MovingPattern = State::PATROL;
			}
			else
			{
				m_CurrentState = State::ROAM;
				m_MovingPattern = State::ROAM;
				GetNewRoamPoint();
			}

		}

		if (!m_LoomieManager)
		{
			AActor* OutActors;
			OutActors = UGameplayStatics::GetActorOfClass(GetWorld(), ASB_LoomieManager::StaticClass());

			ASB_LoomieManager* Manager = Cast<ASB_LoomieManager>(OutActors);
			if (Manager)
			{
				Manager->AddLoomie(this);
				SetLoomieManager(Manager);
			}
		}
	}
}

void ASB_BaseLoomieAI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( !HasAuthority() )
		return;

	// look for players
	if ( ( m_CheckTimer += DeltaTime ) >= m_CheckStep )
	{
		m_CheckTimer -= m_CheckStep;

		TArray< TEnumAsByte< EObjectTypeQuery > > traceObjectTypes;
		traceObjectTypes.Add( UEngineTypes::ConvertToObjectType( ECollisionChannel::ECC_Pawn ) );

		TArray< AActor* > IgnoredActors, RecievedActors;
		IgnoredActors.Init( this, 1 );
		UKismetSystemLibrary::SphereOverlapActors( GetWorld(), GetActorLocation(), m_VisibleRange, traceObjectTypes, ASBCharacter::StaticClass(), IgnoredActors, RecievedActors );
		
		m_Players.Empty();
		for ( AActor* Actor : RecievedActors )
			m_Players.Add( Cast< ASBCharacter >( Actor ) );
	}
	
	if( !m_Aicontroller )
	{
		m_Aicontroller = Cast< AAIController >( GetController() );
		GEngine->AddOnScreenDebugMessage( INDEX_NONE, 1.0f, FColor::Blue, "AiController is null" );
	}

	OffsetLocation();
	IncreaseMovementSpeed( DeltaTime );
}

