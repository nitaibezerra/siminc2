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

include "funcoes_evento_arquivo.php";

if(!$_REQUEST['submodulo']) $_REQUEST['submodulo']='contratos';

$arrMenu = carregarMenuEvento();
echo montarAbasArray($arrMenu, '/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo='.$_REQUEST['submodulo']);

?>
<script type="text/javascript">

function limpaUpload(arqid)
{
	document.getElementById('arquivo_' + arqid).value = "";
}

function uploadArquivos(tbl)
{
	document.getElementById('btn_salvar_' + tbl).value="Carregando...";
	document.getElementById('btn_salvar_' + tbl).disabled = "true";
	document.getElementById('form_arquivo_' + tbl).submit();
}

</script>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
</style>

<?php $arrTbl = dadosTabelasArquivosSubmodulo($_REQUEST['submodulo']); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if(count($arrTbl) <= 0 || $arrMenu == array()): ?>	
		<tr>
			<td class="SubtituloTabela center">Não existe(m) arquivo(s) corrompido(s) para substituir.</td>
		</tr>
	<?php else: ?>
		<?php foreach($arrTbl as $tbl):?>
			<tr>
				<td class="SubtituloTabela center bold"><?php echo $tbl['descricao'] ?></td>
			</tr>
			<tr>
				<td style="background-color:#FFFFFF" ><?php $db->monta_lista($tbl['sql'],$tbl['cabecalho'],10,5,'N','center','','form_arquivo_'.$tbl['nome'] ); ?></td>
			</tr>
			<tr>
				<td class="center" ><input type="button" id="btn_salvar_<?php echo $tbl['nome'] ?>" name="btn_salvar_<?php echo $tbl['nome'] ?>" value="Salvar" onclick="uploadArquivos('<?php echo $tbl['nome'] ?>')" /></td>
			</tr>
		<?php endforeach; ?>
	<?php endif; ?>
</table>