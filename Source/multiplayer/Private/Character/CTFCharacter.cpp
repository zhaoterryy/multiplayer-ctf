// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealNetwork.h"


// Sets default values
ACTFCharacter::ACTFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void ACTFCharacter::Client_PlayDashAnimation_Implementation()
{
	PlayDashAnimation();
}

void ACTFCharacter::Server_Dash_Implementation(FVector Velocity)
{
	GetCharacterMovement()->Velocity = Velocity;
	Client_PlayDashAnimation();
}

bool ACTFCharacter::Server_Dash_Validate(FVector Velocity)
{
	if (DashCooldownTimer < 1.0f || Velocity.Size() > 7600)
	{
		return false;
	}

	return true;
}

void ACTFCharacter::Dash()
{
	if (DashCooldownTimer > 1.0f)
	{
		DashCooldownTimer = 0.0f;
		if (Role == ROLE_Authority)
		{
			Server_Dash_Implementation(GetActorForwardVector() * 7500);
		}
		else
		{
			Server_Dash(GetActorForwardVector() * 7500);
		}
	}
}

// Called when the game starts or when spawned
void ACTFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACTFCharacter::OnRep_NewSkeletalMesh()
{
	GetMesh()->SetSkeletalMesh(NewSkeletalMesh);
}

// Called every frame
void ACTFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DashCooldownTimer += DeltaTime;
}

// Called to bind functionality to input
void ACTFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACTFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTFCharacter, NewSkeletalMesh);
}
