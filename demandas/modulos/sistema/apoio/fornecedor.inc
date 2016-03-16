<?php 

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_REQUEST['requisicao'] == 'salvarFornecedor'){
	
	$entid = salvarRegistroEntidade($_POST);
	
    echo '<script type="text/javascript">
    		alert("Dados gravados com sucesso");
		    window.location = \'?modulo=sistema/apoio/fornecedor&acao=A&entid='.$entid.'\';
	      </script>';
    exit;
	
	
}


function salvarRegistroEntidade($dados) {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->adicionarFuncoesEntidade($dados['funcoes']);
	$entidade->salvar();
	
    return $entidade->getEntId(); 
}


?>


<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Fornecedor - DEMANDAS</title>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<div>

<?

$entidade = new Entidades();
if($_REQUEST['entid']) $entidade->carregarPorEntid($_REQUEST['entid']);
//$entidade->carregarPorFuncaoEntAssociado(FUNID_FORNECEDOR,100);
echo $entidade->formEntidade("demandas.php?modulo=sistema/apoio/fornecedor&acao=A&requisicao=salvarFornecedor",
							 array("funid" => FUNID_FORNECEDOR),
							 array("enderecos"=>array(1))
							 );

?>


<script type="text/javascript">

document.getElementById('tr_entcodent').style.display='none';
document.getElementById('tr_entunicod').style.display='none';
document.getElementById('tr_entungcod').style.display='none';
document.getElementById('tr_entsig').style.display='none';
document.getElementById('tr_njuid').style.display='none';
document.getElementById('tr_tpctgid').style.display='none';
document.getElementById('tr_tpcid').style.display='none';
document.getElementById('tr_tplid').style.display='none';
document.getElementById('tr_tpsid').style.display='none';
document.getElementById('tr_funcoescadastradas').style.display='none';


$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '') {
		alert('CNPJ é obrigatório.');
    	return false;
	}
	/*
	if (trim($F('entnome')) == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	if (trim($F('entdatanasc')) != '') {
		if(!validaData(document.getElementById('entdatanasc'))) {
			alert("Data de nascimento é inválida.");return false;
		}
	}
	*/
	return true;
}
</script>

  </body>
</html>

