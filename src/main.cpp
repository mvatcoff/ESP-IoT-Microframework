#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <WiFiClient.h>
#include <string.h>
#include <PubSubClient.h>
#include <nodo-disp.cpp>
#include <gadget.h>
#include <globales.cpp>
#include <utilidades.cpp>
#include <sensores.cpp>



void WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);
void MQTT_Callback(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(9600);
  
  //Configuracion del nodo
    me.id="NODO-001";
    me.SetSTA(ssid, psk);
    me.SetAP(APSSID, APPSK, local_IP, gateway, subnet);
    me.WS_onEvent(WS_Event);
    me.Start();
    me.Set_MQTT(mqtt_server,mqtt_puerto);  
    me.client_mqtt.setCallback(MQTT_Callback);

  //Configuracion de los sensores
    luz_setup();
    DS18B20_setup();
    ultrasonico_setup();

}
void loop() {
  
  me.loop();
  WebSocket_loop();
  
  /// Añadir aqui de bajo todos los eventos WebSockets    ---> [Gadget]_loop()
  luz_loop();
  DS18B20_loop();
  ultrasonico_loop();
  
}
void WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  me.WS_Event(num,type,payload,length);

  if (me.Gadgets_WS){   //Una vez que es TRUE no hay nada que lo vuelva a FALSE, hay que mejorarlo. puede ser keepalive
  /// Añadir aqui de bajo todos los eventos WebSockets    ---> [Gadget]_WS_Event(num,type,payload, length)
  luz_WS_Event(num,type,payload,length);
  DS18B20_WS_Event(num,type,payload,length);
  
  }
}
void MQTT_Callback(char* topic, byte* payload, unsigned int length) {
  print_MQTT_Callback(topic,payload,length);
  
  /// Añadir aqui debajo todos los callbacks MQTT    ---> [Gadget]_MQTT_Callback(topic,payload,length)
  luz_MQTT_Callback(topic,payload,length);
}
