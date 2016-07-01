<html>
<body><form action="upload.php" method="post"
enctype="multipart/form-data">
<label for="file">Filename:</label>
<input type="file" name="file" id="file" />
<br />
<input type="submit" name="submit" value="Submit" />
</form></body>
</html>
<?php
if ($_FILES["file"]["error"] > 0)
  {
  echo "Error: " . $_FILES["file"]["error"] . "<br />";
  }
else
  {
  echo "Upload: " . $_FILES["file"]["name"] . "<br />";
  echo "Type: " . $_FILES["file"]["type"] . "<br />";
  echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
  echo "Stored in: " . $_FILES["file"]["tmp_name"]. "<br />";
  echo "Data: ". filectime($_FILES["file"]["tmp_name"]). "<br />";
  echo "Caminho: ".$_REQUEST["file"];
	//$uploaddir = '"/var/www/simec/financeiro/arquivos/siafi/stn/';
	$uploaddir = '"/var/www/simec/arquivos/';
	$uploadfile = $uploaddir . $_FILES['userfile']['name'];

  print "<pre>";
	if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploaddir . $_FILES['userfile']['name'])) {
    	print "O arquivo é valido e foi carregado com sucesso. Aqui esta alguma informação:\n";
    	print_r($_FILES);
	} else {
    	print "Possivel ataque de upload! Aqui esta alguma informação:\n";
    	print_r($_FILES);
	}
  print "</pre>";


  }
?>