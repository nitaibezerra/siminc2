<?php
// incluindo as classes necessárias
$oAnexo = new Anexo();
$oPrazoPeca = new PrazoPeca();
$oPeriodoPeca = new PeriodoPeca();
$oPrazoVeiculacao = new PrazoVeiculacao();
$oArquivoPeca = new ArquivoPeca();

//caso tenha postado o formulário
if($_POST['requisicao'] == 'salvar'){
	
	//salva o registro pai
	$resultado = $oAnexo->salvar($_POST);
	
	if($resultado[0]){
		$msg = utf8_decode('Operação realizada com sucesso!');
		direcionar('?modulo=principal/anexo/listaAnexo&acao=A',$msg);
	}
	else if(!$resultado[0]){
		$msg = utf8_decode('Operação falhou!');
		direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_POST['ipaid'],$msg);
	}
}
else if($_POST['requisicaoPrazoVeiculacao'] == 'incluirPrazoVeiculacao'){
	//verifica as datas
	$erroData = $oPrazoVeiculacao->verifica_data($_POST);
	
	//caso não encontre problemas com as datas, salva
	if(empty($erroData)){
		
		//recoloca os dados no array
		$_POST['anetitulo'] = $_POST['anetituloPrazoVeiculacao'];
		$_POST['anedsc'] = $_POST['anedscPrazoVeiculacao'];
		$_POST['ipaid'] = $_POST['ipaidPrazoVeiculacao'];
		$_POST['aneid'] = $_POST['aneidPrazoVeiculacao'];
		unset($_POST['anetituloPrazoVeiculacao'],$_POST['anedscPrazoVeiculacao'],$_POST['ipaidPrazoVeiculacao'],$_POST['aneidPrazoVeiculacao']);
		
		//salva o registro pai
		$resultado = $oAnexo->salvar($_POST);
	
		//se gravar o registro pai, grava o filho
		if($resultado[0]){
			$_POST['aneid'] = $resultado[1];
		
			$resultadoPrazo = $oPrazoVeiculacao->salvar($_POST);
		
			if($resultadoPrazo[0]){
				$msg = utf8_decode('Operação realizada com sucesso!');
				direcionar('?modulo=principal/anexo/'.$resultadoPrazo[3].'&acao=A&ipaid='.$resultadoPrazo[2],$msg);
			}
			else if(!$resultadoPrazo[0]){
				$msg = utf8_decode('Operação falhou!');
				direcionar('?modulo=principal/anexo/'.$resultadoPrazo[3].'&acao=A&ipaid='.$resultadoPrazo[2],$msg);
			}
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/anexo/'.$resultado[3].'&acao=A&ipaid='.$resultado[2],$msg);
		}
	}
	else if($erroData == 'datamenorveiculacao'){
		$msg = utf8_decode('Data Inválida!');
		alerta($msg);
	}
	else if($erroData == 'anoinicialinvalidoveiculacao' || $erroData == 'anofinalinvalidoveiculacao'){
		$msg = utf8_decode('A Data Informada é Inválida!');
		alerta($msg);
	}
	$_POST['erroData'] = $erroData;
	$_POST['anetitulo'] = $_POST['anetituloPrazoVeiculacao'];
	$_POST['anedsc'] = $_POST['anedscPrazoVeiculacao'];
	extract($_POST);
}
else if($_POST['requisicaoPrazo'] == 'incluirPrazo'){
	
	//verifica as datas
	if($_POST['ppeprazo'] != 'S'){
		$erroData = $oPrazoPeca->verifica_data($_POST);
	}
	else{
		$erroData = '';
	}
	
	//caso não encontre problemas com as datas, salva
	if(empty($erroData)){
		
		//recoloca os dados no array
		$_POST['anetitulo'] = $_POST['anetituloPrazo'];
		$_POST['anedsc'] = $_POST['anedscPrazo'];
		$_POST['ipaid'] = $_POST['ipaidPrazo'];
		$_POST['aneid'] = $_POST['aneidPrazo'];
		unset($_POST['anetituloPrazo'],$_POST['anedscPrazo'],$_POST['ipaidPrazo'],$_POST['aneidPrazo']);
		
		//salva o registro pai
		$resultado = $oAnexo->salvar($_POST);
	
		//se gravar o registro pai, grava o filho
		if($resultado[0]){
			$_POST['aneid'] = $resultado[1];
		
			$resultadoPrazo = $oPrazoPeca->salvar($_POST);
		
			if($resultadoPrazo[0]){
				$msg = utf8_decode('Operação realizada com sucesso!');
				direcionar('?modulo=principal/anexo/'.$resultadoPrazo[3].'&acao=A&ipaid='.$resultadoPrazo[2],$msg);
			}
			else if(!$resultadoPrazo[0]){
				$msg = utf8_decode('Operação falhou!');
				direcionar('?modulo=principal/anexo/'.$resultadoPrazo[3].'&acao=A&ipaid='.$resultadoPrazo[2],$msg);
			}
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/anexo/'.$resultado[3].'&acao=A&ipaid='.$resultado[2],$msg);
		}
	}
	else if($erroData == 'datamenor'){
		$msg = utf8_decode('Data Inválida!');
		alerta($msg);
	}
	else if($erroData == 'anoinicialinvalido' || $erroData == 'anofinalinvalido'){
		$msg = utf8_decode('A Data Informada é Inválida!');
		alerta($msg);
	}
	$_POST['erroData'] = $erroData;
	$_POST['anetitulo'] = $_POST['anetituloPrazo'];
	$_POST['anedsc'] = $_POST['anedscPrazo'];
	extract($_POST);
	
}
else if($_POST['requisicaoPeriodo'] == 'incluirPeriodo'){
	
	# verifica as datas
	if($_POST['ppeperiodo'] != 'S')
    {
		$erroData = $oPeriodoPeca->verifica_data($_POST);
	}
	else{
		$erroData = '';
	}
	
	# caso não encontre problemas com as datas, salva
	if(empty($erroData)){
		
		# recoloca os dados no array
		$_POST['anetitulo'] = $_POST['anetituloPeriodo'];
		$_POST['anedsc'] = $_POST['anedscPeriodo'];
		$_POST['ipaid'] = $_POST['ipaidPeriodo'];
		$_POST['aneid'] = $_POST['aneidPeriodo'];
		unset($_POST['anetituloPeriodo'],$_POST['anedscPeriodo'],$_POST['ipaidPeriodo'],$_POST['aneidPeriodo']);
		
		# salva o registro pai
		$resultado = $oAnexo->salvar( $_POST );
	
		# se gravar o registro pai, grava o filho
		if($resultado[0])
        {
			$_POST['aneid'] = $resultado[1];
		
			$resultadoPeriodo = $oPeriodoPeca->salvar( $_POST );
		
			if($resultadoPeriodo[0]){
				$msg = utf8_decode('Operação realizada com sucesso!');
				direcionar('?modulo=principal/anexo/'.$resultadoPeriodo[3].'&acao=A&ipaid='.$resultadoPeriodo[2],$msg);
			}
			else if(!$resultadoPeriodo[0]){
				$msg = utf8_decode('Operação falhou!');
				direcionar('?modulo=principal/anexo/'.$resultadoPeriodo[3].'&acao=A&ipaid='.$resultadoPeriodo[2],$msg);
			}
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/anexo/'.$resultado[3].'&acao=A&ipaid='.$resultado[2],$msg);
		}
	}
	else if($erroData == 'datamenor'){
		$msg = utf8_decode('Data Inválida!');
		alerta($msg);
	}
	else if($erroData == 'anoinicialinvalido' || $erroData == 'anofinalinvalido'){
		$msg = utf8_decode('A Data Informada é Inválida!');
		alerta($msg);
	}
	$_POST['erroData'] = $erroData;
	$_POST['anetitulo'] = $_POST['anetituloPeriodo'];
	$_POST['anedsc'] = $_POST['anedscPeriodo'];
	extract($_POST);
}
else if($_POST['requisicaoArquivo'] == 'incluirArquivo'){
	
	//verifica extensão do arquivo
	$pos = strrpos($_FILES['arquivo']['name'],'.');
	if($pos !== false){
		$ext = strtolower(substr($_FILES['arquivo']['name'],$pos+1));
	}
	if($ext == 'mp3' || $ext == 'pdf' || $ext == 'mpeg' || $ext == 'wmv'){
		
		//recoloca os dados no array
		$_POST['anetitulo'] = $_POST['anetituloArquivo'];
		$_POST['anedsc'] = $_POST['anedscArquivo'];
		$_POST['ipaid'] = $_POST['ipaidArquivo'];
		$_POST['aneid'] = $_POST['aneidArquivo'];
		unset($_POST['anetituloArquivo'],$_POST['anedscArquivo'],$_POST['ipaidArquivo'],$_POST['aneidArquivo']);
		
		//salva o registro pai
		$resultado = $oAnexo->salvar($_POST);

		//se gravar o registro pai, grava o filho
		if($resultado[0]){
			$_POST['aneid'] = $resultado[1];
	
			$resultadoArquivo = $oArquivoPeca->salvar($_POST,$_FILES);
	
			if($resultadoArquivo){
				$msg = utf8_decode('Operação realizada com sucesso!');
				direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_POST['ipaid'],$msg);
			}
			else if(!$resultadoArquivo){
				$msg = utf8_decode('Operação falhou!');
				direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_POST['ipaid'],$msg);
			}
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/anexo/'.$resultado[3].'&acao=A&ipaid='.$resultado[2],$msg);
		}
	}
	else{
		$msg = utf8_decode('A extensão do arquivo é Inválida. São permitidas as extensões mp3, pdf, mpeg e wmv!');
		direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_GET['ipaid'],$msg);
	}
	extract($_POST);
	
}
else if($_GET['requisicao'] == 'excluirPrazoVeiculacao' && !empty($_GET['pveid'])){
	$oPrazoVeiculacao->inativar($_GET['pveid']);
	$msg = utf8_decode('Operação realizada com sucesso!');
	direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_GET['ipaid'],$msg);
}
else if($_GET['requisicao'] == 'excluirPrazo' && !empty($_GET['ppeid'])){
	$oPrazoPeca->inativar($_GET['ppeid']);
	$msg = utf8_decode('Operação realizada com sucesso!');
	direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_GET['ipaid'],$msg);
}
else if($_GET['requisicao'] == 'excluirArquivo' && !empty($_GET['arqid']) && !empty($_GET['aneid'])){
	$oArquivoPeca->inativar($_GET['arqid'],$_GET['aneid']);
	$msg = utf8_decode('Operação realizada com sucesso!');
	direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_GET['ipaid'],$msg);
}
else if($_GET['requisicao'] == 'down' && !empty($_GET['arqid']) && !empty($_GET['aneid'])){
	$oArquivoPeca->baixar($_GET['arqid'],$_GET['aneid']);
	direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_GET['ipaid']);
}
else if($_GET['requisicao'] == 'excluirPeriodo' && !empty($_GET['ppeid'])){
	$oPeriodoPeca->inativar($_GET['ppeid']);
	$msg = utf8_decode('Operação realizada com sucesso!');
	direcionar('?modulo=principal/anexo/cadAnexo&acao=A&ipaid='.$_GET['ipaid'],$msg);
}

