<?

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

echo montarAbasArray((($_REQUEST['acao']=="A")?carregarMenuAnaliseSolicitacao():carregarMenuDetalhamentoSolicitacao()), $_SERVER['REQUEST_URI']);

$titulo_modulo = "Anexos da solicitação";
monta_titulo($titulo_modulo, '&nbsp;');


//permissao para edição da tela
 $habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<script language="javascript" type="text/javascript">

function submeterAnexoSolicitacaoServico() {

	if(document.getElementById('arquivo').value.length == 0) {
		alert('Selecione um arquivo');
		return false;
	}


	if(document.getElementById('tasid').value == '') {
		alert('Selecione o tipo de arquivo');
		return false;
	}

	if(document.getElementById('ansdsc_').value.length == 0) {
		alert('Preencha a descrição');
		return false;
	}

	divCarregando();
	document.getElementById('formulario').submit();
}

</script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirAnexoSolicitacao">
<input type="hidden" name="redirecionamento" value="fabrica.php?modulo=principal/analiseDemandaAnexos&acao=<? echo $_REQUEST['acao'] ?>">
<input type="hidden" name="scsid" value="<? echo $_SESSION['fabrica_var']['scsid']; ?>">
<?

telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="100%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Anexar arquivos na solicitação de serviço</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivo" <?php if($habil == 'N') echo 'disabled';?>></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de anexo:</td>
		<td><?
		$sql = "SELECT tasid as codigo,
					   tasdsc as descricao
				FROM
					fabrica.tipoanexosolicitacao
				WHERE
					tasid not in(23)
				AND
					tasstatus='A' ";
		$db->monta_combo('tasid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'tasid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><? echo campo_texto('ansdsc_', 'S', $habil, 'Descrição', 50, 250, '', '', '', '', 0, 'id="ansdsc_"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda" >
		<?php if($habil == 'S'){?>
		<input type="button" name="inseriranalise" value="Enviar" onclick="submeterAnexoSolicitacaoServico();">
		<?php }?>
		</td>
	</tr>

	</table>
	<?
	$btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"Excluir(\'fabrica.php?modulo=principal/analiseDemandaAnexos&acao=A&requisicao=removerAnexoSolicitacao&ansid='||an.ansid||'\',\'Deseja realmente excluir o anexo?\');\">";
	if($habil == 'N') $btnExcluir = "";

	$sql = "SELECT '<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&requisicao=downloadAnexoSolicitacaoServico&arqid='|| ar.arqid ||'\';\"> $btnExcluir</center>' as acoes, to_char(an.ansdtinclusao,'dd/mm/YYYY'), tp.tasdsc,
					'<a href=onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&requisicao=downloadAnexoSolicitacaoServico&arqid='|| ar.arqid ||'\';\">'||ar.arqnome||'.'||ar.arqextensao||'</a>' as nomearquivo,
					ar.arqtamanho,
					an.ansdsc,
					us.usunome
			FROM fabrica.anexosolicitacao an
			LEFT JOIN fabrica.tipoanexosolicitacao tp ON an.tasid=tp.tasid
			LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
			LEFT JOIN seguranca.usuario us ON us.usucpf=ar.usucpf
			WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' AND ansstatus='A'";
	//dbg($sql,1);
	$cabecalho = array("Ações", "Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição", "Responsável");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
</table>

</form>
