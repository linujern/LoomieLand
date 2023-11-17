// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SBCharacterMovementComponent.h"
#include "SBPawnExtensionComponent.h"
#include "SBCharacter.generated.h"

class USBSpringArmComponent;
class USBCheckpointSystemComponent;
class USBPowerupSystemComponent;
class USBAbility;
class USBAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class SMULTRONBARNA_API ASBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASBCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void BeginPlay					( void ) override;
	virtual void Tick						( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent	( UInputComponent* PlayerInputComponent ) override;
	virtual void Landed						( const FHitResult& Hit ) override;

public:

	virtual USBCharacterMovementComponent*	GetMovementComponent() const override	{ UPawnMovementComponent* x = Super::GetMovementComponent(); return Cast<USBCharacterMovementComponent>( x ); };

	UFUNCTION( BlueprintCallable )
	USBCharacterMovementComponent*			GetCastedMovementComponent()			{ return GetMovementComponent(); }

private:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character", Meta = ( AllowPrivateAccess = "true" ))
	TObjectPtr< USBPawnExtensionComponent > PawnExtComponent;

	UPROPERTY(EditDefaultsOnly, Category="Character", meta=(BlueprintBaseOnly))
	TArray<TSubclassOf<USBAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<USBAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<USBPowerupSystemComponent> PowerupSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<USBCheckpointSystemComponent> CheckpointSystemComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	USBSpringArmComponent* CameraArm;

	/** Follow camera */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	UCameraComponent* FollowCamera;

};
