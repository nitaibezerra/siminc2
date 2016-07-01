<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "academico/classes/TermoCooperacao.class.inc";

$tmcid = $_GET['tmcid'];
$obTermoCooperacao = new TermoCooperacao($tmcid);
if($_GET['tipoEntidade'] == 'entidadeRepProponente'){
	$msg = 'É necessário cadastrar Entidade Proponente';
	$entidAssoc = $obTermoCooperacao->entidproponente;
	$sigla = 'rp';
	$nome_entid = 'entidrepprop';
} elseif($_GET['tipoEntidade'] == 'entidadeRepConcedente'){
	$msg = 'É necessário cadastrar Entidade Concedente';
	$entidAssoc = $obTermoCooperacao->entidconcedente;
	$sigla = 'rc';
	$nome_entid = 'entidrepconc';
}

if(!$entidAssoc){
	echo "<script>
			alert('$msg');
			window.close();
		  </script>";
	exit;
}

if($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entid = $entidade->getEntId();
	if($entid){
		$obTermoCooperacao = new TermoCooperacao($tmcid);
		$obTermoCooperacao->$nome_entid = $entid;
		$obTermoCooperacao->salvar();
	}
	$entidade->salvar();
	
	$mundescricao = $db->pegaUm("select mundescricao from territorios.municipio where muncod = '{$_POST['endereco'][1]['muncod']}' ");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.opener.location.replace(window.opener.location);
			//window.opener.document.getElementById('$nome_entid').value = '".$entid."';
			//window.opener.document.getElementById('entnumcpfcnpj$sigla').value = '".$_POST['entnumcpfcnpj']."';
			//window.opener.document.getElementById('entnome$sigla').value = '".$_POST['entnome']."';
			//window.opener.document.getElementById('entemail$sigla').value = '".$_POST['entemail']."';
			//window.opener.document.getElementById('entnumdddresidencial$sigla').value = '".$_POST['entnumdddresidencial']."';
			//window.opener.document.getElementById('entnumresidencial$sigla').value = '".$_POST['entnumresidencial']."';
			//window.opener.document.getElementById('entnumfax$sigla').value = '".$_POST['entnumfax']."';
			//window.opener.document.getElementById('endcep$sigla').value = '".$_POST['endereco'][1]['endcep']."';
			//window.opener.document.getElementById('endlog$sigla').value = '".$_POST['endereco'][1]['endlog']."';
			//window.opener.document.getElementById('endbai$sigla').value = '".$_POST['endereco'][1]['endbai']."';
			//window.opener.document.getElementById('estuf$sigla').value = '".$_POST['endereco'][1]['estuf']."';
			//window.opener.document.getElementById('mundescricao$sigla').value = '".$mundescricao."';			
			//window.opener.document.getElementById('entnumrg$sigla').value = '".$_POST['entnumrgr']."';			
			//window.opener.document.getElementById('entorgaoexpedidor$sigla').value = '".$_POST['entorgaoexpedidorrc']."';			
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

echo $entidade->formEntidade($_SERVER['REQUEST_URI'].'&opt=salvarRegistro',
							 array("funid" => $_GET['funid'], "entidassociado" => $entidAssoc),
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