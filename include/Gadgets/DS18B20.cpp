#include <OneWire.h>
#include <DallasTemperature.h>

void DS18B20_WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length){};
void DS18B20_MQTT_Callback(char* topic, byte* payload, unsigned int length){};
void DS18B20_loop();
void DS18B20_setup();

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(gpio_temperatura);
DallasTemperature sensorDS18B20(&oneWireObjeto);
int last_temp=0;
int ano=2018;

void DS18B20_setup(){
    pinMode(gpio_temperatura, INPUT);
}

void DS18B20_loop(){
    if (millis() - last_temp > 1000) {
        
        last_temp = millis();
        sensorDS18B20.requestTemperatures();
        String msg = String(sensorDS18B20.getTempCByIndex(0));
        me.client_mqtt.publish(topic_tem.c_str(), msg.c_str());
        
        
        if (me.Gadgets_WS){
            String msg_ws="temperatura= year:\""+String(ano++)+"\", value: "+ msg+" ";
            //Serial.println(msg_ws);
            me.websocket.broadcastTXT(msg_ws);}
    }
}                   