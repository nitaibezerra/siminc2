<?PHP
    include_once '_funcoes_eja_pronatec.php';

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if( !$_SESSION['par']['prgid'] ){
        $db->sucesso( 'principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';

    #PEGA, SELECIONA O MUNICÍPIO OU ESTADO SELECIONADO OU LOGADO PELO USUÁRIO. É USADO PARA CRIAR O TITULO!.
    $sql = " SELECT prgdsc FROM par.programa WHERE prgid = {$_SESSION['par']['prgid']} ";
    $nomePrograma = $db->pegaUm($sql);

    if($_SESSION['par']['muncod']){
        $sql = "SELECT mundescricao, estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
        $rsMunicipio = $db->pegaLinha($sql);
        $descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];

        $munBloqueado = bucarMunicipioBloqueado();

    }else{
        $sql = "SELECT estdescricao, estuf FROM territorios.estado WHERE estuf = '{$_SESSION['par']['estuf']}'";
        $rsEstado = $db->pegaLinha($sql);
        $descricao = $rsEstado['estdescricao'].' - '.$rsEstado['estuf'];
    }

    monta_titulo($nomePrograma, $descricao);
    echo '<br/>';

    $aderiu = verificaAdesaoEJA();

    #VERIFICA SE O PROGRAMA ESTA NA DATA HABIL PARA ADESÃO.
    $prog_habil = programaDataHabil( $_SESSION['par']['prgid'] );

    if($prog_habil == 'S'){
        $habilita = 'onclick="continuaAdesao()"';
    }else{
        $habilita = 'disabled="disabled"';
    }

    if($munBloqueado == 'S'){
        $habilita = 'disabled="disabled"';
    }else{
        if( $aderiu == 'S'){
            $habilita = 'disabled="disabled"';
        }else{
            $habilita = 'onclick="continuaAdesao()"';
        }

    }
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_info_adesao&acao=A');

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
        $('#requisicao').val('continuaAdesaoPronatec');
        $('#formulario').submit();
    }
</script>


