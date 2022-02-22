// Copyright brno32. All Rights Reserved.


#include "SalesforceBlueprintSDKBPLibrary.h"

// Instantiate Salesforce client
void USalesforceBlueprintSDKBPLibrary::InstantiateSalesforceClient(
    const FString& Username,
    const FString& Password,
    const FString& SecurityToken,
    const FString& Domain
)
{
	FString DefaultClientIDPrefix = TEXT("RestForce");

    TArray<FStringFormatArg> args;
    args.Add(FStringFormatArg(DefaultClientIDPrefix));
    args.Add(FStringFormatArg(Username));
    args.Add(FStringFormatArg(Password));
    args.Add(FStringFormatArg(SecurityToken));

    FString LoginSoapRequestBody = FString::Format(
        TEXT(
            "<?xml version='1.0' encoding='utf-8' ?><env:Envelope"
            "    xmlns:xsd='http://www.w3.org/2001/XMLSchema'"
            "    xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'"
            "    xmlns:env='http://schemas.xmlsoap.org/soap/envelope/'"
            "    xmlns:urn='urn:partner.soap.sforce.com'>"
            "<env:Header>"
            "    <urn:CallOptions>"
            "        <urn:client>{0}</urn:client>"
            "        <urn:defaultNamespace>sf</urn:defaultNamespace>"
            "    </urn:CallOptions>"
            "</env:Header>"
            "<env:Body>"
            "    <n1:login xmlns:n1='urn:partner.soap.sforce.com'>"
            "        <n1:username>{1}</n1:username>"
            "        <n1:password>{2}{3}</n1:password>"
            "    </n1:login>"
            "</env:Body>"
            "</env:Envelope>"
        ),
        args
    );

    UE_LOG(LogTemp, Warning, TEXT("%s"), *LoginSoapRequestBody)
};