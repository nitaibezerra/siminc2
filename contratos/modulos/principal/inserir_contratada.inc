<?php
// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

if($_REQUEST['requisicaoAjax'])
{
	$_REQUEST['requisicaoAjax']();
	die;
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";
define("ID_FISCALCONTRATO",62);


if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	$entid = $entidade->getEntId();
	echo '<html><head><title>Contratada</title></head><body>';
	echo '<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>';
	// Verifica se é atualização de responsáveis
	if($_REQUEST["campo"]=='1'){
		salvarFiscalContrato($entid);
	    echo '<script type="text/javascript">
			      $.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=listarFiscalContrato",
					   success: function(msg) {
						   window.opener.document.getElementById("td_lista_fiscal").innerHTML = msg;
	    					window.close();
					   }
					 });
			  </script>';
				
	} elseif($_REQUEST["campo"]=='2'){
		salvarGestorContrato($entid);
	    echo '<script type="text/javascript">
			       $.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=listarGestorContrato",
					   success: function(msg){
						   window.opener.document.getElementById("td_lista_gestor").innerHTML = msg;
	    					window.close();
					   }
					 });
			  </script>';
			}
	echo '</body></html>';
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
  <input type="hidden" name="campo" value="<?php echo $_REQUEST['campo']; ?>">
    <div>
<?php
$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
//	echo $entidade->formEntidade("evento.php?modulo=principal/inserir_fiscal&acao=A&opt=salvarRegistro&tr=".$_REQUEST['tr'],	
	echo $entidade->formEntidade("contratos.php?modulo=principal/inserir_fiscal&acao=A&opt=salvarRegistro&campo=".$_REQUEST['campo']."&entidselecionado=".$_GET['entid'],
								 array("funid" => ID_FISCALCONTRATO, "entidassociado" => null),
								 array("enderecos"=>array(1))
								 );
?>
    </div>
    <script type="text/javascript">
	document.getElementById('frmEntidade').onsubmit  = function(e) {
	if (document.getElementById('entnumcpfcnpj').value == '') {
		alert('O CPF é obrigatório.');
		return false;
	}

	if (document.getElementById('entnome').value == '') {
		alert('O nome é obrigatório.');
		return false;
	}

    if (document.getElementById('entemail').value == '') {
        alert('O email é obrigatório.');
        return false;
    }
	return true;
	}
    </script>
  </body>
</html>
