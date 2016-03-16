<?php
if ($_POST['requisicao'] == 'cancelar') {
    global $db;
    $result = 0;

    $sql = "SELECT docid FROM obras.preobra WHERE preid = {$_POST['preid']}";
    $docid = $db->pegaUm($sql);

    $sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";
    $esdidorigem = $esdid = $db->pegaUm($sql);

    $perfil = pegaPerfilGeral();
    if (!empty($_POST['popjustificativa'])) {

        include_once APPRAIZ . "includes/workflow.php";

        $sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_POST['preid']}";
        $tooid = $db->pegaUm($sql);
        // Se a obra for PAC
        if ($tooid == 1) {
            $esdiddestino = WF_TIPO_OBRA_PRORROGACAO_CANCELADA;
        } else {// Sea obra não for PAC
            $esdiddestino = WF_PAR_PRORROGACAO_CANCELADA;
        }
        # Muda de Aprovado para Aguardando cancelamento
        $sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = $esdidorigem AND esdiddestino = $esdiddestino AND aedstatus = 'A'";
        $aedid = $db->pegaUm($sql);        
        $result = wf_alterarEstado($docid, $aedid, $_POST['popjustificativa'], array('preid' => $_POST['preid']), array('commit'=>true));
        
        $sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = $esdiddestino AND esdiddestino = $esdidorigem AND aedstatus = 'A'";
        $aedid = $db->pegaUm($sql);        
        $result = wf_alterarEstado($docid, $aedid, false, array('preid' => $_POST['preid'], 'cache_estado_atual'=>0), array('commit'=>true));

        #atualia
        $sql = "SELECT popid FROM obras.preobraprorrogacao WHERE popstatus = 'A' and preid = {$_POST['preid']}";
        $popidInativar = $db->pegaUm($sql);      
        
        $sql = "SELECT 
                    popid 
                FROM obras.preobraprorrogacao 
                WHERE preid = {$_POST['preid']} 
                AND (popstatus = 'I' OR popstatus='F')
                AND popid NOT IN (
                        SELECT popidpai FROM obras.preobraprorrogacao WHERE preid = {$_POST['preid']} AND popidpai IS NOT NULL
                ) 
                ORDER BY popid DESC LIMIT 1";
        $popidAtivar = $db->pegaUm($sql); 
        
        $sql = "UPDATE obras.preobraprorrogacao set popstatus = 'I' WHERE popid = $popidInativar";
        $db->executar($sql);

        $popidPai =  ($_POST['popstatus'] == 'F') ? $popidAtivar : $popidInativar;
        
        $sql = "INSERT INTO obras.preobraprorrogacao ( preid, usucpf, popqtddiassolicitado, popdataprazo, popstatus, popdata, popjustificativa, esdidorigem, popidpai )
					VALUES ( " . $_POST['preid'] . ", '" . $_SESSION['usucpf'] . "', NULL, NULL, 'C', 'NOW()', '" . $_POST['popjustificativa'] . "', " . $esdidorigem . ", $popidInativar); ";
        $db->executar($sql);

        if ($popidAtivar) {
            if ($_POST['popstatus'] == 'F') {
                $sql = "UPDATE obras.preobraprorrogacao set popstatus = 'A' WHERE popid = $popidInativar";
            } else {
                $sql = "UPDATE obras.preobraprorrogacao set popstatus = 'A' WHERE popid = $popidAtivar";
            }

            $db->executar($sql);  
        }
        
        $result = true;
        if ($result) {
            $db->commit();
            echo '<script>alert("Dados salvos com sucesso!"); window.opener.location.reload(); window.close();</script>';
        } else {
            $db->rollback();
            echo '<script>alert("Problemas na execução!"); window.close();</script>';
        }
    } else {
        $db->rollback();
        echo '<script>alert("O campo justificativa é Obrigatório!"); window.close();</script>';
    }
}
?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
    $(document).ready(function () {
        $('.cancelar').click(function () {
            if (!$('#popjustificativa').val()) {
                alert('Informe uma justificativa!');
                return false;
            }
            $('#requisicao').val('cancelar');
            $('#form_prorrogacao').submit();
        });
    });
