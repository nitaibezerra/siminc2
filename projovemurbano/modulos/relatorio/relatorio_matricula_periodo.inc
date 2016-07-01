<?

checkAno();

// if( $_REQUEST['req'] ){
// 	$_REQUEST['req']($_REQUEST);
// 	die();
// }

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
echo '<br />';
monta_titulo('Projovem Urbano', 'Relatório Escolas');

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
// 	jQuery('#pflcod').addClass('obrigatorio');
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
			<td class="SubTituloCentro" colspan="3">
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
            <td>
            <td><?= $db->monta_combo("esfera", $arExp, 'S', '', '', '', '', '', 'S', 'esfera', '', '', 'Esfera','class="obrigatorio"'); ?></td>
        </tr>
         <tr>
            <td class="SubTituloDireita">Período:</td>
			<?
			if($_POST['esfera']){
				$perid = $_POST['perid'];
			}
				$sql = "SELECT
							perid as codigo,
							perdesc as descricao
						FROM
							projovemurbano.periodocurso
						WHERE	
							ppuid = {$_SESSION['projovemurbano']['ppuid']}";
			?>
            <td>
            <td><?= $db->monta_combo("perid", $sql, 'S', 'Selecione o Período', '', '', '', '', 'S', 'perid', '', '','','class="obrigatorio"','perid'); ?></td>
        </tr>
		<tr>
			<td class="SubTituloCentro" colspan="3">
				<input type="button"	class="pesquisar" 	value="Pesquisar">
				<input type="button" 	class="btnImprimir" value="Gerar XLS" />
			</td>
		</tr>
	</table>
<?
if($_REQUEST['esfera'] && $_REQUEST['perid']){	
	if($_REQUEST['esfera'] == 'M'){
		$AND = "AND pju.muncod is not null";
		$estuf = "mun.estuf,";
		$mundescricao = "mun.mundescricao,";
		$order = "mun.mundescricao,";
		$INNER = "INNER JOIN territorios.municipio mun ON mun.muncod = pju.muncod";
		$cabecalho = array("UF",'Município',"Público","Meta","Matrícula Total","Matrículas Ativas","Total de Estudantes (Frequência diferente de zero)");
		$alinhamento	= array( 'center', 'center','center', 'center','center','center','center');
	}else{
		$AND = "AND pju.muncod is null";
		$estuf = "pju.estuf,";
		$mundescricao = "";
		$order = "";
		$INNER = "";
		$cabecalho = array("UF","Público","Meta","Matrícula Total","Matrículas Ativas","Total de Estudantes (Frequência diferente de zero)");
		$alinhamento	= array( 'center', 'center','center', 'center','center','center');
	}
		$sql12 = "
		
					WITH meta1 as (SELECT
									mtpvalor as valor,mtp.pjuid,tpr.tprid
								FROM
									projovemurbano.tipoprograma tpr
								INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
								INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
								INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
								WHERE
									nn.ppuid != 1
								AND mtpvalor > 0
								AND tmp.tpmid NOT IN  (2,5,8,11,14,7,10,13)
								GROUP BY
									mtpvalor,mtp.pjuid,tpr.tprid
								),
						meta2 as (SELECT
									mtpvalor as valor,mtp.pjuid,tpr.tprid
								FROM
									projovemurbano.tipoprograma tpr
								INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
								INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
								INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
								WHERE
									nn.ppuid != 1			
								AND mtpvalor > 0
								AND tmp.tpmid NOT IN  (2,5,8,11,14,9,12,15)
								GROUP BY
									mtpvalor,mtp.pjuid,tpr.tprid
							),
						totalmatriculas as (SELECT
										coalesce(count(distinct cae.caeid),0) as qtd,nuc.tprid,cae.pjuid
									FROM
										projovemurbano.cadastroestudante cae
									INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid
									--WHERE
										--cae.caestatus = 'A'
									GROUP BY
										nuc.tprid,cae.pjuid
								),
						totalmatriculasativas as (SELECT
								coalesce(count(distinct cae.caeid),0) as qtd,nuc.tprid,cae.pjuid
							FROM
								projovemurbano.cadastroestudante cae
							INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid
							WHERE
								cae.caestatus = 'A'
							GROUP BY
								nuc.tprid,cae.pjuid
						)
					
					SELECT DISTINCT
						$estuf
						$mundescricao
						tprdesc,
						CASE
							WHEN adesaotermoajustado = 't'
							THEN(met1.valor)
							ELSE(met2.valor)
					
						END as meta,
						tot.qtd as total,
						tot2.qtd as totalativo,
						(SELECT 
							coalesce(count(distinct cae.caeid),0) as qtd
						FROM 
							projovemurbano.diario dia
						INNER JOIN projovemurbano.diariofrequencia dif ON dif.diaid = dia.diaid
						INNER JOIN projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid 
						INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = frq.caeid AND dia.turid = cae.turid
						INNER JOIN projovemurbano.turma tur ON tur.turid=cae.turid
						INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = cae.nucid
						WHERE dia.perid = {$_REQUEST['perid']}
						AND frq.frqqtdpresenca > 0 
						AND cae.caestatus = 'A'
						AND turstatus = 'A'
						AND cae.pjuid = pmu.pjuid
						AND nuc.tprid = pmu.tprid
						GROUP BY 
							cae.pjuid
						) as qtdperiodo
						
					FROM  
						projovemurbano.projovemurbano pju 
					$INNER
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid = pju.pjuid
					INNER JOIN projovemurbano.metasdoprograma mtp ON pmu.pjuid = mtp.pjuid
					INNER JOIN projovemurbano.tipometadoprograma tpm ON tpm.tpmid = mtp.tpmid
					INNER JOIN projovemurbano.tipoprograma tpr ON tpr.tprid = pmu.tprid
					LEFT JOIN meta1 met1 ON met1.pjuid = pmu.pjuid AND met1.tprid= pmu.tprid
					LEFT JOIN meta2 met2 ON met2.pjuid = pmu.pjuid AND met2.tprid= pmu.tprid
					LEFT JOIN totalmatriculas tot ON tot.tprid = pmu.tprid AND tot.pjuid = pmu.pjuid 
					LEFT JOIN totalmatriculasativas tot2 ON tot2.tprid = pmu.tprid AND tot2.pjuid = pmu.pjuid 
					WHERE  
						pju.ppuid = {$_SESSION['projovemurbano']['ppuid']}
					$AND
					GROUP BY
						$estuf
						$order 
						met1.valor,
						met2.valor,
						tprdesc,
						pju.adesaotermoajustado,
						 pmu.pjuid,
						 pmu.tprid,
						 tot.qtd,
						 tot2.qtd
					ORDER BY
						estuf,
						$order
						tprdesc,
						meta
				";
// 		ver($sql12,d);
		
	if($_REQUEST['xls']=='1') {
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatRepasse".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatRepasse".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql12,$cabecalho,100000,5,'N','100%',$par2);
		exit;
	}
?>
</form>
<?php 
	$db->monta_lista($sql12, $cabecalho,50, 5, 'S', 'center', $par2,'','',$alinhamento);
}
	?>
