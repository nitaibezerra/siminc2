<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	echo "<script>			
			window.close();									
		  </script>";
	exit;
}

/*
 * Constante definindo a função (entidade.funcao) prefeito
 */
define("FUNPREFEITO", 2);

/*
 * Validando se existe a entidade prefeito e a entidae prefeitura
 */
if(!$_REQUEST['entid']) {
	echo "<script>
			alert('Nenhum prefeito foi selecionado');
			window.close();
		 </script>";
	exit;
}

if(!$_REQUEST['entidprefeitura']) {
	echo "<script>
			alert('Nenhuma prefeitura esta vinculadas');
			window.close();
		 </script>";
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
    <script type="text/javascript" src="../includes/entidadesn.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;", onunload="window.opener.document.formulario.submit();">
<div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>
<div id="container">
<?php
$entidade = new Entidades();
$entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("cte.php?modulo=prefeito/prefeito&acao=A&opt=salvarRegistro",
							 array("funid" => FUNPREFEITO, "entidassociado" => $_REQUEST['entidprefeitura']),
							 array("enderecos"=>array(1))
							 );
?>
</div>
<script>
document.getElementById('frmEntidade').onsubmit  = function(e) {
	if (document.getElementById('entnumcpfcnpj').value == '') {
		alert('O CPF da entidade é obrigatório.');
		return false;
	}
	if (document.getElementById('entnome').value == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	return true;
}
$('loader-container').hide();
</script>
</body>
</html>
