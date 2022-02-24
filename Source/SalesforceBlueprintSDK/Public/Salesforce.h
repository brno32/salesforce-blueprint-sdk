// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Salesforce.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SALESFORCEBLUEPRINTSDK_API USalesforce : public UObject
{
	GENERATED_BODY()

public:
	FString ApiVersion = TEXT("52.0");

/* Variables that get populated during OnLoginResponseReceived */
public:
	FString SessionId;

	FString OrgRestDomain;

	FString BaseUrl;

protected:
	void OnDebugResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

/* CRUD stuff */
public:
	UFUNCTION(BlueprintCallable, Category="Salesforce", meta = (AutoCreateRefTerm = "Client"))
	void Create(
		const FString& ObjectName,
		const TMap<FString, FString>& Data
	);
};
