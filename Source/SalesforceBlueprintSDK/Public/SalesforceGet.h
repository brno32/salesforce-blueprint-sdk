// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SalesforceGet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetRequestCompleted, const FString&, Payload, bool, bSuccess);

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceGet : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
protected:
	class USalesforce* Salesforce;

	FString ObjectName;

	FString RecordId;

protected:
	void HandleRequestCompleted(const FString& ResponseString, bool bSuccess);

public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Salesforce", WorldContext = "WorldContextObject"))
	static USalesforceGet* GetSalesforceRecord(
		UObject* WorldContextObject,
		class USalesforce* Salesforce,
		const FString& ObjectName,
		const FString& RecordId
	);

	UPROPERTY(BlueprintAssignable)
	FOnGetRequestCompleted Completed;

};
