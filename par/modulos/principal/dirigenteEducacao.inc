<?php

require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "includes/classes/entidades.class.inc";

$entidade = new Entidades();
$oDadosUnidade = new DadosUnidade();

$arDadosUnidade = $oDadosUnidade->recuperarDadosPorInuid($_SESSION['par']['inuid'], DIRIGENTE_ESTADUAL_MUNICIPAL);

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	$msg = "";
	if($_POST['entnumcpfcnpj'] == ""){
		$msg .= 'O campo CPF é obrigatório. \n';
	}

	if($_POST['entnome'] == ""){
		$msg .= 'O campo Nome é obrigatório. \n';
	}
	
	if($_POST['entemail'] == ""){
		$msg .= 'O campo Email é obrigatório. \n';
	}
	
	if($_POST['entnumrg'] == ""){
		$msg .= 'O campo RG é obrigatório. \n';
	}
	
	if($_POST['entorgaoexpedidor'] == ""){
		$msg .= 'O campo Órgão Expedidor é obrigatório. \n';
	}
	
	if($_POST['entnumdddcomercial'] == ""){
		$msg .= 'O campo Telefone Comercial é obrigatório. \n';
	}
	
	if($_POST['entnumdddcelular'] == ""){
		$msg .= 'O campo Celular é obrigatório. \n';
	}
	
	if($msg){
		//ver($msg,d);
		echo "<script>
				alert('{$msg}');			
				//document.location.href = '?modulo=principal/dirigenteEducacao&acao=A';
				history.back();
		  </script>";
		exit;
	}
	
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
			
	if(!empty($arDadosUnidade[0]['dunid'])){
		$oDadosUnidade->dunid 	= $arDadosUnidade[0]['dunid'];
	}
	
	$oDadosUnidade->inuid 	= $_SESSION['par']['inuid'];
	$oDadosUnidade->entid 	= $entidade->getEntId();
	$oDadosUnidade->dundata 	= date('Y-m-d H:i:s');
	$oDadosUnidade->usucpf 	= $_SESSION['usucpf'];
	$oDadosUnidade->dutid 	= DIRIGENTE_ESTADUAL_MUNICIPAL;
	$oDadosUnidade->salvar();
	$oDadosUnidade->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
			document.location.href = '?modulo=principal/dirigenteEducacao&acao=A';
		  </script>";
}

if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
	$stIntrumento = "Prefeitura Municipal";
	// Prefeito(a)
	$funid = 15;
	
} else {
	$stIntrumento = "Governo Estadual";
	// Secretaria Estadual de Educação
	$funid = 25;	
}

print "<br />";

print carregaAbaDadosUnidade("/par/par.php?modulo=principal/dirigenteEducacao&acao=A");
			
monta_titulo( "Dados da unidade", "Sec. {$stIntrumento}/Dirigente {$stIntrumento} de Educação" );
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">


jQuery.noConflict();

jQuery(document).ready(function(){
	var anterior = "<input type=\"button\" name=\"anterior\" value=\"<<\" onclick=\"window.location='par.php?modulo=principal/orgaoEducacao&acao=A'\" >";
	var proximo = "<input type=\"button\" name=\"proximo\" value=\">>\" onclick=\"window.location='par.php?modulo=principal/equipeLocal&acao=A'\" >";
	var acoes = jQuery('#tr_acoes').find('td').html();
	jQuery('#tr_acoes').find('td').html(anterior+acoes+proximo);

	//Campos Desabilitados
	document.getElementById('tr_entobs').style.display 						= 'none';
	document.getElementById('tr_entnumdddfax').style.display 				= 'none';
	document.getElementById('tr_endcep').style.display 						= 'none';
	document.getElementById('tr_endlog').style.display 						= 'none';
	document.getElementById('tr_endcom').style.display 			 			= 'none';
	document.getElementById('tr_endnum').style.display 						= 'none';
	document.getElementById('tr_endbai').style.display 						= 'none';
	document.getElementById('tr_estuf').style.display 						= 'none';
	document.getElementById('tr_mundescricao').style.display 				= 'none';
	document.getElementById('tr_latitude').style.display 					= 'none';
	document.getElementById('tr_longitude').style.display 					= 'none';
	document.getElementById('tr_endmapa').style.display 					= 'none';
	document.getElementById('tr_endereco').style.display 					= 'none';
	document.getElementById('tr_funid').style.display 						= 'none';


	jQuery('#frmEntidade').submit(function(){
		var msg = "";

		if(jQuery('#entnumcpfcnpj').val() == ""){
			msg += "O campo CPF é obrigatório. \n";
		}

		if(jQuery('#entnome').val() == ""){
			msg += "O campo Nome é obrigatório. \n";
		}

		if(jQuery('#entemail').val() == ""){
			msg += "O campo Email é obrigatório. \n";
		}

		if(jQuery('#entnumrg').val() == ""){
			msg += "O campo RG é obrigatório. \n";
		}

		if(jQuery('#entorgaoexpedidor').val() == ""){
			msg += "O campo Órgão Expedidor é obrigatório. \n";
		}

		if(jQuery('#entnumdddcomercial').val() == ""){
			msg += "O campo Telefone Comercial é obrigatório. \n";
		}

		if(jQuery('#entnumdddcelular').val() == ""){
			msg += "O campo Celular é obrigatório. \n";
		}

		if(msg){
			alert(msg);
			//return false;
		}
		
	});


});


</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']); ?>
		</td>
	</tr>
</table>
<?php

$entidade = new Entidades();

if($arDadosUnidade[0]['entid']){
	$entid = $db->pegaUm("SELECT e.entid FROM entidade.entidade e WHERE e.entid = '{$arDadosUnidade[0]['entid']}'");
	$entidade->carregarPorEntid($entid);	
}

echo $entidade->formEntidade("?modulo=principal/dirigenteEducacao&acao=A&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => null),
							 array( "enderecos"=>array(1) ) );

?>