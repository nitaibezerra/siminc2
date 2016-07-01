<?php

require_once APPRAIZ . "includes/classes/entidades.class.inc";

/*
 * Cadastrar coordenador vinculado a secretaria do estado
 */
if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'] || $_REQUEST['entunicod'])) {
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->salvar();
		
		$emi->insereResponsavel( $entidade->getEntId() );
		
    }
}

if( $_REQUEST["entid"] || $_SESSION["emi"]["entidResponsavel"] ){
	
	$_SESSION["emi"]["entidResponsavel"] = !empty($_REQUEST["entid"]) ? $_REQUEST["entid"] :  $_SESSION["emi"]["entidResponsavel"];
	$entid = $_SESSION["emi"]["entidResponsavel"]; 
	
	if ( !emiVerificaResponsavel( $entid ) ){
		
		unset( $_SESSION["emi"]["entidResponsavel"] );
		
		print "<script>
				  alert('O responsável selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
		
	}
	
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Coordenador", "");

if( $_SESSION["emi"]["emeidPai"] ){
	$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"] );
}

$entidade = new Entidades();

if ( $entid ){
	$entidade->carregarPorEntid($entid);
}

echo $entidade->formEntidade("emi.php?modulo=principal/cadastraCoordenador&acao=A&opt=salvarRegistro",
							 array("funid" => EMI_FUNID_COORDENADOR, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
 
?>

<script>

	document.getElementById('tr_entobs').style.display    = 'none';
	document.getElementById('tr_funid').style.display     = 'none';
	document.getElementById('btncancelar').style.display  = 'none';
	
<?php if( !$emiHabilitado ): ?>
	document.getElementById('btngravar').disabled = true;
<? endif; ?>

</script>