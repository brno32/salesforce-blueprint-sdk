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
	FString SessionId;
	FString OrgRestDomain;
	FString BaseUrl;

};
