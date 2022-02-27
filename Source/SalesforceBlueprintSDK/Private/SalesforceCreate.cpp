// Copyright 2022 brno32. All Rights Reserved.


#include "SalesforceCreate.h"

#include "Dom/JsonObject.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Salesforce.h"

void USalesforceCreate::Activate()
{
    FString Endpoint = Salesforce->BaseUrl + TEXT("sobjects/") + ObjectName + TEXT("/");
    FHttpModule& http = FHttpModule::Get();
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request = http.CreateRequest();

    TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject);

    for (auto& Elem : Data)
    {
        Payload->SetStringField(Elem.Key, Elem.Value);
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Payload.ToSharedRef(), Writer);

	Request->SetVerb("POST");
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

		HandleRequestCompleted(ResponseString, bSuccess);
	});
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Salesforce->SessionId);
    Request->SetHeader(TEXT("X-PrettyPrint"), TEXT("1"));
    Request->SetContentAsString(OutputString);
	Request->ProcessRequest();
}

void USalesforceCreate::HandleRequestCompleted(FString ResponseString, bool bSuccess)
{
	FString RecordId;
	if (bSuccess)
	{
        /* Deserialize object */
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

        if (!JsonObject->TryGetStringField("id", RecordId))
		{
			// While response may be successful, we failed to retrieve the string field
			bSuccess = false;
		}
	}

	Completed.Broadcast(RecordId, bSuccess);
}

USalesforceCreate* USalesforceCreate::CreateSalesforceRecord(
	UObject* WorldContextObject,
	USalesforce* Salesforce,
    const FString& ObjectName,
    const TMap<FString, FString>& Data
)
{
	// Create Action Instance for Blueprint System
	USalesforceCreate* Action = NewObject<USalesforceCreate>();
	Action->Salesforce = Salesforce;
    Action->ObjectName = ObjectName;
	Action->Data = Data;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}