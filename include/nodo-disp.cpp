#include "nodo-disp.h"

bool nodo::STAWiFi(String ssid_i, String psk_i){
    websocket.close();
    Serial.print(up_time()+"->");
    Serial.print("Estableciendo conexion con: ");
    Serial.println(ssid_i);
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_i, psk_i); //Conexión a la red
    int i=15;
    while (WiFi.status() != WL_CONNECTED && i != 0){
        delay(500);
        Serial.print(".");
        --i;
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED){
        STA_status=true;
        STAssid=WiFi.SSID();
        Serial.print(up_time()+"->");
        Serial.print("WiFi conectado - Local IP: ");
        Serial.println(WiFi.localIP());
        WebServer();
        WebSocket_Connect();
        return true;
        }
    else {
        Serial.println("No se pudo establecer conexion");
        STA_status=false;
    return false;}
}
bool nodo::STAWiFi(){
    websocket.close();
    Serial.print(up_time()+"->");
    Serial.print("Estableciendo conexion con el ultimo punto de acceso: ");
    Serial.println(WiFi.BSSIDstr());
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(); //Conexión a la red
    int i=15;
    while (WiFi.status() != WL_CONNECTED && i != 0){
        delay(500);
        Serial.print(".");
        --i;
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED){
        STA_status=true;
        STAssid=WiFi.SSID();
        Serial.print(up_time()+"->");
        Serial.print("WiFi conectado - Local IP: ");
        Serial.println(WiFi.localIP());
        WebServer();
        WebSocket_Connect();
        return true;
        }
    else {
        Serial.println("No se pudo establecer conexion");
        STA_status=false;
    return false;}
}
void nodo::SetSTA(String ssid_i,String psk_i){
    this->STAssid=ssid_i;
    this->STApsk=psk_i;
}
bool nodo::APWiFi(String APssid, String APpsk){
    websocket.close();
    Serial.print("\n\n");
    Serial.print(up_time()+"->");
    Serial.print("Estableciendo configuración Soft-AP... ");
    WiFi.enableAP(true);
    bool n = WiFi.softAPConfig(local_IP, gateway, subnet);
    Serial.println(n ? "Listo" : "Falló!");
    Serial.print(up_time()+"->");
    Serial.print("Estableciendo modo Soft-AP... ");
    AP_status=WiFi.softAP(APssid, APpsk);
    Serial.println(AP_status ? "Listo" : "Falló!");
    if (AP_status==true){
        Serial.print(up_time()+"->");
        Serial.print("Ingrese desde un navegador web usando la siguiente IP: ");
        Serial.println(WiFi.softAPIP()); //Obtenemos la IP
        WebServer();
        WebSocket_Connect();
        return true;}
    else {
        AP_status=false;
        return false;}
}
void nodo::SetAP(String ssid_i, String psk_i ,IPAddress local_IP_i,IPAddress gateway_i ,IPAddress subnet_i){
    this->APssid=ssid_i;
    this->APpsk=psk_i;
    this->local_IP=local_IP_i;
    this->gateway=gateway_i;
    this->subnet=subnet_i;
}
bool nodo::Nodo_Connect(){
    if (STAWiFi(STAssid,STApsk)){
        
        return true;}
    else {
       if(APWiFi(APssid, APpsk)){
        
        return true;
       }
        return false;
    }
}
bool nodo::Start(){
    Serial.println("\n\nNODO Start");
    SPIFFS.begin();
    bool i = Nodo_Connect();
    return i;
}
void nodo::WebServer(){

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

	server.onNotFound([](AsyncWebServerRequest *request) {
		request->send(400, "text/plain", "Not found");
	});

	server.begin();
    Serial.print(up_time()+"->");
	Serial.println("HTTP server started");

}
void nodo::loop(){
    if (STA_status==false && AP_status==false){
        this->Start();}
    MQTT_KeepAlive();
    this->client_mqtt.loop();
    websocket.loop();
}
String nodo::up_time(){
    int hora=0;
         int min=0;
         int seg=0;
        String uptime; 
         double now=millis()/1000;
         if (now/3600 >= 1 ){
            hora=now/3600;
         }
        if ((now-hora*3600)/60 >=1){
            min= (now-hora*3600)/60;
        }
        if (hora <10) uptime.concat("0"+String(hora)+":");else uptime.concat(String(hora)+":");
        if (min <10) uptime.concat("0"+String(min)+":");else uptime.concat(String(min)+":");
            seg=now-hora*3600-min*60;
        if (seg <10) uptime.concat("0"+String(seg));else uptime.concat(String(seg));
         return uptime;
}
// MQTT
    void nodo::Set_MQTT(IPAddress domain, int port){
        this->MQTT_domain=domain;
        this->MQTT_port=port;
    }   
    bool nodo::MQTT_Connect(IPAddress  domain, int port){
        bool connected;
        mqtt_lastconnect=millis();
        this->MQTT_domain=domain;
        this->MQTT_port=port;
        client_mqtt.setClient(this->NodoClient);
        client_mqtt.setServer(this->MQTT_domain, this->MQTT_port);
        connected = client_mqtt.connect(this->id.c_str());
        this->mqtt_lastconnect=millis();
        return connected;


    }
    bool  nodo::MQTT_Connect(){
        bool connected;
        mqtt_lastconnect=millis();
        PubSubClient client_mqtt(this->NodoClient);
        client_mqtt.setServer(this->MQTT_domain, this->MQTT_port);
        connected = client_mqtt.connect(this->id.c_str());
        this->mqtt_lastconnect=millis();
        return connected;
    }
    void nodo::MQTT_KeepAlive(){
        if ((!client_mqtt.connected()) && ((millis() - mqtt_lastconnect) > 3000) ) {
        
            Serial.println(up_time()+ "->"+"MQTT Keep Alive - State: " + String(mqtt_state_char()));
            Serial.print(up_time()+ "->"+"Estableciendo conexion con el servidor MQTT...");
            
            this->mqtt_lastconnect=millis();
            if(this->MQTT_Connect(this->MQTT_domain,this->MQTT_port)){ 
                Serial.println("LISTO!\n      state:"+ String(mqtt_state_char()));
                Serial.println(up_time()+"->"+"MQTT - Subcribiendo Topics...");
                client_mqtt.subscribe_topics();
            }
            else {
                Serial.println("FALLO!\n      state: "+ String(mqtt_state_char()));
            }
        WS_SendNodoConfig(0,"nodo_mqtt_state?");
        }        
        if (client_mqtt.connected()){
            if ((millis() - mqtt_keepAlive) > 1000) {
            mqtt_keepAlive=millis();
            client_mqtt.publish("Live", up_time().c_str());}
        }
    }
    const char* nodo::mqtt_state_char(){
        switch (client_mqtt.state())
        {
        case MQTT_CONNECTION_TIMEOUT     :return "Connection Time Out"; break;
        case MQTT_CONNECTION_LOST        :return "Connection Lost"; break;
        case MQTT_CONNECT_FAILED         :return "Connection Failed"; break;
        case MQTT_DISCONNECTED           :return "Disconnected"; break;
        case MQTT_CONNECTED              :return "Connected"; break;
        case MQTT_CONNECT_BAD_PROTOCOL   :return "Bad Protocol"; break;
        case MQTT_CONNECT_BAD_CLIENT_ID  :return "Bad Client ID"; break;
        case MQTT_CONNECT_UNAVAILABLE    :return "UNAVAILABLE"; break;
        case MQTT_CONNECT_BAD_CREDENTIALS:return "Bad Credentials"; break;
        case MQTT_CONNECT_UNAUTHORIZED   :return "Connect Unauthorized"; break;
        default:
          return 0;
            break;
        }
    }

