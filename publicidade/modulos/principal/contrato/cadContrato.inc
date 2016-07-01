<?php
// incluindo as classes necessárias
$oContrato = new Contrato();
$oFornecedor = new Fornecedor();
$oContratoAditivo = new ContratoAditivo();

if(!empty($_GET['id']) && empty($_SESSION['cttid'])){
	$_SESSION['cttid'] = $_GET['id'];	
}
else if(!empty($_SESSION['cttid']) && empty($_GET['id'])){
	$_GET['id'] = $_SESSION['cttid'];
}


//caso tenha postado o formulário
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se já existe o registro
	$repetido = $oContrato->verifica_unico($_POST);
	
	//verifica as datas
	$erroData = $oContrato->verifica_data($_POST);
	
	//caso não encontre repetido, salva
	if(!$repetido && empty($erroData)){

		$resultado = $oContrato->salvar($_POST);
		
		if($resultado[0]){
			
			//caso exista aditivo, exclui
			if($_POST['cttaditivo'] == 'N'){
				$oContratoAditivo->excluir_aditivos_filhos($resultado[1]);
			}
			
			direcionar('?modulo=principal/contrato/cadHonorario&acao=A&id='.$resultado[1],'');
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/contrato/cadHonorario&acao=A&id='.$resultado[1],$msg);
		}
	}
	else if($repetido){
		$msg = utf8_decode('Contrato Já Cadastrado!');
		alerta($msg);
		extract($_POST);
	}
	else if(!empty($erroData)){
		if($erroData == 'datamenor'){
			$msg = utf8_decode('Data Inválida!');
			alerta($msg);
		}
		else if($erroData == 'anoinicialinvalido' || $erroData == 'anofinalinvalido'){
			$msg = utf8_decode('A Data Informada é Inválida!');
			alerta($msg);
		}
		$_POST['erroData'] = $erroData;
		extract($_POST);
	}
}
//caso tenha clicado no botão incluir 
else if($_POST['requisicao'] == 'incluir'){
	
	//verifica se já existe o registro
	$repetido = $oContrato->verifica_unico($_POST);
		
	//verifica as datas
	$erroData = $oContrato->verifica_data($_POST);
		
	//caso não exista, grava o registro pai
	if(!$repetido && empty($erroData)){
		$resultado = $oContrato->salvar($_POST);
		
		//se gravar o registro pai, grava o filho
		if($resultado[0]){
			$_POST['cttid'] = $resultado[1];
			
			//valida a vigência do aditivo
			$erroDataAditivo = $oContratoAditivo->verifica_data($_POST);
			
			if(empty($erroDataAditivo)){
				$resultadoAditivo = $oContratoAditivo->salvar($_POST);
		
				if($resultadoAditivo[0]){
					$msg = utf8_decode('Operação realizada com sucesso!');
					direcionar('?modulo=principal/contrato/'.$resultadoAditivo[3].'&acao=A&id='.$resultadoAditivo[1],$msg);
				}
				else if(!$resultadoAditivo[0]){
					$msg = utf8_decode('Operação falhou!');
					direcionar('?modulo=principal/contrato/'.$resultadoAditivo[3].'&acao=A&id='.$resultadoAditivo[1],$msg);
				}
			}
			else if(!empty($erroDataAditivo)){
				$msg = utf8_decode('Período de Vigência Inválido');
				alerta($msg);
				$_POST['erroDataAditivo'] = $erroDataAditivo;
			}
			extract($_POST);
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/contrato/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else if($repetido){
		$msg = utf8_decode('Contrato Já Cadastrado!');
		alerta($msg);
		extract($_POST);
	}
	else if(!empty($erroData)){
		if($erroData == 'datamenor'){
			$msg = utf8_decode('Data Inválida!');
			alerta($msg);
		}
		else if($erroData == 'anoinicialinvalido' || $erroData == 'anofinalinvalido'){
			$msg = utf8_decode('A Data Informada é Inválida!');
			alerta($msg);
		}
		$_POST['erroData'] = $erroData;
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['id'])){
	
	$contrato = $oContrato->carrega_registro_por_id($_GET['id']);
	extract($contrato);
	
	//caso não venha o id do aditivo, verifica qual é o próximo número
	if(empty($_GET['ctaid'])){
		$ctanumaditivo = $oContratoAditivo->pega_ultimo_numero($_GET['id']);
		if(empty($ctanumaditivo)){
			$ctanumaditivo = 1;
		}
		else{
			$ctanumaditivo = (int)$ctanumaditivo;
			$ctanumaditivo++;
		}
	}
	
	//pega a qtd de aditivos do contrato
	$qtdAditivos = $oContratoAditivo->pega_qtd_aditivos($_GET['id']);
	
	//caso o usuário tenha clicado em visualizar
	if($_GET['visualizar'] == 'S'){
		$_SESSION['visualizar'] = $_GET['visualizar'];
	}
}

if(empty($ctanumaditivo) && empty($_GET['ctaid'])){
	$ctanumaditivo = 1;
}

//caso venha o id do aditivo
if(!empty($_GET['ctaid'])){
	$contadi = $oContratoAditivo->carrega_registro_por_id($_GET['ctaid']);
	extract($contadi);
}




// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Dados do Contrato','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript'>
		
	/**
	 * Submete o formulário
	 * @name salvar
	 * @return void
	 */	
	function salvar(){
		$('btnSalvar').disable();
		$('btnIncluir').disable();
		if(validaFormularioCadastro()){
			$('formularioCadastro').submit();
		}
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('forid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Agência" é obrigatório!'));
			alert(msg);
			$('forid').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('cttnumcontrato').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Nº Contrato" é obrigatório!'));
			alert(msg);
			$('cttnumcontrato').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('cttvigenciainicio').getValue() == '' || $('cttvigenciatermino').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Vigência" é obrigatório!'));
			alert(msg);
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		for(var i=0; i<document.formularioCadastro.cttaditivo.length; i++){
			if(document.formularioCadastro.cttaditivo[i].checked){
				var cttaditivo = document.formularioCadastro.cttaditivo[i].value
			}
		}
		if(cttaditivo == 'S' && $('cttid').getValue() == '' &&($('ctavigenciainicio').getValue() == '' || $('ctavigenciatermino').getValue() == '')){
			var msg = decodeURIComponent(escape('O campo "Vigência" é obrigatório!'));
			alert(msg);
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}
		else if(cttaditivo == 'S' && $('cttid').getValue() != '' && $('ctaid').getValue() != '' &&($('ctavigenciainicio').getValue() == '' || $('ctavigenciatermino').getValue() == '')){
			var msg = decodeURIComponent(escape('O campo "Vigência" é obrigatório!'));
			alert(msg);
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}
		else if(cttaditivo == 'S' && $('cttid').getValue() != '' && $('ctaid').getValue() == '' && $('qtdAditivos').getValue() < 1 &&($('ctavigenciainicio').getValue() == '' || $('ctavigenciatermino').getValue() == '')){
			var msg = decodeURIComponent(escape('O campo "Vigência" é obrigatório!'));
			alert(msg);
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}
		
		return true;
	}

	/**
	 * Grava registro filho
	 * @name incluir
	 * @return void
	 */	
	function incluir(){
		$('btnIncluir').disable();
		$('btnSalvar').disable();
		if(validaFormularioInclusao()){
			$('requisicao').setValue('incluir');
			$('formularioCadastro').submit();
		}
	}

	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioInclusao
	 * @return bool
	 */	
	function validaFormularioInclusao(){
	
		if($('cttid').getValue() == ''){

			if($('forid').getValue() == '') {
				var msg = decodeURIComponent(escape('O campo "Agência" é obrigatório!'));
				alert(msg);
				$('forid').focus();
				$('btnSalvar').enable();
				$('btnIncluir').enable();
	        	return false;
			}

			if($('cttnumcontrato').getValue() == '') {
				var msg = decodeURIComponent(escape('O campo "Nº Contrato" é obrigatório!'));
				alert(msg);
				$('cttnumcontrato').focus();
				$('btnSalvar').enable();
				$('btnIncluir').enable();
	        	return false;
			}

			if($('cttvigenciainicio').getValue() == '' || $('cttvigenciatermino').getValue() == '') {
				var msg = decodeURIComponent(escape('O campo "Vigência" é obrigatório!'));
				alert(msg);
				$('btnSalvar').enable();
				$('btnIncluir').enable();
	        	return false;
			}

		}

		for(var i=0; i<document.formularioCadastro.cttaditivo.length; i++){
			if(document.formularioCadastro.cttaditivo[i].checked){
				var cttaditivo = document.formularioCadastro.cttaditivo[i].value
			}
		}

		if(cttaditivo == 'S' &&($('ctavigenciainicio').getValue() == '' || $('ctavigenciatermino').getValue() == '')){
			var msg = decodeURIComponent(escape('O campo "Vigência" é obrigatório!'));
			alert(msg);
			$('btnSalvar').enable();
			$('btnIncluir').enable();
        	return false;
		}
		
		return true;
	}

	
	/**
	 * Mostra/Esconde o trecho de aditivos
	 * @name mostraEscondeAditivo
	 * @param string valor - valor do radio clicado
	 * @return void
	 */
	function mostraEscondeAditivo(valor){
		if(valor == 'S'){
			ocultaMostraCamposAditivo('M');
		}
		else if(valor == 'N'){
			ocultaMostraCamposAditivo('O');
		}
	}

	 Event.observe(window, 'load', function() {

		 if($('cttid').getValue() == '' && $('erroData').getValue() == ''){
			 ocultaMostraCamposAditivo('O');	
		 }
		 else if($('cttid').getValue() != '' || $('erroData').getValue() != ''){
			 verificaExistenciaAditivo('cttaditivo');	
		 }	

		 //verifica se teve erro de data. 
		 //caso haja coloca o foco nos campos corretos
		 if($F('erroData') == 'datamenor'){
		     $('cttvigenciainicio').focus();	
		 }
		 else if($F('erroData') == 'anoinicialinvalido'){
			 $('cttvigenciainicio').focus();
		 }
		 else if($F('erroData') == 'anofinalinvalido'){
		     $('cttvigenciatermino').focus();
		 }

		 if($F('erroDataAditivo') == 'datamenor'){
		     $('ctavigenciainicio').focus();
		 }
		 else if($F('erroDataAditivo') == 'datamaior'){
			 $('ctavigenciatermino').focus();
		 }
		 else if($F('erroDataAditivo') == 'anoinicialinvalido'){
			 $('ctavigenciainicio').focus();
		 }
		 else if($F('erroDataAditivo') == 'anofinalinvalido'){
		     $('ctavigenciatermino').focus();	
		 }
		 		
	 });


	 /**
	  * Mostra/Esconde o trecho de aditivos
	  * @name ocultaMostraCamposAditivo
	  * @param string operacao - se é para ocultar ou mostrar
	  * @return void
	  */
	 function ocultaMostraCamposAditivo(operacao){
	     if(operacao == 'O'){
	    	 $('linhatituloaditivo').hide();
			 $('linhanumeroaditivo').hide();
			 $('linhavigenciaaditivo').hide();
			 $('linhabotaoaditivo').hide();	

			 if($('linhalistaaditivo')){
			 	$('linhalistaaditivo').hide();
			 }
			 if($('linhatitulolistaaditivo')){
			 	$('linhatitulolistaaditivo').hide();
			 }	
	     }	
	     else if(operacao == 'M'){
	    	 $('linhatituloaditivo').show();
			 $('linhanumeroaditivo').show();
			 $('linhavigenciaaditivo').show();
			 $('linhabotaoaditivo').show();	
			 
			 if($('linhalistaaditivo')){
			 	$('linhalistaaditivo').show();
			 }
			 if($('linhatitulolistaaditivo')){
			 	$('linhatitulolistaaditivo').show();
			 }	
	     }
	 }


	 /**
	  * Verifica o valor do campo aditivo(radio)
	  * @name verificaExistenciaAditivo
	  * @param string field - nome do campo
	  * @return void
	  */
	 function verificaExistenciaAditivo(field){

		 var radioGroup = $(field).cttidname;
	     var el = $(field).form;	
	     
	     var checked = $(el).getInputs('radio', radioGroup).find(
	         function(re) {return re.checked;}
	     );
	     var cttaditivo = (checked) ? $F(checked) : null;
	     if(cttaditivo == 'N'){
	    	 ocultaMostraCamposAditivo('O');    	
	     }	 

	 }

	 /**
	  * Edição do registro
	  * @name detalharAditivo
	  * @param int ctaid - identificador do registro
	  * @return void
	  */
	 function detalharAditivo(ctaid){
		 var cttid = $F('cttid');
		 window.location = '?modulo=principal/contrato/cadContrato&acao=A&id='+cttid+'&ctaid='+ctaid;
	 }

	 
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Ag&ecirc;ncia: </td>
			<td class="campo"><?php $oFornecedor->monta_combo_fornecedor($forid,'S',1);?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&ordm; Contrato: </td>
			<td class="campo">
				<?=campo_texto('cttnumcontrato','S','S','Informe o N&ordm; Contrato',10,8,'###/####','','left','',0,'id="cttnumcontrato"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&ordm; Sidoc: </td>
			<td class="campo">
				<?=campo_texto('cttnumsidoc','N','S','Informe o N&ordm; Sidoc',22,20,'#####.######/####-##','','left','',0,'id="cttnumsidoc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Vig&ecirc;ncia: </td>
			<td class="campo">
				<?php echo campo_data2('cttvigenciainicio','S','S','Informe a Data Inicial','N','','',$cttvigenciainicio);?> a 
				<?php echo campo_data2('cttvigenciatermino','S','S','Informe a Data Final','N','','',$cttvigenciatermino);?> 
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Aditivo:</td>
			<td class="campo">
				<input type='radio' name='cttaditivo' id='cttaditivo' value='S' onclick="javascript:mostraEscondeAditivo('S');" <?php if($cttaditivo == 'S'){ echo 'checked'; } ?>>Sim
				<input type='radio' name='cttaditivo' id='cttaditivo' value='N' onclick="javascript:mostraEscondeAditivo('N');" <?php if($cttaditivo == 'N' || empty($cttaditivo)){ echo 'checked'; } ?>>N&atilde;o
			</td>
		</tr>
		<tr id='linhatituloaditivo'>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Aditivo </td>
		</tr>
		<tr id='linhanumeroaditivo'>
			<td class="SubtituloDireita"> N&ordm; Aditivo: </td>
			<td class="campo">
				<?=campo_texto('ctanumaditivo','N','N','',5,3,'','','left','',0,'id="ctanumaditivo"');?>
            </td>
		</tr>
		<tr id='linhavigenciaaditivo'>
			<td class="SubtituloDireita"> Vig&ecirc;ncia: </td>
			<td class="campo">
				<?php echo campo_data2('ctavigenciainicio','S','S','Informe a Data Inicial','N','','',$ctavigenciainicio);?> a 
				<?php echo campo_data2('ctavigenciatermino','S','S','Informe a Data Final','N','','',$ctavigenciatermino);?> 
            </td>
		</tr>
		<tr id='linhabotaoaditivo'>
        	<td colspan='2' align='center' class="campo">
            	<?php 
            	//só mostra este trecho caso o fluxo NÃO seja advindo do ícone visualizar
            	if($_SESSION['visualizar'] != 'S'){
            	?>
            	<input type='button' class="botao" name='btnIncluir' id='btnIncluir' value='Incluir' onclick="javascript:incluir();">
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<?php 
            	}
            	?>
            	<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid; ?>'>
        		<input type='hidden' name='ctaid' id='ctaid' value='<?php echo $ctaid; ?>'>
        		<input type='hidden' name='erroData' id='erroData' value='<?php echo $erroData; ?>'>
        		<input type='hidden' name='erroDataAditivo' id='erroDataAditivo' value='<?php echo $erroDataAditivo; ?>'>
        		<input type='hidden' name='qtdAditivos' id='qtdAditivos' value='<?php echo $qtdAditivos; ?>'>
            </td>
        </tr>
    </table>
</form>
	
	<!-- TRECHO DOS ADITIVOS -->
	<table class="tabela" align="center" border="0">
        <?php
		//lista os aditivos do contrato
		if($qtdAditivos >= 1){
		?>
		<tr id='linhatitulolistaaditivo'>
			<td bgcolor='#e9e9e9' align='center' colspan='4'> Lista de Aditivos </td>
		</tr>
	</table>
		<?php 	
			$oContratoAditivo->listar_por_id($cttid);
		}
        ?>
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<?php 
        		//só mostra este trecho caso o fluxo NÃO seja advindo do ícone visualizar
        		if($_SESSION['visualizar'] != 'S'){
        		?>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Pr&oacute;xima >>' onclick="javascript:salvar();">
            	<?php 
        		}
            	?>
            </td>
        </tr>
	</table>
	<!-- FIM TRECHO DOS ADITIVOS -->
