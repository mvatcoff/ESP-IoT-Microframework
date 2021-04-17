var grafico_temperatura=new Morris.Line({
    // ID of the element in which to draw the chart.
    element: 'grafica_temperatura',
    // Chart data records -- each entry in this array corresponds to a point on
    // the chart.
    data: temperatura,
    // The name of the data record attribute that contains x-values.
    xkey: 'time',
    // A list of names of data record attributes that contain y-values.
    ykeys: ['value'],
    // Labels for the ykeys -- will be displayed when you hover over the
    // chart.
    labels: ['Value']
  });

  var grafico_altura=new Morris.Line({
    // ID of the element in which to draw the chart.
    element: 'grafica_altura',
    // Chart data records -- each entry in this array corresponds to a point on
    // the chart.
    data: altura,
    // The name of the data record attribute that contains x-values.
    xkey: 'time',
    // A list of names of data record attributes that contain y-values.
    ykeys: ['value'],
    // Labels for the ykeys -- will be displayed when you hover over the
    // chart.
    labels: ['Value']
  });
  var altura=[];
  var temperatura=[];
  var temperatura_show=[];
  var altura_show=[];
  

    websock= new WebSocket('ws://' + '192.168.1.7' + ':81/');
    websock.onopen=function(evt){
                      console.log('websock open');
                      websock.send('gadgets_data=true');};
    websock.onclose=function(evt){
      websock.send('gadgets_data=false');
      console.log('websock close',evt);};  
    websock.onerror=function(evt){console.log(evt);};
    
    websock.onmessage = function(evt){
        
        var id_in=evt.data.substring(0,evt.data.indexOf('='));
        var id_data=evt.data.slice(evt.data.indexOf('=')+1)
        var time_d =id_data.substring(7,id_data.indexOf(",")-1);
        var value_d=id_data.substring(id_data.indexOf("value:")+7);
        
        console.log("websock msg: ", evt.data);

        if (id_in.indexOf('temperatura') != -1){

          var range_tem=document.getElementById("range-chart-1").value;
          //console.log(range_tem);

          var time_now=Date.now();
          temperatura.push({
            time: time_now,
            value:Number(value_d)
          });

          if (temperatura.length<=range_tem) 
                temperatura_show=temperatura;
          else{
                temperatura_show=temperatura.slice(temperatura.length-1-range_tem,temperatura.length-1);}
          
          if (temperatura.length >= 500)
                temperatura.shift();

          //console.log("temperatura",temperatura_show);
          grafico_temperatura.setData(temperatura_show);
          
        }
        if (id_in.indexOf('altura') != -1){

          var range=document.getElementById("range-chart-2").value;
          //console.log(range);

          var time_now=Date.now();
          altura.push({
            time: time_now,
            value:Number(value_d)
          });
         
          if (altura.length<=range) 
          altura_show=altura;
          else{
            altura_show=altura.slice(altura.length-1-range,altura.length-1);}
    
           if (altura.length >= 500)
              temperatura.shift();

          //console.log("altura",altura_show);
          grafico_altura.setData(altura_show);
        }
      
    }
