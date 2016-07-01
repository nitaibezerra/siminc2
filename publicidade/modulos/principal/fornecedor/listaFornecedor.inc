<?php
// incluindo as classes necessárias
$oFornecedor = new Fornecedor();
$oEstado = new Estado();

//caso seja para excluir
if($_POST['requisicao'] == 'excluir'){
	$oFornecedor->inativar($_POST['forid']);
	$msg = utf8_decode('Exclus�o realizada com sucesso!');
	direcionar('?modulo=principal/fornecedor/listaFornecedor&acao=A',$msg);
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Fornecedor','');

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
	
		var foragencia = 2;
		for(var i=0; i<document.formularioPesquisa.foragencia.length; i++){
			if(document.formularioPesquisa.foragencia[i].checked){
				foragencia = document.formularioPesquisa.foragencia[i].value;
			}
		}
		
		if($('fornome').getValue() == '' && $('forcnpj').getValue() == '' && $('estuf').getValue() == '' && foragencia == 2) {
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
		window.location = '?modulo=principal/fornecedor/cadFornecedor&acao=A&forid='+id;
	}

	/**
	 * Redireciona para a exclusão
	 * @name remover
	 * @param id - Id do registro
	 * @return void
	 */
	function remover(id){
		 $('requisicao').setValue('excluir');
		 $('forid').setValue(id);
		 $('formularioPesquisa').submit();
	}

	/**
	 * Função responsável por direcionar à tela de inclusão
	 * @name adicionar
	 * @return void
	 */
	function adicionar(){
		window.location = '?modulo=principal/fornecedor/cadFornecedor&acao=A';
	}	
</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Raz�o Social: </td>
			<td class="campo">
				<?=campo_texto('fornome','N','S','Informe a Raz�o Social',100,255,'','','left','',0,'id="fornome"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CNPJ: </td>
			<td class="campo">
				<?=campo_texto('forcnpj','N','S','Informe o CNPJ',18,18,'','','left','',0,'id="forcnpj"', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);',null,"verificaSomenteNumerosCpfCnpj(this,'O CNPJ deve ser num�rico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo"><?php $oEstado->monta_combo_uf();?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Ag�ncia:</td>
			<td class="campo">
				<?php echo campo_radio_sim_nao('foragencia','h'); ?>
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
        		<input type='hidden' name='forid' id='forid' value='<?php echo $forid; ?>' />
            	<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">
            	<input type='button' class="botao" name='btnAdicionar' id='btnAdicionar' value='Adicionar' onclick="javascript:adicionar();">
            </td>
        </tr>
	</table>
</form>
<?php 
//verifica se é para disparar a consulta no banco por filtro ou buscar todos
if($_POST['requisicao'] == 'filtrar'){
	$oFornecedor->listar_por_filtro($_POST);
}
else{
	$_POST['foragencia'] = 0;
	$oFornecedor->listar_por_filtro($_POST);
}
?>