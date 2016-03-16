<?php
include  APPRAIZ.'includes/cabecalho.inc';
loadStaticScripts();

function tramita(array $collection) {
    global $db;
    
    $strSqlDel = sprintf("DELETE FROM pnbe.obrahomologacao WHERE obrid=%s AND homolano='%s'", $collection['obrid'], $collection['ano']);
    $strSqlInsert = sprintf("INSERT INTO pnbe.obrahomologacao(obrid, homolstatus, homolano) VALUES (%d, '%s', '%s')", $collection['obrid'], $collection['status'], $collection['ano']);
    
    $db->executar($strSqlDel);
    $db->executar($strSqlInsert);
    $db->commit();
}

function homolStatusButton() {
    global $db;

    $statussBtn = array();
    $strQuery = "SELECT COUNT('obrfechahomol') FROM pnbe.obra WHERE obrano = '{$_SESSION['exercicio']}'  and avaid = '2' and obrfechahomol='F'";
    $botaoH = $db->pegaUm($strQuery);
    if ($botaoH <= 0) {
        $strSqlQuery = "SELECT COUNT('obrfechahomol') FROM pnbe.obra WHERE obrano = '{$_SESSION['exercicio']}'  and avaid = '2'";
        $botaoH = $db->pegaUm($strSqlQuery);

        if ($botaoH && $botaoH != 0) {
            $statussBtn['class'] = 'enable';
            $statussBtn['text'] = 'Fechar Homologação';
        } elseif ($botaoH == 0) {
            $statussBtn['class'] = 'nenhum';
            $statussBtn['text'] = 'Fechar Homologação';
        } else {
            $statussBtn['class'] = 'disable';
            $statussBtn['text'] = 'Homologação finalizada';
        }
    }else{
        $statussBtn['class'] = 'disable';
        $statussBtn['text'] = 'Homologação finalizada';
    }

    return $statussBtn;
}

if (isSet($_GET['fechaHomologacao'])) {
    
    $status = ($_GET['fechaHomologacao']=='enable') ? 'F' : 'A';
    $message = ($_GET['fechaHomologacao']=='enable') ? 'Homologação fechada' : 'Homologação aberta';
    $db->executar("UPDATE pnbe.obra SET obrfechahomol='{$status}' WHERE obrid > 0");
    $db->commit();
    
    echo '<script type="text/javascript">';
    echo 'notice("'.$message.'");';
    echo '</script>';
}

if (isAjax()) {
    if (isSet($_GET['act'])) {
        switch ($_GET['act']) {
            case 'tramita':
                tramita($_GET);
                break;
            default:
        }
    }
    exit;
}


$strSQLHomol = "SELECT COUNT('obrfechaavalia') FROM pnbe.obra WHERE obrfechaavalia='F'";
if (!$db->pegaUm($strSQLHomol)) {
    echo '<script type="text/javascript">';
    echo 'notice("A AVALIAÇÃO AINDA NÃO FOI FECHADA, POR FAVOR AGUARDE!");';
    echo '</script>';
}

montaTopoPNBE(PNBE_MODULE_TITLE, $abacod_tela, $url, $parametros, 'HOMOLOGAÇÃO');
$strSqlCategories = "SELECT catid AS codigo, catdescricao AS descricao FROM pnbe.categoria ORDER BY catdescricao";
?>
<form method="POST" action="" name="frmHomol">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" class="SubTituloDireita">Informe a categoria:</td>
            <td align="left">
                <?php $db->monta_combo('catid',$strSqlCategories,'','- Selecione -','','','','','','filterPublisher','',getFilterParam('catid','')); ?>
                <input type="button" class="btn-consulta" value="Consultar">
            </td>
            <td align="center">
                <?php $statusBtn = homolStatusButton();
                if ($statusBtn['class']== 'nenhum'){
                    $btnDisabled = 'disabled';
                }
