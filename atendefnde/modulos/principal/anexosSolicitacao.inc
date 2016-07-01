<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$menuAba = array(0 => array("id" => 1, "descricao" => "Lista de Solicitações", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaSolicitacao&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados da Solicitação", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/dadosSolicitacao&acao=A"),
			  2 => array("id" => 3, "descricao" => "Anexos da Solicitação", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/anexosSolicitacao&acao=A")
		  	  );

echo montarAbasArray($menuAba, $_SERVER['REQUEST_URI']);

$titulo_modulo = "Anexos da Solicitação";
monta_titulo($titulo_modulo, '&nbsp;');


//permissao para edição da tela
 $habil = 'S';

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<script language="javascript" type="text/javascript">

function submeterAnexoSolicitacao() {

	if(document.getElementById('arquivo').value.length == 0) {
		alert('Selecione um arquivo');
		return false;
	}


	if(document.getElementById('dsc_').value.length == 0) {
		alert('Preencha a descrição');
		return false;
	}

	divCarregando();
	document.getElementById('formulario').submit();
}

function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
}

</script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirAnexoSolicitacao">
<input type="hidden" name="redirecionamento" value="atendefnde.php?modulo=principal/anexosSolicitacao&acao=A">
<input type="hidden" name="solid" value="<? echo $_SESSION['solid']; ?>">

<?=cabecalhoSolicitacao();?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="100%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Anexar arquivos no solicitação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivo" <?php if($habil == 'N') echo 'disabled';?>></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><? echo campo_texto('dsc_', 'S', $habil, 'Descrição', 50, 250, '', '', '', '', 0, 'id="dsc_"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda" >
		<?php if($habil == 'S'){?>
		<input type="button" name="btn_enviar" value="Enviar" onclick="submeterAnexoSolicitacao();">
		<?php }?>
		</td>
	</tr>

	</table>
	<?
	$btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"Excluir(\'atendefnde.php?modulo=principal/anexosSolicitacao&acao=A&requisicao=removerAnexoSolicitacao&soaid='||a.soaid||'&arqid='||ar.arqid||'\',\'Deseja realmente excluir o anexo?\');\">";
	if($habil == 'N') $btnExcluir = "";

	$sql = "SELECT '<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'atendefnde.php?modulo=principal/anexosSolicitacao&acao=A&requisicao=downloadAnexoSolicitacao&arqid='|| ar.arqid ||'\';\"> $btnExcluir</center>' as acoes, to_char(a.soadatainclusao,'dd/mm/YYYY'), 
					'<a href=\"atendefnde.php?modulo=principal/anexosSolicitacao&acao=A&requisicao=downloadAnexoSolicitacao&arqid='|| ar.arqid ||'\">'||ar.arqnome||'.'||ar.arqextensao||'</a>' as nomearquivo,
					ar.arqtamanho,
					a.soadescricao,
					us.usunome
			FROM atendefnde.solicitacaoanexo a
			LEFT JOIN public.arquivo ar ON ar.arqid=a.arqid
			LEFT JOIN seguranca.usuario us ON us.usucpf=ar.usucpf
			WHERE solid='".$_SESSION['solid']."' AND soastatus='A'";
	//dbg($sql,1);
	$cabecalho = array("Ações", "Data inclusão", "Nome arquivo", "Tamanho(bytes)", "Descrição", "Responsável");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
</table>

</form>
