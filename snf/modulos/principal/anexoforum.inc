<?php

function excluirArquivoForum($dados) {
	global $db;
	
	$sql = "UPDATE snf.anexo SET anestatus='I' WHERE aneid='".$dados['aneid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Arquivo excluído com sucesso');
			window.location='snf.php?modulo=principal/anexoforum&acao=A';
		  </script>";
	
}

function inserirAnexoForum($dados) {
	global $db;
	
	if($_FILES['arquivo_forum']['size']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("anedatahora" => "now()","anestatus" => "'A'","anetipo" => "4", "estuf" => "'".$_SESSION['snf']['estuf']."'");
		$file   = new FilesSimec("anexo", $campos, "snf");
		$file->setUpload($dados['arquivo_descricao'],"arquivo_forum");
	}
	
	echo "<script>
			alert('Arquivo gravado com sucesso');
			window.location='snf.php?modulo=principal/anexoforum&acao=A';
		  </script>";
	
}


if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br/>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function setarInuId(estuf) {
	window.location='snf.php?modulo=principal/diagnosticoPar&acao=A&requisicao=setarInuIdPorEstuf&estuf='+estuf+'&goto=anexoforum';
}

function excluirArquivoForum(aneid) {
	var conf = confirm('Deseja realmente excluir o anexo?');
	
	if(conf) {
		window.location='snf.php?modulo=principal/anexoforum&acao=A&requisicao=excluirArquivoForum&aneid='+aneid;
	}
}

function downloadArquivo(arqid) {
	window.location='snf.php?modulo=principal/anexoforum&acao=A&requisicao=downloadArquivo&arqid='+arqid;
}
</script>
<?

$perfis = pegaPerfilGeral();

if((in_array(PERFIL_FORUM_APROVACAO,$perfis) || in_array(PERFIL_FORUM_DISTRIBUICAO,$perfis))) {
	
	$sql = "SELECT DISTINCT estuf FROM snf.usuarioresponsabilidade WHERE pflcod IN('".PERFIL_FORUM_APROVACAO."','".PERFIL_FORUM_DISTRIBUICAO."') AND usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$estuf = $db->pegaUm($sql);
	
	if($estuf) setarInuIdPorEstuf(array("estuf" => $estuf));
	
}

/* montando o menu */
echo montarAbasArray(montaMenuAtendimentoForum(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
			<h1 class="notprint">
				Informações do
				Estado
				:
				<?=$db->pegaUm("select estdescricao||' - '||estuf from territorios.estado where estuf='".$_SESSION['snf']['estuf']."'"); ?>
			</h1>
			
			<? if($db->testa_superuser()) : ?>
			<table cellspacing="0" cellpadding="3" border="0"
				style="width: 80px;">
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
					<?
					$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estdescricao"; 
					$db->monta_combo('estuf', $sql, 'S', 'Selecione o Estado', 'setarInuId', '', '', '200', 'N', 'estuf', '', $_SESSION['snf']['estuf']); 
					?>
					</td>
				</tr>
			</table>
			<? endif; ?>
		</th>
	</thead>

<? if($_SESSION['snf']['estuf']) : ?>
	<tr>
		<td>
		<form name="formulario_forum" id="formulario_forum"  method="post" enctype="multipart/form-data" >
		<input type="hidden" name="requisicao" value="inserirAnexoForum">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita">Arquivo:</td>
			<td><input type="file" name="arquivo_forum"/></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Descrição:</td>
			<td><? echo campo_textarea( 'arquivo_descricao', 'S', 'S', '', '70', '4', '200'); ?></td>
		</tr>
		<tr>
			<td colspan="2" align="center"><input type="submit" value="Gravar"></td>
		</tr>
		</table>
		</form>		
		<?
		$sql = "SELECT '<center><img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=\"excluirArquivoForum(\''||an.aneid||'\');\"> <img src=../imagens/anexo.gif align=absmiddle style=cursor:pointer; onclick=\"downloadArquivo(\''||ar.arqid||'\')\"></center>' as acao,
					   ar.arqnome||'.'||ar.arqextensao as arquivo,
					   to_char(anedatahora,'dd/mm/YYYY HH24:MI') as data,
					   us.usunome
				FROM snf.anexo an
				INNER JOIN public.arquivo ar ON ar.arqid = an.arqid  
				INNER JOIN seguranca.usuario us ON us.usucpf = ar.usucpf
				WHERE an.estuf='".$_SESSION['snf']['estuf']."' AND anestatus='A'";
		
		$cabecalho = array("&nbsp;","Arquivo","Inserido em","Inserido por");
		$db->monta_lista_simples($sql,$cabecalho,500,5,'N','95%',$par2);
		?>
		</td>
	</tr>
<? else : ?>
	<tr>
		<td>
		Nenhuma UF foi identificada.		
		</td>
	</tr>
<? endif; ?>
</table>

