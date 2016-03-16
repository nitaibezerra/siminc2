<?php
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

if($_POST['requisicao'] && $_POST['requisicao'] == 'aviso_cumprimento') {
    $cumprimentoObjeto = new CumprimentoObjeto();
    $cumprimentoObjeto->marcarAlertaLido($_POST['coid']);
}

if($_GET['arquivo'])
{
       include_once APPRAIZ . "includes/classes/file.class.inc";
       include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
       $file = new FilesSimec();
       $file-> getDownloadArquivo($_GET['arquivo']);
       exit;
}

function pegaEstruturaFormularioMI(){
    global $db;
    
    $sql = "select * from obras2.tecnologiami";
    
    $tecnologias = $db->carregar($sql);
    foreach ($tecnologias as $key => $tecnologia){
        $sql = "select * from obras2.formulariotecnologiami where ftistatus = 'A' and tmiid = " . $tecnologia['tmiid'];
        $questionario = $db->carregar($sql);
        $tecnologias[$key]['formulario'] = $questionario;
    }
    return $tecnologias;
}

function pegaDadosVistoriaObra($mi = true){
    global $db;
    $resp = array();
    $arPflcod = array();
    $join = '';
    $where  = array();

    $arrUni = Array(PFLCOD_GESTOR_UNIDADE);
    if (possui_perfil($arrUni)) {
        $resp[] = "urs.entid = e.entidunidade ";
        $arPflcod = array_merge($arPflcod, $arrUni);
    }

    if ( possui_perfil( Array(PFLCOD_SUPERVISOR_UNIDADE))  ){
        $usuarioResp = new UsuarioResponsabilidade();
        $arEmpid 	 = $usuarioResp->pegaEmpidPermitido( $_SESSION['usucpf'] );
        $arEmpid	 = ($arEmpid ? $arEmpid : array(0));
        $resp[] 	= " urs.entid = e.entidunidade AND e.empid IN('" . implode("', '", $arEmpid) . "')";
        $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
    }

    if( count($resp) ){
        $join = "JOIN obras2.usuarioresponsabilidade urs ON 
                            urs.rpustatus = 'A' AND
                            urs.usucpf = '". $_SESSION['usucpf'] ."' AND 
                            urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                            (".implode(' OR ',$resp).")";
    }

    if($mi === true)
        $where[] = 'AND o.tpoid IN (104,105)';
    elseif($mi === false)
        $where[] = 'AND o.tpoid NOT IN (104,105)';
    
    $sql = "
        SELECT
            nivelpreenchimento,
            sum(contador) AS total
        FROM (
            SELECT
                DISTINCT(e.empid),
                CASE
                    WHEN d.esdid IN (690, 691) AND o.obrdtvistoria IS NOT NULL THEN
                        (CASE
                            WHEN DATE_PART('days', NOW() - o.obrdtvistoria) <= 45 THEN '1 - Obras atualizadas há menos de 45 dias atrás'
                            WHEN DATE_PART('days', NOW() - o.obrdtvistoria) BETWEEN 45  AND 60 THEN '2 - Obras atualizadas entre 45 e 60 dias'
                            ELSE '3 - Obras atualizadas há mais de 60 dias'
                        END)
                    WHEN d.esdid = 693 THEN '4 - Obras concluídas'
                    ELSE '5 - Não se aplica'
                END AS nivelpreenchimento,
                1 AS CONTADOR
            FROM obras2.obras o
            INNER JOIN obras2.tipologiaobra 		tpo ON tpo.tpoid = o.tpoid
            INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
            INNER JOIN workflow.documento d ON d.docid = o.docid
            $join
            WHERE o.obrstatus = 'A'
            AND e.orgid=3
            AND d.esdid <> 770 --Etapa Concluída
            AND o.obridpai IS NULL
            ".  implode($where, ' AND ') . "
        ) as FOO
        GROUP BY nivelpreenchimento
        ORDER BY 1
        ";


    return $db->carregar($sql,null,3200);
    
}

function montaArrayVistoria (){
    $arrVistoriaTotal = pegaDadosVistoriaObra(null);
    $arrVistoriaMI = pegaDadosVistoriaObra(true);
    $arrVistoria = pegaDadosVistoriaObra(false);

    $arrVistoriaModelo = array(
        array('nivelpreenchimento' => '1 - Obras atualizadas há menos de 45 dias atrás', 'total' => 0, 'corpreenchimento' => '#80BC44', 'mi' => 0, 'convencional' => 0),
        array('nivelpreenchimento' => '2 - Obras atualizadas entre 45 e 60 dias', 'total' => 0, 'corpreenchimento' => '#FFC211', 'mi' => 0, 'convencional' => 0),
        array('nivelpreenchimento' => '3 - Obras atualizadas há mais de 60 dias', 'total' => 0, 'corpreenchimento' => '#E95646', 'mi' => 0, 'convencional' => 0),
        array('nivelpreenchimento' => '4 - Obras concluídas', 'total' => 0, 'corpreenchimento' => '#2B86EE', 'mi' => 0, 'convencional' => 0),
        array('nivelpreenchimento' => '5 - Não se aplica', 'total' => 0, 'corpreenchimento' => '#888888', 'mi' => 0, 'convencional' => 0),
    );

    if(!$arrVistoria)
        return $arrVistoriaModelo;

    foreach($arrVistoria as $vistoria){
        $index = substr($vistoria['nivelpreenchimento'], 0, 1) - 1;
        $arrVistoriaModelo[$index]['convencional'] = $vistoria['total'];
    }
    if($arrVistoriaMI){
        foreach($arrVistoriaMI as $vistoria){
            $index = substr($vistoria['nivelpreenchimento'], 0, 1) - 1;
            $arrVistoriaModelo[$index]['mi'] = $vistoria['total'];
        }
    }
    if($arrVistoriaTotal){
        foreach($arrVistoriaTotal as $vistoria){
            $index = substr($vistoria['nivelpreenchimento'], 0, 1) - 1;
            $arrVistoriaModelo[$index]['total'] = $vistoria['total'];
        }
    }

    return $arrVistoriaModelo;
}

