// Copyright (c) 2020 Smultron Barna AB


#include "AI/SB_Gift.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASB_Gift::ASB_Gift()
	:MaxSpeed(100.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	m_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(m_Collider);
	m_StaticMesh->SetupAttachment(m_Collider);
	

	
	Target = FVector::Zero();
	m_Launch = false;
}

void ASB_Gift::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

// Called when the game starts or when spawned
void ASB_Gift::BeginPlay()
{
	Super::BeginPlay();

	m_Collider->OnComponentHit.AddDynamic(this, &ASB_Gift::OnComponentHit);



}

// Called every frame
void ASB_Gift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Target != FVector::Zero() && !m_Launch)
	{
		FVector OutVelocity;

		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutVelocity, GetActorLocation(), Target);

		
		m_Collider->SetAllPhysicsLinearVelocity(OutVelocity);

		m_Launch = true;
	}

}

