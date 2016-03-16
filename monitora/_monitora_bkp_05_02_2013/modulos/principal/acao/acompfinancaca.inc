<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:acompfisicoaca.inc
   finalidade: permitir o acompanhamento físico de ação
   */
 
$modulo=$_REQUEST['modulo'] ;//
$acao = $_REQUEST['acao'] ;
$tipo = $_REQUEST['tipo'] ;
if (!$tipo) $tipo = '1';
include APPRAIZ."includes/cabecalho.inc";
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

$acaid = (integer) $_REQUEST['acaid'];
$sql = "select acacod, loccod, unicod, prgcod from monitora.acao where acaid = " . $acaid;
$dados = $db->recuperar( $sql );
$acacod = $dados['acacod'];
$loccod = $dados['loccod'];
$unicod = $dados['unicod'];
$prgcod = $dados['prgcod'];
$rofano = $_SESSION['exercicio'];

$sql_fixo_select = <<<EOT
	sum( coalesce( rofdot_ini, 0 ) ) / 1 as dotacao_inicial,	
	sum( coalesce( rofautorizado, 0 ) - coalesce( rofdot_ini, 0 ) ) / 1 as credito_adicional,
	sum( coalesce( rofautorizado, 0 ) ) / 1 as autorizado_valor,
	sum( coalesce( rofempenhado, 0 ) ) / 1 as empenhado,
	sum( coalesce( rofliquidado_favorecido, 0 ) ) / 1 as liquidado,
	sum( coalesce( rofpago, 0 ) ) / 1 as pago,
	case when sum( coalesce( rofautorizado, 0 ) ) > 0
		then to_char(( sum( coalesce( rofpago, 0 ) ) * 100 ) / sum( coalesce( rofautorizado, 0 ) ), '999' ) || ' %'
		else '0 %' end
		as autorizado_porcentagem
EOT;

$sql_fixo_where = <<<EOT
	orc.acacod = '$acacod' and
	orc.loccod = '$loccod' and
	orc.unicod = '$unicod' and
	orc.prgcod = '$prgcod' and
	orc.rofano = '$rofano'
EOT;

if ( $_REQUEST['mostrar_por'] == 'gnd' )
{
	$cabecalho = array(
		'Data <BR>Atualização','Código', 'Grupo de Natureza de Despesa',
		'Lei Após Vetos<br>(Dotação Inicial)', 'Créd. Adic. <br>+- Remanejam.', 'Lei + Créditos<br>(Autorizado)', 'Empenhado', 'Liquidado', 'Pago', '% do Pago s/<br>Autorizado'
	);
	$mostrar_por = 'gnd';
	$sql_acompanhamento = <<<EOT
		select
		    to_char(max(rofdata),'dd/mm/YYYY') as dataatu,
			gnd.gndcod || ' ' as cod,
			gnd.gnddsc as dsc,
			$sql_fixo_select
		from financeiro.execucao orc
			inner join gnd on
				gnd.gndcod = orc.gndcod
		where
			$sql_fixo_where
		group by
			gnd.gndcod,
			gnd.gnddsc
EOT;
}
else if ( $_REQUEST['mostrar_por'] == 'fon' )
{
	$cabecalho = array(
		'Data <BR>Atualização','Código', 'Fonte de Recurso',
		'Lei Após Vetos<br>(Dotação Inicial)', 'Créd. Adic. <br>+- Remanejam.', 'Lei + Créditos<br>(Autorizado)', 'Empenhado', 'Liquidado', 'Pago', '% do Pago s/<br>Autorizado'
	);
	$filtro = array();
	
	if($_POST['foncod']){
		array_push($filtro, " fr.foncod = '" . $_POST['foncod'] . "' ");
		$foncod = $_POST['foncod'];
	}
	
	$mostrar_por = 'fon';
	$sql_acompanhamento = "
		select
		to_char(max(rofdata),'dd/mm/YYYY') as dataatu,
			fr.foncod || ' ' as cod,
			fr.fondsc as dsc,
			$sql_fixo_select
		from financeiro.execucao orc
			inner join public.fonterecurso fr on
				fr.foncod = orc.foncod
		where
			$sql_fixo_where " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
		group by
			fr.foncod,
			fr.fondsc ";
}
ELSE
{
	$cabecalho = array(
		'Data <BR>Atualização','Código', 'Natureza de Despesa',
		'Lei Após Vetos<br>(Dotação Inicial)', 'Créd. Adic. <br>+- Remanejam.', 'Lei + Créditos<br>(Autorizado)', 'Empenhado', 'Liquidado', 'Pago', '% do Pago s/<br>Autorizado'
	);
	$mostrar_por = 'nat';
	
	$filtro = array();
	
	if($_POST['ctecod']){
		array_push($filtro, " nd.ctecod = '" . $_POST['ctecod'] . "' ");
		$ctecod = $_POST['ctecod'];
	}
	if($_POST['gndcod']){
		array_push($filtro, " nd.gndcod = '" . $_POST['gndcod'] . "' ");
		$gndcod = $_POST['gndcod'];		
	}
	if($_POST['mapcod']){
		array_push($filtro, " nd.mapcod = '" . $_POST['mapcod'] . "' ");
		$mapcod = $_POST['mapcod'];
	}
	if($_POST['edpcod']){
		array_push($filtro, " nd.edpcod = '" . $_POST['edpcod'] . "' ");
		$edpcod = $_POST['edpcod'];
	}
	
	$sql_acompanhamento = "
		select
			to_char(max(rofdata),'dd/mm/YYYY') as dataatu,
		    nd.ctecod || '.' || nd.gndcod || '.' || nd.mapcod || '.' || nd.edpcod as cod,
			nd.ndpdsc as dsc,
			$sql_fixo_select
		from financeiro.execucao orc
			inner join public.naturezadespesa nd on
				nd.ctecod = orc.ctecod and
				nd.gndcod = orc.gndcod and
				nd.mapcod = orc.mapcod and
				nd.edpcod = case when trim(orc.edpcod) = 'NÃO INFORMADO' THEN '00' ELSE orc.edpcod END and
				nd.sbecod = '00'
		where
			$sql_fixo_where " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
		group by
			nd.ctecod,
			nd.gndcod,
			nd.mapcod,
			nd.edpcod,
			nd.ndpdsc";
}