?>
<table class="tbl_conteudo" width="100%" border="0" cellpadding="0" cellspacing="0">
    <tbody>
        <tr> 
            <td valign="top" style="padding-bottom:15px;">
                <br><table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tbody><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Início</label></td></tr></tbody></table><style>
                            .divGraf{
                                width: 330px;
                                /*height: 200px;*/
                                float: left;
                                margin-top: 20px;
                                margin-left: 30px;
                                -webkit-border-radius: 20px;
                                -moz-border-radius: 20px;
                                border-radius: 20px;
                            }

                            .linkMais{
                                float: left;
                                margin-left: 10px;
                                margin-top: 10px;
                                color: #FFF;
                                font-size: 18px;
                            } 
                            .btnNormal{
                                background-color: #FFF;
                                -webkit-border-radius: 5px;
                                -moz-border-radius: 5px;
                                border-radius: 5px;
                                float: left;
                                width: 140px;
                                color: #002A6C;
                                font-weight: bold;
                                text-align: center;
                                margin-top: 10px;
                                margin-left: 10px;
                                cursor: pointer;
                                padding:5px;
                            }
                            .btnNormal2{
                                background-color: #FFF;
                                -webkit-border-radius: 5px;
                                -moz-border-radius: 5px;
                                border-radius: 5px;
                                float: left;
                                width: 140px;
                                color: #002A6C;
                                font-weight: bold;
                                text-align: center;
                                margin-top: 10px;
                                margin-left: 10px;
                                padding:5px;
                            }
                            .btnNorma13{
                                max-height: 200px;
                                overflow: auto;
                            }
                            #btnAcao1{
                                width: 300px;
                                text-align: left;
                                cursor: auto;
                            }
                            .btnNormal:hover{
                                background-color: #C0C0C0;
                            }
                            #divAcoes{
                                background-color: yellowgreen;
                                padding-bottom: 10px;
                                /*height: 216px;*/
                            }
                            #divAjuda{
                                background-color: #9966CC;
                                padding-bottom: 10px;
                                /*height: 216px;*/
                            }

                            #divAlerta{
                                background-color: #cc3600;
                                padding-bottom: 10px;
                                /*height: 216px;*/
                            }
                            #divObras{
                                background-color:#EEB422 ;
                                height: 216px;
                            }
                            #divObrasMI{
                                background-color:#00CED1 ;
                                height: 216px;
                            }
                            #divVistoria{
                                background-color:#00772B ;
                                padding-bottom: 10px;
                                /*height: 216px;*/
                            }
                            #divTermo{
                                background-color:#FF7400 ;
                                padding-bottom: 10px;
                                /*height: 216px;*/
                            }
                            .tabela_box td{
                                background: #CCC;
                            }
                            #conteudoPainel{
                                width: 730px;
                                margin: 0 auto;
                            }

                </style>
                <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
                <script>
                    $(document).ready(function()
                    {
                        /* Ações */
                        $('#btnObras').click(function() {
                            location.href = '?modulo=principal/listaObras&acao=A';
                        });
                        $('#btnObrasMI').click(function() {
                            location.href = '?modulo=principal/listaObrasMI&acao=A';
                        });
                        $('#btnAcao1').click(function() {
//                            location.href = '?modulo=principal/listaObrasMI&acao=A';
                        });
                        $('#btnEmissaoMI').click(function() {
                            location.href = 'obras2.php?modulo=principal/listaObrasMI&acao=O';
                        });
                    });
                </script>

                <?php
                $arrVistoria = montaArrayVistoria();
                ?>

                <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
                    <tr>
                        <td>



                    <div id="conteudoPainel">



                            <div id="divAlerta" class="divGraf" style="width: 438px" >
                                <center>
                                    <spam class="linkMais"><img src="/imagens/exclamacao_checklist.png"> Atenção</spam> <br><br><br>
                                </center>

                                <div class="btnNormal2" style="width: 408px;" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                    </div>

                                    <div style="font-size: 14px">
                                        <div>
                                            <p>Em razão das novas regras de transferência de recursos aos municípios, estados e Distrito Federal, o FNDE, com escopo de aprimorar o processo de integração entre os entes federados e esta Autarquia, criou ferramenta no módulo Obras 2.0 (SIMEC), na qual ocorrerá a solicitação do desembolso. Segue abaixo documentos:</p>
                                            <p><a style="font-size:14x; color:#FF0101;" href="/obras2/tutorial/oficio_circular_01_2015_CGIMP_DIGAP_FNDE_MEC.pdf"><img src="/imagens/salvar.png" border="0"> Ofício Circular 01/2015 CGIMP/DIGAP/FNDE/MEC - Clique AQUI</a></p>
                                            <p><a style="font-size:14px; color:#FF0101;" href="/obras2/tutorial/manual_para_solicitar_desembolso.pdf"><img src="/imagens/salvar.png" border="0"> Manual de Solicitação de Desembolso - Clique AQUI</a></p>
                                        </div>
                                    </div>
                                </div>

                                <div class="btnNormal2" style="width: 408px;" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                    </div>

                                    <div style="font-size: 13px">
                                        <div>
                                            O processo de criação de obra vinculada (rescisão contratual / nova contratação) foi alterado, o município deve solicitar a vinculação, conforme manual disponibilizado no sistema. - CLIQUE <a href="/obras2/tutorial/manual_obra_vinculada.pdf" id=""><b><u>AQUI</u></b></a>
                                        </div>
                                    </div>
                                </div>

                                <div class="btnNormal2" style="width: 408px;" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                    </div>

                                    <div>
                                        <div>
                                            <b>METODOLOGIA INOVADORA</b> - Os municípios contemplados, que não tiveram o contrato assinado com as empresas vencedoras da Ata de Registro de Preços, deverão aguardar contato do FNDE nos próximos dias.
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <div id="divAlerta" class="divGraf" style="width: 217px; background-color: #37ABFF/*#9749AD*/" >
                                <center>
                                    <spam class="linkMais">Contatos</spam> <br><br><br>
                                </center>

                                <div class="btnNormal2" style="width: 188px;" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                    </div>

                                    <div>
                                        <div>
                                            CLIQUE <a href="" id="contatos"><b><u>AQUI</u></b></a> para ver a lista de contatos do FNDE (telefone e e-mail's), e em caso de dúvidas tais como alteração no projeto, uso de saldo em conta, troca de terreno, troca de metodologia construtiva, dentre outras.
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <div id="divAlerta" class="divGraf" style="width: 217px; background-color: #9966CC" >
                                <center>
                                    <spam class="linkMais">Manuais</spam> <br><br><br>
                                </center>

                                <div class="btnNormal2" style="width: 188px;" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                    </div>

                                    <div>
                                        <div>
                                            <a href="" id="manuais">Ajuda/Manuais</a>
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <div id="divVistoria"  class="divGraf" style="width: 688px">
                                <center>
                                    <spam class="linkMais">Vistoria</spam> <br><br><br>
                                </center>
                                <div style="font-size: 13px; text-align: center;" class="linkMais">
                                    Situação quanto ao nível de preenchimento
                                </div>
                                <div class="btnNormal2" style="width: 658px;" id="btnAcao1">
                                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                                        <tr class="fundo_tr_wap">
                                            <td class="fundo_td_wap center bold" >Preenchimento</td>
                                            <td class="fundo_td_wap center bold" >MI</td>
                                            <td class="fundo_td_wap center bold" >Conv.</td>
                                            <td class="fundo_td_wap center bold" >Total</td>
                                        </tr>
                                        <?php
                                        if($arrVistoria[0]):
                                            foreach($arrVistoria as $vis) :
                                                if($link=='S'){ ?>
                                                    <tr class="fundo_tr_wap">
                                                        <td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap link" onclick="abreRelatorioObras(<?=$parametroColTotal?>);"><?=$vis['nivelpreenchimento']?></td>
                                                        <td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroColTotal?>);" ><?=$vis['convencional'] ?></td>
                                                    </tr>
                                                <?}else{?>
                                                    <tr class="fundo_tr_wap">
                                                        <td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap"><?=str_replace(array("1 - ","2 - ","3 - ","4 -", "5 - "),"",$vis['nivelpreenchimento'])?></td>
                                                        <td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero" ><?=$vis['mi'] ?></td>
                                                        <td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero" ><?=$vis['convencional'] ?></td>
                                                        <td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero" ><?=$vis['total'] ?></td>
                                                    </tr>
                                                <?
                                                }
                                                $tottotalC += $vis['convencional'];
                                                $tottotalMI += $vis['mi'];
                                                $tottotal += $vis['total'];
                                            endforeach;
                                        endif;
                                        ?>
                                        <tr class="fundo_tr_wap">
                                            <td class="fundo_td_wap bold">Total</td>
                                            <?if($legenda1):?>
                                                <td class="fundo_td_wap numero"><?=$totcoluna1 ?></td>
                                                <td class="fundo_td_wap numero"><?=$totcoluna2 ?></td>
                                            <?endif;?>
                                            <td class="fundo_td_wap numero"><?=$tottotalMI ?></td>
                                            <td class="fundo_td_wap numero"><?=$tottotalC ?></td>
                                            <td class="fundo_td_wap numero"><?=$tottotal ?></td>
                                        </tr>
                                    </table>
                                </div>
                            </div>



                            <div id="divTermo"  class="divGraf" style="width: 688px; ">
                                <center>
                                    <spam class="linkMais">Termo</spam> <br><br><br>
                                </center>
                                <div style="font-size: 13px; text-align: center;" class="linkMais">
                                    Vigente
                                </div>
                                <div class="btnNormal2 btnNorma13" style="width: 658px;" id="btnAcao1">
                                    <? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) { ?>
                                        Nenhuma obra sob sua responsabilidade.
                                    <? } else {?>
                                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                                        <tr class="fundo_tr_wap">
                                            <td class="fundo_td_wap center bold" >ID</td>
                                            <td class="fundo_td_wap center bold" >Nome</td>
                                            <td class="fundo_td_wap center bold" >Tipologia</td>
                                            <td class="fundo_td_wap center bold" >Esfera</td>
                                            <td class="fundo_td_wap center bold" >Nº Termo</td>
                                            <td class="fundo_td_wap center bold" >Fim Vigência</td>
                                        </tr>
                                        <? $obras = pegaTermoVigenteObras();?>
                                        <? foreach($obras as $obra):?>
                                        <tr class="fundo_tr_wap">
                                            <td style="background-color:#80BC44;" class="fundo_td_wap"><?=$obra['obrid']?></td>
                                            <td style="background-color:#80BC44;" class="fundo_td_wap"><?=$obra['obrnome']?></td>
                                            <td style="background-color:#80BC44;" class="fundo_td_wap"><?=$obra['tpodsc']?></td>
                                            <td style="background-color:#80BC44;" class="fundo_td_wap"><?=$obra['empesfera']?></td>
                                            <td style="background-color:#80BC44;" class="fundo_td_wap"><?=$obra['termo']?></td>
                                            <td style="background-color:#80BC44;" class="fundo_td_wap"><?=$obra['fim_vigencia']?></td>
                                        </tr>
                                        <? endforeach; ?>
                                    </table>
                                    <? } ?>
                                </div>


                                <div style="font-size: 13px; text-align: center;" class="linkMais">
                                    Vencimento em 90 dias
                                </div>
                                <div class="btnNormal2 btnNorma13" style="width: 658px;" id="btnAcao1">
                                    <img src="/imagens/atencao.png" style="width: 16px" /> Para prorrogar é necessário acessar o módulo PAR/Lista de Obras, na senha do Prefeito.
                                    <? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) { ?>
                                        <br />Nenhuma obra sob sua responsabilidade.
                                    <? } else {?>
                                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                                        <tr class="fundo_tr_wap">
                                            <td class="fundo_td_wap center bold" >ID</td>
                                            <td class="fundo_td_wap center bold" >Nome</td>
                                            <td class="fundo_td_wap center bold" >Tipologia</td>
                                            <td class="fundo_td_wap center bold" >Esfera</td>
                                            <td class="fundo_td_wap center bold" >Nº Termo</td>
                                            <td class="fundo_td_wap center bold" >Fim Vigência</td>
                                        </tr>
                                        <? $obras = pegaTermoVencendoObras(30, 90);?>
                                        <? foreach($obras as $obra):?>
                                            <tr class="fundo_tr_wap">
                                                <td style="background-color:#FFC211;" class="fundo_td_wap"><?=$obra['obrid']?></td>
                                                <td style="background-color:#FFC211;" class="fundo_td_wap"><?=$obra['obrnome']?></td>
                                                <td style="background-color:#FFC211;" class="fundo_td_wap"><?=$obra['tpodsc']?></td>
                                                <td style="background-color:#FFC211;" class="fundo_td_wap"><?=$obra['empesfera']?></td>
                                                <td style="background-color:#FFC211;" class="fundo_td_wap"><?=$obra['termo']?></td>
                                                <td style="background-color:#FFC211;" class="fundo_td_wap"><?=$obra['fim_vigencia']?></td>
                                            </tr>
                                        <? endforeach; ?>
                                    </table>
                                    <? } ?>
                                </div>


                                <div style="font-size: 13px; text-align: center;" class="linkMais">
                                    Vencimento em 30 dias
                                </div>
                                <div class="btnNormal2 btnNorma13" style="width: 658px;" id="btnAcao1">
                                    <img src="/imagens/atencao.png" style="width: 16px" /> Para prorrogar é necessário acessar o módulo PAR/Lista de Obras, na senha do Prefeito.
                                    <? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) { ?>
                                        <br />Nenhuma obra sob sua responsabilidade.
                                    <? } else {?>
                                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                                        <tr class="fundo_tr_wap">
                                            <td class="fundo_td_wap center bold" >ID</td>
                                            <td class="fundo_td_wap center bold" >Nome</td>
                                            <td class="fundo_td_wap center bold" >Tipologia</td>
                                            <td class="fundo_td_wap center bold" >Esfera</td>
                                            <td class="fundo_td_wap center bold" >Nº Termo</td>
                                            <td class="fundo_td_wap center bold" >Fim Vigência</td>
                                        </tr>
                                        <? $obras = pegaTermoVencendoObras(0, 30);?>
                                        <? foreach($obras as $obra):?>
                                            <tr class="fundo_tr_wap">
                                                <td style="background-color:#E95646;" class="fundo_td_wap"><?=$obra['obrid']?></td>
                                                <td style="background-color:#E95646;" class="fundo_td_wap"><?=$obra['obrnome']?></td>
                                                <td style="background-color:#E95646;" class="fundo_td_wap"><?=$obra['tpodsc']?></td>
                                                <td style="background-color:#E95646;" class="fundo_td_wap"><?=$obra['empesfera']?></td>
                                                <td style="background-color:#E95646;" class="fundo_td_wap"><?=$obra['termo']?></td>
                                                <td style="background-color:#E95646;" class="fundo_td_wap"><?=$obra['fim_vigencia']?></td>
                                            </tr>
                                        <? endforeach; ?>
                                    </table>
                                    <? } ?>
                                </div>
                            </div>
                                <div id="divObras" class="divGraf">
                                    <center>
                                        <spam class="linkMais">Obras</spam> <br><br><br>
                                    </center>
                                    <div class="btnNormal" id="btnObras">
                                        Lista de Obras
                                    </div>
                                    <? if($arrVistoria[2]['total'] > 0): ?>
                                        <div class="btnNormal" id="btnObras" style="background: #E95646">
                                            Você possui <?=$arrVistoria[2]['total']?> obras sem atualização há mais de 60 dias.
                                        </div>
                                    <? endif; ?>
                                </div>
                            <div id="divObrasMI" class="divGraf">
                                <center>
                                    <spam class="linkMais">Obras MI</spam> <br><br><br>
                                </center>
                                <div class="btnNormal" id="btnObrasMI">
                                    Lista de Obras MI
                                </div>
                                <div class="btnNormal" id="btnEmissaoMI">
                                    Aguardando Emissão de OS
                                </div>
                            </div>
                            <div id="divAcoes" class="divGraf" >
                                <center>
                                    <spam class="linkMais">Formulários de Fiscalização</spam> <br><br><br>
                                </center>
                                    <?
                                        $tecnologias = pegaEstruturaFormularioMI();
                                    ?>
                                    <? foreach ($tecnologias as $tecnologia): ?>
                                    <div class="btnNormal2" id="btnAcao1">

                                        <div class="slide" style="font-size: 13px; text-align: center; cursor: pointer">
                                        <?=$tecnologia['tminome']?>
                                        </div>
                                        <div style="display: none">
                                        <?if($tecnologia['formulario']):?>
                                            <? foreach ($tecnologia['formulario'] as $item): ?>
                                            <div>
                                                <a href="obras2.php?modulo=principal/inicioLista&acao=A&arquivo=<?=$item['arqid']?>"><img src="/imagens/salvar.png" border="0"> <?=$item['ftinome']?></a>
                                            </div>
                                            <? endforeach; ?>
                                        <?endif?>
                                        </div>

                                    </div>
                                    <? endforeach; ?>

                            </div>

                            <div id="divAjuda" class="divGraf" >
                                <center>
                                    <spam class="linkMais">Ajuda/Manuais</spam> <br><br><br>
                                </center>

                                <div class="btnNormal2" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                        Gestor Público
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/obras_2_0_cartilha_obras_convencionais.pdf" ><img src="/imagens/salvar.png" border="0"> 1° Orientações ao Gestor Público</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/obras_2_0_perguntas_e_respostas_frequentes.pdf" ><img src="/imagens/salvar.png" border="0"> 2° Perguntas e Respostas Frequentes</a></div>
                                    </div>

                                </div>

                                <div class="btnNormal2" id="btnAcao1">

                                    <div style="font-size: 13px; text-align: center;">
                                        Fiscal (Supervisao da Obra)
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/manual_modulo_monitoramento_de_obras_2_0_gestor_fiscal_25_03_2014.pdf" ><img src="/imagens/salvar.png" border="0"> 1° Manual SIMEC Obras 2</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/Simec_2_0_MI_-_Emissao_e_Aceite_de_OS.pdf" ><img src="/imagens/salvar.png" border="0"> 2° Emissão e Aceite de Ordem de Serviço - Obras MI</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/Simec_2_0_MI_-_Insercao_de_Medicao.pdf" ><img src="/imagens/salvar.png" border="0"> 3° Inserção de Medição</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/superacao_restricoes_e_inconformidades.pdf" ><img src="/imagens/salvar.png" border="0"> 4° Orientações Para Superação de Restrições e Inconformidades</a></div>
                                    </div>

                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/REQUISITOS_PARA_CRIACAO_DE_OBRA_VINCULADA.pdf" ><img src="/imagens/salvar.png" border="0"> 5° Requisitos Para Criação de Obras Vinculadas</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/orientacoes_simec_mi.pdf" ><img src="/imagens/salvar.png" border="0"> 6° Orientações Obras MI</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/edicao_cronograma.pdf" ><img src="/imagens/salvar.png" border="0"> 7° Edição de Cronograma</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/edicao_de_prazos_do_cronograma.pdf" ><img src="/imagens/salvar.png" border="0"> 8° Edição de Prazos do Cronograma</a></div>
                                    </div>
                                    <div>
                                        <div><a class="" target="_blank" href="/obras2/tutorial/manual_obra_vinculada.pdf" ><img src="/imagens/salvar.png" border="0"> 9° Criação de Obra Vinculada</a></div>
                                    </div>
                                </div>
                                <div class="btnNormal2" id="btnAcao1">
                                    <div style="font-size: 13px; text-align: center;">
                                        Vídeos tutoriais
                                    </div>
                                    <div>
                                        <div><a class="video_show" href="" video-name="01_obras2_solicitar_acesso"><img src="/imagens/salvar.png" border="0"> 01 - Solicitação de Acesso ao Sistema</a></div>
                                        <div><a class="video_show" href="" video-name="02_obras2_info_pos_cadastro_inicial" ><img src="/imagens/salvar.png" border="0"> 02 - Informações Após o Cadastro</a></div>
                                        <div><a class="video_show" class="video_show" href="" video-name="03_obras2_acesso_inicial"><img src="/imagens/salvar.png" border="0"> 03 - Acesso Inicial</a></div>
                                        <div><a class="video_show" class="video_show" href="" video-name="04_obras2_telainicial_e_busca"><img src="/imagens/salvar.png" border="0"> 04 - Tela Inicial e Busca</a></div>
                                        <div><a class="video_show" href="" video-name="05_obras2_abas"><img src="/imagens/salvar.png" border="0"> 05 - Abas</a></div>
                                        <div><a class="video_show" href="" video-name="06_obras2_dados_obra"><img src="/imagens/salvar.png" border="0"> 06 - Dados da Obra</a></div>
                                        <div><a class="video_show" href="" video-name="07_obras2_licitacao"><img src="/imagens/salvar.png" border="0"> 07 - Licitação</a></div>
                                        <div><a class="video_show" href="" video-name="08_obras2_contratacao"><img src="/imagens/salvar.png" border="0"> 08 - Contratação</a></div>
                                        <div><a class="video_show" href="" video-name="09_obras2_cronograma"><img src="/imagens/salvar.png" border="0"> 09 - Cronograma</a></div>
                                        <div><a class="video_show" href="" video-name="10_obras2_vistoria"><img src="/imagens/salvar.png" border="0"> 10 - Vistoria</a></div>
                                        <div><a class="video_show" href="" video-name="11_obras2_recursos"><img src="/imagens/salvar.png" border="0"> 11 - Recursos</a></div>
                                        <div><a class="video_show" href="" video-name="12_obras2_documentos"><img src="/imagens/salvar.png" border="0"> 12 - Documentos</a></div>
                                        <div><a class="video_show" href="" video-name="13_obras2_galeria"><img src="/imagens/salvar.png" border="0"> 13 - Galeria</a></div>
                                        <div><a class="video_show" href="" video-name="14_obras2_restricoes"><img src="/imagens/salvar.png" border="0"> 14 - Restrições e Inconformidades</a></div>
                                        <div><a class="video_show" href="" video-name="15_obras2_exec_orcamentaria" ><img src="/imagens/salvar.png" border="0"> 15 - Execução Orçamentária</a></div>
                                        <div><a class="video_show" href="" video-name="16_obras2_aditivos"><img src="/imagens/salvar.png" border="0"> 16 - Aditivos</a></div>
                                        <div><a class="video_show" href="" video-name="17_obras2_paralisadas"><img src="/imagens/salvar.png" border="0"> 17 - Paralisadas</a></div>
                                    </div>

                                    </div>
                                </div>

                        <div style="clear: both"></div>
                    </div>

                        </td>
                    </tr>
                </table>
