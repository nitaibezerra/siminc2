<?php


if (isAjax() && isset($_GET['act']) && $_GET['act'] == 'verificaPendencia') {
    verificaPendencia();
}

if (isAjax() && isset($_GET['statusBtn'])) {
    alteraAvaliacao($_GET['statusBtn']);
}

if (isAjax() && isset($_GET['avaliacao']) && isset($_GET['obrid'])) {
    setAvaliacaoObra($_GET);
}

if (isset($_GET['fileId']) && isset($_GET['act']) && $_GET['act'] == 'download') {
    download($_GET['fileId']);
}

include_once APPRAIZ."includes/cabecalho.inc";
echo "<br>";
loadStaticScripts();

$statusPreAnalise = getStatusPreAnalise();
if ($statusPreAnalise['class'] == 'enable') {
    $btnDisabled = 'disabled';
    ?>
    <script type="text/javascript">notice("A PRÉ-ANALISE DAS OBRAS NÃO FOI ENCERRADA. POR FAVOR AGUARDE.");</script>
    <?
} else {
    $btnDisabled = '';
}

montaTopoPNBE(PNBE_MODULE_TITLE, $abacod_tela, $url, $parametros, 'AVALIAÇÃO');
$strSqlCategories = "SELECT catid AS codigo, catdescricao AS descricao FROM pnbe.categoria ORDER BY catdescricao";
?>
<form method="POST" action="" name="frmAvaliacao">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" class="SubTituloDireita">Informe a categoria:</td>
            <td align="left">
                <?php $db->monta_combo('catid',$strSqlCategories,'','- Selecione -','','','','','','filterPublisher','',getFilterParam('catid','')); ?>
                <input type="button" class="btn-consulta" value="Consultar">
            </td>
            <? if (avaliacaoAberta() == 'c') : ?>
            <td align="center">
                <input type="button" class="btn-pendencia" <?= $btnDisabled; ?> value="Verificar Pendência" <?php echo exercicioCorrente();?>>
            </td>
            <? endif; ?>
            <td align="center">
                <? $statusBtn = retornaDadosBtnAvaid(); ?>
                <input type="button" <?= $btnDisabled; ?> class="btn-fAvaliacao <?= $statusBtn['class']; ?>" value="<?= $statusBtn['text']; ?>" <?php echo exercicioCorrente();?>>
            </td>
        </tr>
    </table>
</form>
<?php

$selectDisabled = ($statusBtn['class'] == 'enable') ? '' : 'disabled';
$attachDisabled = ($statusBtn['class'] == 'enable') ? 'x-icon attach' : 'attach-off';
$iconAttach = ($statusBtn['class'] == 'enable') ? 'edit_on.gif' : 'edit_off.gif';

if (possui_perfil(CONSULTA)) {
    
    $sqlComp = "'<select {$selectDisabled} style=\"width: auto\" class=\"CampoEstilo btn-avaliacao\" id=\"'||obrid||'\" name=\"'||obrid||'\">
        <option value=\"2\" '|| CASE WHEN avaid=2 THEN 'selected=\"selected\"' ELSE '' END ||'>Altamente indicada</option>
        <option value=\"1\" '|| CASE WHEN avaid=1 or avaid is null THEN 'selected=\"selected\"' ELSE '' END ||'>Em avaliação</option>
        <option value=\"3\" '|| CASE WHEN avaid=3 THEN 'selected=\"selected\"' ELSE '' END ||'>Indicada</option>
        <option value=\"4\" '|| CASE WHEN avaid=4 THEN 'selected=\"selected\"' ELSE '' END ||'>Não indicada</option>
     </select>&nbsp
     <span class=\"add-green\" target-data-id=\"'||obrid||'\" /></span>
     ' AS parecer";
    
} else {
    
    $sqlComp = "'<select {$selectDisabled} style=\"width: auto\" class=\"CampoEstilo btn-avaliacao\" id=\"'||obrid||'\" name=\"'||obrid||'\">
        <option value=\"2\" '|| CASE WHEN avaid=2 THEN 'selected=\"selected\"' ELSE '' END ||'>Altamente indicada</option>
        <option value=\"1\" '|| CASE WHEN avaid=1 or avaid is null THEN 'selected=\"selected\"' ELSE '' END ||'>Em avaliação</option>
        <option value=\"3\" '|| CASE WHEN avaid=3 THEN 'selected=\"selected\"' ELSE '' END ||'>Indicada</option>
        <option value=\"4\" '|| CASE WHEN avaid=4 THEN 'selected=\"selected\"' ELSE '' END ||'>Não indicada</option>
     </select>&nbsp;
     <img src=\"../imagens/{$iconAttach}\" title=\"Anexar Parecer de Avaliação\" class=\"{$attachDisabled} add-green\" target-data-id=\"'||obrid||'\" />&nbsp;
     <img '|| 
        CASE WHEN arqid IS NOT NULL THEN 
            'src=\"../imagens/consultar.gif\" title=\"Fazer Download do Parecer em PDF\" class=\"x-icon download add-green\" target-data-id=\"'||arqid||'\"' 
        ELSE 
            'src=\"../imagens/consultar_01.gif\" title=\"Não possui Parecer\"' 
        END ||' 
     />'
    AS parecer";
    
}

