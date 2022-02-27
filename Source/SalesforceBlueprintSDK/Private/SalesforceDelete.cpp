// Copyright 2022 brno32. All Rights Reserved.


#include "SalesforceDelete.h"

#include "Dom/JsonObject.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "Salesforce.h"

void USalesforceDelete::Activate()
{
    FString Endpoint = Salesforce->BaseUrl + TEXT("sobjects/") + ObjectName + TEXT("/") + RecordId;
    FHttpModule& http = FHttpModule::Get();
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request = http.CreateRequest();

	Request->SetVerb("DELETE");
	Request->SetURL(Endpoint);
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		FString ResponseString = "";
		if (bSuccess)
		{
			ResponseString = Response->GetContentAsString();
		}
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s"), *Response->GetContentAsString())
        }

		HandleRequestCompleted(bSuccess);
	});
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Salesforce->SessionId);
    Request->SetHeader(TEXT("X-PrettyPrint"), TEXT("1"));
	Request->ProcessRequest();
}

void USalesforceDelete::HandleRequestCompleted(bool bSuccess)
{
	Completed.Broadcast(bSuccess);
}

USalesforceDelete* USalesforceDelete::DeleteSalesforceRecord(
	UObject* WorldContextObject,
	USalesforce* Salesforce,
    const FString& ObjectName,
	const FString& RecordId
)
{
	// Create Action Instance for Blueprint System
	USalesforceDelete* Action = NewObject<USalesforceDelete>();
	Action->Salesforce = Salesforce;
    Action->ObjectName = ObjectName;
	Action->RecordId = RecordId;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}
