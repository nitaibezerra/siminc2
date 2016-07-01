<?PHP
    include_once '_funcoes_eja.php';

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if( !$_SESSION['par']['prgid'] ){
        $db->sucesso( 'principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';

    #PEGA, SELECIONA O MUNICÍPIO OU ESTADO SELECIONADO OU LOGADO PELO USUÁRIO. É USADO PARA CRIAR O TITULO.
    $sql = " SELECT prgdsc FROM par.programa WHERE prgid = {$_SESSION['par']['prgid']} ";
    $nomePrograma = $db->pegaUm($sql);

    if($_SESSION['par']['muncod']){
        $sql = "SELECT mundescricao, estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
        $rsMunicipio = $db->pegaLinha($sql);
        $descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];

        $munBloqueado = bucarMunicipioBloqueado();

    }else{
        $descricao = $_SESSION['par']['estuf'];
    }

    monta_titulo($nomePrograma, $descricao);
    echo '<br/>';

    $aderiu = verificaAdesaoEJA();

    #VERIFICA SE O PROGRAMA ESTA NA DATA HABIL PARA ADESÃO.
    /*
    $prog_habil = programaDataHabil( $_SESSION['par']['prgid'] );

    if($prog_habil == 'S'){
        $habilita = 'onclick="continuaAdesao()"';
    }else{
        $habilita = 'disabled="disabled"';
    }
    */

    if($munBloqueado != ''){
        $habilita = 'disabled="disabled"';
    }else{
        if( $aderiu == 'S'){
            $habilita = 'disabled="disabled"';
        }else{
            $habilita = 'onclick="continuaAdesao()"';
        }

        echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja/eja_informativo_adesao&acao=A');
    }

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
        $('#requisicao').val('continuaAdesao');
        $('#formulario').submit();
    }
</script>


