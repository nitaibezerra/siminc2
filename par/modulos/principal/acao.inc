<?php 
//include_once APPRAIZ . "includes/workflow.php";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';


$titulo_modulo = "Responder dados do indicador";
//teste
monta_titulo( $titulo_modulo, '' );

if($_GET['tipoDiagnostico'] == 'arvore'){
	$tipoDiagnostico = 'arvore';
	$checkArvore = "checked=\"checked\"";	
} else {
	$tipoDiagnostico = 'listaAgrupada';
	$checkListaAgrupada = "checked=\"checked\"";
}
//if($_POST){
//	ver($_POST,d);
//}

$obPontuacao = new PontuacaoControle();
$arDados = $obPontuacao->carregaPontuacao($_GET['indid']);

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript">
<!--

jQuery.noConflict();

jQuery.metadata.setType("attr", "validate");


jQuery(document).ready(function(){

    var perfil_leitura = new Array();
    var perfil_admin   = new Array("crtid","ptojustificativa");

	var i = 1;
	jQuery('form.elements').each(function(){
		//if(i == 1){
			//alert(perfil_admin+'\n'+$(this).attr('name')+'\n'+$.inArray($(this).attr('name'), perfil_admin))
			if(jQuery.inArray($(this).attr('name'), perfil_leitura) < 0){
				jQuery(this).attr('disabled', true);
			}
		//}
		i++;
	});
	//alert($(form.elements).length);
	//alert($(form:input:visible:not([type=submit]):not([type=button]):not([type=reset])));
	
});

//-->
</script>
<form method="post" name="formulario" id="formulario" action="par.php?modulo=principal/pontuacao&acao=A" class="formulario" >
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="left" colspan="2"><strong>Localização da Ação</strong></td>
	</tr>
	<tr>
		<td width="200px" class="SubTituloDireita">Dimensão:</td>
		<td><?php echo $arDados[0]['codigodimensao'].'. '.$arDados[0]['descricaodimensao']; ?></td>
	<tr>
		<td class="SubTituloDireita">Área:</td>
		<td><?php echo $arDados[0]['codigoarea'].'. '.$arDados[0]['descricaoarea']; ?></td>
	<tr>
		<td class="SubTituloDireita">Indicador:</td>
		<td><?php echo $arDados[0]['codigoindicador'].'. '.$arDados[0]['descricaoindicador']; ?></td>
	</tr>
	<tr>
		<td align="left" colspan="2"><strong>Dados da Ação</strong></td>
	</tr>
	<tr>
		<td width="200px" class="SubTituloDireita"><label for="acidsc">Descrição Ação:</label></td>
		<td><?php echo campo_texto( 'acidsc', 'S', $habilitado, 'Descrição Ação', 50, 50, '', '','','','','id="acidsc"'); ?></td>
	</tr>
	<tr>
		<td width="200px" class="SubTituloDireita"><label for="acinomeresponsavel">Nome do Responsável:</label></td>
		<td><?php echo campo_texto( 'acinomeresponsavel', 'S', $habilitado, 'Nome do Responsável', 50, 50, '', '','','','','id="acinomeresponsavel"'); ?></td>
	</tr>
	<tr>
		<td width="200px" class="SubTituloDireita"><label for="acicargoresponsavel">Cargo do Responsável:</label></td>
		<td><?php echo campo_texto( 'acicargoresponsavel', 'S', $habilitado, 'Cargo do Responsável', 50, 50, '', '','','','','id="acicargoresponsavel"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><label for="acidemandapotencial">Demandas Potenciais:</label></td>
		<td><?php 
				$acidemandapotencial = ''; 
				echo campo_textarea( 'acidemandapotencial', 'S', 'S', 'Demandas Potenciais', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Demandas Potenciais' ); 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><label for="aciresultadoesperado">Resultado Esperado:</label></td>
		<td><?php 
				$aciresultadoesperado = ''; 
				echo campo_textarea( 'aciresultadoesperado', 'S', 'S', 'Resultado Esperado', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Resultado Esperado' );
			?>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#dcdcdc">
		<td style="text-align:center">
			<input type="button" value="Salvar" />
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
		<td width="90px"><strong>Lista de subações cadastradas nesta ação</strong></td>
	</tr>
</table>
<div id="divLista">
<?php 
// CABEÇALHO da lista
$arCabecalho = array("Ação","Subações","Histórico");
		
// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
$arParamCol[0] = array("type" => Lista::TYPESTRING, 
					   "style" => "width:1200px;",
					   //"html" => "{dimcod}.{arecod}.{indcod} - {inddsc}",
					   "align" => "left");
$arParamCol[1] = array("type" => Lista::TYPESTRING, 
					   //"style" => "color:red;",
					   //"html" => "<img src=\"/imagens/fluxodoc.gif\" border=0 title=\"\" style=\"cursor:pointer;\" onclick=\"\">",
					   "align" => "center");

$acao = "<center>
		   <img src=\"/imagens/alterar.gif\" border=0 title=\"\" style=\"cursor:pointer;\" onclick=\"\">
		 <center>";
		
// ARRAY de parametros de configuração da tabela
$arConfig = array("style" => "width:95%;",
				  "totalLinha" => false,
				  "totalRegistro" => false);

$sql = "SELECT  d.dimid,
						d.dimcod,
						d.dimdsc,
				        i.inddsc
			FROM par.dimensao d
			INNER JOIN par.area a ON a.dimid = d.dimid
			INNER JOIN par.indicador i ON i.areid = a.areid
			where d.itrid = 1
			order by dimcod, arecod, indcod limit 10";
		
$arDados = $db->carregar($sql);
$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setCorpo( $arDados, $arParamCol );
$oLista->setAcao( $acao );
$oLista->show();

?>
</div>