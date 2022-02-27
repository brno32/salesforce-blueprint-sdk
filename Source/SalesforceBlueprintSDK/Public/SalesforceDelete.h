// Copyright 2022 brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SalesforceDelete.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteRequestCompleted, bool, bSuccess);

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceDelete : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:
	class USalesforce* Salesforce;

	FString ObjectName;

	FString RecordId;

protected:
	void HandleRequestCompleted(bool bSuccess);

public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Salesforce", WorldContext = "WorldContextObject"))
	static USalesforceDelete* DeleteSalesforceRecord(
		UObject* WorldContextObject,
		class USalesforce* Salesforce,
		const FString& ObjectName,
		const FString& RecordId
	);

	UPROPERTY(BlueprintAssignable)
	FOnDeleteRequestCompleted Completed;
	
};
