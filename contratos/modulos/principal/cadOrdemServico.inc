<?php

    if($_REQUEST['req']) {
        $_REQUEST['req']($_REQUEST);
        exit;
    }
    
    function formatarDataPT_BR( $data ){
	$data = explode( ' ', $data );
	$data = explode( '-', $data[0] );
	return $data[2] . '/' . $data[1] . '/' . $data[0];
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

<?php

    if ($_SESSION['eveid']) {
        $sql = "
            SELECT  ev.evetitulo,  
                    ev.ungcod,
                    ev.tpeid,			 
                    ev.evedatainicio,		 
                    ev.evedatafim,			 
                    ev.eveemail,
                    ev.everespnome,
                    ev.everesptelefone, 		 
                    ev.evenumeropi, 		 
                    ev.evenumeroprocesso,   
                    ev.evecustoprevisto, 	 
                    ev.evepublicoestimado,  
                    ev.evequantidadedias,
                    ev.muncod,
                    mun.mundescricao, 				 
                    ev.estuf, 				 
                    ev.sevid,
                    ev.eveqtdpassagemaerea,
                    u.ungdsc,
                    us.usunome,
                    ev.docid,
                    ev.eveurgente,
                    ev.endid,
                    to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao,
                    oseid,
                    evelocal,
                    ev.ureid,
                    osenumeroos, 
                    to_char(osedataemissaoos::date,'DD/MM/YYYY') AS osedataemissaoos, 
                    to_char(osedatainiciofinal::date,'DD/MM/YYYY') as osedatainiciofinal, 
                    to_char(osedatafimfinal::date,'DD/MM/YYYY') as osedatafimfinal, 
                    osecustofinal, oseobsos, osecnpj, oserazaosocial, 
                    oseproposta, osecodpregao, oseordenador, oseempenho,
                    osetipoordenador, ureordenador, ureordenadorsub, osenumcontrato,
                    ep.empnumero as numero_empenho 
            FROM evento.evento AS ev
            LEFT JOIN evento.tipoevento AS    te ON te.tpeid  = ev.tpeid
            LEFT JOIN public.unidadegestora AS u ON u.ungcod = ev.ungcod
            LEFT JOIN evento.unidaderecurso AS ur ON ur.ungcod = ev.ungcod
            LEFT JOIN seguranca.usuario AS    us ON us.usucpf = ev.usucpf
            LEFT JOIN territorios.municipio mun ON mun.muncod = ev.muncod
            LEFT JOIN evento.ordemservico ord ON ord.eveid = ev.eveid
            LEFT JOIN evento.empenho_unidade ep ON ep.emuid = cast(ord.oseempenho as integer)            
            WHERE ev.eveid = {$_SESSION['eveid']}
        ";
        //ver($sql, d);
        $rsDadosEvento = $db->carregar($sql);
    }


    if ($_REQUEST['imprimir']) {
        /*
          $sql = "SELECT
          pi.plicod as codigo,
          pi.plicod || ' - ' || ug.ungabrev as descricao,
          ug.ungabrev
          FROM monitora.pi_planointerno pi
          INNER JOIN monitora.pi_subacao s ON s.sbaid = pi.sbaid
          INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = s.sbaid
          INNER JOIN public.unidadegestora ug ON ug.ungcod = sau.ungcod
          INNER JOIN evento.unidadeparceira up ON up.ungcod = ug.ungcod
          WHERE pi.plistatus = 'A'
          AND pi.pliano = '".$_REQUEST['anopi']."'
          --AND ug.ungcod IN (select ungcod from evento.unidadeparceira where eveid)
          ORDER BY ug.ungabrev, pi.plicod";

          $evenumeropi = $_REQUEST['nrpi'];
          $db->monta_combo('evenumeropi',$sql,'S','Selecione o PI...','','','','','N','evenumeropi');
         */
        ?>

        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug" >
            <tr bgcolor="#ffffff">
                <td valign="top" align="center">
                    <FONT SIZE="3">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                    <br>
                    <b>MINISTÉRIO DA EDUCAÇÃO</b>
                    <br>
                    <? echo $rsDadosEvento[0]['ungdsc'] ?>
                    <br>
                    <br>
                    <br>
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
                        <? 
                        $osecodpregao = explode("/", $rsDadosEvento[0]['osecodpregao']);
                        echo str_pad($rsDadosEvento[0]['osenumeroos'], 4, "0", STR_PAD_LEFT) . '/' . date("Y");
                        ?>
                    </b>
                </td>			
                <td valign="top" align="center">
                    Valor da Ordem de Serviço
                    <br>
                    <b>
                        R$ <? echo number_format($rsDadosEvento[0]["osecustofinal"], 2, ",", "."); ?>
                    </b>
                </td>
                <td valign="top" align="center">
                    Data Emissão
                    <br>
                    <b>
                        <? echo $rsDadosEvento[0]['osedataemissaoos']; ?>
                    </b>
                </td>
                <td valign="top" align="center">Processo do Evento
                    <br>
                    <b>
                        <?php 
                            //echo mascaraglobal2($rsDadosEvento[0]["evenumeroprocesso"], '#####.######/####-##'); 
                            echo $rsDadosEvento[0]['evenumeroprocesso']; 
                        ?>
                    </b>
                </td>
            </tr>				
        </table>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td align="center" bgcolor="silver" height="30px">
                    <b>
                        ESPECIFICAÇÃO DO SERVIÇO
                    </b>
                </td>			
            </tr>	

        </table>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="bordasimples" >
            <tr bgcolor="#ffffff">
                <td valign="top">
                    Fornecedor:
                    <b>
                        <?
                        echo $rsDadosEvento[0]['oserazaosocial'];
                        ?>
                    </b>
                </td>			
                <td valign="top">
                    CNPJ:
                    <b>
                        <? echo mascaraglobal2($rsDadosEvento[0]["osecnpj"], '##.###.###/####-##'); ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="2">
                    Período de Execução da OS:
                    <b>
                        <?
                            echo formatarDataPT_BR($rsDadosEvento[0]['evedatainicio']) . ' a ' . formatarDataPT_BR($rsDadosEvento[0]['evedatafim']);
                        ?>
                    </b>
                </td>			
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="2">
                    Evento:
                    <b>
                        <? echo $rsDadosEvento[0]["evetitulo"]; ?>
                    </b>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td valign="top" >
                    Local do Evento:
                    <b>
                        <? echo $rsDadosEvento[0]["evelocal"]; ?>
                    </b>
                </td>
                <td valign="top" colspan="2">
                    Cidade:
                    <b>
                        <? echo $rsDadosEvento[0]["mundescricao"] . '/' . $rsDadosEvento[0]["estuf"]; ?>
                    </b>
                </td>
            </tr>	
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="4">
                    Número do Empenho:
                    <b>
                        <?php
                        echo $rsDadosEvento[0]["numero_empenho"];
                        ?>
                    </b>
                </td>
            </tr>	
            <tr bgcolor="#ffffff">
                <td valign="top" colspan="4">
                    <br>
                    <b>Especificações Técnicas:</b>
                    <br><br>
                    
            <li>Os Serviços serão executados de acordo com as especificações desta O.S., e nas condições estabelecidas no contrato <b>Nº. <? echo $rsDadosEvento[0]["osenumcontrato"]; ?></b> – Pregão <b><? echo $rsDadosEvento[0]['osecodpregao']?></b>.
            </li>
            <br>
            <li>Os Serviços deverão ser executados conforme Proposta de Serviço, em anexo, encaminhada pela <b><? echo $rsDadosEvento[0]["oserazaosocial"]; ?></b> e aprovada pelo setor demandante.
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
                    a) A análise e aprovação do evento ocorreu na Ad Referendum reunião do Comitê de Eventos do MEC;
                    <br>
                    b) O Projeto Básico da unidade demandante consta dos autos do processo nº. <b><? echo $rsDadosEvento[0]["evenumeroprocesso"]; ?></b>;
                    <br>
                    c) As Propostas de Serviço/Preço nº. <b><? echo $rsDadosEvento[0]['oseproposta']; ?></b> foi devidamente analisada e aprovada pela Unidade Demandante conforme previsto no Art. 8º da Norma Operacional nº 01/2009;
                    <br>
                    d) Consta disponibilidade orçamentária na Unidade Gestora da <b>SAA</b> em cumprimento do Art. 9º da citada Norma Operacional.
                    <br><br><br>

                    <table width="100%" class="semborda">
                        <tr>
                            <td width="50%" align="center">
                                <b>Brasília, <? echo $rsDadosEvento[0]['osedataemissaoos']; ?><b>
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
                                    <br><?= $rsDadosEvento[0]['everespnome'] ?><br>
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
                    Autorizamos a execução da presente O.S., de acordo com a Proposta de Serviço nº <b><? echo $rsDadosEvento[0]['oseproposta']; ?></b> aprovada pela unidade demandante constante dos autos do processo nº. <b><? echo $rsDadosEvento[0]["evenumeroprocesso"]; ?></b>. 
                    <br><br><br>
                    <table width="100%" class="semborda">
                        <tr>
                            <td width="50%" align="center"><b>Brasília, <? echo $rsDadosEvento[0]['osedataemissaoos']; ?><b></td>
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
                                        if ($rsDadosEvento[0]['osetipoordenador'] == '2') {
                                            echo $rsDadosEvento[0]['ureordenadorsub'];
                                            echo "<br>";
                                            echo "Ordenador de Despesa Substituto";
                                        }else{
                                            echo $rsDadosEvento[0]['ureordenador'];
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

    if ($_POST['action'] == 'grava') {
        if ($_POST['oseid']) {
            $valoros = str_replace(',', '.', str_replace('.', '', $_POST['osecustofinal']));

            //atualiza saldo unidade gestora
            if ($_POST['ureid']) {
                if ($_POST['evecustoprevisto'] > $valoros) {
                    $diffValor = $_POST['evecustoprevisto'] - $valoros;
                    $sql = " insert into evento.unidadecontacorrente(ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf)
                                    values({$_POST['ureid']}, {$_SESSION['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['eveid']} ', {$diffValor}, '" . date('Y-m-d') . "', '{$_SESSION['usucpf']}');
                    ";
                    $sql .= " update evento.unidaderecurso set urevalorsaldo = urevalorsaldo+{$diffValor} where ureid = {$_POST['ureid']}; ";
                    $db->executar($sql);
                } elseif ($_POST['evecustoprevisto'] < $valoros) {
                    $diffValor = $valoros - $_POST['evecustoprevisto'];
                    $sql = " insert into evento.unidadecontacorrente(ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf)
                                    values({$_POST['ureid']}, {$_SESSION['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['eveid']} ', '{$diffValor}', '" . date('Y-m-d') . "', '{$_SESSION['usucpf']}');
                    ";
                    $sql .= " update evento.unidaderecurso set urevalorsaldo = urevalorsaldo-{$diffValor} where ureid = {$_POST['ureid']}; ";
                    $db->executar($sql);
                }
            }
            //atualiza local do evento
            $sql = "
                UPDATE evento.evento 
                        SET evelocal = '" . $_POST['evelocal'] . "'
                    WHERE eveid = " . $_SESSION['eveid'];

            $db->executar($sql);

            //atualiza OS
            $sql = "
                UPDATE evento.ordemservico 
                        SET oseproposta         = '" . $_POST['oseproposta'] . "',
                            oseempenho          = '" . $_POST['oseempenho'] . "',
                            osecustofinal       = '" . $valoros . "',
                            osetipoordenador    = '" . $_POST['osetipoordenador'] . "',
                            oseordenador        = '" . $_POST['oseordenador'] . "',
                            oseobsos            = '" . $_POST['oseobsos'] . "'
                    WHERE oseid = '" . $_POST['oseid'] . "'
            ";
            if ($db->executar($sql)) {
                $db->commit();
                echo"<script>alert('Dados gravados com sucesso.');window.location.href = 'evento.php?modulo=principal/cadOrdemServico&acao=A';</script>";
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

    if (( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '')) {
        array_push($res, array("descricao" => "Documentos Anexos",
            "id" => "3",
            "link" => "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
                ), array("descricao" => "Infraestrutura",
            "id" => "2",
            "link" => "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
                )
        );

        $pflcod = pegaPerfil($_SESSION['usucpf']);
        /*
          if( ( $pflcod == PERFIL_SUPER_USUARIO) || ( $pflcod == PERFIL_SAA ) )
          {
          array_push($res,
          array ("descricao" => "Hotéis",
          "id"		=> "1",
          "link"		=> "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A"
          )
          );
          }
         */
    }

    
/* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
*
    if ($_SESSION['eveid']) {
        array_push($res, array("descricao" => "Estrutura Orçamentária",
            "id" => "6",
            "link" => "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
                )
        );
    }
*/

    if (mostraAbaDocOS($_SESSION['eveid'])) {
        array_push($res, array("descricao" => "Ordem de Serviço",
            "id" => "7",
            "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
                )
        );
    }
	
    if (mostraAbaDocPagamento($_SESSION['eveid'])) {
        array_push($res, array("descricao" => "Documento de Pagamento",
            "id" => "5",
            "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
                )
        );
    }
    
    if (( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '')) {
        array_push($res, array("descricao" => "Avaliação",
            "id" => "0",
            "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
                )
        );
    }

    if ($_SESSION['eveid']) {
        echo'<br><br>';
        echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A");
    }

    headEvento($rsDadosEvento[0]['evetitulo'], $rsDadosEvento[0]['everespnome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao'], false);

    $docid = evtCriarDoc($_SESSION['eveid']);
    $esdid = verificaEstadoDocumento($docid);

    

    $somenteLeitura = 'N';
    if ($esdid == AGUARDANDO_EXECUCAO_EVENTO_WF) {
        $somenteLeitura = 'S';
    }

    //se existir proposta, bloqueia campos
    if ($rsDadosEvento[0]["oseproposta"]) {
        $somenteLeitura = 'N';
    }

    $perfis = arrayPerfil();

?>

<body>
    <form name = "formulario" method="post" id="formulario">

        <input type="hidden" name="action" id="action">
        <input type="hidden" name="oseid" value="<?= $rsDadosEvento[0]['oseid'] ?>">
        <input type="hidden" name="ureid" value="<?= $rsDadosEvento[0]['ureid'] ?>">
        <input type="hidden" name="evecustoprevisto" value="<?= $rsDadosEvento[0]['evecustoprevisto'] ?>">

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td  class="SubTituloDireita" colspan="3">&nbsp;</td>
                <td rowspan="14">
                    <?php wf_desenhaBarraNavegacao($docid, array('' => '')); ?>
                </td>
            </tr>
            <tr>
                <td width="28.5%" class ="SubTituloDireita" align="right">Nº da OS: </td>
                <td> 
                    <?
                    $osecodpregao = explode("/", $rsDadosEvento[0]['osecodpregao']);
                    echo str_pad($rsDadosEvento[0]['osenumeroos'], 4, "0", STR_PAD_LEFT) . '/' . $osecodpregao[0] . '/' . date("Y");
                    ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Data Emissão da OS: </td>
                <td> 
                    <? echo $rsDadosEvento[0]['osedataemissaoos']; ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº do Processo: </td>
                <td> 
                    <?php
                        //echo mascaraglobal2($rsDadosEvento[0]["evenumeroprocesso"], '#####.######/####-##'); 
                        echo $rsDadosEvento[0]['evenumeroprocesso']; 
                    ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº do Contrato: </td>
                <td> 
                    <? echo $rsDadosEvento[0]["osenumcontrato"]; ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Fornecedor: </td>
                <td> 
                    <? echo $rsDadosEvento[0]["oserazaosocial"]; ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">CNPJ: </td>
                <td> 
                    <? echo $rsDadosEvento[0]["osecnpj"]; ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Período Execução da OS:</td>
                <td> 
                    <? echo $rsDadosEvento[0]["osedatainiciofinal"] . ' à ' . $rsDadosEvento[0]["osedatafimfinal"]; ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Valor do Custo do Evento:</td>
                <td> 
                    <? echo number_format($rsDadosEvento[0]["evecustoprevisto"], 2, ",", "."); ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nome do Evento:</td>
                <td> 
                    <? echo $rsDadosEvento[0]["evetitulo"]; ?>     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Local do Evento:</td>
                <td> 
                    <? $evelocal = $rsDadosEvento[0]["evelocal"]; ?>     
                    <? echo campo_texto('evelocal', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="evelocal" onblur="MouseBlur(this);"'); ?>
                    -
                    <? echo $rsDadosEvento[0]["mundescricao"] . '/' . $rsDadosEvento[0]["estuf"]; ?>
                    <? echo obrigatorio(); ?>	              
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº da Proposta de Serviço/Preço:</td>
                <td> 
                    <? $oseproposta = $rsDadosEvento[0]["oseproposta"]; ?>     
                    <?= campo_texto('oseproposta', 'S', $somenteLeitura, '', 12, 10, '', '', 'left', '', 0, 'id="oseproposta" onblur="MouseBlur(this);"'); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nº do Empenho:</td>
                <td>
                    <?php
                        $sql = "SELECT emuid as codigo, empnumero||' - '||empdescricao as descricao FROM evento.empenho_unidade WHERE empstatus = 'A' and ungcod = '".$rsDadosEvento[0]["ungcod"]."' order by descricao";
			$db->monta_combo('oseempenho', $sql, $somenteLeitura, "Selecione...", '', '', '', '145', 'S', 'oseempenho', '', $rsDadosEvento[0]["oseempenho"], '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Valor da OS:</td>
                <td> 
                    <?
                    $osecustofinal = number_format($rsDadosEvento[0]["osecustofinal"], 2, ",", ".");
                    if ($osecustofinal == 0)
                        $osecustofinal = "";
                    ?>         
                    <?= campo_texto('osecustofinal', 'S', $somenteLeitura, '', 20, 20, '#.###.###.###,##', '', 'right', '', 0, 'id="osecustofinal" onblur="MouseBlur(this);"', 'veValorOS()'); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Tipo Ordenador de Despesa:</td>
                <td> 
                    <input type="radio" name="osetipoordenador" id="osetipoordenador" value="1" <?php echo $somenteLeitura == 'N' ? 'disabled="disabled"' : ''; ?> <? if ($rsDadosEvento[0]["osetipoordenador"] == '' || $rsDadosEvento[0]["osetipoordenador"] == '1') echo 'checked'; ?>> Ordenador de Despesa (Nome: <?= $rsDadosEvento[0]["ureordenador"] ?>)
                    <br>
                    <input type="radio" name="osetipoordenador" id="osetipoordenadorsub" value="2" <?php echo $somenteLeitura == 'N' ? 'disabled="disabled"' : ''; ?> <? if ($rsDadosEvento[0]["osetipoordenador"] == '2') echo 'checked'; ?>> Ordenador de Despesa Substituto (Nome: <?= $rsDadosEvento[0]["ureordenadorsub"] ?>)
                    <input type="hidden" name="oseordenador" id="oseordenador" value="">
                </td>
            </tr>
            <tr id="tr_justificativa" style="display: <? if ($rsDadosEvento[0]['evecustoprevisto'] == $rsDadosEvento[0]['osecustofinal']) echo 'none'; ?> ">
                <td class ="SubTituloDireita" align="right">Justificativa:</td>
                <td> 
                    <? $oseobsos = $rsDadosEvento[0]["oseobsos"]; ?>     
                    <?= campo_textarea('oseobsos', 'S', $somenteLeitura, '', 85, 8, 1500); ?>
                </td>
            </tr>
            <tr>
                <td  class ="SubTituloDireita" align="right">

                </td>
                <td>
                    <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');" <?php echo $somenteLeitura == 'N' ? 'disabled="disabled"' : '' ?>>  
                    <input type="button" name="btnCancel" value="Cancelar" id="btnCancel" onclick="validaForm('cancel');" <?php echo $somenteLeitura == 'N' ? 'disabled="disabled"' : '' ?>>
                    <? if ($oseproposta) { ?>  
                        <input type="button" name="btnImprimir" value="Imprimir OS" id="btnImprimir" onclick="imprimirOS();">
                    <? } ?>
                </td>

            </tr>	
        </table>

    </form>

</body>       

<script type="text/javascript">
    jQuery.noConflict();

    function validaForm(tipo){

        var evelocal = document.getElementById('evelocal');
        var oseproposta = document.getElementById('oseproposta');
        var oseempenho = document.getElementById('oseempenho');
        var osecustofinal = document.getElementById('osecustofinal');
        var osetipoordenador = document.getElementById('osetipoordenador');
        var oseordenador = document.getElementById('oseordenador');
        var oseobsos = document.getElementById('oseobsos');

        var dataValida = validarData();

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

            if (osecustofinal.value == ''){
                alert('O Campo "Valor da OS" é Obrigatório');
                document.formulario.osecustofinal.focus();
                return false;
            }

            if (osetipoordenador.checked == true){
                oseordenador.value = "<?= $rsDadosEvento[0]["ureordenador"] ?>";
            }

            if (osetipoordenadorsub.checked == true){
                oseordenador.value = "<?= $rsDadosEvento[0]["ureordenadorsub"] ?>";
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
                }else{
                    x = "f";
                }
            }
        });
        return x;
    }

function veValorOS()
{

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

function imprimirOS()
{
    var janela = window.open('evento.php?modulo=principal/cadOrdemServico&acao=A&imprimir=ok', 'imprimeos', 'height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
}


</script>