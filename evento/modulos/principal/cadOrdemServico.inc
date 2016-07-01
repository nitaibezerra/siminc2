<?php

    if($_REQUEST['req']) {
        $_REQUEST['req']($_REQUEST);
        exit;
    }
?>

<head>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <style type="text/css">
        table.bordasimples {border-collapse: collapse;}
        table.bordasimples tr td {border:1px solid #000000;}
        table.semborda tr td {border:0px solid #000000;}
    </style>
</head>

<?PHP
    #BUSCA DADOS CO EVENTO NECESSARIO PARA O FUNCIONAMENTO DA TELA.
    if( $_SESSION['evento']['eveid'] != ''){

        $eveid = $_SESSION['evento']['eveid'];

        $rsDadosEvento = buscaDadosCadEvento( $eveid );
    }

    if ($_REQUEST['imprimir']) {
?>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug" >
            <tr bgcolor="#ffffff">
                <td valign="top" align="center">
                    <font size="3">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                    <br>
                    <b>MINISTÉRIO DA EDUCAÇÃO</b>
                    <br>
                    <? echo $rsDadosEvento['ungdsc'] ?>
                    <br><br>
                    <b>ORDEM DE SERVIÇO - OS</b>
                    </font>
                </td>
            </tr>
        </table>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td valign="top" align="center">
                    Ordem de Serviço
                    <br>
                    <b>
                        Nº.
                        <?PHP
                            $osecodpregao = explode("/", $rsDadosEvento['osecodpregao']);
                            echo str_pad($rsDadosEvento['osenumeroos'], 4, "0", STR_PAD_LEFT) . '/' . date("Y");
                        ?>
                    </b>
                </td>
                <td valign="top" align="center">
                    Valor da Ordem de Serviço
                    <br>
                    <b>
                        R$ <? echo number_format($rsDadosEvento["osecustofinal"], 2, ",", "."); ?>
                    </b>
                </td>
                <td valign="top" align="center">
                    Data Emissão
                    <br>
                    <b>
                        <? echo $rsDadosEvento['osedataemissaoos']; ?>
                    </b>
                </td>
                <td valign="top" align="center">Processo do Evento
                    <br>
                    <b>
                        <?php
                            //echo mascaraglobal2($rsDadosEvento["evenumeroprocesso"], '#####.######/####-##');
                            echo $rsDadosEvento['evenumeroprocesso'];
                        ?>
                    </b>
                </td>
            </tr>
        </table>

        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td align="center" bgcolor="silver" height="30px">
                    <b> ESPECIFICAÇÃO DO SERVIÇO </b>
                </td>
            </tr>
        </table>

        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td valign="top">
                    Fornecedor:
                    <b>
                        <?
                            echo $rsDadosEvento['oserazaosocial'];
                        ?>
                    </b>
                </td>
                <td valign="top">
                    CNPJ:
                    <b>
                        <?
                            echo mascaraglobal2($rsDadosEvento["osecnpj"], '##.###.###/####-##');
                        ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="2">
                    Período de Execução da OS:
                    <b>
                        <?
                            echo $rsDadosEvento['evedatainicio'] . ' a ' . $rsDadosEvento['evedatafim'];
                        ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="2">
                    Evento:
                    <b>
                        <?
                            echo $rsDadosEvento["evetitulo"];
                        ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" >
                    Local do Evento:
                    <b>
                        <?
                            echo $rsDadosEvento["evelocal"];
                        ?>
                    </b>
                </td>
                <td valign="top" colspan="2">
                    Cidade:
                    <b>
                        <?
                            echo $rsDadosEvento["mundescricao"] . '/' . $rsDadosEvento["estuf"];
                        ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="4">
                    Número do Empenho:
                    <b>
                        <?php
                            echo $rsDadosEvento["numero_empenho"];
                        ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="4">
                    <br>
                    <b>Especificações Técnicas:</b>
                    <br>
                    <br>

                    <li>
                        Os Serviços serão executados de acordo com as especificações desta O.S., e nas condições estabelecidas no contrato <b>Nº. <? echo $rsDadosEvento["osenumcontrato"]; ?></b> – Pregão <b><? echo $rsDadosEvento['osecodpregao']?></b>.
                    </li>
                    <br>
                    <li>
                        Os Serviços deverão ser executados conforme Proposta de Serviço, em anexo, encaminhada pela <b><? echo $rsDadosEvento["oserazaosocial"]; ?></b> e aprovada pelo setor demandante.
                    </li>
                    <br>
                </td>
            </tr>
        </table>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td align="center" bgcolor="silver" height="30px">
                    <b>
                        APOIO AO EVENTO
                    </b>
                </td>
            </tr>

        </table>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td valign="top">
                    <br>
                    <b>Ao Ordenador de Despesa,</b>
                    <br><br>
                    Tendo em vista as informações abaixo, solicito autorização final para execução da presente O.S.:
                    <br>
                    <?PHP
                        $adRef = $rsDadosEvento["eveurgente"];
                        if( $adRef == 't' ){
                            $msg = "no Ad Referendum";
                        }else{
                            $msg = "na {$rsDadosEvento['rcodescricao']}";
                        }
                    ?>
                    a) A análise e aprovação do evento ocorreu <?=$msg;?> do Comitê de Eventos do MEC;
                    <br>
                    b) O Projeto Básico da unidade demandante consta dos autos do processo nº. <b><? echo $rsDadosEvento["evenumeroprocesso"]; ?></b>;
                    <br>
                    c) As Propostas de Serviço/Preço nº. <b><? echo $rsDadosEvento['oseproposta']; ?></b> foi devidamente analisada e aprovada pela Unidade Demandante conforme previsto no Art. 8º da Norma Operacional nº 01/2009;
                    <br>
                    d) Consta disponibilidade orçamentária na Unidade Gestora <b> <?=$rsDadosEvento['ungdsc'] ?> </b> em cumprimento do Art. 9º da citada Norma Operacional.
                    <br><br><br>

                    <table width="100%" class="semborda">
                        <tr>
                            <td width="50%" align="center">
                                <b>Brasília, <? echo $rsDadosEvento['osedataemissaoos']; ?><b>
                            </td>
                            <td>&nbsp;</td>
                        </tr>
                    </table>

                    <br><br>

                    <table width="100%" class="semborda">
                        <tr>
                            <td width="50%">&nbsp;</td>
                            <td align="center">
                                <b>__________________________________________________
                                    <br><?= $rsDadosEvento['everespnome'] ?><br>
                                    Fiscal do Evento.
                                <b>
                            </td>
                        </tr>
                    </table>
                    <br>
                </td>
            </tr>
        </table>

        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td align="center" bgcolor="silver" height="30px">
                    <b>AUTORIZAÇÃO FINAL</b>
                </td>
            </tr>
        </table>

        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td valign="top">
                    <br>
                    Autorizamos a execução da presente O.S., de acordo com a Proposta de Serviço nº <b><? echo $rsDadosEvento['oseproposta']; ?></b> aprovada pela unidade demandante constante dos autos do processo nº. <b><? echo $rsDadosEvento["evenumeroprocesso"]; ?></b>.
                    <br><br><br>
                    <table width="100%" class="semborda">
                        <tr>
                            <td width="50%" align="center"><b>Brasília, <? echo $rsDadosEvento['osedataemissaoos']; ?><b></td>
                            <td>&nbsp;</td>
                        </tr>

                    </table>
                    <br><br>
                    <table width="100%" class="semborda">
                    <tr>
                        <td width="50%">&nbsp;</td>
                        <td align="center">
                            <b>__________________________________________________
                                <br>
                                    <?php
                                        if ($rsDadosEvento['osetipoordenador'] == '2') {
                                            echo $rsDadosEvento['ureordenadorsub'];
                                            echo "<br>";
                                            echo "Ordenador de Despesa Substituto";
                                        }else{
                                            echo $rsDadosEvento['ureordenador'];
                                            echo "<br>";
                                            echo "Ordenador de Despesa";
                                        }
                                    ?>
                                <br>
                            </b>
                         </td>
                         </tr>
                    </table>
                </td>
            </tr>
        </table>
