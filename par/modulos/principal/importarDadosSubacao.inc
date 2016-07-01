<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$sbaid = $_GET['sbaid'];

$anopara = $_GET['ano'];

$obEntidadeParControle = new EntidadeParControle();
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);

monta_titulo( $nmEntidadePar, 'Importar Dados' );
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<form name="formulario" id="formulario" method="post">
<input type="hidden" name="sbaid" id="sbaid" value="<?=$sbaid; ?>">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="2">
			De
			<br/>
			<?php
				$sql = "SELECT sbdano as codigo, sbdano as descricao FROM par.subacaodetalhe WHERE sbaid = $sbaid AND sbdano != '$anopara' ORDER BY 1";
// 				$anos = array(
// 						array('codigo' => 2011, 'descricao' => 2011), 
// 						array('codigo' => 2012, 'descricao' => 2012), 
// 						array('codigo' => 2013, 'descricao' => 2013), 
// 						array('codigo' => 2014, 'descricao' => 2014)
// 					);
				/*foreach ($anos as $key => $v) {
					if( $anopara == $v['codigo'] ){
						unset($anos[$key]);
						//break;
					}
				}
				ver(array_change_key_case($anos, CASE_UPPER));*/
				
// 				$anode = $_POST['anode'];
				$db->monta_combo( "anode", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'anode' );
			?>
		</td>
		<td colspan="2">
			Para
			<br/>&nbsp;&nbsp;&nbsp;&nbsp;
			<?php
				echo $anopara;
			?>
			<input type="hidden" name="anopara" id="anopara" value="<?=$anopara; ?>">
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="3" align="center">
			<input type="button" value="Importar Dados" name="btnImportar" id="btnImportar" onclick="verificaObra();" style="cursor: pointer;">
		</td>
	</tr>
</table>
</form>
<div id="erro"></div>
<script type="text/javascript">
	
	function verificaObra(){
		var anode = document.getElementById('anode').value;
		var anopara = document.getElementById('anopara').value;
		var sbaid = document.getElementById('sbaid').value;
		
		if( anode == '' || anopara == '' || sbaid == '' ){
			alert('Faltam dados!');
			return false;
		}
		
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/importarDadosSubacao&acao=A",
	   		data: "requisicao=verificaExisteObra&anode="+anode+'&anopara='+anopara+'&sbaid='+sbaid,
	   		async: false,
	   		success: function(msg){
	   			if( msg > 0 ){
	   				if(confirm('A obra será movida do ano '+anode+' para o ano '+anopara+'\nnão sendo mais acessavel no ano '+anode+'!\nDeseja continuar?')){
	   					importarDados();
	   				} else {
	   					return false;
	   				}
	   			} else {
	   				importarDados();
	   			}
	   		}
		});
	}
	
	function importarDados(){
		var anode = document.getElementById('anode').value;
		var anopara = document.getElementById('anopara').value;
		var sbaid = document.getElementById('sbaid').value;
		
		if( anode == '' || anopara == '' ){
			alert('Informe o ano de importação');
			return false;
		}
		
		document.getElementById('btnImportar').disabled = true;
		divCarregando(this);
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/importarDadosSubacao&acao=A",
	   		data: "requisicao=importarDadosSubacao&anode="+anode+'&anopara='+anopara+'&sbaid='+sbaid,
	   		async: false,
	   		success: function(msg){
	   			if( msg == 1 ){
	   				alert('Importação realizada com sucesso!');
	   				window.close();
	   				window.opener.location.href = 'par.php?modulo=principal/subacao&acao=A&sbaid='+sbaid+'&anoatual='+anopara;
	   			} else {
	   				alert('Falha na importação!');
	   				window.document.getElementById('erro').innerHTML = msg;
	   			}
	   		}
		});
		divCarregado(this);
		document.getElementById('btnImportar').disabled = false;
	}
</script>