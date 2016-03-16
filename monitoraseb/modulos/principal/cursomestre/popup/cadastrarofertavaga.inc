<?php

if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

echo "<script language='javascript' type='text/javascript' src='./js/monitoraseb.js'></script>";
include APPRAIZ."monitoraseb/classes/CursoMestre.class.inc";

$oCrusoMestre = new CursoMestre();
if($_SERVER['REQUEST_METHOD']=="POST"){
	$listagem = (isset($_SESSION['listagemOfertaVaga'])) ? $_SESSION['listagemOfertaVaga']: "";
	if($_REQUEST['requisicao']=='alterar'){
		$listagem[$_REQUEST['nLinha']] = array('ovgnumvagas'=>$_REQUEST['ovgnumvagas'],'ovganobase'=>$_REQUEST['ovganobase'],'ovgid'=>$_REQUEST['ovgid']);
	}else{
		$listagem[] = array('ovgnumvagas'=>$_REQUEST['ovgnumvagas'],'ovganobase'=>$_REQUEST['ovganobase'],'ovgid'=>$_REQUEST['ovgid']);
	}
	$_SESSION['listagemOfertaVaga'] = $listagem;
	
	echo "<script>
				executarScriptPai('mostraListaOfertaVaga(\'null\')');
				self.close();
			</script>";
}else{
	if($_REQUEST['requisicao']=='alterar'){
		$itemParaAltera = $_SESSION['listagemOfertaVaga'][$_REQUEST['nLinha']];
		$ovgid = $itemParaAltera['ovgid'];
		$ovgnumvagas = $itemParaAltera['ovgnumvagas'];
		$ovganobase = $itemParaAltera['ovganobase'];
	}
}
?>
<html>
	<head>
		<title>Previsão de Oferta de Vagas</title>
	</head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script language="JavaScript" type="text/javascript" src="../includes/funcoes.js"></script>

	<script type="text/javascript">

		function gravar(){
			if(validaFormulario()){
				$('#formularioCadastro').submit();
			}
			
		}
		
		function validaFormulario(){
			var erro = 0;
			$("[class~=obrigatorio]").each(function() { 
				if(!this.value){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			});
			if(erro == 0){
				return true;
			}
		}

		function limiteTexto(campo, limiteMax) {
	      	var conteudo = campo.value;
	
	      	if (conteudo.length > limiteMax){
	      		var texto = conteudo.substring(0, limiteMax);
	      		campo.value = texto;
	      	}
	   	}

	</script>
	<body>
		<form method="POST"  name="formularioCadastro" action="" id="formularioCadastro">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="99%" >
				<tr>
					<th class="TituloTela" colspan="2">Previsão de Oferta de Vagas</th>
				</tr>
				<tr>
					<td>
						Vagas: <?=campo_texto('ovgnumvagas','S','S','Vagas',10,10,'##########','');?> &nbsp; Ano:<?=campo_texto('ovganobase','S','S','Ano',10,10,'####','');?>
					</td>
				</tr>
					<th style="text-align: left;">
						<input type="hidden" name="nLinha" value="<?=$_REQUEST['nLinha']?>"/>
						<input type="hidden" name="requisicao" value="<?=$_REQUEST['requisicao']?>" />
						<input style="cursor: pointer;"  type="button" value="Salvar" name="btnGravar" onclick="gravar();">
						<input style="cursor: pointer;"  type="button" value="Cancelar" name="btnCancelar" onclick="self.close();">
					</th>
				</tr>
			</table>
		</form>
		<?php if($msg): ?>
			
		<?php endif; ?>
	</body>
</html>

