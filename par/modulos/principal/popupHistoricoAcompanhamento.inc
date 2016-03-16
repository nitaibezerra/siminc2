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
$tipoAcompanhamento = '';
$sql = '';
$cabecalho = array();
$tipoacompanhamento = $_REQUEST['tipoacompanhamento'];
$id = $_REQUEST['id'];
if ($_REQUEST['tipoacompanhamento'] == 'contratos') {
    $tipoAcompanhamento = 'Contratos';
    $sql = "SELECT 
                '<img onclick=\"downloadArquivo('|| con.arqid ||');\" style=\"cursor:pointer\" title=\"Download de Contrato\" src=\"../imagens/anexo.gif\">' as acao,
                sic.icodescricao,
                TO_CHAR(con.condata, 'DD/MM/YYYY HH12:MI:SS') as data,
                con.usucpf::text,
                con.connumerocontrato,
                con.convlrcontrato	
            FROM par.subacaoitenscomposicaocontratos sicc
            INNER JOIN par.contratos con ON con.conid = sicc.conid
            INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid = sicc.icoid
            WHERE sicc.icoid = {$id} 
            ORDER BY sccdata DESC";
    $cabecalho = array("Ação", "Descrição", "Data Alteração", "CPF", "Número Contrato", "Valor do Contrato");
} elseif ($_REQUEST['tipoacompanhamento'] == 'notas') {
    $tipoAcompanhamento = 'Notas';
     $sql = "SELECT 
                '<img onclick=\"downloadArquivo('|| nf.arqid ||');\" style=\"cursor:pointer\" title=\"Download de Nota Fiscal\" src=\"../imagens/anexo.gif\">' as acao,
                sic.icodescricao,
                TO_CHAR(nf.ntfdata, 'DD/MM/YYYY HH12:MI:SS') as data,
                nf.usucpf::text,
                nf.ntfnumeronotafiscal,
                nf.ntfvlrnota
            FROM par.subacaoitenscomposicaonotasfiscais sicnf
            INNER JOIN par.notasfiscais nf ON nf.ntfid = sicnf.ntfid
            INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid = sicnf.icoid
            WHERE sicnf.icoid = {$id}  
            ORDER BY ntfdata DESC;";
    $cabecalho = array("Ação", "Descrição", "Data Alteração", "CPF", "Número da Nota", "Valor da Nota");
} else {
    $tipoAcompanhamento = 'Monitoramento';
    $sql = "SELECT 
                hsic.icodescricao,
                TO_CHAR(hsic.icodatamonitoramento, 'DD/MM/YYYY HH12:MI:SS') as data,
                hsic.icousucpfmonitoramento::text,
                ( CASE
                    WHEN sbacronograma = 1 THEN ( 	
                        SELECT DISTINCT
                            CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
                        FROM
                            par.subacaoitenscomposicao sic
                        WHERE
                                sic.sbaid = s.sbaid
                                AND sic.icoano = sd.sbdano
                                and sic.icoid = si.icoid
                                AND sic.icostatus = 'A'
                        GROUP BY sic.sbaid, sic.icovalidatecnico 
                    )
                    ELSE (	
                        SELECT DISTINCT
                            CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN sum(coalesce(se.sesquantidadetecnico,0))-- escolas sem itens
                                ELSE CASE
                                    WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(ssi.seiqtdtecnico,0)) -- validado (caso não o item não é contado) -- escolas com itens
                                END
                            END as vlrsubacao
                        FROM
                                entidade.entidade t
                        INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
                        LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
                        INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
                        LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
                        LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
                        INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
                        INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
                        LEFT  JOIN par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
                        WHERE
                                sic.sbaid = s.sbaid
                                AND sic.icoano = sd.sbdano
                                AND sic.icoid = si.icoid
                                AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
                                AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
                        GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
                    END 
                ) as quantidade,
                hsic.icoquantidadetecnico,
                hsic.icoquantidaderecebida,
                hsic.icovalor::NUMERIC(10,2)
            FROM par.historicosubacaoitenscomposicao hsic
            INNER JOIN par.subacaoitenscomposicao si ON si.icoid = hsic.icoid
            INNER JOIN par.subacao s ON si.sbaid = s.sbaid AND si.icostatus = 'A'
            INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid  AND si.icoano = sd.sbdano
            INNER JOIN par.acao aca on aca.aciid = s.aciid AND acistatus = 'A'
            INNER JOIN par.pontuacao pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
            INNER JOIN par.instrumentounidade inu on inu.inuid = pon.inuid
            WHERE hsic.icoid = {$id} AND hsic.icodatamonitoramento IS NOT NULL
            ORDER BY hsic.icodatamonitoramento DESC;";
    $cabecalho = array("Descrição", "Data Alteração", "CPF", "Quantidade", "Quantidade Aprovada", "Quantidade recebida", "Valor");
}

#Montar Titulo
monta_titulo("Histórico de Acompanhamento: {$tipoAcompanhamento}", '');
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
$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%');
?>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript">   
    function downloadArquivo(arqid) {
        window.open('par.php?modulo=principal/popupHistoricoAcompanhamento&acao=A&arqid=' + arqid + "&download=S" ,  '' ,'width=200,height=200,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);  
    }
</script>