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

if($_GET['arquivoremessa'] && $_GET['rmcid'])
{
	$n = new RemessaCabecalho($_GET['rmcid']);
	$n->gerarArquivoRemessaCadastro($_GET['rmcid']);
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
monta_titulo( "Lista de Remessa de Cadastro", "&nbsp;");
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
				count(tut.tutid) as qtde_tutores,
				count(sup.tutid) as qtde_supervisores,
				cab.arqid
			from
				maismedicos.remessacabecalho cab
			inner join
				maismedicos.remessadetalhe det ON det.rmcid = cab.rmcid
			inner join
				maismedicos.remessarodape rod ON rod.rmcid = cab.rmcid
			left join
				maismedicos.tutor tut ON tut.tutid = det.tutid and tut.tuttipo = 'T'
			left join
				maismedicos.tutor sup ON sup.tutid = det.tutid and sup.tuttipo = 'S'
			WHERE
				cab.cs_tipo_lote = '".TPLID_CADASTRO."'
			group by
				cab.rmcid,cab.dt_hora,cab.arqid
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
			<td class="bold center" >Data de Criação</td>
			<td class="bold center" >Arquivo de Remessa</td>
			<td class="bold center" >Arquivo de Retorno</td>
		</tr>
		<?php foreach($arrRemessas as $n => $t): ?>
			<tr bgcolor="<?php echo $n%2 ? "#FFFFFF" : "" ?>" >
				<td class="center" >
					<a href="javascript:detalharRemessaCadastro('<?php echo $t['rmcid']; ?>')" ><img src="../imagens/consultar.gif" class="link" /></a>
				</td>
				<td><?php echo $t['qtde_tutores']; ?></td>
				<td><?php echo $t['qtde_supervisores']; ?></td>
				<td><?php echo $t['dt_hora']; ?></td>
				<td><a href="javascript:downloadArquivoRemessaCadastro('<?php echo $t['rmcid']; ?>')" >Arquivo de Remessa</a></td>
				<td>
					<?php if($t['arqid']): ?>
						<a href="javascript:downloadArquivoRetornoCadastro('<?php echo $t['arqid']; ?>')" >Arquivo de Retorno</a>
					<?php else: ?>
						Não Processado
					<?php endif; ?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else: ?>
		<tr>
			<td colspan="2" align="center" >Não existem remessas.</td>
		</tr>
	<?php endif; ?>
</table>
</form>
<script>
function downloadArquivoRemessaCadastro(rmcid)
{
	window.location.href="maismedicos.php?modulo=principal/listarRemessaCadastro&acao=A&arquivoremessa=1&rmcid="+rmcid;
}

function downloadArquivoRetornoCadastro(arqid)
{
	window.location.href="maismedicos.php?modulo=principal/listarRemessaCadastro&acao=A&arquivoretorno="+arqid;
}

function detalharRemessaCadastro(rmcid)
{
	window.location.href="maismedicos.php?modulo=principal/detalharRemessaCadastro&acao=A&rmcid="+rmcid;
}


$(function() {
	<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
	<?php endif; ?>
});
</script>