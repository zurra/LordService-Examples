// Fill out your copyright notice in the Description page of Project Settings.


#include "FPTemplate/Public/FPGamemode.h"

#include "Online/Auth.h"
#include "Online/OnlineServices.h"
#include "Online/Sessions.h"
#include "Online/OnlineAsyncOpHandle.h"


// Sets default values
AFPGamemode::AFPGamemode()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void AFPGamemode::FindSessionFromNative()
{
    const UE::Online::IOnlineServicesPtr OnlineServices = UE::Online::GetServices();

    const UE::Online::ISessionsPtr SessionsInterface = OnlineServices->GetSessionsInterface();

    UE::Online::FFindSessions::Params params;
    UE::Online::FAuthGetLocalOnlineUserByPlatformUserId::Params GetAccountParams = {FPlatformMisc::GetPlatformUserForUserIndex(0)};
    const UE::Online::IAuthPtr AuthInterface = OnlineServices->GetAuthInterface();
    UE::Online::TOnlineResult<UE::Online::FAuthGetLocalOnlineUserByPlatformUserId> GetAccountResult = AuthInterface->GetLocalOnlineUserByPlatformUserId(MoveTemp(GetAccountParams));
    if (GetAccountResult.IsOk())
    {
        params.LocalAccountId = GetAccountResult.GetOkValue().AccountInfo->AccountId;
    }
    UE::Online::FFindSessionsSearchFilter Filter;
    Filter.Key = FName("ADDRESS");
    Filter.Value = UE::Online::FSchemaVariant(FString("0.0.0.0"));
    Filter.ComparisonOp = UE::Online::ESchemaAttributeComparisonOp::NotEquals;
    params.Filters.Add(Filter);
    
    SessionsInterface->FindSessions(MoveTemp(params)).OnComplete([this](const UE::Online::TOnlineResult<UE::Online::FFindSessions>& Result)
    {
        if (Result.IsOk())
        {
            const TArray<UE::Online::FOnlineSessionId> OnlineSessionIds = Result.GetOkValue().FoundSessionIds;
            if (OnlineSessionIds.Num() == 0)
                UE_LOG(LogTemp, Warning, TEXT("No sessions found"))
            else
                UE_LOG(LogTemp, Warning, TEXT("found at least one session"))

            FoundSessions = OnlineSessionIds;
        }
        else
        {
            const UE::Online::FOnlineError Error = Result.GetErrorValue();
            UE_LOG(LogTemp, Warning, TEXT("Error on find sessions %s"), *Error.GetLogString())
        }
    });
}

void AFPGamemode::JoinSessionFromNative()
{
    const UE::Online::IOnlineServicesPtr OnlineServices = UE::Online::GetServices();
    const UE::Online::ISessionsPtr SessionsInterface = OnlineServices->GetSessionsInterface();

    UE::Online::FJoinSession::Params Params;
    UE::Online::FAuthGetLocalOnlineUserByPlatformUserId::Params GetAccountParams = {FPlatformMisc::GetPlatformUserForUserIndex(0)};
    const UE::Online::IAuthPtr AuthInterface = OnlineServices->GetAuthInterface();
    UE::Online::TOnlineResult<UE::Online::FAuthGetLocalOnlineUserByPlatformUserId> GetAccountResult = AuthInterface->GetLocalOnlineUserByPlatformUserId(MoveTemp(GetAccountParams));
    if (GetAccountResult.IsOk())
    {
        Params.LocalAccountId = GetAccountResult.GetOkValue().AccountInfo->AccountId;
    }

    Params.SessionId = FoundSessions[0];
    Params.SessionName = "RandomizedName";

    SessionsInterface->JoinSession(MoveTemp(Params)).OnComplete([this](const UE::Online::TOnlineResult<UE::Online::FJoinSession>& Result)
{
    if (Result.IsOk())
    {
        UE_LOG(LogTemp, Warning, TEXT("SUCCESS"))
    }
    else
    {
        const UE::Online::FOnlineError Error = Result.GetErrorValue();
        UE_LOG(LogTemp, Warning, TEXT("Error on joining session %s"), *Error.GetLogString())
    }
});
}
