<?PHP
    #ATRIBUI O VALOR NAS VARIAVEIS DE SESSAO
    if( $_REQUEST['muncod'] != '' ){
        iniciaVariaveisSessao( $_REQUEST['muncod'], 'M' );

        if( $_SESSION['maismedicomec']['muncod'] != '' ){
            $etqid = cadastraEntidadeQuestionarioMunicipio( $_SESSION['maismedicomec']['muncod'] );

            if( $etqid != '' ){
                iniciaVariaveisSessao( $etqid, 'E' );
            }
        }

    }else{
        if( $_SESSION['maismedicomec']['muncod'] == '' ){
            $db->sucesso('principal/inicio_direcionamento', '&acao=A', 'Não foi possivél acessar o sistema, Ocorreu um problema interno ou houve perca de sessão. Tente novamente mais tarde!');
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação',"Serviços de saúde do município, {$_SESSION['maismedicomec']['mundescricao']}. <br> Candidato a sediar curso de medicina em IES privada." );

    if( $_SESSION['maismedicomec']['relatorio'] == '' ){
        $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
        $url            = 'maismedicomec.php?modulo=relatorio/relatorio_resposta_questionario&acao=A';
        $parametros     = '';

        $db->cria_aba($abacod_tela, $url, $parametros);
    }
    
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>


<script type="text/javascript">

    function voltarListagemMunicipio(){
        window.location.href ='maismedicomec.php?modulo=relatorio/relatorio_lista_grid_municipio&acao=A';
    }
    
</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="PDF" name="PDF" value=""/>
    <input type="hidden" id="muncod" name="muncod" value="<?=$_SESSION['maismedicomec']['muncod'];?>"/>

    <div id = "div_rolagem" class="notprint">
        <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="2" class="subtituloCentro">
                    <input type="button" name="bt_imprimir" value="Imprimir" onclick="javascript: window.print();">
                    <?PHP
                        if( $_SESSION['maismedicomec']['relatorio'] == 'S' ){
                    ?>
                            <input type="button" name="bt_voltar" value="Voltar" onclick="voltarListagemMunicipio();">
                    <?
                        }
                    ?>
                </td>
            </tr>
        </table>
    </div>
    
    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela">
        <tr>
            <td>
                <?PHP
                    $arrPerguntas = array(1,2,3,4,5);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );
                ?>
                <tr>
                    <td class ="SubTituloCentro" colspan="2" style="height: 35px;"> 1. Avaliação documental. Relatar os documentos avaliados e a sua pertinência/importância.</td>
                </t>
                <?PHP
                    #perguntas inativadas: 7,8,9
                    $arrPerguntas = array(6,10,11,12,13,14,15);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );
               ?>
                <tr>
                   <td class ="SubTituloCentro" colspan="4"> 2. Descrição e comentários sobre aspectos gerais da proposta.</td>
                </t>
                <?PHP
                    #PERGUNTA 16 É A PERGUNTA AGRUPADORA DAS 217,218 E 17,18,19,20,21,22,23,24,25,26,27,28,29
                    $arrPerguntas = array(16);
                    echo montaPerguntasQuestionarioImpressao( '', 'Aspectos Gerais', '', $arrPerguntas );

                    $arrPerguntas = array(217,218);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );

                    $arrPerguntas = array(17,18,19,20,21,22,23,24,25,26,27,28,29);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );

                    #BUSCA TODAS UNIDADES DE SAÚDE RELACIONADAS COM O MUNICÍPIO DO TIPO 1 - HOSPITAL
                    $sql = "
                        SELECT  ussid,
                                ussdsc
                        FROM maismedicomec.unidservicosaude
                        WHERE tusid = 1 AND ussstatus = 'A' AND etqid = {$_SESSION['maismedicomec']['etqid']} 
                        ORDER BY 1
                    ";
                    $dados = $db->carregar($sql);
                    if( $dados != '' ){
                        foreach( $dados as $ussid ){
                ?>
                            <tr>
                                <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde (Hospital) - <?=$ussid['ussdsc'];?> </td>
                            </t>

                <?PHP
                            #PERGUNTA 216 É A PERGUNTA AGRUPADORA DAS 30,35,36
                            $arrPerguntas = array(216);
                            echo montaPerguntasQuestionarioImpressao( 'A - 1)', 'Unidades de Internação', '', $arrPerguntas, $ussid['ussid'] );

                            #PERGUNTA 216 É A PERGUNTA AGRUPADORA DAS 30,35,36
                            $arrPerguntas = array(30,35,36);
                            echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(38,39,40,41);
                            echo montaPerguntasQuestionarioImpressao( 'A - 2)', 'Unidades Cirurgicas', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(42,43,44,45,46,47,48,49);
                            echo montaPerguntasQuestionarioImpressao( 'A - 3)', 'Recursos Propedêuticos', '', $arrPerguntas, $ussid['ussid'] );

                            #perguntas inativadas: 52,53,54,55
                            $arrPerguntas = array(50,51,56,57);
                            echo montaPerguntasQuestionarioImpressao( 'A - 4)', 'Unidade de Urgência/Emergência', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(58,59);
                            echo montaPerguntasQuestionarioImpressao( 'A - 5)', 'Recursos de Hemoterapia ', '', $arrPerguntas, $ussid['ussid'] );

                            #perguntas inativadas: 62,64,63,65,66
                            $arrPerguntas = array(60,67,68,69);
                            echo montaPerguntasQuestionarioImpressao( 'A - 6)', 'Residência médica', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(70,71);
                            echo montaPerguntasQuestionarioImpressao( 'A - 7)', 'Ambiente de Prática para alunos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(72);
                            echo montaPerguntasQuestionarioImpressao( 'A - 8)', 'Papel do Hospital no serviço de saúde do município', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(73);
                            echo montaPerguntasQuestionarioImpressao( 'A - 9)', 'Avaliação do gestor municipal', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(74);
                            echo montaPerguntasQuestionarioImpressao( 'A - 10)', 'Dados Estatísticos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(215);
                            echo montaPerguntasQuestionarioImpressao( 'A - 11)', 'Comentários Gerais sobre o Hospital', '', $arrPerguntas, $ussid['ussid'] );
                        }
                    }
                ?>

                <tr>
                    <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde - Conjunto Hospitalar. </td>
                </t>

                <?PHP
                    $arrPerguntas = array(75,76,77,78,79);
                    echo montaPerguntasQuestionarioImpressao( 'A - 11)', 'Conjunto Hospitalar do Município', '', $arrPerguntas );

                    #BUSCA TODAS UNIDADES DE SAÚDE RELACIONADAS COM O MUNICÍPIO DO TIPO 2 - UBS
                    $sql = "
                        SELECT  ussid,
                                ussdsc
                        FROM maismedicomec.unidservicosaude
                        WHERE tusid = 2 AND ussstatus = 'A' AND etqid = {$_SESSION['maismedicomec']['etqid']}
                        ORDER BY 1
                    ";
                    $dados = $db->carregar($sql);
                    if( $dados != '' ){
                        foreach( $dados as $ussid ){
                ?>
                            <tr>
                                <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde (UBS) - <?=$ussid['ussdsc'];?> </td>
                            </t>
                <?PHP
                            $arrPerguntas = array(80,81);
                            echo montaPerguntasQuestionarioImpressao( 'B - 1)', 'Instalações Físicas', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(82,83);
                            echo montaPerguntasQuestionarioImpressao( 'B - 2)', 'Consultórios', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(84,85);
                            echo montaPerguntasQuestionarioImpressao( 'B - 3)', 'Sala de Curativos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(86,87);
                            echo montaPerguntasQuestionarioImpressao( 'B - 4)', 'Sala de Vacinação', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(88,89,90);
                            echo montaPerguntasQuestionarioImpressao( 'B - 5)', 'Farmácia Básica', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(91,92);
                            echo montaPerguntasQuestionarioImpressao( 'B - 6)', 'Sala de Reuniões', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(93,94);
                            echo montaPerguntasQuestionarioImpressao( 'B - 7)', 'Banheiros', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(95,96);
                            echo montaPerguntasQuestionarioImpressao( 'B - 8)', 'Internet', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(97,98);
                            echo montaPerguntasQuestionarioImpressao( 'B - 9)', 'Telesaúde', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(99,100);
                            echo montaPerguntasQuestionarioImpressao( 'B - 10)', 'Materiais e Instrumentos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(104,105,106,107,108);
                            echo montaPerguntasQuestionarioImpressao( 'B - 11)', 'ESF', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(112);
                            echo montaPerguntasQuestionarioImpressao( 'B - 12)', 'População Vinculada', '', $arrPerguntas, $ussid['ussid'] );

                            #perguntas inativadas: 113,115,116,117,119,121,122,123
                            $arrPerguntas = array(113, 119);
                            echo montaPerguntasQuestionarioImpressao( 'B - 13)', 'Normas de funcionamento', '', $arrPerguntas, $ussid['ussid'] );

                            #perguntas inativadas: 126,128
                            $arrPerguntas = array(125,127,129,130,131,132,133,134,135,136);
                            echo montaPerguntasQuestionarioImpressao( 'B - 14)', 'Atividades desenvolvidas', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(137,140,141,142,143);
                            echo montaPerguntasQuestionarioImpressao( 'B - 15)', 'Profissionais em Atividades', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(138,139);
                            echo montaPerguntasQuestionarioImpressao( 'B - 16)', 'Sistema de referência', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(144,145);
                            echo montaPerguntasQuestionarioImpressao( 'B - 17)', 'Ações de Educação Permanentes', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(146);
                            echo montaPerguntasQuestionarioImpressao( 'B - 18)', 'Dados Estatísticos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(147,148);
                            echo montaPerguntasQuestionarioImpressao( 'B - 19)', 'Alunos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(149,150);
                            echo montaPerguntasQuestionarioImpressao( 'B - 20)', 'NASF', '', $arrPerguntas, $ussid['ussid'] );
                        }
                    }
                ?>

                <tr>
                   <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde - Equipe de Atenção Básica. </td>
                </t>

                <?PHP
                    $arrPerguntas = array(151,152);
                    echo montaPerguntasQuestionarioImpressao( 'B - 21)', ' Equipes de Unidades Básicas', '', $arrPerguntas);

                    #BUSCA TODAS UNIDADES DE SAÚDE RELACIONADAS COM O MUNICÍPIO DO TIPO 3 - UPA
                    $sql = "
                        SELECT  ussid,
                                ussdsc
                        FROM maismedicomec.unidservicosaude
                        WHERE tusid = 3 AND ussstatus = 'A' AND etqid = {$_SESSION['maismedicomec']['etqid']}
                        ORDER BY 1
                    ";
                    $dados = $db->carregar($sql);
                    if( $dados != '' ){
                        $controle = 0;
                        foreach( $dados as $ussid ){
                ?>
                            <tr>
                                <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde (UPA) - <?=$ussid['ussdsc'];?> </td>
                            </t>
                <?PHP
                            if( $controle == 0 ){
                                $arrPerguntas = array(202,203);
                                echo montaPerguntasQuestionarioImpressao( '', 'Existência de UPA', '', $arrPerguntas );
                            }
                        
                            $arrPerguntas = array(153);
                            echo montaPerguntasQuestionarioImpressao( 'C - 1)', 'Instalações Físicas', 'Adequação, funcionalidade etc', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(154,155);
                            echo montaPerguntasQuestionarioImpressao( 'C - 2)', 'Normas de funcionamento/atendimento', '', $arrPerguntas, $ussid['ussid'] );

                            #perguntas inativadas: 157,158
                            $arrPerguntas = array(156);
                            echo montaPerguntasQuestionarioImpressao( 'C - 3)', 'Procedimentos realizados', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(159);
                            echo montaPerguntasQuestionarioImpressao( 'C - 4)', 'Leitos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(160);
                            echo montaPerguntasQuestionarioImpressao( 'C - 5)', 'Acesso a Internet', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(162,163);
                            echo montaPerguntasQuestionarioImpressao( 'C - 6)', 'Telesaúde', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(164,165);
                            echo montaPerguntasQuestionarioImpressao( 'C - 7)', 'Alunos', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(166);
                            echo montaPerguntasQuestionarioImpressao( 'C - 8)', 'Dados Estatísticos', '', $arrPerguntas, $ussid['ussid'] );
                            
                            $controle = $controle + 1;
                        }
                    }

                    #BUSCA TODAS UNIDADES DE SAÚDE RELACIONADAS COM O MUNICÍPIO DO TIPO 4 - CAPS
                    $sql = "
                        SELECT  ussid,
                                ussdsc
                        FROM maismedicomec.unidservicosaude
                        WHERE tusid = 4 AND ussstatus = 'A' AND etqid = {$_SESSION['maismedicomec']['etqid']}
                        ORDER BY 1
                    ";
                    $dados = $db->carregar($sql);
                    if( $dados != '' ){
                        $controle = 0;
                        foreach( $dados as $ussid ){
                ?>
                            <tr>
                                <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde (CAPS) - <?=$ussid['ussdsc'];?> </td>
                            </t>
                <?PHP       
                            if( $controle == 0 ){
                                $arrPerguntas = array(208,209);
                                echo montaPerguntasQuestionarioImpressao( '', 'Existência de CAPS', '', $arrPerguntas );
                            }
                            $arrPerguntas = array(170,171,172,173,174,175,176,177);
                            echo montaPerguntasQuestionarioImpressao( 'D - 1)', 'CAPS', '', $arrPerguntas, $ussid['ussid'] );
                            
                            $controle = $controle + 1;
                        }
                    }

                    #BUSCA TODAS UNIDADES DE SAÚDE RELACIONADAS COM O MUNICÍPIO DO TIPO 5 - AMBULATORIO
                    $sql = "
                        SELECT  ussid,
                                ussdsc
                        FROM maismedicomec.unidservicosaude
                        WHERE tusid = 5 AND ussstatus = 'A' AND etqid = {$_SESSION['maismedicomec']['etqid']}
                        ORDER BY 1
                    ";
                    $dados = $db->carregar($sql);
                    if( $dados != '' ){
                        $controle = 0;
                        foreach( $dados as $ussid ){
                ?>
                            <tr>
                                <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde (Ambulatório) - <?=$ussid['ussdsc'];?> </td>
                            </t>
                <?PHP
                            if( $controle == 0 ){
                                $arrPerguntas = array(206,207);
                                echo montaPerguntasQuestionarioImpressao( '', 'Existência de Ambulatórios de Especialidades ou Policlínicas', '', $arrPerguntas );
                            }
                            $arrPerguntas = array(180,181);
                            echo montaPerguntasQuestionarioImpressao( 'E - 1)', 'Consultórios', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(182,183);
                            echo montaPerguntasQuestionarioImpressao( 'E - 2)', 'Salas de Reuniões', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(184,185);
                            echo montaPerguntasQuestionarioImpressao( 'E - 3)', 'Acesso a Internet', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(186,187);
                            echo montaPerguntasQuestionarioImpressao( 'E - 3)', 'Acesso à Telesaúde', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(188);
                            echo montaPerguntasQuestionarioImpressao( 'E - 5)', 'Especialidades Atendidas', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(189,190,191);
                            echo montaPerguntasQuestionarioImpressao( 'E - 6)', 'Profissionais em Atividade', '', $arrPerguntas, $ussid['ussid'] );

                            $arrPerguntas = array(192,194);
                            echo montaPerguntasQuestionarioImpressao( 'E - 7)', 'Alunos', '', $arrPerguntas,$ussid['ussid']);

                            $arrPerguntas = array(195);
                            echo montaPerguntasQuestionarioImpressao( 'E - 8)', 'Dados Estatísticos', '', $arrPerguntas, $ussid['ussid']);
                            
                            $controle = $controle + 1;
                        }
                    }
                ?>

                <tr>
                    <td class ="SubTituloCentro" colspan="3"> 4. Outros Serviços de Saúde.</td>
                </t>

                <?PHP
                    $arrPerguntas = array(196);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );
                ?>

                <tr>
                    <td class ="SubTituloCentro" colspan="3"> 5. Rede de Ateção à saúde.</td>
                </t>
                <?PHP
                    $arrPerguntas = array(197);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );
                ?>

                <tr>
                    <td class ="SubTituloCentro" colspan="3"> 6. Conclusão.</td>
                </t>
                <?PHP
                    $arrPerguntas = array(220);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );

                    $arrPerguntas = array(198);
                    echo montaPerguntasQuestionarioImpressao( '', '', '', $arrPerguntas );
                ?>
            </td>
        </tr>
    </table>
    <div id = "div_rolagem" class="notprint">
        <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="2" class="subtituloCentro">
                    <input type="button" name="bt_imprimir" value="Imprimir" onclick="javascript: window.print();">
                    <?PHP
                        if( $_SESSION['maismedicomec']['relatorio'] == 'S' ){
                    ?>
                            <input type="button" name="bt_voltar" value="Voltar" onclick="voltarListagemMunicipio();">
                    <?
                        }
                    ?>
                </td>
            </tr>
        </table>
    </div
