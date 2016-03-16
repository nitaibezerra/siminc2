<?php

function monta_grid(){
	global $db;
	extract($_REQUEST);

	$where = array();
	
	// estado
	if( $estuf[0] && $estuf_campo_flag != '' ){
		array_push($where, "CASE WHEN ie.muncod IS NOT NULL THEN mu2.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') 
				 			WHEN us.muncod IS NOT NULL THEN mu.estuf" . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "')  
				 			ELSE false END");
	}
	
	// municipio
	if( $muncod[0] && $muncod_campo_flag != '' ){
		array_push($where, "CASE WHEN ie.muncod IS NOT NULL THEN mu2.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') 
				 			WHEN us.muncod IS NOT NULL THEN mu.muncod" . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "')  
				 			ELSE false END");
	}

	// parcela
	if( $fpbid[0] && $fpbid_campo_flag != '' ){
		array_push($where, " f.fpbid " . (!$fpbid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fpbid ) . "') ");
	}
	
	// perfil
	if( $pflcod[0]  && $pflcod_campo_flag != '' ){
		array_push($where, " p.pflcod " . (!$pflcod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pflcod ) . "') ");
	}
	
	if($iusnome) {
		array_push($where, " UPPER(public.removeacento(us.iusnome)) ILIKE '%".removeAcentos($iusnome)."%' ");
	}

	$sql = "SELECT 
		'<center><img src=\"../imagens/historico.png\" style=\"cursor:pointer;\" onclick=\"carregarRelatorio'||pb.pflcod||'('||us.iusid||','||f.fpbid||');\"></center>' as acao,
		us.iusnome || ' ( ' || replace(to_char(us.iuscpf::numeric, '000:000:000-00'), ':', '.') || ' )' as nome,
		es.esddsc as situacao,
		p.pfldsc as perfil,
		'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as parcela,
			
		CASE WHEN us.iusrede='M' THEN 'Rede Municipal' 
			 WHEN us.iusrede='E' THEN 'Rede Estadual'
			 WHEN pb.pflcod=".PFL_COORDENADORESTADUAL." THEN 'Rede Estadual'
			 ELSE 'Não identificado' END as esfera,
			
		CASE WHEN us.muncodatuacao IS NOT NULL THEN mu.mundescricao 
			 WHEN ie.muncod IS NOT NULL THEN mu2.mundescricao
			 ELSE 'Não identificado' END as municipio,
			
		CASE WHEN us.muncodatuacao IS NOT NULL THEN mu.estuf 
			 ELSE up.estuf END as estado 
			 
		FROM escolaterra.pagamentobolsista pb 
		INNER JOIN escolaterra.periodoreferencia f ON f.fpbid = pb.fpbid 
		INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
		INNER JOIN seguranca.perfil p ON p.pflcod = pb.pflcod 
		INNER JOIN workflow.documento d ON d.docid = pb.docid AND d.tpdid=".TPD_PAGAMENTOBOLSA." 
		LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid 
		INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
		INNER JOIN escolaterra.identificacaousuario us ON us.iusid = pb.iusid 
		INNER JOIN escolaterra.identificaoendereco ie ON ie.iusid = us.iusid 
		INNER JOIN escolaterra.ufparticipantes up ON up.ufpid = us.ufpid 
		LEFT JOIN territorios.municipio mu ON mu.muncod = us.muncodatuacao 
		LEFT JOIN territorios.municipio mu2 ON mu2.muncod = ie.muncod  
		".(($where)?"WHERE ".implode(" AND ",$where):"")." 
		ORDER BY ".(($agrupador)?implode(",",$agrupador).",":"")."(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date,us.iusnome";

	$cabecalho = array('&nbsp;','Nome','Situação do pagamento','Perfil','Parcela','Esfera','Município','UF');
	$db->monta_lista_simples($sql, $cabecalho, 1000000, 5, 'center', '95%');
}

include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes.php";
include_once "_funcoes_pagamentos.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
monta_titulo( "Relatório Pagamento", 'Selecione os filtros e agrupadores desejados' );

?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">

function carregarRelatorio<?=PFL_TUTOR ?>(iusid,fpbid) {

	window.open(window.location+'&requisicao=carregarRelatorioTutor&iusid='+iusid+'&fpbid='+fpbid,'relatorios','scrollbars=1,width=1366,height=768,resizable=yes');


}

function carregarRelatorio<?=PFL_COORDENADORESTADUAL ?>(iusid,fpbid) {

	window.open(window.location+'&requisicao=carregarRelatorioCoordenadorEstadual&iusid='+iusid+'&fpbid='+fpbid,'relatorios','scrollbars=1,width=1366,height=768,resizable=yes');


}
	
function exibeRelatorioGeral(){
	
	var formulario = document.formulario;
	
    selectAllOptions(document.getElementById('estuf'));
    selectAllOptions(document.getElementById('muncod'));
    selectAllOptions(document.getElementById('fpbid'));
    selectAllOptions(document.getElementById('pflcod'));

	
	formulario.submit();
	
}

	
/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
				
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}
	
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
	
	
//-->
</script>


<form action="" method="post" name="formulario" id="filtro"> 

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

<?php

echo '<tr>';
echo '<td class="SubTituloDireita">Nome:</td>';
echo '<td>'.campo_texto('iusnome', "N", "S", "Nome", 45, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome'] ).'</td>';
echo '</tr>';


$stSql = "SELECT
				estuf AS codigo,
				estuf || ' / ' || estdescricao AS descricao
			FROM 
				territorios.estado
			ORDER BY
				2 ";
mostrarComboPopup( 'UFs:', 'estuf',  $stSql, '', 'Selecione a(s) UF(s)' );

$stSql = "SELECT
				muncod AS codigo,
				estuf || ' / ' || mundescricao AS descricao
			FROM 
				territorios.municipio
			ORDER BY
				2 ";
mostrarComboPopup( 'Municípios:', 'muncod',  $stSql, '', 'Selecione o(s) Município(s)' );				

$stSql = "SELECT f.fpbid as codigo, 'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
			FROM escolaterra.folhapagamento f 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A' ORDER BY fpbanoreferencia::text||m.mescod||'01'";

mostrarComboPopup( 'Parcelas:', 'fpbid',  $stSql, '', 'Selecione a(s) parcela(s)' );


$stSql = "SELECT p.pflcod as codigo, p.pfldsc as descricao 
			FROM seguranca.perfil p 
			INNER JOIN escolaterra.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE p.pflstatus='A' AND p.sisid=".SIS_ESCOLATERRA." ORDER BY 2";

mostrarComboPopup( 'Perfis:', 'pflcod',  $stSql, '', 'Selecione o(s) perfil(s)' );
				

?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				 <input type="button" name="visualizaravaliacoes" value="Visualizar Avaliações" onclick="exibeRelatorioGeral();"/>
			</td>
		</tr>
	</table>

</form>

<?

monta_grid();


?>
