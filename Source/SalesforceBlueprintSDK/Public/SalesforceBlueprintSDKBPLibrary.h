// Copyright brno32. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SalesforceBlueprintSDKBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SALESFORCEBLUEPRINTSDK_API USalesforceBlueprintSDKBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="Salesforce", meta = (AutoCreateRefTerm = "Client"))
	static void InstantiateSalesforceClient(
		const FString& Username,
		const FString& Password,
		const FString& SecurityToken,
		const FString& Domain
	);
	
};
