<?php 
include_once(APPRAIZ."academico/classes/MaisMedicosResponsabilidade.class.inc");
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_REQUEST['mmrtipo'] == "S") {
	$funid = FUNCAO_ENTIDADE_SUPERVISOR;
} else {
	$funid = FUNCAO_ENTIDADE_TUTOR;
}

if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	if($_REQUEST['funcoes']) {
		$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
		$entidade->salvar();
		$entid = $entidade->getEntid();
		$tutor = new MaisMedicosResponsabilidade();
		$tutor->recuperarPorEntid($entid,null,$_REQUEST['mmrtipo']);
		$arrDados['entidresponsavel'] = $entid;
		$arrDados['entidunidade']     = $_SESSION['academico']['entid'];
		$arrDados['usucpfcadastro']   = $_SESSION['usucpf'];
		$arrDados['mmrdatainclusao']  = date("Y-m-d H:m:s");
		$arrDados['mmrtipo']  = $_REQUEST['mmrtipo'] ? $_REQUEST['mmrtipo'] : "T";
		$tutor->popularDadosObjeto($arrDados);
		$tutor->salvar();
		$tutor->commit();
		echo '<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>';
		echo '<script type="text/javascript">
                        //window.parent.listaTutoresAjax('.$_SESSION['academico']['entid'].');
                        //window.opener.execScript(\'listaTutoresAjax('.$_SESSION['academico']['entid'].')\');
                       $.ajax({
					   type: "POST",
					   url: "academico.php?modulo=principal/maisMedicos&acao=A",
					   data: "requisicaoAjax=listaTutoresAjax&classe=MaisMedicosResponsabilidade&entid='.$_SESSION['academico']['entid'].'&mmrtipo='.$arrDados['mmrtipo'].'",
					   success: function(msg){
					   		$( \'#'.($arrDados['mmrtipo'] == "T" ? "div_lista_tutores" : "div_lista_supervisores").'\', window.opener.document ).html( msg );
					   		window.close();
					   		}
					   });

                      </script>';
	} else {
		echo '<script type="text/javascript">
                alert("Informações sobre entidade não enviadas corretamente. Refaça o procedimento.");
                window.close();
              </script>';
	}

	exit;
}

?>
<!doctype html>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<?php

$entidade = new Entidades();
if($_REQUEST['entid'])
    $entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("academico.php?modulo=principal/cadTutores&acao=A&opt=salvarRegistro&mmrtipo={$_REQUEST['mmrtipo']}",
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