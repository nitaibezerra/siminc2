<?php
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}
require_once APPRAIZ . "includes/classes/entidades.class.inc";

define("ID_VISTORIADOROBRAS", 47);
define("ID_RVISTORIA", 76);
define("ID_ROBRAS", 77);

if ($_REQUEST['opt'] == 'salvarRegistro') {
//    ver($_REQUEST);
    $entidade = new Entidades();
    $entidade->carregarEntidade($_REQUEST);
    
    if($_REQUEST['funcoes']) {
        $entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
        $entidade->salvar();
        echo '<script type="text/javascript">
                window.opener.jQuery("#entnomeresponsavel").html(\'' . $_REQUEST['entnome'] . '\');
                window.opener.jQuery("#entidresponsavel").val(\'' . $entidade->getEntid() . '\');
                window.close();
              </script>';
        

    } else {
        echo '<script type="text/javascript">
                alert("Informações sobre entidade não enviadas corretamente. Refaça o procedimento.");
                window.close();
              </script>';
    }

    exit;
}

if($_REQUEST['funid']) {
    $funid = $_REQUEST['funid'];
} else {
    $funid = ID_VISTORIADOROBRAS;
}

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/entidadesn.js"></script>

<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<?php

$entidade = new Entidades();
if($_REQUEST['entid']){
    $entidade->carregarPorEntid($_REQUEST['entid']);    
}

echo $entidade->formEntidade("obras2.php?modulo=principal/monitoramentoespecial/inserir_responsavel&acao=A&opt=salvarRegistro",
                             array("funid" => $funid, "entidassociado" => null),
                             array("enderecos"=>array(1))
                             );

?>
<script type="text/javascript">

function enviaForm()
{
	var erro = 0;
	if (document.getElementById('entnumcpfcnpj').value == '') {
        erro = 1;
        alert('O CPF é obrigatório.');
        return false;
    }
   
    if (document.getElementById('entnome').value == '') {
        erro = 1;
        alert('O nome da entidade é obrigatório.');
        return false;
    }

    if ( document.getElementById('entdatanasc').value != '' && !validaData(document.getElementById('entdatanasc')) ) {
        erro = 1;
        alert('Formato de data inválido.');
        document.getElementById('entdatanasc').focus();
        document.getElementById('entdatanasc').value = '';
        return false;
    }
    if(erro == 0){
	    <?php if($_GET['validaCPFProfissionalEmrpesa']): ?>
	    	validaCPFProfissionalEmrpesa(document.getElementById('entnumcpfcnpj').value);
	    <?php else: ?>
	    	document.frmEntidade.submit();
	   		return true;
	    <?php endif; ?>
    }
}

function validaCPFProfissionalEmrpesa(temcpf)
{
	
	new Ajax.Request(window.location,
	{
		method: 'post',
		asynchronous : false,
		parameters: 'requisicaoAjax=validaCPFProfissionalEmrpesa&temcpf='+temcpf,
		onSuccess: function(msg){
			if(msg.responseText != "ok"){
	   			alert('O responsável pela supervisão precisa estar ativo no cadastro de ténicos da empresa no sistema.');
	   			return false;
	   		}else{
	   			document.frmEntidade.submit();
	   			return true;
	   		}
		}
	}
	);
}
</script>

</body>
</html>