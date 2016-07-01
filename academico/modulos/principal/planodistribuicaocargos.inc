<?php
pegaSessoes(1,0,0);

// instancia o objeto do módulo
$academico = new academico();
$autoriazacaoconcursos = new autoriazacaoconcursos();

// Define as variáveis da tela
$orgid 	 	 = $_SESSION["academico"]["orgid"];
$prtid 	 	 = $_SESSION["academico"]["prtid"];
$entidcampus = $_SESSION["academico"]["entidcampus"];
$orgdesc 	 = ($orgid == 1) ? "Educação Superior" : "Educação Profissional";

$_SESSION["academico"]["carga_cargos"] =  $_REQUEST["carga"]?  $_REQUEST["carga"] : $_SESSION["academico"]["carga_cargos"];
$carga = $_SESSION["academico"]["carga_cargos"];

// carrega os campus de acordo com a unidade selecionada
if ( $_REQUEST['carga'] ){
	$academico->listacampusentidade( $_REQUEST['carga'], $orgid );
	$academico->listareitoriaentidade( $_REQUEST["carga"], $orgid);
	die;
}

// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

// Monta as abas da tela
echo montarAbasArray(carregardadosmenuacademico('dadosPortaria'), '/academico/academico.php?modulo=principal/planodistribuicaocargos&acao=' . $_REQUEST['acao']);		
//$db->cria_aba($abacod_tela,$url,'');

// cria o titulo da tela
monta_titulo( $orgdesc, "Unidades");
// cria o cabeçalho padrão do módulo
//if (isset($prtid) && isset($entidcampus))	
//	echo $autoriazacaoconcursos->cabecalho( $prtid, $entidcampus );
if (isset($prtid))	
	echo $autoriazacaoconcursos->cabecalho_ini($prtid);

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	var params;
		
	function formatarParametros(){
	    params = Form.serialize($('pesquisar'));	    
	}
	
	function desabilitarConteudo( id ){
		var url = 'academico.php?modulo=principal/planodistribuicaocargos&acao=C&carga='+id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				});
		}
	}
	
	function insereUnidades(){
		window.open('academico.php?modulo=principal/inserir_unidades&acao=A','Unidades', 'width=500, height=400, scrollbars=yes');
	}
	
</script>
<form name="formulario" id="pesquisar" method="post" action="">
	<input type="hidden" name="pesquisa" value="1"/>
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr>
			<td class="SubTituloDireita">Listar por</td>
			<td>
				<input type="radio" name="agrupador" value="unidade" <?php if ( $_REQUEST["agrupador"] == "unidade" ){ echo 'checked="checked"'; } ?>> Unidades
				<input type="radio" name="agrupador" value="campus" <?php if ( $_REQUEST["agrupador"] == "campus" ){ echo 'checked="checked"'; } ?>> Campus
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="submit" value="Listar"/>
			</td>
		</tr>
	</table>
</form>
<?php

// Monta a lista
if ( $_REQUEST["pesquisa"] ){
	
	$_REQUEST["agrupador"] == "unidade" ? $academico->listaunidades( $orgid ) 
										: $academico->listacampus( $orgid ); 
	
}else{
	$academico->listaunidades( $orgid );
}


?>
<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
	<tr>
		<td>
			<input type="button" value="Inserir Unidades" onclick="insereUnidades();" style="cursor:pointer;" <? echo $disabled; ?>/>
		</td>
	</tr>
</table>
<div id="loading"></div>
<script type="text/javascript">
<!--
//mantendo a arvore espandida
	<?
	if(isset($carga) && ($carga != '') ){
	?>
	abreconteudo('academico.php?modulo=principal/planodistribuicaocargos&acao=C&subAcao=gravarCarga&carga=<?=$carga?>&params=' + params, <?=$carga?>);
	<?
	}
	?>	
//-->
</script>