<script type="text/javascript">

    function PopupContatos()
    {
        if(window.windowContatos) window.windowContatos.close();
        window.windowContatos = window.open('', 'Contatos', 'height=648,width=1024,scrollbars=yes');
        window.windowContatos.document.write($('#print-popup1').html());
        return true;
    }

    $(function(){
        $('#manuais').click(function (e) {
            e.preventDefault();
            $('body').scrollTop($('#divAjuda').position().top - 50);
        })

        $('.slide').click(function () {
            $(this).next().slideToggle()
        });

        $('#termos').click(function(e){
            $('body').scrollTop($('#divTermo').position().top - 50);
            e.preventDefault();
        });

        $('#contatos').click(function(e){
            e.preventDefault();

            PopupContatos();
        })

        $('.video_show').click(function (e) {
            e.preventDefault();
            var videoUrl = "/obras2/tutorial/" + $(this).attr('video-name');
            var videoNameOgv = videoUrl + '.ogv';
            var videoNameMp4 = videoUrl + '.mp4';
            var modalTitle = $(this).text();

            $('#dialog_tutorial').find('h3').text(modalTitle);
            $('#dialog_tutorial').find('source:eq(0)').attr('src', videoNameMp4);
            $('#dialog_tutorial').find('source:eq(1)').attr('src', videoNameOgv);
            $('video').load();

            jQuery("#dialog_tutorial").dialog({
                modal: true,
                width: 880,
                close: function(ev, ui) { $('video').load(); },
                buttons: {
                    OK: function() {
                        jQuery( this ).dialog( "close" );
                        $('video').load();
                    }
                }
            });
        })
    })
