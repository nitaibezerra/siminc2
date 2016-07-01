<?php
verificaSessao('obra');
$obrid = $_SESSION['obras2']['obrid'];
$orgid = $_SESSION['obras2']['orgid'];
$empid = $_SESSION['obras2']['empid'];

include APPRAIZ . 'includes/cabecalho.inc';
echo '<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>';
echo '<br>';

switch ($_REQUEST['req']) {
    case 'cadastrarOsMi':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras($obrid);
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/cadOsMi&acao=A&tomid=' . $_POST['tomid']);
        die();
}

if($_REQUEST['arqid']){
    download();
}


function download(){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    die();
}


$_acao = filter_input(INPUT_GET, 'acao', FILTER_SANITIZE_STRING);
if ($_acao != 'V') {

    $arMenuBlock = bloqueiaMenuObjetoPorSituacao($obrid);
    //Gestor MEC, Super Usuario, Supervisor Unidade, Gestor Unidade.

    if (!$obrid && !$empid) {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP, $url, $parametros, $arMenuBlock);
    } elseif ($obrid) {
        if ($orgid == ORGID_EDUCACAO_BASICA) {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, $arMenuBlock);
        } else {

            $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros, $arMenuBlock);
        }
    } else {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA, $url, $parametros, $arMenuBlock);
    }

    $habilitado = true;
    $habilita = 'S';
} else {
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $_SESSION['obras2']['obrid'] = isset($_REQUEST['obrid']) ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
    $db->cria_aba($abacod_tela, $url, $parametros);
    $somenteLeitura = false;
    $habilitado = false;
    $habilita = 'N';
}
echo cabecalhoObra($obrid);
monta_titulo('Pendências da Obra', '');

$obra = new Obras();
$param = array();
$param['not(obridpai)'] = true;
$param['obrid'] = $obrid;

$sql = $obra->listaSql($param);
$rs = $db->pegaLinha($sql);


$obra = new Obras($obrid);
$endereco = new Endereco($obra->endid);
$empreendimento = new Empreendimento($obra->empid);

$crtid = $obra->pegaContratoPorObra($obrid);
$contrato = new Contrato($crtid);
$dados = $contrato->getDados();

$termoconv = $db->pegaLinha("SELECT * FROM obras2.v_termo_convenio_obras  WHERE obrid = $obrid");
$venc_termo = $db->pegaUm("SELECT \"Fim Vigência Termo\" as fim_vig FROM obras2.vm_termo_obras WHERE \"ID Obra\" =  $obrid");


$strSQL = "SELECT DISTINCT 
            CASE
                    WHEN ed.esdid = 693 THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
                    WHEN ed.esdid = 769 THEN '<font COLOR=\"#000000\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
                    ELSE ed.esddsc 
            END as situacao,
        '<font ' || 
	CASE 
	WHEN obrdtultvistoria IS NOT NULL AND ed.esdid IN (691, 693, 690) THEN
            CASE 
                WHEN o.staid NOT IN (3, 10) THEN 
                    CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 45 THEN
                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                        'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' || 
                                        COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), 
                                        to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' - '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)'
                                ELSE
                                        'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' || 
                                        COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), 
                                        to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' - '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)'
                                END
                         WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 45 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 60 THEN
                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                        'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' || 
                                        COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), 
                                        to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' - '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)'
                                ELSE
                                        'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' || 
                                        COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), 
                                        to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' - '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)'
                                END
                         WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 60 THEN
                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                        'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">' || 
                                        COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), 
                                        to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' - '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)' 
                                ELSE
                                        'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">'||  
                                        COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), 
                                        to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' - '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)'
                                END
                END
                WHEN o.staid IN (3, 10) THEN 
                        'COLOR=\"#000000\" >' 
            END
	ELSE
		'COLOR=\"#000000\" TITLE=\" \">' ||
		to_char(obrdtultvistoria, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - o.obrdtultvistoria)||' dia(s) )'
	END
	|| '</FONT>' as ultima_atualizacao,
            o.obrpercentultvistoria, o.obrdtultvistoria
        FROM obras2.obras o
            INNER JOIN obras2.situacaoatividade  sa ON o.staid = sa.staid
            LEFT JOIN workflow.documento         d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento   ed ON ed.esdid = d.esdid
            LEFT JOIN entidade.entidade 	 ent ON ent.entid = o.entid
            LEFT JOIN obras2.tipologiaobra       tpo ON tpo.tpoid = o.tpoid
            LEFT JOIN obras2.empreendimento 	 e ON e.empid = o.empid AND e.empstatus = 'A'
            LEFT JOIN obras2.obrascontrato 	 oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'	
            LEFT JOIN obras2.programatipologia	 ptp ON ptp.tpoid = tpo.tpoid
            LEFT JOIN obras2.programafonte	 prf ON prf.prfid = ptp.prfid
            LEFT JOIN obras2.tipoorigemobra	 too ON e.tooid = too.tooid
        WHERE o.obrid = {$obrid}";