$sql = "select * from acao where acaid=".$_SESSION['acaid'];
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$nl=$nlinhas;
//if ($nlinhas >= 0) {
    $res =  $db->carrega_registro($RS,0);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    

$unidsc = $db->pegaUm( "select unidsc from public.unidade where unicod = '" . $unicod . "'" );

?>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Acompanhamento Financeiro da Ação';
monta_titulo($titulo_modulo,'');
?>
<form name="formulario" method="post">
<input type="hidden" value="<?=$mostrar_por; ?>" id="mostrar">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <?@$db -> cabecalho_acao($_SESSION['acaid']);?>
	<tr>
		<td align='right' class="SubTituloDireita">
			Mostrar:
		</td>
		<td style="color:#000000;">
			<input type="radio" name="mostrar_por" value="nat" onclick="sumeter('nat');" id="mostrar_por_nat" <?php echo $mostrar_por == 'nat' ? 'checked="checked"' : ''; ?>/>
			<label for="mostrar_por_nat">Natureza</label>
			&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="mostrar_por" value="gnd" onclick="sumeter('gnd');" id="mostrar_por_gnd" <?php echo $mostrar_por == 'gnd' ? 'checked="checked"' : ''; ?>/>
			<label for="mostrar_por_gnd">GND</label>
			&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="mostrar_por" value="fon" onclick="sumeter('fon');" id="mostrar_por_fon" <?php echo $mostrar_por == 'fon' ? 'checked="checked"' : ''; ?>/>
			<label for="mostrar_por_fon">Fonte</label>
			<!--
			<input type="Radio" name="tipo" value="1" onclick="tipo_relatorio()" <?if ($tipo=='1'){print "checked";}?>> Resumido&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="Radio"  name="tipo" value="2" onclick="tipo_relatorio()" <?if ($tipo=='2'){print "checked";}?>> Detalhado
			-->
		</td>
	</tr>
	<tr id="f_natureza" style="display: '';">
		<td class="SubTituloDireita">Categoria Econômica:</td>
		<?
		$sql = "SELECT 
				  ctecod as codigo,
				  ctecod || ' - ' || ctedsc as descricao
				FROM 
				  public.categoriaeconomica
				WHERE ctestatus = 'A'
				ORDER BY ctecod"
		?>
		<td><? $db->monta_combo('ctecod', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'ctecod') ?></td>
	</tr>
	<tr id="f_GND" style="display: '';">
		<td class="SubTituloDireita">GND:</td>
		<?$sql = "SELECT 
				  gndcod as codigo,
				  gndcod || ' - ' || gnddsc as descricao
				FROM 
				  public.gnd
				WHERE gndstatus = 'A'
				ORDER BY gndcod" ?>
		<td><? $db->monta_combo('gndcod', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'gndcod') ?></td>
	</tr>
	<tr id="f_mapcod" style="display: n'';">
		<?
			$sql = "SELECT 
					  mapcod as codigo,
					  mapcod || ' - ' || mapdsc as descricao
					FROM 
					  public.modalidadeaplicacao
					ORDER BY mapcod";
		?>
		<td class="SubTituloDireita">Modalidade Aplicação:</td>
		<td><? $db->monta_combo('mapcod', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'mapcod') ?></td>
	</tr>
	<tr id="f_edpcod" style="display: '';">
		<td class="SubTituloDireita">Elemento de Despesa:</td>
		<?
		$sql = "SELECT 
				  edpcod as codigo,
				  edpcod || ' - ' || edpdsc as descricao,
				  edpstatus
				FROM 
				  public.elementodespesa
				ORDER BY edpcod";
		?>
		<td><? $db->monta_combo('edpcod', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'edpcod') ?></td>
	</tr>
	<tr id="f_foncod" style="display: '';">
		<td class="SubTituloDireita">Fonte de Recurso:</td>
		<?
		$sql = "SELECT 
				  foncod as codigo,
				  foncod || ' - ' || fondsc as descricao
				FROM 
				  public.fonterecurso
				WHERE fonstatus = 'A'
				ORDER BY foncod";
		?>
		<td><? $db->monta_combo('foncod', $sql, 'S', 'Selecione', '', '', '', '240', 'N', 'foncod') ?></td>
	</tr>
	<tr id="btnP" style="display: '';">
		<td class="SubTituloDireita"></td>
		<td><input type="button" value="Pesquisar" onclick="pesquisa();" style="cursor: pointer;"/></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" colspan="2" style="height:1px;padding:0px;"></td>
	</tr>
