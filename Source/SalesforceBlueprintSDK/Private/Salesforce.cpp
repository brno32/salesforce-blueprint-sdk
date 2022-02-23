// Copyright brno32. All Rights Reserved.


#include "Salesforce.h"

void USalesforce::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Warning, TEXT("%s"), *ResponseString)

    FString Discardable;
    FString RightSide;
    ResponseString.Split(TEXT("<sessionId>"), &Discardable, &RightSide);

    FString RightSideOfSessionId; // can discard
    RightSide.Split(TEXT("</sessionId>"), &SessionId, &RightSideOfSessionId);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *SessionId)

    FString RightSideOfServerUrl;
    ResponseString.Split(TEXT("<serverUrl>"), &Discardable, &RightSideOfServerUrl);

    FString ServerUrl;
    RightSideOfServerUrl.Split(TEXT("</serverUrl>"), &ServerUrl, &Discardable);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *ServerUrl)

    FString OrgRestDomainWithoutProtocol = ServerUrl.Replace(TEXT("https://"), TEXT("")).Replace(TEXT("http://"), TEXT(""));

    FString OrgRestDomainDirty;
    OrgRestDomainWithoutProtocol.Split(TEXT("/"), &OrgRestDomainDirty, &Discardable);

    OrgRestDomain = OrgRestDomainDirty.Replace(TEXT("-api"), TEXT(""));

    UE_LOG(LogTemp, Warning, TEXT("%s"), *OrgRestDomain)

	BaseUrl = TEXT("https://") + OrgRestDomain + "/services/data/v" + ApiVersion + "/";

    UE_LOG(LogTemp, Warning, TEXT("%s"), *BaseUrl)
}