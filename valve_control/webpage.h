const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style type="text/css">
.button {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}
</style>
<body style="background-color: #f9e79f ">
<center>
<div>
<h1>Control de valvula</h1>
  <button class="button" onclick="sendOpen(document.getElementById("steps"))">ABRIR</button>
  <button class="button" onclick="sendClose(document.getElementById("steps"))">CERRAR</button><BR>
</div>
 <br>
<div><h2>
  Estado: <span id="state">NA</span>
</h2>
</div>
<br>
<div>
	<h1>Configuracion de pasos</h1>
	<input type="number" id="steps" step="5"></input>
</div>
<script>
function sendOpen(steps) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "valve_open?steps="+steps, true);
  xhttp.onload = function() {
    document.getElementById("state").innerHTML = this.responseText;
  };
  
  xhttp.send();
}

function sendClose(steps) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "valve_close?steps="+steps, true);
  xhttp.onload = function() {
    document.getElementById("state").innerHTML = this.responseText;
  };
  xhttp.send();
}

</script>
</center>
</body>
</html>
)=====";