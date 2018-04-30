// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CTFPlayerState.h"
#include "CTFShinbiTableRow.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct MULTIPLAYER_API FCTFShinbiTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShinbi ShinbiType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* SkeletalMesh;
};

USTRUCT(Blueprintable)
struct MULTIPLAYER_API FCTFPlayerSpawnsTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};