<?
// Verificando a segurança
$permissoes = verificaPerfilSig();
// Fim segurança

/* Executando função (_funcoes.php) */
if($_REQUEST['requisicao']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

echo montarAbasArray(carregardadosmenusigsolicitacoes(), $_SERVER['REQUEST_URI']);


?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/sig.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<?

monta_titulo( "Lista de encaminhamentos", "ommomom momo momomo momomomo momo momomomomo momomomo moom");

$sql = "SELECT '<img src=../imagens/alterar.gif border=0 style=cursor:pointer; onclick=window.location=\'sig.php?modulo=principal/atendimento&acao=A&ecaid='|| ens.ecaid ||'\';>' as acoes, 
			   '<center>' || sol.solid || '.' || enc.encid || '</center>',
			   tip.tipdesc,
			   enc.encdesc,
			   to_char(enc.encdataenc, 'dd/mm/YYYY HH24:MI') as encdataenc,
			   to_char(enc.encprazo, 'dd/mm/YYYY HH24:MI') as encprazo,
			   to_char(ate.atddataatend, 'dd/mm/YYYY HH24:MI') as atddataatend 
		FROM sig.encaminhados ens 
		LEFT JOIN sig.atendimento ate ON ate.ecaid = ens.ecaid
		LEFT JOIN sig.encaminhamento enc ON enc.encid = ens.encid 
		LEFT JOIN sig.solicitacao sol ON sol.solid = enc.solid 
		LEFT JOIN sig.tiposolicitacao tip ON tip.tipid = sol.tipid
		WHERE ens.usucpfencaminhado='".$_SESSION['usucpf']."'";

$cabecalho = array("", "Código", "Tipo", "Descrição", "Data de envio", "Prazo resposta", "Data resposta");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);


if($_REQUEST['ecaid']) {
	
?>
<br />
<form action="sig.php?modulo=principal/atendimento&acao=A" enctype="multipart/form-data" method="post" name="formulario" id="formulario">
<?

$sql = "SELECT * FROM sig.encaminhados a
		LEFT JOIN sig.atendimento e ON a.ecaid = e.ecaid 
		LEFT JOIN sig.encaminhamento enc ON enc.encid = a.encid 
		LEFT JOIN sig.solicitacao sol ON sol.solid = enc.solid WHERE a.ecaid = '".$_REQUEST['ecaid']."'";
$atendimento = $db->pegaLinha($sql);

if($atendimento['atdid']) {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"atualizaratendimento\">";
	echo "<input type=\"hidden\" name=\"atdid\" value=\"".$atendimento['atdid']."\">";
	echo "<input type=\"hidden\" name=\"ecaid\" value=\"".$_REQUEST['ecaid']."\">";
} else {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"inseriratendimento\">";
	echo "<input type=\"hidden\" name=\"ecaid\" value=\"".$_REQUEST['ecaid']."\">";
}
	


?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloCentro" colspan="2">Dados do encaminhamento</td>
</tr>
<tr>
<td class="SubTituloDireita">Código:</td>
<td><? echo $atendimento['solid'].'.'.$atendimento['encid']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Atendimento:</td>
<td>
<?
$atdtxtresposta = $atendimento['atdtxtresposta'];
echo campo_textarea( 'atdtxtresposta', 'N', 'S', '', '120', '10', '5000'); 
?>
</td>
</tr>
<tr>
	<td class="SubTituloDireita">Anexo:</td>
<td>
<?

if($atendimento['atdid']) {
	 
	$sql = "SELECT arqid, anxdsc FROM sig.anexo WHERE atdid = '".$atendimento['atdid']."'";
	$anexos = $db->carregar($sql);
	
	if($anexos[0]) {
		echo "<table>";
		foreach($anexos as $key => $ane) {
?>
	<tr>
		<td><a href="sig.php?modulo=principal/encaminhamento&acao=A&requisicao=downloadarquivo&arqid=<? echo $ane['arqid']; ?>"> <? echo $ane['anxdsc']; ?></a></td>
	</tr>
<? 
		}
		echo "</table>";
	}
} else { 
?>

	<table id="anexos">
	<tr>
		<td><input type="file" name="arquivo[]"></td>
		<td colspan="2">Nome do arquivo : <input class="normal" type="text" name="arquivonome[]"></td>
	</tr>
	<tr>
		<td colspan="3"><a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a></td>
	</tr>
	</table>
	
<? 
} 
?>
</td>
</tr>
</table>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
<td colspan="2" align="center">	
<input type='button' onclick="document.getElementById('formulario').submit();this.disabled=true" value='Enviar'>
</td>
</tr>
</table>
</form>
<?
}
?>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>