// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Salesforce.generated.h"

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforce : public UObject
{
	GENERATED_BODY()
	
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
