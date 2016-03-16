<?php

switch ($_REQUEST["requisicao"]) {
	
	case "criarprojeto":
		$ies->criaProjetoIes( $_REQUEST["iesid"] );
	break;
	
}


$iesHabilitado = $_REQUEST["acao"] == 'C' ? "disabled" : "";

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'] || $_REQUEST['entunicod'])) {
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->salvar();
		
		$ies->cadastraResponsavelIes( $_REQUEST["entid"] );
		
    }
}

if ( $_REQUEST["iesid"] || $_SESSION["ies"]["iesid"] ){
	
	$_SESSION["ies"]["iesid"] = !empty($_REQUEST["iesid"]) ? $_REQUEST["iesid"] : $_SESSION["ies"]["iesid"];  
	$iesid = $_SESSION["ies"]["iesid"];
	
	if ( !iesVerificaIes( $iesid ) ){
		
		echo '<script>
				alert("A Instituição informada não existe!");
				history.back(-1);
		  	  </script>';
		
		unset($_SESSION["ies"]["iesid"]);
		die;
		
	}else{
		$entid = iesPegaResponsavel( $iesid );
	}
	
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Dados do Responsável pelo Programa IES na Instituição", "");

$ies->cabecalhoIES( $iesid );

$entidade = new Entidades();

if ( $entid ){
	$entidade->carregarPorEntid($entid);
}

echo "<table class='tabela' width='95%' bgcolor='#f5f5f5' cellspacing='1' cellpadding='2' align='center'><tr><td class='subtitulocentro'>&nbsp;</td></tr></table>";

$javascript = "<script>
					document.getElementById('frmEntidade').onsubmit  = function(e) {
					
						var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): ';
						var validacao = true;
						
						if (document.getElementById('entemail').value == ''){
							mensagem += 'E-mail; ';
							validacao = false;
						}
						
						if (document.getElementById('entnumcomercial').value == ''){
							mensagem += 'Telefone Comercial; ';
							validacao = false;
						}
						
						if (document.getElementById('entnumfax').value == ''){
							mensagem += 'Fax;';
							validacao = false;
						}
						
						if( !validacao ){
							alert(mensagem);
							return false;
						}else{
							return true;
						}
						
					}
			   </script>";

echo $entidade->formEntidade("ies.php?modulo=principal/dadosResponsavel&acao=A&opt=salvarRegistro",
							 array("funid" => IES_FUNID_RESPONSAVEL, "entidassociado" => null));

							 
if($iesHabilitado) {
	echo "<script>
			document.getElementById('entnumcpfcnpj').disabled=true
			document.getElementById('btngravar').disabled=true
		 </script>";
} 

echo $javascript;

?>
<script>
	
	<?php if ( !$entid ): ?>
	
		var cnpj = mascaraglobal("###.###.###-##", "<?=$_SESSION["usucpf"];?>");
		document.getElementById('entnumcpfcnpj').value = cnpj;
		getEntidadePeloCPF(cnpj);
		
	<?php endif; ?>
	
	document.getElementById('tr_entobs').style.display = 'none';
	document.getElementById('tr_funid').style.display  = 'none';
	document.getElementById('tr_titulo').style.display = 'none';
	
</script>