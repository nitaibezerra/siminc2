<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
loadStaticScripts();

/**
 * Upload de arquivos, apenas com extensao PDF
 */
if (isset($_FILES) && count($_FILES) > 0) {
    
    $fileinfo = pathinfo($_FILES['fileadd']['name']);
    $arquivoSalvo = false;
    
    if($_FILES['fileadd']['error']=='0' && strtolower($fileinfo['extension']) == UPLOAD_VALID_EXENSION) {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file = new FilesSimec("obra", array() ,"pnbe");

        $arquivoSalvo = $file->setUpload($_POST['descricao'], null, false);
    }
    
    if ($arquivoSalvo) {
        updateParecerObra($file->getIdArquivo(), $_POST['obrid']);
        ?>
        <script> notice("Arquivo anexado com sucesso!"); </script>
        <?
    } else {
        ?>
        <script> notice("Falha ao tentar subir arquivo, somente extensão PDF."); </script>
        <?
    }
}

/**
 * Apaga um determinado arquivo do servidor
 */
if (isset($_GET['act'])) {
    
    if (($_GET['act'] == 'detach') && isset($_GET['fileid']) && isset($_GET['obrid'])) {
        if (apagaArquivoUpload($_GET['fileid'])) {
            updateParecerObra('NULL', $_GET['obrid']);
            header('location: pnbe.php?modulo=principal/anexarparecer&acao=A&act=attach&msg=success&obrid='.$_GET['obrid'].'&urlback='.$_GET['urlback'].'&acao=A');
        }
        ?> <script> notice("Erro ao tentar apagar o arquivo!"); </script> <?
    }
    
    if (isset($_GET['msg']) && $_GET['msg'] == 'success') {
        ?> <script> notice("Arquivo apagado com sucesso!"); </script> <?
    }
}

montaTopoPNBE(PNBE_MODULE_TITLE, $abacod_tela, $url, $parametros, 'PARECER PRÉ-ANALISE');
if (isset($_GET['obrid'])) :
    $strSqlObra = sprintf('SELECT c.catdescricao, o.obrcodigo, 
        o.obrtitulo, e.edirazaosocial, o.obrautor, o.obrano
        FROM pnbe.obra o 
        INNER JOIN pnbe.categoria c ON (o.catid = c.catid)
        INNER JOIN pnbe.editora e ON (o.ediid = e.ediid)
        WHERE obrid=%d', (int)$_GET['obrid']);

    $cabecalho = array('Categoria', 'Código da obra', 'Título da obra', 'Editora', 'Autor', 'Ano');
    $db->monta_lista($strSqlObra,$cabecalho,50,20,'','','');
    $rs = $db->carregar("SELECT o.arqid FROM pnbe.obra o WHERE o.obrid=".(int)$_GET['obrid']);
?>
    <form method="POST" action="" name="frmAnexar" enctype="multipart/form-data">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td width="30%" align="left" class="SubTituloDireita">Somente arquivos com extensão PDF:</td>
                <td align="left">
                    <input type="hidden" name="obrid" id="obrid" value="<?=$_GET['obrid'];?>"/>
                    <input type="file" name="fileadd" <?=(!empty($rs[0]['arqid']))?'disabled':'';?> id="fileadd" size="30"/>
                </td>
            </tr>
            <tr>
                <td width="30%" align="left" class="SubTituloDireita">Descrição:</td>
                <td align="left">
                    <input type="text" name="descricao" <?=(!empty($rs[0]['arqid']))?'disabled':'';?> id="descricao" size="70" />
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2">
                    <center>
                        <input type="button" <?=(!empty($rs[0]['arqid']))?'disabled':'';?> class="btnAttach" value="Salvar">&nbsp;&nbsp;
                        <input type="button" class="btnCancel" value="Voltar">
                    </center>
                </td>
            </tr>
            <tr>
                <td align="left" colspan="2">
                    <? if ($rs) : ?>
                        <? if (!empty($rs[0]['arqid'])): ?>
                            <? $row = getArquivoParecer($rs[0]['arqid']); ?>
                            <img 
                                target-file-id="<?=$rs[0]['arqid']?>" 
                                src="../imagens/excluir.gif" 
                                title="Excluir Parecer" 
                                class="x-icon file-del"
                            />
                            &nbsp;&nbsp;
                            <?= $row[0]['arqdescricao']; ?>
                        <? endif; ?>
                    <? endif; ?>
                </td>
            </tr>
        </table>
    </form>

    <script type="text/javascript">
    $(document).ready(function(){
        $(".btnAttach").on("click", function(e){
            e.preventDefault();
            
            var isValid = true;
            
            if (!$("#fileadd").val()) {
                notice("Anexe um arquivo PDF para upload");
                isValid = false;
            }
            
            if (!$("#descricao").val()) {
                notice("Adicione uma descrição para o arquivo");
                isValid = false;
            }
            
            (isValid) ? document.forms.frmAnexar.submit() : null;
        });
        
        $(".btnCancel").on("click", function(e){
            e.preventDefault();
            
            var queryString = location.href.split("&urlback=");
            location.href=queryString[1];
        });
        
        $(".file-del").on("click", function(e){
            e.preventDefault();
            
            if (!confirm("Deseja realmente excluir esse arquivo?")) {
                return false;
            }
            
            var targetFile = $(this).attr("target-file-id")
              , urlback = location.href.split("&urlback=")[1]
              , urlString = location.href.replace(/&act=\w+/g, "&act=detach");
            
            location.href=urlString+"&fileid="+targetFile+'&urlback='+urlback;
            
            //console.log(urlback[1]);
        });
    });
    </script>
<? endif; ?>