// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SalesforceUpdate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateRequestCompleted, bool, bSuccess);

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceUpdate : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:
	class USalesforce* Salesforce;

	FString ObjectName;

	TMap<FString, FString> Data;

protected:
	void HandleRequestCompleted(bool bSuccess);

public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Salesforce", WorldContext = "WorldContextObject"))
	static USalesforceUpdate* UpdateSalesforceRecord(
		UObject* WorldContextObject,
		class USalesforce* Salesforce,
		const FString& ObjectName,
		const TMap<FString, FString>& Data
	);

	UPROPERTY(BlueprintAssignable)
	FOnUpdateRequestCompleted Completed;
	
};
