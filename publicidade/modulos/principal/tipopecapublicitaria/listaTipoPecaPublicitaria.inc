<?php
// incluindo as classes necess√°rias
$oTipoPeca = new TipoPeca();

//caso seja para excluir
if($_POST['requisicao'] == 'excluir'){
	$oTipoPeca->inativar($_POST['tppid']);
}

// monta cabe√ßalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o t√≠tulo da tela
monta_titulo('Tipo de PeÁa Publicit·ria','');

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
	 * @param requisicao - Requisi√ß√£o que ser√° executada
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
	 * Valida os campos obrigat√≥rios
	 * @name validaFormularioPesquisa
	 * @return bool
	 */	
	function validaFormularioPesquisa(){
	
		var tppperiodo = '';
		tppperiodo = $$('input[name=tppperiodo]').filter(function(el){
		    return $F(el);
		}).size();

		if($('tppdsc').getValue() == '' && tppperiodo < 1) {
			alert('ObrigatÛrio preenchimento de um item da pesquisa!');
			$('btnPesquisar').enable();
	        return false;
		}
	
		return true;
	}

	/**
	 * Redireciona para a edi√ß√£o
	 * @name detalhar
	 * @param id - Id do registro
	 * @return void
	 */
	function detalhar(id){
		window.location = '?modulo=principal/tipopecapublicitaria/cadTipoPecaPublicitaria&acao=A&id='+id;
	}

	/**
	 * Redireciona para a exclus√£o
	 * @name remover
	 * @param id - Id do registro
	 * @return void
	 */
	function remover(id){
		 $('requisicao').setValue('excluir');
		 $('tppid').setValue(id);
		 $('formularioPesquisa').submit();
	}

	/**
	 * Fun√ß√£o respons√°vel por direcionar √† tela de inclus√£o
	 * @name adicionar
	 * @return void
	 */
	function adicionar(){
		window.location = '?modulo=principal/tipopecapublicitaria/cadTipoPecaPublicitaria&acao=A';
	}	
</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Tipo de PeÁa Publicit·ria: </td>
			<td class="campo">
				<?=campo_texto('tppdsc','N','S','Informe o Tipo de PeÁa Publicit·ria',100,255,'','','left','',0,'id="tppdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Possui PerÌodo de VeiculaÁ„o? </td>
			<td class="campo">
				<input type='radio' name='tppperiodo' id='tppperiodo' value='S' <?php if($tppperiodo == 'S'){ echo 'checked'; } ?>>Sim
				<input type='radio' name='tppperiodo' id='tppperiodo' value='N' <?php if($tppperiodo == 'N' || empty($tppperiodo)){ echo 'checked'; } ?>>N„o
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
        		<input type='hidden' name='tppid' id='tppid' value='<?php echo $tppid; ?>' />
            	<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">
            	<input type='button' class="botao" name='btnAdicionar' id='btnAdicionar' value='Adicionar' onclick="javascript:adicionar();">
            </td>
        </tr>
	</table>
</form>
<?php
//verifica se √© para disparar a consulta no banco por filtro ou buscar todos
if($_POST['requisicao'] != 'filtrar'){
	$_POST['tppperiodo'] = 'N';	
}

$oTipoPeca->listar_por_filtro($_POST);
?>