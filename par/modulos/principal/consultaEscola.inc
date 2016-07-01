<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

/*
if (!$_POST)
{
	$sql = "select ent.entnome
		from entidade.entidade ent
		where ent.entcodent is not null and ent.entstatus = 'A' and ent.tpcid in (1,3)
		order by ent.entnome";

	$arEscolas = $db->carregar($sql);

	$_SESSION['arescolas'] = $arEscolas;
}

if($_REQUEST['req'] == 'ACentnome')
{
	$arEscolas = $_SESSION['arescolas'];
	ob_clean();

	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arEscolas as $key=>$value){
		if (strpos(strtolower($value['entnome']), $q) !== false) {
			$entnome = $value['entnome'];
			echo "{$entnome}|{$entnome}\n";
		}
	}

	die;
}
*/

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<script>
	/*
	jQuery(document).ready(function() {
		$('input[name="entnome"]').autocomplete("par.php?modulo=principal/consultaEscola&acao=A&req=ACentnome", {
	  	cacheLength:10,
		width: 440,
		scrollHeight: 220,
		selectFirst: true,
		autoFill: true
		});
	});
	*/
</script>
<form method="post" name="formulario" id="formulario">
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tbody>
			<tr>
				<td class="SubTituloDireita">Código INEP:</td>
					<td colspan="3">
					<?php
						$filtro = simec_htmlentities( $_REQUEST['entcodent'] );
						$entcodent = $filtro;
						echo campo_texto( 'entcodent', 'N', 'S', '', 50, 200, '', ''); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nome:</td>
					<td colspan="3">
					<?php
						$filtro = simec_htmlentities( $_REQUEST['entnome'] );
						$entnome = $filtro;
						echo campo_texto( 'entnome', 'N', 'S', '', 50, 200, '', ''); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Estado:</td>
				<td>
					<?php
						$estuf = $_POST['estuf'];
						$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
						$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Município:</td>
				<td>
					<?php 
						$filtro = simec_htmlentities( $_POST['mundescricao'] );
						$mundescricao = $filtro;
						echo campo_texto( 'mundescricao', 'N', 'S', '', 50, 200, '', ''); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Localização:</td>
				<td>
					<?php
						$tplid = $_POST['tplid'];
						$sql = "select tplid as codigo, tpldesc as descricao  from entidade.tipolocalizacao where tplstatus = 'A' order by descricao";
						$db->monta_combo( "tplid", $sql, 'S', 'Todas Localizações', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Esfera:</td>
				<td>
					<?php
						$tpcid = $_POST['tpcid'];
						$sql = "select tpcid as codigo, tpcdesc as descricao  from entidade.tipoclassificacao where tpcid in (1,3) order by tpcdesc";
						$db->monta_combo( "tpcid", $sql, 'S', 'Todas Esfera', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Situação:</td>
				<td>
					<?php
						$tpsid = $_POST['tpsid'];
						$sql = "select tpsid as codigo, tpsdesc as descricao  from entidade.tiposituacao  where tpsstatus = 'A' order by tpsstatus  ";
						$db->monta_combo( "tpsid", $sql, 'S', 'Todas Situações', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita"></td>
				<td colspan="3">
				<input type="submit" name="pesquisar" value="Pesquisar" />
				<input type="button" name="todos" value="Exportar Excel" onclick="window.location='par.php?modulo=principal/consultaEscola&acao=A&exportarexcel=true';" />
			</tr>
		</tbody>
	</table>
</form>
<?php
if($_REQUEST['pesquisar'] || $_REQUEST['exportarexcel']) {

	$where = array();
	
	if($_REQUEST['entcodent']) {
		$where[] = "ent.entcodent = '{$_REQUEST['entcodent']}'";
	}
	
	if($_REQUEST['entnome']) {
		$where[] = "removeacento(ent.entnome) ilike removeacento('%".$_REQUEST['entnome']."%')";
	}
	
	if($_REQUEST['estuf']) {
		$where[] = "ed.estuf = '{$_REQUEST['estuf']}'";
	}
	
	if($_REQUEST['mundescricao']) {
		$where[] = "removeacento(mun.mundescricao) ILIKE removeacento('%".$_REQUEST['mundescricao']."%')";
	}
	
	if($_REQUEST['tplid']) {
		$where[] = "ent.tplid = {$_REQUEST['tplid']} ";
	}
	
	if($_REQUEST['tpcid']) {
		$where[] = "ent.tpcid = {$_REQUEST['tpcid']} ";
	}
	
	if($_REQUEST['tpsid']) {
		$where[] = "ent.tpsid = {$_REQUEST['tpsid']} ";
	}
}


$cabecalho = array("Código","INEP","Nome" , "Estado" , "Município", "Localização", "Esfera", "Função", "Situação");

$sql = "select ent.entid ,ent.entcodent, ent.entnome, ed.estuf, mun.mundescricao, tpl.tpldesc, tpc.tpcdesc, fun.fundsc, tps.tpsdesc
		from entidade.entidade ent
			left join entidade.funcaoentidade funent on funent.entid = ent.entid 
			left join entidade.endereco ed on ed.entid = ent.entid
			left join territorios.municipio mun on mun.muncod = ed.muncod
			left join entidade.tipolocalizacao tpl on tpl.tplid = ent.tplid
			left join entidade.tipoclassificacao tpc on tpc.tpcid = ent.tpcid
			left join entidade.funcao fun on fun.funid = funent.funid
			left join entidade.tiposituacao tps on tps.tpsid = ent.tpsid
			where ent.entcodent is not null and ent.entstatus = 'A' and ent.tpcid in (1,3)
			".(($where)?" AND ".implode(" AND ", $where):"")." 
			order by ent.entnome";
		
if($_REQUEST['exportarexcel']) {
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$formato = array('', '', '', '', '', '','','','');
	$db->sql_to_excel($sql, 'relEscolas', $cabecalho, $formato);
	exit;
} else {
if ($_POST)
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','form');	
}
?>
