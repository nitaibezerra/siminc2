<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$menuAba = array(0 => array("id" => 1, "descricao" => "Lista de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaAtendimento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/dadosAtendimento&acao=A"),
			  2 => array("id" => 3, "descricao" => "Anexos de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/anexosAtendimento&acao=A"),
			  3 => array("id" => 4, "descricao" => "Lista de Solicitações", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaSolicitacao&acao=A"),
			  4 => array("id" => 5, "descricao" => "Cadastro de Solicitação", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/cadastraSolicitacao&acao=A")
		  	  );

echo montarAbasArray($menuAba, $_SERVER['REQUEST_URI']);

$titulo_modulo = "Anexos de Atendimento";
monta_titulo($titulo_modulo, '&nbsp;');


//permissao para edição da tela
 $habil = 'S';

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<script language="javascript" type="text/javascript">

function submeterAnexoAtendimento() {

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
<input type="hidden" name="requisicao" value="inserirAnexoAtendimento">
<input type="hidden" name="redirecionamento" value="atendefnde.php?modulo=principal/anexosAtendimento&acao=A">
<input type="hidden" name="ateid" value="<? echo $_SESSION['ateid']; ?>">

<?=cabecalhoAtendimento(); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="100%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Anexar arquivos no atendimento</td>
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
		<input type="button" name="btn_enviar" value="Enviar" onclick="submeterAnexoAtendimento();">
		<?php }?>
		</td>
	</tr>

	</table>
	<?
	$btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"Excluir(\'atendefnde.php?modulo=principal/anexosAtendimento&acao=A&requisicao=removerAnexoAtendimento&ataid='||a.ataid||'&arqid='||ar.arqid||'\',\'Deseja realmente excluir o anexo?\');\">";
	if($habil == 'N') $btnExcluir = "";

	$sql = "SELECT '<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'atendefnde.php?modulo=principal/anexosAtendimento&acao=A&requisicao=downloadAnexoAtendimento&arqid='|| ar.arqid ||'\';\"> $btnExcluir</center>' as acoes, to_char(a.atadatainclusao,'dd/mm/YYYY'), 
					'<a href=\"atendefnde.php?modulo=principal/anexosAtendimento&acao=A&requisicao=downloadAnexoAtendimento&arqid='|| ar.arqid ||'\">'||ar.arqnome||'.'||ar.arqextensao||'</a>' as nomearquivo,
					ar.arqtamanho,
					a.atadescricao,
					us.usunome
			FROM atendefnde.atendimentoanexo a
			LEFT JOIN public.arquivo ar ON ar.arqid=a.arqid
			LEFT JOIN seguranca.usuario us ON us.usucpf=ar.usucpf
			WHERE ateid='".$_SESSION['ateid']."' AND atastatus='A'";
	//dbg($sql,1);
	$cabecalho = array("Ações", "Data inclusão", "Nome arquivo", "Tamanho(bytes)", "Descrição", "Responsável");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
</table>

</form>
