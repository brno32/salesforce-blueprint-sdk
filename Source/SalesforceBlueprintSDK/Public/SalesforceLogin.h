// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SalesforceLogin.generated.h"

// Event that will be the 'Completed' exec wire in the blueprint node along with all parameters as output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHttpRequestCompleted, class USalesforce*, Salesforce, bool, bSuccess);

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceLogin : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
protected:
	void HandleRequestCompleted(FString ResponseString, bool bSuccess);

public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Salesforce", WorldContext = "WorldContextObject"))
	static USalesforceLogin* LoginToSalesforce(
		UObject* WorldContextObject,
		const FString& Username,
		const FString& Password,
		const FString& SecurityToken,
		const FString& Domain,
		const FString& ApiVersion = TEXT("52.0")
	);

	UPROPERTY(BlueprintAssignable)
	FOnHttpRequestCompleted Completed;

protected:
	FString Username;
	FString Password;
	FString SecurityToken;
	FString Domain;
	FString ApiVersion = TEXT("52.0");

};
