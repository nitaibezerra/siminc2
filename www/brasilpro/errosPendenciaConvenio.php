<?php
require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/cte/_funcoes.php';
//dbg($_SESSION['erro_empenho']);
$conteudo = $_SESSION['erro_empenho'];
$cont = 1;
?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<table class="tabela listagem" id="itensDeMatricula" border="0" align="center" cellpadding="0" cellspacing="4" width="100%" style="text-align: center">
  <tr>
  	<th colspan="2">Ocorreu o seguinte problema:</th>
  </tr>
 
  <? 
   $exibe = 0;
  foreach($conteudo as $conteudo){ 
  		if($conteudo['cvrnumprocesso'] == NULL ){
  			$exibe += 1;
  			if($exibe == 1){
  ?>
   <tr>
  	<th colspan="2">As subações abaixo não foram conveniadas devida a pendencia de empenho do convênio anterior (FNDE):</th>
  </tr>
  			<? } ?>
  <tr>
  <td class="SubTituloEsquerda" style="font-weight:normal; padding-left:5px;" colspan="2"  ><br> <strong> -<?=$conteudo['sbadsc']; ?> </strong></td>
  </tr>
  <? 
		}else{	
			if($cont == 1){	
  ?>
  <tr>
  	<th colspan="2">Lista de subações do convênio <?=$_SESSION['cvrnumprocesso']; ?> aguardando empenho </th>
  </tr>
  <tr>
  <? } ?>
  	<td width="15%"  class="SubtituloEsquerda">&nbsp; Subacão <?=$cont; ?>: </td>
  	<td>
  	<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - <?=$conteudo['sbadsc']; ?>
  	</td>
  </tr>
  
<? 		$cont++;
		} 
	} 
?>
</table>