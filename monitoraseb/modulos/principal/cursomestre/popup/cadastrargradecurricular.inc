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
	$listagem = (isset($_SESSION['listagemGradeCurricular'])) ? $_SESSION['listagemGradeCurricular']: "";
	$valorCombo = explode(":", $_REQUEST['combotipo']);
	if($_REQUEST['requisicao']=='alterar'){
		$listagem[$_REQUEST['nLinha']] = array('tgcid'=>$valorCombo[0],'tgcdsc'=>$valorCombo[1],'gcrnome'=>$_REQUEST['gcrnome'],'gcrnumhoraula'=>$_REQUEST['gcrnumhoraula'],'gcrementa'=>$_REQUEST['gcrementa'],'gcrid'=>$_REQUEST['gcrid']);
	}else{
		$listagem[] = array('tgcid'=>$valorCombo[0],'tgcdsc'=>$valorCombo[1],'gcrnome'=>$_REQUEST['gcrnome'],'gcrnumhoraula'=>$_REQUEST['gcrnumhoraula'],'gcrementa'=>$_REQUEST['gcrementa'],'gcrid'=>$_REQUEST['gcrid']);
	}
	$_SESSION['listagemGradeCurricular'] = $listagem;

	echo "<script>
				executarScriptPai('mostraListaGradeCurricular(\'null\',\'ture\')');
				self.close();
			</script>";
}else{
	if($_REQUEST['requisicao']=='alterar'){
		$itemParaAltera = $_SESSION['listagemGradeCurricular'][$_REQUEST['nLinha']];
		$tgcid = $itemParaAltera['tgcid'];
		$tgcdsc = $itemParaAltera['tgcdsc'];
		$gcrnome = $itemParaAltera['gcrnome'];
		$gcrnumhoraula = $itemParaAltera['gcrnumhoraula'];
		$gcrementa = $itemParaAltera['gcrementa'];
		$combotipo = $itemParaAltera['tgcid'].":".$itemParaAltera['tgcdsc'];
	}
}
?>
<html>
	<head>
		<title>Adicionar Grade Curricular</title>
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
					<th class="TituloTela" colspan="2">Adicionar Grade Curricular</th>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Tipo:</td>
					<td>
						<?php $oCrusoMestre->comboTipoGC($combotipo);?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Nome:</td>
					<td>
						<?=campo_texto('gcrnome','S','S','Nome',30,30,'','');?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Carga Horária:</td>
					<td>
						<?=campo_texto('gcrnumhoraula','S','S','Descrição',10,10,'##########','');?>
						Hora(s) / Aula
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Ementa:</td>
					<td>
						<?=campo_textarea('gcrementa','S', 'S', '', 54, 6, '','onkeyup=\'limiteTexto(this, 500);\'','','','','',''); ?>
					</td>
				</tr>
				<tr>
					<th></th>
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

