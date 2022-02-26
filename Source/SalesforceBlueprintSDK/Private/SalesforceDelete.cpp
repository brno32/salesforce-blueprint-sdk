// Copyright brno32. All Rights Reserved.


#include "SalesforceDelete.h"

#include "Salesforce.h"

#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void USalesforceDelete::Activate()
{
    FString Endpoint = Salesforce->BaseUrl + TEXT("sobjects/") + ObjectName + TEXT("/");
    FHttpModule& http = FHttpModule::Get();
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request = http.CreateRequest();

	Request->SetVerb("DELETE");
	Request->SetURL(Endpoint);
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
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
    const FString& ObjectName
)
{
	// Create Action Instance for Blueprint System
	USalesforceDelete* Action = NewObject<USalesforceDelete>();
	Action->Salesforce = Salesforce;
    Action->ObjectName = ObjectName;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}
