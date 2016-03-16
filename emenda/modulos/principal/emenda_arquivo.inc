<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

if($_FILES['arquivo']) {
	
	include APPRAIZ ."includes/funcoes_public_arquivo.php";
	
	$arrValidacao = array('extensao');
	
	$resp = atualizarPublicArquivo($arrValidacao);
	
	if($resp['TRUE']) $msg .= 'Foram processados '.count($resp['TRUE']).' arquivos.'.'\n';
	if($resp['FALSE']) {
		$msg .= 'Problemas encontrados:'.'\n';
		foreach($resp['FALSE'] as $k => $v) {
			$msg .= 'ARQID : '.$k.' | '.$v.'\n';
		}
	}
	
	die("<script>
			alert('".$msg."');
			window.location = window.location;
		 </script>");
}


include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

monta_titulo( "Recuperação dos arquivos anexos do PTA", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
	
$perfis = pegaPerfilArray( $_SESSION['usucpf'] );
?>
<script>
	function pesquisar()
	{
		document.getElementById('formulario_pesquisa').submit();
	}
	
	function limpar()
	{
		window.location.href = window.location;
	}
</script>
<form name="formulario_pesquisa" id="formulario_pesquisa" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td colspan="2" class="subTituloCentro">Argumentos de Pesquisa</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita">Nome de Usuário:</td>
			<td>
				<?php echo campo_texto("usunome","S","S",'',80,200,"","",'',"",'',"","",$_REQUEST['usunome']) ?>
			</td>
		</tr>
		<tr>
			<td></td>
			<td>
				<input type="button" name="btn_buscar" value="Pesquisar" onclick="pesquisar()" />
				<input type="button" name="btn_buscar" value="Limpar" onclick="limpar()" />
			</td>
		</tr>
	</table>	
</form>
<?php

if($_REQUEST['usunome']){
	$arrWhere[] = "UPPER(removeacento(u.usunome)) like removeacento(('%".strtoupper($_REQUEST['usunome'])."%'))";	
}

$cabecalho = array("CPF", "Nome", "PTA", "Entidade", "Anexos Descrição", "ID arquivo", "Nome do arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
$clausula = "a.usucpf, u.usunome,";
	
$sql = "select distinct a.usucpf, u.usunome, pt.ptrcod||'&#92;'||pt.ptrexercicio as pta, eb.enbnome, de.anxdsc,
		a.arqid, a.arqnome||'.'||a.arqextensao, a.arqtamanho, to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
		'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload 
		from emenda.anexo de
		inner join emenda.planotrabalho pt on pt.ptrid = de.ptrid
		inner join emenda.entidadebeneficiada eb on eb.enbid = pt.enbid
		inner join public.arquivo a on a.arqid=de.arqid 
		-- inner join emenda.emendadetalheentidade ee on ee.enbid = eb.enbid
		inner join emenda.usuarioresponsabilidade ur on ur.enbid = eb.enbid
		inner join seguranca.usuario u ON u.usucpf = ur.usucpf 
		where a.arqid/1000 between 647 and 725 and 
		a.arqid not in(select arqid from public.arquivo_recuperado) and a.sisid=57
		".((!$db->testa_superuser() && !in_array(ADMINISTRADOR_INST, $perfis) )?"and a.usucpf='".$_SESSION['usucpf']."'":"")."
		".($arrWhere? " and ".implode(" and ",$arrWhere) : "")." 
		order by u.usunome";

$db->monta_lista($sql,$cabecalho,10,5,'N','center',$par2,'form_arquivo');
?>
<script type="text/javascript">

function limpaUpload(arqid)
{
	document.getElementById('arquivo_' + arqid).value = "";
}

function uploadArquivos()
{
	document.getElementById('btn_salvar').value="Carregando...";
	document.getElementById('btn_salvar').disabled = "true";
	document.getElementById('form_arquivo').submit();
}

</script>
<?

if($_REQUEST['tabela']) $_REQUEST['tabela']();

?>

<table cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" class="tabela">
	<tr>
		<td style="text-align:center;"><input type="button" name="btn_salvar" id="btn_salvar" value="Salvar" onclick="uploadArquivos()"  /></td>
	</tr>
</table>