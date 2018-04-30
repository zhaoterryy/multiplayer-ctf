// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTFCharacter.generated.h"

UCLASS()
class MULTIPLAYER_API ACTFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACTFCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayDashAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDashAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Dash(FVector Velocity);

	void Dash();

	UPROPERTY(ReplicatedUsing = OnRep_NewSkeletalMesh)
	class USkeletalMesh* NewSkeletalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_NewSkeletalMesh();

	float DashCooldownTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
