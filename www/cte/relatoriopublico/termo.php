<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include "funcoes.php";
$db = new cls_banco();
$inuid =  $_REQUEST['inuid'];
$sql = "select terdocumento from cte.termo where inuid =".$inuid." order by terid desc limit 1";	
$termo =  $db->pegaUm($sql);
if($termo){
echo $termo; 
}else{
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<tr>
				    <td colspan="5" valign="top" class="Erro">
						Não existe termo para este Município.
				 	</td>
				 	</tr>
				 	</table>
<?
}
?>