<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.close();
		  </script>";
	exit;
}

define("FUNPREFEITURA", 1);

/*
 * Validando se a prefeitura foi enviada por REQUEST
 */
if(!$_REQUEST['entid']) {
	echo "<script>
			alert('Nenhuma prefeitura foi selecionada');
			window.close();
		 </script>";
	exit;
}

if($_REQUEST['opt']=='salvarRegistro') {

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
$entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("cte.php?modulo=prefeito/cadastrarprefeitura&acao=A&opt=salvarRegistro",
							 array("funid" => FUNPREFEITURA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
?>
</div>
<script type="text/javascript">
document.getElementById('tr_entcodent').style.display = 'none';
document.getElementById('tr_entunicod').style.display = 'none';
document.getElementById('tr_entungcod').style.display = 'none';
document.getElementById('tr_tpctgid').style.display = 'none';

/*
 * DESABILITANDO O CAMPO DE CNPJ
 */
document.getElementById('entnumcpfcnpj').readOnly = true;
document.getElementById('entnumcpfcnpj').className = 'disabled';
document.getElementById('entnumcpfcnpj').onfocus = "";
document.getElementById('entnumcpfcnpj').onmouseout = "";
document.getElementById('entnumcpfcnpj').onblur = "";
document.getElementById('entnumcpfcnpj').onkeyup = "";

/*
 * DESABILITANDO O CÓDIGO DA UNIDADE
 */
document.getElementById('entunicod').readOnly = true;
document.getElementById('entunicod').className = 'disabled';
document.getElementById('entunicod').onfocus = "";
document.getElementById('entunicod').onmouseout = "";
document.getElementById('entunicod').onblur = "";
document.getElementById('entunicod').onkeyup = "";
/*
 * DESABILITANDO O CAMPO DE INSCRIÇÃO ESTADUAL
 */
document.getElementById('entnuninsest').readOnly = true;
document.getElementById('entnuninsest').className = 'disabled';
document.getElementById('entnuninsest').onfocus = "";
document.getElementById('entnuninsest').onmouseout = "";
document.getElementById('entnuninsest').onblur = "";
document.getElementById('entnuninsest').onkeyup = "";

/*
 * DESABILITANDO O NOME DA ENTIDADE
 */
document.getElementById('entnome').readOnly = true;
document.getElementById('entnome').className = 'disabled';
document.getElementById('entnome').onfocus = "";
document.getElementById('entnome').onmouseout = "";
document.getElementById('entnome').onblur = "";
document.getElementById('entnome').onkeyup = "";

/*
 * DESABILITANDO A SIGLA DA ENTIDADE
 */
document.getElementById('entsig').readOnly = true;
document.getElementById('entsig').className = 'disabled';
document.getElementById('entsig').onfocus = "";
document.getElementById('entsig').onmouseout = "";
document.getElementById('entsig').onblur = "";
document.getElementById('entsig').onkeyup = "";

</script>