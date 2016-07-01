<?php

$campo = $_SESSION['par']['muncod'] ? 'muncod' 					 : 'estuf';
$valor = $_SESSION['par']['muncod'] ? $_SESSION['par']['muncod'] : $_SESSION['par']['estuf'];
$_SESSION['par']['tooid'] = 1;

$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

if( $_SESSION['par']['esfera'] == 'M' ){
	$_SESSION['par']['inuid'] = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$_SESSION['par']['muncod']."'" );
	$_SESSION['par']['itrid'] = 2;
} elseif( $_SESSION['par']['esfera'] == 'E' ){
	$_SESSION['par']['inuid'] = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$_SESSION['par']['estuf']."'" );
	$_SESSION['par']['itrid'] = 1;
}

$obSubacaoControle = new SubacaoControle();
$obPreObra = new PreObra();

$arDados = $obSubacaoControle->recuperarPreObra($preid);

$lnk = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dadosOrcamentarios&preid=".$_REQUEST['preid'];
echo carregaAbasProInfancia($lnk, $_REQUEST['preid'], $descricaoItem);

monta_titulo( 'Dados Orçamentários', $obraDescricao  );

if(count($arDados)){

	if($arDados['preid']){
		$_SESSION['par']['preid'] = $arDados['preid'];
	}

	if($arDados['muncod']){
		$municipio = $obSubacaoControle->recuperaDescricaoMunicipio($arDados['muncod']);
	}
}

$muncod = !empty($arDados['muncod']) ? $arDados['muncod'] : $_SESSION['par']['muncod'];
?>
<body>

