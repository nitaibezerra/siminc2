<?php
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
print "<br/>";
//validaSessionPTA( $_SESSION['emenda']['federal'] );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

$boConvenio = ($estadoAtual == CONVENIO_CANCELADO ? 'Sim' : 'Não');

$ptrid = $_SESSION["emenda"]["ptrid"];

echo exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

monta_titulo( 'Dados do Convênio', '');

echo cabecalhoPlanoTrabalho( $ptrid );

$sql = "SELECT
			ptr.ptrcod,
		    ptr.ptrnumconvenio,
		    ptr.ptrnumprocessoempenho,
		    enb.enbsituacaohabilita,
		    to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio,
		    to_char(ptp.pubdatapublicacao, 'DD/MM/YYYY') as pubdatapublicacao,
		    to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim,
		    b.bconome,
			cc.cocnu_agencia,
		    cc.cocnu_conta_corrente,
		    to_char(ptv.vigdatafim - now(), 'DD') as diasfim,
		    to_char(CAST(ptv.vigdatafim AS DATE) + 60, 'DD/MM/YYYY') as dataprestacao
		FROM
			emenda.planotrabalho ptr
		    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
		    inner join emenda.ptvigencia ptv on ptv.ptrid = ptr.ptrid
		    inner join emenda.ptminutaconvenio ptm on ptm.ptrid = ptr.ptrid and ptm.pmcstatus = 'A'
		    left join emenda.ptpublicacao ptp on ptp.pmcid = ptm.pmcid
		    left join emenda.contacorrente cc on cc.ptrid = ptr.ptrid
			left join emenda.banco b on b.bcoid = ptr.bcoid
		WHERE
			ptr.ptrid = $ptrid
		    and enb.enbstatus = 'A'
		    and ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')";
$arDados = $db->pegaLinha( $sql );

$arDados['ptrnumprocessoempenho'] = substr($arDados['ptrnumprocessoempenho'],0,5) . "." .
											substr($arDados['ptrnumprocessoempenho'],5,6) . "/" .
											substr($arDados['ptrnumprocessoempenho'],11,4) . "-" .
											substr($arDados['ptrnumprocessoempenho'],15,2);

if( $arDados['diasfim'] <= 60 ){
	$fimVigencia = "<div style='text-align: left; width: 50%; color: red;'>".$arDados['vigdatafim']."</div><div style='text-align: center; width: 50%; color: red;'>ATENÇÃO<br>
	Faltam ".$arDados['diasfim']." dias para o fim da vigência do convênio.</div>";
} else {
	$fimVigencia = $arDados['vigdatafim'];
}
?>
<form name="formulario" id="formulario" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<?if( !empty($arDados['ptrnumconvenio']) ){ ?>
		<tr>
			<td class="SubtituloDireita" style="width: 33%">Número do Convênio:</td>
			<td bgcolor="#F7F7F7"><?=$arDados['ptrnumconvenio']; ?></td>
			<td class="SubtituloDireita" style="width: 33%">Número do Processo:</td>
			<td bgcolor="#F7F7F7"><?=$arDados['ptrnumprocessoempenho']; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Situação de Habilitação:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$arDados['enbsituacaohabilita']; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Dados Bancários:</td>
			<td colspan="4" bgcolor="#F7F7F7">
				<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
				<tr>
					<td class="SubtituloDireita" style="width: 20%">Banco:</td>
					<td style="80%" bgcolor="#F7F7F7"><?=$arDados['bconome']; ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita">Agência</td>
					<td bgcolor="#F7F7F7"><?=$arDados['cocnu_agencia']; ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita">Conta</td>
					<td bgcolor="#F7F7F7"><?=$arDados['cocnu_conta_corrente']; ?></td>
				</tr></table>
			</td>	
		</tr>
		<tr>
			<td class="SubtituloDireita">Objeto do Convênio:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?
			$sql = "SELECT
						obj.obcdsc
					FROM
						emenda.objetoconvenio obj
					    inner join emenda.objetominutaconvenio obm on obm.obcid = obj.obcid
					    inner join emenda.ptminutaconvenio ptm on ptm.pmcid = obm.pmcid and ptm.pmcstatus = 'A'
					WHERE
						ptm.ptrid = ".$ptrid;
			$arObjeto = $db->carregarColuna( $sql );
			$arObjeto = $arObjeto ? $arObjeto : array();
			echo implode( '<br>', $arObjeto );
			?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data de Assinatura Convênio e/ou Termo Aditivo:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$arDados['vigdatainicio']; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data da Publicação no DOU:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$arDados['pubdatapublicacao']; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data do Fim da vigência:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$fimVigencia;?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Quantidade de dias para apresentação da Prestação de Contas:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$arDados['diasfim'] + 60; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data para apresentação da Prestação de Contas:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$arDados['dataprestacao']; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Convênio cancelado:</td>
			<td colspan="4" bgcolor="#F7F7F7"><?=$boConvenio; ?></td>
		</tr>
		<?} else { ?>
		<tr>
			<td style="text-align: center; color: red;">Para este plano de trabalho não existe convênio firmado até o momento.</td>
		</tr>
		<?} ?>
	</table>
</form>