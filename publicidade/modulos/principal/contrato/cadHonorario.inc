<?php
// incluindo as classes necessarias
$oContrato = new Contrato();
$oTipoServico = new TipoServico();
$oContratoHonorario = new ContratoHonorario();

if(!empty($_SESSION['cttid']) && empty($_GET['id'])){
	$_GET['id'] = $_SESSION['cttid'];
}
else if(empty($_SESSION['cttid']) && !empty($_GET['id'])){
	$_SESSION['cttid'] = $_GET['id'];
}

//caso tenha postado o formulario
if($_POST['requisicao'] == 'incluir'){
	
	//verifica se já existe o registro
	$repetido = $oContratoHonorario->verifica_unico($_POST);
	
	//caso não encontre repetido, salva
	if(!$repetido){
		//salva
		$resultado = $oContratoHonorario->salvar($_POST);
	
		if($resultado[0]){
			$msg = 'Operação realizada com sucesso!';
			direcionar('?modulo=principal/contrato/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = 'Operação falhou!';
			direcionar('?modulo=principal/contrato/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else if($repetido){
		$msg = 'Honorário Já Cadastrado!';
		alerta($msg);
		extract($_POST);
	}
}
else if($_GET['requisicao'] == 'excluir'){
	$oContratoHonorario->inativar($_GET['cthid']);
	$msg = 'Operação realizada com sucesso!';
	direcionar('?modulo=principal/contrato/cadHonorario&acao=A&id='.$_GET['id'],$msg);
}

$cttid = $_GET['id'];
if($_SESSION['visualizar'] != 'S'){
	$qtdHonorarios = $oContratoHonorario->pega_qtd_honorarios($cttid,'A');
}
else if($_SESSION['visualizar'] == 'S'){
	$qtdHonorarios = $oContratoHonorario->pega_qtd_honorarios($cttid);
}


// monta cabecalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Honorário','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>
		
	/**
	 * Submete o formulario
	 * @name salvar
	 * @return void
	 */	
	function salvar(){
		//$('btnIncluir').disable();
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
	
		/*
		if($('cthhonorario').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Honorário" é obrigatório!'));
			alert(msg);
			$('cthhonorario').focus();
			$('btnIncluir').enable();
	        return false;
		}

		var tseid = '';
		for(var i=0; i<document.formularioCadastro.tseid.length; i++){
			if(document.formularioCadastro.tseid[i].checked){
				var tseid = document.formularioCadastro.tseid[i].value
			}
		}
		if(tseid == '') {
			var msg = decodeURIComponent(escape('O campo "Tipo de Serviço" é obrigatório!'));
			alert(msg);
			$('btnIncluir').enable();
	        return false;
		}

		if($('cthdsc').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Descrição" é obrigatório!'));
			alert(msg);
			$('cthdsc').focus();
			$('btnIncluir').enable();
	        return false;
		}
		*/
		
		if(document.formularioCadastro.cthhonorario.value == ''){
			alert('O campo "Honorário" é obrigatório!');
			document.formularioCadastro.cthhonorario.focus();
		 	return false;
		}
		if(document.formularioCadastro.tseid[0].checked == false && document.formularioCadastro.tseid[1].checked == false){
			alert('O campo "Tipo de Serviço" é obrigatório!');
			document.formularioCadastro.tseid[0].focus();
		 	return false;
		}
		if(document.formularioCadastro.cthdsc.value == ''){
			alert('O campo "Descrição" é obrigatório!');
			document.formularioCadastro.cthdsc.focus();
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
		//$('btnIncluir').disable();
		if(validaFormularioCadastro()){
			$('requisicao').setValue('incluir');
			$('formularioCadastro').submit();
		}
	}

	/**
	 * Volta a  tela anterior
	 * @name voltar
	 * @return void
	 */	
	function voltar(){
		var cttid = $('cttid').getValue();
		window.location = '?modulo=principal/contrato/cadContrato&acao=A&id='+cttid;
	}

	/**
	 * Inativa o registro
	 * @name removerHonorario
	 * @param int id - identificador do registro
	 * @return void
	 */
	function removerHonorario(id){
		 var cttid = $('cttid').getValue();
		 window.location = '?modulo=principal/contrato/cadHonorario&acao=A&cthid='+id+'&requisicao=excluir&id='+cttid;
	}

    /**
     * Direciona para a tela de orçamento
     * @name avancar
     * @return void
     */
     function avancar(){
         var id = $F('cttid');
         window.location = '?modulo=principal/contrato/cadOrcamento&acao=A&id='+id;	
     }

	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Honorário %: </td>
			<td class="campo">
				<?=campo_texto('cthhonorario','S','S','Informe o Honorário',8,6,'###,##','','left','',0,'id="cthhonorario"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tipo de Serviço:</td>
			<td class="campo">
				<?php 
				$tipos = $oTipoServico->carrega_registros();
				if(is_array($tipos) && count($tipos) >= 1){
					foreach($tipos as $key => $value){
				?>
				<input type='radio' name='tseid' id='tseid' value='<?=$value['tseid']?>' <?php if($tseid == $value['tseid']){ echo 'checked'; } ?>><?=$value['tsedsc']?>
				<?php 
					}
				}
				?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descrição: </td>
			<td class="campo">
				<?php echo campo_textarea('cthdsc','S', 'S', '', 54, 6, 200,'','','','','',''); ?>
            </td>
		</tr>
		<tr>
        	<td colspan='2' align='center' class="campo">
            	<?php 
            	//se mostra este trecho caso o fluxo não seja advindo do i­cone visualizar
            	//if($_SESSION['visualizar'] != 'S'){
            	?>
            	<input type='button' class="botao" name='btnIncluir' id='btnIncluir' value='Incluir' onclick="javascript:incluir();">
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<?php 
            	//}
            	?>
            	<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid; ?>'>
            	<input type='hidden' name='cthid' id='cthid' value='<?php echo $cthid; ?>'>
            	<input type='hidden' name='qtdHonorarios' id='qtdHonorarios' value='<?php echo $qtdHonorarios; ?>'>
            </td>
        </tr>
    </table>
</form>
    
    <!-- TRECHO DOS HONORáRIOS -->
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1"> 
		<?php
		//lista os honorarios do contrato
		if(!empty($cttid) && $qtdHonorarios >= 1){
		?>
		<tr id='linhatitulolistahonorario'>
			<td bgcolor='#e9e9e9' align='center' colspan='4'> Lista de Honorários </td>
		</tr>
    </table>
		<?php 	
			if($_SESSION['visualizar'] != 'S'){
				$oContratoHonorario->listar_por_id($cttid,'A');	
			}
			else if($_SESSION['visualizar'] == 'S'){
				$oContratoHonorario->listar_por_id($cttid);	
			}
			
		}
        ?>
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<?php 
            	//se mostra este trecho caso o fluxo NaO seja advindo do i­cone visualizar
        		if($_SESSION['visualizar'] != 'S'){
            	?>
            	<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='<< Anterior' onclick="javascript:voltar();">
            	<input type='button' class="botao" name='btnAvancar' id='btnAvancar' value='Próxima >>' onclick="javascript:avancar();">
            	<?php 
            	}
            	?>
            </td>
        </tr>
	</table>
	<!-- FIM TRECHO DOS HONORaRIOS -->
