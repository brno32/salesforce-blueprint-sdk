// Copyright brno32. All Rights Reserved.


#include "SalesforceUpdate.h"

#include "Salesforce.h"

#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void USalesforceUpdate::Activate()
{
    FString Endpoint = Salesforce->BaseUrl + TEXT("sobjects/") + ObjectName + TEXT("/") + RecordId;
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

	Request->SetVerb("PATCH");
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
    Request->SetContentAsString(OutputString);
	Request->ProcessRequest();
}

void USalesforceUpdate::HandleRequestCompleted(bool bSuccess)
{
	Completed.Broadcast(bSuccess);
}

USalesforceUpdate* USalesforceUpdate::UpdateSalesforceRecord(
	UObject* WorldContextObject,
	USalesforce* Salesforce,
    const FString& ObjectName,
    const FString& RecordId,
    const TMap<FString, FString>& Data
)
{
	// Create Action Instance for Blueprint System
	USalesforceUpdate* Action = NewObject<USalesforceUpdate>();
	Action->Salesforce = Salesforce;
    Action->ObjectName = ObjectName;
	Action->Data = Data;
    Action->RecordId = RecordId;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}