<form action="" method="POST" id="formulario" name="formulario">

    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <center>
        <table bgcolor="#f5f5f5" align="center" class="tabela listagem" >
            <tr>
                <td width="90%" align="center">
                    <br>
                    <fieldset style="width: 70%;">
                        <legend style="font-family: sans-serif; font-size: 14px;"> ORIENTAÇÕES </legend>
                                <? if($munBloqueado == 'S'){ ?>
                                        <p style="text-indent: 5%; text-align: justify; font-family: sans-serif; font-size: 14px; line-height:200%;">
                                            <span style="font-weight: bold;">
                                                ESTE MUNICÍPIO NÃO PODERÁ REALIZAR ESTA ADESÃO POR NÃO ATENDER AOS REQUISITOS ESTABELECIDOS NA PORTARIA Nº 125 DE 13 DE FEVEREIRO DE 2014. ESSES CRITÉRIOS SÃO:
                                            </span>
                                        </p>
                                        <br>
                                        <p style="text-indent: 0%; text-align: justify; font-family: sans-serif; font-size: 14px; line-height:200%;">
                                            Art. 4º - Estão aptos a serem unidades demandantes da SECADI para oferta de cursos de EJA articulada à Educação Profissional, no âmbito do PRONATEC:
                                            <br>
                                            I - Os estados e o Distrito Federal;
                                            <br>
                                            II - Os municípios que atendam pelo menos um dos seguintes critérios:
                                            <br>
                                            a) ter aderido a Resolução FNDE/CD nº 48, no ano de 2012 ou no ano de 2013 e ter solicitado matrículas de "EJA integrada à qualificação profissional";
                                            <br>
                                            b) ser Polo da Educação Inclusiva, Direito à Diversidade;
                                            <br>
                                            c) ser integrante do G100: municípios populosos, com baixa receita per capita e alta vulnerabilidade socioeconômica;
                                            <br>
                                            d) integrar o Plano Juventude Viva;
                                            <br>
                                            e) estar entre os 20 municípios com o maior número de Escolas do Campo, de acordo com o Censo do INEP, por unidade da federação;
                                            <br>
                                            f) ter comunidades remanescentes de quilombos certificadas ou tituladas pela Fundação Palmares; ou
                                            <br>
                                            g) ser capital ou ter mais de 200 mil habitantes.
                                        </p>

                                <? }else{ ?>

                                        <img width=70px" src="../imagens/brasao.gif" style="margin-bottom: 15px;">

                                        <p style="font-size: 14px; font-weight: bold; margin-bottom: 10px;">
                                            MINISTÉRIO DA EDUCAÇÃO SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO, DIVERSIDADE E INCLUSÃO
                                        </p>
                                        <p style="font-size: 14px; font-weight: bold;">
                                            DIRETORIA DE POLÍTICAS DE ALFABETIZAÇÃO E EDUCAÇÃO DE JOVENS E ADULTOS
                                        </p>
                                        <br>
                                        <p style="font-size: 14px; font-weight: bold; text-decoration: underline; margin-bottom: 25px;">
                                            COMUNICADO
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; margin-bottom: 10px;">

                                            PORTARIA Nº 125 DE 13 DE FEVEREIRO DE 2014 - Dispõe sobre a oferta de Educação de Jovens e Adultos - EJA com Qualificação Profissional no âmbito do

                                            Programa Nacional de Acesso ao Ensino Técnico e Emprego - PRONATEC pelos Municípios, de que trata a Lei nº 12.513 de 26 de outubro de 2011

                                            <a href="http://pronatec.mec.gov.br/institucional-90037/base-legal" target="_blank"> pronatec.mec.gov.br/institucional-90037/base-legal </a> e da Resolução CD/FNDE nº 48 de 11 de dezembro de 2013

                                            <a href="http://www.fnde.gov.br/fnde/legislacao/resolucoes" target="_blank"> www.fnde.gov.br/fnde/legislacao/resolucoes</a>. Leia a íntegra da Portaria.

                                            <a href="http://pesquisa.in.gov.br/imprensa/servlet/INPDFViewer?jornal=1&pagina=15&data=14/02/2014&captchafield=firistAccess" target="_blank"> CLIQUE AQUI </a>

                                        </p>

                                        <!--
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            1 - Acessar o SIMEC por meio do seguinte endereço eletrônico: http://simec.mec.gov.br ;
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            2 - Informe o CPF e a Senha do(a) Prefeito(a) ou do(a) Secretário(a) Estadual de Educação;
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            3 - Se for Unidade da Federação, aparecerá a seguinte tela:
                                        </p>

                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/1_tela.png">
                                        <br>
                                        <br>

                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            4 - Clicar na opção "Programa", indicado dentro do círculo vermelho; Aparecerá a tela abaixo. Se for município, a tela acima não aparecerá, o programa irá diretamente para a tela abaixo;
                                        </p>

                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/2_tela.png">
                                        <br>
                                        <br>
                                        
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            5 - Clique no banner marcado "EJA PRONATEC";
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            6 - A tela a seguir irá aparecer;
                                        </p>
                                         -->
                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/3_tela.png">
                                        <br>
                                        <br>

                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            1 - Após a leitura do comunicado, clique em "Continuar Adesão", caso queira realizar a adesão, ou caso contrário em "Sair";
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            2 - Se a opção for por "Continuar Adesão", a tela abaixo será apresentada. Nela estará contida a prévia do Termo de Adesão que em outro momento poderá ser impresso, assinado pelo Prefeito ou Secretário Estadual de Educação, com firma reconhecida para ser encaminhado ao Ministério da Educação no endereço indicado no próprio Termo;
                                        </p>

                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/4_tela.png">
                                        <br>
                                        <br>

                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            3 - Se a opção for por "Iniciar Processo", a tela abaixo será aberta. Se a opção for por "Cancelar Processo", a Adesão será inicializada. Se a opção for "Sair", o Sistema encaminhará o usuário para a página principal do SIMEC;
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            4 - Ao "Iniciar Processo", aparecerá a seguinte tela:
                                        </p>

                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/5_tela.png">
                                        <br>
                                        <br>

                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            5 - Aparecerá a seguinte tela:
                                        </p>

                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/6_tela.png">
                                        <br>
                                        <br>
                                        
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            6 - Informar todos os campos e Salvar, quando quiser continuar a preencher os outros formulários ou Cancelar;
                                        </p>
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            7 - Preencher o formulário, abaixo, com os dados do "Supervisor de Demandas". O "Supervisor de Demandas" deverá ser, obrigatoriamente, servidor(a) da Secretaria Municipal de Educação ou Servidor(a) da Secretaria Estadual de Educação, indicados pelo(a) Prefeito(a) ou Secretário(a) Estadual de Educação, respectivamente. O ?Supervisor de Demandas? será o ente de ligação entre o município ou o estado com o Ministério da Educação. No campo "Data Início", deverá ser informada a data prevista para iniciar o(s) curso(s). Salvar quando for continuar a preencher os demais formulários ou Cancelar;
                                        </p>
                                        
                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/7_tela.png">
                                        <br>
                                        <br>
                                        
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            8 - Na tela abaixo, preencher os campos quando houver público para as respectivas modalidades. Se não houver público em alguma modalidade, informar número 0 (zero). Salvar quando for preencher outros formulários ou Cancelar;
                                        </p>
                                        
                                        <br>
                                            <img width=860px" src="../imagens/eja_pronatec/8_tela.png">
                                        <br>
                                        <br>
                                        
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            9 - No formulário a seguir, anexar o Projeto Político Pedagógico Integrado (PPPintegrado). Sugerimos que o PPPintegrado seja redigido no Microsoft Word e salvado no formato PDF; 
                                        </p>
                                        
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            10 - No mesmo formulário, após o preenchimento dos campos, ir em "estado atual" no lado direito da tela e tramitar o processo de adesão ao Ministério da Educação para análise;
                                        </p>
                                        
                                        <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                            11 - Após a análise realizada pelo Ministério da Educação, imprimir o Termo de Adesão, colher a assinatura do(a) Prefeito(a), autenticar o documento e encaminha-lo ao MEC.    
                                        </p>
                                        
                                <? } ?>
                    </fieldset>
                    <br>
                </td>
            </tr>
            <tr>
                <td align="center">
                    <div style="float: center; width: 70%; ">
                        <input type="button" name="continua" value="Continuar Adesão" <?=$habilita;?>/>
                        <!-- <input type="button" name="continua" value="Continuar Adesão" onclick="continuaAdesao();"/> -->

                        <input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
                    </div>
                </td>
            </tr>
        </table>
    </center>
</form>