<?php
        exit();
    }

    if ($_POST['action'] == 'grava'){

        #VARIAVEIS USADOS PARA AUTUALIZAR DADOS.
        $eveid = $_SESSION['evento']['eveid'];
        $emuid = $_POST['emuid'];
        $ureid = $_POST['ureid'];
        $coeid = $_POST['coeid'];

        if ($_POST['oseid']){
            #BUSCA DATAS.
            $evedatainclusao        = buscaDataInclusaoEvento();
            $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

            $valor_os = str_replace(',', '.', str_replace('.', '', $_POST['osecustofinal']));

            #ATUALIZA SALDO UNIDADE GESTORA
            if ($_POST['ureid']){
                if ($_POST['evecustoprevisto'] > $valor_os){
                    $diffValor = $_POST['evecustoprevisto'] - $valor_os;
                    #LEGADO
                    if( $evedatainclusao < $data_nova_regra_evento ){
                        $sql = "
                            INSERT INTO evento.unidadecontacorrente_old(
                                    ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                                )VALUES(
                                    {$_POST['ureid']}, {$_SESSION['evento']['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['evento']['eveid']} ', {$diffValor}, '" . date('Y-m-d') . "', '{$_SESSION['usucpf']}'
                            );
                        ";
                        $sql .= "
                            UPDATE evento.unidaderecurso_old set urevalorsaldo = urevalorsaldo+{$diffValor} WHERE ureid = {$_POST['ureid']};
                        ";
                    }else{
                        $sql = "
                            INSERT INTO evento.unidadecontacorrente(
                                    ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                                )VALUES(
                                    {$_POST['ureid']}, {$_SESSION['evento']['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['evento']['eveid']} ', {$diffValor}, '" . date('Y-m-d') . "', '{$_SESSION['usucpf']}'
                            );
                        ";
                    }
                    $db->executar($sql);

                }elseif( $_POST['evecustoprevisto'] < $valor_os ){
                    $diffValor = $valor_os - $_POST['evecustoprevisto'];

                    #LEGADO
                    if( $evedatainclusao < $data_nova_regra_evento ){
                        $sql = "
                            INSERT INTO evento.unidadecontacorrente_old(
                                    ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                                )values(
                                    {$_POST['ureid']}, {$_SESSION['evento']['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['evento']['eveid']} ', '{$diffValor}', '" . date('Y-m-d') . "', '{$_SESSION['usucpf']}'
                            );
                        ";
                        $sql .= "
                            UPDATE evento.unidaderecurso_old set urevalorsaldo = urevalorsaldo-{$diffValor} WHERE ureid = {$_POST['ureid']};
                        ";
                    }else{
                        $sql = "
                            INSERT INTO evento.unidadecontacorrente(
                                    ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                                )values(
                                    {$_POST['ureid']}, {$_SESSION['evento']['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['evento']['eveid']} ', '{$diffValor}', '" . date('Y-m-d') . "', '{$_SESSION['usucpf']}'
                            );
                        ";
                    }
                    $db->executar($sql);
                }
            }

            #NOVAS REGRAS.
            if( $evedatainclusao >= $data_nova_regra_evento ){
                #INICIO: ATUALIZA - EMPENHO
                #BUSCA E SOMA O VALOR "GASTO" NO EVENTO. ISSO SIGNIFICA QUE É SOMADO O VALOR PREVISTO OU DA "O.S" DE TODOS OS EVENTOS ATIVOS E QUE NÃO ESTEJAM EM CADASTRAMENTO, REALIZADOS COM O EMPENHO. DANDO PREFERENCIA AO VALOR DA OS.
                $sql = "
                    SELECT  SUM(
                                CASE WHEN osecustofinal IS NOT NULL
                                    THEN osecustofinal
                                    ELSE evecustoprevisto
                                END
                            ) AS custo
                    FROM evento.evento AS e
                    JOIN evento.ordemservico AS o ON o.eveid = e.eveid
                    JOIN workflow.documento AS d ON d.docid = e.docid
                    WHERE evestatus = 'A' AND esdid <> ".EM_CADASTRAMENTO_WF." AND e.emuid = {$emuid} AND e.eveid <> {$eveid}
                ";
                $ev_valor_utilizado = $db->pegaUm($sql);

                $OS_valor_utilizado = $ev_valor_utilizado + $valor_os;

                #ATUALIZA O EMPENHO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.empenho_unidade SET empvalorutilizado = '{$OS_valor_utilizado}' WHERE emuid = {$emuid} RETURNING emuid;";
                $db->pegaUm($sql);
                #FIM: ATUALIZA - EMPENHO

                #INICIO: ATUALIZA - UNIDADE
                #BUSCA VALOR UTILIZADO PELO EMPENHO, QUE É A SOMA DO VALOR UTILIZADO NO EMEPNHO PELA UNIDADE.
                $sql = "SELECT sum(empvalorutilizado) AS empvalorutilizado FROM evento.empenho_unidade WHERE ureid = {$ureid};";
                $ep_valor_utilizado = $db->pegaUm($sql);

                #ATUALIZA O EMPENHO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.unidaderecurso SET urevalorutilizado = '{$ep_valor_utilizado}' WHERE ureid = {$ureid} RETURNING ureid;";
                $db->pegaUm($sql);
                #FIM: ATUALIZA - UNIDADE

                #INICIO: ATUALIZA - CONTRATO
                #BUSCA VALOR UTILIZADO PELA UNIDADE, QUE É A SOMA DO VALOR UTILIZADO NA UNIDADE.
                $sql = "SELECT sum(urevalorutilizado) AS urevalorutilizado FROM evento.unidaderecurso WHERE ureid = {$ureid};";
                $co_valor_utilizado = $db->pegaUm($sql);

                #ATUALIZA O EMPENHO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.contratopregao SET coevalorutilizado = '{$co_valor_utilizado}' WHERE coeid = {$coeid} RETURNING coeid;";
                $up_saldo = $db->pegaUm($sql);
                #FIM: ATUALIZA - CONTRATO
            }

            #ATUALIZA LOCAL DO EVENTO
            #ATUALIZA OS
            $sql = "                
                UPDATE evento.evento
                    SET evelocal = '{$_POST['evelocal']}'
                WHERE eveid = {$_SESSION['evento']['eveid']};
                        
                UPDATE evento.ordemservico
                    SET oseproposta     = '{$_POST['oseproposta']}',
                        oseempenho      = '{$_POST['oseempenho']}',
                        osecustofinal   = '{$valor_os}',
                        osetipoordenador= '{$_POST['osetipoordenador']}',
                        oseordenador    = '{$_POST['oseordenador']}',
                        oseobsos        = '{$_POST['oseobsos']}',
                        rcoid           = {$_POST['rcoid']}
                WHERE oseid = {$_POST['oseid']}
            ";

            if ($db->executar($sql)) {
                $db->commit();
                echo"<script>alert('Dados gravados com sucesso.');window.location.href = 'evento.php?modulo=principal/cadOrdemServico&acao=A';</script>";
            }else{
                echo"<script>alert('Os dados não foram gravados, ocorreu algum problema. Entre em contato com o administrador do sistema!');window.location.href = 'evento.php?modulo=principal/cadOrdemServico&acao=A';</script>";
            }
        }
    }

    $titulo_modulo = "Eventos";
    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';
    monta_titulo($titulo_modulo, 'Ordem de Serviço.');

    $res = array(
        0 => array("descricao" => "Lista",
            "id" => "4",
            "link" => "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
        ),
        1 => array("descricao" => "Informações Básicas",
            "id" => "4",
            "link" => "/evento/evento.php?modulo=principal/cadEvento&acao=A"
        )
    );

    if (( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '')) {
        array_push($res, array("descricao" => "Documentos Anexos",
            "id" => "3",
            "link" => "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
                ), array("descricao" => "Infraestrutura",
            "id" => "2",
            "link" => "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
                )
        );

        $pflcod = pegaPerfil($_SESSION['usucpf']);
    }

    if (mostraAbaDocOS($_SESSION['evento']['eveid'])) {
        array_push($res, array("descricao" => "Ordem de Serviço",
            "id" => "7",
            "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
                )
        );
    }

    if (mostraAbaDocPagamento($_SESSION['evento']['eveid'])) {
        array_push($res, array("descricao" => "Documento de Pagamento",
            "id" => "5",
            "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
                )
        );
    }

    if (( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '')) {
        array_push($res, array("descricao" => "Avaliação",
            "id" => "0",
            "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
                )
        );
    }

    if ($_SESSION['evento']['eveid']) {
        echo'<br><br>';
        echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A");
    }

    headEvento($rsDadosEvento['evetitulo'], $rsDadosEvento['everespnome'], $rsDadosEvento['ungdsc'], $rsDadosEvento['ungcod'], $rsDadosEvento['eveurgente'], $rsDadosEvento['evedatainclusao'], false);

    $docid = evtCriarDoc($_SESSION['evento']['eveid']);
    $esdid = verificaEstadoDocumento($docid);



    #PODE SER HABILITADO OU NÃO TANTO PELA SITUAÇÃO OU POR JÁ EXISTIR PROPROSTA DA O.S.
    $habilitado = 'N';
    if ($esdid == AGUARDANDO_EXECUCAO_EVENTO_WF) {
        $habilitado = 'S';
    }

    #SE EXISTIR PROPOSTA, BLOQUEIA CAMPOS
    if( $rsDadosEvento["oseproposta"] ){
    //if( $rsDadosEvento["oseproposta"] == '' ){
        $habilitado = 'N';
    }

    $perfis = arrayPerfil();

    #CHECK BOX ORDENADOR.
    if( $habilitado == 'N' ){
        $disabled = 'disabled="disabled"';
    }else{
        $disabled = '';
    }

    if ($rsDadosEvento["osetipoordenador"] == '' || $rsDadosEvento["osetipoordenador"] == '1'){
        $checked_01 = 'checked';
    }else{
        $checked_01 = '';
    }

    if ($rsDadosEvento["osetipoordenador"] == '2'){
        $checked_02 = 'checked';
    }else{
        $checked_02 = '';
    }
?>

<body>
    <form name = "formulario" method="post" id="formulario">

        <input type="hidden" name="action" id="action">
        <input type="hidden" name="oseid" id="oseid" value="<?= $rsDadosEvento['oseid'] ?>">
        <input type="hidden" name="emuid" id="emuid" value="<?= $rsDadosEvento['emuid'] ?>">
        <input type="hidden" name="ureid" id="ureid" value="<?= $rsDadosEvento['ureid'] ?>">
        <input type="hidden" name="coeid" id="coeid" value="<?= $rsDadosEvento['coeid'] ?>">
        <input type="hidden" name="jusid" id="jusid" value="">

        <input type="hidden" name="evecustoprevisto" value="<?= $rsDadosEvento['evecustoprevisto'] ?>">

        <table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td  class="SubTituloDireita" colspan="3">&nbsp;</td>
                <td rowspan="16" width="6%" style="vertical-align: top;">
                    <?php wf_desenhaBarraNavegacao($docid, array('' => '')); ?>
                </td>
            </tr>
            <tr>
                <td width="28.5%" class ="SubTituloDireita" align="right">Nº da OS: </td>
                <td>
                    <?
                        $osecodpregao = explode("/", $rsDadosEvento['osecodpregao']);
                        echo str_pad($rsDadosEvento['osenumeroos'], 4, "0", STR_PAD_LEFT) . '/' . date("Y");
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Data Emissão da OS: </td>
                <td>
                    <? echo $rsDadosEvento['osedataemissaoos']; ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº do Processo: </td>
                <td>
                    <?php
                        //echo mascaraglobal2($rsDadosEvento["evenumeroprocesso"], '#####.######/####-##');
                        echo $rsDadosEvento['evenumeroprocesso'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº do Contrato: </td>
                <td>
                    <? echo $rsDadosEvento["osenumcontrato"]; ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Fornecedor: </td>
                <td>
                    <? echo $rsDadosEvento["oserazaosocial"]; ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">CNPJ: </td>
                <td>
                    <? echo $rsDadosEvento["osecnpj"]; ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Período Execução da OS:</td>
                <td>
                    <? echo $rsDadosEvento["osedatainiciofinal"] . ' à ' . $rsDadosEvento["osedatafimfinal"]; ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Valor do Custo do Evento:</td>
                <td>
                    <? echo number_format($rsDadosEvento["evecustoprevisto"], 2, ",", "."); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nome do Evento:</td>
                <td>
                    <? echo $rsDadosEvento["evetitulo"]; ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Local do Evento:</td>
                <td>
                    <?PHP
                        $evelocal = $rsDadosEvento["evelocal"];
                        echo campo_texto('evelocal', 'N', $habilitado, '', 78, 200, '', '', 'left', '', 0, 'id="evelocal" onblur="MouseBlur(this);"');
                    ?>
                    -
                    <?PHP
                        echo $rsDadosEvento["mundescricao"] . '/' . $rsDadosEvento["estuf"];
                        echo obrigatorio();
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº da Proposta de Serviço/Preço:</td>
                <td>
                    <? $oseproposta = $rsDadosEvento["oseproposta"]; ?>
                    <?= campo_texto('oseproposta', 'S', $habilitado, '', 30, 10, '', '', 'left', '', 0, 'id="oseproposta" onblur="MouseBlur(this);"'); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº do Empenho:</td>
                <td>
                    <?php
                        $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);
                        $data_inclusao_evento = strtotime($_SESSION['evento']['evedatainclusao']);

                        if( $data_inclusao_evento >= $data_nova_regra_evento ){
                            $emuid = $_SESSION['evento']['emuid'];
                            $sql = "SELECT emuid as codigo, empnumero||' - '||empdescricao as descricao FROM evento.empenho_unidade WHERE emuid = {$emuid}";
                        }else{
                            $sql = "SELECT emuid as codigo, empnumero||' - '||empdescricao as descricao FROM evento.empenho_unidade_old WHERE empstatus = 'A' and ungcod = '".$rsDadosEvento["ungcod"]."' order by descricao";
                        }
			$db->monta_combo('oseempenho', $sql, $habilitado, "Selecione...", '', '', '', '350', 'S', 'oseempenho', '', $rsDadosEvento["oseempenho"], '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Reunião do Comitê:</td>
                <td>
                    <?php
                        $sql = "
                            SELECT  rcoid as codigo,
                                    rcodescricao as descricao
                            FROM evento.reuniaocomite
                            WHERE rcostatus = 'A' AND ( extract( year from age( NOW()::DATE, rcodata::DATE ) ) * 12 + extract( month from age( NOW()::DATE, rcodata::DATE ) ) ) <= 6
                            ORDER BY descricao
                        ";
			$db->monta_combo('rcoid', $sql, $habilitado, "Selecione...", '', '', '', '350', 'S', 'rcoid', '', $rsDadosEvento["rcoid"], '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Valor da OS:</td>
                <td>
                    <?
                    $osecustofinal = number_format($rsDadosEvento["osecustofinal"], 2, ",", ".");
                    if ($osecustofinal == 0)
                        $osecustofinal = "";
                    ?>
                    <?= campo_texto('osecustofinal', 'S', $habilitado, '', 30, 20, '#.###.###.###,##', '', 'right', '', 0, 'id="osecustofinal" onblur="MouseBlur(this);"', 'veValorOS()'); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Tipo Ordenador de Despesa:</td>
                <td>
                    <input type="radio" name="osetipoordenador" id="osetipoordenador" value="1" <?PHP echo $disabled; echo $checked_01;?> > Ordenador de Despesa (Nome: <?= $rsDadosEvento["ureordenador"] ?>)
                    <br>
                    <input type="radio" name="osetipoordenador" id="osetipoordenadorsub" value="2" <?PHP echo $disabled; echo $checked_02;?> > Ordenador de Despesa Substituto (Nome: <?= $rsDadosEvento["ureordenadorsub"] ?>)
                    <input type="hidden" name="oseordenador" id="oseordenador" value="">
                </td>
            </tr>
            <tr id="tr_justificativa" style="display: <? if ($rsDadosEvento['evecustoprevisto'] == $rsDadosEvento['osecustofinal']) echo 'none'; ?> ">
                <td class ="SubTituloDireita" align="right">Justificativa:</td>
                <td>
                    <? $oseobsos = $rsDadosEvento["oseobsos"]; ?>
                    <?= campo_textarea('oseobsos', 'S', $habilitado, '', 85, 8, 1500); ?>
                </td>
            </tr>
            <tr>
                <td  class ="SubTituloDireita" align="right">

                </td>
                <td>
                    <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>>
                    <input type="button" name="btnCancel" value="Cancelar" id="btnCancel" onclick="validaForm('cancel');" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>>
                    <?  if ($oseproposta) { ?>
                        <input type="button" name="btnImprimir" value="Imprimir OS" id="btnImprimir" onclick="imprimirOS();">

                    <?PHP
                        }

                        $perfis = pegaPerfilGeral();

                        if( ( in_array(EVENTO_PERFIL_SUPER_USUARIO, $perfis) || in_array(EVENTO_PERFIL_SAA, $perfis) ) && $oseproposta ){
                    ?>
                            <input type="button" id="cancelar" name="cancelar" value="OS Extraordinaria" onclick="abrirJustOsExtraord();"/>
                    <?PHP
                        }
                    ?>
                </td>

            </tr>
        </table>

    </form>

</body>

<script type="text/javascript">
    jQuery.noConflict();

    function abrirJustOsExtraord(){
        if(confirm("A OS já foi gerada, gravada e a justificativa já foi realizada. Deseja alterar ou acrecentar algo a justificativa?")){
            var url = 'evento.php?modulo=principal/eventos/cad_gera_os_estraordinario&acao=A';
            var janela = window.open(url, 'imprimeos', 'height=620,width=800,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
        }
    }

    function validaForm(tipo){

        var evelocal        = document.getElementById('evelocal');
        var oseproposta     = document.getElementById('oseproposta');
        var oseempenho      = document.getElementById('oseempenho');
        var osecustofinal   = document.getElementById('osecustofinal');
        var osetipoordenador= document.getElementById('osetipoordenador');
        var oseordenador    = document.getElementById('oseordenador');
        var oseobsos        = document.getElementById('oseobsos');
        var rcoid           = document.getElementById('rcoid');

        //ESSA VARIAVEL É "PREENCHIDA" PELA JANELA FILHO E É USADO APENAS PARA CONTROLE. VERIFICAÇÃO QUE JA FOI FEITA A JUSTIFICATIVA DA OS EXTRAORDINARIA.
        var jusid           = document.getElementById('jusid');

        var dataValida = validarData();

        //BLOCO DE CONTROLE.
        if( jusid.value == '' ){
            if( trim(dataValida) == 'saa' ){
                if(confirm("A data de geração da OS não esta em conformidade com realização do evento que já aconteceu. Deseja gravar a OS mesmo assim?")){
                    var url = 'evento.php?modulo=principal/eventos/cad_gera_os_estraordinario&acao=A';
                    var janela = window.open(url, 'imprimeos', 'height=620,width=800,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
                }
                return false;
            }
        }

        if (tipo == 'grava'){

            if( dataValida == 'f' ){
                alert('Data de emissão da Ordem de Serviço tem que ser menor que a data de início do evento!');
                return false;
            }

            if (evelocal.value == ''){
                alert('O Campo "Local do Evento:" é Obrigatório');
                document.formulario.evelocal.focus();
                return false;
            }

            if (oseproposta.value == ''){
                alert('O Campo "Nº da Proposta de Serviço/Preço" é Obrigatório');
                document.formulario.oseproposta.focus();
                return false;
            }

            if (oseempenho.value == ''){
                alert('O Campo "Nº do Empenho" é Obrigatório');
                document.formulario.oseempenho.focus();
                return false;
            }

            if (rcoid.value == ''){
                alert('O Campo "Reunião do Comitê" é Obrigatório');
                document.formulario.rcoid.focus();
                return false;
            }

            if (osecustofinal.value == ''){
                alert('O Campo "Valor da OS" é Obrigatório');
                document.formulario.osecustofinal.focus();
                return false;
            }

            if (osetipoordenador.checked == true){
                oseordenador.value = "<?= $rsDadosEvento["ureordenador"] ?>";
            }

            if (osetipoordenadorsub.checked == true){
                oseordenador.value = "<?= $rsDadosEvento["ureordenadorsub"] ?>";
            }

            if (oseordenador.value == ''){
                alert('Não existe o nome do "Ordenador de Despesa", entre em contato com o administrador do sistema!');
                document.formulario.osetipoordenador.focus();
                return false;
            }

            var vlcontrato = document.formulario.evecustoprevisto.value;
            if (!vlcontrato){
                vlcontrato = 0;
            }else{
                vlcontrato = Number(vlcontrato);
            }

            var vlos = document.getElementById('osecustofinal').value;
            vlos = vlos.replace(/\./gi, '');
            vlos = Number(vlos.replace(/,/gi, "."));

            if(vlos != vlcontrato){
                if (oseobsos.value == ''){
                    alert('O Campo "Justificativa" é Obrigatório');
                    document.formulario.oseobsos.focus();
                    return false;
                }
            }
            document.getElementById('action').value = tipo;
            document.formulario.submit();
        }

        if (tipo == 'cancel'){
            window.location.href = "evento.php?modulo=principal/cadOrdemServico&acao=A";
        }
    }

    function validarData(){
        jQuery.ajax({
            url     : window.location,
            type    : "POST",
            data    : "req=validaDataEvento",
            dataType: 'script',
            async   : false,
            success: function(dados){
                if( trim(dados) == "ok"){
                    x = "t";
                }else
                    if( trim(dados) == "saa" ){
                    x = "saa";
                }else{
                    x = "f";
                }
            }
        });
        return x;
    }

    function veValorOS(){

        var vlcontrato = document.formulario.evecustoprevisto.value;
        if (!vlcontrato)
            vlcontrato = 0;
        else
            vlcontrato = Number(vlcontrato);

        var vlos = document.getElementById('osecustofinal').value;
        vlos = vlos.replace(/\./gi, '');
        vlos = Number(vlos.replace(/,/gi, "."));

        var tr_justificativa = document.getElementById('tr_justificativa');

        if (vlos != vlcontrato) {
            tr_justificativa.style.display = '';
        }
        else {
            tr_justificativa.style.display = 'none';
        }
    }

    function imprimirOS(){
        var janela = window.open('evento.php?modulo=principal/cadOrdemServico&acao=A&imprimir=ok', 'imprimeos', 'height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no').focus();
    }

</script>