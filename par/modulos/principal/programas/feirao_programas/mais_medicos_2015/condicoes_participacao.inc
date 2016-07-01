<?PHP
    include_once '_funcoes_maismedicos_2015.php';

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    #É USADO APENAS QUANDO ESSA PAGINA É CHAMADA PELO RELATÓRIO MAIS MÉDICO.
    if( $_REQUEST['rel_muncod'] != '' && $_REQUEST['rel_prgid'] != '' ){
        $_SESSION['par']['prgid']   = $_REQUEST['rel_prgid'];
        $_SESSION['par']['muncod']  = $_REQUEST['rel_muncod'];

        if( $_SESSION['par']['muncod'] != '' ){
            $inuid = $db->pegaUm("SELECT inuid FROM par.instrumentounidade WHERE muncod = '{$_SESSION['par']['muncod']}'");
            $_SESSION['par']['adpid'] = $db->pegaUm("select max(adpid) from par.pfadesaoprograma where pfaid = 27 and inuid = {$inuid}");
        }
    }

    if( !$_SESSION['par']['prgid'] || $_SESSION['par']['muncod'] == '' ){
        $db->sucesso( 'principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';
    monta_titulo("Mais Médicos - Edital nº 1/2015", $titulo_modulo.' <br> '.recuperaMunicipioEstado());
    echo '<br>';

    $rqmid = verificarRQMID();
    if( $rqmid != '' ){
        $_SESSION['par']['rqmid'] = $rqmid;
        $resp = verificaInformacaoMunicipio( $rqmid );
    }

    if( $resp['rqmaceitetermoresidencia'] == 't'){
        $habilita = 'disabled="disabled"';
    }else{
        $habilita = 'onclick="continuaAdesao()"';
    }

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/condicoes_participacao&acao=A');

    $populacao  = verificarPopulacao();
    $capital    = verificarCapitalEstado();
    $oferta     = possuiOfertaCurso();
    $infoMun    = informacaoMunicipios_2015();

    $aderiu = verificaAdesaoMaisMedicoNovo();

    $habilitado = verificaDataFechamento();

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">

    function continuaAdesao(){
        $('#continua').attr('disabled', true);
        $('#requisicao').val('continuaAdesao');
        $('#formulario').submit();
    }
</script>


<form action="" method="POST" id="formulario" name="formulario">

    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <input type="hidden" id="muncod" name="muncod" value="<?=$_SESSION['par']['muncod'];?>"/>
    <input type="hidden" id="prgid" name="prgid" value="<?=$_SESSION['par']['prgid'];?>"/>
    <input type="hidden" id="rqmquestao03" name="rqmquestao03" value="<?=$infoMun['ifmpopulacao'];?>"/>
    <input type="hidden" id="rqmquestao04" name="rqmquestao04" value="<?=$capital;?>"/>
    <input type="hidden" id="rqmquestao05" name="rqmquestao05" value="<?=$oferta;?>"/>
    <input type="hidden" id="rqmquestao06" name="rqmquestao06" value="<?=$infoMun['ifmhospleito'];?>"/>

    <center>
        <table bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td width="90%" align="center">
                    <br>
                    <fieldset style="width: 70%;">
                        <legend style="font-family: sans-serif; font-size: 14px;"> CONDIÇÕES DE PARTICIPAÇÃO </legend>
                            <p style="text-indent: 5%; text-align: justify; font-family: sans-serif; font-size: 14px; line-height:200%;">

                                <? if($aderiu == 'N'){ ?>
                                        <span style="font-weight: bold;">
                                            Observação: "Este município não aceitou a adesão ao Programa Mais Médico - Edital nº 1/2015. Dessa forma não é possivél dar continuidade ao porcesso".
                                        </span>
                                        <br><br>
                                <? } ?>
                            </p>
                            <img width=70px" src="../imagens/brasao.gif">
                            <br><br>

                            <p style="font-size: 14px; font-weight: bold;">
                                MINISTÉRIO DA EDUCAÇÃO
                            </p>
                            <p style="font-size: 13px; font-weight: bold; text-align: left;">
                                COMUNICADO
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                Os municípios chamados a aderir a este Edital foram pré-selecionados de acordo com os critérios de relevância e necessidade social da oferta de curso de medicina, ouvido o Ministério da Saúde - MS.
<!--                            </p>
                            <p style="font-size: 12px; text-align: justify;">-->
                                Em obediência ao art. 1º, inciso I, da Lei nº 12.871, de 2013, e visando corrigir assimetrias regionais concernentes à proporção de médicos por habitantes, o perfil dos municípios pré-selecionados prevê o atendimento cumulativo aos seguintes critérios:
                            </p>
<!--                            <p style="font-size: 12px; text-align: justify;">
                                A resolução em questão tem o objetivo de transferência direta de recursos financeiros aos estados, municípios e Distrito Federal para a manutenção e desenvolvimento de <b>novas turmas de Educação de Jovens e Adultos</b> oferecidas pelas redes públicas de ensino, <b>na modalidade presencial</b>, cujas matrículas ainda não tenham sido contempladas com recursos do Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação (FUNDEB).
                            </p>-->
                            <p style="font-size: 12px; font-weight: bold; text-align: justify;">
                                I - não se constituem como capital de Estado;
                            </p>
                            <p style="font-size: 13px; font-weight: bold; text-align: left;">
                                II - não possuem oferta de curso de medicina em seu território ou na região de saúde;
                            </p>
                            <p style="font-size: 12px; font-weight: bold; text-align: justify;">
                                III - possuem mais de 50.000 habitantes, de acordo com os dados do Instituto Brasileiro de Geografia e Estatística, estimativas do IBGE 2014;
                            </p>
                            <p style="font-size: 12px; font-weight: bold; text-align: justify;">
                                IV - estão localizados em Unidades da Federação - UFs que, conforme tabela constante do Anexo I:
                            </p>
                            <p style="font-size: 13px; text-align: justify;">
                                a) possuem relação vaga em curso de medicina por dez mil habitantes inferior a 1,34, considerando os cursos de medicina previstos no plano de expansão pública do Programa Mais Médicos ou de municípios constantes dos Anexos I e II da Portaria SERES nº 543, de 4 de setembro de 2014 e de acordo com dados da Secretaria de Gestão do Trabalho e Educação em Saúde - SGTES, do MS.
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                b) possuem relação médicos por mil habitantes inferior a 2,7, de acordo com dados da Secretaria de Gestão do Trabalho e Educação em Saúde - SGTES, do MS.
                            </p>
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                V - estão distantes, pelo menos, setenta e cinco quilômetros de local de curso de medicina pré-existente; de cursos de medicina previstos no plano de expansão pública do Programa Mais Médicos ou de municípios constantes dos Anexos I e II da Portaria SERES nº 543, de 4 de setembro de 2014.
                            </p>
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                VI - estão localizados em região de saúde que apresenta estrutura de equipamentos públicos, cenários de atenção na rede e programas de saúde adequados para comportar a oferta de curso de graduação em medicina, conforme os critérios previstos no art. 3º deste Edital.
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                Na verificação in <b>loco</b>, será observado se o município pré-selecionado atende aos seguintes critérios:
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                a) número de leitos do SUS disponíveis por aluno, em quantidade igual ou maior a cinco, tendo em vista a abertura de turmas com, no mínimo, cinquenta alunos;
                                <br>
                                b) número de alunos por Equipe de Atenção Básica - EAB menor ou igual a três, tendo em vista a abertura de turmas, com, no mínimo, cinquenta alunos;
                                <br>
                                c) existência de leitos de urgência e emergência ou Pronto Socorro;
                                <br>
                                d) existência de, pelo menos, três Programas de Residência Médica nas especialidades prioritárias, preferencialmente em Medicina Geral de Família e Comunidade;
                                <br>
                                e) adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ, do MS;
                                <br>
                                f) existência de Centro de Atenção Psicossocial - CAPS; e
                                <br>
                                g) existência de hospital de ensino ou unidade hospitalar com mais de 80 (oitenta) leitos, com potencial para hospital de ensino, conforme legislação de regência.
                            </p>
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                A adesão expressa ao edital e a assunção dos compromissos estabelecidos no Termo são indispensáveis para a seleção do município para implementação do curso de graduação em Medicina no âmbito do Programa Mais Médicos.
                            </p>
                    </fieldset>
                    <br>
                </td>
            </tr>
            <tr>
                <td align="center">
                    <div style="float: center; width: 70%; ">
                        <?PHP
                            if( $habilitado == 'S' ){
                        ?>
                            <input type="button" name="continua" id="continua" value="Continuar Adesão" <?=$habilita;?>>
                        <?PHP
                            }else{
                        ?>
                            <input type="button" name="continua_dis" id="continua_dis" value="Continuar Adesão" disabled="disabled">
                        <?PHP
                            }
                        ?>
                        <input type="button" name="cancelar" id="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
                    </div>
                </td>
            </tr>
        </table>
    </center>
</form>



