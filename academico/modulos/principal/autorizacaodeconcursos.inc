<?php
pegaSessoes(1, 0, 0 );

// instancia o objeto do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();

// Define as variáveis da tela
$_SESSION["academico"]["entidcampus"] = $_REQUEST['entid'];
$entidcampus = $_SESSION["academico"]["entidcampus"];
$prtid 		 = $_SESSION["academico"]["prtid"];
$ano   		 = $_SESSION["academico"]["ano"];
$tprnivel 	 = $_SESSION["academico"]["tprnivel"]; 
$prtid		 = $_SESSION["academico"]["prtid"];
$orgid 		 = $_SESSION["academico"]["orgid"];

// Verifica se existe a entidade informada
	$existe_entidade = academico_existecampus(  $_REQUEST["entid"] );
	if ( !$existe_entidade ){
		echo "<script>
				alert('A unidade informada não existe!');
				history.back(-1);
			  </script>";
		die;
	}

// cria as sessões das entidades
if ( $_REQUEST["entid"] ){

	
	
	$_SESSION["academico"]["entidcampus"] = $_REQUEST["entid"];
	$_SESSION["academico"]["entid"] 	  = $autoriazacaoconcursos->buscaentidade( $_SESSION["academico"]["entidcampus"] );
	
}

// Cadastra os valores no banco
if ( $_REQUEST['requisicao'] == 1 ){
	$autoriazacaoconcursos->cadastraautoriazacaoconcursos( $_REQUEST, $_SESSION["academico"]["orgid"], $_SESSION['academico']['tprnivel'] );
}



// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

// 
$db->cria_aba($abacod_tela,$url,$parametros);

// cria o titulo da tela
$titulo_modulo = ($tprnivel == ACA_TPORTARIA_PROVIMENTO) ?  "Autorização de Provimentos" : "Autorização de Concursos"; 

monta_titulo( $titulo_modulo, "");

