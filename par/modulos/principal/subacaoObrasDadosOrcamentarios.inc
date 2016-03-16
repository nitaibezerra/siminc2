<?php

if( $_SESSION['par']['itrid'] == 2 || $_SESSION['par']['esfera'] == 'M'){
	$mun = $_SESSION['par']['muncod'];
} else {
	$mun = '';
}

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

if($_SESSION['par']['muncod']){
	$arEscolasQuadraSelecionadas = $oPreObraControle->verificaEscolasQuadraSelecionadas($_SESSION['par']['muncod']);
	$boTipo_A = $oPreObraControle->verificaGrupoMunicipioTipoObra_A($_SESSION['par']['muncod']);
}
$arEscolasQuadraSelecionadas = $arEscolasQuadraSelecionadas ? $arEscolasQuadraSelecionadas : array();


//$arEscolasQuadra = $oPreObraControle->recuperarEscolasQuadra($preid);
$sqlEscolasQuadra = $oPreObraControle->recuperarSqlEscolasQuadra($preid);
$arEscolasQuadra = $arEscolasQuadra ? $arEscolasQuadra : array();

$estadoAtualSubacao = wf_pegarEstadoAtual( $subacao['docid'] );

if($preid){
	$obSubacaoControle = new SubacaoControle();
	$obPreObra = new PreObra();
	$arDados = $obSubacaoControle->recuperarPreObra($preid);
	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);

/* 	if( (($esdid == WF_PAR_EM_CADASTRAMENTO || $esdid == WF_PAR_EM_DILIGENCIA || $esdid == WF_PAR_OBRA_EM_REFORMULACAO || $esdid == WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL) &&
		(
			in_array(PAR_PERFIL_PREFEITO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)
		)) || (in_array(PAR_PERFIL_ADM_OBRAS, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil))
	){
		$habil = 'S';
	} else {
		$habil = 'N';
	}
} else {
	if( $estadoAtualSubacao['esdid'] == WF_SUBACAO_ATUALIZACAO && (
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil))
	){
		$habil = 'S';
	} */
}

cabecalho();

?>
<form name="formulario" id="formulario" method="post" action="" >
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
		$sql = "select ne as empnumero, CASE WHEN p.prevalorobra > 0 THEN (v.saldo * 100) / coalesce(p.prevalorobra)::NUMERIC(20,2) ELSE 0::NUMERIC(20,2) END  as porcentagemempenhado, saldo as eobvalorempenho,  empsituacao ,p.prevalorobra 
			from par.v_saldo_obra_por_empenho v
			inner join par.empenho e on e.empid = v.empid
			inner join obras.preobra p on p.preid = v.preid 
			where v.preid= {$preid}
                            AND CASE WHEN p.prevalorobra > 0 THEN (v.saldo * 100) / coalesce(p.prevalorobra)::NUMERIC(20,2) ELSE 0::NUMERIC(20,2) END > 0
			ORDER BY
			empnumero		
		"; 
		$dados = $db->carregar($sql);
		if( is_array($dados) ){
			foreach( $dados as $dado ){
	?>
		<tr>
			<td><?=$dado['empnumero']?></td>
			<td><?=$dado['porcentagemempenhado']?> %</td>
			<td><?=number_format($dado['eobvalorempenho'],2,',','.')?></td>
			<td><?=$dado['empsituacao']?></td>
		</tr>

	<?
			}
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
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>Parcela</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Pagamento</b></td>
		</tr>
	<?php
		$sql = "SELECT
					pagparcela,
					poppercentualpag,
					popvalorpagamento,
					pagsituacaopagamento
				FROM
					par.pagamento pag
				INNER JOIN par.pagamentoobrapar pob ON pob.pagid = pag.pagid
				WHERE
					pagsituacaopagamento not ilike '%CANCELADO%'
					AND pag.pagstatus = 'A'
					AND preid = $preid";
// 		ver($sql,d);
		$dados = $db->carregar($sql);
// 		ver($dados);
		if( is_array($dados) ){
			foreach( $dados as $dado ){
	?>
		<tr>
			<td><?=$dado['pagparcela']?></td>
			<td><?=$dado['poppercentualpag']?> %</td>
			<td><?=number_format($dado['popvalorpagamento'],2,',','.')?></td>
			<td><?=$dado['pagsituacaopagamento']?></td>
		</tr>

	<?
			}
		}else{
	?>
		<tr>
			<td style="color:red;text-align:center" colspan="4">Não possui registros</td>
		</tr>
	<?
		}
	?>
	</table>
</form>