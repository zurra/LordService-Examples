// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "FPGamemode.generated.h"

UCLASS()
class FPTEMPLATE_API AFPGamemode : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPGamemode();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool DoWeHaveSessions() const { return FoundSessions.Num() > 0 ;}

	UFUNCTION(BlueprintCallable)
	void FindSessionFromNative();

	UFUNCTION(BlueprintCallable)
	void JoinSessionFromNative();

	TArray<UE::Online::FOnlineSessionId> FoundSessions;
};
