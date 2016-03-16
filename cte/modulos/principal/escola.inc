<?
cte_verificaSessao();
require_once APPRAIZ . "includes/classes/entidades.class.inc";
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
cte_montaTitulo( $titulo_modulo, '' );
$inuid = (integer) $_SESSION["inuid"];
$entid = (integer) $_REQUEST["entid"];

$entidade = new Entidades();
$entidade->carregarPorEntid($entid);
echo $entidade->formEntidade("rehuf.php?modulo=principal/dadoshospital&acao=A&requisicao=salvarRegistroEntidade",
							 array("funid" => '3', "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
?>
<p align="center"><input type="button" name="Voltar" value="Voltar" align="right" onclick="window.location.href = '?modulo=principal/escolas&acao=A';"	/></p>
<script type="text/javascript">
document.getElementById('tr_acoes').style.display = 'none';
var form = document.getElementById('frmEntidade');
for(var i=0; i < form.elements.length; i++) {
	form.elements[i].disabled = true;
	form.elements[i].readOnly = true;
	form.elements[i].className = 'disabled';
	form.elements[i].onfocus = "";
	form.elements[i].onmouseout = "";
	form.elements[i].onblur = "";
	form.elements[i].onkeyup = "";
}
document.getElementById( 'aguarde' ).style.visibility = 'hidden';
document.getElementById('aguarde').style.display = 'none';
</script>