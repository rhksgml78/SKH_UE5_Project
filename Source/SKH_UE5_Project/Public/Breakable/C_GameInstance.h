// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "C_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FTreasureData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Treasure")
	FString TreasureName;
};

/**
 * 
 */
UCLASS()
class SKH_UE5_PROJECT_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UC_GameInstance();

private:
	UDataTable* DataTable;

public:
	UFUNCTION(BlueprintCallable)
	FString GetItemName(int32 ItemNum);

public:
	int32 GetAllItemNum();

};
