<?php

	

function inserirValor(){
	global $db;
	
	$valor = $_POST['vlpvalorponto'];
	$valor = str_replace('.','',$valor);
	$valor = str_replace(',','.',$valor);
	
	$sql = " INSERT INTO demandas.valorpontuacao
			 (
			 	vlpvalorponto, 
			 	vlpstatus
			 ) VALUES (
			 	'".$valor."', 
			 	'A'
			 );";
	$db->executar($sql);
	$db->commit();
}



function alterarValor(){
	global $db;
	
	$valor = $_POST['vlpvalorponto'];
	$valor = str_replace('.','',$valor);
	$valor = str_replace(',','.',$valor);
	
	$sql = "UPDATE 
				demandas.valorpontuacao 
			SET 
				vlpvalorponto = '".$valor."'
			WHERE 
				vlpid = ".$_POST['vlpid'];

	$db->executar($sql);					
	$db->commit();
}






if($_POST){
	
	if(!$_POST['vlpid']) {
		inserirValor();
	}
	else{
		alterarValor();
	}
	unset($_POST);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	//exit();
	
}

$sql = "SELECT vlpid, vlpvalorponto, vlpstatus FROM demandas.valorpontuacao";
$dados = $db->carregar($sql);
if($dados) extract($dados[0]);
if($vlpvalorponto) $vlpvalorponto = number_format($vlpvalorponto,2,",",".");



include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Valor da Pontuação', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formOrigem" name="formOrigem" action="" method="post" onsubmit="return validaForm();" >

<input type="hidden" name="vlpid" value="<?=$vlpid?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor do Ponto</td>
	<td>
		<?= campo_texto( 'vlpvalorponto', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, ''); ?>
	</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		</td>
	</tr>
	</table>

	</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function validaForm(){
	
	if(document.formOrigem.vlpvalorponto.value == ''){
		alert ('O campo Valor do Ponto deve ser preenchido.');
		document.formOrigem.vlpvalorponto.focus();
		return false;
	}

}
</script>
</body>