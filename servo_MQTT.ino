#include <LWiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "string.h"

Servo myservo;  // create servo object to control a servo
Servo myservo1;    // variable to read the value from the analog pin
Servo myservo2;    // variable to read the value from the analog pin
Servo myservo3;    // variable to read the value from the analog pin

char ssid[] = "123";
char password[] = "00000001";
char mqtt_server[] = "10.105.1.232";
char sub_topic[] = "7697 subscribe topic";
char sub_topic1[] = "topic";
char pub_topic[] = "mqtt_test";
char client_Id[] = "7697client-01-";
char pub_msg[] = "hello world!";
char store[]="";
String s,s_next,y; 
int num=0;
String cmp[10]={"0","1","2","3","4","5","6","7","8","9"};
int status = WL_IDLE_STATUS;

WiFiClient mtclient;     
PubSubClient client(mtclient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    myservo.attach(12,500,2400);  // 3
    myservo.write(90);    
    myservo1.attach(11,500,2400);  //2
    myservo1.write(90);    
    myservo2.attach(10,500,2400);  // 1
    myservo2.write(90);    
    myservo3.attach(4,500,2400);  //4
    myservo3.write(90);    
    Serial.begin(9600);
    //while (!Serial) {
         // wait for serial port to connect. Needed for native USB port only
    //}
    setup_wifi();
    client.setServer(mqtt_server,1883);
    client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

  /*  Serial.print("Publish message: ");
    Serial.println(pub_msg);*/
    //client.publish(pub_topic, pub_msg);
    client.setCallback(callback);
    s=String(store);
    for(int i=0;i<10;i++)
    {
      if(s.equals(cmp[i]))
      {
          num=i;
      }
    }
    //Serial.println(num);
    /*a_times(num);*/
    if(s_next!=s && y!=s)
    {
      //Serial.println(s_next);
      s_next=s;
      y=s;
    }
    else
    {
      s_next="";
    }
    Serial.println(s_next);
    if(s_next.equals("0"))
     {
       for(int y = 175;y>=60;y-=5)
        {
          myservo.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(100);
        for(int y = 180;y>=55;y-=5)
        {
          myservo1.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(100);
        for(int y = 180;y>=60;y-=5)
        {
          myservo2.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(100);
     }
     else if(s_next.equals("1"))
     {
      for(int y = 175;y>=60;y-=5)
        {
          myservo.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(100);
       for(int y = 180;y>=55;y-=5)
        {
          myservo1.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(100);
        for(int y = 175;y>=60;y-=5)
        {
          myservo3.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(100);
     }
     else if(s_next.equals("2"))
     {
       for(int y = 180;y>=60;y-=5)
        {
          myservo2.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(1000);
     }
     else if(s_next.equals("3"))
     {
       for(int y = 175;y>=60;y-=5)
        {
          myservo3.write(y);
          delay(50);                           // waits for the servo to get there 
        }
        delay(1000);
     }
     else 
     {
        myservo.write(145);
        myservo1.write(150);
        myservo2.write(155);
        myservo3.write(150);
        delay(5000);
     }
     Serial.println(s_next);
  }
}


void printWifiStatus() {                     //print Wifi status
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void setup_wifi() {                       //setup Wifi
   // attempt to connect to Wifi network:
   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("Connected to wifi");
    printWifiStatus();
}

void callback(char* topic, byte* payload, unsigned int length) {   //MQTT sub
  Serial.print("Input Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    store[i]=(char)payload[i];
  }
   //Serial.print("123456789");
}

void reconnect() {  //reconnect MQTT
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = client_Id;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pub_topic, client_Id);
      // ... and resubscribe
      client.subscribe(sub_topic);
      client.subscribe(sub_topic1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void a_times(int i)
{
  for(int x = 0; x<i ; x++)
  {
     //Serial.println(i);
     for(int y = 175;y>=75;y-=5)
        {
          myservo.write(y);
          delay(100);                           // waits for the servo to get there 
        }
  }
//  store="";
}

