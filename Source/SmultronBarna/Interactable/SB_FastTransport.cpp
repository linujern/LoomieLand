// Copyright (c) 2020 Smultron Barna AB


#include "SB_FastTransport.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASB_FastTransport::ASB_FastTransport()
	:m_EndPos(nullptr)
	,m_StartPos(FVector(0.0f))
	,m_Launching(false)
	,m_LerpTarget(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	m_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(m_BoxCollider);

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_Mesh->SetupAttachment(m_BoxCollider);
}

void ASB_FastTransport::PlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if ( ASBCharacter* Player = Cast< ASBCharacter >( OtherActor ) )
	{
		m_StartBlendPos = Player->GetActorLocation();
		m_Player = Player;
		m_Launching = true;
	}
}

void ASB_FastTransport::LaunchPlayer(float DeltaTime)
{
	USBCharacterMovementComponent* CharactherMovement = m_Player->GetComponentByClass<USBCharacterMovementComponent>();
	if(CharactherMovement)
	{
			FVector OutVelocity;
			if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutVelocity, m_Player->GetActorLocation(), m_EndPos->GetActorLocation(), GetWorld()->GetGravityZ(), 0.45f))
			{
				CharactherMovement->BrakingFrictionFactor = 0.0f;
				LaunchPlayer(OutVelocity, m_Player);

				m_Launching = false;
			}
		
	}
}

// Called when the game starts or when spawned
void ASB_FastTransport::BeginPlay()
{
	Super::BeginPlay();
	
	m_StartPos = GetActorLocation();

	m_BoxCollider->OnComponentBeginOverlap.AddDynamic( this, &ASB_FastTransport::PlayerCollision );
	
	AActor* PlayerActor = UGameplayStatics::GetActorOfClass( GetWorld(), ASBCharacter::StaticClass() );
	if (PlayerActor != nullptr)
		m_Player = Cast< ASBCharacter >( PlayerActor );
		
}

// Called every framed
void ASB_FastTransport::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
	
	if ( m_Launching && m_EndPos )
		LaunchPlayer( DeltaTime );
}

