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
monta_titulo('Projovem Urbano', 'Relatório Secretários/Coordenadores');

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
if($_REQUEST['esfera']){	
	if($_REQUEST['esfera']=='E'){
		$sql = "(SELECT DISTINCT
					pfl.pfldsc as cargo,
					usu.usunome as nome,
					--ise.iserg as rg,
					usu.usufoneddd||' - '||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA ESTADUAL'||'-'|| ise.iseuf  as secome,
					ise.iseuf as uf,
					mun.mundescricao as municipio,
					CASE
						WHEN ise.isecomplemento is not null
						THEN ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero ||' Complemento - '||isecomplemento 
						ELSE ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero 
					END as endereco,
					entnumdddcomercial||'-'||entnumcomercial as telsecretaria
							
				FROM
					projovemurbano.usuarioresponsabilidade res
				INNER JOIN seguranca.usuario usu ON usu.usucpf = res.usucpf
				INNER JOIN seguranca.perfil pfl ON pfl.pflcod = res.pflcod
				INNER JOIN projovemurbano.projovemurbano pro ON pro.estuf = res.estuf AND pro.muncod is null
				INNER JOIN entidade.entidade ent ON ent.entid = pro.entid
				INNER JOIN projovemurbano.identificacaosecretario ise ON  ise.isecpf = res.usucpf AND ise.pjuid = pro.pjuid
				INNER JOIN territorios.municipio mun ON mun.muncod = ise.isemunicipio
				WHERE
					res.pflcod in (645)
				AND res.rpustatus = 'A'
				AND pro.ppuid = 3
				AND pro.pjustatus = 'A'
				AND pro.adesaotermo = 't'
				Order By
					uf, municipio, usu.usunome, cargo
				)UNION ALL(
					SELECT DISTINCT
					pfl.pfldsc as cargo,
					usu.usunome as nome,
					--'' as rg,
					usu.usufoneddd||' - '||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA ESTADUAL'||'-'|| mun.estuf  as secome,
					mun.estuf as uf,
					mun.mundescricao as municipio,
					CASE
						WHEN ise.isecomplemento is not null
						THEN ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero ||' Complemento - '||isecomplemento 
						ELSE ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero 
					END as endereco,
					entnumdddcomercial||'-'||entnumcomercial as telsecretaria
							
				FROM
					projovemurbano.usuarioresponsabilidade res
				INNER JOIN seguranca.usuario usu ON usu.usucpf = res.usucpf
				INNER JOIN seguranca.perfil pfl ON pfl.pflcod = res.pflcod
				INNER JOIN projovemurbano.projovemurbano pro ON pro.estuf = res.estuf AND pro.muncod is null
				INNER JOIN projovemurbano.identificacaosecretario ise ON  ise.pjuid = pro.pjuid AND ise.pjuid = pro.pjuid
				INNER JOIN entidade.entidade ent ON ent.entid = pro.entid
				INNER JOIN projovemurbano.coordenadorresponsavel cor ON  cor.corcpf = res.usucpf
				INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				WHERE
					res.pflcod in (647)
				AND res.rpustatus = 'A'
				AND pro.ppuid = 3
				AND pro.pjustatus = 'A'
				AND pro.adesaotermo = 't'
				Order By
					uf, municipio, usu.usunome, cargo
				)
			";
		$cabecalho = array("Função","Nome","Telefone","E-mail","Secretaria","UF","Município","Endereço","Telefone");
		$alinhamento	= array( 'center', 'center','center', 'center','center','center', 'center','center' );
	}elseif($_REQUEST['esfera']=='M'){
		$sql = "(SELECT DISTINCT
					pfl.pfldsc as cargo,
					usu.usunome as nome,
					--ise.iserg as rg,
					usu.usufoneddd||' - '||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA MUNICPAL'||'-'||mun.mundescricao||'-'||ise.iseuf  as secome,
					ise.iseuf as uf,
					mun.mundescricao as municipio,
					CASE
						WHEN ise.isecomplemento is not null
						THEN ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero ||' Complemento - '||isecomplemento 
						ELSE ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero 
					END as endereco,
					entnumdddcomercial||'-'||entnumcomercial as telsecretaria
							
				FROM
					projovemurbano.usuarioresponsabilidade res
				INNER JOIN seguranca.usuario usu ON usu.usucpf = res.usucpf
				INNER JOIN seguranca.perfil pfl ON pfl.pflcod = res.pflcod
				INNER JOIN projovemurbano.projovemurbano pro ON pro.muncod = res.muncod AND pro.muncod is not null
				INNER JOIN entidade.entidade ent ON ent.entid = pro.entid
				INNER JOIN projovemurbano.identificacaosecretario ise ON  ise.isecpf = res.usucpf AND ise.pjuid = pro.pjuid
				INNER JOIN territorios.municipio mun ON mun.muncod = ise.isemunicipio
				WHERE
					res.pflcod in (646)
				AND res.rpustatus = 'A'
				AND pro.ppuid = 3
				AND pro.pjustatus = 'A'
				AND pro.adesaotermo = 't'
				Order By
					uf, municipio, usu.usunome, cargo
				)UNION ALL(
					SELECT DISTINCT
					pfl.pfldsc as cargo,
					usu.usunome as nome,
					--'' as rg,
					usu.usufoneddd||' - '||usufonenum as telefone,
					usu.usuemail as email,
					'SECRETARIA MUNICPAL'||'-'||mun.mundescricao||'-'||ise.iseuf  as secome,
					mun.estuf as uf,
					mun.mundescricao as municipio,
					CASE
						WHEN ise.isecomplemento is not null
						THEN ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero ||' Complemento - '||isecomplemento 
						ELSE ise.iseendereco||'-'||ise.isebairro||' N°'|| ise.isenumero 
					END as endereco,
					entnumdddcomercial||'-'||entnumcomercial as telsecretaria
							
				FROM
					projovemurbano.usuarioresponsabilidade res
				INNER JOIN seguranca.usuario usu ON usu.usucpf = res.usucpf
				INNER JOIN seguranca.perfil pfl ON pfl.pflcod = res.pflcod
				INNER JOIN projovemurbano.projovemurbano pro ON pro.muncod = res.muncod AND pro.muncod is not null
				INNER JOIN projovemurbano.identificacaosecretario ise ON  ise.pjuid = pro.pjuid AND ise.pjuid = pro.pjuid
				INNER JOIN entidade.entidade ent ON ent.entid = pro.entid
				INNER JOIN projovemurbano.coordenadorresponsavel cor ON  cor.corcpf = res.usucpf
				INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				WHERE
					res.pflcod in (648)
				AND res.rpustatus = 'A'
				AND pro.ppuid = 3
				AND pro.pjustatus = 'A'
				AND pro.adesaotermo = 't'
				Order By
					uf, municipio, usu.usunome, cargo
				)";
		
		$cabecalho = array("Função","Nome","Telefone","E-mail","Secretaria","UF","Município","Endereço","Telefone");
		$alinhamento	= array( 'center', 'center','center', 'center','center','center', 'center','center' );
	}
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
