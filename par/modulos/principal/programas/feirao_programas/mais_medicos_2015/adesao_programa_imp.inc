<?php
include_once '_funcoes_maismedicos_2015.php';

$m          = recuperaDadosPrefeitura();
$gestor_sus = recuperaGestorSusMunicipio();

$aderiu = verificarAdesao();
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<html>
<head>
    <title>Mais Médicos - Termo de Adesão</title>
    <style>
            @media print {
                .notprint {
                    display: none;
                }
                .div_rolagem {
                    display: none;
                }
            }
            .div_rolagem{
                overflow-x: auto;
                overflow-y: auto;
                height: 50px;
            }
            .notscreen {
                display: none;
            }
            .bordaarredonda {
                background:#FFFFFF;
                color:#000; border: #000 1px solid;
                padding: 10px;
                -moz-border-radius:10px 10px;
                -webkit-border-radius:10px 10px;
                border-radius:10px 10px;
                width:95%;
                text-align:left;
            }
            .quebra{
                page-break-after: always !important;
                height: 0px;
                clear: both;
            }
    </style>
</head>

<body>
<!--    <fieldset style="width: 97%; background:#fff; margin-bottom:35px;">
        <legend> Adesão ao Programa </legend>-->
            <table bgcolor="#ffffff" align="center" class="quebra">
                <tr>
                    <td width="90%" align="center">
                        <div style="width: 95%; magin-top:10px;">
                            <br>
                            <p align="center" style="margin-bottom: 0px;">
                                <font face="Arial, sans-serif" size="3"><b>Termo de Adesão ao Programa Mais Médicos - Edital nº 1/2015</b></font>
                            </p>

                            <br>

                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    TERMO DE ADESÃO QUE ENTRE SI CELEBRAM A SECRETARIA DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR DO MINISTÉRIO DA EDUCAÇÃO E O MUNICÍPIO DE <?php echo strtoupper($m['municipio']);?>, NO ESTADO DO <?php echo strtoupper($m['estado']);?>,
                                    PARA DISPONIBILIZAÇÃO DA ESTRUTURA DE EQUIPAMENTOS PÚBLICOS, CENÁRIOS DE ATENÇÃO NA REDE E PROGRAMAS DE SAÚDE NECESSÁRIOS À AUTORIZAÇÃO DE FUNCIONAMENTO DE CURSOS DE MEDICINA.
                                </span>
                            </p>

                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    O MINISTÉRIO DA EDUCAÇÃO, CNPJ nº 00.394.445/0001-01, neste ato representado por MARTA WENDEL ABRAMO, Secretária de Regulação e Supervisão da Educação Superior, com endereço na
                                    Esplanada dos Ministérios, Bloco "L", 1º andar, sala 100 - CEP 70.047-900, Brasília (DF), e o MUNICÍPIO DE <?php echo strtoupper($m['municipio'])."-".strtoupper($m['estuf']);?>,
                                    <?php echo $m['endereco'];?>, neste ato representado por <?php echo $m['prefeito'];?> (qualificação), nos termos da Lei nº 12.871, de 22 de outubro de 2013, resolvem celebrar o
                                    presente Termo de Adesão para implantação e funcionamento de cursos de medicina, por instituição de educação superior privada, mediante as cláusulas e condições seguintes:
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    1. CLÁUSULA PRIMEIRA - DO OBJETO
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    1.1. O presente termo tem por objeto a adesão do Município de <?php echo $m['municipio']."-".$m['estuf'];?> ao Chamamento Público previsto no Edital nº 1, de 1º de abril de 2015 e
                                    definição de obrigações e responsabilidades do Município no oferecimento de estrutura de equipamentos públicos, cenários de atenção na rede e programas de saúde necessários à
                                    autorização de funcionamento de curso de graduação em medicina a ser ofertado por instituição de educação superior privada nos termos do art. 3º, inciso IV, da Lei nº 12.871/2013.
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    2. CLÁUSULA SEGUNDA - DAS OBRIGAÇÕES DO MUNICÍPIO
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    2.1. Para consecução do objeto estabelecido neste Termo de Adesão, o Município deverá disponibilizar a estrutura de equipamentos públicos, cenários de atenção na rede e programas
                                    de saúde existentes no município e na região de saúde, previstos no artigo 4º, da Portaria Normativa nº 5, de 1º de abril de 2015 e no item 3.2.2 do Edital nº 1/2015, além de
                                    outros que podem ser estabelecidos pela SERES:
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    a) número de leitos do SUS por aluno, maior ou igual a 5 (cinco);
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    b) existência de Equipes Multiprofissionais de Atenção Domiciliar - EMAD;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    c) número de alunos por Equipe de Atenção Básica (EAB) menor ou igual a 3 (três);
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    d) existência de leitos de urgência e emergência ou Pronto Socorro;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    e) grau de comprometimento dos leitos SUS para utilização acadêmica;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    f) existência de, pelo menos, 03 (três) Programas de Residência Médica nas especialidades prioritárias, conforme legislação de regência;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    g) adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    h) existência de Centro de Atenção Psicossocial - CAPS; e
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    i) hospital de ensino ou unidade hospitalar com mais de 80 (oitenta) leitos, com potencial para hospital de ensino, conforme legislação de regência.
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    3. CLÁUSULA TERCEIRA - DAS OBRIGAÇÕES DA SECRETARIA DE REGULAÇÃO E SUPERVISÃO DO MINISTÉRIO DA EDUCAÇÃO
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size:13px;">
                                    3.1. Constituem obrigações da SERES:
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    a) selecionar os Municípios para implantação e funcionamento de cursos de medicina, por instituições de educação superior privadas;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    b) celebrar Termo de Adesão com o município selecionado, acompanhar e monitorar a implantação do curso de medicina naquela localidade;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    c) selecionar as instituições de educação superior privadas para oferta de cursos de graduação em medicina nos municípios selecionados;
                                </span>
                            </p>
                            <p align="justify" style="margin-left: 0.98in;">
                                <span style="font-size: 12px;">
                                    d) editar normas complementares necessárias ao cumprimento do estabelecido neste Termo de Adesão.
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    4. CLÁUSULA QUARTA - DA VIGÊNCIA
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    4.1. Os compromissos assumidos pelo Município no presente Termo de Adesão estão vinculados à regularidade da oferta do curso pela instituição de educação superior.
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    5. CLÁUSULA QUINTA - DA RESCISÃO
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    5.1. No caso de rescisão do presente Termo, cumpre ao Município informar à instituição de educação superior privada ofertante do curso e à Secretaria de Regulação
                                    e Supervisão da Educação Superior, com antecedência mínima de 90 (noventa) dias, a fim de preservar a continuidade da oferta do curso.
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    6. CLÁUSULA SÉTIMA - DA PUBLICAÇÃO
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    6.1. O presente Termo de Adesão estará disponível no Sistema Integrado de Monitoramento e Controle <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>.
                                </font>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    7. CLÁUSULA OITAVA - DAS ALTERAÇÕES
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    7.1. As eventuais alterações do presente Termo de Adesão serão realizadas por meio de termo aditivo acordado entre os partícipes.
                                </span>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    8. CLÁUSULA NONA - DA SOLUÇÃO DE CONTROVÉRSIAS
                                </font>
                            </p>
                            <p align="justify" style="text-indent: 0.59in;">
                                <span style="font-size: 12px;">
                                    8.1. Eventual controvérsia surgida durante a execução do presente Termo de Adesão poderá ser dirimida administrativamente entre os partícipes ou,
                                    em seguida, perante a Câmara de Conciliação e Arbitragem da Administração Federal da Advocacia - Geral da União e, se inviável, posteriormente
                                    perante o foro da Justiça Federal - Seção Judiciária do Distrito Federal.
                                </span>
                            </p>
                            <br><br>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px;">
                                    Brasília-DF, ___ de ____________ de 2015.
                                </span>
                            </p>
                            <br><br>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px;">____________________________________</span>
                            </p>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px; font-weight: bold;">
                                    MARTA WENDEL ABRAMO
                                </span>
                            </p>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px; font-weight: bold;">
                                    SECRETÁRIA DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR
                                </span>
                            </p>
                            <br><br>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px;">
                                    _____________________________________________________
                                </span>
                            </p>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px; font-weight: bold;">
                                    <?php echo $m['prefeito']; ?>
                                </span>
                            </p>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px; font-weight: bold;">
                                    PREFEITO MUNICIPAL DE <?php echo strtoupper($m['municipio'])."-".strtoupper($m['estuf']);?>
                                </span>
                            </p>
                            <br><br>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px;">
                                    _____________________________________________________
                                </span>
                            </p>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px; font-weight: bold;">
                                    <?php echo $gestor_sus ? $gestor_sus : 'Não Informado'; ?>
                                </span>
                            </p>
                            <p align="right" style="margin-right: 0.98in;">
                                <span style="font-size: 12px; font-weight: bold;">
                                    GESTOR LOCAL DO SISTEMA ÚNICO DE SAÚDE
                                </span>
                            </p>
                        </div>
                    </td>
                </tr>
            </table>
<!--    </fieldset>-->

    <table bgcolor="#f5f5f5" align="center" class="tabela notprint">
        <tr>
            <td align="center">
                <div style="float: center; width: 70%;">
                    <input id="imprimir" type="button" name="imprimir" value="Imprimir" onclick="window.print();">
                </div>
            </td>

        </tr>
    </table>