//WebSocket

    void nodo::WS_onEvent(WebSocketServerEvent WS_Event){
        websocket.onEvent(WS_Event);
    }
    void nodo::WS_SetNodoConfig(uint8_t num, String payload){
        if (payload.indexOf("scan_ssid=") != -1){
            STAssid= payload.substring(payload.indexOf("scan_ssid=")+10);
            Serial.print("WS Message -> SET SSID:");
            Serial.println(STAssid);
        }
        if (payload.indexOf("psk=") != -1){
            STApsk= payload.substring(payload.indexOf("psk=")+4);
            Serial.print("WS Message -> SET SSID-KEY:");
            Serial.println(STApsk);
        }
        if (payload.indexOf("ssid_AP=") != -1){
            APssid= payload.substring(payload.indexOf("ssid_AP=")+8);
            Serial.print("WS Message -> SET AP-SSID:");
            Serial.println(APssid);
        }
        if (payload.indexOf("psk_AP=") != -1){
            APpsk= payload.substring(payload.indexOf("psk_AP=")+7);
            Serial.print("WS Message -> SET AP-KEY:");
            Serial.println(APpsk);
        }
        if (payload.indexOf("ssid_connect_STA=") != -1){
            bool m=STAWiFi(STAssid, STApsk); 
            if (!m) APWiFi(APssid, APpsk);
        }
        if (payload.indexOf("ssid_connect_AP=") != -1){
            bool n=APWiFi(APssid, APpsk); 
            if (!n) STAWiFi(STAssid, STApsk);
        }
        if (payload.indexOf("mqtt_server=") != -1){
            String r(payload);
            MQTT_domain.fromString(r.substring(payload.indexOf("mqtt_server=")+12));
            Serial.print("WS Message -> SET MQTT Server:");
            Serial.println(MQTT_domain);  
        }
        if (payload.indexOf("mqtt_port=") != -1){
            String v = payload.substring(payload.indexOf("mqtt_port=")+10);
            MQTT_port=v.toInt();
            Serial.print("WS Message -> SET MQTT Port:");
            Serial.println(MQTT_port);
        }
        if (payload.indexOf("gadgets_data=true") != -1){
            this->Gadgets_WS=true;
        }
        if (payload.indexOf("gadgets_data=false") != -1){
            this->Gadgets_WS=false;
        }
        /*if (payload.indexOf("gpio-0=") != -1){
            gpio_0= payload.substring(payload.indexOf("gpio-0=")+7);
            Serial.print("WS Message ->  SET GPIO 0:");
            Serial.println(gpio_0);
        }
        if (payload.indexOf("gpio-2=") != -1){
            gpio_2= payload.substring(payload.indexOf("gpio-2=")+7);
            Serial.print("WS Message ->  SET GPIO 2:");
            Serial.println(gpio_2);
        }*/

    }
    void nodo::WS_SendNodoConfig(uint8_t num, String payload){    
        if (payload.indexOf("scan_ssid?") != -1){
            WiFi.scanDelete();
            int n = WiFi.scanNetworks();
            //delay(10);
            if (n)
                for (int i = (n-1); i >=0 ; i--)
                {
                websocket.broadcastTXT("scan_ssid="+WiFi.SSID(i));
                Serial.print("WS Send Scanned SSID:");
                Serial.println(WiFi.SSID(i));
                }
        }  
        if (payload.indexOf("nodo_mqtt_server_on?") != -1){
            websocket.broadcastTXT("nodo_mqtt_server_on="+ MQTT_domain.toString());
            Serial.print("WS Send MQTT Server:");
            Serial.println(MQTT_domain);
        }
        if (payload.indexOf("nodo_mqtt_port_on?") != -1){
            websocket.broadcastTXT("nodo_mqtt_port_on=" + String(MQTT_port));
            Serial.print("WS Send MQTT Port:");
            Serial.println(MQTT_port);
        }
        if (payload.indexOf("nodo_mqtt_state?") != -1){
            websocket.broadcastTXT("nodo_mqtt_state=" + String(mqtt_state_char()));
        }
        if (payload.indexOf("nodo_ssid_on?") != -1){
            websocket.broadcastTXT("nodo_ssid_on=" + String(STAssid));
            Serial.print("WS Send SSID on:");
            Serial.println(STAssid);
        }
        if (payload.indexOf("nodo_ssid_pwr?") != -1){
            websocket.broadcastTXT("nodo_ssid_pwr=" + String(WiFi.RSSI())+ "dbm");
            Serial.print("WS Send RSSI:");
            Serial.println(WiFi.RSSI());
        }
        if (payload.indexOf("nodo_mode?") != -1){
            websocket.broadcastTXT("nodo_mode=" + String(wifi_get_opmode()));
            Serial.print("WS Send mode:");
            Serial.println(wifi_get_opmode());
        }
        if (payload.indexOf("nodo_ap_ssid?") != -1){
            websocket.broadcastTXT("nodo_ap_ssid=" + String(APssid));
            Serial.print("WS Send AP SSID:");
            Serial.println(APssid);
        }
        if (payload.indexOf("nodo_ap_password?") != -1){
            websocket.broadcastTXT("nodo_ap_password=" + String(APpsk));
            Serial.print("WS Send AP psk:");
            Serial.println(APpsk);
        }
        if (payload.indexOf("nodo_uptime?") != -1){
            websocket.broadcastTXT("nodo_uptime=" + up_time());
            Serial.print("WS Send Up Time:");
            Serial.println(up_time());
        }
        if (payload.indexOf("nodo_looptime?") != -1){
            websocket.broadcastTXT("nodo_looptime= FALTA PROGRAMAR!!!" );
            Serial.print("WS Send Loop Time:");
            Serial.println("FALTA PROGRAMAR!!!");
        }
    
         /* if (payload.indexOf("gpio-0?") != -1){
        websocket.broadcastTXT("gpio-0=" + String(gpio_0));
        Serial.print("WS Send GPIO 0:");
        Serial.println(gpio_0);
        }
      
          if (payload.indexOf("gpio-2?") != -1){
        websocket.broadcastTXT("gpio-2=" + String(gpio_2));
        Serial.print("WS Send GPIO 2:");
        Serial.println(gpio_2);
        }*/
    
    }
    void nodo::WebSocket_Connect(){
        Serial.print(up_time()+"->");
        Serial.println("Web Socket connected...");
        websocket.begin();
        }
    void nodo::WS_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
        char p[length];
        for (size_t i=0; i<length ; i++)
          p[i]=payload[i];
          p[length]='\0';
        String payload_s(p);
        Serial.print(up_time()+"->");
        switch(type) {
            case WStype_DISCONNECTED:
                Serial.printf("[%u] Disconnected!\r\n", num);
            break;
            case WStype_CONNECTED:
            {
                IPAddress ip = websocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);     
            }
            break;
            case WStype_TEXT:
                Serial.printf("[%u] get Text: %s\r\n", num, payload);
                if (payload_s.indexOf("=") != -1)
                    WS_SetNodoConfig(num,payload_s);
                if (payload_s.indexOf("?") != -1)
                    WS_SendNodoConfig(num,payload_s);
            break;
            case WStype_BIN:
                Serial.printf("[%u] get binary length: %u\r\n", num, length);
                hexdump(payload, length);

                // echo data back to browser
                websocket.sendBIN(num, payload, length);
            break;
            default:
                Serial.printf("Invalid WStype [%d]\r\n", type);
            break;
      }
        }
