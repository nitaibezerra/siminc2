<?php
include APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if (isAjax()) {
    processaTriagemObra($_GET);
    exit;
}

if (isset($_GET['triagem'])) {
    alteraStatusTriagem($_GET['triagem']);
}

loadStaticScripts();

/**
 * Verifica se a consulta possui filtro por categoria ou não
 */
if (isset($_GET['ediid']) && is_numeric($_GET['ediid'])) {
    $sqlComplement = "WHERE o.ediid = '{$_GET['ediid']}' AND o.obrano = {$_SESSION['exercicio']} AND obrstatus='A'";
} else {
    $sqlComplement = "WHERE o.obrano = {$_SESSION['exercicio']} AND obrstatus='A'";
}

$strSqlEditoras = 'SELECT 
    ediid AS codigo, edirazaosocial AS descricao
FROM pnbe.editora 
WHERE ediid IS NOT NULL ORDER BY edirazaosocial ASC';

montaTopoPNBE(PNBE_MODULE_TITLE, $abacod_tela, $url, $parametros, 'OBRAS');
?>
<form method="POST" action="" name="frmTriagem">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" class="SubTituloDireita">Informe a Editora:</td>
            <td align="left">
                <?php $db->monta_combo('ediid',$strSqlEditoras,'',' - Selecione - ','','','','','','filterPublisher','',isset($_GET['ediid'])?$_GET['ediid']:''); ?>
            </td>
            <td align="center">
                <?php $statusBtn = getStatusTriagem(); ?>
                <?php $pflcod = array(GESTOR, SUPER_USUARIO); ?>
                <?php if (possui_perfil($pflcod)) : ?>
                    <input type="button" <?= $btnDisabled; ?> class="btn-triagem <?=$statusBtn['class'];?>" value="<?=$statusBtn['text'];?>" <?php echo exercicioCorrente();?>>
                <?php endif; ?>
            </td>
        </tr>
    </table>
</form>
<?php

$xIcon = ($statusBtn['class'] == 'disable') ? '' : 'x-icon';
$imgUXAlter = ($statusBtn['class'] == 'disable') ? 'alterar_01.gif' : 'alterar.gif';
$imgUXDelet = ($statusBtn['class'] == 'disable') ? 'excluir_01.gif' : 'excluir.gif';

