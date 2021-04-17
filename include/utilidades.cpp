
unsigned long ult=0;
unsigned long now=0;
unsigned long ult2=0;
int last_nodo_uptime=0;


void loop_time(){
    //tiempo del sketch
    ult=millis()-now;
    if ((int)ult2 != (int)ult && ult>1){Serial.print("sketch loop time:"); Serial.print(ult); Serial.println("ms");}
    ult2=ult;
    now=millis();
}
String up_time(){
    int hora=0;
         int min=0;
         int seg=0;
        String uptime; 
         double now=millis()/1000;
         if (now/3600 >= 1 ){
            hora=now/3600;
         }
         if (hora <10) uptime.concat("0"+String(hora)+":");else uptime.concat(String(hora)+":");
        if ((now-hora*3600)/60 >=1){
            min= (now-hora*3600)/60;
        }
        if (min <10) uptime.concat("0"+String(min)+":");else uptime.concat(String(min)+":");
            seg=now-hora*3600-min*60;
            if (seg <10) uptime.concat("0"+String(seg));else uptime.concat(String(seg));
         return uptime;
}
void print_MQTT_Callback(char* topic, byte* payload, unsigned int length){
    Serial.print(up_time()+"->");
    Serial.print("MQTT Mensaje recibido [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      }
    Serial.println();
}
void WebSocket_loop(){

  if (millis()- last_nodo_uptime > 1000){
    last_nodo_uptime=millis();
    me.websocket.broadcastTXT("nodo_uptime=" + up_time());
  }
}
