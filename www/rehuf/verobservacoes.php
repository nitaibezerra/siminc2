<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$db = new cls_banco();

$dadosid = explode("_",$_REQUEST['id']);

/*
 * VALIDANDO SE OS PARÂMETROS ESTÃO CORRETOS
 */
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.close();
		  </script>";
	exit;
}

if(!$dadosid[1] || !$dadosid[2]) {
	echo "<script>
			alert('Problemas no carregamento dos dados. Entre em contato com suporte técnico');
			window.close();
		  </script>";
	exit;
}
/*
 * FIM
 * VALIDANDO SE OS PARÂMETROS ESTÃO CORRETOS
 */

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form id="formulario" name="formulario" onsubmit="return false;">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" >
	<tr>
	<td class="SubTituloDireita" width="100">Nome do hospital :</td>
	<td><? echo $db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid = '". $_SESSION['rehuf_var']['entid'] ."'"); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Linha :</td>
	<td><? echo $db->pegaUm("SELECT lindsc FROM rehuf.linha lin WHERE linid='".$dadosid[1]."'"); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Coluna :</td>
	<td><? echo $db->pegaUm("SELECT coldsc FROM rehuf.coluna WHERE colid='".$dadosid[2]."'"); ?></td>
	</tr>
	<?
	if($dadosid[3]) {
		echo "<tr>
				<td class=\"SubTituloDireita\">Período :</td>
				<td>".$db->pegaUm("SELECT perdsc FROM rehuf.periodogrupoitem WHERE perid='".$dadosid[3]."'")."</td>
			  </tr>";
	}
	?>
	<tr>
	<td class="SubTituloDireita">Observações:</td>
	<td>
	<?
	echo campo_textarea( 'cpiobs', 'N', 'S', '', '70', '4', '500');
	?>
	</td>
	</tr>
	<tr bgcolor="#C0C0C0">
	<td/>
	<td align="left"><input type="button" name="cpiobsbutton" value="Gravar" onclick="processagravacao();"></td>
	</tr>
</table>
</form>
<script>
document.getElementById('cpiobs').value = window.opener.document.getElementById('<? echo $_REQUEST['id'] ?>').value;

function processagravacao() {
	window.opener.document.getElementById('<? echo $_REQUEST['id'] ?>').value = document.getElementById('cpiobs').value;
	if(document.getElementById('cpiobs').value != "") {
		window.opener.document.getElementById('img<? echo $_REQUEST['id'] ?>').title = document.getElementById('cpiobs').value;
		window.opener.document.getElementById('img<? echo $_REQUEST['id'] ?>').src='../imagens/edit_on.gif';
	} else {
		window.opener.document.getElementById('img<? echo $_REQUEST['id'] ?>').src='../imagens/edit_off.gif';
		window.opener.document.getElementById('img<? echo $_REQUEST['id'] ?>').title = '';
	}
	window.close();	
}
</script>