if (possui_perfil(CONSULTA)) {
    
    $partialStrSQL = "SELECT
            CASE 
                WHEN sitid = ".ADICIONA_TRIAGEM." THEN 
                    '<span class=\"{$xIcon} icon-crud-add add-green\" add-target-id=\"'||obrid||';\"></span>'
                WHEN sitid = ".REMOVE_TRIAGEM." THEN 
                    '<span class=\"{$xIcon} icon-crud-add del-red\" add-target-id=\"'||obrid||';\"></span>'
                WHEN sitid = ".TRIAGEM_SELECIONADA." THEN 
                    '<span class=\"{$xIcon} icon-crud-add add-green\" add-target-id=\"'||obrid||';\"></span>'
                ELSE 
                    '<span class=\"{$xIcon} icon-crud-add add-green\" add-target-id=\"'||obrid||';\"></span>'
                END AS triagem,
            o.obrcodigo, o.obrtitulo, c.catdescricao, o.obrautor, o.obrano
        FROM pnbe.obra o 
        INNER JOIN pnbe.categoria c ON (o.catid = c.catid) 
        %s
        ORDER BY o.obrid, o.obrtitulo ASC";
    
    $columnsWidth = array('1%','6%','40%','10%','40%','4%');
    $cabecalho = array('&nbsp;','código da Obra', 'Título da Obra', 'Categoria', 'Autor', 'Ano');
    
} else {
    $partialStrSQL = "SELECT 
        CASE 
            WHEN sitid = ".ADICIONA_TRIAGEM." THEN 
                '<img src=\"../imagens/{$imgUXAlter}\" title=\"Adiciona a triagem\" class=\"{$xIcon} icon-crud-add add-green\" add-target-id=\"'||obrid||';\">
                 <img src=\"../imagens/{$imgUXDelet}\" title=\"Excluí da triagem\"  class=\"{$xIcon} icon-crud-del add-green\" del-target-id=\"'||obrid||';\">'
            WHEN sitid = ".REMOVE_TRIAGEM." THEN 
                '<img src=\"../imagens/{$imgUXAlter}\" title=\"Adiciona a triagem\" class=\"{$xIcon} icon-crud-add del-red\" add-target-id=\"'||obrid||';\">
                 <img src=\"../imagens/{$imgUXDelet}\" title=\"Excluí da triagem\"  class=\"{$xIcon} icon-crud-del del-red\" del-target-id=\"'||obrid||';\">'
            WHEN sitid = ".TRIAGEM_SELECIONADA." THEN 
                '<img src=\"../imagens/{$imgUXAlter}\" title=\"Adiciona a triagem\" class=\"{$xIcon} icon-crud-add add-green\" add-target-id=\"'||obrid||';\">
                 <img src=\"../imagens/{$imgUXDelet}\" title=\"Excluí da triagem\"  class=\"{$xIcon} icon-crud-del add-green\" del-target-id=\"'||obrid||';\">' 
            ELSE 
                '<img src=\"../imagens/{$imgUXAlter}\" title=\"Adiciona a triagem\" class=\"{$xIcon} icon-crud-add add-green\" add-target-id=\"'||obrid||';\">
                 <img src=\"../imagens/{$imgUXDelet}\" title=\"Excluí da triagem\"  class=\"{$xIcon} icon-crud-del add-green\" del-target-id=\"'||obrid||';\">' 
            END
        AS triagem,
        o.obrcodigo, o.obrtitulo, c.catdescricao, o.obrautor, o.obrano
    FROM 
        pnbe.obra o 
    INNER JOIN pnbe.categoria c ON (o.catid = c.catid) 
    %s
    ORDER BY o.obrid, o.obrtitulo ASC";
    
    $columnsWidth = array('2%','4%','30%','7%','30%','4%');
    $cabecalho = array('Triagem', 'código da Obra', 'Título da Obra', 'Categoria', 'Autor', 'Ano');
}

$strSqlGrid = sprintf($partialStrSQL, $sqlComplement);
$db->monta_lista($strSqlGrid,$cabecalho,50,20,'','','','', $columnsWidth);
?>
<script type="text/javascript">

$(document).ready(function(){
    
    formatGridColor();

    /**
     * Seta o status da triagem[aberto ou fechado]
     */
    if ($(".btn-triagem").hasClass("disable")) {
        $(".btn-triagem").attr("style", "color:red;");
    }
    
    /**
     * Botão de ação do filtro
     */
    $("#filterPublisher").on("change", function() {        
        var uriString = location.href.split("&ediid")[0];  
        location.href = ($(this).val()) ? uriString+"&ediid="+$(this).val() : uriString;
    });
    
    <?php if ( !possui_perfil(CONSULTA) ) : ?>

    /**
     * Acoes do CRUD que adicionam ou excluem obras na triagem
     */
    $(".x-icon").on("click", function(e) {
        e.preventDefault();

        var $this = $(this)
          , $tr = $this.parent().parent()
          , url = location.href+"&act=";

        if ( $this.hasClass("icon-crud-add") ) {

            callAjax(
                url+'add&obrid='+$this.attr("add-target-id")
              , function(){ $tr.removeClass("row-del").addClass("row-add"); }
            );

        } else if ( $this.hasClass("icon-crud-del") ) {

            callAjax(
                url+'del&obrid='+$this.attr("del-target-id")
              , function(){ $tr.removeClass("row-add").addClass("row-del"); }
            );
        }
    });
    
    /**
     * botão de acao que muda o status da triagem[aberto|fechado]
     */
    $(".btn-triagem").on("click", function(e) {
        e.preventDefault();
        location.href=location.href+"&triagem=<?=$statusBtn['class'];?>";
    });
    
    <?php endif; ?>
});
</script>