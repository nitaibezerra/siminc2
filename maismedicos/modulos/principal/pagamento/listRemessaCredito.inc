<?php

include_once APPRAIZ . 'maismedicos/classes/Pagamento_RemessaCabecalho.class.inc';

if(($_REQUEST['requisicao'] || $_REQUEST['requisicaoAjax']) && $_REQUEST['classe']){
	
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

if($_GET['arquivoremessacredito'] && $_GET['rmcid']){
	$n = new Pagamento_RemessaCabecalho($_GET['rmcid']);
	$n->gerarArquivoRemessaCredito($_GET['rmcid']);
	exit;
}

if($_GET['arquivoretorno'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$sql = "select
				sis.sisdiretorio
			from
				public.arquivo arq
			inner join
				seguranca.sistema sis ON sis.sisid = arq.sisid
			where
				arqid = {$_GET['arquivoretorno']}";
	$schema = $db->pegaUm($sql);
	$file = new FilesSimec(null,null,$schema);
	$file-> getDownloadArquivo($_GET['arquivoretorno']);
	exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Lista de Remessa de Crédito", "&nbsp;");
?>
<style>
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.center{text-align:center}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" >
<input type="hidden" id="requisicao" name="requisicao" value="" />
<input type="hidden" id="classe" name="classe" value="Pagamento_RemessaCabecalho" />
<?php 
	$sql = "select
				cab.rmcid,
				to_char(cab.dt_hora,'DD/MM/YYYY HH:MM:SS') as dt_hora,
				det1.total as qtde_beneficiarios,
				cab.arqid,
				sum(vl_credito) as vl_reg_detalhe
			from
				maismedicos.pagamento_remessacabecalho cab
			inner join
				maismedicos.pagamento_remessadetalhe det ON det.rmcid = cab.rmcid
			inner join
				maismedicos.pagamento_remessarodape rod ON rod.rmcid = cab.rmcid			
		
			left join (select
						count(*) as total,
						cab1.rmcid,
						ben.funid
					from maismedicos.pagamento_beneficiario ben
					left join maismedicos.pagamento_beneficiariofuncao fun on fun.funid = ben.funid
					--inner join maismedicos.universidade uni1 ON uni1.uniid = ben.uniid
					inner join maismedicos.pagamento_remessadetalhe det1 ON det1.benid = ben.benid
					inner join maismedicos.pagamento_remessacabecalho cab1 ON cab1.rmcid = det1.rmcid						   			
					group by cab1.rmcid,ben.funid) as det1 on det1.rmcid = det.rmcid 
			where
				cab.cs_tipo_lote = '".TPLID_CREDITO."'
			group by
				cab.rmcid,cab.dt_hora,cab.arqid,rod.vl_reg_detalhe,det1.total
			order by
				cab.dt_hora;";
	$arrRemessas = $db->carregar($sql);
	
?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%"  >
	<?php if($arrRemessas): ?>
		<tr bgcolor="#c9c9c9"  >
			<td class="bold center" >Ação</td>
			<td class="bold center" >Qtde. Beneficiários</td>
			<td class="bold center" >Descrição</td>
			<td class="bold center" >Data de Criação</td>
			<td class="bold center" >Arquivo de Remessa</td>
			<td class="bold center" >Arquivo de Retorno</td>
		</tr>
		<?php foreach($arrRemessas as $n => $t): ?>
			<tr bgcolor="<?php echo $n%2 ? "#FFFFFF" : "" ?>" >
				<td class="center" >
					<a href="javascript:detalharRemessaCredito('<?php echo $t['rmcid']; ?>')" ><img src="../imagens/consultar.gif" class="link" /></a>
				</td>
				<td><?php $totBeneficiarios+=$t['qtde_beneficiarios']; echo $t['qtde_beneficiarios']; ?></td>
				<td style="text-align:right;color:blue" >
					<?php $valor = substr($t['vl_reg_detalhe'],0,strlen($t['vl_reg_detalhe'])-2).".".substr($t['vl_reg_detalhe'],strlen($t['vl_reg_detalhe'])-2,strlen($t['vl_reg_detalhe']));?>
					<?php $total_geral+=$valor; echo number_format($valor,2,',','.'); ?>
				</td>
				<td><?php echo $t['dt_hora']; ?></td>
				<td><a href="javascript:downloadArquivoRemessaCredito('<?php echo $t['rmcid']; ?>')" >Arquivo de Remessa</a></td>
				<td>
					<?php if($t['arqid']): ?>
						<a href="javascript:downloadArquivoRetornoCredito('<?php echo $t['arqid']; ?>')" >Arquivo de Retorno</a>
					<?php else: ?>
						Não Processado
					<?php endif; ?>
				</td>
			</tr>
		<?php endforeach; ?>
		<tr bgcolor="#c9c9c9" >
			<td class="bold center" >Total</td>
			<td class="bold" ><?php echo number_format($totBeneficiarios,0,',','.') ?></td>
			<td class="bold" style="text-align:right;color:blue" ><?php echo number_format($total_geral,2,',','.') ?></td>
			<td class="bold center" ></td>
			<td class="bold center" ></td>
			<td class="bold center" ></td>
			<td class="bold center" ></td>
		</tr>
	<?php else: ?>
		<tr>
			<td colspan="2" align="center" >Não existem remessas de crédito.</td>
		</tr>
	<?php endif; ?>
</table>
</form>
<script>
function downloadArquivoRemessaCredito(rmcid)
{
	window.location.href="maismedicos.php?modulo=principal/pagamento/listRemessaCredito&acao=A&arquivoremessacredito=1&rmcid="+rmcid;
}

function downloadArquivoRetornoCredito(arqid)
{
	window.location.href="maismedicos.php?modulo=principal/pagamento/listRemessaCredito&acao=A&arquivoretorno="+arqid;
}

function detalharRemessaCredito(rmcid)
{
	window.location.href="maismedicos.php?modulo=principal/pagamento/detalharRemessaCredito&acao=A&rmcid="+rmcid;
}


$(function() {
	<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
	<?php endif; ?>
});
</script>