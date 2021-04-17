
class nodo{
    public:
        String nombre;
        String id;
        
        String APssid;
        String APpsk;
        bool AP_status=false;
        String STAssid;
        String STApsk;
        bool STA_status=false;

        IPAddress local_IP;
        IPAddress gateway;
        IPAddress subnet;
        
        WiFiClient NodoClient;
        
        //MQTT
        IPAddress  MQTT_domain;
        uint16_t MQTT_port;
        int mqtt_lastconnect;
        int mqtt_keepAlive;
        PubSubClient client_mqtt;
        const char* mqtt_state_char();

        WebSocketsServer websocket = WebSocketsServer(81);
        typedef void (*WebSocketServerEvent)(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
        
        AsyncWebServer server= AsyncWebServer(80);

        bool Start();
        String up_time();

        void SetSTA(String,String);
        void SetAP(String, String, IPAddress, IPAddress, IPAddress);
        bool APWiFi(String APssid, String APpsk);
        bool STAWiFi(String STAssid, String STApsk);
        bool STAWiFi();
        
        void loop();
        void WebServer();
        bool Nodo_Connect();

        void Set_MQTT(IPAddress domain, int port);
        bool MQTT_Connect(IPAddress domain, int port);
        bool MQTT_Connect();
        void MQTT_KeepAlive();
        void MQTT_KAL_Subscriptions();
        

        void WebSocket_Connect();
        void WS_SetNodoConfig(uint8_t num, String payload);
        void WS_SendNodoConfig(uint8_t num, String payload);
        void WS_onEvent(WebSocketServerEvent);//Le asigna al objeto websocket el evento WS_Event
        void WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length); //Procesa los datos que llegan por el WebSocket
        void WebSocket_Add_Event(WebSocketServerEvent); //añade eventos a WS_Event
        bool Gadgets_WS=false;
};