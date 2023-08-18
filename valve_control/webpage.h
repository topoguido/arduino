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
  <button class="button" onclick="send(1, number.value)">ABRIR</button>
  <button class="button" onclick="send(0, number.value)">CERRAR</button><BR>
</div>
 <br>
<div><h2>
  Estado: <span id="state">NA</span>
</h2>
</div>
<br>
<div>
	<h1>Configuracion de pasos</h1>
	<input type="number" name="steps" step="5"></input>
</div>
<script>
function send(valve_sts, steps) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "valve_state?state="+valve_sts + "&steps="+steps, true);
  xhttp.send();
}

</script>
</center>
</body>
</html>
)=====";