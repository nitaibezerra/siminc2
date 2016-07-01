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
            <td class="SubTituloDireita">Possui polo?:</td>
			<?
				if($_POST['possuipolo']){
					$possuipolo = $_POST['possuipolo'];
				}
				$arExp = array(
				    array("codigo" => '',
				        "descricao" => 'Selecione'),
				    array("codigo" => '1',
				        "descricao" => 'Sim'),
				    array("codigo" => '2',
				        "descricao" => 'Não')
				);
			?>
            <td>
            <td><?= $db->monta_combo("possuipolo", $arExp, 'S', '', '', '', '', '', 'S', 'possuipolo', '', '','Possui Polo?','class="obrigatorio"','possuipolo'); ?></td>
        </tr>
		<tr>
			<td class="SubTituloCentro" colspan="3">
				<input type="button"	class="pesquisar" 	value="Pesquisar">
				<input type="button" 	class="btnImprimir" value="Gerar XLS" />
			</td>
		</tr>
	</table>
<?
if($_REQUEST['esfera'] && $_REQUEST['possuipolo']){	
	if($_REQUEST['esfera'] == 'M'){
		$AND = "AND pju.muncod is not null";
	}else{
		$AND = "AND pju.muncod is null";
	}
	if($_REQUEST['possuipolo'] == '2'){
		$sql = "
				SELECT DISTINCT
					CASE 
						WHEN pju.estuf is not null
						THEN 'SECRETARIA ESTADUAL'||'-'|| pju.estuf  
						ELSE 'SECRETARIA MUNICIPAL'||'-'||mu.mundescricao ||'-'||mu.estuf
					END AS secnome,
					'Não possui polo' as polo,
					'NÚCLEO'||'-'||nuc.nucid as nucleo,
					tur.turdesc as turma,
					(SELECT
						count(caeid)
					FROM
						(
					SELECT  
						caeid,
						SUM(total) as total,
						notatotal,
						presencaconselho,
						notaconselho         
					FROM 
						(
						SELECT 
							cae2.caeid as caeid,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE frq.caeid = cae2.caeid),0)as total,
							(coalesce(npc.notaciclo1,0) + coalesce(npc.notaciclo2,0) + coalesce(npc.notaciclo3,0))as notatotal,
							totalnotas as notaconselho,
							totalpresenca as presencaconselho
						FROM  projovemurbano.cadastroestudante cae2
						LEFT JOIN projovemurbano.avaliacaoconselhodeclasse avc ON avc.caeid = cae2.caeid   
						INNER JOIN projovemurbano.frequenciaestudante frq ON frq.caeid = cae2.caeid AND frq.frqstatus = 'A'
						INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
						INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
						INNER JOIN projovemurbano.periodocurso prc USING(perid)
						LEFT  JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae2.caeid AND npc.npc_status = 'A'
						WHERE  
						cae2.turid = tur.turid  AND cae2.caestatus = 'A'
				
						GROUP BY 
							cae2.caeid,
							cae2.caenome,
							cae2.caeid,
							dia.turid,
							frq.caeid,
							cae2.caecpf,
							cae2.turid,
							notatotal,
							totalnotas,
							totalpresenca) as somatoria
					GROUP BY
						caeid, 
						notatotal,
						notaconselho,
						presencaconselho 
					HAVING(((SUM(total) >= 1080) AND (notatotal >= 1100)) or (( presencaconselho>= 1080) AND (notaconselho >= 1100))))as somatoria2) as alunosap,
					(SELECT
						count(caeid)
					FROM
						(
					SELECT  
						caeid,
						SUM(total) as total,
						notatotal,
						presencaconselho,
						notaconselho         
					FROM 
						(
						SELECT 
							cae1.caeid as caeid,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE frq.caeid = cae1.caeid),0)as total,
							(coalesce(npc.notaciclo1,0) + coalesce(npc.notaciclo2,0) + coalesce(npc.notaciclo3,0))as notatotal,
							totalnotas as notaconselho,
							totalpresenca as presencaconselho
						FROM  projovemurbano.cadastroestudante cae1
						LEFT JOIN projovemurbano.avaliacaoconselhodeclasse avc ON avc.caeid = cae1.caeid   
						INNER JOIN projovemurbano.frequenciaestudante frq ON frq.caeid = cae1.caeid AND frq.frqstatus = 'A'
						INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
						INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
						INNER JOIN projovemurbano.periodocurso prc USING(perid)
						LEFT  JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae1.caeid AND npc.npc_status = 'A'
						WHERE  
						cae1.turid = tur.turid  AND cae1.caestatus = 'A'
						
						GROUP BY 
							cae1.caeid,
							cae1.caenome,
							cae1.caeid,
							dia.turid,
							frq.caeid,
							cae1.caecpf,
							cae1.turid,
							notatotal,
							totalnotas,
							totalpresenca) as somatoria
					GROUP BY
						caeid, 
						notatotal,
						notaconselho,
						presencaconselho 
					HAVING(((SUM(total) < 1080) OR (notatotal < 1100)) AND(notaconselho is NULL)))as somatoria2) as alunosrp
				FROM 
					projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.turma tur ON tur.nucid = nuc.nucid
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN entidade.entidade ent ON ent.entid = nes.entid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN territorios.municipio mu ON mu.muncod = mun.muncod 
				INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = mun.pmuid 
				INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = pm.pjuid 
				WHERE 
					munstatus='A' 
					AND nuc.nucstatus='A' 
					and nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}
				$AND
				ORDER By
					secnome,
					polo,
					nucleo,
					turma
				 ";
		}elseif($_REQUEST['possuipolo']=='1'){		
			$sql = "SELECT DISTINCT
						CASE 
							WHEN pju.estuf is not null
							THEN 'SECRETARIA ESTADUAL'||'-'|| pju.estuf  
							ELSE 'SECRETARIA MUNICIPAL'||'-'||mu.mundescricao||'-'||mu.estuf
						END AS secnome,
						'POLO'||'-'||pol.polid as polo,
						'NÚCLEO'||'-'||nuc.nucid as nucleo,
						tur.turdesc as turma,
						(SELECT
							count(caeid)
						FROM
							(
						SELECT  
							caeid,
							SUM(total) as total,
							notatotal,
							presencaconselho,
							notaconselho         
						FROM 
							(
							SELECT 
								cae2.caeid as caeid,
								coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE frq.caeid = cae2.caeid),0)as total,
								(coalesce(npc.notaciclo1,0) + coalesce(npc.notaciclo2,0) + coalesce(npc.notaciclo3,0))as notatotal,
								totalnotas as notaconselho,
								totalpresenca as presencaconselho
							FROM  projovemurbano.cadastroestudante cae2
							LEFT JOIN projovemurbano.avaliacaoconselhodeclasse avc ON avc.caeid = cae2.caeid   
							INNER JOIN projovemurbano.frequenciaestudante frq ON frq.caeid = cae2.caeid AND frq.frqstatus = 'A'
							INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
							INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
							INNER JOIN projovemurbano.periodocurso prc USING(perid)
							LEFT  JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae2.caeid AND npc.npc_status = 'A'
							WHERE  
							cae2.turid = tur.turid  AND cae2.caestatus = 'A'
					
							GROUP BY 
								cae2.caeid,
								cae2.caenome,
								cae2.caeid,
								dia.turid,
								frq.caeid,
								cae2.caecpf,
								cae2.turid,
								notatotal,
								totalnotas,
								totalpresenca) as somatoria
						GROUP BY
							caeid, 
							notatotal,
							notaconselho,
							presencaconselho 
						HAVING(((SUM(total) >= 1080) AND (notatotal >= 1100)) or (( presencaconselho>= 1080) AND (notaconselho >= 1100))))as somatoria2) as alunosap,
						(SELECT
							count(caeid)
						FROM
							(
						SELECT  
							caeid,
							SUM(total) as total,
							notatotal,
							presencaconselho,
							notaconselho         
						FROM 
							(
							SELECT 
								cae1.caeid as caeid,
								coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE frq.caeid = cae1.caeid),0)as total,
								(coalesce(npc.notaciclo1,0) + coalesce(npc.notaciclo2,0) + coalesce(npc.notaciclo3,0))as notatotal,
								totalnotas as notaconselho,
								totalpresenca as presencaconselho
							FROM  projovemurbano.cadastroestudante cae1
							LEFT JOIN projovemurbano.avaliacaoconselhodeclasse avc ON avc.caeid = cae1.caeid   
							INNER JOIN projovemurbano.frequenciaestudante frq ON frq.caeid = cae1.caeid AND frq.frqstatus = 'A'
							INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
							INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
							INNER JOIN projovemurbano.periodocurso prc USING(perid)
							LEFT  JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae1.caeid AND npc.npc_status = 'A'
							WHERE  
							cae1.turid = tur.turid  AND cae1.caestatus = 'A'
							
							GROUP BY 
								cae1.caeid,
								cae1.caenome,
								cae1.caeid,
								dia.turid,
								frq.caeid,
								cae1.caecpf,
								cae1.turid,
								notatotal,
								totalnotas,
								totalpresenca) as somatoria
						GROUP BY
							caeid, 
							notatotal,
							notaconselho,
							presencaconselho 
						HAVING(((SUM(total) < 1080) OR (notatotal < 1100)) AND(notaconselho is NULL)))as somatoria2) as alunosrp
					FROM 
						projovemurbano.nucleo nuc
					INNER JOIN projovemurbano.turma tur ON tur.nucid = nuc.nucid
					INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
					INNER JOIN entidade.entidade ent ON ent.entid = nes.entid
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
					INNER JOIN territorios.municipio mu ON mu.muncod = mun.muncod 
					INNER JOIN projovemurbano.associamucipiopolo asm ON asm.munid = mun.munid 
					INNER JOIN projovemurbano.polo pol ON pol.polid = asm.polid 
					INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = pol.pmuid 
					INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = pm.pjuid 
					WHERE 
						mun.munstatus='A' 
						AND nuc.nucstatus='A' 
						and nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}
					$AND
					ORDER By
						secnome,
						polo,
						nucleo,
						turma
				";
		}
		$cabecalho = array("Secretaria","Polo","Núcleo","Turma","Qtd. Alunos Aprovados","Qtd. Alunos Reprovados");
		$alinhamento	= array( 'left', 'center','center', 'center','center','center', 'center','center','center', 'center','center' );
// ver($sql);
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
	$db->monta_lista($sql, $cabecalho,50, 5, 'N', 'center', $par2,'','',$alinhamento);
}
	?>
