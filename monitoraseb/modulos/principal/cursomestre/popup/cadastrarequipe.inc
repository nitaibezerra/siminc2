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
	$listagem = (isset($_SESSION['listagemEquipe'])) ? $_SESSION['listagemEquipe']: "";
	$valorComboCategoria = explode(":", $_REQUEST['combocategoria']);
	$valorComboUnidadeRef = explode(":", $_REQUEST['combounidaderef']);
	if($_REQUEST['requisicao']=='alterar'){
		$listagem[$_REQUEST['nLinha']] = array('catid'=>$valorComboCategoria[0],'catnome'=>$valorComboCategoria[1],'unrid'=>$valorComboUnidadeRef[0],'unrdsc'=>$valorComboUnidadeRef[1],'ecmdscfuncao'=>$_REQUEST['ecmdscfuncao'],'ecmnummin'=>$_REQUEST['ecmnummin'],'ecmnummax'=>$_REQUEST['ecmnummax'],'ecmdscatribuicao'=>$_REQUEST['ecmdscatribuicao'],'ecmid'=>$_REQUEST['ecmid']);
	}else{
		$listagem[] = array('catid'=>$valorComboCategoria[0],'catnome'=>$valorComboCategoria[1],'unrid'=>$valorComboUnidadeRef[0],'unrdsc'=>$valorComboUnidadeRef[1],'ecmdscfuncao'=>$_REQUEST['ecmdscfuncao'],'ecmnummin'=>$_REQUEST['ecmnummin'],'ecmnummax'=>$_REQUEST['ecmnummax'],'ecmdscatribuicao'=>$_REQUEST['ecmdscatribuicao'],'ecmid'=>$_REQUEST['ecmid']);
	}
	$_SESSION['listagemEquipe'] = $listagem;

	echo "<script>
				executarScriptPai('mostraListaEquipe(\'null\')');
				self.close();
			</script>";
}else{
	if($_REQUEST['requisicao']=='alterar'){
		$itemParaAltera = $_SESSION['listagemEquipe'][$_REQUEST['nLinha']];
		$ecmid = $itemParaAltera['ecmid'];
		$ecmdscfuncao = $itemParaAltera['ecmdscfuncao'];
		$ecmnummin = $itemParaAltera['ecmnummin'];
		$ecmnummax = $itemParaAltera['ecmnummax'];
		$ecmdscatribuicao = $itemParaAltera['ecmdscatribuicao'];
		$combocategoria = $itemParaAltera['catid'].":".$itemParaAltera['catnome'];
		$combounidaderef = $itemParaAltera['unrid'].":".$itemParaAltera['unrdsc'];
	}
}
?>
<html>
	<head>
		<title>Equipe</title>
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
			var formCurso = document.formularioCadastro;
			var vlMin = (formCurso.elements['ecmnummin'].value!="")?parseInt(formCurso.elements['ecmnummin'].value):0;
		    var vlMax = (formCurso.elements['ecmnummax'].value!="")?parseInt(formCurso.elements['ecmnummax'].value):0;
		    if(vlMin > vlMax){
		        alert('O campo "Máximo" deve ser maior do que o campo "Mínimo"!');
		        document.getElementById('btnGravar').disabled=false;
		        return false;
		    }
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

		function comparaEquipeMinMax(){
			var formEquipe = document.formularioCadastro;
			var vlMin = parseInt(formEquipe.elements['ecmnummin'].value);
		    var vlMax = parseInt(formEquipe.elements['ecmnummax'].value);
		    if(vlMin > vlMax){
		        alert('O campo "Número máximo da equipe" deve ser maior do que o "Número mínimo da equipe"!');
		        formEquipe.elements['ecmnummax'].focus();
		    }
		}

	</script>
	<body>
		<form method="POST"  name="formularioCadastro" action="" id="formularioCadastro">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="99%" >
				<tr>
					<th class="TituloTela" colspan="2">Adicionar Equipe</th>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Categoria:</td>
					<td>
						<?php $oCrusoMestre->comboCategoria();?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Função:</td>
					<td>
						<?=campo_texto('ecmdscfuncao','S','S','Funãoo',30,30,'','');?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Por:</td>
					<td>
						Mínimo:<?=campo_texto('ecmnummin','S','S','Mínimo',10,10,'##########','','left','',0,'','',null, 'comparaEquipeMinMax()', null);?>&nbsp; à &nbsp;
						Máximo:<?=campo_texto('ecmnummax','S','S','Máximo',10,10,'##########','','left','',0,'','',null, 'comparaEquipeMinMax()', null);?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Unidade de Referência:</td>
					<td>
						<?php $oCrusoMestre->comboUnidadeReferencia()?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Atribuição:</td>
					<td>
						<?=campo_textarea('ecmdscatribuicao','S', 'S', '', 54, 6, '','onkeyup=\'limiteTexto(this, 255);\'','','','','',''); ?>
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