</script>
<div style="display: none" id="dialog_tutorial" title="Tutoriais">
    <h3></h3>
    <video width="854" height="480" controls>
        <source src="/obras2/tutorial/01_obras2_solicitar_acesso.mp4" type="video/mp4">
        <source src="/obras2/tutorial/01_obras2_solicitar_acesso.ogv" type="video/ogg">
        O seu navegador não suporta a tag vídeo. Utilize as versões mais recentes
        do Mozilla Firefox, Google Chrome, Safari ou Internet Explorer. Para uma lista
        de navegadores e suas compatibilidades acesse <a href="http://www.w3schools.com/html/html5_video.asp" target="_blank">
            http://www.w3schools.com/html/html5_video.asp</a>
    </video>
</div>

<div id="print-popup1" style="display: none">
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
    <?php echo monta_cabecalho_relatorio(100); ?>
    <br />

    <table class="tabela" align="center" cellpadding="3" cellspacing="1" id="tabela_contatos">
        <tr>
            <td rowspan="2" class="SubTituloCentro">Assunto</td>
            <td colspan="2" class="SubTituloCentro">Contato</td>
        </tr>
        <tr>
            <td class="SubTituloCentro">E-mail</td>
            <td class="SubTituloCentro">Telefone</td>
        </tr>
        <tr>
            <td>Alteração no  projeto</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4663</td>
        </tr>
        <tr>
            <td>Uso de Saldo</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4638</td>
        </tr>
        <tr>
            <td>Troca de Terreno</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4663/4621</td>
        </tr>
        <tr>
            <td>Troca de metodologia construtiva</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4663/4621</td>
        </tr>
        <tr>
            <td>Senha: acesso, alteração, bloqueio</td>
            <td>senha.monitoramento@fnde.gov.br; atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5201</td>
        </tr>
        <tr>
            <td>Obras MI - duvidas sobre SIGARP</td>
            <td>saladesituacaoMI@fnde.gov.br</td>
            <td>2022-4913</td>
        </tr>
        <tr>
            <td>Obras MI - duvidas sobre contratação</td>
            <td>saladesituacaoMI@fnde.gov.</td>
            <td>2022-4913</td>
        </tr>
        <tr>
            <td>Obras MI - duvidas sobre</td>
            <td>saladesituacaoMI@fnde.gov.br</td>
            <td>2022-4913</td>
        </tr>
        <tr>
            <td>Desembolso de parcelas</td>
            <td>monitoramento.obras@fnde.gov.br</td>
            <td>-</td>
        </tr>
        <tr>
            <td>Restrições e inconformidades</td>
            <td>atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5201</td>
        </tr>
        <tr>
            <td>Paralisação de obras</td>
            <td>atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5202</td>
        </tr>
        <tr>
            <td>Preenchimento SIMEC-Obras 2.0</td>
            <td>atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5203</td>
        </tr>
        <tr>
            <td>Preenchimento SIMEC-PAR</td>
            <td>par@fnde.gov.br</td>
            <td>2022-5935/5915/5922/5854/5978</td>
        </tr>
        <tr>
            <td>Prorrogação de Termo de Compromisso/Convênios</td>
            <td>grupo.prorrogação@fnde.gov.br</td>
            <td>2022-4905/5046/4822</td>
        </tr>
        <tr>
            <td>Mobiliário / Equipamentos</td>
            <td>par@fnde.gov.br</td>
            <td>2022-5935/5915/5922/5854/5978</td>
        </tr>
        <tr>
            <td>Execução de obras - dúvidas de engenharia</td>
            <td>ver aba documentos - e-mail do técnico que acompanha o Estado</td>
            <td>
                -
            </td>
        </tr>
    </table>

