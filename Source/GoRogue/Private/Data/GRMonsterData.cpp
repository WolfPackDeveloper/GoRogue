// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/GRMonsterData.h"

FPrimaryAssetId UGRMonsterData::GetPrimaryAssetId() const
{
	//return FPrimaryAssetId("Monsters", GetName());
	return FPrimaryAssetId(FPrimaryAssetType("Monsters"), FName(GetName()));
}
