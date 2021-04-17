
nodo me;

//declaracion de pines 
      const byte gpio_temperatura = 0;
      const byte gpio_luz=2;
      const byte gpio_boton=5;

//estado de los sensores
      bool state_luz=true;
      bool state_boton=false;
      int state_temperatura=0;

//topic sensores
      String topic_tem = "temperatura";
      String topic_luz = "luz";
      String topic_boton= "boton";

//id sensores
      int id_tem;
      int id_luz;
      int id_boton;

//Configuracion Soft-AP
      #define APSSID "ESP-8266"
      #define APPSK "conf.123"

      IPAddress local_IP(192,168,0,50);
      IPAddress gateway(192,168,0,2);
      IPAddress subnet(255,255,255,0);

//Configuracion MQTT
      IPAddress mqtt_server(192,168,1,41);
      int mqtt_puerto = 1882;

//Configuracion Servidor
      WiFiServer server(80);
      String ssid = "lared";
      String psk = "4774624mendoza";
