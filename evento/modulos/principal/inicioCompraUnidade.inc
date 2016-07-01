<?php
if( $_POST['ajaxUnidade']){
	 $_SESSION['unidade'] = $_POST['ajaxUnidade'];  
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>'; 
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Lista de Unidades", 'Selecione uma Unidade' );
?>
<!-- biblioteca javascript local -->
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" >
function selecionaUnidade( id ){
	var req = new Ajax.Request(window.location.href, {
	        method:     'post',
	        parameters: '&ajaxUnidade=' + id,							         
	        onComplete: function (res) { 
				window.location.href = 'evento.php?modulo=principal/listaProcessoUnidade&acao=A';
			}
		});
}
</script>
<!-- corpo da página -->
<?php
$codicaoUasg = "";
$msg = "";
$arUnidadesVisiveis = verificaUnidadesPermitidadas();

if( $arUnidadesVisiveis[0] && !$arUnidadesVisiveis[1] ){
	echo "<script>selecionaUnidade( ".$arUnidadesVisiveis[0]." );</script>";
	die;
}

if(is_array($arUnidadesVisiveis) && $arUnidadesVisiveis[0]){
	$codicaoUasg = " where u.usgid in (". implode(",", $arUnidadesVisiveis).") ";
} elseif(strlen($arUnidadesVisiveis) > 2 ) {
	$msg = $arUnidadesVisiveis;
}

if(!$msg){
	$sql = "select
				'<center><a href=\"javascript: selecionaUnidade('|| u.usgid ||');\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a></center>' as acao,	
				u.usgcod  as cod,
				u.usggestaouasg,
				u.usgdsc as unidade  
			from evento.uasg as u $codicaoUasg ";
	$cabecalho = array("Ação","Código UASG / Entidade","Código Gestão","Unidade");
	$db->monta_lista($sql,$cabecalho,200,10,'N','center','');
} else {
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
    	<td class ="SubTit,0uloCentro" align="center"><font color="red"><?php echo $msg ?></font></td>
    </tr>
</table>
<?php	
}
?>