<form action="" method="POST" id="formulario" name="formulario">

    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <center>
        <table bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td width="90%" align="center">
                    <br>
                    <fieldset style="width: 70%;">
                        <legend style="font-family: sans-serif; font-size: 14px;"> ORIENTAÇÕES </legend>
                            <p style="text-indent: 5%; text-align: justify; font-family: sans-serif; font-size: 14px; line-height:200%;">

                                <? if($munBloqueado != ''){ ?>
                                        <span style="font-weight: bold;">
                                            Observação: "Este município não está apto a realizar esta adesão. Constatou-se, por meio da versão preliminar do censo escolar de 2013,
                                            que nesse ano  houve redução das matrículas, em comparação ao ano de 2012".
                                        </span>
                                        <br><br>
                                <? } ?>
                                <!--
                                O Minisério da Educação transfere recusos financeiros a titulo de apoio á manutenção de novas turmas de Educação de Joves e Adultos oferecidas
                                pelas rede públicas de ensino que tenham matrículas ainda não contempladas com recursos do FUNDEB. Para ter acesso a esse recursos os Municípios,
                                os Estados e o Distrito Federal precisam acessar o sistema e preencher todos os campos do formulário eletrônico para que a adesão seja efetivada em 2014.
                                É recomendável a leitura prévia da Resolução por meio do link "Não Informado".
                                -->
                            </p>

                            <img width=70px" src="../imagens/brasao.gif">

                            <p style="font-size: 14px; font-weight: bold;">
                                MINISTÉRIO DA EDUCAÇÃO SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO, DIVERSIDADE E INCLUSÃO
                            </p>
                            <p style="font-size: 14px; font-weight: bold;">
                                DIRETORIA DE POLÍTICAS DE ALFABETIZAÇÃO E EDUCAÇÃO DE JOVENS E ADULTOS
                            </p>
                            <p style="font-size: 14px; font-weight: bold; text-decoration: underline;">
                                Comunicado
                            </p>
                            <p style="font-size: 13px; font-weight: bold; text-align: left;">
                                I - APRESENTAÇÃO
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                Informa-se por meio desse, que a nova Resolução CD/FNDE Nº 48 de 11 de dezembro de 2013, foi divulgada no site do FNDE no link: <a href="http://www.fnde.gov.br/fnde/legislacao/resolucoes" target="_blank">http://www.fnde.gov.br/fnde/legislacao/resolucoes</a>
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                A Resolução CD/FNDE Nº 48 de 11 de dezembro de 2013, altera o art. 4º, o caput e o § 2º do art. 6º, o art. 8º, o caput do art. 15, no qual se inclui o § 4º, e os arts. 18 e 23 da <b>Resolução CD/FNDE nº 48, de 2 de outubro de 2012.</b> Dessa forma, se faz necessário o conhecimento do inteiro teor dessa Resolução.
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                A resolução em questão tem o objetivo de transferência direta de recursos financeiros aos estados, municípios e Distrito Federal para a manutenção e desenvolvimento de <b>novas turmas de Educação de Jovens e Adultos</b> oferecidas pelas redes públicas de ensino, <b>na modalidade presencial</b>, cujas matrículas ainda não tenham sido contempladas com recursos do Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação (FUNDEB).
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                Os beneficiários dessa iniciativa são as pessoas com 15 anos ou mais que não completaram o ensino fundamental ou médio, e que ainda não foram cadastradas no Censo Escolar (Educacenso) de 2013. Dentre as quais são considerados prioritários os egressos do Programa Brasil Alfabetizado, populações do campo, comunidades quilombolas, povos indígenas e pessoas privadas de liberdade.
                            </p>
                            <p style="font-size: 13px; font-weight: bold; text-align: left;">
                                II - PERÍODO ESTABELECIDO PARA A REALIZAÇÃO DA ADESÃO
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                A adesão do estado, Distrito Federal ou município será permitida no Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação (SIMEC) a partir do dia 17/12/2013, no portal eletrônico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a> , que ficará aberto por 30 dias, a contar da data de abertura.
                            </p>
                            <p style="font-size: 13px; font-weight: bold; text-align: left;">
                                III - PROCEDIMENTOS PARA A ADESÃO - PASSO A PASSO
                            </p>
                            <p style="font-size: 12px; text-align: justify;">
                                <b>Passo 01 - Acessar o SIMEC, pelo endereço eletrônico</b> <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br,</a> <b>clique na Aba PAR e aparecerá a primeira tela.</b>
                            </p>
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 02 - Na mesma tela, tela inical, Clicar o ícone: Financiamento de Novas Turmas de Educação de Jovens e Adultos
                            </p>

                            <img width=860px" src="../imagens/eja/1_tela.png">
                            <!--
                            <img width=860px" src="../imagens/eja/segunda_tela.png">
                            -->
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 03: Clicar em:  continuar a adesão.
                            </p>

                            <img width=860px" src="../imagens/eja/3_tela.png">
                            <br>
                            <b>.</b><br>
                            <b>.</b><br>
                            <b>.</b><br>
                            <img width=860px" src="../imagens/eja/3_1_tela.png">

                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Após clicar continuar a adesão aparecerá uma mensagem de boas vindas à ação de financiamento de novas turmas.
                            </p>

                            <img width=860px" src="../imagens/eja/4_tela.png">

                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 04: Clicar em: Iniciar o Processo
                            </p>
                            
                            <img width=860px" src="../imagens/eja/5_tela.png">
                            
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 05: Preencher os campos indicados e salvar no final. Nesta página estão disponíveis as informações sobre as matrículas de EJA no Censo Escolar de 2010 a 2013 e o número de egressos do Programa Brasil Alfabetizado.
                            </p>
                            
                            <img width=860px" src="../imagens/eja/6_tela.png">
                            
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 06: A tela abaixo aparecerá automaticamente. Nela serão informadas as escolas onde as novas turmas irão funcionar. A relação das escolas surgirá após duplo click no retângulo apontado abaixo. Após indicar todas as escolas, clicar em salvar.
                            </p>
                            
                            <img width=860px" src="../imagens/eja/7_tela.png">
                            
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Caso seja necessário remover uma escola informada indevidamente, basta selecionalá e clicar no "X" (vermelho)
                            </p>
                            
                            <img width=330px" src="../imagens/eja/8_tela.png">
                            
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 07: Após salvar, clicar em: enviar para análise do MEC  conforme indicação abaixo. Após o envio não será possível alterar o documento antes da análise técnica do MEC.
                            </p>
                            
                            <img width=860px" src="../imagens/eja/9_tela.png">
                            
                            <p style="font-size: 12px; text-align: justify; font-weight: bold;">
                                Passo 08: Após a aprovação do MEC será possível imprimir a ficha de aprovação com todas as informações sobre o processo de análise. A ficha pode ser impressa, mas NÃO PRECISA ser enviada ao MEC. 
                            </p>
                            
                            <img width=860px" src="../imagens/eja/10_tela.png">

                    </fieldset>
                    <br>
                </td>
            </tr>
            <tr>
                <td align="center">
                    <div style="float: center; width: 70%; ">
                        <input type="button" name="continua" value="Continuar Adesão" <?=$habilita;?>/>
                        <input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
                    </div>
                </td>
            </tr>
        </table>
    </center>
</form>



