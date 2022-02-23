// Copyright brno32. All Rights Reserved.


#include "Salesforce.h"

void USalesforce::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    FString ResponseString = Response->GetContentAsString();

    FString Discardable;
    FString RightSide;
    ResponseString.Split(TEXT("<sessionId>"), &Discardable, &RightSide);

    FString RightSideOfSessionId; // can discard
    RightSide.Split(TEXT("</sessionId>"), &SessionId, &RightSideOfSessionId);

    FString RightSideOfServerUrl;
    ResponseString.Split(TEXT("<serverUrl>"), &Discardable, &RightSideOfServerUrl);

    FString ServerUrl;
    RightSideOfServerUrl.Split(TEXT("</serverUrl>"), &ServerUrl, &Discardable);

    FString OrgRestDomainWithoutProtocol = ServerUrl.Replace(TEXT("https://"), TEXT("")).Replace(TEXT("http://"), TEXT(""));

    FString OrgRestDomainDirty;
    OrgRestDomainWithoutProtocol.Split(TEXT("/"), &OrgRestDomainDirty, &Discardable);

    OrgRestDomain = OrgRestDomainDirty.Replace(TEXT("-api"), TEXT(""));

	BaseUrl = TEXT("https://") + OrgRestDomain + "/services/data/v" + ApiVersion + "/";
}