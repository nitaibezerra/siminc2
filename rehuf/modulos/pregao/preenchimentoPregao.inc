<?php

// se for atalho da area administrativa dos pregões, não exibir abas. Tratamento especial para tela
if($_REQUEST['entid']&&$_REQUEST['preid']) {
	$dadosestruturaunidadeid = $db->pegaUm("SELECT esuid FROM rehuf.estruturaunidade WHERE entid = '".$_REQUEST['entid']."'");
	/* FIM - Tratamento dos documentos */
	$_SESSION['rehuf_var'] = array("esuid"         => $dadosestruturaunidadeid,
								   "entid"         => $_REQUEST['entid'],
								   "pregaosemabas" => true,
								   "preid"         =>  $_REQUEST['preid']);
}


if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

if($_REQUEST['exec_function']){
	if (function_exists($_REQUEST['exec_function'])){
		$_REQUEST['exec_function']($_REQUEST);
	}	
	exit;
}

if($_REQUEST['preid']) {
	/*
	 * Verificando se o pregão existe
	 */
	$preid = verificaPregao($_REQUEST['preid']);
	if (!$preid){
		echo "<script>
				alert('Pregão selecionado não existe');
				window.location='?modulo=inicio&acao=C';
			 </script>";
		exit;	
	}
	$_SESSION['rehuf_var']['preid'] = $_REQUEST['preid'];
}

/*
 * Validando a variavel de sessão do pregão
 */
if(!$_SESSION['rehuf_var']['preid']) {
	echo "<script>
			alert('Não existe pregão selecionado');
			window.location='?modulo=inicio&acao=C';
			window.close();
		  </script>";
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
require APPRAIZ . "www/includes/webservice/pj.php";
echo '<br>';
if(!$_SESSION['rehuf_var']['pregaosemabas']) {
	/* montando o menu */
	$menurehuf = carregardadosmenurehuf();
	$t = count($menurehuf);
	// apagando as abas apos pregão
	for($i=8;$i<$t;$i++) {
		 unset($menurehuf[$i]);
	}
	
	if($permissoes['hospfederal']) $menurehuf[2] = array("id" => 3, "descricao" => "Itens do pregão", "link" => "/rehuf/rehuf.php?modulo=pregao/preenchimentoPregao&acao=A&preid=".$_SESSION['rehuf_var']['preid']);
	else $menurehuf[8] = array("id" => 8, "descricao" => "Itens do pregão", "link" => "/rehuf/rehuf.php?modulo=pregao/preenchimentoPregao&acao=A&preid=".$_SESSION['rehuf_var']['preid']);
	
	echo montarAbasArray($menurehuf, $_SERVER['REQUEST_URI']);
} else {
	$db->cria_aba( $abacod_tela, $url, '' );
}


?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script>
function imprimirExtrato(){
	window.open('rehuf.php?modulo=pregao/popupImprimirExtrato&acao=A', 'imprimirExtrato', "scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen");			
}
function popupAssociaItemPregao(){
	var j = window.open('rehuf.php?modulo=pregao/popupItemPreenchimento&acao=A', 'itensPregao', 'scrollbars=no,height=550,width=930,status=no,toolbar=no,menubar=no,location=no');
	j.focus();					
}
function carregaNomeFornecedor(obj) {
	if(obj.value != "") {
		if(validarCnpj(obj.value)) {
			var comp = new dCNPJ();
			comp.buscarDados(obj.value);
			if(comp.dados.no_empresarial_rf != "") {
				document.getElementById('nomefornecedor'+obj.id.substr(14)).value=comp.dados.no_empresarial_rf;
			} else {
				alert('CNPJ não encontrado na base de dados da Receita Federal');
				obj.value="";
			}
		} else {
			alert('Formato do CNPJ incorreto');
			obj.value="";
		}
	} else {
		document.getElementById('nomefornecedor'+obj.id.substr(14)).value="";
	}
}

function validaFormularioItens() {
	for(i=0;i < $('formulario').elements.length;i++) {
		// validando a data
		if($('formulario').elements[i].id.substr(0,8) == 'vigencia') {
			if(!validaData($('formulario').elements[i]) && $('formulario').elements[i].value != "") {
				alert('"Data Vigência" esta no formato incorreto. Erro na linha: \n CAT/MAT: '+$('formulario').elements[i].parentNode.parentNode.cells[0].innerHTML+'\n Descrição/Apresentação: '+$('formulario').elements[i].parentNode.parentNode.cells[1].title);
				$('formulario').elements[i].focus();
				return false;
			}
		}
		
	}
	return true;
}
function deletarItem(ippid) {
	var conf = confirm("Deseja realmente deletar este item?");
	if(conf) {
		ajaxatualizar('exec_function=deletaItem&ippid='+ippid,'');
		ajaxatualizar('exec_function=listaPreenchimentoItens','listaItemPreenchido');
	}
	redimencionarBodyData();
}
function redimencionarBodyData() {
	if(document.getElementById('bodydata')) {
		var myHeight;
		if( typeof( window.innerWidth ) == 'number' ) {
		   	//Non-IE
		   	myHeight = window.innerHeight;
		   	document.getElementById('bodydata').style.height = myHeight - 480; 
		} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
		   	//IE 6+ in 'standards compliant mode'
		   	myHeight = document.documentElement.clientHeight;
		   	document.getElementById('bodydata').style.height = myHeight - 700;
		} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
		   	//IE 4 compatible
		   	myHeight = document.body.clientHeight;
		   	document.getElementById('bodydata').style.height = 0;
		}
	}
}

</script>
<?
$titulo_modulo = "Preenchimento do Hospital";
monta_titulo( $titulo_modulo, '&nbsp;' );
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid'],$_SESSION['rehuf_var']['preid']);
?>
<table class="tabela" cellspacing="1" cellpadding="2" border="0" align="center" style="background: #F5F5F5">
<tr><td style="background: #FCFDDB;"><b>Como preencher?</b> Clique no botão  para adicionar itens de acordo com a sua necessidade. Posteriormente, preencha o formulário e salve as informações.<br />
<b>Obs:</b> Salvem as informações na medida em que forem preenchendo o formulário. <b>Você não poderá ficar mais de 20 minutos sem salvar os dados.</b></td></tr>
</table>
<?
montaSubTitulo('Preenchimento de Itens');
?>
<div id="listaItemPreenchido"></div>
<script>
// carregando a lista de preenchimento de itens
document.observe("dom:loaded", function() {
	ajaxatualizar('exec_function=listaPreenchimentoItens','listaItemPreenchido');
	redimencionarBodyData();
});

</script>
<?
if($_SESSION['rehuf_var']['pregaosemabas']) {
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2" align="center">
			<input type="button" name="nt" value="Voltar" onclick="window.location='?modulo=pregao/hospitaisParticipantes&acao=A';"/>
	</td>
</tr>
</table>
<?
}
?>