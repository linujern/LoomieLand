// Copyright (c) 2020 Smultron Barna AB


#include "SB_MovingPlatform.h"
#include "Kismet/GameplayStatics.h"
#include "CheckpointSystem/SBCheckpointSystemComponent.h"


// Sets default values
ASB_MovingPlatform::ASB_MovingPlatform()
	:m_CollisionBeforeMove(true)
	,m_MovingDuringCollision(true)
	,m_ShouldReturn(true)
	,m_ShouldMove(true)
	,m_DisableMovement(false)
	,m_EndGoal(0.0f)
	,m_MovementSpeed(50.0f)
	,m_DistanceTillReturn(1.0f)
	,m_ShouldReturnInstantly(false)
	,m_DelayUntilMove(0)
	,m_MoveToIndex(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_BoxCollider = CreateDefaultSubobject< UBoxComponent >( TEXT( "BoxCollider" ) );
	SetRootComponent( m_BoxCollider );

	m_Mesh = CreateDefaultSubobject< UStaticMeshComponent >( TEXT( "Mesh" ) );
	m_Mesh->SetupAttachment( m_BoxCollider );
}

void ASB_MovingPlatform::Moving(float DeltaTime)
{
	if( m_EndGoal != FVector::Zero() && (m_DelayUntilMove -= DeltaTime) <= 0 )
	{
		FVector Dir = m_EndGoal - GetActorLocation();
		Dir.Normalize();

		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), m_EndGoal, DeltaTime, m_MovementSpeed));

		
		if ( FVector::Distance( m_EndGoal, GetActorLocation() ) < m_DistanceTillReturn )
		{
			if (m_MoveToLocations.Num() == 1)
			{


				if (m_ShouldReturn)
				{
					FVector Temp = m_EndGoal;
					m_EndGoal = m_StartPos;
					m_StartPos = Temp;
				}
				else if (m_ShouldReturnInstantly)
				{
					for (ASBCharacter* Player : m_TouchingPlayers)
					{
						USBCheckpointSystemComponent* CheckPointSystem = Player->GetComponentByClass<USBCheckpointSystemComponent>();
						if (CheckPointSystem && CheckPointSystem->HasCheckpoint())
						{
							m_Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
							return;
						}

					}


				}
			}
			else
			{
				if(m_MoveToIndex == m_MoveToLocations.Num() - 1)
				{
					m_MoveToIndex = 0;
					m_EndGoal = m_StartPos;
				}
				else
				{
					if(m_EndGoal != m_StartPos)
					m_MoveToIndex++;
					m_EndGoal = m_MoveToLocations[m_MoveToIndex]->GetActorLocation();
				}

				
			}
		}
	}
}

// Called when the game starts or when spawned
void ASB_MovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if( m_CollisionBeforeMove )
	{
		m_DisableMovement = true;
	}
	
	m_BoxCollider->OnComponentBeginOverlap.AddDynamic( this, &ASB_MovingPlatform::PlayerCollision );

	m_BoxCollider->OnComponentEndOverlap.AddDynamic( this, &ASB_MovingPlatform::OnOverlapEnd );

	m_StartPos = GetActorLocation();

	if ( !m_MoveToLocations.IsEmpty() )
		m_EndGoal = m_MoveToLocations[0]->GetActorLocation();

	m_InitialDelayUntilMove = m_DelayUntilMove;
}

// Called every frame
void ASB_MovingPlatform::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	if ( !HasAuthority() )
		return;

	if(!m_DisableMovement)
		Moving( DeltaTime );

	if(m_TouchingPlayers.IsEmpty() && m_ShouldReturnInstantly && m_Mesh->GetCollisionResponseToChannels() != ECR_Block)
	{
		SetActorLocation(m_StartPos);
		m_Mesh->SetCollisionResponseToAllChannels(ECR_Block);
		m_DelayUntilMove = m_InitialDelayUntilMove;
	}

}

void ASB_MovingPlatform::PlayerCollision( UPrimitiveComponent* OverlappedComponent
										, AActor* OtherActor
										, UPrimitiveComponent* OtherComp
										, int32 OtherBodyIndex
										, bool bFromSweep
										, const FHitResult& SweepResult )
{
	if ( !HasAuthority() )
		return;

	Super::NotifyActorBeginOverlap( OtherActor );

	if ( ASBCharacter* Player = Cast< ASBCharacter >( OtherActor ) )
		m_TouchingPlayers.Add( Player );

	else return;

	if ( m_CollisionBeforeMove )
	{
		m_DisableMovement = false;
	}

	if ( !m_MovingDuringCollision )
	{
		m_DisableMovement = true;
	}
	
}

void ASB_MovingPlatform::OnOverlapEnd( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	if ( !HasAuthority() )
		return;

	Super::NotifyActorEndOverlap(OtherActor);

	if ( ASBCharacter* Player = Cast< ASBCharacter >( OtherActor ) )
		m_TouchingPlayers.RemoveSingle( Player );

	else return;

	if ( m_TouchingPlayers.IsEmpty()  )
	{
		if (!m_MovingDuringCollision)
			m_DisableMovement = false;
	}
}
