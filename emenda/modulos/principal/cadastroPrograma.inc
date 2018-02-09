<?php
if ($_REQUEST['mostraAcoesCadastradas']) {
	header('content-type: text/html; charset=ISO-8859-1');
	listaAcaoPrograma($_POST['proid']);
	exit;
}
if ($_REQUEST['excluiAcoesCadastradas']) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiAcoesCadastradas($_POST['acaid']);
	exit;
}

include_once( APPRAIZ. "emenda/classes/ProgramaAcao.class.inc" );
include_once( APPRAIZ. "emenda/classes/Programa.class.inc" );

if($_GET["proid"]){
	$_SESSION['emenda']['proid'] = $_GET["proid"];
	$proid = $_SESSION['emenda']['proid'];
}
else
	$proid = $_SESSION['emenda']['proid'];

$obPrograma = new Programa( $proid );

if( isset( $_POST['acao'] ) ){
	$sql = " select proid from emenda.programa where pronome = '{$_POST["pronome"]}' and prostatus = 'I' ";	
	$proid = $_GET['proid'] ? $_GET['proid'] : $db->pegaUm( $sql );
	$_SESSION['emenda']['proid'] = $_GET['proid'] ? $_GET['proid'] : $db->pegaUm( $sql );
	
	$arDados = array( 'proid', 'tpeid', 'pronome', 'proinstrucoes', 'procontrapartidaminima', 'provigenciainicial', 'provigenciafinal' );	
	$obPrograma->popularObjeto( $arDados );
	$obPrograma->provigenciainicial = formata_data_sql($_POST['provigenciainicial']);
	$obPrograma->provigenciafinal = formata_data_sql($_POST['provigenciafinal']);
	$obPrograma->prostatus = 'A';
	$obPrograma->procontrapartidaminima = number_format($_POST['procontrapartidaminima'], 2);
	if( $obPrograma->salvar() ){
		$obPrograma->commit();
		$erro = "";
		if($_POST['acaoVinculada'] == "1"){
			$erro = "alert('Não há nenhuma ação vinculada até o momento!');";
		}
		echo "<script>
				alert('Operação realizada com sucesso');
        		$erro
		      	window.location.href='emenda.php?modulo=principal/cadastroPrograma&acao=C&proid={$obPrograma->proid}';
		     </script>";
		die;
	}
	else{
		$obPrograma->rollback();	
	}
}

$proinstrucoes = $obPrograma->proinstrucoes;
$provigenciainicial = formata_data($obPrograma->provigenciainicial);
$provigenciafinal = formata_data($obPrograma->provigenciafinal);

