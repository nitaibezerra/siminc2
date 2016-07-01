<?php 
$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

if( $_REQUEST['obrid'] ){
	
	$endereco = $_REQUEST['endlog'].' - '.$_REQUEST['endnum'].', '.$_REQUEST['endcom'].', '.$_REQUEST['endbai'];
?>
<table>
	<tr>
		<td>
			Nome da Obra: 
		</td>
		<td>
			<?=$_REQUEST['obrdesc'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Programa/Fonte:
		</td>
		<td>
			<?=$_REQUEST['plititulo'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Tipo de Obra:
		</td>
		<td>
			<?=$_REQUEST['tobadesc'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Valor da Obra:
		</td>
		<td>
			<?=number_format($_REQUEST['obrvalorprevisto'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td>	
			Endereço completo:
			</br>CEP:
		</td>
		<td>
			<?=$endereco ?></br> <?=$_REQUEST['endcep'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Situação:
		</td>
		<td>
			<?=$_REQUEST['situacao'] ?>
		</td>
	</tr>
</table>

<?php 
	exit;
}

if($_SESSION['par']['muncod']){

//	$sql = "SELECT
//				'<div border=\"0\" onclick=\"redireciona(' || oi.obrid || ')\" 
//				onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/subacaoObras&acao=A&tipoAba=listaObras&preid={$preid}&obrid=' || oi.obrid ||
//				'&obrdesc=' || coalesce(oi.obrdesc,'não informado') ||
//				'&plititulo=' || coalesce(plititulo,'não informado') ||
//				'&tobadesc=' || coalesce(tobadesc,'não informado') ||
//				'&obrvalorprevisto=' || coalesce(obrvalorprevisto,0) ||
//				'&endcep=' || coalesce(endcep,'não informado') ||
//				'&endlog=' || coalesce(endlog,'não informado') ||
//				'&endnum=' || coalesce(endnum,'não informado') ||
//				'&endcom=' || coalesce(endcom,'não informado') ||
//				'&endbai=' || coalesce(endbai,'não informado') ||
//				'&situacao=' || 
//				CASE WHEN iexsitdominialimovelregulariza = true
//					THEN 'Regularizado'
//					ELSE 'Não Regularizado'
//				END || 
//				'\')\" 
//				onmouseout=\"SuperTitleOff( this );\" >' || obrdesc || '</div>' as descricao
//			FROM
//				ob ras.o brainfraestrutura oi
//			INNER JOIN
//				entidade.endereco e ON e.endid = oi.endid AND e.muncod = '{$_SESSION['par']['muncod']}'
//			LEFT JOIN
//				monitora.pi_obra pio ON pio.obrid = oi.obrid
//			LEFT JOIN
//				monitora.pi_planointerno pipi ON pio.pliid = pipi.pliid 
//			LEFT JOIN
//				obras.tipoobra tpo ON tpo.tobaid = oi.tobraid
//			LEFT JOIN
//				obras.infraestrutura iex ON iex.iexid = oi.iexid
//			WHERE oi.prfid = 41";
	
	$sql = "SELECT
				'<div border=\"0\" onclick=\"redireciona(' || oi.obrid || ')\" 
				onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/subacaoObras&acao=A&tipoAba=listaObras&preid={$preid}&obrid=' || oi.obrid ||
				'&obrdesc=' || coalesce(oi.obrnome,'não informado') ||
				'&plititulo=' || coalesce(plititulo,'não informado') ||
				'&tobadesc=' || coalesce(tobdesc,'não informado') ||
				'&obrvalorprevisto=' || coalesce(obrvalorprevisto,0) ||
				'&endcep=' || coalesce(endcep,'não informado') ||
				'&endlog=' || coalesce(endlog,'não informado') ||
				'&endnum=' || coalesce(endnum,'não informado') ||
				'&endcom=' || coalesce(endcom,'não informado') ||
				'&endbai=' || coalesce(endbai,'não informado') ||
				'&situacao=' || 
				CASE WHEN iexsitdominialimovelregulariza = true
					THEN 'Regularizado'
					ELSE 'Não Regularizado'
				END || 
				'\')\" 
				onmouseout=\"SuperTitleOff( this );\" >' || obrnome || '</div>' as descricao
			FROM
				obras2.obras oi
			INNER JOIN obras2.empreendimento emp ON emp.empid = oi.empid
			INNER JOIN entidade.endereco e ON e.endid = oi.endid AND e.muncod = '{$_SESSION['par']['muncod']}'
			LEFT JOIN monitora.pi_obra pio ON pio.obrid = oi.obrid
			LEFT JOIN monitora.pi_planointerno pipi ON pio.pliid = pipi.pliid 
			LEFT JOIN obras2.tipoobra tpo ON tpo.tobid = oi.tobid
			LEFT JOIN obras2.infraestrutura iex ON iex.iexid = oi.iexid
			WHERE emp.prfid = 41";
	
	$arrObras = $db->carregar($sql);
}
?>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
	<?php if(!$_SESSION['par']['muncod'] || !$arrObras){ ?>
		<tr>
			<td align="center">
			Não existem obras no Município.
			</td>
		</tr>
	<?php }else{ ?>
		<tr>
			<td align="center">
			<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="color: rgb(51, 51, 51);">
			<thead>
				<tr>
					<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">Nome da Obra
					</td>
				</tr> 
			</thead>
			</table>
			<?php $arrCabecalho = array(""); ?>
			<?php $db->monta_lista_simples($sql,$arrCabecalho,100,20 )?>
			</td>
		</tr>
	<?php } ?>
</table>
<script>

function redireciona( obrid ){

	window.opener.location = '/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid;
	return false;
}
</script>