//caso seja via url
if(!empty($_GET['ipaid'])){
	$anexo = $oAnexo->carrega_registro_por_peca($_GET['ipaid']);
	if(is_array($anexo) && count($anexo) >= 1){
		extract($anexo);
	}
}
else{
	direcionar('?modulo=principal/anexo/listaAnexo&acao=A');
}

$ipaid = $_GET['ipaid'];

//pega a qtd de períodos e de arquivos do anexo
if(!empty($aneid) || !empty($_POST['aneid'])){
	if(empty($aneid)){
		$aneid = $_POST['aneid'];
	}
    $qtdPeriodos = $oPeriodoPeca->pega_qtd_periodos($aneid);
	$qtdPrazos = $oPrazoPeca->pega_qtd_prazos($aneid);
	$qtdPrazosVeiculacao = $oPrazoVeiculacao->pega_qtd_prazos($aneid);
	$qtdArquivos = $oArquivoPeca->pega_qtd_arquivos($aneid);
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Pe&ccedil;a Publicit&aacute;ria','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Redireciona para a pesquisa
	 * @name voltar
	 * @return void
	 */
	function voltar(){
		window.location = '?modulo=principal/anexo/listaAnexo&acao=A';
	}
	
	/**
	 * Submete o formulário
	 * @name salvar
	 * @return void
	 */	
	function salvar(){
		habilitaDesabilitaBotoes('D');
		if(validaFormularioCadastro()){
			$('formularioCadastro').submit();
		}
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioCadastro
	 * @param string origem - De onde vem a chamada para validação
	 * @return bool
	 */	
	function validaFormularioCadastro(origem){
	
		if($('anetitulo').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Título" é obrigatório!'));
			alert(msg);
			$('anetitulo').focus();
			habilitaDesabilitaBotoes('H');
	        return false;
		}

		if($('anedsc').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Descrição" é obrigatório!'));
			alert(msg);
			$('anedsc').focus();
			habilitaDesabilitaBotoes('H');
	        return false;
		}

		if(origem == 'incluirPrazo'){
			if(!$('ppeprazo').checked && ($F('ppeprazoinicio') == '' || $F('ppeprazotermino') == '')){
				alert('Favor informar o prazo!');
				habilitaDesabilitaBotoes('H');
				return false;
			}
		}

		if(origem == 'incluirArquivo'){
			if($F('arquivo') == ''){
				alert('Favor informar o arquivo!');
				habilitaDesabilitaBotoes('H');
				return false;
			}
		}

		if(origem == 'incluirPrazoVeiculacao'){
			if(($F('pveprazoinicio') != '' && $F('pveprazotermino') == '') || ($F('pveprazoinicio') == '' && $F('pveprazotermino') != '') || ($F('pveprazoinicio') == '' && $F('pveprazotermino') == '')){
				alert('Favor informar o prazo!');
				habilitaDesabilitaBotoes('H');
				return false;
			}
		}
		
		return true;
	}

	/**
	 * Incluir prazo
	 * @name incluirPrazo
	 * @return void
	 */
	function incluirPrazo(){
		habilitaDesabilitaBotoes('D');
		if(validaFormularioCadastro('incluirPrazo')){
			$('requisicaoPrazo').setValue('incluirPrazo');

			$('anetituloPrazo').setValue($F('anetitulo'));
			$('anedscPrazo').setValue($F('anedsc'));
			
			$('formularioPrazo').submit();
		}
		
	}

	/**
	 * Incluir periodo
	 * @name incluirPeriodo
	 * @return void
	 */
	function incluirPeriodo(){
		habilitaDesabilitaBotoes('D');
		if( validaFormularioCadastro( 'incluirPeriodo' ))
        {
			$('requisicaoPeriodo').setValue('incluirPeriodo');
			$('anetituloPeriodo').setValue($F('anetitulo'));
			$('anedscPeriodo').setValue($F('anedsc'));
		
			$('formularioPeriodo').submit();
		}
	}

	/**
	 * Incluir prazo de veiculação
	 * @name incluirPrazoVeiculacao
	 * @return void
	 */
	function incluirPrazoVeiculacao(){
		habilitaDesabilitaBotoes('D');
		if(validaFormularioCadastro('incluirPrazoVeiculacao')){
			$('requisicaoPrazoVeiculacao').setValue('incluirPrazoVeiculacao');
			
			$('anetituloPrazoVeiculacao').setValue($F('anetitulo'));
			$('anedscPrazoVeiculacao').setValue($F('anedsc'));
			
			$('formularioPrazoVeiculacao').submit();
		}
		
	}

	/**
	 * Incluir arquivo
	 * @name incluirArquivo
	 * @return void
	 */
	function incluirArquivo(){
		habilitaDesabilitaBotoes('D');
		if(validaFormularioCadastro('incluirArquivo')){
			$('requisicaoArquivo').setValue('incluirArquivo');

			$('anetituloArquivo').setValue($F('anetitulo'));
			$('anedscArquivo').setValue($F('anedsc'));
			
			$('formularioArquivo').submit();
		}
	}

	/**
	 * Verifica a checkbox de prazo
	 * @name verificaPrazo
	 * @return void
	 */	
	function verificaPrazo(){
		if($('ppeprazo').checked){
			habilitaDesabilita('D');
		}
		else{
			habilitaDesabilita('H');
		}
	}
    
	/**
	 * Mostra/Esconde o trecho de vigência
	 * @name habilitaDesabilita
	 * @param string acao - Se é para esconder/mostrar
	 * @return void
	 */
	function habilitaDesabilita(acao){
		if(acao == 'D'){
			$('linhaperiodohabilitado').hide();
			$('linhaperiododesabilitado').show();
		}
		else if(acao == 'H'){
			$('linhaperiodohabilitado').show();
			$('linhaperiododesabilitado').hide();
		}
	}

	/**
	 * Verifica a checkbox de periodo
	 * @name verificaPeriodo
	 * @return void
	 */	
	function verificaPeriodo(){
		if($('ppeperiodo').checked){
			habilitaDesabilitaPeriodo( 'D' );
		}
		else{
			habilitaDesabilitaPeriodo( 'H' );
		}
	}

	/**
	 * Mostra/Esconde o trecho de vigência
	 * @name habilitaDesabilita
	 * @param string acao - Se é para esconder/mostrar
	 * @return void
	 */
	function habilitaDesabilitaPeriodo(acao){
		if(acao == 'D'){
			$('linhaperiodopublicitariahabilitado').hide();
			$('linhaperiodopublicitariadesabilitado').show();
		}
		else if(acao == 'H'){
			$('linhaperiodopublicitariahabilitado').show();
			$('linhaperiodopublicitariadesabilitado').hide();
		}
	}

	/**
	 * Excluir um prazo do anexo
	 * @name excluirPrazo
	 * @param int ppeid - Identificador do registro
	 * @return void
	 */
	function excluirPrazo(ppeid){
		var ipaid = $F('ipaidPrazo');
		window.location = '?modulo=principal/anexo/cadAnexo&acao=A&ipaid='+ipaid+'&ppeid='+ppeid+'&requisicao=excluirPrazo';
	}

	/**
	 * Excluir um prazo de veiculação
	 * @name excluirPrazoVeiculacao
	 * @param int pveid - Identificador do registro
	 * @return void
	 */
	function excluirPrazoVeiculacao(pveid){
		var ipaid = $F('ipaidPrazoVeiculacao');
		window.location = '?modulo=principal/anexo/cadAnexo&acao=A&ipaid='+ipaid+'&pveid='+pveid+'&requisicao=excluirPrazoVeiculacao';
	}

	 /**
		 * Excluir um periodo de pe�a publicitaria
		 * @name excluirPrazoVeiculacao
		 * @param int pveid - Identificador do registro
		 * @return void
		 */
		function excluirPeriodo(ppeid){
			var ipaid = $F('ipaidPeriodo');
			window.location = '?modulo=principal/anexo/cadAnexo&acao=A&ipaid='+ipaid+'&ppeid='+ppeid+'&requisicao=excluirPeriodo';
		}

	/**
	 * Excluir um arquivo do anexo
	 * @name excluirArquivo
	 * @param int arqid - Identificador do registro
	 * @return void
	 */
	function excluirArquivo(arqid){
		var ipaid = $F('ipaidArquivo');
		var aneid = $F('aneidArquivo');
		window.location = '?modulo=principal/anexo/cadAnexo&acao=A&ipaid='+ipaid+'&arqid='+arqid+'&aneid='+aneid+'&requisicao=excluirArquivo';
	}

	/**
	 * Baixar um arquivo do anexo
	 * @name baixar
	 * @param int arqid - Identificador do registro
	 * @return void
	 */
	function baixar(arqid){
		var ipaid = $F('ipaidArquivo');
		var aneid = $F('aneidArquivo');
		window.location = '?modulo=principal/anexo/cadAnexo&acao=A&ipaid='+ipaid+'&arqid='+arqid+'&aneid='+aneid+'&requisicao=down';
	}

	/**
	 * Habilita/Desabilita os botões
	 * @name habilitaDesabilitaBotoes
	 * @param string acao - Se é para habilitar/desabilitar
	 * @return void
	 */
	function habilitaDesabilitaBotoes(acao){
		if(acao == 'D'){
			$('btnSalvar').disable();
            $('btnIncluirPeriodo').disable();
			$('btnIncluirPrazo').disable();
			$('btnIncluirPrazoVeiculacao').disable();
			$('btnIncluirArquivo').disable();
		}
		else if(acao == 'H'){
			$('btnSalvar').enable();
			$('btnIncluirPrazo').enable();
            $('btnIncluirPeriodo').enable();
			$('btnIncluirPrazoVeiculacao').enable();
			$('btnIncluirArquivo').enable();
		}
	}	


	 Event.observe(window, 'load', function() {

		 //verifica se teve erro de data. 
		 //caso haja coloca o foco nos campos corretos
		 if($F('erroData') == 'datamenor'){
		     $('ppeprazoinicio').focus();	
		 }
		 else if($F('erroData') == 'anoinicialinvalido'){
			 $('ppeprazoinicio').focus();
		 }
		 else if($F('erroData') == 'anofinalinvalido'){
		     $('ppeprazotermino').focus();
		 }
		 else if($F('erroData') == 'datamenorveiculacao'){
		 	 $('pveprazoinicio').focus();
		 }
		 else if($F('erroData') == 'anoinicialinvalidoveiculacao'){
		     $('pveprazoinicio').focus();
		 }
		 else if($F('erroData') == 'anofinalinvalidoveiculacao'){
		     $('pveprazotermino').focus();
		 }
		 		
	 });
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> T&iacute;tulo: </td>
			<td class="campo">
				<?=campo_texto('anetitulo','S','S','Informe o T&iacute;tulo',100,200,'','','left','',0,'id="anetitulo"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_textarea('anedsc','S', 'S', '', 54, 6, 4000,'','','','','',''); ?>
				<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
				<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'>
				<input type='hidden' name='aneid' id='aneid' value='<?php echo $aneid; ?>'>
            </td>
		</tr>
	</table>
</form>

<form name="formularioPrazoVeiculacao" id="formularioPrazoVeiculacao" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Per&iacute;odo de Veicula&ccedil;&atilde;o </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Per&iacute;odo: </td>
			<td class="campo">
				<?php echo campo_data2('pveprazoinicio','N','S','Informe a Data Inicial','N','','',$pveprazoinicio);?> a 
				<?php echo campo_data2('pveprazotermino','N','S','Informe a Data Final','N','','',$pveprazotermino);?> 
            </td>
		</tr>
		<tr>
        	<td colspan='2' align='center' class="campo">
            	<input type='button' class="botao" name='btnIncluirPrazoVeiculacao' id='btnIncluirPrazoVeiculacao' value='Incluir' onclick="javascript:incluirPrazoVeiculacao();">
            	<input type='hidden' name='requisicaoPrazoVeiculacao' id='requisicaoPrazoVeiculacao' value='salvar'>
            	<input type='hidden' name='anetituloPrazoVeiculacao' id='anetituloPrazoVeiculacao' value='<?php echo $anetitulo; ?>'>
            	<input type='hidden' name='anedscPrazoVeiculacao' id='anedscPrazoVeiculacao' value='<?php echo $anedsc; ?>'>
            	<input type='hidden' name='ipaidPrazoVeiculacao' id='ipaidPrazoVeiculacao' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='aneidPrazoVeiculacao' id='aneidPrazoVeiculacao' value='<?php echo $aneid; ?>'>
            </td>
        </tr>
    </table>
</form>
    
    <!-- TRECHO DOS PERÍODOS VEICULAÇÃO -->
	<?php
	//lista os períodos do anexo
	if($qtdPrazosVeiculacao >= 1){
		$oPrazoVeiculacao->listar_por_anexo($aneid);
	}
    ?>
	<!-- FIM TRECHO DOS PERÍODOS VEICULAÇÃO -->

<form name="formularioPeriodo" id="formularioPeriodo" method="post">
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'>Per&iacute;odo de Pe&ccedil;a Publicit&aacute;ria</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Prazo Indeterminado: </td>
			<td class="campo">
				<input type='checkbox' name='ppeperiodo' id='ppeperiodo' value='S' onclick='javascript:verificaPeriodo();'>
			</td>
		</tr>
		<tr id='linhaperiodopublicitariahabilitado'>
			<td class="SubtituloDireita"> Per&iacute;odo: </td>
			<td class="campo">
				<?php echo campo_data2('ppeperiodoinicio','N','S','Informe a Data Inicial','N','','',$ppeperiodoinicio );?> a 
				<?php echo campo_data2('ppeperiodotermino','N','S','Informe a Data Final','N','','',$ppeperiodotermino );?> 
            </td>
		</tr>
		<tr id='linhaperiodopublicitariadesabilitado' style='display: none'>
			<td class="SubtituloDireita"> Per&iacute;odo: </td>
			<td class="campo">
				<?php echo campo_texto('periodoinicio','N','N','',10,10,'','','left','',0,'id="periodoinicio"');?> a 
				<?php echo campo_texto('periodotermino','N','N','',10,10,'','','left','',0,'id="periodotermino"');?> 
            </td>
		</tr>
		<tr>
        	<td colspan='2' align='center' class="campo">
            	<input type='button' class="botao" name='btnIncluirPeriodo' id='btnIncluirPeriodo' value='Incluir' onclick="javascript:incluirPeriodo();">
            	<input type='hidden' name='requisicaoPeriodo' id='requisicaoPeriodo' value='salvar'>
            	<input type='hidden' name='anetituloPeriodo' id='anetituloPeriodo' value='<?php echo $anetitulo; ?>'>
            	<input type='hidden' name='anedscPeriodo' id='anedscPeriodo' value='<?php echo $anedsc; ?>'>
            	<input type='hidden' name='ipaidPeriodo' id='ipaidPeriodo' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='aneidPeriodo' id='aneidPeriodo' value='<?php echo $aneid; ?>'>
            </td>
        </tr>
    </table>
</form>

<!-- Periodo da peca publicitaria -->
<?php
# lista os periodos
if( $qtdPeriodos >= 1 )
{
    $oPeriodoPeca->listar_por_anexo( $aneid );
}
?>
<!-- Periodo da peca publicitaria -->

<form name="formularioPrazo" id="formularioPrazo" method="post">
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Prazo Autorizado de Exibi&ccedil;&atilde;o </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Prazo Indeterminado: </td>
			<td class="campo">
				<input type='checkbox' name='ppeprazo' id='ppeprazo' value='S' onclick='javascript:verificaPrazo();'>
			</td>
		</tr>
		<tr id='linhaperiodohabilitado'>
			<td class="SubtituloDireita"> Per&iacute;odo: </td>
			<td class="campo">
				<?php echo campo_data2('ppeprazoinicio','N','S','Informe a Data Inicial','N','','',$ppeprazoinicio);?> a 
				<?php echo campo_data2('ppeprazotermino','N','S','Informe a Data Final','N','','',$ppeprazotermino);?> 
            </td>
		</tr>
		<tr id='linhaperiododesabilitado' style='display: none'>
			<td class="SubtituloDireita"> Per&iacute;odo: </td>
			<td class="campo">
				<?php echo campo_texto('prazoinicio','N','N','',10,10,'','','left','',0,'id="prazoinicio"');?> a 
				<?php echo campo_texto('prazotermino','N','N','',10,10,'','','left','',0,'id="prazotermino"');?> 
            </td>
		</tr>
		<tr>
        	<td colspan='2' align='center' class="campo">
            	<input type='button' class="botao" name='btnIncluirPrazo' id='btnIncluirPrazo' value='Incluir' onclick="javascript:incluirPrazo();">
            	<input type='hidden' name='requisicaoPrazo' id='requisicaoPrazo' value='salvar'>
            	<input type='hidden' name='anetituloPrazo' id='anetituloPrazo' value='<?php echo $anetitulo; ?>'>
            	<input type='hidden' name='anedscPrazo' id='anedscPrazo' value='<?php echo $anedsc; ?>'>
            	<input type='hidden' name='ipaidPrazo' id='ipaidPrazo' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='aneidPrazo' id='aneidPrazo' value='<?php echo $aneid; ?>'>
            </td>
        </tr>
    </table>
</form>
        
    <!-- TRECHO DOS PERÍODOS -->
	<?php
	//lista os períodos do anexo
	if($qtdPrazos >= 1){
		$oPrazoPeca->listar_por_anexo($aneid);
	}
    ?>
	<!-- FIM TRECHO DOS PERÍODOS -->

<form name="formularioArquivo" id="formularioArquivo" method="post" enctype="multipart/form-data">
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Arquivo Anexo (mp3, pdf, mpeg e wmv) </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Arquivo: </td>
			<td class="campo">
				<input type='file' name='arquivo' id='arquivo'>
            </td>
		</tr>
		<tr>
        	<td colspan='2' align='center' class="campo">
            	<input type='button' class="botao" name='btnIncluirArquivo' id='btnIncluirArquivo' value='Incluir' onclick="javascript:incluirArquivo();">
            	<input type='hidden' name='requisicaoArquivo' id='requisicaoArquivo' value='salvar'>
            	<input type='hidden' name='anetituloArquivo' id='anetituloArquivo' value='<?php echo $anetitulo; ?>'>
            	<input type='hidden' name='anedscArquivo' id='anedscArquivo' value='<?php echo $anedsc; ?>'>
            	<input type='hidden' name='ipaidArquivo' id='ipaidArquivo' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='aneidArquivo' id='aneidArquivo' value='<?php echo $aneid; ?>'>
            	<input type='hidden' name='erroData' id='erroData' value='<?php echo $erroData; ?>'>
            </td>
        </tr>
    </table>
</form>
    <!-- TRECHO DOS ARQUIVOS -->
	<?php
	//lista os arquivos do anexo
	if($qtdArquivos >= 1){
		$oArquivoPeca->listar_por_anexo($aneid);
	}
    ?>
	<!-- FIM TRECHO DOS ARQUIVOS -->
        
        
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