$strSqlBase = "SELECT c.catdescricao, o.obrcodigo, o.obrtitulo, 
    e.edirazaosocial, o.obrautor, o.obrano,
    %s
FROM 
    pnbe.obra o 
INNER JOIN pnbe.categoria c ON (c.catid = o.catid)
INNER JOIN pnbe.editora e ON (e.ediid = o.ediid) 
WHERE 
    %s
    o.sitid=".OBRAS_SELECIONADAS_PREANALISE." AND
    o.obrano = {$_SESSION['exercicio']}
ORDER BY c.catdescricao, o.obrtitulo ASC";

$sqlComplement = (isset($_GET['catid'])) ? "o.catid = {$_GET['catid']} AND " : '';
$strSqlGrid = sprintf($strSqlBase, (string)$sqlComp, (string)$sqlComplement);

$cabecalho = array('Categoria', 'código da Obra', 'Título da Obra', 'Editora', 'Autor', 'Ano', 'Parecer');
$colunmsSize = array('','','20%','','20%','','14%');
$colunmsAlign = array('','','','','','','center');

$db->monta_lista($strSqlGrid,$cabecalho,50,20,'','','','',$colunmsSize, $colunmsAlign);

?>

<script type="text/javascript">
$(document).ready(function(){
  
    formatGridColor();
    
    if ($(".btn-fAvaliacao").hasClass("disable")) {
        $(".btn-fAvaliacao").attr("style", "color:red;");
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
    
    <? if ( !possui_perfil(CONSULTA) ) : ?>
    
    /**
     * Adiciona avaliacao em uma obra
     */
    $(".btn-avaliacao").on("change", function(){
        if ($(this).val() && $(this).attr("id")) {
            
            var url = location.href+"&avaliacao="+$(this).val()+"&obrid="+$(this).attr("id");
            
            callAjax(url, function(){
                notice("Obra avaliada com sucesso!");
            });
        }
    });
    
    $(".download").on("click", function(e){
        e.preventDefault();
        
        var targetId = $(this).attr("target-data-id")
          , urlAction = location.href+"&act=download&fileId="+targetId;
          
        location.href=urlAction;
    });
    
    /**
     * Adiciona uma anexo a obra 
     */
    $(".attach").on("click", function(){
        if ($(this).attr("target-data-id")) {
            var urlAction="pnbe.php?modulo=principal/anexarparecer&acao=A";
            urlAction+="&act=attach&obrid="+$(this).attr("target-data-id");
            urlAction+="&urlback="+location.href;
            
            location.href=urlAction;
        }
    });
    
    /**
     * Verifica pendencia de obras
     */
    $(".btn-pendencia").on("click", function(e) {
        e.preventDefault();
        
        var $modal = $(".modal-dialog").css({width:"750px",height:"500px",display:"none"})
          , posX = ($(window).width()/2) - (($modal.width()-50)/2)
          , posY = 130
          , urlAction = "pnbe.php?modulo=principal/avaliacao&acao=A&act=verificaPendencia"
        
        $modal.dialog({
            height: $modal.height(),
            width: $modal.width(),
            title: "Obras com pendências",
            modal: true,
            position: [posX, posY],
            open: function(event, ui) {
                callAjax(".modal-dialog", urlAction);
            }
        });
//        setTimeOut(function(){
        $('.ui-dialog-content').css({position: 'static'});
        //$('.ui-dialog-content').css({position:'absolute',background:'none repeat scroll 0 0 rgba(0, 0, 0, 0)',overflow:'auto',overflow:'0.5em 1em'});
            
//        }, 300);
    });
    
    
    $(".btn-fAvaliacao").on("click", function(e) {
        e.preventDefault();
        
        var $modal = $(".modal-dialog").css({width:"750px",height:"500px",display:"none"})
          , positionX = ($(window).width()/2) - (($modal.width()-50)/2)
          , positionY = 130
          , status = $(this).hasClass("enable") ? 'F' : 'A'
          , urlAction = "pnbe.php?modulo=principal/avaliacao&acao=A&statusBtn="+status;
        
        $modal.dialog({
            height: $modal.height(),
            width: $modal.width(),
            position: [positionX, positionY],
            title: "Obras com pendências",
            modal: true,
            open: function(event, ui) {
                callAjax(".modal-dialog", urlAction);
                //console.log(urlAction);
            }
        });
    });
    
    <? endif; ?>
    
});
</script>