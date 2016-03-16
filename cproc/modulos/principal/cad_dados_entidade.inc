<?php

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------

	switch ($_POST['requisicao']) {
		case 'cadastrar':
			cadastraEntidade($_POST);
			break;
			
		case 'alterar':
			alteraEntidade($_POST);
			break;
			
		case 'inativar':
			inativarEntidade($_POST['entid']);
			break;
	}

	if ($_REQUEST['entid']){
		$entid = $_REQUEST['entid'];
		
		$entidade = exibirEntidade($entid);
		
		extract($entidade);
	}

	include APPRAIZ . "includes/cabecalho.inc";

    $titulo_modulo = "Controle de Processos - Cadastro";
    $modulo = "CPROC";
    monta_titulo($titulo_modulo, $modulo);

    $habil = true;
    
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

	function limparDadosEntidade(){
		window.location.href = 'cproc.php?modulo=principal/cad_dados_entidade&acao=A';
	}

	function alteraEntidade(entid){
		window.location.href = 'cproc.php?modulo=principal/cad_dados_entidade&acao=A&entid='+entid;
	}

	function inativarEntidade(entid){
		if (confirm('Deseja inativar esta entidade?')){
			document.getElementById('requisicao').value = 'inativar';
			document.getElementById('entid').value = entid;

			document.formulario.submit();
		}
	}
	
	function salvarDadosEntidade(){
		var entdsc = document.getElementById('entdsc').value;

		if (entdsc == ""){
			alert('Informe o nome da entidade.');
			return;
		}

		document.formulario.submit();
	}
</script>

<form method="POST" id="formulario" name="formulario" action="">
    <?PHP if ($entid) { ?>
        <input type="hidden" id="requisicao" name="requisicao" value="alterar"/>
    <?PHP } else { ?>
        <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    <?PHP } ?>
        <input type="hidden" id="entid" name="entid" value="<?PHP echo $entid; ?>"/>
    
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
    	<tr>
    		<td class="subtituloDireita" width="20%" colspan="2">Nome da Entidade:</td>
    		<td>
                <?php
                    echo campo_texto('entdsc', 'S', 'S', 'Descrição', '51', '25', '', '', '', '', '', 'id="entdsc"', '', '', '');
                ?>
    		</td>
    	</tr>
    	<tr>
    		<td class="subtituloDireita" width="20%" colspan="2">Status da Entidade:</td>
    		<td>
    			<select id="entdtstatus" name="entdtstatus">
    				<option value="A" <?php echo $entdtstatus == 'A' ? 'selected="true"' : '' ?>>Ativo</option>
    				<option value="I" <?php echo $entdtstatus == 'I' ? 'selected="true"' : '' ?>>Inativo</option>
    			</select>
    		</td>
    	</tr>
        <tr>
            <td class="SubTituloCentro" colspan="3">
                <input type="button" name="salvarEntidade" id="salvarEntidade" value="Salvar" onclick="salvarDadosEntidade();"/>
                <input type="button" name="novaEntidade" id="novaEntidade" value="Novo" onclick="limparDadosEntidade();"/>
            </td>
        </tr>
    </table>
</form>

<div id="div_lista_entidade"><?php listarEntidade(); ?></div>