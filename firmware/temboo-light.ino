#include "application.h"
// This #include statement was automatically added by the Spark IDE.
#include "Temboo/Temboo.h"

#define MAX_PHONE_NUMBER 16
#define MAX_MESSAGE 64
#define SEP_CHAR '|'

const int ldrPin = A7;
const int ledPin = D2;

#define TEMBOO_ACCOUNT "krvarma"  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME "myFirstApp"  // Your Temboo app key name
#define TEMBOO_APP_KEY "4bd126a7-1209-4044-a"  // Your Temboo app key

TCPClient client;

int sensorValue = 0;
int thresholdLimit = -1;
char szNotificationNumber[MAX_PHONE_NUMBER];
char szMessage[MAX_MESSAGE];
bool isTembooCalled = false;

int setNotificationNumber(String strPhone){
    int retVal = 0;
    
    if(strPhone.length() <= MAX_PHONE_NUMBER){
        strPhone.toCharArray(szNotificationNumber, MAX_PHONE_NUMBER);
        
        Serial.print("Phone: ");
        Serial.println(strPhone);
        
        retVal = 1;
    }
    
    return retVal;
}

int setNotificationMessage(String strMessage){
    int retVal = 0;
    
    if(strMessage.length() <= MAX_MESSAGE){
        strMessage.toCharArray(szMessage, MAX_MESSAGE);
        
        Serial.print("Message: ");
        Serial.println(strMessage);
        
        retVal = 1;
    }
    
    return retVal;
}

int setThresholdLimit(String strLimit){
    thresholdLimit = strLimit.toInt();
    
    Serial.print("Threshold Limit: ");
    Serial.println(thresholdLimit);
    
    return 1;
}

void setup() {
    Serial.begin(9600);
    
    pinMode(ldrPin, INPUT);
    pinMode(ledPin, OUTPUT);
    
    Spark.function("setlimit", setThresholdLimit);
    Spark.function("setmsg", setNotificationMessage);
    Spark.function("setphone", setNotificationNumber);
}

void loop() {
    sensorValue = map(analogRead(ldrPin), 0, 4095, 0, 100);
    
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);
    
    // Check if we have a valid threshold limit
    if(-1 != thresholdLimit){
        // Check if above the limit
        if(sensorValue > thresholdLimit){
            digitalWrite(ledPin, LOW);
            
            isTembooCalled = false;
        }
        else if(!isTembooCalled){ // If we are below the limit and not notified once
            isTembooCalled = true;
            
            int input = callTembooAndWaitForDigit(sensorValue);
            
            digitalWrite(ledPin, (1 == input) ? HIGH : LOW);
        }
    }
    
    delay(2000);
}

int callTembooAndWaitForDigit(int sensorValue){
    Serial.println("Calling Temboo...");
    
    TembooChoreo tembooChoreo(client);

    // Set Temboo account credentials
    tembooChoreo.setAccountName(TEMBOO_ACCOUNT);
    tembooChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    tembooChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set profile to use for execution
    tembooChoreo.setProfile("SendMessageAndGetText");

    // Set Choreo inputs
    String messageText = String("The current sensor value is: ") + String(sensorValue);
  
    tembooChoreo.addInput("ByeText", "Thanks and Good Bye.");
    tembooChoreo.addInput("MaxDigits", "1");
    tembooChoreo.addInput("Text", szMessage);
    tembooChoreo.addInput("To", szNotificationNumber);
  
    tembooChoreo.addOutputFilter("digits", "/digits", "CallbackData");

    // Identify the Choreo to run
    tembooChoreo.setChoreo("/Library/Nexmo/Voice/CaptureTextToSpeechPrompt");

    // Run the Choreo
    unsigned int returnCode = tembooChoreo.run();

    // A return code of zero means everything worked
    if (returnCode == 0) {
        Serial.println("Done!\n");
    } 
    
    String result = "";
    
    while (tembooChoreo.available()) {
        char c = tembooChoreo.read();
        
        result += c;
    }
    
    // Retrieve the returned digit variable,
    // For more information about processing the return values goto he link
    // https://www.temboo.com/arduino/yun/using-output-filters
    int valueStart = result.lastIndexOf(0x1F);
    int valueEnd = result.lastIndexOf('\n');
    int digitEntered = 0;
    
    if(-1 != valueStart && -1 != valueEnd){
        digitEntered = result.substring(valueStart + 1, valueEnd).toInt();
        
        Serial.println(digitEntered);
    }
    
    Serial.println(result);
    Serial.println();

    tembooChoreo.close();  
    
    return digitEntered;
}