</table>
</form>
<script language="JavaScript">
	escondeFiltro();
	/*
	function tipo_relatorio()
	{
		document.formulario.submit();
	}
	*/
	function escondeFiltro(){
		if(document.getElementById('mostrar').value != "nat"){
			document.getElementById('f_natureza').style.display = 'none';
			document.getElementById('f_GND').style.display = 'none';
			document.getElementById('f_mapcod').style.display = 'none';
			document.getElementById('f_edpcod').style.display = 'none';
			document.getElementById('btnP').style.display = 'none';
		}
		
		if(document.getElementById('mostrar').value != "fon"){
			document.getElementById('f_foncod').style.display = 'none';
		}else{
			document.getElementById('btnP').style.display = '';
		}
	}

	function sumeter(filtro){
		if(filtro == 'nat'){
			document.formulario.submit();
		}else if(filtro == 'gnd'){
			escondeFiltro()
			document.formulario.submit();
		}else{
			escondeFiltro();
			document.formulario.submit();
		}
	}
	function pesquisa(){
		document.formulario.submit();
	}

</script>
<link rel="stylesheet" href="includes/grid/styles/classic/grid.css" type="text/css" ></link>
<script src="includes/grid/lib/grid.js"></script>
	<style>
		#grid1 {height: 200px; width:95%; border: 2px inset; background: white}

		.active-controls-grid {height: 100%; font: menu;}
 		.active-column-0 {width:  200px;}
		.active-column-1 {text-align: right;}
		.active-column-2 {text-align: right;}
		.active-column-3 {text-align: right;}
		.active-column-4 {text-align: right;}

		.active-grid-column {border-right: 1px solid threedshadow;}
		.active-grid-row {border-bottom: 1px solid threedlightshadow;}
	</style>
<?php
	$db->monta_lista(
		$sql_acompanhamento,
		$cabecalho,
		300,
		20,
		'S',
		'',
		''
	);

	/*
	if ($tipo=='2'){
	//Relatorio detalhado
	$cabecalho = array('Unidade','Dotação Atual','Provisão Concedida','Provisão Recebida','Destaque Concedido','Destaque Recebido','Valor Empenhado','Valor Liquidado','Valor Pago');
	//$sql = "Select no_unidade_orcamentaria, dotacao_disponivel, provisao_recebida, destaque_recebido, empenhos_emitidos, empenhos_liquidados, empenhos_emitidos - empenhos_liquidados as aliquidar, valor_pago,  empenhos_liquidados - valor_pago as apagar from  siof_simec where co_programa='".$prgcod."' and co_acao='".$acacod."' and co_unidade_orcamentaria='".$unicod."' and sfsstatus='A' order by 1";
	$sql = "Select no_unidade_orcamentaria, dotacao_disponivel, provisao_concedida, provisao_recebida, destaque_concedido, destaque_recebido, empenhos_emitidos, empenhos_liquidados, valor_pago from  siof_simec where co_programa='".$prgcod."' and co_acao='".$acacod."'  and sfsstatus='A' order by 1";

	$db->monta_lista($sql,$cabecalho,300,20,'S','','');
	
	}
	else
	{
	//Relatorio Resumido
	$cabecalho = array('Unidade','Dotação Atual','Valor Empenhado','Valor Liquidado','Valor Pago');
	$sql = "Select no_unidade_orcamentaria, dotacao_disponivel, empenhos_emitidos, empenhos_liquidados, valor_pago from siof_simec where co_programa='".$prgcod."' and co_acao='".$acacod."' and co_unidade_orcamentaria='".$unicod."' and sfsstatus='A' order by 1";
	//$sql = "Select no_unidade_orcamentaria, dotacao_disponivel, empenhos_emitidos, empenhos_liquidados, valor_pago from  siof_simec where co_programa='".$prgcod."' and co_acao='".$acacod."'  and sfsstatus='A' order by 1";
	$db->monta_lista($sql,$cabecalho,300,20,'S','','');
	}
	/exemplo de grid
	$RS = $db->record_set($sql); 
	$name = "grid1";
	grid($name, $RS);
	
	}
	*/
?>
