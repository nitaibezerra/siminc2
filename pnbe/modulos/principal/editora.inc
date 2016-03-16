<?php

if (isset($_GET['fileId']) && isset($_GET['act']) && $_GET['act'] == 'download') {
    download($_GET['fileId']);
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
loadStaticScripts();

montaTopoPNBE(PNBE_MODULE_TITLE,$abacod_tela,$url,$parametros,'EDITORA');

if (possui_perfil(EDITORA)) {
    $strSqlEditoras = "SELECT 
                           e.ediid AS codigo, e.edirazaosocial AS descricao 
                       FROM pnbe.editora e
                       INNER JOIN pnbe.usuarioresponsabilidade ur ON (e.ediid = ur.ediid)
                       WHERE ur.usucpf = '".trim($_SESSION['usucpf'])."' AND ur.rpustatus = 'A'
                       ORDER BY e.edirazaosocial";
} else {
    $strSqlEditoras = "SELECT ediid AS codigo, edirazaosocial AS descricao FROM pnbe.editora ORDER BY edirazaosocial";
}
?>
<form method="POST" action="" name="frmEditora">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" class="SubTituloDireita">Informe a Editora:</td>
            <td align="left">
                <?php $db->monta_combo('ediid',$strSqlEditoras,'','- Selecione -','','','','','','editora','',getFilterParam('query','')); ?>
                <input type="button" class="btn-consulta" value="Consulta">
            </td>
        </tr>
    </table>
</form>
<?php

if (isset($_GET['query'])) :
    $strSqlBase = "SELECT c.catdescricao, o.obrcodigo, o.obrtitulo, o.obrautor, o.obrano,
        CASE 
            WHEN o.avaid=1 AND o.sitid=3 THEN 'Excluído na triagem'
            WHEN o.avaid=1 AND o.sitid=5 THEN 'Excluído na pré-análise'
            WHEN o.avaid=2 THEN 'Altamente indicada'
            WHEN o.avaid=3 THEN 'Indicada'
            WHEN o.avaid=4 THEN 'Não indicada' 
            ELSE '' 
        END
        AS resultado,
         '<img '|| 
            CASE WHEN o.arqid IS NOT NULL THEN 
                'src=\"../imagens/consultar.gif\" title=\"Fazer Download do Parecer em PDF\" class=\"x-icon download add-green\" target-data-id=\"'||arqid||'\"' 
            ELSE 
                'src=\"../imagens/consultar_01.gif\" title=\"Não possuí Parecer\" class=\"add-green\"' 
            END ||' 
         />'
        AS parecer
    FROM 
        pnbe.obra o 
    INNER JOIN pnbe.categoria c ON (c.catid = o.catid)
    INNER JOIN pnbe.editora e ON (e.ediid = o.ediid) 
    WHERE 
        e.ediid = %d AND
        o.obrano = {$_SESSION['exercicio']}
    ORDER BY c.catdescricao ASC";

    $strSqlGrid = sprintf($strSqlBase, (int)$_GET['query']);

    $cabecalho = array('Categoria', 'código da Obra', 'Título da Obra', 'Autor', 'Ano', 'Resultado', 'Parecer');
    $db->monta_lista($strSqlGrid,$cabecalho,50,20,'','','','',array('','','20%','','5%','10%','7%'), array('','','','','center','center','center'));
endif;

?>
<script type="text/javascript">
$(document).ready(function(){
    
    formatGridColor();
    
    /**
     * busca editora
     */
    $(".btn-consulta").on("click", function(e){
        e.preventDefault();
        
        if ($("#editora").val()) {
            var urlAction = "pnbe.php?modulo=principal/editora&acao=A&query="+$("#editora").val();
            location.href = urlAction;
        } else {
            notice("É necessário um parametro para consulta");
        }
    });
    
    <? if ( !possui_perfil(CONSULTA) ) : ?>
    
    /**
     * faz download do parecer
     */
    $(".download").on("click", function(e){
        e.preventDefault();
        
        var targetId = $(this).attr("target-data-id")
          , urlAction = location.href+"&act=download&fileId="+targetId;
          
        location.href=urlAction;
    });
    
    <? endif; ?>
    
});
</script>