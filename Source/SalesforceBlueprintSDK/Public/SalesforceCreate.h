// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SalesforceCreate.generated.h"

// Event that will be the 'Completed' exec wire in the blueprint node along with all parameters as output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateRequestCompleted, const FString&, RecordId, bool, bSuccess);

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceCreate : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:
	class USalesforce* Salesforce;

	FString ObjectName;

	TMap<FString, FString> Data;

protected:
	void HandleRequestCompleted(FString ResponseString, bool bSuccess);

public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Salesforce", WorldContext = "WorldContextObject"))
	static USalesforceCreate* CreateSalesforceRecord(
		UObject* WorldContextObject,
		class USalesforce* Salesforce,
		const FString& ObjectName,
		const TMap<FString, FString>& Data
	);

	UPROPERTY(BlueprintAssignable)
	FOnCreateRequestCompleted Completed;
	
};
