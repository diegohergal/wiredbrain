
#include <ESP8266WiFi.h>
#include <WifiLocation.h>



const char* googleApiKey = "AIzaSyCl-Mp8VJ-Kdiogb3JKULV9bI3PqhmAmBE";
const char* ssid = "Telefono monky";
const char* password = "majchegd01";
String geolocalizacion = "";
 String latitud = "";
String longitud = "";
String presicion ="";

char server[] = "mail.smtp2go.com"; // The SMTP Server 

WifiLocation location(googleApiKey);
WiFiClient espClient;
//PubSubClient client(espClient); 



void setup()
  {
    
  pinMode(0, INPUT);       // D3 entrada
  Serial.begin(115200);
  
WiFi.begin(ssid, password);              //Inicializamos  la conexión del chip ESP8266 con la red WiFi
  Serial.printf("Conectando a la red: %s\n", WiFi.SSID().c_str());
      while (WiFi.status() != WL_CONNECTED) 
            {  // Verifica el estado de la conexión del NodeMCU cada 0,5s hasta que conecta 
             //digitalWrite(13, HIGH);
             delay(500);
             Serial.print(".");                    
            }
  WiFi.setAutoReconnect(true);             // Realiza la reconexión a la red WiFi si se pierde la conexión
  Serial.println("");
  Serial.println("WiFi conectada");  // Indica que el NodeMCU conectado con la red WiFi 
      //digitalWrite(13, LOW);
 Serial.printf("Conectado a esta IP: http://%s\n", WiFi.localIP().toString().c_str()); 



  
  }

void loop() {
  
   if (digitalRead(0) == 0)
          {
            location_t loc = location.getGeoFromWiFi();

    
    Serial.println("Location request data");
    Serial.println(location.getSurroundingWiFiJson());
    Serial.println("Latitude: " + String(loc.lat, 8));
    Serial.println("Longitude: " + String(loc.lon, 8));
    Serial.println("Accuracy: " + String(loc.accuracy));
    geolocalizacion = String("https://maps.google.com/?q=" + String(loc.lat, 7) + "," + String(loc.lon, 7) + "&z=18&t=m");
    latitud = String(loc.lat, 8);
    longitud = String(loc.lon, 8);
    presicion = String(loc.accuracy);
     Serial.println(geolocalizacion);

     //____________________envio de email__________________

 if (espClient.connect(server, 2525) == 1) 
  {
    Serial.println("connected");//Serial.println(F("connected"));
  } 
  else 
  {
    Serial.println("connection failed");// Serial.println(F("connection failed"));
    //return 0;
  }
  if (!emailResp()) 
    //return 0;
  //
  Serial.println("Sending EHLO"); //Serial.println(F("Sending EHLO"));
  espClient.println("EHLO www.example.com");
  if (!emailResp()) 
    //return 0;
  //
  /*Serial.println(F("Sending TTLS"));
  espClient.println("STARTTLS");
  if (!emailResp()) 
  return 0;*/
  //  
  Serial.println("Sending auth login");  //Serial.println(F("Sending auth login"));
  espClient.println("AUTH LOGIN");
  if (!emailResp()) 
    //return 0;
  //  
  Serial.println("Sending User");// Serial.println(F("Sending User"));
  // Change this to your base64, ASCII encoded username
  /*
  For example, the email address test@gmail.com would be encoded as dGVzdEBnbWFpbC5jb20=
  */
  espClient.println("cHJ1ZWJhcmVkZnJpb0BnbG9iYWxoZWFsdC5jb20="); //base64, ASCII encoded Username
  if (!emailResp()) 
    //return 0;
  //
  Serial.println("Sending Password");//Serial.println(F("Sending Password"));
  // change to your base64, ASCII encoded password
  /*
  For example, if your password is "testpassword" (excluding the quotes),
  it would be encoded as dGVzdHBhc3N3b3Jk
  */
  espClient.println("MmM0bHBvYzB2b1V3");//base64, ASCII encoded Password
  if (!emailResp()) 
    //return 0;
  //
  Serial.println("Sending From");//Serial.println(F("Sending From"));
  // change to sender email address
  espClient.println("MAIL From: pruebaredfrio@globalhealt.com");//espClient.println(F("MAIL From: pruebaredfrio@globalhealt.com"));
  if (!emailResp()) 
    //return 0;
  // change to recipient address
  Serial.println("Sending To");//Serial.println(F("Sending To"));
  espClient.println("RCPT To: sibytvd@gmail.com");//espClient.println(F("RCPT To: sibytvd@gmail.com"));
  if (!emailResp()) 
    //return 0;
  //
  Serial.println("Sending DATA");//Serial.println(F("Sending DATA"));
  espClient.println("DATA");//espClient.println(F("DATA"));
  if (!emailResp()) 
    //return 0;
  Serial.println("Sending email");//Serial.println(F("Sending email"));
  // change to recipient address
  espClient.println("To:  sibytvd@gmail.com, diegohergal@gmail.com");//espClient.println(F("To:  sibytvd@gmail.com"));
  // change to your address
  espClient.println("From: pruebaredfrio@globalhealt.com");// espClient.println(F("From: pruebaredfrio@globalhealt.com"));
  espClient.println("Subject: Ayudame\r\n");// espClient.println(F("Subject: Ayudame\r\n"));
  espClient.println("Por favor ayudame.\n");//espClient.println(F("Por favor ayudame.\n"));
  espClient.println("Estoy en: " + geolocalizacion +" ven.");//espClient.println(F("https://maps.google.com/?q=" + String(loc.lat, 7) + "," + String(loc.lon, 7) + "&z=18&t=m"));
  espClient.println("Llamame o ven inmediatamente.");//espClient.println(F("Llamame o ven inmediatamente."));
  //
  espClient.println(".");// espClient.println(F("."));
  if (!emailResp()) 
  //  return 0;
  //
  Serial.println("Sending QUIT");//  Serial.println(F("Sending QUIT"));
  espClient.println("QUIT");//espClient.println(F("QUIT"));
  if (!emailResp()) 
//    return 0;
  //
  espClient.stop();
  Serial.println("disconnected");//  Serial.println(F("disconnected"));
//  return 1;


     //________________fin envio de email_____________________
     

           }}

 byte emailResp()
{
  byte responseCode;
  byte readByte;
  int loopCount = 0;

  while (!espClient.available()) 
  {
    delay(1);
    loopCount++;
    // Wait for 20 seconds and if nothing is received, stop.
    if (loopCount > 20000) 
    {
      espClient.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  responseCode = espClient.peek();
  while (espClient.available())
  {
    readByte = espClient.read();
    Serial.write(readByte);
  }

  if (responseCode >= '4')
  {
    //  efail();
    return 0;
  }
  return 1;
}

 
