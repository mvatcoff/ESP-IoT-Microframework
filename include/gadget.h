#ifndef gadget_h
#define gadget_h

#include <PubSubClient.h>
#define SENSOR_LOOP std::function<void()> sensor_loop

class gadget {
    public:
        uint8_t gpio;
        String topic;
        int id;
        int pin_mode;
        int pin_state;
        PubSubClient * client_mqtt;
        SENSOR_LOOP;



                void SetConf(int gpio,String topic,int id,int pin_mode);
                void SetConf(int gpio,String topic,int id,int pin_mode, PubSubClient *);
                void pin(int pin_state);
                void add_MQTT_topic(const char *);
                void loop();
                void SetLoop(SENSOR_LOOP);

                                    

};

#endif