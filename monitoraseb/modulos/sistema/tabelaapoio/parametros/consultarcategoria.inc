<?php
include APPRAIZ."monitoraseb/classes/Categoria.class.inc";
$categoria = new Categoria();
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Categoria de Membro de Equipe','');
$mensagem = "";

if($_REQUEST['idCategoriaExclusao'] !=null && !empty($_REQUEST['idCategoriaExclusao'])){
	$mensagem = $categoria->excluirCategoria($_REQUEST['idCategoriaExclusao']);
	if($mensagem != ""){?>
		<script>
			alert('<?=$mensagem?>');
		</script>
	<?php }
}
if($_REQUEST['requisicao']=='salvar'){
	$mensagem = $categoria->salvarCategoria($_REQUEST[catid],
									$_REQUEST[catnome],
									$_REQUEST[catvalidacao],
									$_REQUEST[catbolsista],
									$_REQUEST[catnumdoclegal],
									$_REQUEST[catvalunibolsa]);

	if($mensagem != ""){?>
		<script>
			alert('<?=$mensagem?>');
		</script>
	<?php }
}
?>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/monitoraseb.js"></script>
<script type="text/javascript" language="">

	function fadeOut(id, time) {
		var target = document.getElementById(id);
		var alpha = 500;
		var i = setInterval(
		function() {
			if (alpha <= 0){
				clearInterval(i);
			}
			if(alpha==0){
				target.style.display = 'none';
			}
			setAlpha(target, alpha);
			alpha -= 2;
		}, time);
	}
	function setAlpha(target, alpha) {
		if(alpha<100){
		target.style.filter = "alpha(opacity="+ alpha +")";
		target.style.opacity = alpha/100;
		}
	}


	function gravar(){
		if(validaFormulario()){
			document.formularioCadastro.submit();
		}
	}

	function validaFormulario(){
		var formCategoria = document.formularioCadastro;
		if(formCategoria.elements['catnome'].value == "") {
	        formCategoria.elements['catnome'].focus();
	        alert('O campo "Nome" é obrigatório!');
	        document.getElementById('btnGravar').disabled=false;
	        return false;
	    }
		if(formCategoria.elements['catnumdoclegal'].value == "" && formCategoria.elements['catnumdoclegal'].className != 'disabled') {
	        formCategoria.elements['catnumdoclegal'].focus();
	        alert('O campo "Número do Documento Legal" é obrigatório!');
	        document.getElementById('btnGravar').disabled=false;
	        return false;
	    }
		if(formCategoria.elements['catvalunibolsa'].value == "" && formCategoria.elements['catvalunibolsa'].className != 'disabled') {
	        formCategoria.elements['catvalunibolsa'].focus();
	        alert('O campo "Valor Unitário da Bolsa" é obrigatório!');
	        document.getElementById('btnGravar').disabled=false;
	        return false;
	    }
		return true;
	}
	function pesquisar(acao){
		var url = 'monitoraseb.php?modulo=sistema/tabelaapoio/parametros/consultarcategoria&acao=A&pesquisar='+acao;
		var formularioPesquisa=document.getElementById("formularioPesquisa");
		formularioPesquisa.action = url;
		formularioPesquisa.submit();
	}

	function detalharCategoria(idCategoria){
		mostraCadastro(idCategoria);
	}

	function removerCategoria(idCategoria){
		var formCategoria = document.formularioPesquisa;
		formCategoria.elements['idCategoriaExclusao'].value = idCategoria;
		formCategoria.submit();
	}

	function habilitaDesabilitaValorBolsa(valor){
        var campoValorBolsa = document.getElementsByName('catvalunibolsa').item(0);
        campoValorBolsa.value='';
        campoValorBolsa.disabled = (valor == 'N'|| valor == 'n')? true : false;
        campoValorBolsa.className = (valor == 'N'|| valor == 'n')? 'disabled' : 'obrigatorio normal';
        campoValorBolsa.readOnly = (valor == 'N'|| valor == 'n')? true : false;

        var campoDocLegal = document.getElementsByName('catnumdoclegal').item(0);
        campoDocLegal.value='';
        campoDocLegal.disabled = (valor == 'N'|| valor == 'n')? true : false;
        campoDocLegal.className = (valor == 'N'|| valor == 'n')? 'disabled' : 'obrigatorio normal';
        campoDocLegal.readOnly = (valor == 'N'|| valor == 'n')? true : false;
	}

	function mostraCadastro(idCategoria){
	    var div_on = document.getElementById( 'catid_on' );
	    var div_off = document.getElementById( 'catid_off' );
	    if(div_on && div_off){
	        div_on.style.display = 'block';
	        div_off.style.display = 'none';

	        return new Ajax.Updater(div_on, 'ajax.php',
	                {
	                    method: 'post',
	                    parameters: '&servico=mostraCadastro&idCategoria='+idCategoria,
	                    onComplete: function(res){ }
	                });
	    }
	}


</script>
<form method="POST"  name="formularioCadastro" action="" id="formularioCadastro">
        <?php if($categoria->getMessage()): ?>
	<table id="tb_msg_sucess" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr class="center SubtituloTabela" >
              	<th >
                    <?php echo $categoria->getMessage() ?>
            	</th>
            </tr>
    </table>
            <script>
            	fadeOut('tb_msg_sucess',20);
           	</script>
        <?php endif; ?>
	<div id="catid_on" style="display:none;" align="left"></div>
	<div id="catid_off" style="color:#909090;"></div>
	<script>mostraCadastro(null);</script>
</form>
<form method="POST"  name="formularioPesquisa" action="" id="formularioPesquisa">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Busca:</td>
			<td>
				<input style="cursor: pointer;"  type="text" name="campoPesquisa">
				<input type="hidden" name="idCategoriaExclusao" value="">
				<input style="cursor: pointer;"  type="button" value="OK" name="btnPesquisa" onclick="pesquisar('filtrar');">
			</td>
		</tr>
	</table>
</form>
<?php
	$categoria->buscaListaCategoria($_REQUEST['campoPesquisa']);
?>
