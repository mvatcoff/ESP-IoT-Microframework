#include <gadget.cpp>

void luz_WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void luz_MQTT_Callback(char* topic, byte* payload, unsigned int length);
void luz_loop();
void luz_setup();

gadget luz;
int last_boton=0;
bool luz_state=false;
void luz_WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  //
}
void luz_MQTT_Callback(char* topic, byte* payload, unsigned int length){
  String topic_s = String(topic);
  if (topic_s.indexOf(topic_luz) != -1){
    if ((char)payload[0] == '1'){
      digitalWrite(gpio_luz, LOW);
      luz_state=true;
      Serial.println(up_time()+"->Luz on");  
      }
    if ((char)payload[0] == '0') {
      digitalWrite(gpio_luz, HIGH);
      Serial.println(up_time()+"->Luz off"); 
      luz_state=false;
      }
  }
}
void luz_loop(){
  if (digitalRead(gpio_boton) && (millis() - last_boton > 500)){
    last_boton=millis();
    //boton=true;
    if (luz_state){
      digitalWrite(gpio_luz, HIGH);
      me.client_mqtt.publish(topic_luz.c_str(), "0");
      luz_state=false;
      Serial.println(up_time() + "-> Botton Click - Luz off");
      }
    else{
      digitalWrite(gpio_luz, LOW); 
      me.client_mqtt.publish(topic_luz.c_str(), "1");
      luz_state=true;
      Serial.println(up_time() + "-> Botton Click - Luz on");
      }
    }
}
void luz_setup(){
    luz.SetConf(gpio_luz,topic_luz,id_luz,OUTPUT,&me.client_mqtt);
    luz.SetLoop(luz_loop);
    luz.add_MQTT_topic(topic_luz.c_str());
    
    pinMode(gpio_luz, OUTPUT);
    pinMode(gpio_boton, INPUT);
}
