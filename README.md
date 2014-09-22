Spark Core and Temboo Light Monitor
-----------------------------------

Here is a project I worked on previous weekend using Spark Core and [Temboo](https://www.temboo.com/). Temboo provides easy and fast access to different APIs. It provides different libraries to connect to Temboo services and access it. There is a great [Spark Core port of Temboo Arduino library](munity.spark.io/t/temboo-works-with-spark/4890) which I decided to use for this project.

This project uses a low cost Light Sensor to constantly monitor the light level. If it is below a particular level then call a Temboo Choreo. For this project I am using Nexmo library `(Library\Nexmo\Voice\CaptureTextToSpeechPrompt)` to call a particular phone number, speaks out a message and get input (digits).  Temboo library provides a easy way to call any Choreo and get the return value. The return value may vary based on the Choreo we are using. It can XML or JSON. In this project I am using JSON return string. 

As everybody knows parsing JSON/XML from a memory limited MCU is not that easy. For this reason Temboo library provides [Output Filters](https://www.temboo.com/arduino/yun/using-output-filters) to make it very easy to process the return values. The above link provides a detailed documentation on how to use Output Filters. The Nexmo's return value in JSON format contains an element '*digits*' which contains the user entered digit. In this sample if the digit is '*1*' then the LED attached to D2 is turn on. When the light level goes above the limit the LED is turned off. Instead of LED we can connect anything more meaningful like a Relay control to turn on the light, etc...

**Using the project**

To use this project, you should configure the Temboo Nexmo library and save it to a profile (https://www.temboo.com/library/Library/Nexmo/Voice/CaptureTextToSpeechPrompt/). You should also have one Nexmo account either trial/paid. Replace the following #defines of `temboo-light.ino` with actual Temboo credentials.

    // Your Temboo account name 
    #define TEMBOO_ACCOUNT "<<temboo account>>"  
    // Your Temboo app key name
    #define TEMBOO_APP_KEY_NAME "<<temboo app key name>>"  
    // Your Temboo app key
    #define TEMBOO_APP_KEY "<<temboo app key>>"  

Replace the Temboo profile name (which is saved previously) at line no. 111 of `temboo-light.ino`

Also you have Replace the Spark Core credentials in the `temboo.html` page.

**Screenshots**

![enter image description here](https://raw.githubusercontent.com/krvarma/SparkCore_Temboo/master/images/IMG_20140923_015730.jpg)

![enter image description here](https://raw.githubusercontent.com/krvarma/SparkCore_Temboo/master/images/IMG_20140923_015823.jpg)

![enter image description here](https://raw.githubusercontent.com/krvarma/SparkCore_Temboo/master/images/web.png)

**Demo Video**

https://www.youtube.com/watch?v=BORNp10bbY0