<?php
include_once(APPRAIZ . 'includes/classes/dateTime.inc');
$obdata 	= new Data();

/************************* DECLARAÇÃO DE VARIAVEIS *********************************/
$prsid 	= $_REQUEST['prsid'];
$hmcid 	= $_REQUEST['hmcid'];

$convenio = "SELECT 	h.hmcdatamonitoramento AS referencia, 
						h.hmcstatus AS status, 
						h.hmcdatareferenciamonitoramento AS feito,
						s.prsnumconvsape AS convenio
			 FROM cte.historicomonitoramentoconvenio h
			 INNER JOIN cte.projetosape s ON s.prsid = h.prsid
			 WHERE h.hmcid = ".$hmcid;
$convenio 	= $db->pegaLinha($convenio);
/*
if($convenio['status'] == "I"){
	echo "<script>
		alert('Este monitoramento não foi finalizado para poder gerar um relatório.');
		window.close();
	</script>";
	die();
}
*/

$subacoes = "	SELECT si.sbaid, si.sbadsc, ss.ssudescricao
				FROM cte.historicomonitoramentoconvenio h
				INNER JOIN cte.historicomonitoramentoconvsubac 	hs ON h.hmcid  = hs.hmcid 
				INNER JOIN cte.subacaoindicador si ON si.sbaid = hs.sbaid
				INNER JOIN cte.statussubacao ss	ON ss.ssuid = hs.ssuid
				WHERE h.hmcid=".$hmcid;
$subacoes 	= $db->carregar($subacoes);

if($subacoes == false){
	echo "<script>
		alert('Não existem subações monitoradas até o momento para que seja possível gerar um relatório.');
		window.close();
	</script>";
	die();
}

$itens = "	SELECT 	hi.cosid, 
					hi.hmsquantidadeempenhada, 
					hi.hmsquantidadeliquidada, 
					hi.hmsquantidadepaga, 
					hi.hmsvalorunitario, 
					hi.hmsvalortotalempenhado, 
					hi.hmsvalortotalliquidado, 
					hi.hmsvalortotalpago, 
					hi.hmsobs, 
					hi.scsid, 
					hi.micid,
					sc.scsdesc,
					m.micdescricao,
					hs.sbaid,
					cs.cosdsc	
			FROM cte.historicomonitoramentoconvenio h
			INNER JOIN cte.historicomonitoramentoconvsubac 	hs ON h.hmcid  = hs.hmcid 
			INNER JOIN cte.historicoconvitemcomposicao 	hi ON hi.hmsid = hs.hmsid 
			INNER JOIN cte.statuscomposicaosubacao sc ON sc.scsid = hi.scsid
			INNER JOIN cte.composicaosubacao cs ON cs.cosid = hi.cosid
			LEFT JOIN cte.motivoitemcomposicao m	ON m.micid = hi.micid
			WHERE h.hmcid=".$hmcid;
$itens 	= $db->carregar($itens);

if($convenio['status'] == 'F'){
	$convenio['status'] = "Finalizado";
}else if($convenio['status'] == 'I') {
	$convenio['status'] = "Iniciado";
}

$mes 		= $obdata->formataData($convenio['referencia'],"mesTextual","yyyy-mm-dd");
$executado = $obdata->formataData($convenio['feito'],"dd/mm/yyyy","yyyy-mm-dd");
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../../includes/funcoes.js"></script>
	<style rel="stylesheet" type="text/css" media="print">
   		#botao{display:none}
	</style>
</head>
<body>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%";>
	<tr>
		<th colspan="2">Relatório de histórico de convênio n° <?=$convenio['convenio']; ?></th>
	</tr>
	<tr>
		<th style="width: 220px;" class="SubTituloDireita">Monitoramento referente ao mês de:</th>
		<td><?=$mes;?></td>
	</tr>
	<tr>
		<th class="SubTituloDireita">Monitoramento criado em:</th>
		<td><?=$executado;?></td>
	</tr>
	<tr>
		<th class="SubTituloDireita">Status do Monitoramento:</th>
		<td><?=$convenio['status'];?></td>
	</tr>
	<?php foreach($subacoes as $dadosSubacoes){ ?>
	<tr>
		<th class="SubTituloDireita">Subação:</th>
		<th class="SubTituloEsquerda"><?=$dadosSubacoes['sbadsc'];?></th>
	</tr>
	<tr>
		<th class="SubTituloDireita">Status:</th>
		<th class="SubTituloEsquerda"><?=$dadosSubacoes['ssudescricao']; ?></th>
	</tr>
	<tr>
		<th class="SubTituloDireita">Itens de composição</th>
		<td>
			<?php  
			if(is_array($itens)){
				foreach($itens as $dadositens){ 
					if($dadositens['sbaid'] == $dadosSubacoes['sbaid']){		
			?>
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%";>
				<tr>
					<th class="SubTituloDireita" style="width: 80px;">Item:</th>
					<th class="SubTituloEsquerda"><?=$dadositens['cosdsc']; ?></th>
				</tr>
				<?php
					if($dadositens['scsdesc']){
				?>
				<tr>
					<th class="SubTituloDireita">Status:</th>
					<td><?=$dadositens['scsdesc']; ?></td>
				</tr>
				<?php } 
					if($dadositens['micdescricao']){ ?>
				<tr>
					<th class="SubTituloDireita">Motivo:</th>
					<td><?=$dadositens['micdescricao']; ?></td>
				</tr>
				<?php } if($dadositens['hmsquantidadeempenhada']){ ?>
				<tr>
					<th class="SubTituloDireita">Qtd. Empenhada:</th>
					<td><?=$dadositens['hmsquantidadeempenhada']; ?></td>
				</tr>
				<tr>
					<th class="SubTituloDireita">Qtd. Liquidada:</th>
					<td><?=$dadositens['hmsquantidadeliquidada']; ?></td>
				</tr>
				<tr>
					<th class="SubTituloDireita">Qtd. Paga:</th>
					<td><?=$dadositens['hmsquantidadepaga']; ?></td>
				</tr>
				<tr>
					<th class="SubTituloDireita">Vlr. Unitário:</th>
					<td><?=$dadositens['hmsvalorunitario']; ?></td>
				</tr>
				<tr>
					<th class="SubTituloDireita">Vlr. total Empenhado:</th>
					<td><?=$dadositens['hmsvalortotalempenhado']; ?></td>
				</tr>
				<tr>
					<th class="SubTituloDireita">Vlr. total Liquidado:</th>
					<td><?=$dadositens['hmsvalortotalliquidado']; ?></td>
				</tr>
				<tr>
					<th class="SubTituloDireita">Vlr. total Pago:</th>
					<td><?=$dadositens['hmsvalortotalpago']; ?></td>
				</tr>
				<?php }
				if($dadositens['hmsobs']){
				?>
				<tr>
					<th class="SubTituloDireita">Obs:</th>
					<td><?=$dadositens['hmsobs']; ?></td>
				</tr>
				<?php } ?>
			</table>
			<?php 	} 
			}
		}else{
		?>
		<tr>
		<th class="SubTituloDireita">Obs:</th>
					<td >Não existem itens de composição monitorados.</td>
				</tr>
		<?php 
		}
		?>
		</td>
	</tr>
	<?php 		
		}	
	?>
	<tr>
		<td colspan="2">
			<center>
				<input type="button" value="imprimir" onClick="javascript:window.print()" id="botao" />
			</center>
		</td>
	</tr>
</table>
</body>
</html>