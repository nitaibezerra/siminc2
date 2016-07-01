<?php
/*----------------- inicializa sistema ---------------------*/ 
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

/*----------------- Ações - Formulario ---------------------*/ 
$dcuid 	= $_REQUEST["dcuid"];
$ID 	= $_REQUEST["ID"];
$desc 	= $_REQUEST["desc"];

/*----------------- Recupera dados ---------------------*/  	
/*if($dcuid != 0){
 $sqlCombo = "	select dcudescinadequado
 				from pdeescola.dependenciascondicaouso
 				where dcuid = ".$dcuid;
 $dcudescinadequado = $db->pegaUm( $sqlCombo );
}else{
	$dcudescinadequado = $desc;
}*/

$dcudescinadequado = $desc;

$titulo_modulo = 'O que está inadequado';
monta_titulo($titulo_modulo, 'Instrumento 1');
?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
  </head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<table class="tabela listagem" bgcolor="#f5f5f5" align="center">
  <tr>
    <td class="SubtituloEsquerda" >Descrição:</td>
    <td>

    <?= campo_textarea( 'dcudescinadequado', 'N', 'S', 'Descrição ', 70 , 15, 150, $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>             
	 

    </td>
  </tr>
   <tr>
  	<td  class="SubTituloDireita"  colspan="2">
  		<div style="float:right;">
  		<input type='button' class="botao" name='cadastra' value='Confirmar' onclick="submeterDados()" title="Salvar dados" />
  		</div>
  	</td>
  </tr>
</table>
</form>
<script language="javascript" type="text/javascript">
function submeterDados() {
	
	
	var dcudescinadequado = document.formulario.dcudescinadequado.value.length;
	if(dcudescinadequado > 150){
		alert('Texto muito grande. Limite máximo de 150 caracteres.');
		//form.dcudescinadequado.focus(); 
		return false;
	} 
	
	//pegando valor do popup e transferindo para dentro do input
	opener.document.formulario.dcudescinadequado<?=$ID?>.value =  document.getElementById('dcudescinadequado').value ;
	window.close();

}
</script>
</html>
