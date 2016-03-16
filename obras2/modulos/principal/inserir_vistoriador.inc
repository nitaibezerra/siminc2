<?php
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_REQUEST['entid']){
    $_REQUEST['entid'] = strip_tags($_REQUEST['entid']);
}

define("ID_VISTORIADOROBRAS", 47);
define("ID_RVISTORIA", 76);
define("ID_ROBRAS", 77);

if ($_REQUEST['opt'] == 'salvarRegistro') {
    $entidade = new Entidades();
    $entidade->carregarEntidade($_REQUEST);    
    if($_REQUEST['funcoes']) {
        $entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
        $entidade->salvar();
        switch($_REQUEST['funcoes']['funid']) {
            case ID_RVISTORIA:
                echo '<script type="text/javascript">
                        window.opener.jQuery("#entnomerespvistoria").html(\'' . $_REQUEST['entnome'] . '\');
                        window.opener.jQuery("#entidrespvistoria").val(\'' . $entidade->getEntid() . '\');
                        window.close();
                      </script>';
                break;
            case ID_ROBRAS:
                echo '<script type="text/javascript">
                        window.opener.jQuery("#entnomeresptecnico").html(\'' . $_REQUEST['entnome'] . '\');
                        window.opener.jQuery("#entidresptecnico").val(\'' . $entidade->getEntid() . '\');
                        window.close();
                      </script>';
                break;
            default:
            $sql = "select temcargo from obras2.tecnico_empresa where temcpf = '".$_REQUEST['entnumcpfcnpj']."'";
            $temcargo = $db->pegaUm($sql);
            echo '<script type="text/javascript">
            		if ( window.opener.jQuery("#tr_cargo_vistoriador").length ){
            			window.opener.jQuery("#suecargovistoriador").val(\'' . $temcargo . '\');
            			window.opener.jQuery("#tr_cargo_vistoriador").css(\'display\', \'\');
        			}
                    window.opener.jQuery("#entnomevistoriador").html(\'' . $_REQUEST['entnome'] . '\');
                    window.opener.jQuery("#entidvistoriador").val(\'' . $entidade->getEntid() . '\');
                    window.close();
                  </script>';
        }
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
echo $entidade->formEntidade("obras2.php?modulo=principal/inserir_vistoriador&acao=A&opt=salvarRegistro",
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
<?php
        //não permitir edição para perfil CALL CENTER demanda: 311298
        if(possui_perfil(array(PFLCOD_CALL_CENTER) )){
             $blockEdicao = TRUE;
        }
        if(isset($blockEdicao)){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('tblentidade'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#tblentidade').find('input[type=button], input[type=text], input[type=checkbox], input[type=radio], textarea, select').attr('disabled','disabled');
                      //jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                     // jQuery('#btnGerarXls').attr('disabled', false);
                     // jQuery('#xls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>