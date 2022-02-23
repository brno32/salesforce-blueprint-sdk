// Copyright brno32. All Rights Reserved.


#include "Salesforce.h"

#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

void USalesforce::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    FString ResponseString = Response->GetContentAsString();

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
}

void USalesforce::OnDebugResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString())
}

void USalesforce::Create(
    const FString& ObjectName,
    const TMap<FString, FString>& Data
)
{
    FString Endpoint = BaseUrl + TEXT("sobjects/") + ObjectName + TEXT("/");
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
    Request->OnProcessRequestComplete().BindUObject(this, &USalesforce::OnDebugResponseReceived);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + SessionId);
    Request->SetHeader(TEXT("X-PrettyPrint"), TEXT("1"));
    Request->SetContentAsString(OutputString);
	Request->ProcessRequest();
}
