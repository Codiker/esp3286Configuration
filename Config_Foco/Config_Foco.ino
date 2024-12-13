#include <ESP8266WiFi.h>

int ledPin =4;

String ssid     = "WiFi";

String password = "Password";

WiFiServer server(80); //objeto de la clase WiFiServer

int estado = 0;



void setup() {

  // Inicia Serial

  Serial.begin(115200);

  Serial.println("\n");

  

pinMode(ledPin,OUTPUT);

digitalWrite( ledPin , LOW);



  // Conexión WIFI

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED ) { 

    delay(500);

    Serial.print(".");

  }

  Serial.println("");

  Serial.println("********************************************");

  Serial.print("Conectado a la red WiFi: ");

  Serial.println(WiFi.SSID());

  Serial.print("IP: ");

  Serial.println(WiFi.localIP());

  Serial.print("macAdress: ");

  Serial.println(WiFi.macAddress());

  Serial.println("*********************************************");

  

  server.begin(); 

  

}



void loop() {

  

  WiFiClient client = server.available(); 



  if(!client){

    return;

  }

  

  Serial.println("Nuevo cliente...");

  while(!client.available()){ 

    delay(1);

  }



  String peticion = client.readStringUntil('\r'); 

  Serial.println(peticion);

  client.flush(); //



  if(peticion.indexOf('LED=ON') != -1)

  {

    estado=1;

  }

  if(peticion.indexOf('LED=OFF') != -1)

  {

    estado=0;

  }

 digitalWrite( ledPin , estado);



  client.println("HTTP/1.1 200 OK");

  client.println("");

  client.println("");

  client.println("");

  client.println(""); 



    //INICIA LA PAGINA



client.println("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>");

client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");

client.println("<title>Control de LED de fremext</title></head>");

client.println("<body style='font-family: Century gothic; width: 800;'><center>");

client.println("<div style='box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 300px; display: inline-block; margin: 30px;'> ");

  client.println("<h1>INTERRUPTOR DE LUZ</h1>");

client.println("<h1>FOCO 1</h1>");



if(estado==1)

  client.println("<h2>El led esta APAGADO</h2>");

else

  client.println("<h2>El led esta ENCENDIDO</h2>");      

client.println("<button style='background-color:red;  color:white; border-radius: 10px; border-color: rgb(0, 255, 0);' ");

client.println("type='button' onClick=location.href='/LED=OFF'><h2>Encender</h2>");

client.println("</button> <button style='background-color:blue; color:white; border-radius: 10px; border-color: rgb(255, 0, 0);' ");

client.println("type='button' onClick=location.href='/LED=ON'><h2>Apagar</h2>");

client.println("</button></div></center></body></html>");







    //FIN DE LA PAGINA



  delay(10);

  Serial.println("Peticion finalizada");

  Serial.println("");  

}