$_dadosPercent = $db->pegaLinha($strSQL);

// Vigencia do contrato
$strSQLVigencia = "SELECT
	'<span' ||
            CASE 
                WHEN DATE_PART('days', NOW() - crtdttermino) <= 60 THEN 
                    ' style=\"color:red;\">' || 
                        COALESCE(TO_CHAR(crtdttermino, 'DD/MM/YYYY'),TO_CHAR(crtdttermino, 'DD/MM/YYYY'))
                    || ' - '||DATE_PART('days', NOW() - crtdttermino)||' dia(s) '
                WHEN DATE_PART('days', NOW() - crtdttermino) > 60 AND DATE_PART('days', NOW() - crtdttermino) <= 90 THEN 
                    ' style=\"color:yellow;\">' || 
                        COALESCE(TO_CHAR(crtdttermino, 'DD/MM/YYYY'),TO_CHAR(crtdttermino, 'DD/MM/YYYY'))
                    || ' - '||DATE_PART('days', NOW() - crtdttermino)||' dia(s) '
                WHEN DATE_PART('days', NOW() - crtdttermino) > 90 THEN 
                    ' style=\"color:green;\">' || 
                        COALESCE(TO_CHAR(crtdttermino, 'DD/MM/YYYY'),TO_CHAR(crtdttermino, 'DD/MM/YYYY'))
                    || ' - '||DATE_PART('days', NOW() - crtdttermino)||' dia(s) '
                ELSE
                    ' style=\"color:vencida;\">' || 
                        COALESCE(TO_CHAR(crtdttermino, 'DD/MM/YYYY'),TO_CHAR(crtdttermino, 'DD/MM/YYYY'))
                    || ' - '||DATE_PART('days', NOW() - crtdttermino)||' dia(s) '
            END
	|| '</span>' as alerta_vigencia
    FROM obras2.contrato c
            INNER JOIN obras2.obrascontrato oc ON (oc.crtid = c.crtid)
            INNER JOIN obras2.obras o ON (o.obrid = oc.obrid)
    WHERE o.obrid = {$obrid}";
$dadosVigencia = $db->pegaLinha($strSQLVigencia);

function getOSMI($obrid, $tomID, $label)
{
    $markup = "<a onclick='cadastrarOS(\"%s\", %d);' href='#'>%s</a>";
    return sprintf($markup, $obrid, $tomID, $label);
}

