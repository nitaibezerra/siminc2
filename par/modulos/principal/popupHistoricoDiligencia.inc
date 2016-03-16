<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?php

# Realizar download
if($_REQUEST['download'] == 'S'){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}

global $db;
$id = $_REQUEST['id'];

#Montar Titulo
monta_titulo("Histórico de Diligência", '');
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <colgroup>
        <col width="20%" />
        <col width="80%" />
    </colgroup>
    <tr>
        <td class="SubTituloDireita">Descrição:</td>
        <td>
            <?php 
                $sqlDescricao = "SELECT icodescricao FROM par.subacaoitenscomposicao where icoid = {$id}";
                $icodescricao = $db->pegaUm($sqlDescricao);
                echo $icodescricao;
            ?>
        </td>
    </tr>
</table>

<?php
monta_titulo("Contratos", '');
$sqlContrato = "SELECT 
            '<img onclick=\"downloadArquivo('|| con.arqid ||');\" style=\"cursor:pointer\" title=\"Download de Contrato\" src=\"../imagens/anexo.gif\">' as acao,
            con.concpfdiligencia::text,
            TO_CHAR(con.condtdiligencia, 'DD/MM/YYYY HH12:MI:SS') as data,
            con.condiligencia	
        FROM par.subacaoitenscomposicaocontratos sicc
        INNER JOIN par.contratos con ON con.conid = sicc.conid
        INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid = sicc.icoid
        WHERE sicc.icoid = {$id} AND sicc.sccstatus = 'I' AND con.condiligencia IS NOT NULL
        ORDER BY sccdata DESC";
$cabecalho = array("Ação", "CPF", "Data Alteração", "Diligência");
$db->monta_lista_simples($sqlContrato, $cabecalho, 50, 5, 'N', '100%');
?>

<br />
<br />
<?php
monta_titulo("Notas Fiscais", '');
$sqlNota = "SELECT 
            '<img onclick=\"downloadArquivo('|| nf.arqid ||');\" style=\"cursor:pointer\" title=\"Download de Nota Fiscal\" src=\"../imagens/anexo.gif\">' as acao,
            nf.ntfcpfdiligencia::text,
            TO_CHAR(nf.ntfdtdiligencia, 'DD/MM/YYYY HH12:MI:SS') as data,
            nf.ntfdiligencia
        FROM par.subacaoitenscomposicaonotasfiscais sicnf
        INNER JOIN par.notasfiscais nf ON nf.ntfid = sicnf.ntfid
        INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid = sicnf.icoid
        WHERE sicnf.icoid = {$id} AND sicnf.scnstatus = 'I' AND nf.ntfdiligencia IS NOT NULL
        ORDER BY ntfdata DESC;";
        
$cabecalho = array("Ação", "CPF", "Data Alteração", "Diligência");
$db->monta_lista_simples($sqlNota, $cabecalho, 50, 5, 'N', '100%');
?>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript">   
    function downloadArquivo(arqid) {
        window.open('par.php?modulo=principal/popupHistoricoAcompanhamento&acao=A&arqid=' + arqid + "&download=S" ,  '' ,'width=200,height=200,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);  
    }
</script>