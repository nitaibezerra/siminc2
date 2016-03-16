<?php

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "includes/classes/entidades.class.inc";
define("FUNCAO_SECRETARIO_MUNICIPAL_EDUCACAO", 15);

if ($_REQUEST['opt'] == 'salvarRegistro') {

	/*
	 * CÓDIGO DO NOVO COMPONENTE 
	 */
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	echo "<script>		
			var entid   = window.opener.document.getElementById( 'entid' );
			var entnome = window.opener.document.getElementById( 'entnome' );
			entid.value = '". $entidade->getEntid() ."';
			entnome.value = '". $entidade->getEntnome() ."';
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
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>
<?php
/*
 * Código do componentes de entidade
 */
$entidade = new Entidades();
if( $_REQUEST['entid'] ){
	$entidade->carregarPorEntid( $_REQUEST['entid'] );
}
echo $entidade->formEntidade("?modulo=principal/programas/feirao_programas/popupContatoTerra&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_SECRETARIO_MUNICIPAL_EDUCACAO, "entidassociado" => null),
							 array( "enderecos"=>array(1) ) );

?>

<script type="text/javascript">
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
</div>
</body>
</html>