if ($rs && $_dadosPercent && $dadosVigencia) :
    ?>
    <script type="text/javascript">
        function cadastrarOS(obrid, tomid) {
            $('#req').val('cadastrarOsMi');
            $('#obrid').val(obrid);
            $('#tomid').val(tomid);

            janela('', 800, 650, 'cadastrarOsMi');

            $('#formListaObra').attr('target', 'cadastrarOsMi');
            $('#formListaObra').submit();
        }
    </script>

    <form method="post" name="formListaObra" id="formListaObra">
        <input type="hidden" name="req" id="req" value="">
        <input type="hidden" name="obrid" id="obrid" value="">
        <input type="hidden" name="empid" id="empid" value="">
        <input type="hidden" name="tomid" id="tomid" value="">
    </form>

    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="3">
        <tr>
            <td class="SubTituloDireita" width="50%">Pendências da Entidade</td>
            <?
            ?>
            <? if ($empreendimento->empesfera == 'M'): ?>
                <td><img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer"
                         class="detalhar_pendencias_obras" muncod="<?= $endereco->muncod ?>" title="Detalhar"/></td>
            <? else: ?>
                <td><img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer"
                         class="detalhar_pendencias_obras" estuf="<?= $endereco->estuf ?>" title="Detalhar"/></td>
            <? endif; ?>
        </tr>


        <tr>
            <td class="SubTituloDireita" width="50%">Bloqueio no PAR</td>
            <td>
                <?
                $sql = "SELECT
                          situacao
                        FROM obras2.vm_total_pendencias
                        WHERE
                            obrid = $obrid
                            AND pendencia = 'par'";
                $pendencias = $db->carregarColuna($sql);
                if (!$pendencias) echo ' N/A';
                foreach ($pendencias as $pendencia)
                    echo ' - ' . $pendencia . '<br />';
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="50%">Bloqueio de Recursos</td>
            <td>
                <?
                $sql = "SELECT
                          situacao
                        FROM obras2.vm_total_pendencias
                        WHERE
                            obrid = $obrid
                            AND pendencia = 'recursos'";
                $pendencias = $db->carregarColuna($sql);
                if (!$pendencias) echo ' N/A';
                foreach ($pendencias as $pendencia)
                    echo ' - ' . $pendencia . '<br />';
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="50%">Bloqueio de Novos Desembolsos</td>
            <td>
                <?
                $sql = "SELECT
                          situacao
                        FROM obras2.vm_total_pendencias
                        WHERE
                            obrid = $obrid
                            AND pendencia = 'desembolso'";
                $pendencias = $db->carregarColuna($sql);
                if (!$pendencias) echo ' N/A';
                foreach ($pendencias as $pendencia)
                    echo ' - ' . $pendencia . '<br />';
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="50%">Pendência de Preenchimento</td>
            <td>
                <?
                $sql = "SELECT
                          situacao
                        FROM obras2.vm_total_pendencias
                        WHERE
                            obrid = $obrid
                            AND pendencia = 'preenchimento'";
                $pendencias = $db->carregarColuna($sql);
                if (!$pendencias) echo ' N/A';
                foreach ($pendencias as $pendencia)
                    echo ' - ' . $pendencia . '<br />';
                ?>
            </td>
        </tr>
    </table>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="3">
    <tr>
        <?
        $supervisao = new Supervisao();
        $documento = wf_pegarDocumento($obra->docid);
        $supid = $supervisao->pegaUltSupidByObra($obrid, $param);
        $vistoria = new Supervisao($supid);
        $vistoriaEstrutura = $vistoria->getDados();

        $paralisacao = new HistoricoParalisacao();

        $historicoParalisacao = $paralisacao->getHistoricoParalisacao($supid);

        $arquivo = new Arquivo($historicoParalisacao['arqid']);
        $arquivoBoletim = new Arquivo($historicoParalisacao['arqidboletim']);
        $arquivoOSparalisacao = new Arquivo($historicoParalisacao['arqidosparalisacao']);
        $arquivoDocumentoEmbargo = new Arquivo($historicoParalisacao['arqiddocumentoembargo']);
        $arquivoplanilha = new Arquivo($vistoriaEstrutura['arqidplanilha']);
        $arquivotermo = new Arquivo($vistoriaEstrutura['arqidtermo']);
        $arqidosexecucao = new Arquivo($vistoriaEstrutura['arqidosexecucao']);

        $somenteLeitura = 'N';


        ?>

        <td class="SubTituloDireita" width="50%">Situação da Obra</td>
        <td>
            <table>
                <tr>
                    <td style="font-size: 12px"><?= ($rs['situacao']) ? trim($rs['situacao']) : 'Não informado'; ?></td>
                </tr>
                <? if ($documento['esdid'] == '691'): ?>
                    <tr style="" id="tr_tplid">
                        <td class="SubTituloDireita">Tipo de Paralisação</td>
                        <td>
                            <?php
                            $tipoParalisacao = new TipoParalisacao();
                            $sql = $tipoParalisacao->listaCombo();
                            $tplid = $historicoParalisacao['tplid'];
                            $db->monta_combo("tplid", $sql, $somenteLeitura, "Selecione...", "", '', '', '', 'S', "tplid");
                            ?>
                        </td>
                    </tr>
                <? if (obraMI($obrid) && (ultVistoriaOS($obrid) || !empty($vistoriaEstrutura['arqidosexecucao']))): ?>

                    <tr style="display:none" id="tr_osexecucao">
                        <td class="SubTituloDireita">OS de Execução</td>
                        <td>
                            <?php
                            if ($arqidosexecucao->arqid) {
                                echo "<a href='?arqid=$arqidosexecucao->arqid'>(" . $arqidosexecucao->arqnome . "." . $arqidosexecucao->arqextensao . ")</a>";
                            }
                            ?>
                        </td>
                    </tr>

                <? endif; ?>
                    <tr style="display:none" id="tr_stpid">
                        <td class="SubTituloDireita">Subtipo de Paralisação</td>
                        <td id="td_stpid">
                            <?php
                            if ($tplid) {
                                $subTipoParalisacao = new SubTipoParalisacao();
                                $sql = $subTipoParalisacao->listaCombo(array("tplid" => $tplid));
                                $stpid = $historicoParalisacao['stpid'];
                                $db->monta_combo("stpid", $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'N', "stpid");
                            }
                            ?>
                        </td>
                    </tr>
                    <tr style="display:none" id="tr_hprobs">
                        <td class="SubTituloDireita">Observações da Paralisação</td>
                        <td>
                            <?php
                            $hprobs1 = $historicoParalisacao['hprobs'];
                            echo campo_textarea('hprobs1', 'S', $somenteLeitura, '', '70', '8', '1000', '', 0, '', '', false, '', '');
                            ?>
                        </td>
                    </tr>

                    <tr id="tr_osparalisacao" style="display: none;">
                        <td class="SubTituloDireita" style="vertical-align:top;">OS de Paralisação:</td>
                        <td>
                            <?php
                            if ($arquivoOSparalisacao->arqid) {
                                echo "<a href='".$_SERVER['SCRIPT_NAME']."?modulo=relatorio/pendencias&acao=A&arqid=$arquivoOSparalisacao->arqid'>(" . $arquivoOSparalisacao->arqnome . "." . $arquivoOSparalisacao->arqextensao . ")</a>";
                            }
                            ?>
                        </td>
                    </tr>

                    <tr id="tr_arqid" style="display: none;">
                        <td class="SubTituloDireita" style="vertical-align:top;">Recisão do Contrato:</td>
                        <td>
                            <?php
                            if ($arquivo->arqid) {
                                echo "<a href='".$_SERVER['SCRIPT_NAME']."?modulo=relatorio/pendencias&acao=A&arqid=$arquivo->arqid'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                            }
                            ?>
                        </td>
                    </tr>

                    <tr id="tr_arquivo_embargo" style="display: none;">
                        <td class="SubTituloDireita" style="vertical-align:top;">Documento de Embargo:</td>
                        <td>
                            <?php
                            if ($arquivoDocumentoEmbargo->arqid) {
                                echo "<a href='".$_SERVER['SCRIPT_NAME']."?modulo=relatorio/pendencias&acao=A&arqid=$arquivoDocumentoEmbargo->arqid'>(" . $arquivoDocumentoEmbargo->arqnome . "." . $arquivoDocumentoEmbargo->arqextensao . ")</a>";
                            }
                            ?>
                        </td>
                    </tr>

                    <tr id="tr_arqid_boletim" style="display: none;">
                        <td class="SubTituloDireita" style="vertical-align:top;">Boletim de medição acumulado:</td>
                        <td>
                            <?php
                            if ($arquivoBoletim->arqid) {
                                echo "<a href='".$_SERVER['SCRIPT_NAME']."?modulo=relatorio/pendencias&acao=A&arqid=$arquivoBoletim->arqid'>(" . $arquivoBoletim->arqnome . "." . $arquivoBoletim->arqextensao . ")</a>";
                            }
                            ?>
                        </td>
                    </tr>


                    <script type="text/javascript">

                        <?php
                            if ($vistoriaEstrutura['staid'] == STAID_PARALISADO) {
                        ?>
                        $('#tr_tplid').show();
                        <?php
                        if ($historicoParalisacao['tplid'] == TPLID_OUTROS) {
                            ?>
                        $('#tr_hprobs').show();
                        <?php
                    }if ($historicoParalisacao['tplid'] == TPLID_OUTROS) {
                        ?>
                        $('#tr_hprobs').show();
                        <?php
                    }
                    if ($historicoParalisacao['tplid'] == TPLID_EMBARGO) {
                        ?>
                        $('#tr_stpid').show();
                        $('#tr_arquivo_embargo').show();
                        $('#tr_hprobs').show();
                        $('#tr_hprobs .SubTituloDireita').html('AÇÕES ADOTADAS PARA RESOLUÇÃO DO EMBARGO');
                        <?php
                    }
                    if ($historicoParalisacao['tplid'] == TPLID_CONTRATO_RESCINDIDO) {
                        ?>
                        $('#tr_arqid_boletim').show();
                        $('#tr_arqid').show();
                        $('#tr_hprobs').show();
                        $('#tr_hprobs .SubTituloDireita').html('Medidas Administrativas e Judiciais Adotadas');
                        <?php
                    }
                    if ($historicoParalisacao['tplid'] == TPLID_ABANDONO_EMPRESA) {
                        ?>
                        $('#tr_hprobs').show();
                        $('#tr_hprobs .SubTituloDireita').html('Medidas Administrativas e Judiciais Adotadas');
                        <?php
                      }
                    if ($historicoParalisacao['tplid'] == TPLID_OS_PARALISACAO) {
                                            ?>
                        $('#tr_osparalisacao').show();
                        <?php
                    }
                    if ($historicoParalisacao['tplid'] == TPLID_PARALISACAO) {
                        ?>
                        $('#tr_osexecucao').show();
                        <?php
                    }
                }
                ?>
                    </script>
                <? endif; ?>
            </table>

        </td>

    </tr>
    <tr>
        <td class="SubTituloDireita">Percentual última vistoria</td>
        <td><?= ($rs['obrpercentultvistoria']) ? trim($rs['obrpercentultvistoria']) : 'Não informado'; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Situação vistoria</td>
        <td>
            <?= ($_dadosPercent['ultima_atualizacao']) ? trim($_dadosPercent['ultima_atualizacao']) : 'Não informado'; ?>
            <a href="obras2.php?modulo=principal/cadObraRecursos&acao=A">Visualizar Recursos</a>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Nº Terno/Convênio</td>
        <td><?= ($termoconv['termo_convenio']) ? trim($termoconv['termo_convenio']) : 'Não informado'; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Ano Terno/Convênio</td>
        <td><?= ($termoconv['ano_termo_convenio']) ? trim($termoconv['ano_termo_convenio']) : 'Não informado'; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Vigência do contrato</td>
        <td>
            <?= ($dadosVigencia['alerta_vigencia']) ? $dadosVigencia['alerta_vigencia'] : 'Não informado'; ?>
            <a href="obras2.php?modulo=principal/exibeContrato&acao=A">Visualizar Contrato</a>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Vigência do Termo</td>
        <td>
            <?= $venc_termo ?>
        </td>
    </tr>
    <!--<tr>
        <td class="SubTituloDireita">Cronograna previsto x Executado</td>
        <td>&nbsp;</td>
    </tr>-->
    <tr>
        <?
        if (obraMi($obrid)):
            ?>
            <td valign="top" class="SubTituloDireita">Ordem de serviço MI</td>
            <td>
                <table width="95%">
                    <tr>
                        <td><?= getOSMI($obrid, 3, 'Criar OS de implantação'); ?></td>
                    </tr>
                    <tr>
                        <td><?= getOSMI($obrid, 2, 'Criar OS de sondagem'); ?></td>
                    </tr>
                    <tr>
                        <td><?= getOSMI($obrid, 1, 'Criar OS de execução'); ?></td>
                    </tr>
                </table>
            </td>
        <? endif; ?>
    </tr>
    </table>
<?php
else :
    ?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro">Nenhum resultado encontrado</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">&nbsp;</td>
        </tr>
    </table>
<?php
endif;

monta_titulo('Histórico de Pendências da Obra', '');

extract($_POST);
?>



    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

    <script type="text/javascript">
        jQuery(function ($) {
            $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro', 'Fevereiro', 'Mar&ccedil;o', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
                monthNamesShort: ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
                dayNames: ['Domingo', 'Segunda-feira', 'Ter&ccedil;a-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira', 'S&aacute;bado'],
                dayNamesShort: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
                dayNamesMin: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''
            };
            $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
        });

    </script>

    <form name="filtro" id="filtro" action="" method="post">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="subtituloDireita" style="width: 40%;">Tipo da Pendência:</td>
                <td>
                    <?php
                    $sql = "SELECT hpopendencia as codigo, hpopendencia as descricao FROM obras2.historicopendenciaobras GROUP BY hpopendencia ORDER BY hpopendencia";
                    $db->monta_combo("pendencia", $sql, "S", "Todas", "", "", "", 200, "N", "pendencia", false);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="190px">Data de Superação:</td>
                <td>
                    <?php

                    ?>
                    de: <input type="text" id="data_de" name="data_de" value="<?php echo $data_de; ?>" size="15"
                               maxlength="10" class="normal">
                    &nbsp;
                    até: <input type="text" id="data_ate" name="data_ate" value="<?php echo $data_ate; ?>" size="15"
                                maxlength="10" class="normal">
                </td>
                <td>&nbsp;</td>
            </tr>


            <tr>
                <td colspan="2" class="subtituloDireita">&nbsp;</td>
            </tr>
            <tr>
                <td colspan="2">
                    <center>
                        <input type="submit" id="btn_filtrar" name="btn_filtrar" value="Filtrar"/>
                    </center>
                </td>
            </tr>

        </table>

    </form>


    <script lang="javascript">

        setTimeout(function () {

            jQuery('#rstdtinclusao_de').mask('99/99/9999');
            jQuery('#rstdtsuperacao_de').mask('99/99/9999');
            jQuery('#rstdtinclusao_ate').mask('99/99/9999');
            jQuery('#rstdtsuperacao_ate').mask('99/99/9999');

            var d = new Date();
            var data_hoje = d.toUTCString();

            jQuery("#data_de").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                showAnim: 'drop'
            });
            jQuery("#data_ate").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                showAnim: 'drop'
            });

        }, 500);

    </script>


