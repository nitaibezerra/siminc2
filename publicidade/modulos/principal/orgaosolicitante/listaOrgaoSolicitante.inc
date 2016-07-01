<?php
// incluindo as classes necessárias
$oOrgaoSolicitante = new OrgaoSolicitante();

//caso seja para excluir
if($_POST['requisicao'] == 'excluir'){
	$oOrgaoSolicitante->inativar($_POST['orgid']);
	$msg = 'Exclus�o realizada com sucesso!';
	direcionar('?modulo=principal/orgaosolicitante/listaOrgaoSolicitante&acao=A',$msg);
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
$msg = '�rg�o Solicitante';
monta_titulo($msg,'');

extract($_POST);
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Aciona o filtro
	 * @name pesquisar
	 * @param requisicao - Requisição que será executada
	 * @return void
	 */	
	function pesquisar(requisicao){
		$('btnPesquisar').disable();
		if(validaFormularioPesquisa()){
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
	
		if($('orgsg').getValue() == '' && $('orgdsc').getValue() == '') {
			alert('Obrigat�rio preenchimento de um item da pesquisa!');
			$('btnPesquisar').enable();
	        return false;
		}
	
		return true;
	}
	
	/**
	 * Redireciona para a edição
	 * @name detalhar
	 * @param id - Id do registro
	 * @return void
	 */
	function detalhar(id){
		window.location = '?modulo=principal/orgaosolicitante/cadOrgaoSolicitante&acao=A&id='+id;
	}
	
	/**
	 * Redireciona para a exclusão
	 * @name remover
	 * @param id - Id do registro
	 * @return void
	 */
	function remover(id){
		 $('requisicao').setValue('excluir');
		 $('orgid').setValue(id);
		 $('formularioPesquisa').submit();
	}

	/**
	 * Função responsável por direcionar à tela de inclusão
	 * @name adicionar
	 * @return void
	 */
	function adicionar(){
		window.location = '?modulo=principal/orgaosolicitante/cadOrgaoSolicitante&acao=A';
	}	
</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Sigla: </td>
			<td class="campo">
				<?=campo_texto('orgsg','N','S','Informe a Sigla',30,30,'','','left','',0,'id="orgsg"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> �rg�o: </td>
			<td class="campo">
				<?=campo_texto('orgdsc','N','S','Informe o �rg�o',100,255,'','','left','',0,'id="orgdsc"');?>
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
        		<input type='hidden' name='orgid' id='orgid' />
            	<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">
            	<input type='button' class="botao" name='btnAdicionar' id='btnAdicionar' value='Adicionar' onclick="javascript:adicionar();">
            </td>
        </tr>
	</table>
</form>
<?php
//verifica se é para disparar a consulta no banco por filtro ou buscar todos
if($_POST['requisicao'] == 'filtrar'){
	$oOrgaoSolicitante->listar_por_filtro($_POST);
}
else{
	$oOrgaoSolicitante->listar_por_filtro('');
}
?>