unset($_SESSION['InsereAcao']);
unset($_SESSION['emenda']['InsereAcaoPrograma']);
unset($_SESSION['excluiAcaoPrograma']);
carregaAcaoProgramaBanco($proid);
$_SESSION['emenda']['InsereAcaoPrograma'] ? $acaoVinc = "" : $acaoVinc = "1";

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( "Programa de Trabalho", "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );

?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script	src="/includes/calendario.js"></script>
<body>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
<input type="hidden" id="proid" name="proid" value="<?=$proid ?>">
<input type="hidden" id="acaoVinculada" name="acaoVinculada" value="<?=$acaoVinc; ?>">
<input type="hidden" id="acao" name="acao" value="">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Programa:</td>
		<td><?=campo_texto( 'pronome', 'S', 'S', 'Nome do Programa', 40, 500, '', '','','','','id="pronome"', '', $obPrograma->pronome ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nível de Ensino:</td>
		<?
		$sql = "SELECT 
				  tpeid as codigo,
				  tpedesc as descricao
				FROM 
				  public.tipoensino
				WHERE
					tpestatus = 'A'";
		?>
		<td><?= $db->monta_combo("tpeid",$sql, 'S','-- Selecione --','', '', '',215,'S','tpeid', '', $obPrograma->tpeid, 'Nível de Ensino');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Vigência:</td>
		<td><?=campo_data('provigenciainicial', 'S','S','Vigência Inicial','','','');?> a
						<?=campo_data('provigenciafinal', 'S','S','Vigência Final','','',''); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Instruções:</td>
		<td><?= campo_textarea('proinstrucoes','N', 'S', '', 80, 05, 4000,'','','','','Instruções');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Ações Vinculadas:</td>
		<td><img src="/imagens/gif_inclui.gif " id="add" border=0 alt="Ir" title="Adicionar Item" onclick="inserirAcao();"><span style="cursor: pointer" onclick="inserirAcao();"> Vincular Ações ao Programa</span>
			<div id="mostraAcao"><? 	listaAcaoPrograma($proid);	?></div>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Valor da contra-parte mínimo(%):</td>
		<td><?=campo_texto( 'procontrapartidaminima', 'N', 'S', 'Valor da contra-parte mínimo', 40, 6, '[#]#,##', '','','','',"id='procontrapartidaminima'", '', $obPrograma->procontrapartidaminima, '' );?></td>
	</tr>
	<tr>
		<th align="center" colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="validaForm1();">
			<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="voltar();">
		</th>
	</tr>
</table>
</form>
<div id="erro"></div>
<script type="text/javascript">
	function validaForm1(){		
		var nomeform 		= 'formulario';
		var submeterForm 	= false;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[1] 			= "tpeid";
		campos[0] 			= "pronome";
		campos[2] 			= "provigenciainicial";
		campos[3] 			= "provigenciafinal";
						 
		tiposDeCampos[1] 	= "select";
		tiposDeCampos[0] 	= "texto";
		tiposDeCampos[2] 	= "texto";
		tiposDeCampos[3] 	= "texto";
		
		if( ($('provigenciainicial').value != "") && ($('provigenciafinal').value != "") ){
			if( !validaDataMaior($('provigenciainicial'), $('provigenciafinal')) ){
				alert("A vigência inicial não pode ser maior que vigência final!");
					$('provigenciainicial').focus();
				return false;
			}else if(!validaData($('provigenciainicial') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('provigenciainicial').focus();
				return false;
			}else if(!validaData($('provigenciafinal') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('provigenciafinal').focus();
				return false;
			}
		}
		
		if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
			/*if($('acaoVinculada').value != ""){
				alert('Não há nenhuma ação vinculada até o momento!');
			}*/
			document.formulario.submit();
		}		
	}
	function voltar(){
		window.location.href = 'emenda.php?modulo=principal/listaPrograma&acao=A';
	}
	
	function inserirAcao(){
		window.open('emenda.php?modulo=principal/popInsereAcao&acao=A&proid='+$('proid').value,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=400,height=500');
	}
	
	function mostraAcoesCadastradas(proid){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/cadastroPrograma&acao=A', {
			        method:     'post',
			        parameters: '&mostraAcoesCadastradas=true&proid='+proid,
			        asynchronous: false,
			        onComplete: function (res){
						$('mostraAcao').innerHTML = res.responseText;
						if( res.responseText.indexOf('Nenhuma ação cadastrada!') != -1 ){
							$('acaoVinculada').value = '1';
						}else{
							$('acaoVinculada').value = '';
						}
			        }
			  });
	}
	function excluiPrograma(acaid){
		if(confirm("Tem certeza que deseja excluir este registro?")){
			var myajax = new Ajax.Request('emenda.php?modulo=principal/cadastroPrograma&acao=A', {
			        method:     'post',
			        parameters: '&excluiAcoesCadastradas=true&acaid='+acaid,
			        asynchronous: false,
			        onComplete: function (res){
						//alert(res.responseText);
						//$('erro').innerHTML = res.responseText;
						mostraAcoesCadastradas($('proid').value);
			        }
			  });
		}
	}
	
	/*function formataValor(valor){
		if(valor.value.indexOf('.') == -1){
			if(valor.value.length > 5){
				valor.value = valor.value.substr(0, 5);
			}
		}
	}
	
	function verificaOcorrenciaPonto(valor){
		if( valor.value.indexOf('.') != valor.value.lastIndexOf('.') ){
			valor.value = valor.value.substr(0, valor.value.lastIndexOf('.') ); 		
		}
		
		if(valor.value.length == 6){
			if(valor.value.indexOf('.') == -1){
				valor.value = valor.value.substr(0, 5) + '.' + valor.value.substr(5, 6);
			}
		}
	}
	
	function formataDecimal(e) {	
		if(window.event) {
	    	//Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados.
	        key = e.keyCode;
	    }
	    else if(e.which) {
	    	//Netscape 
	        key = e.which;
	    }
	    if( ( (key > 47) && (key < 58) ) || (key == 46) || (key == 8) || (key == 9) ){
			return true;
	    }else{
	    	return false;
	    }
	
	}*/
</script>
</body>
</html>