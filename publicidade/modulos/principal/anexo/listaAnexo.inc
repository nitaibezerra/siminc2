<?php
// incluindo as classes necessárias
$oAnexo = new Anexo();
$oArquivoPeca = new ArquivoPeca();

if($_GET['requisicao'] == 'down' && !empty($_GET['arqid']) && !empty($_GET['aneid'])){
	$oArquivoPeca->baixar($_GET['arqid'],$_GET['aneid']);
	direcionar('?modulo=principal/anexo/listaAnexo&acao=A');
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_ANEXAR_ARQUIVO);
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Pe&ccedil;a Publicit&aacute;ria','');

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
	
		var anexo = '';
		for(var i=0; i<document.formularioPesquisa.anexo.length; i++){
			if(document.formularioPesquisa.anexo[i].checked){
				anexo = document.formularioPesquisa.anexo[i].value;
			}
		}

		if($('padnumero').getValue() == '' && anexo == '') {
			var msg = decodeURIComponent(escape('É obrigatório o preenchimento de pelo menos um campo para a realização da pesquisa!'));
			alert(msg);
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
		window.location = '?modulo=principal/anexo/cadAnexo&acao=A&ipaid='+id;
	}

	/**
	 * Baixar um arquivo do anexo
	 * @name baixar
	 * @param int arqid - Identificador do registro
	 * @param int aneid - Identificador do registro pai
	 * @return void
	 */
	function baixar(arqid,aneid){
		window.location = '?modulo=principal/anexo/listaAnexo&acao=A&arqid='+arqid+'&aneid='+aneid+'&requisicao=down';
	}	

</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> N&ordm; PAD: </td>
			<td class="campo">
				<?=campo_texto('padnumero','N','S','Informe o Nº PAD',10,8,'####/###','','left','',0,'id="padnumero"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Arquivo Anexo: </td>
			<td class="campo">
				<?php echo campo_radio_sim_nao('anexo','h'); ?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">
            </td>
        </tr>
	</table>
</form>
<?php
//verifica se é para disparar a consulta no banco por filtro ou buscar todos
if($_POST['requisicao'] != 'filtrar'){
	$_POST['anexo'] = '0';
}
$oAnexo->listar_por_filtro($_POST);
?>
