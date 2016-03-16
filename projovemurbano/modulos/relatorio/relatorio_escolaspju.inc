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
	if($_REQUEST['esfera'] == 'M'){
		$AND = "AND pju.muncod is not null";
	}else{
		$AND = "AND pju.muncod is null";
	}
		$sql = "
				(
				SELECT DISTINCT
					ent.entnome as NOME,
					ent.entcodent as CODIGO,
					ende. endcep as CEP,
					ende.endlog as logradouro,
					ende.endcom as complemento,
					ende.endbai as bairro,
					ende.endnum as numero,
					ende.estuf as UF,
					mu.mundescricao as municipio,
					ent.entnumdddcomercial as DDD,
					ent.entnumcomercial as TELEFONE
				FROM 
					projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN entidade.entidade ent ON ent.entid = nes.entid
				INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN territorios.municipio mu ON mu.muncod = mun.muncod 
				INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = mun.pmuid 
				INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = pm.pjuid $AND
				WHERE 
					munstatus='A' 
					AND nuc.nucstatus='A' 
					and nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
				ORDER BY 
					UF, municipio
				) UNION ALL (
				SELECT DISTINCT
					ent.entnome as NOME,
					ent.entcodent as CODIGO,
					ende. endcep as CEP,
					ende.endlog as logradouro,
					ende.endcom as complemento,
					ende.endbai as bairro,
					ende.endnum as numero,
					ende.estuf as UF,
					mu.mundescricao as municipio,
					ent.entnumdddcomercial as DDD,
					ent.entnumcomercial as TELEFONE
				FROM 
					projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN entidade.entidade ent ON ent.entid = nes.entid
				INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN territorios.municipio mu ON mu.muncod = mun.muncod 
				INNER JOIN projovemurbano.associamucipiopolo asm ON asm.munid = mun.munid 
				INNER JOIN projovemurbano.polo pol ON pol.polid = asm.polid 
				INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = pol.pmuid
				INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = pm.pjuid  $AND
				
				WHERE 
					mun.munstatus='A' 
					AND nuc.nucstatus='A' 
					and nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
				ORDER BY 
					UF, municipio
				)
			";
		
		$cabecalho = array("Nome","Código","CEP","Logradouro","Complemento","Bairro","Número","UF","Município","DDD","TELEFONE");
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
	$db->monta_lista($sql, $cabecalho,200, 5, 'N', 'center', $par2,'','',$alinhamento);
}
?>
