    var mqtt_server = document.getElementById('mqtt_server');
    var mqtt_puerto = document.getElementById('mqtt_puerto');
    var topic_gpio0 = document.getElementById('gpio-0');
    var topic_gpio2 = document.getElementById('gpio-2');
    var nodo_ssid_on = document.getElementById('nodo_ssid_on');
    var nodo_ssid_pwr = document.getElementById('nodo_ssid_pwr');
    var nodo_mode = document.getElementById('nodo_mode');
    var nodo_ap_ssid = document.getElementById('nodo_ap_ssid');
    var nodo_ap_password = document.getElementById('nodo_ap_password');
    var nodo_mqtt_server_on = document.getElementById('nodo_mqtt_server_on');
    var nodo_mqtt_port_on = document.getElementById('nodo_mqtt_port_on');
    var nodo_uptime = document.getElementById('nodo_uptime');
    var nodo_looptime = document.getElementById('nodo_looptime');
    var ssid = document.getElementById('ssid');
    var psk= document.getElementById('psk');
    var ssid_AP=document.getElementById('ssid_AP');
    var psk_AP=document.getElementById('psk_AP');

        websock= new WebSocket('ws://' + window.location.host + ':81/');
        websock.onopen=function(evt){
                      console.log('websock open');
                      
                      websock.send('nodo_ssid_on?');
                      websock.send('nodo_ssid_pwr?');
                      websock.send('nodo_mode?');
                      websock.send('nodo_ap_ssid?');
                      websock.send('nodo_ap_password?');
                      websock.send('nodo_mqtt_server_on?');
                      websock.send('nodo_mqtt_port_on?');
                      websock.send('nodo_uptime?');
                      websock.send('nodo_looptime?');
                      websock.send("gpio-0?");
                      websock.send("gpio-2?");
                      websock.send('scan_ssid?');};

        websock.onclose=function(evt){console.log('websock close');};  
        websock.onerror=function(evt){console.log(evt);};

        websock.onmessage = function(evt){
          
          var entrada = evt.data;
          
          if (entrada.indexOf('scan_ssid=') != -1){
            ssid.insertAdjacentHTML("beforeend","<option>" + entrada.slice(entrada.indexOf("=")+1)+ "</option>");
            console.log("Setting Scan SSID Option...",evt.data)
          }
          if (entrada.indexOf('nodo_ssid_on') != -1){
            nodo_ssid_on.innerHTML="SSID: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_ssid_on...")
          }
          if (entrada.indexOf('nodo_ssid_pwr') != -1) {
            nodo_ssid_pwr.innerHTML="PWR: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_ssid_pwr...")
          }
          if (entrada.indexOf('nodo_mode') != -1) {
            nodo_mode.innerHTML="Mode: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_mode...")
          }
          if (entrada.indexOf('nodo_ap_ssid') != -1) {
            nodo_ap_ssid.innerHTML="AP SSID: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_ap_ssid...")
          }
          if (entrada.indexOf('nodo_ap_password') != -1) {
            nodo_ap_password.innerHTML="AP PassWord: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_ap_password...")
          }
          if (entrada.indexOf('nodo_mqtt_server_on') != -1) {
            nodo_mqtt_server_on.innerHTML="MQTT Server: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_mqtt_server_on...")
          }
          if (entrada.indexOf('nodo_mqtt_port_on') != -1) {
            nodo_mqtt_port_on.innerHTML="MQTT Port: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_mqtt_port_on...")
          }
          if (entrada.indexOf('nodo_mqtt_state') != -1) {
            nodo_mqtt_state.innerHTML="MQTT State: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_mqtt_state...")
          }
          if (entrada.indexOf('nodo_uptime') != -1) {
            nodo_uptime.innerHTML="Up Time: " + entrada.slice(entrada.indexOf("=")+1)
            
          }
          if (entrada.indexOf('nodo_looptime') != -1) {
            nodo_looptime.innerHTML="Loop Time: "+entrada.slice(entrada.indexOf("=")+1)
            console.log("Setting nodo_looptime...")
          }
        }
          

          function bottonclick_config(evt){
            console.log("Config Botton Click",evt)
            websock.send("mqtt_server="+mqtt_server.value)
            websock.send("mqtt_port="+mqtt_puerto.value)
            websock.send("nodo_mqtt_server_on?")
            nodo_mqtt_server_on.innerHTML="MQTT Server: ";
            nodo_mqtt_port_on.innerHTML="MQTT Port: ";
            websock.send("nodo_mqtt_port_on?")
          }
          function bottonclick_connect(evt){
            console.log("Connect Botton Click",evt)
            if ((psk.value.length) >= 8 && psk.value.length<=63 ){
              websock.send("scan_ssid="+ ssid.value)
              websock.send("psk="+ psk.value)
              websock.send("ssid_connect_STA=")
              nodo_ssid_on.innerHTML="SSID: "
              window.alert("Station mode on, set your WiFi at: "+ ssid.value);
            }
            else
            {
              window.alert("FAIL - Passpharase must be between 8 and 63 characters!")
            }
        }
          function bottonclick_connect_AP(evt){
            console.log("Connect AP Botton Click",evt)
            if ((psk_AP.value.length) >= 8 && psk_AP.value.length<=63 ){
              websock.send("ssid_AP="+ ssid_AP.value)
              websock.send("psk_AP="+ psk_AP.value)
              websock.send("ssid_connect_AP=")
              nodo_ap_ssid.innerHTML="AP SSID: "
              nodo_ap_password.innerHTML="AP PassWord: "
              window.alert("Acces Point mode on, set your WiFi at: "+ ssid_AP.value);
            }
            else
            {
              window.alert("FAIL - Passpharase must be between 8 and 63 characters!")
            }
        }
          