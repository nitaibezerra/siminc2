<? 
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sisindigena2']['universidade']['uncid'])); 

$estado = wf_pegarEstadoAtual( $_SESSION['sisindigena2']['universidade']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirNucleoUniversidade() {
	if(jQuery('#uniid').val()=='') {
		alert('Selecione uma Universidade para ser núcleo');
		return false;
	}
	divCarregando();
	jQuery('#formulario2').submit();

}


function excluirNucleoUniversidade(picid) {
	var conf = confirm('Deseja realmente excluir o Núcleo da Universidade?');
	if(conf) {
		window.location='sisindigena2.php?modulo=principal/universidade/universidade&acao=A&requisicao=excluirNucleoUniversidade&picid='+picid;
	}
}

function mostrarModalInserirNucleo() {

	jQuery("#modalInserirNucleo").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

</script>

<div id="modalInserirNucleo" style="display:none;">
<form method="post" id="formulario2" name="formulario2">
<input type="hidden" name="uncid" id="uncid" value="<?=$_SESSION['sisindigena2']['universidade']['uncid'] ?>">
<input type="hidden" name="requisicao" value="inserirNucleoUniversidade">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir novo núcleo</td>
</tr>
<tr>
	<td class="SubTituloDireita">Universidade Núcleo:</td>
	<td>
	<?
    $sql = "SELECT uniid as codigo, unisigla||' - '||uninome as descricao FROM sisindigena2.universidade WHERE uniid NOT IN(SELECT uniid FROM sisindigena2.nucleouniversidade WHERE picstatus='A' AND uncid='".$_SESSION['sisindigena2']['universidade']['uncid']."')";
    $db->monta_combo('uniid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'uniid',''); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Inserir Núcleo" onclick="inserirNucleoUniversidade();"></td>
</tr>
</table>
</form>
</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Núcleos</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td colspan="2">
		<? if(!$consulta) : ?>
		<table bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
			<tr>
				<td align="right"><input type="button" value="Inserir núcleo" onclick="mostrarModalInserirNucleo();" id="inserirnucleo"></td>
			</tr>
		</table>
		<? endif; ?>
		
		<? carregarNucleosUniversidades(array('uncid'=>$_SESSION['sisindigena2']['universidade']['uncid'],'consulta' => $consulta)); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=orcamento';"> 
			<input type="button" value="Próxima" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=visualizacao_projeto';">
		</td>
	</tr>
</table>