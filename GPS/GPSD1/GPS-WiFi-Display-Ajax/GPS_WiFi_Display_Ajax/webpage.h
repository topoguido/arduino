const char webpage[] = R"=====(
<!DOCTYPE html>
<html>
<head>
	<meta name='viewport' content='width=device-width, initial-scale=1.0'/>
	<meta charset='utf-8'>
	<script>
	   function refresh(refreshPeriod) 
	   {
		  setTimeout("location.reload(true);", refreshPeriod);
	   } 
	   window.onload = refresh(2000);
	  </script>
	  <title>Matria GPS</title>
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
				<td style="width: 50%; text-align: center;">%lat%</td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">Longitud:</td>
				<td style="width: 50%; text-align: center;">%long%</td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">Altitud:</td>
				<td style="width: 50%; text-align: center;">%alt%</td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">Sat&eacute;lites:</td>
				<td style="width: 50%; text-align: center;">%sats%</td>
			</tr>
			<tr>
				<td style="width: 20%; text-align: center;">%date%</td>
				<td style="width: 50%; text-align: center;">%time%</td>
			</tr>
		</tbody>
	</table>
</body>
</html>
)=====";
