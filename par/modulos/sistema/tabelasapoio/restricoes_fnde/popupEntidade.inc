<?php
header('content-type: text/html; charset=ISO-8859-1');
require_once APPRAIZ . 'includes/workflow.php';

#Verifica se foi postado um resid e atribui valor
$docid = $_REQUEST['docid'];
$muncod = $_REQUEST['muncod'];
if ($_REQUEST['resid']) {
    # SQL Pegar Restricao por resid
    $resid = $_REQUEST['resid'];
    $sql = "
            SELECT DISTINCT
                res.resid,
                CASE WHEN re.inuid IS NULL THEN
                    'Termo'
                WHEN i.estuf IS NOT NULL THEN
                    'Estado'
                ELSE
                    'Município'
                END AS esfera,
                res.resdescricao,
                TO_CHAR(res.resdatainicio, 'DD/MM/YYYY') AS resdatainicio,
                TO_CHAR(res.resdatafim, 'DD/MM/YYYY') AS resdatafim,
                res.restemporestricao,
                res.usucpf,
                res.resstatus,
                res.tprid,
                res.resinformar,
                res.resprovidencia,
                res.ressuperado,
                res.resfluxo,
                res.docid
            FROM
                par.restricaofnde res
            LEFT JOIN par.restricaoentidade re ON(res.resid = re.resid)
            LEFT JOIN par.instrumentounidade i ON(re.inuid = i.inuid)
            WHERE
                res.resid = " . $resid . ";
        ";
    #restricao por resid
    $restricao = $db->pegaLinha($sql);

    #situacao do workflow do resid
    $sqlEstados = "
            SELECT DISTINCT
            	e.estuf,
                e.estdescricao
            FROM
                par.restricaofnde res
                JOIN par.restricaoentidade re ON(res.resid = re.resid)
                JOIN par.instrumentounidade i ON(re.inuid = i.inuid)
                JOIN territorios.estado e ON(i.estuf = e.estuf)
            WHERE
                res.resid = {$resid}";

    $restricao['listaEstado'] = $db->carregar($sqlEstados);

    #Municipios
    $sqlMunicipios = "
            SELECT DISTINCT
            	m.muncod,
                m.mundescricao || ' - ' || m.estuf AS descricao
            FROM
                par.restricaofnde res
                JOIN par.restricaoentidade re ON(res.resid = re.resid)
                JOIN par.instrumentounidade i ON(re.inuid = i.inuid)
                JOIN territorios.municipio m ON(i.muncod = m.muncod)
            WHERE
                res.resid = {$resid}";
    $restricao['listaMunicipio'] = $db->carregar($sqlMunicipios);
}
?>

