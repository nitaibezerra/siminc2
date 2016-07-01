<?php
//cadastraGuiaPTA('2503', '');
include_once APPRAIZ . 'includes/workflow.php';
//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

$obEmenda = new Emenda();
unset( $_SESSION['emenda']['federal'] );

$ptrid = $_SESSION["emenda"]["ptrid"];

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$sisid = '';

if($ptrid){
	$obPTA->verificaExistePTA($ptrid);				
	$arDadosPTA = $obPTA->carregaDadosDefinirRecursos( $ptrid );
	$arDadosPTA = $arDadosPTA ? $arDadosPTA : array(); 
	extract($arDadosPTA);
}
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual, $sisid );
monta_titulo( 'Dados Orçamentários', '');
echo $obPTA->cabecalhoPlanoTrabalho($ptrid);
?>
<html>
<head>
<!-- <script type="text/javascript" src="/includes/funcoes.js"></script> -->
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/pta.js"></script>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
</head>
<body>
	<br>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center;font-size:12px;" colspan="4"><b>Dados de Empenhos</b></td>
		</tr>
	</table>
	<br>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>NE</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Empenho</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Empenhado</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Empenho</b></td>
		</tr>
	<?php
		$sql = "SELECT 
					em.empnumero as empenho,
					((SUM(eobvalorempenho) * 100) / sum(pte.pedvalor)  )::NUMERIC(20,0) as percent_empenhado,
					sum(pte.pedvalor) as valor_pta,
					SUM(eobvalorempenho) as valor_empenho,
					em.empsituacao as situacao_empenho,
					pt.ptrcod,
					pt.ptrexercicio
				FROM 
					par.empenhosubacao                      e
				INNER JOIN par.subacao                      s   ON s.sbaid 		= e.sbaid and eobstatus = 'A'
				INNER JOIN par.subacaodetalhe               sd  ON sd.sbaid 	= s.sbaid AND e.eobano = sd.sbdano
				INNER JOIN par.subacaoemendapta          	se  ON se.sbdid 	= sd.sbdid and se.sepstatus = 'A' 
				INNER JOIN par.acao                         a   ON a.aciid 		= s.aciid 
				INNER JOIN par.pontuacao					p   ON p.ptoid 		= a.ptoid
				INNER JOIN par.instrumentounidade        	iu  ON iu.inuid 	= p.inuid
				INNER JOIN territorios.municipio            m   ON m.muncod 	= iu.muncod
				INNER JOIN emenda.planotrabalho           	pt  ON pt.ptrid 	= se.ptrid
				INNER JOIN emenda.ptemendadetalheentidade 	pte ON pte.ptrid 	= pt.ptrid
				INNER JOIN par.empenho                      em  ON e.empid 		= em.empid and empstatus = 'A'
				WHERE
					em.empsituacao <> 'CANCELADO'
					AND pt.ptrid = $ptrid
				GROUP BY
					em.empnumero,
					em.empsituacao,
					pt.ptrcod,
					pt.ptrexercicio";
		
		$dados = $db->carregar($sql);
		if( is_array($dados) ){
			foreach( $dados as $dado ){
	?>
		<tr>
			<td><center><?=$dado['empenho']?></center></td>
			<td><center><?=$dado['percent_empenhado']?> %</center></td>
			<td><center><?=number_format($dado['valor_empenho'],2,',','.')?></center></td>
			<td><center><?=$dado['situacao_empenho']?></center></td>
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
	<br>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center;font-size:12px;" colspan="4"><b>Dados de Pagamentos</b></td>
		</tr>
	</table>
	<br>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>Parcela</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Pagamento</b></td>
		</tr>
	<?php
		$sql = "SELECT DISTINCT
					pg.pagparcela as parcela,
					((SUM(pg.pagvalorparcela) * 100) / sum(pte.pedvalor)  )::NUMERIC(20,0) as percent_pago,
					sum(pte.pedvalor) as valor_pta,
					SUM(pg.pagvalorparcela) as valor_parcela,
					pg.pagsituacaopagamento as situacao_pagamento,
					pt.ptrcod,
					pt.ptrexercicio
				FROM 
					par.empenhosubacao                      e
				INNER JOIN par.subacao                      s   ON s.sbaid 		= e.sbaid and eobstatus = 'A' 
				INNER JOIN par.subacaodetalhe               sd  ON sd.sbaid 	= s.sbaid AND e.eobano = sd.sbdano
				INNER JOIN par.subacaoemendapta          	se  ON se.sbdid 	= sd.sbdid and se.sepstatus = 'A' 
				INNER JOIN par.acao                         a   ON a.aciid 		= s.aciid 
				INNER JOIN par.pontuacao                    p   ON p.ptoid 		= a.ptoid
				INNER JOIN par.instrumentounidade        	iu  ON iu.inuid 	= p.inuid
				INNER JOIN territorios.municipio            m   ON m.muncod 	= iu.muncod
				INNER JOIN emenda.planotrabalho           	pt  ON pt.ptrid 	= se.ptrid
				INNER JOIN emenda.ptemendadetalheentidade 	pte ON pte.ptrid 	= pt.ptrid
				INNER JOIN par.empenho                      em  ON e.empid 		= em.empid 
				INNER JOIN par.pagamento                	pg  ON pg.empid 	= em.empid and empstatus = 'A' AND pg.pagstatus = 'A'
				WHERE
					em.empsituacao <> 'CANCELADO'
					AND pt.ptrid = $ptrid
				GROUP BY
					pg.pagparcela,
					pg.pagsituacaopagamento,
					pt.ptrcod,
					pt.ptrexercicio";
		
		$dados = $db->carregar($sql);
		if( is_array($dados) ){
			foreach( $dados as $dado ){
	?>
		<tr>
			<td><center><?=$dado['parcela']?></center></td>
			<td><center><?=$dado['percent_pago']?> %</center></td>
			<td><center><?=number_format($dado['valor_parcela'],2,',','.')?></center></td>
			<td><center><?=$dado['situacao_pagamento']?></center></td>
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
</body>
</html>
