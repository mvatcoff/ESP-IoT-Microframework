#include "gadget.h"

void gadget::SetConf(int gpio_in,String topic_in,int id_in,int pin_mode_in){
    gpio=gpio_in;
    topic=topic_in;
    id=id_in;
    pin_mode=pin_mode_in;
    pinMode(gpio, pin_mode); 
}
void gadget::SetConf(int gpio_in,String topic_in,int id_in,int pin_mode_in, PubSubClient * client_mqtt_in){
    gpio=gpio_in;
    topic=topic_in;
    id=id_in;
    pin_mode=pin_mode_in;
    client_mqtt=client_mqtt_in;
    pinMode(gpio, pin_mode); 
}

void gadget::pin(int pin_state_in){
    digitalWrite(gpio,pin_state_in);
    pin_state=pin_state_in;
}
void gadget::add_MQTT_topic(const char * topic){
    client_mqtt->add_topic(topic);
}

void gadget::loop(){
    sensor_loop();
}
void gadget::SetLoop(SENSOR_LOOP){
    this->sensor_loop = sensor_loop;
}
