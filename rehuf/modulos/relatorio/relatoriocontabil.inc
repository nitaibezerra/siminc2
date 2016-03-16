<?php
if ( isset( $_REQUEST['buscar'] ) ) {
	
	switch($_REQUEST['planillha']) {
		case '2004-2008':
			include "relatoriocontabil_resultado.inc";
			exit;
		case '2009-2011':
			include "relatoriocontabil2009_resultado.inc";
			exit;
	}
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Contabil - REHUF", "" );

?>
<script type="text/javascript">
function acessarplanilha(valor) {
	window.location='rehuf.php?modulo=relatorio/relatoriocontabil&acao=A&planilha='+valor;
}

function exibirRelatorio() {
	var formulario = document.formulario;
		// verifica se tem algum ano selecionado
	if (formulario.elements['anos[]'].length > 0) {
		var anomarcado = false;
		for(i=0;i<formulario.elements['anos[]'].length;i++) {
			if(formulario.elements['anos[]'][i].checked) {
				anomarcado = true;
			}
		}
		if(!anomarcado) {
			alert( 'Escolha ao menos um ano.' );
			return false;
		}
	}
	
	selectAllOptions( document.getElementById( 'entid' ) );

	// submete formulario
	formulario.target = 'relatoriogeralrehuf';
	var janela = window.open( '', 'relatoriogeralrehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
        <td class="SubTituloDireita">Ano referente:</td>
        <td><?
		$planil = array(0 => array("codigo" => "2004-2008", "descricao" => "Planilha Contabil 2004-2008"),
						1 => array("codigo" => "2009-2011", "descricao" => "Planilha Contabil 2009-2011"));

		$planilha = $_REQUEST['planilha'];
		
		if(!$planilha) {
			$planilha = "2009-2010";
		}
						
		$db->monta_combo('planillha', $planil, 'S', 'Selecione', 'acessarplanilha', '', '', '200', 'N', 'planilha', false, $planilha);
        ?></td>
        </tr>
        <tr><td>Filtros</td></tr>		
		<tr>
			<td class="SubTituloDireita" valign="top">Hospitais :</td>
			<td>
			<?
			$permissoes = verificaPerfilRehuf();
			if(count($permissoes['verhospitais']) > 0) {
				$sqlHospitais = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM entidade.entidade ent 
								 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
								 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
								 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') 
								 ORDER BY ena.entsig";
				combo_popup( "entid", $sqlHospitais, "Hospitais", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
				
			} else {
				echo "Seu perfil não possui hospitais";	
			}
			?>
			</td>
		</tr>
		<?
		switch($planilha) {
			case '2004-2008':
				?>
				<tr>
					<td class="SubTituloDireita" valign="top">Ano :</td>
					<td><input type="checkbox" name="anos[]" value="2004"> 2004 <input type="checkbox" name="anos[]" value="2005"> 2005 <input type="checkbox" name="anos[]" value="2006"> 2006 <input type="checkbox" name="anos[]" value="2007"> 2007 <input type="checkbox" name="anos[]" value="2008"> 2008</td>
				</tr>
				<?
				break;
			case '2009-2011':
				?>
				<tr>
					<td class="SubTituloDireita" valign="top">Ano :</td>
					<td><input type="radio" name="anos[]" value="2009"> 2009 <input type="radio" name="anos[]" value="2010" > 2010 <input type="radio" name="anos[]" value="2011" checked> 2011</td>
				</tr>
				<?
				break;
		}
		?>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
	</tr>
</table>