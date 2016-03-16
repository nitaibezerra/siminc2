<?php

switch($_REQUEST['buscar']) {
	case '1':
		include "relatorioplantao_resultado.inc";
		exit;
	case '2':
		if($_POST['filano'])
			$_SESSION['rehuf_var']['filano']=$_POST['filano'];
		if($_POST['entid'])
			$_SESSION['rehuf_var']['hospitalentid']=$_POST['entid'];
			
		if($_REQUEST['agrupador'] && !$_REQUEST['gitid']) {
			echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
			
			foreach($_REQUEST['agrupador'] as $tblid) {
				// Pegando dados da TABELA (pai de toda a estrutura)
				$tabela = $db->pegaLinha("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid ='".$tblid."'");
				echo "<table class=\"tabela\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">";
				echo "<tr><td class='SubTituloCentro' colspan='2'>".$tabela['tabtdsc']."</td></tr>";
				$grupoitem = (array) $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ORDER BY gitordem");
				foreach($grupoitem as $grp) {
					echo "<tr><td class='SubTituloDireita' width='50%'>".$grp['gitdsc']."</td><td><form action='' method='post' name='formulario'><input type='hidden' name='buscar' value='2'><input type='hidden' name='agrupador[]' value='".$tblid."'><input type='hidden' name='gitid' value='".$grp['gitid']."'><input type='submit' value='Clique aqui'></form></td></tr>";
				}
				echo "</table>";

			}
			exit;			
		}
		include "relatoriogeralexcel_resultado.inc";
		exit;
	case '3':
		include "relatoriogeralagrupado_resultado.inc";
		exit;
		
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Geral - REHUF", "" );

$_SESSION['rehuf_var']['filano']=array();

?>
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	formulario.buscar.value='1';
	selectAllOptions( document.getElementById( 'entid' ) );
	selectAllOptions( document.getElementById( 'setid' ) );
	selectAllOptions( document.getElementById( 'pplid' ) );

	// submete formulario
	formulario.target = 'relatorioplantaorehuf';
	var janela = window.open( '', 'relatorioplantaorehuf', 'width=1000,height=800,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}

</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
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
		<tr>
			<td class="SubTituloDireita" valign="top">Setores :</td>
			<td>
			<?
			$sqlSetores = "SELECT setid as codigo, setnome as descricao FROM rehuf.setorplantao";
			combo_popup( "setid", $sqlSetores, "Setores", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Períodos :</td>
			<td>
			<?
			$sqlPeriodos = "SELECT pplid as codigo, to_char(ppldata, 'mm/YYYY') as descricao FROM rehuf.periodoplantao WHERE pplstatus='A' ORDER BY ppldata";
			combo_popup( "pplid", $sqlPeriodos, "Períodos :", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Somente totalizadores :</td>
			<td><input type="checkbox" name="somentetotalizadores" value="sim"></td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" <? echo ((count($permissoes['verhospitais']) > 0)?"":"disabled"); ?>/></td>
	</tr>
</table>