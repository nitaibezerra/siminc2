<?php
// ver($_REQUEST,d);
if (!$_REQUEST['sueid'] && !$_REQUEST['smiid'] && !$_REQUEST['sfndeid']) 
{
	// @todo
	// verificar responsabilidade do usuario na supervisao
	// verificar se supervisao existe no banco de dados
	echo "<script>alert('Erro! Supervisão não informada.');self.close()</script>";	
}
$_SESSION['obras2']['sueid']   = $_REQUEST['sueid'];
$_SESSION['obras2']['smiid']   = $_REQUEST['smiid'];
$_SESSION['obras2']['sfndeid'] = $_REQUEST['sfndeid'];

if( $_SESSION['obras2']['sueid'] && !is_dir(APPRAIZ."arquivos/obras2/vistoriaEmp/".$_SESSION['obras2']['sueid'])) {
	mkdir(APPRAIZ."arquivos/obras2/vistoriaEmp/".$_SESSION['obras2']['sueid'], 0777);
}

if( $_SESSION['obras2']['smiid'] && !is_dir(APPRAIZ."arquivos/obras2/vistoriaEmp/".$_SESSION['obras2']['smiid'])) {
	mkdir(APPRAIZ."arquivos/obras2/vistoriaEmp/".$_SESSION['obras2']['smiid'], 0777);
}

if( $_SESSION['obras2']['sfndeid'] && !is_dir(APPRAIZ."arquivos/obras2/vistoriaEmp/".$_SESSION['obras2']['sfndeid'])) {
	mkdir(APPRAIZ."arquivos/obras2/vistoriaEmp/".$_SESSION['obras2']['sfndeid'], 0777);
}
?>
<style type="text/css">
body {margin: 0; padding: 0; }
</style>
<iframe src="plugins/arquivos_dragndrop/arquivos.php" style="width:95%;height:98%" scroll="no"></iframe> 
<?php 
die;
?>