<?php
header('content-type: text/html; charset=ISO-8859-1');
echo'<br>';


if (temPagamento($_REQUEST['dopid'])) {

    if ($perfilTipoAcompanhamento == 3) {
        $arrAbas = array(
            0 => array("descricao" => "Contratos", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid']),
            1 => array("descricao" => "Monitoramento", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
            2 => array("descricao" => "Notas fiscais", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
            3 => array("descricao" => "Pendências/Finalizar", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
        );
    } else {
        $arrAbas = array(
            0 => array("descricao" => "Contratos", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid']),
            1 => array("descricao" => "Monitoramento", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
            2 => array("descricao" => "Detalhamento do Serviço / Item", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=detalhamentoItem&dopid=" . $_SESSION['dopid']),
            3 => array("descricao" => "Notas fiscais", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
            4 => array("descricao" => "Pendências/Finalizar", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
        );
    }

    if (temPermissaoAnaliseFNDE()) {
        array_push($arrAbas, array("descricao" => "Analise FNDE",
            "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoAnaliseFNDE&dopid=" . $_SESSION['dopid']));
    }
} else {
    $arrAbas = array(
        0 => array("descricao" => "Contratos", "link" => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'])
    );
}

echo montarAbasArray($arrAbas, "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid']);

$habil = 'N';

if (!verificaTermoEmReformulacao($_SESSION['dopid'])) {
    $habil = 'S';
} else {
    $textTermo = '<style>
                    #tabela_reformulacao {
                        border:1px solid red;
                    }
                    #tabela_reformulacao  tr td {
                        border:0;
                    }
                </style>
                <table id="tabela_reformulacao">
                    <tr>
                        <td>
                            <img src="../imagens/par/carimbo-reformulacao.png" border="0">
                        </td>
                        <td>
                            <label style="color:red; font-size:16px;">Finalize a reformulação para realizar o acompanhamento do termo.</label><br>
                        </td>
                    </tr>
                </table>';
}
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
    <tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Lista de Contratos</label></td></tr>
    <tr>
        <td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> <?= $local ?> </b></td>
        <td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>
        <td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" > 
            <img src="../imagens/icone_lupa.png" style="cursor:pointer;" title="Visualizar Termo" onclick="carregaTermoMinuta('<?= $dopid; ?>');" border="0">
            <?= getNumDoc($dopid) ?> 
        </td>
        <td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" ></td>
    </tr>
    <tr>
        <td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>
                <?php
                echo $textTermo;
                if ($habil == 'S') {
                    ?>
                    Após a inserção dos contratos é necessário ir à aba monitoramento e informar a execução de cada item.
                <?php } 
                
                $sqlContratoDiligencia = "SELECT DISTINCT
                            '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location.href=\'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid='||dop.dopid||'\'\" border=\"0\">' as acao,
                            dop.dopnumerodocumento as numero_termo,
                            par.retornacodigosubacao(sd.sbaid) as subacao,
                            con.connumerocontrato,
                            con.condiligencia
                        FROM par.processopar pp 
                        INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = pp.prpid
                        INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = pp.prpid AND ppc.ppcstatus = 'A'
                        INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid 
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                        INNER JOIN par.subacaoitenscomposicaocontratos sicc ON sicc.icoid = sic.icoid AND sicc.sccstatus = 'A'
                        INNER JOIN par.contratos con ON con.conid = sicc.conid AND con.constatus = 'A' AND con.condiligencia IS NOT NULL
                        WHERE dop.dopid = {$_REQUEST['dopid']}";
                $arrContratoDiligencia = $db->carregar($sqlContratoDiligencia);
                $arrContratoDiligencia = ($arrContratoDiligencia) ? $arrContratoDiligencia : array();
                
                if ($arrContratoDiligencia) {
                    echo "<br /><span style='color: #FF0000'>Existe(m) contrato(s) em diligência, destacado(s) em vermelho que deve(m) ser corrigido(s)</span>";
                }
                        
            
          ?>
        </b>
        </td>
    </tr>
</table>

<?php

function carregaDadoSubacao() {

    global $db;

    // PAR
    $sql = "SELECT DISTINCT  
                CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
                    '<center>
                        <a href=\"#\" onclick=\"javascript:listarSubacao(\''||s.sbaid||'\', \'' || sd.sbdano || '\', \'' || prp.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
                        <img style=\"cursor:pointer\" id=\"img_subacao_' || s.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\',\'' || dp.prpid || '\', \'' || dp.dopid || '\');\" border=\"0\" />
                    </center>'
                END
                as acao,  s.sbadsc,
                '<center > <span style=\"color:red;\">' || 
                (
                    SELECT  
                        count(DISTINCT si.icoid)
                    from par.subacao s
                    INNER JOIN par.subacaodetalhe sdd ON sdd.sbaid = s.sbaid
                    INNER JOIN par.termocomposicao tc ON tc.sbdid = sdd.sbdid
                    INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sdd.sbdano AND si.icostatus = 'A'
                    INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
                    LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
                    WHERE 
                        sdd.sbdid = sd.sbdid
                        AND si.icovalidatecnico = 'S'
                        AND icomonitoramentocancelado = false
                ) 
                -
                (
                SELECT  
                        count(DISTINCT si.icoid)
                FROM par.subacao s
                INNER JOIN par.subacaodetalhe sddd ON sddd.sbaid = s.sbaid
                INNER JOIN par.termocomposicao tc ON tc.sbdid = sddd.sbdid
                INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sddd.sbdano AND si.icostatus = 'A'
                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
                LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
                WHERE 
                        sddd.sbdid = sd.sbdid
                        AND si.icovalidatecnico = 'S'
                        AND icomonitoramentocancelado = false
                        AND 
                                EXISTS(	
                                        SELECT 
                                                sit.icoid
                                        FROM
                                                par.subacaoitenscomposicaoContratos sicon 
                                        INNER JOIN par.subacaoitenscomposicao sit ON si.icoid = sicon.icoid AND sit.icostatus = 'A'
                                        WHERE sicon.icoid = si.icoid
                                )
                )
                ||
                '</span></center>'
                ||
                '</td></tr>
                <tr style=\"display:none\" id=\"listaDocumentos2_' || sd.sbdid || '\" >
                        <td id=\"trI_' || sd.sbdid || '\" colspan=8 ></td>
                </tr>' as itens_sem_contrato
            FROM par.subacao s
            INNER JOIN par.subacaodetalhe 	sd  ON sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
            INNER JOIN par.termocomposicao 	tc  ON tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
            INNER JOIN par.documentopar 	dp  ON dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
            INNER JOIN par.processopar 		prp ON prp.prpid = dp.prpid AND prp.prpstatus = 'A'
            WHERE 
                dp.dopid = {$_SESSION['dopid']}
            ORDER BY
                s.sbadsc";
//    ver(simec_htmlentities($sql), d);
    $cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Qtd de itens sem contrato vinculado");
    $db->monta_lista($sql, $cabecalho, 20, 5, 'N', '95%', 'S');
}
carregaDadoSubacao();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<style type="">
    .ui-dialog-titlebar{
        text-align: center;
    }
</style>
<form name="formulario" id="formulario" method="post" action="" >			
    <div id="debug"></div>
    <script type="text/javascript">
            function voltar()
            {
                location.href = 'par.php?modulo=principal/administracaoDocumentos&acao=A';
            }

            function cancelaMonitoramentoItem(icoId)
            {
                if (confirm('Realmente deseja cancelar este item?')) {
                    $.ajax({
                        type: "POST",
                        url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
                        data: "requisicao=cancelarMonitoramentoItem&icoId=" + icoId,
                        async: false,
                        success: function (msg) {
                            alert(msg);
                            window.location.href = window.location;
                            $('#' + icoId).val(0);
                        }
                    });
                }

            }

            function ativaMonitoramentoItem(icoId)
            {
                if (confirm('Realmente deseja reativar este item?')) {
                    $.ajax({
                        type: "POST",
                        url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
                        data: "requisicao=ativaMonitoramentoItem&icoId=" + icoId,
                        async: false,
                        success: function (msg) {
                            alert(msg);
                            window.location.href = window.location;
                            $('#' + icoId).val(0);
                        }
                    });
                }
            }

            function anexarContrato()
            {
                var arrIcoId = '';
                $('input:checkbox').each(function () {
                    if (this.checked)
                    {
                        arrIcoId = arrIcoId + $(this).val() + '|';
                    }
                });
                if (arrIcoId != '')
                {
                    window.open('par.php?modulo=principal/popupAnexoAcompanhamento&acao=A&arrIcoId=' + arrIcoId, '', 'width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=' + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
                }
                else
                {
                    alert('Selecione no mínimo 1 item');
                }
            }


            function carregarListaItens(idImg, sbdid, prpid, dopid) {

                var img = $('#' + idImg);

                var tr_nome = 'listaDocumentos2_' + sbdid;
                var td_nome = 'trI_' + sbdid;

                if ($('#' + tr_nome).css('display') == 'none' && $('#' + td_nome).html() == "")
                {
                    $('#' + td_nome).html('Carregando...');
                    img.attr('src', '../imagens/menos.gif');
                    carregaListaItens(sbdid, td_nome, prpid, dopid);
                }
                if ($('#' + tr_nome).css('display') == 'none' && $('#' + td_nome).html() != "")
                {
                    $('#' + tr_nome).css('display', '');
                    img.attr('src', '../imagens/menos.gif');
                } else
                {
                    $('#' + tr_nome).css('display', 'none');
                    img.attr('src', '/imagens/mais.gif');
                }
            }

            function carregaListaItens(sbdid, td_nome, prpid, dopid) {
                divCarregando();
                var habil = '<?php echo $habil ?>';
                $.ajax({
                    type: "POST",
                    url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
                    data: "requisicao=carregaDadosItens&sbdid=" + sbdid + "&prpid=" + prpid + "&dopid=" + dopid + "&habil=" + habil,
                    async: false,
                    success: function (msg) {
                        $('#' + td_nome).html(msg);
                        divCarregado();
                    }
                });
            }

            function listarSubacao(sbaid, ano, inuid) {
                var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
                janela(local, 800, 600, "Subação");
            }
            
    </script>
</form>	
<div class="demo"></div>

<div id="dialog-aut" title="Autenticação" style="display:none;">
    <div id="dialog-content-aut">
        <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td width="30%" style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
                <td>					
                    <input type="text" class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only" title="Usuário" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
                           onfocus="MouseClick(this);
                                   this.select();" onmouseover="MouseOver(this);" value="<?= $usuario ?>" size="23" id="wsusuario" name="wsusuario">
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Senha:</td>
                <td>
                    <input type="password" class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
                           onfocus="MouseClick(this);
                                   this.select();" onmouseover="MouseOver(this);" value="<?= $senha ?>" size="23" id="wssenha" name="wssenha">
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
        </table>
    </div>
    <div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>
<?php
if ($habil == 'S' && (!bloqueiaParaConsultaEstMun()) && ( getFinalizado() != 'finalizado' )) {
    print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
    print '<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;"><input type="button" onclick="anexarContrato()" value="Anexar Contrato"></label></td></tr><tr>';
    print '</tr></table>';
}

//global $db;
//
//
//$sql = "SELECT mdoid, tpdcod, mdonome, mdostatus, usucpf FROM par.modelosdocumentos limit 20";
//$cabecalho = array("Ação", "UF", "IBGE", "Município", "Processo");
//$db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', 'N');
?>