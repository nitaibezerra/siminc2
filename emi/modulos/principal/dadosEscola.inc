<?php

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'] || $_REQUEST['entunicod'])) {
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->salvar();
		
		echo "<script>
				alert('Escola atualizada com sucesso');
				window.location='emi.php?modulo=principal/dadosEscola&acao=A';
			  </script>";
		exit;
		
    }
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo("Dados da Escola", "");

if ( $_REQUEST["emeid"] || $_SESSION["emi"]["emeidEscola"] ){

	$_SESSION["emi"]["emeidEscola"] = !empty($_REQUEST["emeid"]) ? $_REQUEST["emeid"] : $_SESSION["emi"]["emeidEscola"];
	$emeid = $_SESSION["emi"]["emeidEscola"];
	
	if ( !emiVerificaEntidade( $emeid ) ){
		
		unset( $_SESSION["emi"]["emeidEscola"] );
		
		print "<script>
				  alert('A escola selecionada não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
	}
	
}

if( $_SESSION["emi"]["emeidEscola"] ){
	$emi->montaCabecalho( $_SESSION["emi"]["emeidEscola"], "escola" );
}

if(!$emeid) {
	die("<script>
			alert('Problemas com variáveis. Refaça o procedimento.');
			window.location='emi.php?modulo=inicio&acao=C';
		  </script>");
}


// pegando a entidade da escola
$sql = "SELECT entid FROM emi.ementidade WHERE emeid='".$emeid."'";
$entid = $db->pegaUm($sql);

$entidade = new Entidades();

if ( $entid ){
	$entidade->carregarPorEntid($entid);
}

echo $entidade->formEntidade("emi.php?modulo=principal/dadosEscola&acao=A&opt=salvarRegistro",
							 array("funid" => EMI_FUNID_ESCOLA, "entidassociado" => $_SESSION['emi']['entidpai']),
							 array("enderecos"=>array(1))
							 );

?>
<script type="text/javascript">
document.getElementById('btncancelar').style.display  = 'none';
	
<?php if( !$emiHabilitado ): ?>
	document.getElementById('btngravar').disabled = true;
<? endif; ?>

document.getElementById('tr_entunicod').style.display = 'none';
document.getElementById('tr_entnuninsest').style.display = 'none';
document.getElementById('tr_entungcod').style.display = 'none';
document.getElementById('tr_tpctgid').style.display = 'none';
document.getElementById('tr_entsig').style.display = 'none';

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
 * DESABILITANDO O CAMPO DE CODIGO INEP
 */
document.getElementById('entcodent').readOnly = true;
document.getElementById('entcodent').className = 'disabled';
document.getElementById('entcodent').onfocus = "";
document.getElementById('entcodent').onmouseout = "";
document.getElementById('entcodent').onblur = "";
document.getElementById('entcodent').onkeyup = "";



/*
 * DESABILITANDO O NOME DA ENTIDADE
 */
document.getElementById('entnome').readOnly = true;
document.getElementById('entnome').className = 'disabled';
document.getElementById('entnome').onfocus = "";
document.getElementById('entnome').onmouseout = "";
document.getElementById('entnome').onblur = "";
document.getElementById('entnome').onkeyup = "";

</script>