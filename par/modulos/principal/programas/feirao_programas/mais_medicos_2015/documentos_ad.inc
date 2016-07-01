<?php
    include_once '_funcoes_maismedicos_2015.php';
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
        echo"<script>window.location.href = 'par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A';</script>";
        exit;
    }

    include_once APPRAIZ . "includes/workflow.php";
    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';
    monta_titulo("Mais Médico - Edital nº 1/2015", $titulo_modulo.' <br> '.recuperaMunicipioEstado());
    
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A');

    if($_SESSION['par']['rqmid']){
        $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);
    }
    
    $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
    
    $habilitado = verificaDataFechamento();
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
        if( confirm("Deseja realmente excluir o Arquivo?") ){
            $.ajax({
                url: 'par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A',
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

<form id="formulario" method="post" name="formulario" action="par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A" enctype="multipart/form-data" onsubmit="return validarDocumento();">
    <input type="hidden" id="rqmid" name="rqmid" value="<?php echo $_SESSION['par']['rqmid']; ?>"/>
    <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
        <tr>
            <td class="subtituloDireita">Arquivo:</td>
            <td>
                <?PHP
                    if( $habilitado == 'S' ){
                        $disabled = '';
                    }else{
                        $disabled = 'disabled="disabled"';
                    }
                ?>
                <input type="file" name="arquivo" <?=$disabled;?>>
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
                    $sql = "
                        SELECT  tpaid AS codigo,
                                tpadsc AS descricao
                        FROM par.tipoarquivo
                        ORDER BY tpadsc
                    ";
                    $db->monta_combo('tpaid', $sql, $habilitado, 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Descrição:</td>
            <td><?php echo campo_textarea('arqdescricao', 'S', $habilitado, '', '80', '5', '255'); ?></td>
        </tr>
        <tr>
            <td align="center" bgcolor="#CCCCCC" colspan="4">
                <?PHP
                    if( $habilitado == 'S' ){
                ?>
                        <input type="submit" value="Salvar" id="btnSalvar">
                <?PHP
                    }else{
                ?>
                        <input type="submit" value="Salvar" id="btnSalvar" disabled="disabled">
                <?PHP
                    }
                ?>                     
            </td>
        </tr>
    </table>
</form>

<br>

<div id="div_documento"><?php exibirListaDoc(); ?></div>
