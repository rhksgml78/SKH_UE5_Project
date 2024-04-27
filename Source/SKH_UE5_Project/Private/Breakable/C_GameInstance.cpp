// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/C_GameInstance.h"

UC_GameInstance::UC_GameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>DATATABLE(TEXT("/Script/Engine.DataTable'/Game/Blueprints/DataTable/TreasureTable.TreasureTable'"));
	if (DATATABLE.Succeeded())
		DataTable = DATATABLE.Object;
}

FString UC_GameInstance::GetItemName(int32 ItemNum)
{
	if (!DataTable) return FString("Empty");

	FString ContextString;
	FString RowString = FString::FromInt(ItemNum);
	FName RowName = FName(RowString);

	FTreasureData* ItemRow = DataTable->FindRow<FTreasureData>(RowName, ContextString);
	
	return ItemRow->TreasureName;
}

int32 UC_GameInstance::GetAllItemNum()
{
	if (DataTable)
	{
		TArray<FTreasureData*> OutResult;
		FString ContextString;

		DataTable->GetAllRows<FTreasureData>(ContextString, OutResult);

		return OutResult.Num();
	}

	return 0;
}
