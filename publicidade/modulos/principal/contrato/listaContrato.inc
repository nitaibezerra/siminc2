<?php
// incluindo as classes necessárias
$oContrato = new Contrato();
$oContratoAditivo = new ContratoAditivo();
$oContratoHonorario = new ContratoHonorario();
$oContratoOrcamento = new ContratoOrcamento();

unset($_SESSION['cttid'],$_SESSION['visualizar']);

//caso seja para excluir
if($_GET['requisicao'] == 'excluir'){
	$oContratoAditivo->inativar_aditivos_filhos($_GET['id']);
	$oContratoHonorario->inativar_honorarios_filhos($_GET['id']);
	$oContratoOrcamento->inativar_orcamentos_filhos($_GET['id']);
	$oContrato->inativar($_GET['id']);
}
else if($_POST['requisicao'] == 'filtrar'){
	//verifica as datas
	$erroData = $oContrato->verifica_data($_POST);
	
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

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_CONTRATO_HONORARIOS,ABA_CONTRATO_ORCAMENTO);
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Contrato','');

extract($_POST);
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Aciona o filtro
	 * @name pesquisar
	 * @param requisicao - Requisição que será executada
	 * @return void
	 */	
	function pesquisar(requisicao){
		$('btnPesquisar').disable();
		if( validaFormularioPesquisa() )
        {
			$('requisicao').setValue(requisicao);
			$('formularioPesquisa').submit();
		}
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioPesquisa
	 * @return bool
	 */	
	function validaFormularioPesquisa(){
	
		if(
            $( 'fornome' ).getValue() == ''
            &&
            $('cttnumcontrato').getValue() == ''
            &&
            $('cttnumsidoc').getValue() == ''
            &&
            $('cttstatus').getValue() == ''
            &&
            $('cttvigenciainicio').getValue() == ''
            &&
            $('cttvigenciatermino').getValue() == ''
        ){
			alert( '� obrigat�rio o preenchimento de pelo menos um campo para a realiza��o da pesquisa' );
			$( 'btnPesquisar' ).enable();
	        return false;
		}

		return true;
	}
	
	 /**
	  * Edição do registro
	  * @name detalhar
	  * @param int id - identificador do registro
	  * @return void
	  */
	 function detalhar(id){
		 window.location = '?modulo=principal/contrato/cadContrato&acao=A&id='+id;
	 }

	 /**
	  * Exclusão do registro
	  * @name remover
	  * @param int id - identificador do registro
	  * @return void
	  */
	 function remover(id){
		 window.location = '?modulo=principal/contrato/listaContrato&acao=A&id='+id+'&requisicao=excluir';
	 }

	/**
	 * Função responsável por direcionar à tela de inclusão
	 * @name adicionar
	 * @return void
	 */
	function adicionar(){
		window.location = '?modulo=principal/contrato/cadContrato&acao=A';
	}	

	/**
	 * Visualização do registro
	 * @name visualizar
	 * @param int id - identificador do registro
	 * @return void
	 */
	function visualizar(id){
		window.location = '?modulo=principal/contrato/cadContrato&acao=A&id='+id+'&visualizar=S';
	}


	Event.observe(window, 'load', function() {

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
		 		
	 });
</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Ag&ecirc;ncia: </td>
			<td class="campo">
				<?=campo_texto('fornome','N','S','Informe a Ag&ecirc;ncia',100,255,'','','left','',0,'id="fornome"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&ordm; Contrato: </td>
			<td class="campo">
				<?=campo_texto('cttnumcontrato','N','S','Informe o N&uacute;mero',10,8,'###/####','','left','',0,'id="cttnumcontrato"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&ordm; Sidoc: </td>
			<td class="campo">
				<?=campo_texto('cttnumsidoc','N','S','Informe o N&uacute;mero',22,20,'#####.######/####-##','','left','',0,'id="cttnumsidoc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Status: </td>
			<td class="campo"><?php $oContrato->monta_combo_status();?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Vig&ecirc;ncia: </td>
			<td class="campo">
				<?php echo campo_data2('cttvigenciainicio','N','S','Informe a Data Inicial','N','','',$cttvigenciainicio);?> a 
				<?php echo campo_data2('cttvigenciatermino','N','S','Informe a Data Final','N','','',$cttvigenciatermino);?> 
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
        		<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid; ?>' />
        		<input type='hidden' name='erroData' id='erroData' value='<?php echo $erroData; ?>' />
            	<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">
            	<input type='button' class="botao" name='btnAdicionar' id='btnAdicionar' value='Adicionar' onclick="javascript:adicionar();">
            </td>
        </tr>
	</table>
</form>
<?php
//verifica se é para disparar a consulta no banco por filtro ou buscar todos
if($_POST['requisicao'] == 'filtrar' && empty($erroData)){
	$oContrato->listar_por_filtro($_POST);
}
else{
	$oContrato->listar_por_filtro('');
}

# Exibi mensagem de registro excluido
if( $_REQUEST[ 'requisicao' ] == 'excluir' )
{
    echo "
        <script language='javascript' type='text/javascript'>
            alert( 'Exclus�o realizada com sucesso!' );
        </script>
    ";
}
?>
