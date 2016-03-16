<?php
/*
 * Validando as veriaveis de sessão do REHUF
 */
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}
/*
 * Validando a variavel de sessão do pregão
 */
if(!$_SESSION['rehuf_var']['preid']) {
	echo "<script>
			alert('Não existe pregão selecionado');
			window.opener.location='?modulo=inicio&acao=C';
			window.close();
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

$hupid = pegaPreenchimento($_SESSION['rehuf_var']['entid'], $_SESSION['rehuf_var']['preid']);
$igpidArr = (array) carregaItemSelecionado ($hupid);
$dados	  = carregaItem($_SESSION['rehuf_var']['preid']);
?>
<html>
<head>
<meta http-equiv="Pragma" content="no-cache">
<title>Incluir Itens</title>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
</head>
<body leftmargin="0" topmargin="7" bottommargin="7" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<form name="formulario" id="formulario" method="post">
<table cellspacing="1" cellpadding="2" border="0" align="center" width="100%">
<tr>
<td class="title" colspan="7" style="background: #CCCCCC;"><center><b>Selecione os itens para preenchimento</b></center></td>
</tr>
<tr>
<td class="title" style="background: #DCDCDC;" width="5%"><center><b><input title="Marcar Todos" type="checkbox" name="chektodos" id="checktodos1" onclick="marcarTodos(this);"></b></center></td>
<td class="title" style="background: #DCDCDC;" width="10%"><center><b>CAT MAT</b></center></td>
<td class="title" style="background: #DCDCDC;" width="10%"><center><b>Lote</b></center></td>
<td class="title" style="background: #DCDCDC;" width="10%"><center><b>Item</b></center></td>
<td class="title" style="background: #DCDCDC;" width="10%"><center><b>Código SUS</b></center></td>
<td class="title" style="background: #DCDCDC;" width="40%"><center><b>Descrição</b></center></td>			
<td class="title" style="background: #DCDCDC;" width="5%"><center><b>Apresentação</b></center></td>			
</tr>
</table>
<div style="border: 1px solid #999999; width:100%; height: 470px; overflow: auto;">
<table cellspacing="1" cellpadding="2" border="0" align="center" width="100%">
<?
// Verificando se todos os itens estão marcados, caso sim, desabilitar checkbox de marcar todos
$todosItensMarcados = true;

if($dados[0]) {
	foreach($dados as $dado):
		$color    = $color == '#DFDFDF' ? '#FFFFFF' : '#DFDFDF';
		$disabled = in_array($dado['igpid'], $igpidArr) ? 'disabled="disabled" checked="checked"' : '';
		if(!$disabled) {
			$todosItensMarcados = false;
		}
		if($dado['gruid'] != $gruidant) {
			$color = '';
			$gruidant = $dado['gruid']; 
		?>	
			<tr>
				<td class="title" style="background: #CCCCCC;" colspan="7"><strong>Grupo:</strong> <? echo $dado['grunome']; ?></td>
			</tr>
		<? 
		}
	?>	
	<tr>
		<td class="title" style="background: <?=$color?>;" width="5%"><center><b><input title="Marcar Todos" type="checkbox" onclick="insereItem(this);" <?=$disabled ?> name="item" value='<?=$dado['igpid'] ?>'></b></center></td>
		<td class="title" style="background: <?=$color?>;" width="10%"><center><?=$dado['itecatmat'] ?></center></td>
		<td class="title" style="background: <?=$color?>;" width="10%"><center><?=$dado['itlabrev'] ?></center></td>
		<td class="title" style="background: <?=$color?>;" width="10%"><center><?=$dado['iteitem'] ?></center></td>
		<td class="title" style="background: <?=$color?>;" width="10%"><center><?=$dado['itecodsus'] ?></center></td>
		<td class="title" style="background: <?=$color?>;" width="40%"><?=$dado['itedescricao'] ?></td>			
		<td class="title" style="background: <?=$color?>;" width="7%"><?=$dado['iteapresentacao']?></td>			
	</tr>
	<? 
	endforeach;
} else {
	if($_SESSION['rehuf_var']['hupid']) {
		deletaHUParticipante($_SESSION['rehuf_var']['hupid']);
	}
	?>
	<tr>
		<td class="title" style="background: <?=$color?>;" colspan="7">Não existem itens cadastrados no pregão.</td>
	</tr>
	<?
}
?>	
</table>
</div>
<table cellspacing="1" cellpadding="2" border="0" align="center" width="100%">
<tr>
<td class="title" style="background: #DCDCDC;" width="5%"><center><b><input title="Marcar Todos" type="checkbox" name="chektodos" id="checktodos2" onclick="marcarTodos(this);"></b></center></td>
<td class="title" style="background: #DCDCDC;" width="95%" align="center" id="tdfechar"><input type="button" name="fechar" value="Fechar" onclick="window.close();" id="btnfechar"></td>			
</tr>
</table>			  
</form> 				
</body>
<script>
function insereItem (obj){
	if(obj.checked && !obj.disabled) {
		obj.disabled = true;
		ajaxatualizar('exec_function=cadastraItem&igpid=' + obj.value);
		ajaxatualizar('exec_function=listaPreenchimentoItens','listaItemPreenchido',true);
		window.opener.redimencionarBodyData();
	}
}

function inserirTodos(igps) {
	ajaxatualizar('exec_function=cadastraTodosItem' + igps);
	ajaxatualizar('exec_function=listaPreenchimentoItens','listaItemPreenchido',true);
}

function marcarTodos(obj) {
	var form  = document.getElementById('formulario');
	
	document.getElementById('checktodos1').checked = true;
	document.getElementById('checktodos2').checked = true;
	
	// desabilitando "marcar todos"
	document.getElementById('checktodos1').disabled = true;
	document.getElementById('checktodos2').disabled = true;
	document.getElementById('tdfechar').innerHTML = "<font size=1><b>Aguarde, o sistema esta inserindo os ítens...</b></font>";
	var igps = "";
	for(var i=0;i<form.elements.length;i++) {
		if(form.elements[i].type=='checkbox') {
			if(form.elements[i].checked == false) {
				form.elements[i].checked = true;
				igps += "&igpid[]="+form.elements[i].value;
				form.elements[i].disabled = true;
			}
		}
	}
	inserirTodos(igps);
	document.getElementById('tdfechar').innerHTML = "<input type=\"button\" name=\"fechar\" value=\"Fechar\" onclick=\"window.close();\" id=\"btnfechar\">";
}
<?
if($todosItensMarcados) {
	echo "document.getElementById('checktodos1').checked=true;document.getElementById('checktodos1').disabled=true;";
	echo "document.getElementById('checktodos2').checked=true;document.getElementById('checktodos2').disabled=true;";
}
?>
</script>
</html>
