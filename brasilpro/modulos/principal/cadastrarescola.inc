<?php
$inuid = $_SESSION["inuid"];
$estuf  = cte_pegarEstuf( $inuid );

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

cte_verificaSessao();
$itrid = cte_pegarItrid( $inuid );

require_once APPRAIZ . "includes/classes/entidades.class.inc";
define("FUNCAO_ESCOLA", 3);

if ($_REQUEST['opt'] && $_REQUEST['opt'] == 'gerarEntcodent') {
    global $db;
    $last_value = 'EN' . sprintf("%06d", $db->pegaUm('SELECT NEXTVAL(\'entidade.entidadeentcodent_seq\')'));
    echo trim($last_value);
    die();
}


if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * CÓDIGO DO NOVO COMPONENTE 
	 */
	
	$_REQUEST["tpcid"] 	  = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? 1 : 3;
	$_REQUEST["entproep"] = 'false';	
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='brasilpro.php?modulo=principal/cadastrarescola&acao=A&entid={$entidade->getEntId()}';
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
if($_GET['entid']){
	$entid = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entid = {$_GET['entid']}");
	if(!$entid){
		echo "<script>
			alert('A Entidade informada não existe!');
			window.close();
		  </script>";
		die;
	}
	$entidade->carregarPorEntid($entid);
}
//Nova escola (não possui código INEP)
echo $entidade->formEntidade("?modulo=principal/cadastrarescola&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_ESCOLA, "entidassociado" => null),
							 array("enderecos"=>array(1), 'novaEscola' => 1)
							 );

?>

<script type="text/javascript">
$('frmEntidade').onsubmit  = function(e) {
		
	if (trim($F('entcodent')) == '' && trim($F('entnumcpfcnpj')) == '') {
		alert('O INEP ou CNPJ da entidade é obrigatório.');
		return false;
	}
	if (trim($F('entnome')) == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}	
	
	if (trim($F('endcep1')) == '') {
		alert('O CEP da entidade é obrigatório.');
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

     $('endcep1').onblur = function(e){
     	MouseBlur(this);
     	getEnderecoPeloCEP($('endcep1').value,'1');
    	verificaCEPIgual();
    }
    
    function verificaCEPIgual(){
    	var estufCep = $('estuf1').value;
    	if( estufCep && '<?php echo $estuf ?>' ){
			if(  '<?php echo $estuf ?>' != $('estuf1').value )
				alert( "A UF do CEP informado é diferente à do estado escolhido!\nFavor Verificar." );
    	}
    	else
    	   	setTimeout("verificaCEPIgual()", 50);
    }

function gerarEntcodent( boChecado ){

	if( boChecado ){
		
		return new Ajax.Request(window.location.href, {
			method: 'post',
			parameters: '&opt=gerarEntcodent',
			asynchronous: false,
			onComplete: function(res) {	
				document.getElementById( 'entcodent' ).value = res.responseText;
				document.getElementById( 'entescolanova' ).value = 'true';
				document.getElementById( 'entnome' ).readOnly = false;
				document.getElementById( 'entnome' ).style.borderLeft="#000000 3px solid";
				document.getElementById( 'entnome' ).style.borderRight="#cccccc 1px solid";
				document.getElementById( 'entnome' ).style.borderBottom="#cccccc 1px solid";
				
				document.getElementById( 'entrazaosocial' ).readOnly = false;
				document.getElementById( 'entrazaosocial' ).style.borderLeft="#000000 3px solid";
				document.getElementById( 'entrazaosocial' ).style.borderRight="#cccccc 1px solid";
				document.getElementById( 'entrazaosocial' ).style.borderBottom="#cccccc 1px solid";
				
			}
		});
	}
	else{
		document.getElementById( 'entcodent' ).value = '';
		document.getElementById( 'entescolanova' ).value = 'false';
	}

}

</script>
</div>
</body>
</html>