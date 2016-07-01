<?php


if(!$_SESSION['rehuf_var']['entid']) {
	die("<script>
			alert('Problemas na navegação, selecione o hospital e tente novamente.');
			window.location='rehuf.php?modulo=inicio&acao=C';
		 </script>");
}

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('gerarmodeloimportacao'=>true,
									 'carregarmodeloimportacao'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<?

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
/* fim montando cabeçalho */
?>
<script>
function carregarCsv() {

	if(document.getElementById('arquivo').value == '') {
		alert('Selecione um arquivo');
		return false;
	}

	document.getElementById('requisicao').value='carregarmodeloimportacao';
	document.getElementById('modelo').submit();
}

function gerarModeloCsv() {
	if(document.getElementById('periodo').value == '') {
		alert('Selecione um Período');
		return false;
	}
	if(document.getElementById('setid').value == '') {
		alert('Selecione um Setor');
		return false;
	}
	document.getElementById('modelo').submit();

}
</script>
<form id="modelo" method="post" enctype="multipart/form-data" id="anexo">
<input type='hidden' name='requisicao' id='requisicao' value='gerarmodeloimportacao'>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td colspan="2" class="SubTituloCentro">Gerar modelo CSV</td>
</tr>
<tr>
	<td class="SubTituloDireita">Período:</td>
	<td>
	<?
	if($db->testa_superuser()) {
		
		$sql = "SELECT p.pplid as codigo, to_char(p.ppldata, 'mm/YYYY') as descricao 
				FROM rehuf.periodoplantao p WHERE p.pplstatus='A' ORDER BY ppldata"; 
		
	} else {
		$sql = "SELECT p.pplid as codigo, to_char(p.ppldata, 'mm/YYYY') as descricao 
				FROM rehuf.periodoplantao p 
				LEFT JOIN rehuf.periodoplantaodata pd ON p.pplid = pd.pplid
				WHERE pd.pplpreenchimentoinicial <= cast(NOW() as date) AND 
					  pd.pplpreenchimentofinal >= cast(NOW() as date) AND 
					  p.pplstatus='A' AND 
					  pd.entid='".$_SESSION['rehuf_var']['entid']."'";
	}
	$db->monta_combo('periodo', $sql, 'S', 'Selecione', 'atualizarGridPlantao', '', '', '', 'S', 'periodo');
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Setor:</td>
	<td>
	<?
	$sql = "SELECT sep.setid as codigo, sep.setnome as descricao FROM rehuf.setorplantao sep 
			INNER JOIN rehuf.setorhospitalplantao shp ON shp.setid=sep.setid 
			WHERE setstatus='A' AND shp.entid='".$_SESSION['rehuf_var']['entid']."'";
	$db->monta_combo('setid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'setid');
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" colspan="2"><input type="button" value="Gerar" onclick="gerarModeloCsv();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Carregar modelo CSV</td>
</tr>
<tr>
	<td class="SubTituloDireita">Arquivo:</td>
	<td>
	<input type="file" name="arquivo" id="arquivo" />
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" colspan="2"><input type="button" name="carregar" value="Carregar" onclick="carregarCsv();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>

</table>
</form>