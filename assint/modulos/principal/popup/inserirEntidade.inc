<?php
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] == 'salvarRegistro') {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();

    echo '<script type="text/javascript">
	        window.opener.document.getElementsByName("cpf")[0].value = \''.$_REQUEST['entnumcpfcnpj'].'\';
	        window.opener.document.getElementsByName("bennome")[0].value = \''.$_REQUEST['entnome'].'\';
	        window.opener.document.getElementsByName("bencidade")[0].value = \''.$_REQUEST['mundescricao2'].'\';
	        window.opener.document.getElementById("entid").value       = \''.$entidade->getEntid().'\';
	        window.close();
	      </script>';
    exit;
}
?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
<?php
$entidade = new Entidades();
//if($_REQUEST['entid'])
//	$entidade->carregarPorEntid($_REQUEST['entid']);
//	$entidade->carregarPorEntid(387884);
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);

//echo $html;
//
//$entidade->setBuscarEntidadePorNome('juridica');

echo $entidade->formEntidade("assint.php?modulo=principal/popup/inserirEntidade&acao=A&opt=salvarRegistro",
							 array("funid" => FUNC_BENINTERNACIONAL, "entidassociado" => null),
							 array("enderecos"=>array(2))							
							);

//echo $javascript;

?>
</div>
<script>
document.getElementById('frmEntidade').onsubmit  = function(e) {
	if (document.getElementById('entnumcpfcnpj').value == '') {
		alert('O campo "CPF" é obrigatório!');
		document.getElementById('entnumcpfcnpj').select();
		document.getElementById('entnumcpfcnpj').focus();
		return false;
	}
	return true;
}
//document.getElementById('tr_entnumcpfcnpj').style.display = 'none';
</script>						
</body>
</html>
