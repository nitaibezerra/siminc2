<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
if (isAjax()) {
    if (isset($_GET['act']) && isset($_GET['obrid'])) {
        atualizaObraPreAnalise($_GET);
    }
}

if (isset($_GET['preanalise'])) {
    alteraStatusPreAnalise($_GET['preanalise']);
}

if (isset($_GET['fileId']) && isset($_GET['act']) && $_GET['act'] == 'download') {
    download($_GET['fileId']);
}

loadStaticScripts();


montaTopoPNBE(PNBE_MODULE_TITLE, $abacod_tela, $url, $parametros, 'PRÉ-ANALISE');
$strSqlObraStatus = "SELECT sitid AS codigo, sitdescricao AS descricao FROM pnbe.situacao WHERE sitid IN(2,5) ORDER BY sitid ASC";

$statusTriagem = getStatusTriagem();
if ($statusTriagem['class'] == 'enable') {
    $btnDisabled = 'disabled';
    ?>
    <script type="text/javascript">notice("A TRIAGEM DAS OBRAS NÃO FOI ENCERRADA. POR FAVOR AGUARDE.");</script>
    <?
} else {
    $btnDisabled = '';
}

?>
<form method="POST" action="" name="frmPreanalise">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" class="SubTituloDireita">Informe a seleção:</td>
            <td align="left">
                <?php $db->monta_combo('sitid',$strSqlObraStatus,'','','','','','','','filterPublisher','',getFilterParam('sitid')); ?>
                <input type="button" class="btn-consulta" value="Consulta">
            </td>
            <td align="center">
                <? $pflcod = array(GESTOR, SUPER_USUARIO); ?>
                <? if (possui_perfil($pflcod)) : ?>
                    <? $statusBtn = getStatusPreAnalise(); ?>
                    <? if (TRIAGEM_SELECIONADA == getFilterParam('sitid')) : ?>
                        <input type="button" <?= $btnDisabled; ?> class="btn-preAnalise <?=$statusBtn['class'];?>" value="<?=$statusBtn['text'];?>" <?php echo exercicioCorrente();?>>
                    <? endif; ?>
                <? endif; ?>
            </td>
        </tr>
    </table>
</form>
<?php

$cabecalho = array('Pré-análise', 'código da Obra', 'Título da Obra', 'Editora', 'Categoria', 'Autor', 'Ano', 'Parecer');

if ($statusBtn['class'] == 'disable') {
    
    if (OBRAS_EXCLUÍDAS_PREANALISE == getFilterParam('sitid')) {
        
        if ( possui_perfil(CONSULTA) ) :
            
            $sqlCase = "'<span class=\"icon-crud-del del-red\" del-target-id=\"'||obrid||'\"></span>
                       &nbsp;&nbsp;<span class=\"icon-crud-add del-red\" del-target-id=\"'||obrid||'\"></span>'
                       AS triagem,";
        
            array_pop($cabecalho);
            $cabecalho[0] = '&nbsp;';
            $columnsWidth = array('1%','7%','30%','30%','8%','25%','4%');
            
        else :
            $sqlCase = "'<img src=\"../imagens/alterar_01.gif\" title=\"Adicionar a Pré-análise\" class=\"icon-crud-del del-red\" del-target-id=\"'||obrid||'\">
                    &nbsp;&nbsp;<img src=\"../imagens/anexo.gif\" title=\"Anexar Parecer\" class=\"icon-crud-add del-red\" del-target-id=\"'||obrid||'\">'
                    AS triagem,";
            $columnsWidth = array();
        endif;
        
    } else {
        
        if ( possui_perfil(CONSULTA) ) :
            $sqlCase = "'<span class=\"icon-crud-del add-green\" del-target-id=\"'||obrid||'\"></span>' AS triagem,";
            array_pop($cabecalho);
            $cabecalho[0] = '&nbsp;';
            $columnsWidth = array('1%','7%','30%','30%','8%','25%','4%');
        else :
            $sqlCase = "'<img src=\"../imagens/excluir_01.gif\" title=\"Excluí da Pré-análise\"  class=\"icon-crud-del add-green\" del-target-id=\"'||obrid||'\">' AS triagem,";
            $columnsWidth = array();
        endif;
        $sqlComplement = '='.OBRAS_SELECIONADAS_PREANALISE;
    }
    
} else {
    
    if ( possui_perfil(CONSULTA) ) :
        
        $sqlCase = "CASE 
            WHEN sitid = ".TRIAGEM_SELECIONADA." THEN 
                '<span class=\"x-icon icon-click icon-crud-del add-green\" del-target-id=\"'||obrid||'\"></span>'
            ELSE
                '<span class=\"x-icon icon-click icon-crud-add del-red\" del-target-id=\"'||obrid||'\"></span>
                 &nbsp;&nbsp;<span class=\"x-icon icon-click icon-crud-attach del-red\" del-target-id=\"'||obrid||'\"></span>'
            END
        AS triagem,";
        array_pop($cabecalho);
        $cabecalho[0] = '&nbsp;';
        $columnsWidth = array('1%','7%','30%','30%','8%','25%','4%');
    else :
        $sqlCase = "CASE 
            WHEN sitid = ".TRIAGEM_SELECIONADA." THEN 
                '<img src=\"../imagens/excluir.gif\" title=\"Excluí da Pré-análise\"  class=\"x-icon icon-click icon-crud-del add-green\" del-target-id=\"'||obrid||'\">'
            ELSE
                '<img src=\"../imagens/alterar.gif\" title=\"Adiciona a Pré-análise\"  class=\"x-icon icon-click icon-crud-add del-red\" del-target-id=\"'||obrid||'\">
                 &nbsp;&nbsp;<img src=\"../imagens/anexo.gif\" title=\"Anexar Parecer\"  class=\"x-icon icon-click icon-crud-attach del-red\" del-target-id=\"'||obrid||'\">'
            END
        AS triagem,";
        $columnsWidth = array();
    endif;
    
    $sqlComplement = '='.TRIAGEM_SELECIONADA;
}

