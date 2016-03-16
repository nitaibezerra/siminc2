<?

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Projovem Campo', 'Relatório de Perfil Estudante ');

$bloq = 'S';
$disabled = '';

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemcampo.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
	jQuery('#esfera').addClass('obrigatorio');
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
			<td class="SubTituloCentro" colspan="2">
			<input type="button"	class="pesquisar" 	value="Pesquisar">
				<input type="button" 	class="btnImprimir" value="Gerar XLS" />
			</td>
		</tr>
	</table>
<?
if($_REQUEST['esfera']){
		$sql = "SELECT
					est.estnome as nome,
					est.estdatanascimento as datanasc,
					extract(year from age(TO_CHAR(estdatanascimento,'YYYY-MM-DD')::DATE)) as idade,
					CASE 
						WHEN est.estsexo = 'F'
						THEN 'Feminino'
						ElSE 'Masculino'
					END as sexo,
					cor.cordescricao as cor,
					CASE
						WHEN est.defid is not null
						THEN def.defdescricao
						ELSE 'Não tem Deficiência'
					END as deficiencia,
					esc.escdescricao as estadocivil,
					CASE 
						WHEN estfilhos = 't'
						THEN 'Sim'
						ELSE 'Não'
					END as filhos,
					CASE 
						WHEN estbeneficiariooutroprograma = 't'
						THEN 'Sim'
						ELSE 'Não'
					END as beneoutro,
					CASE 
						WHEN est.estocupacao = 't'
						THEN ocudescricao
						ELSE 'Não tem ocupação'
					END as ocupacao,
					seg.sesdescricao as segmento,
					CASE 
						WHEN estparticipouprojovemcampo = 't'
						THEN 'Sim'
						ELSE 'Não'
					END as projovemurbano,
					ende.estuf as estado,
					mun.mundescricao as municipio,
					CASE 
						WHEN apc.apcesfera = 'E'
						THEN 'Estadual'
						ELSE 'Municipal'
					END as esfera
			FROM
				projovemcampo.estudante est
			INNER JOIN projovemcampo.corraca cor ON cor.corid = est.corid
			INNER JOIN projovemcampo.estadocivil esc ON esc.escid = est.escid
			INNER JOIN projovemcampo.segmentosocial	seg ON seg.sesid = est.sesid
			INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
			INNER JOIN entidade.endereco ende ON ende.entid = tur.entid
			LEFT JOIN  projovemcampo.deficiencia def ON def.defid = est.defid
			LEFT JOIN projovemcampo.ocupacao ocu ON ocu.ocuid = est.ocuid
			LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE
					est.eststatus = 'A'
			AND apc.apcesfera = '{$_REQUEST['esfera']}'
			ORDER BY esfera,ende.estuf, mun.mundescricao, nome, datanasc, sexo, cor, estadocivil, deficiencia, filhos, beneoutro, ocupacao, segmento, projovemurbano";
		
		$cabecalho = array("Nome","Data de Nascimento","Idade","Sexo","Cor/Raça","Pessoa com Deficiência?","Estado Civil","Tem Filhos?","Beneficiário de Outros Programas","Tem Alguma Ocupação?","Segmento Social","Já participou do Projovem Urbano?","UF","Município","Esfera");
		$alinhamento	= array( 'center', 'center','center', 'left','center' );
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
$db->monta_lista($sql, $cabecalho,200, 5, 'N', 'center', $par2,'','',$alinhamento);
}
?>
