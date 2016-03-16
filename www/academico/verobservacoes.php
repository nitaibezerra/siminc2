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

$pos = strpos($_REQUEST['id'] ,"_");

$itmid = substr($_REQUEST['id'], 0, $pos); 
if( ! $itmid ) $itmid =$_REQUEST['id'];
else{
	$ano = substr($_REQUEST['id'], $pos + 1, 4); 
}
$sql_nome = "select itmdsc from academico.item where itmid=".$itmid;
$nome = $db->pegaUm($sql_nome);



$sql_campus = "select ent.entnome from academico.campus cam 
				inner join entidade.entidade ent on cam.entid = ent.entid
				where cmpid=".$_SESSION['sig_var']['cmpid'];
$campus = $db->pegaUm($sql_campus);

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form id="formulario" name="formulario" onsubmit="return false;">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" >
	<tr>
	<td class="SubTituloDireita" width="100">Nome do Campus:</td>
	<td><?=$campus ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Nome do Item:</td>
	<td><?=$nome ?></td>
	</tr>
	<?
		if(! $itmid ){
			echo("<tr>
				<td class='SubTituloDireita'>Ano do Item:</td>
				<td>".$ano."></td>
				</tr>");
		}
	?>
	<tr>
	<td class="SubTituloDireita">Observações:</td>
	<td>
	<?
	$permissoes = verificaPerfilAcademico();
	
	echo campo_textarea( 'cpiobs', 'N', (!$permissoes['gravar']?'N':'S'), '', '70', '4', '500');
	
	?>
	</td>
	</tr>
	<?
	if($permissoes['gravar']) {
	?>
	<tr bgcolor="#C0C0C0">
	<td/>
	<td align="left"><input type="button" name="cpiobsbutton" value="Gravar" onclick="processagravacao();"></td>
	</tr>
	<?
	}
	?>
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