if (OBRAS_EXCLUÍDAS_PREANALISE == getFilterParam('sitid')) {
    $sqlComplement = 'IN('.OBRAS_EXCLUÍDAS_PREANALISE.')';
    if ( possui_perfil(CONSULTA) ) :
        $sqlDownload = "";
    else :
        $sqlDownload = ",'<center><img '|| CASE WHEN arqid IS NOT NULL THEN 'src=\"../imagens/consultar.gif\" title=\"Fazer Download do Parecer em PDF\" class=\"x-icon download\" del-target-id=\"'||arqid||'\"' ELSE 'src=\"../imagens/consultar_01.gif\" title=\"Não possuí Parecer\"' END ||'\"></center>' AS parecer";
    endif;
}

$strSQLGridPartial = "SELECT %s 
    o.obrcodigo, o.obrtitulo, e.edirazaosocial, c.catdescricao, o.obrautor, o.obrano
    %s
FROM 
    pnbe.obra o 
INNER JOIN pnbe.categoria c ON (o.catid = c.catid) 
INNER JOIN pnbe.editora e ON (e.ediid = o.ediid) 
WHERE o.sitid %s AND o.obrano = {$_SESSION['exercicio']}
ORDER BY o.catid, o.obrtitulo ASC";

if (strlen($sqlDownload) == 0) {
    array_pop($cabecalho);
}

$strSQLGrid = sprintf($strSQLGridPartial, $sqlCase, $sqlDownload, $sqlComplement);
$db->monta_lista($strSQLGrid,$cabecalho,50,20,'','','','',$columnsWidth);
?>

<script type="text/javascript">
$(document).ready(function(){
    
    formatGridColor();
    
    if ($(".btn-preAnalise").hasClass("disable")) {
        $(".btn-preAnalise").attr("style", "color:red;");
    }
    
    $("#filterPublisher").on("change", function() {        
        uriString = location.href.replace(/&sitid=\d+/g,"");
        location.href = ($(this).val()) ? uriString+"&sitid="+$(this).val() : uriString;
    });
    
    <? if ( !possui_perfil(CONSULTA) ) : ?>
    
    var uriString;
    
    $(".icon-click").on("click", function(e){
        e.preventDefault();
        
        var $that = $(this)
          , targetID = $that.attr("del-target-id")
          , $tableRow = $that.parent().parent()
          , redirect = false
          , msg;
          
        uriString = location.href.replace(/&sitid=\d+/g,"");
        
        if ($that.hasClass("icon-crud-del")) {
            uriString+="&act=del";
            msg = "Obra excluída com sucesso!";
        }
        else if ($that.hasClass("icon-crud-add")) {
            uriString+="&act=add";
            msg = "Obra adicionada com sucesso!";
        }
        else if ($that.hasClass("icon-crud-attach")) {
            uriString = "pnbe.php?modulo=principal/anexarparecer&acao=A";
            uriString+="&act=attach";
            uriString+="&urlback="+location.href;
            redirect = true;
        }
       
        uriString = uriString+"&obrid="+targetID;
        
        if (redirect) location.href=uriString;
        
        callAjax(uriString, function(){
            $tableRow.remove();
            notice(msg);
        });
    });
    
    $(".download").on("click", function(e){
        e.preventDefault();
        
        var targetId = $(this).attr("del-target-id")
          , urlAction = location.href+"&act=download&fileId="+targetId;
          
        location.href=urlAction;
    });
    
    $(".btn-preAnalise").on("click", function(e) {
        e.preventDefault();
        location.href=location.href+"&preanalise=<?=$statusBtn['class'];?>";
    });
    
    <? endif; ?>
    
});
</script>