?>
                <input type="button" <?= $btnDisabled; ?> class="btn-fHomol <?= $statusBtn['class']; ?>" value="<?= $statusBtn['text']; ?>" <?php echo exercicioCorrente();?>>
            </td>
        </tr>
    </table>
</form>
<?php

$disabled = ($statusBtn['class'] == 'disable') ? 'disabled' : '';

$strSqlBase = "SELECT 
    CASE 
        WHEN oh.homolstatus = 'A' THEN 
            '<input type=\"checkbox\" {$disabled} name=\"obrid[]\" checked=\"checked\" title=\"Selecionada na Homologação\" class=\"icon-crud-add add-green\" value=\"'||o.obrid||'\">'
        ELSE 
            '<input type=\"checkbox\" {$disabled} name=\"obrid[]\" title=\"Não selecionada Homologação\" class=\"icon-crud-del del-red\" value=\"'||o.obrid||'\">' 
    END AS acao, 
    c.catdescricao, o.obrcodigo, o.obrtitulo, e.edirazaosocial, o.obrautor, o.obrano
FROM 
    pnbe.obra o 
INNER JOIN pnbe.categoria c ON (c.catid = o.catid) 
INNER JOIN pnbe.editora e ON (e.ediid = o.ediid) 
LEFT JOIN pnbe.avaliacao a ON (a.avaid = o.avaid) 
LEFT JOIN pnbe.obrahomologacao oh ON (oh.obrid = o.obrid) 
WHERE 
    %s
    o.sitid=".OBRAS_SELECIONADAS_PREANALISE." 
    AND o.avaid = 2
    AND o.obrfechaavalia = 'F'
    AND o.obrano = {$_SESSION['exercicio']}
ORDER BY c.catdescricao, o.obrtitulo ASC";

$cellWidth = array('5%','7%','7%','30%','15%','40%','7%');
$cellAlign = array();
$cabecalho = array('Ação','Categoria','Código','Título','Editora','Autor','Ano');
$sqlComplement = (isset($_GET['catid'])) ? "o.catid = {$_GET['catid']} AND " : '';
$strSqlGrid = sprintf($strSqlBase, (string)$sqlComplement);
$db->monta_lista($strSqlGrid,$cabecalho,50,20,'','','','', $cellWidth, $cellAlign);
?>
<script type="text/javascript">
$(document).ready(function() {
    
    formatGridColor();
    
    if ($(".btn-fHomol").hasClass("disable")) {
        $(".btn-fHomol").css("color","red");
    }
    
    /**
     * Acao para botao de consulta
     */
    $(".btn-consulta").on("click", function(e){
        e.preventDefault();
        if ($("#filterPublisher").val()) {
            var urlLocation = location.href.replace(/&catid=\d+/g,"");
            location.href = urlLocation+"&catid="+$("#filterPublisher").val();
        }
    });
    
    $("input[name='obrid[]']").on("click", function(e) {
        //e.preventDefault();
        
        var targetId = $(this).attr("value")
          , $tr = $(this).parent().parent()
          , url = location.href+"&act=";
          //console.log($tr);
        
        if ($(this).is(':checked')) {
            callAjax(
                url+'tramita&status=A&ano=<?=$_SESSION['exercicio_atual'];?>&obrid='+targetId
              , function(xhr){ 
                  $tr.removeClass("row-del").addClass("row-add");
                  notice("Obra adicionada na homologação!");
              }
            );
        } else {
        
            callAjax(
                url+'tramita&status=I&ano=<?=$_SESSION['exercicio_atual'];?>&obrid='+targetId
              , function(xhr){ 
                  $tr.removeClass("row-add").addClass("row-del");
                  notice("Obra excluida na homologação!");
              }
            );
        }
    });
    
    $(".btn-fHomol").on("click", function(e){
        e.preventDefault();
        location.href=location.href.replace(/&fechaHomologacao=\w+/g,"")+"&fechaHomologacao=<?=$statusBtn['class'];?>";
    });
    
});
</script>