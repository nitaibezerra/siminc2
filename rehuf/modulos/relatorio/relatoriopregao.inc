<?php

switch($_REQUEST['buscar']) {
	case '1':
		include "relatoriopregao_resultado.inc";
		exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório de Estimativa do Consumo Anual - REHUF", "" );

$permissoes = verificaPerfilRehuf();

?>
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	formulario.buscar.value='1';
	var marcado=false;
	for(var i=0;i<formulario.preid.length;i++) {
		if(formulario.preid[i].checked) marcado=true;
	}
	if(marcado==false) {
		alert("Selecione um pregão");
		return false;
	}
	// submete formulario
	formulario.target = 'relatoriopregaorehuf';
	var janela = window.open( '', 'relatoriopregaorehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}

</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td>Filtros</td></tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Selecione um pregão :</td>
			<td style="height:100px;overflow:auto;" valign="top">
			<?
			$sql = "SELECT preid as codigo, preobjeto||' ('||COALESCE(precodigo,'-')||')' as descricao FROM rehuf.pregao WHERE prestatus='A'";
			$pregoes = $db->carregar($sql);
			
			if($pregoes[0]) {
				foreach($pregoes as $pregao) {
					echo "<input type='radio' name='preid' value='".$pregao['codigo']."'>".$pregao['descricao']."<br />";
				}
			}
							
			?>
			</td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" <? echo ((count($permissoes['verhospitais']) > 0)?"":"disabled"); ?>/></td>
	</tr>
</table>