</script>
<form name="form_prorrogacao" id="form_prorrogacao" method="post" action="" >
    <?php
    global $db;

    $sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_REQUEST['preid']}";
    $tooid = $db->pegaUm($sql);

    // Se a obra for PAC
    if ($tooid == 1) {
        $sql = "SELECT 
                    MIN(pag.pagdatapagamentosiafi) AS data_primeiro_pagamento,
                    MIN(pag.pagdatapagamentosiafi) + 720 AS prazo
                FROM par.pagamentoobra po 
                INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                WHERE po.preid = " . $_REQUEST['preid'];
    } else {// Sea obra não for PAC
        $sql = "SELECT 
                    MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
                    MIN(pag.pagdatapagamentosiafi) + 720 as prazo
                FROM par.pagamentoobrapar po 
                INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                WHERE po.preid = {$_REQUEST['preid']}";
    }

    $dataPrimeiroPagamento = $db->carregar($sql);

    $sql = "SELECT 
                pre.predescricao, 
                mun.mundescricao || '/' || mun.estuf as entidade ,
                COALESCE(obr.obrpercentultvistoria, 0) as percexec,
                doc.esdid, 
                esd2.esddsc as situacao, 
                pto.ptoclassificacaoobra as classificacao,
                CASE 
                    WHEN ov.supdtinclusao IS NOT NULL THEN 
                        ov.supdtinclusao
                    WHEN obr.obrdtvistoria IS NOT NULL THEN 
                        obr.obrdtvistoria
                    ELSE 
                        obr.obrdtinclusao
                END as ultatualizacao,
                pto.ptodescricao
            FROM 
                obras.preobra pre
            INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
            LEFT  JOIN obras2.obras obr 
                INNER JOIN workflow.documento doc ON doc.docid = obr.docid
                INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc.esdid
            ON obr.preid = pre.preid
            INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid 
            INNER JOIN workflow.documento d ON d.docid = pre.docid
            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
            LEFT  JOIN
                (SELECT
                    supvid as supervisao, rsuid,obrid, supdtinclusao
                FROM
                    obras.supervisao s
                WHERE supvid = (SELECT supvid FROM obras.supervisao ss 
                                          WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid 
                                          ORDER BY supvdt desc, supvid DESC LIMIT 1)
                ) AS ov 
            ON ov.obrid = obr.obrid
            WHERE pre.prestatus = 'A' AND pre.preid = {$_REQUEST['preid']}";

    $dadosObra = $db->pegaLinha($sql);

    if ($dataPrimeiroPagamento[0]['data_primeiro_pagamento']) {
        $sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = " . $_REQUEST['preid'];
        $prorrogado = $db->pegaUm($sql);
        if ($prorrogado) {
            $dataAtual = $prorrogado;
        } else {
            $dataAtual = $dataPrimeiroPagamento[0]['prazo'];
        }
    }

    $arrCondicao1 = Array(OBR_ESDID_AGUARDANDO_REGISTRO_DE_PRECOS,
        OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE,
        OBR_ESDID_EM_LICITACAO);
    $arrCondicao2 = Array(OBR_ESDID_EM_REFORMULACAO,
        OBR_ESDID_PARALISADA);
    if ($dadosObra['classificacao'] == 'P') {
        if (in_array($dadosObra['esdid'], $arrCondicao1)) {
            $prazoMaximo = "12 Meses";
            $calcula = 12;
        } elseif ($dadosObra['esdid'] == OBR_ESDID_EM_EXECUCAO) {
            if ($dadosObra['percexec'] < 26) {
                $prazoMaximo = "12 Meses";
                $calcula = 12;
            } elseif ($dadosObra['percexec'] > 25 && $dadosObra['percexec'] < 51) {
                $prazoMaximo = "9 Meses";
                $calcula = 9;
            } elseif ($dadosObra['percexec'] > 50 && $dadosObra['percexec'] < 76) {
                $prazoMaximo = "6 Meses";
                $calcula = 6;
            } elseif ($dadosObra['percexec'] > 75) {
                $prazoMaximo = "4 Meses";
                $calcula = 4;
            }
        } elseif (in_array($dadosObra['esdid'], $arrCondicao2)) {
            $prazoMaximo = "Análise Individual";
            $calcula = 0;
        }
    } elseif ($dadosObra['classificacao'] == 'Q') {
        if ($dadosObra['esdid'] == OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE) {
            $prazoMaximo = "12 Meses";
            $calcula = 12;
        } elseif ($dadosObra['esdid'] == OBR_ESDID_EM_LICITACAO) {
            $prazoMaximo = "9 Meses";
            $calcula = 9;
        } elseif ($dadosObra['esdid'] == OBR_ESDID_EM_EXECUCAO) {
            if ($dadosObra['percexec'] < 51) {
                $prazoMaximo = "6 Meses";
                $calcula = 6;
            } elseif ($dadosObra['percexec'] > 50) {
                $prazoMaximo = "3 Meses";
                $calcula = 3;
            }
        } elseif (in_array($dadosObra['esdid'], $arrCondicao2)) {
            $prazoMaximo = "Análise Individual";
            $calcula = 0;
        }
    }
    ?>
    <input type="hidden" id="dataAtual" name="dataAtual" value="<?php echo $dataAtual; ?>">
    <input type="hidden" id="preid" name="preid" value="<?php echo $_REQUEST['preid']; ?>">
    <input type="hidden" id="calcula" name="calcula" value="<?php echo $calcula; ?>">
    <input type="hidden" id="requisicao" name="requisicao" value="">
    <input type="hidden" id="dias" name="dias" value="">
    <table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
        <tr>
            <td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
                <b>Cancelar Prorrogação de Prazo</b>
            </td>
        </tr>
    </table>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <colgroup>
            <col width="40%" />
            <col width="65%" />
        </colgroup>
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td><?php echo $dadosObra['predescricao'] ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo de Obra:</td>
            <td><?php echo $dadosObra['ptodescricao'] ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação:</td>
            <td><?php echo $dadosObra['situacao']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data da Vigência:</td>
            <td><?php echo formata_data($dataAtual); ?></td>
        </tr>
    </table>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <colgroup>
            <col width="40%" />
            <col width="65%" />
        </colgroup>
        <tr>
            <td class="SubTituloDireita">
                Justificativa:
            </td>
            <td>
                <?php echo campo_textarea('popjustificativa', 'S', 'S', 'Justificativa', 60, 10, 200) ?>
            </td>
        </tr>
        <tr align="center">
            <td style="background-color: #dcdcdc" colspan="2">
                <input type="hidden" name="popstatus" value="<?php echo $_REQUEST['popstatus'] ?>">
                <input type="button" class="cancelar" value="Cancelar Prorrogação">
                <input type="button" value="Fechar" onclick="window.close()">
            </td>
        </tr>
    </table>
</form>