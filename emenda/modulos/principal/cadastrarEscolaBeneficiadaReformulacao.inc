<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

$obRefor = new reformulacaoPTA( $db );
verificaPermissao();

//Persistencia
if($_REQUEST['salvarEscola']) {
	header('content-type: text/html; charset=ISO-8859-1');
	salvarEscola($_POST);
	exit;
}

//Ajax de busca de escolas
if ($_REQUEST['filtraEscolasAjax'] && $_REQUEST['entcodent']) {
	header('content-type: text/html; charset=ISO-8859-1');
	buscaEscola($_REQUEST['entcodent']);
	exit;
}

//Ajax para excluir
if ($_REQUEST['excluirEscola'] && $_REQUEST['esbid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	removerEscola($_REQUEST['esbid']);
	exit;
}

//Pesquisa lista
if($_REQUEST['pesquisa'] && $_REQUEST['ptrid']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->listaEscolaBeneficiada($_REQUEST['ptrid']);
	exit;
}
$ptrid = $_REQUEST["ptrid"];
validaSessionPTA( $ptrid );

monta_titulo( 'Escolas Beneficiadas', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

?>	
<html>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
	
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td align="center" bgcolor="#dedfde"><b>Escolas Beneficiadas</b></td>
	</tr>
	<tr>
		<td align="center">
			<br />
			<div id="lista"><?  $obRefor->listaEscolaBeneficiada( $ptrid ); ?></div>
			<br />
		</td>
	</tr>
</table>

</form> 
<div id="erro"></div>




<script type="text/javascript">

function salvarEscola(){
	if($('esbquantidadealunos').value <= 0){
		alert("Favor informar algum valor no campo 'Alunos Beneficiados'.");
		$('esbquantidadealunos').focus();
		return false;
	}
	if($F('inepID')&& $F('esbquantidadealunos')){
//		var params = 'inepID=' + $F('inepID') + '&entid=' + $F('entid') + '&esbquantidadealunos=' + $F('esbquantidadealunos') + '&ptrid=' + $F('ptrid')+ '&esbid=' + $F('esbid') + '&ptridpai=' + $F('ptridpai') + '&refid=' + $F('refid');
		var params = 'inepID=' + $F('inepID') + '&entid=' + $F('entid') + '&esbquantidadealunos=' + $F('esbquantidadealunos') + '&ptrid=' + $F('ptrid')+ '&esbid=' + $F('esbid');
		
		$('loader-container').show();
		var req = new Ajax.Request('emenda.php?modulo=principal/cadastrarEscolaBeneficiadaReformulacao&acao=A', {
		        method:     'post',
		        parameters: 'salvarEscola=true&'+params,
		        asynchronous: false,
		        onComplete: function (res){
		        	//$('erro').innerHTML = res.responseText;
		        	if(res.responseText==1){
		        		alert('O código INEP já está cadastrado!');
		        		document.formulario.inepID.value='';
		        		document.formulario.esbquantidadealunos.value='';
		        		document.formulario.nmescola.value='';
		        		document.formulario.inepID.focus();
		        	}
		        	else{
						pesquisar();
					} 
		        }
		  });
		$('loader-container').hide();
		window.opener.location =  window.opener.location; //'emenda.php?modulo=principal/reformulacaoPTA&acao=A&ptrid='+$F('ptrid');
	}
	else {
		alert('Todos os campos são obrigatórios!');
		return false;
	}
		
}

function removerEscola(esbid){
	if(confirm('Deseja realmente excluir a Escola beneficiada?')) {
		$('loader-container').show();
		var req = new Ajax.Request('emenda.php?modulo=principal/cadastrarEscolaBeneficiada&acao=A', {
		        method:     'post',
		        parameters: 'excluirEscola=true&'+'esbid='+esbid,
		        asynchronous: false,
		        onComplete: function (res){
					pesquisar();
		        }
		  });
		$('loader-container').hide();
		window.opener.location =  window.opener.location;
	}
}

function pesquisar(){
		$('loader-container').show();
		var myajax = new Ajax.Request('emenda.php?modulo=principal/cadastrarEscolaBeneficiada&acao=A', {
			        method:     'post',
			        parameters: 'pesquisa=true&ptrid='+$('ptrid').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('lista').innerHTML = res.responseText;
			        }
			  });
		$('loader-container').hide();
}

function buscaEscola(entcodent){

	if(entcodent||''){
		var destino = document.getElementById("td_escola");
		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: "filtraEscolasAjax=true&" + "entcodent=" + trim(entcodent),
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText==1){
						alert('O código INEP inválido!');
						document.formulario.inepID.focus();
		        		document.formulario.inepID.value='';
		        		document.formulario.esbquantidadealunos.value='';
		        		document.formulario.nmescola.value='';
		        		return false;
		        	}
		        	else{
		        		passa();
						destino.innerHTML = resp.responseText;
					}
				},
				onLoading: function(){
					destino.innerHTML = 'Carregando...';
				}
			});
	}
	else {
		document.formulario.inepID.focus();
		document.formulario.inepID.value='';
		document.formulario.esbquantidadealunos.value='';
		document.formulario.nmescola.value='';
		return false;
	}
	
}

function alterar(esbid){
	document.formulario.esbid.value = document.getElementById("esbid_"+esbid).value;
	document.formulario.entid.value = document.getElementById("entid_"+esbid).value;	
	document.formulario.inepID.value = document.getElementById("inepID_"+esbid).value;
	document.getElementById('inepID').setAttribute('readOnly','readOnly');
	document.formulario.nmescola.value = document.getElementById("entnome_"+esbid).value;
	document.formulario.esbquantidadealunos.value = document.getElementById("esbquantidadealunos_"+esbid).value;
	document.formulario.btSalvar.value = "Alterar";
	document.formulario.esbquantidadealunos.focus();
}

function passa(){
	document.formulario.esbquantidadealunos.focus();
}

function CancelaEscola(){
	pesquisar(); 	
}

function consultarINEP() {
	window.open('emenda.php?modulo=principal/popConsultaINEP&acao=A','page12','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=400,height=500');
}

</script>
	


