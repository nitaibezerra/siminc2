<?php
global $db;

if ($_REQUEST['popInsereAcao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	popInsereAcaoPrograma($_POST);
	exit;
}
$proid = $_REQUEST['proid'];

monta_titulo( 'Selecionar Ações', '' );

?>
<style>
<!--
#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 97%;
    text-align: center;
    z-index: 8000;
    height: 90%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 250px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
-->
</style>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="formulario" name="formulario" action="" method="post"	enctype="multipart/form-data">
<div id="erro"></div>
<input type="hidden" value="<?=$proid; ?>" name="proid" id="proid"> 
<?
	$linha2 = "<label for=\"seleciona\"> <input type=\"checkbox\" name=\"seleciona'\" id=\"seleciona\" onclick=\"selecionaTodos();\"><b>Selecionar todos</b></label>";
			
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<td bgcolor="#e9e9e9" align="left" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
	$arDados = array();
	
	$sql = "SELECT 
			  acaid,
			  acanome
			FROM 
			  emenda.acao 
			WHERE acastatus = 'A'
			ORDER BY acanome";

	$cabecalho = array("Selecionar", "Descrição");
	
	$arDados = $db->carregar($sql);
	?>

<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td align="Center" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"
				title="Ordenar por Selecionar"><strong>Selecionar</strong>
			
			
			<td align="Center" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"
				title="Ordenar por Descrição"><strong>Descrição</strong>
		
		</tr>
	</thead>
	<?$check = "";
	
	$arNomeAcao = array();
	$arAcaoSessao = isset( $_SESSION['emenda']['InsereAcaoPrograma'] ) ? $_SESSION['emenda']['InsereAcaoPrograma'] : array();
	foreach( $arAcaoSessao as $arAcao ){
		$arNomeAcao[] = $arAcao["acanome"];
	}
	
	foreach ($arDados as $key => $value) {

		$par = $value['acaid'].'_'.$value['acanome'].'_null_null';
		$key % 2 ? $cor = "#f7f7f7" : $cor = "";

	?>
		<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
			<td title="Selecionar">
				<center><input type="checkbox" <?php echo in_array( $value['acanome'], $arNomeAcao) ? 'checked="checked"' : "" ?>  onclick="carregaCheck('<?=$par; ?>', this);" value="<?=$par; ?>" name="acaid_<?=$value['acaid'] ?>" id="acaid_<?=$value['acaid'] ?>"></center>
			</td>
			<td title="Descrição"><?=$value['acanome']; ?></td>
		</tr>
	<?} ?>
</table>

<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#ffffff">
		<td><b>Total de Registros: <?=count($arDados) ?></b></td>
		<td></td>
	</tr>
</table>


</form>
<script type="text/javascript">
	function selecionaTodos(){
		$('loader-container').show();
		for(i=0; i<formulario.length; i++){
 	
	 		if(formulario.elements[i].type == 'checkbox'){
	 			
	 			if(!formulario.elements[i].disabled){ 				
	 				if($('seleciona').checked){
	 					formulario.elements[i].checked = true;
	 				}else{
	 					formulario.elements[i].checked = false;
	 				}
	 				
	 				if(formulario.elements[i].id != "seleciona"){
	 					carregaCheck(formulario.elements[i].value, formulario.elements[i]);
	 				}
	 			}
	 		}
	 	}
	 	$('loader-container').hide();
	}
	function carregaCheck(valor, check){
	
		var myajax = new Ajax.Request('emenda.php?modulo=principal/popInsereAcao&acao=A', {
			        method:     'post',
			        parameters: '&popInsereAcao=true&proid='+$('proid').value+
			        							   '&valor='+valor+
			        							   '&check='+check.checked,
			        asynchronous: false,
			        onComplete: function (res){
						$('erro').innerHTML = res.responseText;
			        }
			  });
		window.opener.mostraAcoesCadastradas( $('proid').value );
	}
</script>
</body>