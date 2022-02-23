// Copyright brno32. All Rights Reserved.


#include "SalesforceBlueprintSDKBPLibrary.h"

#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "Salesforce.h"

// Instantiate Salesforce client
void USalesforceBlueprintSDKBPLibrary::InstantiateSalesforceClient(
    const FString& Username,
    const FString& Password,
    const FString& SecurityToken,
    const FString& Domain
)
{
	FString DefaultClientIDPrefix = TEXT("RestForce");
    FString DefaultApiVersion = TEXT("52.0");

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

    // TMap<FString, FString> LoginSoapRequestHeaders;
    // LoginSoapRequestHeaders.Add(TEXT("content-type"), TEXT("text/xml"));
    // LoginSoapRequestHeaders.Add(TEXT("charset"), TEXT("UTF-8"));
    // LoginSoapRequestHeaders.Add(TEXT("SOAPAction"), TEXT("login"));

    USalesforce* Salesforce = NewObject<USalesforce>();

    FHttpModule& http = FHttpModule::Get();
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request = http.CreateRequest();

	Request->SetVerb("POST");
	Request->SetURL(TEXT("https://") + Domain + TEXT("salesforce.com/services/Soap/u/") + DefaultApiVersion);
    Request->OnProcessRequestComplete().BindUObject(Salesforce, &USalesforce::OnLoginResponseReceived);
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/xml"));
    Request->SetHeader(TEXT("charset"), TEXT("UTF-8"));
    Request->SetHeader(TEXT("SOAPAction"), TEXT("login"));
    // Request->SetContent(LoginSoapRequestBody);
    Request->SetContentAsString(LoginSoapRequestBody);
	Request->ProcessRequest();
};
