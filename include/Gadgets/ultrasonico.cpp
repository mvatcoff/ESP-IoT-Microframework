const uint8_t trig = 12;
const uint8_t echo = 14;
int last_ultra =0;

void ultrasonico_setup(){
    pinMode(trig,OUTPUT);
    pinMode(echo,INPUT);
}


void ultrasonico_loop(){
  double tiempo;
  double distancia;
  String s_dist;
  if (millis()-last_ultra>1000){
      digitalWrite(trig,LOW);
      delayMicroseconds(5);
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      tiempo=pulseIn(echo,HIGH);
      distancia = 0.017*tiempo;
      if (distancia > 400) distancia=0;
      s_dist=distancia;


      
      if (luz.client_mqtt->connected())
      luz.client_mqtt->publish("altura", s_dist.c_str());
      

            if (me.Gadgets_WS){
              last_ultra=millis();
              String msg_ws="altura= year:\""+String(millis())+"\", value: "+ s_dist+" ";
              //Serial.println(msg_ws);
              me.websocket.broadcastTXT(msg_ws);
              }
    }
}