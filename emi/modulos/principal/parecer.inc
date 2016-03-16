<?php

switch ($_REQUEST["requisicao"]) {
	case "salvar" :
		$emi->salvarParecer( $_REQUEST );	
	break;
	
	case "excluirparecer" :
		$emi->excluirParecer( $_REQUEST["prcid"] );
	break;
	
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
print "<br/>";

$arMnuid = array();

if( !emiPossuiPerfil(EMI_PERFIL_ADMINISTRADOR) ){
	$arMnuid = array(5887);
}

$db->cria_aba($abacod_tela,$url,$parametros,$arMnuid);
monta_titulo("Parecer", "");

if( $_SESSION["emi"]["emeidPai"] ){
	
	$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"] );
	
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form method="post" action="" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="salvar">
	<input type="hidden" name="emeid" id="emeid" value="<?=$_SESSION["emi"]["emeidPai"];?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulodireita" width="190px">Parecer:</td>
			<td>
				<?php echo campo_textarea( 'prcparecer', '', $emiSomenteLeitura, '', 80, 10, 1000, '', 0, '' ); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Salvar" onclick="emiValidaParecer();" style="cursor: pointer;" <?=$emiDisabled; ?>/>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr>
			<td class="subtitulocentro" colspan="2">Histórico de Pareceres</td>
		</tr>
	</table>
</form>
<?php 
	
	$sql = "SELECT
				prcparecer,
				to_char( prcdataparecer, 'DD/MM/YYYY' ) as data,
				usunome 
			FROM 
				emi.emparecer ep
			INNER JOIN
				seguranca.usuario su ON ep.usucpf = su.usucpf 
			WHERE 
				emeid = {$_SESSION["emi"]["emeidPai"]}
			ORDER BY
				prcid DESC";
	
	$cabecalho = array( "Parecer", "Data", "Inserido Por" );
			
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','center', '', '', '', '' );
	
?>