</div>



<?php

function getSqlTermo()
{
    $orWhere = array();
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

        $arrObr = array(
            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
            PFLCOD_EMPRESA_VISTORIADORA_GESTOR
        );

        $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = Array(PFLCOD_ADMINISTRADOR,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC);

        $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

        $arPflcod = array();
        $orWhere = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
        }

        if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
            $usuarioResp = new UsuarioResponsabilidade();
            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
        }

    }
    $sql = "
    
                SELECT 
                o.obrid,
                o.obrnome,
                tpo.tpodsc,
                CASE WHEN e.empesfera = 'M' THEN 'Municipal' ELSE 'Estadual' END empesfera,
                t.termo,
                TO_CHAR(t.fim_vigencia, 'dd/mm/YYYY') as fim_vigencia
            FROM obras2.obras o
            LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
            LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
            LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
            LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
            JOIN (SELECT obrid, numero_termo as termo, dt_fim_vigencia_termo as fim_vigencia FROM obras2.vm_vigencia_obra) as t ON t.obrid = o.obrid
            WHERE 
                o.obridpai IS NULL 
                AND o.obrstatus = 'A'
	            " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "

    ";
    return $sql;
}
function pegaTermoVigenteObras(){
    global $db;
    $sql = getSqlTermo();
    $sql .= "

        AND t.fim_vigencia >= NOW()

        ";

    $result = $db->carregar($sql, null, 86400);
    return ($result) ? $result : array();
}
function pegaTermoVencidosObras(){
    global $db;
    $sql = getSqlTermo();
    $sql .= " AND t.fim_vigencia < NOW ()";

    $result = $db->carregar($sql, null, 86400);
    return ($result) ? $result : array();
}
function pegaTermoVencendoObras($dias_inicio, $dias_fim){
    global $db;
    $sql = getSqlTermo();
    $sql .= "
        AND t.fim_vigencia <= NOW() + '$dias_fim days'::interval
        AND t.fim_vigencia >= NOW() + '$dias_inicio days'::interval
        ";

    $result = $db->carregar($sql, null, 86400);
    return ($result) ? $result : array();
}