<!-- Carrega Scripts -->
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<!--\ Carrega Scripts -->
<table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <th colspan="2">Dados da Restrição</th>
    </tr>
    <tr>
        <td style="width: 100%;" valign="top">
            <!-- Conteudo-->
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Código</b></td>
                    <td>
                        <?php
                        echo $resid;
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Restrição para</b></td>
                    <td>
                        <?php
                        echo $restricao['esfera'];
                        ?>
                    </td>
                </tr>
                <?php if ($restricao['esfera'] == 'Termo') { ?>
                    <!--Termo-->
                    <tr class="trRestricaoTermo">
                        <td class="SubTituloDireita" valign="top"><b>Lista de Termos</b></td>
                        <td>
                            <?php
                            # Carrega lista de Restricoes por resid
                            $listaFiltro = array('resid' => $resid, 'docid'=> $docid);
                            $sqlTermos = listarTermo($listaFiltro, 'SQL', true);
                            $cabecalho = array("Ação", "Termo", "Tipo do Termo", "Vigência", "Banco", "Agência", "Conta Corrente", "Valor Pago(R$)", "Processo", "Tipo do Processo");
                            $db->monta_lista($sqlTermos, $cabecalho, 25, 10, 'N', 'center', 'N');
                            ?>
                        </td>
                    </tr>
                    <!--\Termo-->
                <?php } else if ($restricao['esfera'] == 'Estado') { ?>
                    <!--Estado-->
                    <tr class="trRestricaoEstado">
                        <td class="SubTituloDireita" valign="top"><b>UF</b></td>
                        <td>
                            <?php
                            foreach ($restricao['listaEstado'] as $estado) {
                                if (!empty($muncod) && $muncod == $estado['estuf']) {
                                    echo $estado['estuf'] . " - " . $estado['estdescricao'] . "<br/>";
                                } else if (empty($muncod)) {
                                    echo $estado['estuf'] . " - " . $estado['estdescricao'] . "<br/>";
                                }
                            }
                            ?>

                        </td>
                    </tr>
                    <!--\Estado-->
                <?php } else { ?>
                    <!--Municipio-->
                    <tr class="trRestricaoMunicipio">
                        <td class="SubTituloDireita" valign="top"><b>Municípios</b></td>
                        <td>
                            <?php
                            foreach ($restricao['listaMunicipio'] as $municipio) {
                                if (!empty($muncod) && $muncod == $municipio['muncod']) {
                                    echo $municipio['muncod'] . " - " . $municipio['descricao'] . "<br/>";
                                } else if (empty($muncod)) {
                                    echo $municipio['muncod'] . " - " . $municipio['descricao'] . "<br/>";
                                }
                            }
                            ?>

                        </td>
                    </tr>
                    <!--Municipio-->
                <?php } ?>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Tipo da restrição FNDE</b></td>
                    <td>
                        <?php
                        $sql = "SELECT DISTINCT
                        tpr.tprdescricao AS descricao
                    FROM 
                        par.tiporestricaofnde tpr
                    WHERE
                        tpr.tprstatus = 'A'
                        AND tpr.tprid = " . $restricao['tprid'];
                        echo $db->pegaUm($sql);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
                    <td>
                        <?php echo $restricao['resdescricao']; ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Informar Município/Estado:</b></td>
                    <td>
                        <?php
                        if ($restricao['resinformar'] == 'S') {
                            $resinformar = 'Sim';
                        }
                        if ($restricao['resinformar'] == 'N') {
                            $resinformar = 'Não';
                        }
                        echo $resinformar;
                        ?> 
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Providências:</b></td>
                    <td>
                        <?php echo $restricao['resprovidencia']; ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Prazo da providência:</b></td>
                    <td>
                        <?php
                        if ($restricao['restemporestricao'] == 't') {
                            $restemporestricao = 'Sim';
                        }
                        if ($restricao['restemporestricao'] == 'f' || empty($restricao['restemporestricao'])) {
                            $restemporestricao = 'Não';
                        }
                        echo $restemporestricao;
                        ?>
                    </td>
                </tr>
                <?php if ($restemporestricao == 'Sim') { ?>
                    <tr class="trDataProvidencia">
                        <td class="SubTituloDireita" valign="top"><b>Data de início</b></td>
                        <td>
                            <?php echo $restricao['resdatainicio']; ?>
                        </td>
                    </tr>
                    <tr class="trDataProvidencia">
                        <td class="SubTituloDireita" valign="top"><b>Data de fim</b></td>
                        <td>
                            <?php echo $restricao['resdatafim']; ?>
                        </td>
                    </tr>
                <?php } ?>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Superado:</b></td>
                    <td>
                        <?php
                        if ($restricao['ressuperado'] == 'S') {
                            $restemporestricao = 'Sim';
                        }
                        if ($restricao['ressuperado'] == 'N') {
                            $restemporestricao = 'Não';
                        }
                        echo $restemporestricao;
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Arquivos:</b></td>
                    <td id="tdAnexo">
                        <br />
                        <br />
                        <?php
                        $acoes = '\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="excluirArquivo">
                        <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
                        </a>&nbsp;\'||
                        \'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
                        \' || arq.arqdescricao || \'
                        </a>\'
                    ';

                        if (!empty($restricao['resid'])) {
                            $cabecalho = array('Arquivo');

                            $sql = "SELECT
                            {$acoes} as acoes
                        FROM
                            par.anexorestricao ares
                            JOIN public.arquivo arq ON arq.arqid = ares.arqid
                            JOIN par.restricaofnde res ON ares.resid = res.resid
                        WHERE
                            ares.areststatus = 'A'
                            AND res.resid = {$restricao['resid']}";

                            $arrArquivos = $db->carregar($sql);
                            if (is_array($arrArquivos)) {
                                $db->monta_lista($arrArquivos, $cabecalho, 50, 10, '', 'center', 'N', '', '', '', '', '');
                            }
                        }
                        ?>
                    </td>
                </tr>
            </table>
            <!-- \Conteudo-->
        </td>
        <?php if ($docid) : ?>
            <td>
                <!-- Workfow -->
                <?php
                wf_desenhaBarraNavegacao($docid, array('docid' => $docid));
                ?>
                <!-- \Workfow -->
            </td>
        <?php endif; ?>
    </tr>
