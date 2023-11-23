// Copyright 2022 brno32. All Rights Reserved.


#include "SalesforceLogin.h"

#include "Dom/JsonObject.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "Salesforce.h"

void USalesforceLogin::Activate()
{
	FString DefaultClientIDPrefix = TEXT("RestForce");
    FString DefaultApiVersion = TEXT("52.0");

    TArray<FStringFormatArg> args;
    args.Add(FStringFormatArg(DefaultClientIDPrefix));
    args.Add(FStringFormatArg(Username));
    args.Add(FStringFormatArg(Password));
    args.Add(FStringFormatArg(SecurityToken));

    FString LoginSoapRequestBody = FString::Format(
		TEXT(R"(<?xml version="1.0" encoding="utf-8" ?>
<env:Envelope
    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:env="http://schemas.xmlsoap.org/soap/envelope/"
    xmlns:urn="urn:partner.soap.sforce.com">
    <env:Header>
        <urn:CallOptions>
            <urn:client>{0}</urn:client>
            <urn:defaultNamespace>sf</urn:defaultNamespace>
        </urn:CallOptions>
    </env:Header>
    <env:Body>
        <n1:login xmlns:n1="urn:partner.soap.sforce.com">
            <n1:username>{1}</n1:username>
            <n1:password>{2}{3}</n1:password>
        </n1:login>
    </env:Body>
</env:Envelope>
)"),
        args
    );

    USalesforce* Salesforce = NewObject<USalesforce>();

    FHttpModule& http = FHttpModule::Get();
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request = http.CreateRequest();

	Request->SetVerb("POST");
	Request->SetURL(TEXT("https://") + Domain + TEXT(".salesforce.com/services/Soap/u/") + DefaultApiVersion);
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		FString ResponseString = Response->GetContentAsString();
		int32 StatusCode = Response->GetResponseCode();

		bool bSuccessfulResponse = bSuccess && StatusCode <= 308;
		if (!bSuccessfulResponse)
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *ResponseString)
		}

		HandleRequestCompleted(ResponseString, bSuccessfulResponse);
	});
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/xml"));
    Request->SetHeader(TEXT("charset"), TEXT("UTF-8"));
    Request->SetHeader(TEXT("SOAPAction"), TEXT("login"));
    Request->SetContentAsString(LoginSoapRequestBody);
	Request->ProcessRequest();
}

void USalesforceLogin::HandleRequestCompleted(FString ResponseString, bool bSuccess)
{
	USalesforce* Salesforce = NewObject<USalesforce>();

	if (bSuccess)
	{
		ApiVersion = TEXT("52.0");
		FString SessionId;
		FString OrgRestDomain;
		FString BaseUrl;

		FString Discardable;
		FString RightSide;
		ResponseString.Split(TEXT("<sessionId>"), &Discardable, &RightSide);

		RightSide.Split(TEXT("</sessionId>"), &SessionId, &Discardable);

		FString RightSideOfServerUrl;
		ResponseString.Split(TEXT("<serverUrl>"), &Discardable, &RightSideOfServerUrl);

		FString ServerUrl;
		RightSideOfServerUrl.Split(TEXT("</serverUrl>"), &ServerUrl, &Discardable);

		FString OrgRestDomainWithoutProtocol = ServerUrl.Replace(TEXT("https://"), TEXT("")).Replace(TEXT("http://"), TEXT(""));

		FString OrgRestDomainDirty;
		OrgRestDomainWithoutProtocol.Split(TEXT("/"), &OrgRestDomainDirty, &Discardable);

		OrgRestDomain = OrgRestDomainDirty.Replace(TEXT("-api"), TEXT(""));

		BaseUrl = TEXT("https://") + OrgRestDomain + "/services/data/v" + ApiVersion + "/";

		Salesforce->SessionId = SessionId;
		Salesforce->OrgRestDomain = OrgRestDomain;
		Salesforce->BaseUrl = BaseUrl;
		Salesforce->ApiVersion = ApiVersion;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Salesforce login attempt failed!"))
	}

	Completed.Broadcast(Salesforce, bSuccess);
}

USalesforceLogin* USalesforceLogin::LoginToSalesforce(
	UObject* WorldContextObject,
	const FString& Username,
	const FString& Password,
	const FString& SecurityToken,
	const FString& Domain,
	const FString& ApiVersion
)
{
	// Create Action Instance for Blueprint System
	USalesforceLogin* Action = NewObject<USalesforceLogin>();
	Action->Username = Username;
	Action->Password = Password;
	Action->SecurityToken = SecurityToken;
	Action->Domain = Domain;
	Action->ApiVersion = ApiVersion;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}