#include <WiFi.h>
#include <HTTPClient.h>

const int triggerPin = 5;//Assigning Trigger Pin(Send)
const int echoPin = 18;//Assigning Echo Pin(Recieve)
const char* ssid = "Your-SSID";//Add your WiFi ssid
const char* password =  "Your-Password";//Add your WiFi password
String apiKey = "22222";//Add your Token number that bot has sent you on signal messenger
String phone_number = "+919999999999";//Add your signal app registered phone number (same number that bot send you in url)
String url;//url String will be used to store the final generated URL
String send_message;
#define SOUND_SPEED 0.0034//Velocity of sound in the air at 20°C in cm/uS
long duration;
float disCm;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);//Sends an ultrasound
  pinMode(echoPin, INPUT);//Recieve the reflected Ultrasound
  WiFi.begin(ssid, password);// Try to connect with the given SSID and PSS
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {//Wait until WiFi is connected
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to the WiFi network");//Print wifi connect message

}

void loop() {
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);//Producing a 10uS High Pulse on triggerPin which helps it to emit an Ultrasound
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  disCm = duration * SOUND_SPEED/2;//To Calculate the time taken by the ultrasound to be received by the echoPin

  //Prints the distance in the Serial Monitor
  Serial.print("Distance(cm): ");
  Serial.println(disCm);

  if(disCm < Your_Optimal_Height){
    send_message = "Your Tank has Filled, Kindly Close Water Supply";
    Serial.println("Tank Filled, Sending Message...");
    Serial.println(send_message);
    message_to_signal(send_message);//Send send_message to Signal app
  }

  delay(1000);
}
void  message_to_signal(String message) // user define function to send meassage to Signal app
{
  //adding all number, your api key, your message into one complete url
  url = "https://api.callmebot.com/signal/send.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);

  postData();// calling postData to run the above-generated url once so that you will receive a message.
}

void postData()     //userDefine function used to call api(POST data)
{
  int httpCode;     // variable used to get the responce http code after calling api
  HTTPClient http;  // Declare object of class HTTPClient
  http.begin(url);  // begin the HTTPClient object with generated url
  httpCode = http.POST(url); // Finaly Post the URL with this function and it will store the http code
  if (httpCode == 200)      // Check if the responce http code is 200
  {
    Serial.println("Sent ok."); // print message sent ok message
  }
  else                      // if response HTTP code is not 200 it means there is some error.
  {
    Serial.println("Error."); // print error message.
  }
  http.end();         // After calling API end the HTTP client object.
}

String urlencode(String str)  // Function used for encoding the url
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}

