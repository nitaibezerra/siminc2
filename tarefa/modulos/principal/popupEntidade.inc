<?php

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	$entid 			   = $entidade->getEntId();
	$cpf			   = $_REQUEST["entnumcpfcnpj"];
	$nome			   = $_REQUEST["entnome"];
	$entnumresidencial = $_REQUEST["entnumdddresidencial"].' - '.$_REQUEST["entnumresidencial"];
	$entemail 		   = $_REQUEST["entemail"];
	
	echo "<script>
			alert('Dados gravados com sucesso');
			
			//window.opener.document.getElementsByName('entid')[0].value	 =	'".$entid."';
			
			window.opener.document.getElementsByName('solcpf')[0].value	 	=	'".$cpf."';
			window.opener.document.getElementsByName('solcpf')[0].className = 'disabled';
			window.opener.document.getElementsByName('solcpf')[0].readOnly  = true;
			
			window.opener.document.getElementsByName('solnome')[0].value 	 =	'".$nome."';
			window.opener.document.getElementsByName('solnome')[0].className = 'disabled';
			window.opener.document.getElementsByName('solnome')[0].readOnly  = true;
			
			window.opener.document.getElementsByName('soltelefone')[0].value 	 =	'".$entnumresidencial."';
			window.opener.document.getElementsByName('soltelefone')[0].className = 'disabled';
			window.opener.document.getElementsByName('soltelefone')[0].readOnly  = true;			
			
			window.opener.document.getElementsByName('solemail')[0].value 	 =	'".$entemail."';
			window.opener.document.getElementsByName('solemail')[0].className = 'disabled';
			window.opener.document.getElementsByName('solemail')[0].readOnly  = true;
			
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
    <title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
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

/*
 * Código do componente de entidade
 */
$entidade = new Entidades();

if($_GET['entid'] && $_GET['entid'] != "") {
	$entid = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entid = {$_GET['entid']}");
	if(!$entid){
		echo "<script>
				alert('A entidade informada não existe!');
				window.close();
			  </script>";
		die;
	}
	$entidade->carregarPorEntid($entid);
}

echo $entidade->formEntidade("?modulo=principal/popupEntidade&acao=A&opt=salvarRegistro",
							 array("funid" => FUN_SOLICITANTE_TAREFA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
							 
?>
<script type="text/javascript">

$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '') {
		alert("O campo 'CPF' deve ser informado.");
		$('entnumcpfcnpj').focus();
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert("O campo 'Nome' deve ser informado.");
		$('entnome').focus();
		return false;
	}
	return true;
}

var btncancelar = document.getElementById('btncancelar');

if(window.addEventListener){ // Mozilla, Netscape, Firefox
	btncancelar.setAttribute( "onclick", "fechar()" );
} else{ // IE
	btncancelar.attachEvent( "onclick", function() { fechar(  ) } );
}
 
function fechar(){
	window.close();
}
</script>
</div>
</body>
</html>