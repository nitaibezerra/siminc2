<?php 
include_once '_funcoes_maismedicos.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['requisicao']=='excluir'){
    header('Content-Type: text/html; charset=iso-8859-1');
    excluirDoc($_REQUEST['arquivo']);
    exibirListaDoc();
    exit();
}

if($_REQUEST['requisicao']=='cadastrar'){
    salvarDoc($_FILES,$_POST);
    exit();	
}

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A';</script>";
    exit;
}

include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A');
monta_titulo( $titulo_modulo, recuperaMunicipioEstado());
$_SESSION['maismedicos'] = true; 

if($_SESSION['par']['rqmid']){
    $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);	
}

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">
	function validarDocumento(){
		if($('[name=arquivo]').val() == ''){
			alert('O campo "Arquivo" é obrigatório!');
			$('[name=arquivo]').focus();
			return false;
		}
		if($('[name=tpaid]').val() == ''){
			alert('O campo "Tipo de Arquivo" é obrigatório!');
			$('[name=tpaid]').focus();
			return false;
		}
		if($('[name=arqdescricao]').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			$('[name=arqdescricao]').focus();
			return false;
		}		
		return true;		
	}

	function excluirDoc(arqid){
            if(confirm("Deseja realmente excluir o Arquivo?")){
                $.ajax({
                    url: 'par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A',
                    data: { requisicao: 'excluir', arquivo: arqid},
                    async: false,
                    success: function(data) {
                                    alert('Arquivo excluído com sucesso!');
                                    $("#div_documento").html(data);
                    }
                });
            }
	}	
</script>

<form id="formulario" method="post" name="formulario" action="par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A" enctype="multipart/form-data" onsubmit="return validarDocumento();">
    <input type="hidden" id="rqmid" name="rqmid" value="<?php echo $_SESSION['par']['rqmid']; ?>"/>
    <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
        <tr>
            <td class="subtituloDireita">Arquivo:</td>
            <td>
                <input type="file" name="arquivo" />&nbsp;
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
            <td>
                <?php
                    if($docid){
                        echo '<td align="right" valign="top" width="10%" rowspan="5">';
                        wf_desenhaBarraNavegacao($docid, array('rqmid' => $_SESSION['cap']['rqmid']), '');
                        echo '</td>';
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Tipo de Arquivo:</td>
            <td>
                <?php
                $sql = "SELECT  	tpaid AS codigo, 
                        	    	tpadsc AS descricao
                    	FROM 		par.tipoarquivo
                    	ORDER BY 	tpadsc";
                $db->monta_combo('tpaid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
                ?>			
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Descrição:</td>
            <td><?php echo campo_textarea('arqdescricao', 'S', 'S', '', '80', '5', '255'); ?></td>
        </tr>		
        <tr>
            <td align="center" bgcolor="#CCCCCC" colspan="4">
                <input type="submit" value="Salvar" id="btnSalvar"/>
            </td>
        </tr>
    </table>
</form>

<br>

<div id="div_documento"><?php exibirListaDoc(); ?></div>

<script type="text/javascript" language="javascript">
	<?php if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) ||  in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){  ?>
		jQuery("input,select,textarea").attr('disabled','disabled');
	<?php } ?>
</script>
