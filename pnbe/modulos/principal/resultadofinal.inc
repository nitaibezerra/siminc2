<?php
if (isset($_GET['fileId']) && isset($_GET['act']) && $_GET['act'] == 'download') {
    download($_GET['fileId']);
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
loadStaticScripts();

if (avaliacaoAberta() == 'c') {
    ?>
    <script>notice("A AVALIAÇÃO DAS OBRAS NÃO FOI ENCERRADA. POR FAVOR AGUARDE.");</script>
    <?
}

montaTopoPNBE(PNBE_MODULE_TITLE, $abacod_tela, $url, $parametros, 'RESULTADO FINAL');

$strSqlCategories = "SELECT catid AS codigo, catdescricao AS descricao FROM pnbe.categoria ORDER BY catdescricao";
?>
<form method="POST" action="" name="frmResultadoFinal">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" class="SubTituloDireita">Informe a categoria:</td>
            <td align="left">
                <?php $db->monta_combo('catid',$strSqlCategories,'','- Selecione -','','','','','','filterPublisher','',getFilterParam('catid','')); ?>
                <input type="button" class="btn-consulta" value="Consultar">
            </td>
        </tr>
    </table>
</form>
<?php

$cabecalho = array('Categoria', 'código da Obra', 'Título da Obra', 'Editora', 'Autor', 'Ano', 'Avaliação', 'Parecer');
$cellWidth = array('','','20%','','30%','','','');
$cellAlign = array('','','','','','','','center');

if (possui_perfil(CONSULTA)) :
    $sqlCase = ", '<span class=\"add-green\"></span>' AS parecer";
    $cabecalho[count($cabecalho)-1] = '&nbsp';
else :
    $sqlCase = ", '<img '|| 
                CASE WHEN arqid IS NOT NULL THEN 
                    'src=\"../imagens/consultar.gif\" title=\"Fazer Download do Parecer em PDF\" class=\"x-icon download add-green\" target-data-id=\"'||arqid||'\"' 
                ELSE 
                    'src=\"../imagens/consultar_01.gif\" title=\"Não Possui Parecer\" class=\"add-green\"' END ||' 
                  />' AS parecer";
endif;

$strSqlBase = "SELECT c.catdescricao, o.obrcodigo, o.obrtitulo, 
    e.edirazaosocial, o.obrautor, o.obrano, a.avadescricao
    %s
FROM 
    pnbe.obra o 
INNER JOIN pnbe.categoria c ON (c.catid = o.catid) 
INNER JOIN pnbe.editora e ON (e.ediid = o.ediid) 
LEFT JOIN pnbe.avaliacao a ON (a.avaid = o.avaid) 
INNER JOIN pnbe.obrahomologacao oh ON (oh.obrid = o.obrid) 
WHERE 
    %s
    o.sitid=".OBRAS_SELECIONADAS_PREANALISE." 
    AND o.avaid = 2
    AND o.obrfechaavalia = 'F'
    AND o.obrfechahomol = 'F'
    AND oh.homolstatus = 'A'
    AND o.obrano = {$_SESSION['exercicio']}
ORDER BY c.catdescricao, o.obrtitulo ASC";

$sqlComplement = (isset($_GET['catid'])) ? "o.catid = {$_GET['catid']} AND " : '';
$strSqlGrid = sprintf($strSqlBase, (string)$sqlCase ,(string)$sqlComplement);
$db->monta_lista($strSqlGrid,$cabecalho,50,20,'','','','', $cellWidth, $cellAlign);
?>
<script type="text/javascript">
$(document).ready(function(){
    
    formatGridColor();
    
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
    
    <? if ( !possui_perfil(CONSULTA) ) : ?>
    
    /**
     * Acao para downlaod de parecer
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