</table>
<?php
if (!$docid) {
    if ($restricao['esfera'] != 'Termo') {
        $coluna = '';
        $acao = '';
        $cabecalho = array();
        if ($restricao['esfera'] == 'Estado') {
            $coluna .= " uf.estuf, ";
            $cabecalho = array("Ações", "UF", "Estado");
            $acao = "'<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid || ',\'' || uf.estuf || '\')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'";
        }
        if ($restricao['esfera'] == 'Município') {
            $coluna .= " mun.estuf, mun.mundescricao, ";
            $cabecalho = array("Ações", "UF", "Município", "Estado");
            $acao = "'<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid || ',' || mun.muncod ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'";
        }
        
         $acao = "
        CASE WHEN re.inuid IS NULL THEN
        '<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
        WHEN uf.estuf IS NOT NULL THEN
        '<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid ||',\'' || uf.estuf || '\')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
        ELSE
        '<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid || ',' || mun.muncod ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
        END
        ||
        CASE WHEN resstatus = 'A' THEN
        '<a href=\"#\" onclick=\"alterarRestricaoFNDE(' || re.resid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
        ||
        '<a href=\"#\" onclick=\"excluirRestricaoEntidade(' || re.renid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
        ELSE
            ''
        END
    ";

        $sql = "
            SELECT DISTINCT 
                '<center>'||" . $acao . "||'</center>' AS acao,
                $coluna
                esd.esddsc
            FROM par.restricaoentidade re
            INNER JOIN par.restricaofnde res on re.resid=res.resid
            INNER JOIN par.instrumentounidade iu on iu.inuid=re.inuid
            LEFT JOIN territorios.estado uf on uf.estuf=iu.estuf
            LEFT JOIN territorios.municipio mun on mun.muncod=iu.muncod
            LEFT JOIN workflow.documento doc ON doc.docid = re.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE re.resid= {$resid}
        ";
        $db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', 'N');
    } else {
         # Carrega lista de Restricoes por resid
        $acaoTermo = "'<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'";
        $listaFiltro = array('resid'=> $resid, 'botaoacao' => $acaoTermo, 'camposRestricao' => 'esd.esddsc, ', );
        $sqlTermos = listarTermo($listaFiltro, 'SQL', false);
        $cabecalho = array("Ação", "Situacao", "Termo", "Tipo do Termo", "Vigência", "Banco", "Agência", "Conta Corrente", "Valor Pago(R$)", "Processo", "Tipo do Processo");
        $db->monta_lista($sqlTermos, $cabecalho, 25, 10, 'N', 'center', 'N');
    }
} else {
    if ($_REQUEST['btnVoltar'] != 'N') {
        echo "<input type='button' onclick='listaEntidade(\"$resid\")' value='Voltar' /> ";
    }
}
?>
<script>
    $(document).ready(function() {
       $('.trRestricaoTermo input').remove(); 
    });
       
</script>
<?php exit();?>
