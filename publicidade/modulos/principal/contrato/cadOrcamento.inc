<?php
// incluindo as classes necessárias
$oContrato = new Contrato();
$oContratoOrcamento = new ContratoOrcamento();

if(!empty($_SESSION['cttid']) && empty($_GET['id'])){
	$_GET['id'] = $_SESSION['cttid'];
}

//caso tenha postado o formulário
if($_POST['requisicao'] == 'salvar'){
	$msg = utf8_decode('Operação realizada com sucesso!');
	direcionar('?modulo=principal/contrato/listaContrato&acao=A',$msg);

}
else if($_POST['requisicao'] == 'incluir'){
	
	//verifica se já existe o registro
	$repetido = $oContratoOrcamento->verifica_unico($_POST);
	
	//caso não encontre repetido, salva
	if(!$repetido){
		//salva
		$resultado = $oContratoOrcamento->salvar($_POST);
	
		if($resultado[0]){
			$msg = utf8_decode('Operação realizada com sucesso!');
			direcionar('?modulo=principal/contrato/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/contrato/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else if($repetido){
		$msg = utf8_decode('Orçamento Já Cadastrado!');
		alerta($msg);
		$erro = 'orcamentoexiste';
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['ctoid'])){
	$contorc = $oContratoOrcamento->carrega_registro_por_id($_GET['ctoid']);
	extract($contorc);
}

$cttid = $_GET['id'];
$qtdOrcamentos = $oContratoOrcamento->pega_qtd_orcamentos($cttid);

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Ano de Exerc&iacute;cio','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>
		
	/**
	 * Submete o formulário
	 * @name salvar
	 * @return void
	 */	
	function salvar(){
		$('btnSalvar').disable();
		$('formularioCadastro').submit();
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('ctoanoexercicio').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Ano de Exercício" é obrigatório!'));
			alert(msg);
			$('ctoanoexercicio').focus();
			$('btnIncluir').enable();
	        return false;
		}

		if($('ctovalor').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Verba" é obrigatório!'));
			alert(msg);
			$('ctovalor').focus();
			$('btnIncluir').enable();
	        return false;
		}
		
		return true;
	}

	/**
	 * Volta à tela anterior
	 * @name voltar
	 * @return void
	 */	
	function voltar(){
		var cttid = $F('cttid');
		window.location = '?modulo=principal/contrato/cadHonorario&acao=A&id='+cttid;
	}


	/**
	 * Grava registro filho
	 * @name incluir
	 * @return void
	 */	
	function incluir(){
		$('btnIncluir').disable();
		if(validaFormularioCadastro()){
			$('requisicao').setValue('incluir');
			$('formularioCadastro').submit();
		}
	}


	/**
	 * Edição do registro
	 * @name detalharOrcamento
	 * @param int ctoid - identificador do registro
	 * @return void
	 */
	function detalharOrcamento(ctoid){
	    var cttid = $F('cttid');
	    window.location = '?modulo=principal/contrato/cadOrcamento&acao=A&id='+cttid+'&ctoid='+ctoid;
	}

	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Ano de Exerc&iacute;cio: </td>
			<td class="campo">
				<?php 
				if(empty($ctoanoexercicio)){
					$ctoanoexercicio = date('Y');
				}
				echo campo_texto('ctoanoexercicio','S','N','',6,4,'####','','left','',0,'id="ctoanoexercicio"');
				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Verba: </td>
			<td class="campo">
				<?php 
				if(empty($ctovalor) || (!empty($ctovalor) && $erro == 'orcamentoexiste')){
					echo campo_texto('ctovalor','S','S','',16,14,'[###.]###,##','','left','',0,'id="ctovalor"');
				}
				else{
					echo campo_texto('ctovalor','S','S','',16,14,'[###.]###,##','','left','',0,'id="ctovalor"','',number_format($ctovalor, 2, ',', '.'));
				}
				?>
            </td>
		</tr>
		<tr>
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
            	<input type='hidden' name='ctoid' id='ctoid' value='<?php echo $ctoid; ?>'>
            </td>
        </tr>
    </table>
</form>
    
    <!-- TRECHO DOS EXERCÍCIOS -->
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">   
		<?php
		//lista os orçamentos do contrato
		if(!empty($cttid) && $qtdOrcamentos >= 1){
		?>
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='4'> Lista de Exerc&iacute;cios </td>
		</tr>
	</table>
		<?php 	
			$oContratoOrcamento->listar_por_id($cttid);
		}
        ?>
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1"> 
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<?php 
            	//só mostra este trecho caso o fluxo NÃO seja advindo do ícone visualizar
        		if($_SESSION['visualizar'] != 'S'){
            	?>
            	<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='<< Anterior' onclick="javascript:voltar();">
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
            	<?php 
            	}
            	?>
            </td>
        </tr>
	</table>
	<!-- FIM TRECHO DOS EXERCÍCIOS -->
