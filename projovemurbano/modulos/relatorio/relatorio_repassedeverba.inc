<?

checkAno();

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
echo '<br />';
monta_titulo('Projovem Urbano', 'Relatório de Repasse de Verba');

$bloq = 'S';
$disabled = '';

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
	jQuery('#esfera').addClass('obrigatorio');
	jQuery('#perid').addClass('obrigatorio');
    jQuery('.pesquisar').click(function(){
		jQuery('#xls').val('0');
    	var erro = false;
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).val() == '' ){
				alert('Campo obrigatório.');
				jQuery(this).focus();
				erro = true;
				return false;
			}
		});
		if( erro ){
			return false;
		}
        jQuery('#form_busca').submit();
    });
	jQuery('.btnImprimir').click(function(){
		jQuery('#xls').val('1');
		jQuery('#form_busca').submit();
    });
});
</script>
<form id="form_busca" name="form_busca" method="POST">
	<input type="hidden" id="req" name="req" value="">
	<input type="hidden" id="xls" name="xls" value="0">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2" width="30%">
				<b>Filtros de pesquisa</b>
			</td>
		</tr>
		 <tr>
            <td class="SubTituloDireita">Esfera:</td>
<?
if($_POST['esfera']){
	$esfera = $_POST['esfera'];
}
$arExp = array(
    array("codigo" => '',
        "descricao" => 'Selecione'),
    array("codigo" => 'M',
        "descricao" => 'Municipal'),
    array("codigo" => 'E',
        "descricao" => 'Estadual')
);
?>
            <td><?= $db->monta_combo("esfera", $arExp, 'S', '', '', '', '', '', 'S', 'esfera', '', '', 'Esfera','','perid'); ?></td>
        </tr>
		<tr>
			<td class="SubTituloDireita">Período</td>
			<td>
				<?
				if($_POST['perid']){
					$perid =$_POST['perid'];
				}
				$sql = "SELECT 
							perid as codigo,
							perdesc as descricao
						FROM 
							projovemurbano.periodocurso
						WHERE
							ppuid = {$_SESSION['projovemurbano']['ppuid']}
						ORDER BY
							1";
				$db->monta_combo('perid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'perid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button"	class="pesquisar" 	value="Pesquisar">
				<input type="button" 	class="btnImprimir" value="Gerar XLS" />
			</td>
		</tr>
	</table>
<?
if($_REQUEST['perid']){
	if($_SESSION['projovemurbano']['ppuid']!='1'){
		$programa 		= "tpr.tprdesc as programa,";
		$innerjoin1	  	= "INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid 
					 	   INNER JOIN projovemurbano.tipoprograma tpr ON tpr.tprid = nuc.tprid";
		$tprid 			= ",nuc.tprid";
		$wheretprid 	= "AND presenca.tprid = tpr.tprid";
		$innerjoin2	  	= "INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid";
	}
	
	if($_REQUEST['esfera']=='E'){
		$sql = "SELECT DISTINCT
					tm.estuf AS uf,
					$programa
					pju.pjuprefcnpj as cnpj, 
					entproj.entnome AS nome ,
					COALESCE(presenca.total,0) AS qtd
				FROM projovemurbano.cadastroestudante cae 
				INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = cae.pjuid
				$innerjoin1
				INNER JOIN entidade.entidade entproj ON entproj.entid = pju.entid 
				INNER JOIN projovemurbano.turma tur ON tur.turid=cae.turid
				INNER JOIN projovemurbano.diario dia ON dia.turid=tur.turid 
				left JOIN (SELECT cae.pjuid, count(distinct cae.caeid) as total $tprid 
					   FROM projovemurbano.diario dia 
					   INNER JOIN projovemurbano.diariofrequencia dif ON dif.diaid = dia.diaid 
					   INNER JOIN projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid 
					   INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = frq.caeid AND dia.turid = cae.turid
					   $innerjoin2
					   INNER JOIN projovemurbano.turma tur ON tur.turid=cae.turid 
					   WHERE dia.perid = {$_REQUEST['perid']} AND frq.frqqtdpresenca > 0 and cae.caestatus = 'A' AND turstatus = 'A' 
					   GROUP BY cae.pjuid $tprid) presenca ON presenca.pjuid = pju.pjuid $wheretprid
				INNER JOIN entidade.entidade ent ON ent.entid = tur.entid 
				INNER JOIN entidade.endereco ede ON ede.entid = ent.entid 
				INNER JOIN territorios.municipio tm ON tm.muncod = ede.muncod 
				WHERE 
					cae.caestatus = 'A' 
				AND pjustatus = 'A' 
				AND turstatus = 'A' 
				AND dia.perid = {$_REQUEST['perid']} 
				AND pju.muncod is null 
		ORDER BY tm.estuf";
		// ver($sql);
		$cabecalho = array("Estado","Programa","CNPJ","NOME","QTD DE ALUNOS ENTIDADE");
		$alinhamento	= array( 'center', 'center','center', 'left','center' );
	}else{
		$sql = "SELECT DISTINCT
					tm.estuf AS uf,
					tm.mundescricao as municipio,
					$programa
					pju.pjuprefcnpj as cnpj,
					entproj.entnome AS nome ,
					COALESCE(presenca.total,0) AS qtd
				FROM projovemurbano.cadastroestudante cae
				INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = cae.pjuid
				$innerjoin1
				INNER JOIN entidade.entidade entproj ON entproj.entid = pju.entid
				INNER JOIN projovemurbano.turma tur ON tur.turid=cae.turid
				INNER JOIN projovemurbano.diario dia ON dia.turid=tur.turid
				left JOIN (SELECT cae.pjuid,count(distinct cae.caeid) as total $tprid
							FROM projovemurbano.diario dia
							INNER JOIN projovemurbano.diariofrequencia dif ON dif.diaid = dia.diaid
							INNER JOIN projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid
							INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = frq.caeid AND dia.turid = cae.turid
							$innerjoin2
							INNER JOIN projovemurbano.turma tur ON tur.turid=cae.turid
							WHERE dia.perid = {$_REQUEST['perid']} AND frq.frqqtdpresenca > 0 and cae.caestatus = 'A' AND turstatus = 'A'
							GROUP BY cae.pjuid $tprid) presenca ON presenca.pjuid = pju.pjuid $wheretprid
				INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
				INNER JOIN entidade.endereco ede ON ede.entid = ent.entid
				INNER JOIN territorios.municipio tm ON tm.muncod = ede.muncod
				WHERE
				cae.caestatus = 'A'
				AND pjustatus = 'A'
				AND turstatus = 'A'
				AND dia.perid = {$_REQUEST['perid']}
				AND pju.estuf is null
				ORDER BY tm.estuf";
		
		$cabecalho = array("Estado","Município","Programa","CNPJ","NOME","QTD DE ALUNOS ENTIDADE");
		$alinhamento	= array( 'center', 'center','center', 'left','center' );
	}

if($_REQUEST['xls']=='1') {
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatRepasse".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatRepasse".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
	exit;
}
?>
</form>
<?php 
$db->monta_lista($sql, $cabecalho,200, 5, 'N', 'center', $par2,'','',$alinhamento);
}
?>
