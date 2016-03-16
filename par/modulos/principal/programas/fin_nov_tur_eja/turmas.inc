<?php 

$_SESSION['par']['cod_inep'] = $_REQUEST['cod_inep'] ? $_REQUEST['cod_inep'] : $_SESSION['par']['cod_inep'];

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";


$sql = "SELECT DISTINCT
			no_entidade as escola
		FROM 
			".SCHEMAEDUCACENSO.".tab_entidade
		WHERE
			cod_orgao_regional_inep = '".$_REQUEST['cod_inep']."'";
$linha1 = $db->pegaUm($sql); 
if($_SESSION['par']['muncod']){
	
	$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	$linha2 = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
	
}else{
	
	$linha2 = $_SESSION['par']['estuf'];
}
monta_titulo($linha1, $linha2);

echo "<br/>"; 
$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo('Turmas',''); 
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
	});
</script>
<center>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td width="90%" align="center">
				Conteudo Turmas
			</td>
		</tr>
	</table>
	<p>
	<input type="button" value="Voltar" onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/planoAcoes&acao=A'" />
	<input type="button" value="Sair" 	onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/lista_escolas&acao=A'" />
</center>