<form action="" method="post" name="formulario" id="formulario">
	<?php echo cabecalho(); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="10%" style="text-align: right;" class="SubTituloDireita">Nome da Obra:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=$arDados['predescricao'];?>
			</td>
		</tr>
		<tr>
			<td width="10%" style="text-align: right;" class="SubTituloDireita">Tipo da Obra:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=$db->pegaUm("SELECT ptodescricao FROM obras.pretipoobra WHERE ptoid = {$arDados['ptoid']}");?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center;font-size:12px;" colspan="4"><b>Dados de Empenhos</b></td>
		</tr>
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>NE</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Empenho</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Empenhado</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Empenho</b></td>
		</tr>
	<?php
		$sql = "select ne as empnumero, ((v.saldo * 100) / p.prevalorobra )::NUMERIC(20,2)  as porcentagemempenhado, valorempenho as eobvalorempenho,  empsituacao ,p.prevalorobra 
			from par.v_saldo_obra_por_empenho v
			inner join par.empenho e on e.empid = v.empid
			inner join obras.preobra p on p.preid = v.preid and p.prevalorobra > 0
			where v.preid={$preid}
				ORDER BY
					empnumero";
		
		$dados = $db->carregar($sql);
		if( is_array($dados) ){
			$tot_emp = 0;
			foreach( $dados as $dado ){
				$tot_emp += $dado['eobvalorempenho'];
	?>
		<tr>
			<td><b><?=$dado['empnumero']?></b></td>
			<td><?=number_format($dado['porcentagemempenhado'],2,',','.')?> %</td>
			<td>R$ <?=number_format($dado['eobvalorempenho'],2,',','.')?></td>
			<td><?=$dado['empsituacao']?></td>
		</tr>

	<?
			}
	?>
		<tr>
			<td style="background-color:#CCCCCC;text-align:right"><b>Total:</b></td>
			<td style="background-color:#CCCCCC;"><b><?=number_format((($tot_emp*100) / $dado['prevalorobra'] ),2,',','.')?> %</b></td>
			<td style="background-color:#CCCCCC;"><b>R$ <?=number_format($tot_emp,2,',','.')?></b></td>
			<td style="background-color:#CCCCCC;text-align:center"><b></b></td>
		</tr>
	<?
		}else{
	?>
		<tr>
			<td style="color:red;text-align:center" colspan="4">Não possui registros</td>
		</tr>
	<?
		}
	?>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center;font-size:12px;" colspan="4"><b>Dados de Pagamento</b></td>
		</tr>
	<?php
		$sql = "SELECT
					pagparcela,
					(( pobvalorpagamento * 100) / pre.prevalorobra  )::NUMERIC(20,2) as pobpercentualpag,
					pobvalorpagamento,
					pagsituacaopagamento,
					emp.empnumero,
					pre.prevalorobra
				FROM
					par.pagamento pag
				INNER JOIN par.empenho 		emp ON emp.empid = pag.empid
				INNER JOIN par.pagamentoobra 	pob ON pob.pagid = pag.pagid
				INNER JOIN obras.preobra 	pre ON pre.preid = pob.preid AND pre.prevalorobra > 0
				WHERE
					pagsituacaopagamento not ilike '%CANCELADO%'
					AND pag.pagstatus = 'A'
					AND pre.preid = $preid
				ORDER BY
					empnumero,
					pagparcela";
		
		$dados = $db->carregar($sql);
		if( is_array($dados) ){
			$numeroempenho = 0;
			$tot_pag = 0;
			foreach( $dados as $dado ){
				if( $numeroempenho != $dado['empnumero'] ){
					$numeroempenho = $dado['empnumero'];
					if( $tot_pag > 0 ){
	?>
		<tr>
			<td style="background-color:#CCCCCC;text-align:right"><b>Total:</b></td>
			<td style="background-color:#CCCCCC;"><b><?=number_format((($tot_pag*100) / $dado['prevalorobra'] ),2,',','.')?> %</b></td>
			<td style="background-color:#CCCCCC;"><b>R$ <?=number_format($tot_pag,2,',','.')?> </b></td>
			<td style="background-color:#CCCCCC;"><b></b></td>
		</tr>
	<?
						$tot_pag = 0;
					}
	?>
		<tr>
			<td colspan="4" ><b>Empenho <?=$dado['empnumero']?></b></td>
		</tr>
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>Parcela</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Pagamento</b></td>
		</tr>

	<?
				}
	?>
		<tr>
			<td><?=$dado['pagparcela']?></td>
			<td><?=number_format($dado['pobpercentualpag'],2,',','.')?> %</td>
			<td>R$ <?=number_format($dado['pobvalorpagamento'],2,',','.')?> </td>
			<td><?=$dado['pagsituacaopagamento']?></td>
		</tr>

	<?
				$tot_perc_pag 	+= $dado['pobpercentualpag'];
				$tot_pag 		+= $dado['pobvalorpagamento'];
			}
	?>
		<tr>
			<td style="background-color:#CCCCCC;text-align:right"><b>Total:</b></td>
			<td style="background-color:#CCCCCC;"><b><?=number_format((($tot_pag*100) / $dado['prevalorobra'] ),2,',','.')?> %</b></td>
			<td style="background-color:#CCCCCC;"><b>R$ <?=number_format($tot_pag,2,',','.')?> </b></td>
			<td style="background-color:#CCCCCC;"><b></b></td>
		</tr>
	<?
		}else{
	?>
		<tr>
			<td style="color:red;text-align:center" colspan="4">Não possui registros</td>
		</tr>
	<?
		}
	?>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"  align="center">
		<tr bgcolor="#dcdcdc">
			<td style="text-align: center">
				<table width="100%">
					<tr>
						<td align="left">
							<?php if($preid){ ?>
							<input class="navegar" type="button" value="Anterior" disabled />
							<?php } ?>
						</td>
						<td align="center">
						</td>
						<td align="right">
							<?php if($preid){ ?>
								<input class="navegar" type="button" value="Próximo" />
							<?php } ?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
	if(jQuery('#muncod1').val()){
		alteraComboMuncod();
	}
</script>
<div id="divDebug"></div>
</body>