// cria o cabeçalho padrão do módulo
if (isset($prtid) && isset($entidcampus)){
	echo $autoriazacaoconcursos->cabecalho( $prtid, $entidcampus );
}else if  (isset($prtid) and $prtid!=''){
	echo $autoriazacaoconcursos->cabecalho_ini( $prtid );
}
?>
<script type="text/javascript">

	function salvarAutorizacaoConcursos(){
		formulario.submit();
	}
	
	function preencheAutorizado( autorizado ){
	
		var projetado  = document.getElementsByName('acpvalor[' + autorizado + ']')[0].value;
		var campoautorizado = document.getElementsByName('lnpvalor_con[' + autorizado + ']')[0];
	
		campoautorizado.value = projetado; 
	
	}
	
	function validaprojetado( autorizado ){
	
		var projetado  = document.getElementsByName('acpvalor[' + autorizado + ']')[0].value;
		var campoautorizado = document.getElementsByName('lnpvalor_con[' + autorizado + ']')[0];
		
		if ( projetado == '' ){
			alert('Para preencher autorizado, é necessário ter valores Projetados!');
			campoautorizado.value = '';
			return false;
		}		
	}

	function validaautorizado_con( autorizado ){
	
		var projetado  		= document.getElementsByName('acpvalor[' + autorizado + ']')[0].value;
		var campoautorizado = document.getElementsByName('lnpvalor_con[' + autorizado + ']')[0];
	
		if ( Number( campoautorizado.value ) > Number( projetado ) ){
			alert('A quantidade do autorizado não pode ser maior do que o projetado!');
			campoautorizado.value = '';
			return false;
		}	
	}
	
	function validaautorizado_prov( autorizado ){
	
		var projetado  		= document.getElementsByName('acpvalor[' + autorizado + ']')[0].value;
		var campoautorizado = document.getElementsByName('lnpvalor_con[' + autorizado + ']')[0];
	
		if ( Number( campoautorizado.value ) > Number( projetado ) ){
			alert('A quantidade do autorizado não pode ser maior do que o projetado!');
			campoautorizado.value = '';
			return false;
		}	
	}
	
	function validalancamento( id ){	
		var projetado  		= document.getElementsByName('acpvalor[' + id + ']')[0];
		var autorizado 		= document.getElementsByName('lnpvalor_con[' + id + ']')[0];
		var provido 		= document.getElementsByName('lnpvalor_prov[' + id + ']')[0];
		var saldoaut 		= document.getElementsByName('saldoaut[' + id + ']')[0];
		var projetado_old 	= document.getElementsByName('acpvalor_old[' + id + ']')[0];
		var autorizado_old 	= document.getElementsByName('lnpvalor_con_old[' + id + ']')[0];
		var provido_old 	= document.getElementsByName('lnpvalor_prov_old[' + id + ']')[0];
		var saldoaut_old 	= document.getElementsByName('saldoaut_old[' + id + ']')[0];
		
				
		if ( Number(autorizado.value) > Number(projetado_old.value) ){
			alert('A quantidade do autorizado não pode ser maior do que o projetado!');
			autorizado.value = autorizado_old.value;
			autorizado.focus();
			autorizado.select();
			return false;
		}	
		if(provido){			
			if ( Number(provido.value) > Number(autorizado_old.value) ){
				alert('A quantidade para provimento não pode ser maior do que o autorizado!');
				provido.value = provido_old.value;
				provido.focus();
				provido.select();
				return false;
			}	
		}
		if(saldoaut){			
			if ( Number(provido.value) > (Number(saldoaut_old.value) + Number(provido_old.value)) ){
				alert('A quantidade para provimento não pode ser maior do que o saldo autorizado!');
				provido.value = provido_old.value;
				provido.focus();
				provido.select();
				
				return false;
			}	
		}
	}
	function atualizatotal( id ){
		var totalprov 		= document.getElementsByName('totalprov[' + id + ']')[0];
		var saldoaut  		= document.getElementsByName('saldoaut[' + id + ']')[0];
		var provido 		= document.getElementsByName('lnpvalor_prov[' + id + ']')[0];
		var saldototal 		= Number(saldoaut.value) - Number(provido.value);
		
		if(! isNaN(provido.value)){
		
			if(saldototal < 0){
				var provido_old 	= document.getElementsByName('lnpvalor_prov_old[' + id + ']')[0];
				alert('A quantidade para provimento não pode ser maior do que o saldo autorizado!');
				provido.value = provido_old.value;
			}else{
				totalprov.value = saldototal;
			}
				
		}
	}

</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="requisicao" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td><br/></td>
		</tr>
		<tr>
			<td>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" style="width: 80%;">
					<thead>
						<tr>							
							<?
							if($tprnivel == ACA_TPORTARIA_PROVIMENTO){
								echo("<td width=\"24%\" align=\"center\"><b>Classes</b></td>
										<td width=\"19%\" align=\"center\"><b>Projetado</b></td>
										<td width=\"19%\" align=\"center\"><b>Concursos Autorizados</b></td>										
										<td width=\"19%\" align=\"center\"><b>Autorização de Provimentos</b></td>
										<td width=\"19%\" align=\"center\"><b>Saldo para Provimentos</b></td>
										");
							}else{
								echo("<td width=\"33%\" align=\"center\"><b>Classes</b></td>
										<td width=\"33%\" align=\"center\"><b>Projetado</b></td>
										<td width=\"33%\" align=\"center\"><b>Concursos Autorizados</b></td>");
							}
							 ?>
						</tr>
					</thead>
					<tbody>
						<?php 
						if (isset($entidcampus) && isset($orgid) && isset($prtid) )
							echo $autoriazacaoconcursos->montatabelatecnicos( $entidcampus, $orgid, $prtid); 
						
						?>
					</tbody>
				</table>
			</td>
		</tr>
		<tr>
			<td><br/></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<input style="cursor:pointer;" type="button" value="Salvar" onclick="salvarAutorizacaoConcursos();" <? echo $disabled; ?>/>
				<input style="cursor:pointer;" type="button" value="Voltar" onclick="history.back(-1);"/>
			</td>
		</tr>
	</table>
</form>