var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}

function load(){
 if(xmlHttp.readyState===0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/configs.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
}

function loadi(jsonName){
 if(xmlHttp.readyState===0 || xmlHttp.readyState==4){
  xmlHttp.open("PUT",jsonName+".json",true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
 jsonName = 0;
}

function loadNRF(){
 if(xmlHttp.readyState===0 || xmlHttp.readyState==4){
  xmlHttp.open("PUT","/nrf.json",true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
}

function loadi2c(){
 if(xmlHttp.readyState===0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/i2c.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
}


function loadGet(){
 if(xmlHttp.readyState===0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/HTTPGet.json',true);
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
function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("POST", server, true);
 request.send();
 save_status(submit,request);
}
function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Подождите...';
 submit_disabled(true);
}
function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}
function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
function move() {
  var elem = document.getElementById("myBar");
  var width = 0;
  var id = setInterval(frame, 150);
  function frame() {
    if (width >= 100) {
      clearInterval(id);
      location.reload();
    } else {
      width++;
      elem.style.width = width + '%';
    }
  }
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

      $(document).ready( function() {
    $(".file-upload input[type=file]").change(function(){
         var filename = $(this).val().replace(/.*\\/, "");
         $("#filename").val(filename);
    });
});
