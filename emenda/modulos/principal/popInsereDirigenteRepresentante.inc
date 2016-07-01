<?php

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
	
	$entid 	=	$entidade->getEntId();
	$cpf	=	$_REQUEST["entnumcpfcnpj"];
	$nome	=	$_REQUEST["entnome"];
	
	echo "<script>
			alert('Dados gravados com sucesso');
			
			window.opener.document.getElementsByName('entid_dirigente_representante')[0].value	=	'".$entid."';
			window.opener.document.getElementsByName('usucpfdirigente')[0].value				=	'".$cpf."';
			window.opener.document.getElementsByName('usunome')[0].value						=	'".$nome."';
			window.opener.document.getElementById('labelCPF').innerHTML    					    =	'<img src=\"../imagens/gif_inclui.gif\" border=\"0\"> Clique aqui para alterar o CPF';
			
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

$arrFunidEspecifico = $db->carregarColuna("SELECT 
											  funid
											 FROM 
											  emenda.cargos");

echo $entidade->formEntidade("?modulo=principal/popInsereDirigenteRepresentante&acao=A&opt=salvarRegistro", 
							 array("funid" => FUN_DIRIGENTE_REPRESENTANTE, "entidassociado" => null),
							 array("enderecos"=>array(1), "funidEspecifico" => $arrFunidEspecifico)
							 );
							 
?>

<script type="text/javascript">

$('frmEntidade').onsubmit  = function(e) {
	
	selectAllOptions( $('funcoescadastradas') );
	
	if (trim($F('entnumcpfcnpj')) == '') {
		alert("O campo 'CPF' deve ser informado.");
		$('entnumcpfcnpj').focus();
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert("O campo 'nome' deve ser informado.");
		$('entnome').focus();
		return false;
	}
	if (trim($F('entnumrg')) == '') {
		alert("O campo 'Registro Geral(RG)' deve ser informado.");
		$('entnumrg').focus();
		return false;
	}
	if (trim($F('entorgaoexpedidor')) == '') {
		alert("O campo 'Orgão Expeditor' deve ser informado.");
		$('entorgaoexpedidor').focus();
		return false;
	}
	/*if($('funcoescadastradas').options[0].text == 'Não existem funções cadastradas'){
		alert("A função desta entidade não está cadastrada.");
		$('funcoescadastradas').focus();
		return false;		
	}*/
	if (trim($('endcep1').value) == '') {
		alert("O campo 'CEP' deve ser informado.");
		$('endcep1').focus();
		return false;
	}
	if (trim($('endlog1').value) == '') {
		alert("O campo 'Logradouro' deve ser informado.");
		$('endlog1').focus();
		return false;
	}
	if (trim($('endnum1').value) == '') {
		alert("O campo 'Número' deve ser informado.");
		$('endnum1').focus();
		return false;
	}
	if (trim($('endbai1').value) == '') {
		alert("O campo 'Bairro' deve ser informado.");
		$('endbai1').focus();
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