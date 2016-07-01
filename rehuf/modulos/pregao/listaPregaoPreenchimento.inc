<?php
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

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

// pregão sem abas
$_SESSION['rehuf_var']['pregaosemabas'] = false;
/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), $_SERVER['REQUEST_URI']);

$titulo_modulo = "Listar Pregões";
monta_titulo( $titulo_modulo, '&nbsp;' );
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
?>
<table class="tabela" cellspacing="1" cellpadding="2" border="0" align="center" style="background: #F5F5F5">
<tr><td style="background: #FCFDDB;"><b>Como preencher?</b> Clique no botão <img src="/imagens/alterar.gif" border="0"> da Lista de Pregões para acessar a página onde você poderá adicionar os itens dos pregões conforme sua necessidade.</td></tr>
</table>
<?
montaSubTitulo('Filtro');
?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<center style="margin: 10px;">
	<input type="radio" value="1" name="pregao_selecionado" id="pregao_meus" onclick="filtrarPregao(this)">
	<label for="pregao_meus">Pregões que participo</label>
	<input type="radio" value="2" name="pregao_selecionado" id="pregao_todos" onclick="filtrarPregao(this)" checked>
	<label for="pregao_todos">Todos os pregões</label>
</center>
<? 
montaSubTitulo('Lista de Pregões');
?>
<div id="listapregao"></div>
<script>
function filtrarPregao(obj) {
	if(obj.checked) {
		switch(obj.value) {
			case '1':
				ajaxatualizar('exec_function=listaPregao&meuspregoes=true','listapregao');
				break;
			case '2':
				ajaxatualizar('exec_function=listaPregao','listapregao');
				break;
		}
	}
}
ajaxatualizar('exec_function=listaPregao','listapregao');
</script>