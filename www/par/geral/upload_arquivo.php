<?php 

include_once 'config.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

$db = new cls_banco();
$preid = $_REQUEST['preid']? $_REQUEST['preid'] : $_SESSION['par']['preid'];

if(!$preid){
	"<script>alert('Não foi possível realizar a operação!');window.close()</script>";
}

$sql = "SELECT										
			predescricao					 
		FROM obras.preobra 
		WHERE preid = '{$preid}'";
		
$stDescricaoObra = $db->pegaUm($sql);

if($_FILES['arquivo'] && $_FILES['arquivo']['error'] == '0') {
	
	$campos	= array(
				"preid" 	    => $preid,
				"datainclusao" 	=> "now()" ,
				"usucpf"    	=> $_SESSION['usucpf'],
				"poadescricao"  => "'".$_POST['poadescricao']."'",
				"podid"     	=> $_GET['podid']
				);	
				
	$file = new FilesSimec("preobraanexo", $campos ,"obras");
	$arquivoSalvo = $file->setUpload( $arDados['poadescricao']);	
	
	if($arquivoSalvo){
		echo '<script type="text/javascript"> 
				alert("Operação realizada com sucesso.");
				window.opener.location.reload();
			  </script>';
		exit;
	}
}

?>
<html>
	<head>
		<title>Documentos anexo</title>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
		<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../../includes/funcoes.js" ></script>
	</head>
	<body>
		<?php monta_titulo('Documento anexo', $stDescricaoObra) ?>
		<form action="" method="post" enctype="multipart/form-data">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
				<tr>
					<td class="subtitulodireita">Descrição:</td>
					<td>
						<?php			 
						 $predescricao = $arDados['predescricao'];			 			 		 
						 echo campo_texto( "poadescricao", 'N', $boAtivo, '', 40, '', '', '','','','','','',$predescricao);
						 ?>						
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Arquivo:</td>
					<td>
						<input type="file" name="arquivo" id="arquivo" />						
					</td>
				</tr>
				<tr>
					<td colspan="2" class="subtituloCentro">
						<input type="submit" value="Salvar">
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>