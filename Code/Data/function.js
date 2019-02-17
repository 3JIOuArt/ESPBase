var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}

function handleServerResponse(){
    document.body.style.backgroundColor="rgb("+jsonResponse.rgb+")";
   }

function loadJson(jsonName){
 if(xmlHttp.readyState===0 || xmlHttp.readyState==4){
  xmlHttp.open("PUT",jsonName+".json",true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
}

function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
 handleServerResponse();
}

function val(id){
 var v = document.getElementById(id).value;
 return v;
}

 function restart(submit,texts){
    if (confirm(texts)) {
	   server = "/restart?device=ok";
     send_request(submit,server);
     return true;
    }
    else {
     return false;
    }
      }

      function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("POST", server, true);
 request.send();
}
