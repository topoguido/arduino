const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
  <meta charset='utf-8'>
  
  <style>
    body {font-size:100%;} 
    #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
    h2 {text-align:center; } 
    p { text-align:center; }
  </style>

	<script>
		const lat =  document.getElementById("lat");
		const long = document.getElementById("long");
		const alt =  document.getElementById("alt");
		const sats = document.getElementById("sats");
		const dateTime = document.getElementById("dateTime");

		setInterval(function() 
		{
		  getData();
		}, 1000); 
		function getData() {

		  var xhttp = new XMLHttpRequest();
		  xhttp.open("GET", "read", true);
		  xhttp.getResponseHeader("Content-type", "text/json");
		  xhttp.onload = function() {
			  const obj = JSON.parse(this.responseText);
			  document.getElementById("lat").innerHTML = obj.lat;
        document.getElementById("long").innerHTML = obj.long;
        document.getElementById("alt").innerHTML = obj.alt;
        document.getElementById("sats").innerHTML = obj.sats;
        document.getElementById("dateTime").innerHTML = obj.dateTime;
		  };
		  
		  xhttp.send();
		  
		}

	</script>
  <title>GPS</title>
 </head>
 <body style="background-color: #f4f0bb;">
	<table style="border-collapse: collapse; width: 60%; height: 14px; margin-left: auto; margin-right: auto;" border="0">
  <tbody>
    <tr style="height: 14px;">
      <td style="width: 32.5665%; text-align: center; height: 14px;">
       <h3><strong><span style="font-size: 18pt;"><em>Matria GPS</em></span><br /></strong></h3>
      </td>
    </tr>
  </tbody>
	<p>&nbsp;</p>
	<table style="border-collapse: collapse; width: 60%; height: 206px; margin-left: auto; margin-right: auto;" border="1">
		<tbody>
			<tr>
				<td style="width: 20%; text-align: center;">Latitud:</td>
				<td style="width: 50%; text-align: center;" id="lat"></td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">Longitud:</td>
				<td style="width: 50%; text-align: center;" id="long"></td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">Altitud:</td>
				<td style="width: 50%; text-align: center;" id="alt"></td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">Sat&eacute;lites:</td>
				<td style="width: 50%; text-align: center;" id="sats"></td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;" >Date/Time:</td>
				<td style="width: 50%; text-align: center;" id="dateTime"></td>
			</tr>
		</tbody>
	</table>
</body>
</html>

)=====";