// Copyright brno32. All Rights Reserved.


#include "Salesforce.h"

void USalesforce::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString())
	// // Create a pointer to hold the json serialized data
	// TSharedPtr<FJsonObject> JsonObject;

	// //Create a reader pointer to read the json data
	// TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// //Deserialize the json data given Reader and the actual object to deserialize
	// if (FJsonSerializer::Deserialize(Reader, JsonObject))
	// {
	// 	//Get the value of the json object by field name
	// 	int32 recievedInt = JsonObject->GetIntegerField("customInt");

	// 	//Output it to the engine
	// 	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	// }
}