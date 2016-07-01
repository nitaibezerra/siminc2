<?

/* Executando função (_funcoes.php) */
if($_REQUEST['exec_function']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['exec_function']($_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";

if($_REQUEST['entid'] && $_REQUEST['orgid'] && $_REQUEST['iscampus']) {
	$_SESSION['sig_var']['entid'] = $_REQUEST['entid'];
	$_SESSION['sig_var']['orgid'] = $_REQUEST['orgid'];
	$_SESSION['sig_var']['iscampus'] = $_REQUEST['iscampus'];
}

// Verificando a segurança
$permissoes = verificaPerfilSig();
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['sig_var']['orgid']);
// Fim segurança

echo montarAbasArray(carregardadosmenusig(), "/sig/sig.php?modulo=principal/editarentidade&acao=A&orgid=".$_SESSION['sig_var']['orgid']."&entid=".$_SESSION['sig_var']['entid']);
?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/sig.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script>
function controleagrupamento(cnt) {
	if(cnt.checked) {
		switch(cnt.value) {
			case 'porunidade':
				ajaxatualizar('exec_function=carregarItensEntidadeVisualizar','itensEntidadeVisualizar');
				aplicarmascara();			
				break;
			case 'porcampus':
				ajaxatualizar('exec_function=carregarItensEntidadeVisualizar&porcampus=sim','itensEntidadeVisualizar');
				aplicarmascara();
				break;
		}
	}
}
</script>

<form action="?modulo=principal/editarcampus&acao=A" method="post" name="formulario" id="formulario">
<?
//montando cabeçalho
monta_cabecalho_sig($_SESSION['sig_var']['entid']);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">Agrupamento :</td>
	<td><input type="radio" name="agrup" onclick="controleagrupamento(this);" value="porunidade" checked> Por unidade <input type="radio" name="agrup" value="porcampus" onclick="controleagrupamento(this);"> Por  
	<?
	// Verificando o tipoensino
	$tipolocalidade = definirtipolocalidade($_SESSION['sig_var']['orgid']);
	echo $tipolocalidade['nome'];
	?>
	</td>
</tr>
</table>

<div id="itensEntidadeVisualizar">
</div>

<script>
ajaxatualizar('exec_function=carregarItensEntidadeVisualizar','itensEntidadeVisualizar');
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0"><td colspan="2" align="center">&nbsp;</td></tr>
</table>
</form>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>
<script>
aplicarmascara();
function aplicarmascara() {
	var form = document.getElementById('formulario');
	for (var j=0; j < form.length; j++){
		if(form[j].value != "" && form[j].type == "hidden" && form[j].name.substr(0,14) == "gravacaocampo_") {
			form[j].onkeyup();
			form[j].parentNode.innerHTML = form[j].parentNode.innerHTML+form[j].value;
			form[j].parentNode.style.textAlign = 'right';
		}
	}
}
</script>