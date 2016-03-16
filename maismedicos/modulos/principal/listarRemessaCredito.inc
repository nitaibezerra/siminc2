<?php
if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

if($_GET['arquivoremessacredito'] && $_GET['rmcid'])
{
	$n = new RemessaCabecalho($_GET['rmcid']);
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
<input type="hidden" id="classe" name="classe" value="RemessaCabecalho" />
<?php 
	$sql = "select
				cab.rmcid,
				to_char(cab.dt_hora,'DD/MM/YYYY HH:MM:SS') as dt_hora,
		
				det1.total as qtde_tutores,
				det2.total as qtde_supervisores,
		
				cab.arqid,
				sum(vl_credito) as vl_reg_detalhe,
				fpgdsc
			from
				maismedicos.remessacabecalho cab
			inner join
				maismedicos.remessadetalhe det ON det.rmcid = cab.rmcid
			inner join
				maismedicos.remessarodape rod ON rod.rmcid = cab.rmcid
			inner join
	   			maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
		
			left join (select
						count(*) as total,
						cab1.rmcid
					from maismedicos.tutor tut
					inner join maismedicos.universidade uni1 ON uni1.uniid = tut.uniid
					inner join maismedicos.remessadetalhe det1 ON det1.tutid = tut.tutid
					inner join maismedicos.remessacabecalho cab1 ON cab1.rmcid = det1.rmcid
					inner join maismedicos.folhapagamento fpg1 ON fpg1.fpgid = cab1.fpgid	   			
					where tut.tuttipo = 'T'
					group by cab1.rmcid) as det1 on det1.rmcid = det.rmcid 

			left join (select
						count(*) as total,
						cab2.rmcid
					from maismedicos.tutor sup
					inner join maismedicos.universidade uni2 ON uni2.uniid = sup.uniid
					inner join maismedicos.remessadetalhe det2 ON det2.tutid = sup.tutid
					inner join maismedicos.remessacabecalho cab2 ON cab2.rmcid = det2.rmcid
					inner join maismedicos.folhapagamento fpg2 ON fpg2.fpgid = cab2.fpgid	   			
					where sup.tuttipo = 'S'
					group by cab2.rmcid) as det2 on det2.rmcid = det.rmcid
		
			where
				cab.cs_tipo_lote = '".TPLID_CREDITO."'
			and
				fpgstatus = 'A'
			group by
				cab.rmcid,cab.dt_hora,cab.arqid,rod.vl_reg_detalhe,fpgdsc,det1.total,det2.total
			order by
				cab.dt_hora;";
	$arrRemessas = $db->carregar($sql);
	
?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%"  >
	<?php if($arrRemessas): ?>
		<tr bgcolor="#c9c9c9"  >
			<td class="bold center" >Ação</td>
			<td class="bold center" >Qtde. Tutores</td>
			<td class="bold center" >Qtde. Supervisores</td>
			<td class="bold center" >Valor Total (R$)</td>
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
				<td><?php $total_tut+=$t['qtde_tutores']; echo $t['qtde_tutores']; ?></td>
				<td><?php $total_sup+=$t['qtde_supervisores']; echo $t['qtde_supervisores']; ?></td>
				<td style="text-align:right;color:blue" >
					<?php $valor = substr($t['vl_reg_detalhe'],0,strlen($t['vl_reg_detalhe'])-2).".".substr($t['vl_reg_detalhe'],strlen($t['vl_reg_detalhe'])-2,strlen($t['vl_reg_detalhe']));?>
					<?php $total_geral+=$valor; echo number_format($valor,2,',','.'); ?>
				</td>
				<td><?php echo $t['fpgdsc']; ?></td>
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
			<td class="bold" ><?php echo number_format($total_tut,0,',','.') ?></td>
			<td class="bold" ><?php echo number_format($total_sup,0,',','.') ?></td>
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
	window.location.href="maismedicos.php?modulo=principal/listarRemessaCredito&acao=A&arquivoremessacredito=1&rmcid="+rmcid;
}

function downloadArquivoRetornoCredito(arqid)
{
	window.location.href="maismedicos.php?modulo=principal/listarRemessaCredito&acao=A&arquivoretorno="+arqid;
}

function detalharRemessaCredito(rmcid)
{
	window.location.href="maismedicos.php?modulo=principal/detalharRemessaCredito&acao=A&rmcid="+rmcid;
}


$(function() {
	<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
	<?php endif; ?>
});
</script>