?>





    <style>

        .box.box-small {
            width: 20%;
        }

        .box.box-medium {
            width: 46%;
        }

        .box.box-large {
            width: 94.5%;
        }

        .box {
            FONT: 11pt Arial;
            -moz-border-radius: 20px;
            border-radius: 20px;
            padding: 10px;
            float: left;
        }

        .box .box-header {
            text-align: center;
            color: #FFFFFF;
            height: 30px;
            font-weight: bold;
            font-size: 14px;
        }

        .box .box-header .box-header-options{
            cursor: pointer;
            float: right;
            margin: 0 8px 0 0;
        }

        .box .box-body {
            text-align: center;
            background-color: #FFFFFF;
            border-radius: 20px;
            border-radius: 5px;
            padding: 4px;
            text-align: center;
            min-height: 130px;
        }
        .box .box-body .box-body-title {
            font-weight: bold;
            font-size: 14px;
        }
        .box .box-body .box-body-subtitle {
            font-size: 11px;
        }

        .box.box-red {
            background-color: #EE3B3B;
        }

        .box p{
            margin: 0;
            padding: 0;
        }
        .info .ui-widget-header {
            background:#cc3600;
            border: 1px solid #A22B00;
        }
    </style>

    <div id="dialog_atencao" title="<img src='/imagens/exclamacao_checklist.png'> Atenção" >

        <div class="" style="font-size: 14px; color: #002A6C; font-weight: bold;">
            <div>
                <p>Em razão das novas regras de transferência de recursos aos municípios, estados e Distrito Federal, o FNDE, com escopo de aprimorar o processo de integração entre os entes federados e esta Autarquia, criou ferramenta no módulo Obras 2.0 (SIMEC), na qual ocorrerá a solicitação do desembolso. Segue abaixo documentos:</p>
                <p></p>
                <p><a style="font-size:14x; color:#FF0101;" href="/obras2/tutorial/oficio_circular_01_2015_CGIMP_DIGAP_FNDE_MEC.pdf"><img src="/imagens/salvar.png" border="0"> Ofício Circular 01/2015 CGIMP/DIGAP/FNDE/MEC - Clique AQUI</a></p>
                <p><a style="font-size:14px; color:#FF0101;" href="/obras2/tutorial/manual_para_solicitar_desembolso.pdf"><img src="/imagens/salvar.png" border="0"> Manual de Solicitação de Desembolso - Clique AQUI</a></p>
            </div>
        </div>



    <div style="clear: both"></div>
    <br />
    </div>
    <?
    $cumprimentoObjeto = new CumprimentoObjeto();
    $saida = $cumprimentoObjeto->alertaInicio();
    if($saida):
    ?>
    <div id="dialog_atencao_cumprimento_obj" title="<img src='/imagens/exclamacao_checklist.png'> Atenção" >
        <div class="" style="font-size: 14px; color: #002A6C; font-weight: bold;">
            <div>
                <p>
                    <?php
                    if(possui_perfil(array(PFLCOD_GESTOR_UNIDADE))): ?>
                    Sr (a) Gestor
                    <?php
                    elseif(possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE))): ?>
                    Sr (a) Fiscal
                    <?php
                    endif; ?>
                </p>
                <p>
                    Informamos que está disponível para a(s) obra(s) abaixo a aba Cumprimento de Objeto.<br>
                    Essa aba faz parte da fase de Prestação de Contas do instrumento pactuado, e deve ser preenchida no prazo de 45 dias, independentemente da abertura ou não do SIGPC-Contas On Line.<br>
                    Em caso de dúvidas sobre o preenchimento, contatar através do e-mail <b>cumprimentodoobjeto@fnde.gov.br</b>.
                </p><br>
                <form method="post" action="">
                    <input type="hidden" name="requisicao" value="aviso_cumprimento">
                    <table class="tabela">
                        <tr>
                            <td class="subTituloEsquerda">ID da Obra</td>
                            <td class="subTituloEsquerda">Nome</td>
                        </tr>
                        <?
                        foreach($saida as $dado): ?>
                        <tr>
                            <td><input type="hidden" name="coid[]" value="<?=$dado['coid']?>"><?=$dado['obrid']?></td>
                            <td><?=$dado['obrnome']?></td>
                        </tr>
                        <?
                        endforeach; ?>
                    </table>
                    <?php
                    if(possui_perfil(array(PFLCOD_GESTOR_UNIDADE))): ?>
                    <button type="submit" >Li e Entendi</button>
                    <?php
                    elseif(possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE))): ?>
                    <strong>Favor, informar Gestor da Unidade.</strong>
                    <?php
                    endif; ?>
                </form>
            </div>
        </div>
        <div style="clear: both"></div>
        <br>
    </div>
    <?
    endif;
    ?>

    <script type="text/javascript">
        jQuery(function(){
            jQuery("#dialog_atencao").dialog({
                modal: true,
                width: 540,
                dialogClass: 'info',
            });
            if(jQuery('#dialog_atencao_cumprimento_obj')) {
                jQuery("#dialog_atencao_cumprimento_obj").dialog({
                    modal: true,
                    width: 540,
                    dialogClass: 'info',
                });
            }
        });
        jQuery('#dialog_atencao').parent().find('.ui-dialog-titlebar').css('background', 'red')
    </script>