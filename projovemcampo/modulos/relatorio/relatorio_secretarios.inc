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
monta_titulo('Projovem Campo', 'Relatório Secretários/Coordenadores');

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
<!-- 		<tr> -->
<!-- 			<td class="SubTituloDireita">Perfil</td> -->
<!-- 			<td> -->
				<?
// 				if($_POST['perid']){
// 					$pflcod =$_POST['pflcod'];
// 				}
// 				$sql = "
// 						SELECT
// 							pflcod as codigo,
// 							pfldsc as  descricao
// 						FROM
// 							seguranca.perfil
// 						WHERE
// 							pflcod in(645,646,647,648)
// 						ORDER BY
// 							descricao";
// 				$db->monta_combo('pflcod', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'pflcod');
// 				?>
<!-- 			</td> -->
<!-- 		</tr> -->
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button"	class="pesquisar" 	value="Pesquisar">
				<input type="button" 	class="btnImprimir" value="Gerar XLS" />
			</td>
		</tr>
	</table>
<?
	
if($_REQUEST['esfera']=='E'){
		$sql = "(SELECT DISTINCT
					'Secretário Estadual' as cargo,
					seco.seconome as nome,
					--seco.seconumrg as rg,
					usu.usufoneddd||'-'||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA ESTADUAL'||'-'|| mun.estuf  as nome2,
					mun.estuf as uf,
					mun.mundescricao as municipio,
					to_char(sec.secacep::numeric, '99999-999') as cep,
					sec.secaendereco as endereco,
					sec.secabairro as Bairro,
					sec.secanumero as numero,
					sec.secacomplemento as complemento,
					to_char(sec.secatelefone::numeric, '99-9999-9999') as telefone2
					
				FROM
					projovemcampo.adesaoprojovemcampo ads 
				INNER JOIN projovemcampo.secretaria sec ON sec.secaid = ads.secaid
				INNER JOIN projovemcampo.secretario seco ON sec.secoid = seco.secoid 
				LEFT JOIN seguranca.usuario usu ON seco.secocpf = usu.usucpf
				INNER JOIN territorios.municipio mun ON mun.muncod = sec.secamuncod
				WHERE
					ads.apcstatus = 't'
				AND ads.apctermoaceito = 't'
				and apcesfera in ('E')
				Order By
					uf, municipio, nome, cargo
				)UNION ALL(
				SELECT DISTINCT
					'Coordenador Estadual' as cargo,
					usu.usunome as nome,
					--'--' as rg,
					usu.usufoneddd||'-'||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA ESTADUAL'||'-'|| mun.estuf  as nome2,
					mun.estuf as uf,
					mun.mundescricao as municipio,
					to_char(sec.secacep::numeric, '99999-999') as cep,
					sec.secaendereco as endereco,
					sec.secabairro as Bairro,
					sec.secanumero as numero,
					sec.secacomplemento as complemento,
					to_char(sec.secatelefone::numeric, '99-9999-9999') as telefone2
					
				FROM
					projovemcampo.adesaoprojovemcampo ads 
				INNER JOIN projovemcampo.secretaria sec ON sec.secaid = ads.secaid
				LEFT JOIN seguranca.usuario usu ON sec.secoordcpf = usu.usucpf
				INNER JOIN territorios.municipio mun ON mun.muncod = sec.secamuncod
				WHERE
					 ads.apcstatus = 't'
				AND ads.apctermoaceito = 't'
				and apcesfera in ('E')
				Order By
					uf, municipio, usu.usunome, cargo)";
		$cabecalho = array("Função","Nome","Telefone","E-mail","Secretaria","UF","Município","CEP","Endereço","Bairro","Número","Complemento","Telefone");
		$alinhamento	= array('center','center','center','center', 'center','center','center', 'center','center' );
	}elseif($_REQUEST['esfera']=='M'){
		$sql = "(SELECT DISTINCT
						'Secretário Municipal' as cargo,
						seco.seconome as nome,
						--seco.seconumrg as rg,
						to_char(seco.secocelular::numeric, '99-9999-9999') as telefone,
						usu.usuemail as email,
						'SECRETARIA ESTADUAL'||'-'|| mun.estuf  as secnome,
						mun.estuf as uf,
						mun.mundescricao as municipio,
						to_char(sec.secacep::numeric, '99999-999') as cep,
						sec.secaendereco as endereco,
						sec.secabairro as Bairro,
						sec.secanumero as numero,
						sec.secacomplemento as complemento,
						to_char(sec.secatelefone::numeric, '99-9999-9999') as telefone2
				FROM
						projovemcampo.secretaria sec 
				INNER JOIN projovemcampo.secretario seco ON sec.secoid = seco.secoid
				INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.apccodibge::character(7) = sec.secamuncod and apcesfera in ('M') 
				LEFT JOIN seguranca.usuario usu ON seco.secocpf = usu.usucpf
				INNER JOIN territorios.municipio mun ON mun.muncod = sec.secamuncod
				WHERE
					ads.apcstatus = 't'
				AND ads.apctermoaceito = 't'
				Order By
					uf, municipio, nome, cargo
				)UNION ALL(
				SELECT DISTINCT
					'Coordenador Municipal' as cargo,
					usu.usunome as nome,
					--'' as rg,
					usu.usufoneddd||'-'||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA ESTADUAL'||'-'|| mun.estuf  as secnome,
					mun.estuf as uf,
					mun.mundescricao as municipio,
					to_char(sec.secacep::numeric, '99999-999') as cep,
					sec.secaendereco as endereco,
					sec.secabairro as Bairro,
					sec.secanumero as numero,
					sec.secacomplemento as complemento,
					to_char(sec.secatelefone::numeric, '99-9999-9999') as telefone2
				FROM
						projovemcampo.secretaria sec 
				INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.apccodibge::character(7) = sec.secamuncod and apcesfera in ('M') 
				LEFT JOIN seguranca.usuario usu ON sec.secoordcpf = usu.usucpf
				INNER JOIN territorios.municipio mun ON mun.muncod = sec.secamuncod
				WHERE
						ads.apcstatus = 't'
				AND ads.apctermoaceito = 't'
				Order By
					uf, municipio, usu.usunome, cargo)";
		
		$cabecalho = array("Função","Nome","Telefone","E-mail","Secretaria","UF","Município","CEP","Endereço","Bairro","Número","Complemento","Telefone");
		$alinhamento	= array('center','center', 'center','center', 'center','center','center', 'center','center');
	}
// ver($sql,d);
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
?>