<?

$where = array();

if (!empty($_POST['pendencia']))
    $where[] = " hpopendencia ILIKE '{$_POST['pendencia']}' ";

if (!empty($_POST['data_de']))
    $where[] = " hpodata >= '" . ajusta_data($_POST['data_de']) . "' ";

if (!empty($_POST['data_ate']))
    $where[] = " hpodata <= '" . ajusta_data($_POST['data_ate']) . "' ";

$where = implode(' AND ', $where);
$where = ($where) ? ' AND ' . $where : '';

$sql = "SELECT
  TO_CHAR(hpodata, 'DD/MM/YYYY') as data,
  hpopendencia
  FROM obras2.historicopendenciaobras
  WHERE obrid = $obrid {$where}
  ORDER BY hpodata DESC";

$cabecalho = array('Data da Verificação', 'Pendência');

$db->monta_lista($sql, $cabecalho, 10, 5, 'N', 'center', $par2, "formulario", "", "center");

?>

    <script type="text/javascript">
        $(function () {
            jQuery("#div_detalhe_pendencias_obras").load('/obras2/ajax.php?detalhar_pendencias_obras=1&muncod=' + jQuery('.detalhar_pendencias_obras:eq(0)').attr('muncod') + '&estuf=' + jQuery('.detalhar_pendencias_obras:eq(0)').attr('estuf'));
        });
    </script>



<?
function ultVistoriaOS($obrid)
{
    global $db;
    $sql = "
            SELECT s.*, h.tplid
            FROM obras2.supervisao s
            LEFT JOIN obras2.historicoparalisacao h  ON h.supidparalisacao = s.supid
            WHERE s.obrid = {$obrid} AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
            AND s.rsuid = 1
            --AND s.staid = 2
            --AND h.tplid = 5
            ORDER BY s.supdata DESC LIMIT 1";
    $sup = $db->pegaLinha($sql);

    if ($sup['staid'] == 2 AND $sup['tplid'] == 5)
        return true;
    else
        return false;
}

?>