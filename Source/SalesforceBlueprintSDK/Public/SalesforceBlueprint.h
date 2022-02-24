// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SalesforceBlueprint.generated.h"

// Event that will be the 'Completed' exec wire in the blueprint node along with all parameters as output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHttpRequestCompleted, class USalesforce*, Salesforce, bool, bSuccess);

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceBlueprint : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
protected:
	void HandleRequestCompleted(FString ResponseString, bool bSuccess);

public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Salesforce", WorldContext = "WorldContextObject"))
	static USalesforceBlueprint* ConnectToSalesforce(
		UObject* WorldContextObject,
		const FString& Username,
		const FString& Password,
		const FString& SecurityToken,
		const FString& Domain
	);

	UPROPERTY(BlueprintAssignable)
	FOnHttpRequestCompleted Completed;

protected:
	FString Username;
	FString Password;
	FString SecurityToken;
	FString Domain;

};
