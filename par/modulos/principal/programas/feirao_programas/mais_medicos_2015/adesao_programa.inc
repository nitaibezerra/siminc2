<?php
include_once '_funcoes_maismedicos_2015.php';

if($_REQUEST['requisicao'] == 'salvarTermo'){
    salvarAdesao($_POST);
    exit();
}
$m          = recuperaDadosPrefeitura();
$gestor_sus = recuperaGestorSusMunicipio();

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
monta_titulo("Mais Médicos - Edital nº 1/2015", $titulo_modulo.' <br> '.recuperaMunicipioEstado());

echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/adesao_programa&acao=A');

$aderiu = verificarAdesao();
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

$habilitado = verificaDataFechamento();
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">
    function salvarTermo(aceite){
        if(aceite == 'S'){
            jQuery("#adpresposta").val('S');
        } else {
            jQuery("#adpresposta").val('N');

            if( !confirm('Você clicou em "Não aceito.". Caso confirme esta opção o município não poderá participar do Edital nº 1/2015. Deseja confirmar?') ){
                return false;
            }
        }

        $('#requisicao').val('salvarTermo');
        $('#form').submit();
    }

    function sair(){
        window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
    }

    function imprimirAdesao(){
        return window.open('par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/adesao_programa_imp&acao=A', 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }
</script>

<div id="div_termo">
    <form id="form" name="form" method="POST">
	<input type="hidden" id="requisicao" name="requisicao" value=""/>
	<input id="adpresposta" type="hidden" name="adpresposta" value=""/>
	<input id="tapid" type="hidden" name="tapid" value="28"/>

	<table bgcolor="#f5f5f5" align="center" class="tabela">
            <tr>
                <td width="90%" align="center">
                    <?PHP
                        if( $aderiu != 'S' || $aderiu == ''){
                    ?>
                            <div style="overflow: auto; height:350px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
                    <?PHP
                        }else{
                    ?>
                            <fieldset style="width: 97%; background: #fff; margin-bottom:35px;">
                            <legend><a id="link_termo_adesao" href="javascript:void(0)"> Adesão ao Programa </a></legend>
                            <div id="div_termo_adesao" class="div_termo_adesao" style="overflow: auto; margin-bottom: 15px; display: block;">

                            <table bgcolor="#ffffff" align="center">
                                <tr>
                                    <td width="90%" align="center">
                    <?PHP
                        }
                    ?>
                            <!-- Termo -->
                            <div style="width: 95%; magin-top: 10px;">
                                <br>
                                <p align="center" style="margin-bottom: 0px;"><font face="Arial, sans-serif" size="3"><b>Termo de Adesão ao Programa Mais Médicos</b></font></p><br>

                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        TERMO DE ADESÃO QUE ENTRE SI CELEBRAM A SECRETARIA DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR DO MINISTÉRIO DA EDUCAÇÃO E O MUNICÍPIO DE <?php echo strtoupper($m['municipio']);?>, NO ESTADO DO <?php echo strtoupper($m['estado']);?>,
                                        PARA DISPONIBILIZAÇÃO DA ESTRUTURA DE EQUIPAMENTOS PÚBLICOS, CENÁRIOS DE ATENÇÃO NA REDE E PROGRAMAS DE SAÚDE NECESSÁRIOS À AUTORIZAÇÃO DE FUNCIONAMENTO DE CURSOS DE MEDICINA.
                                    </font>
                                </p>

                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        O MINISTÉRIO DA EDUCAÇÃO, CNPJ nº 00.394.445/0001-01, neste ato representado por MARTA WENDEL ABRAMO, Secretária de Regulação e Supervisão da Educação Superior, com endereço na
                                        Esplanada dos Ministérios, Bloco "L", 1º andar, sala 100 - CEP 70.047-900, Brasília (DF), e o MUNICÍPIO DE <?php echo strtoupper($m['municipio'])."-".strtoupper($m['estuf']);?>,
                                        <?php echo $m['endereco'];?>, neste ato representado por <?php echo $m['prefeito'];?> (qualificação), nos termos da Lei nº 12.871, de 22 de outubro de 2013, resolvem celebrar o
                                        presente Termo de Adesão para implantação e funcionamento de cursos de medicina, por instituição de educação superior privada, mediante as cláusulas e condições seguintes:
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        1. CLÁUSULA PRIMEIRA - DO OBJETO
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        1.1. O presente termo tem por objeto a adesão do Município de <?php echo $m['municipio']."-".$m['estuf'];?> ao Chamamento Público previsto no Edital nº 1, de 1º de abril de 2015 e
                                        definição de obrigações e responsabilidades do Município no oferecimento de estrutura de equipamentos públicos, cenários de atenção na rede e programas de saúde necessários à
                                        autorização de funcionamento de curso de graduação em medicina a ser ofertado por instituição de educação superior privada nos termos do art. 3º, inciso IV, da Lei nº 12.871/2013.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        2. CLÁUSULA SEGUNDA - DAS OBRIGAÇÕES DO MUNICÍPIO
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        2.1. Para consecução do objeto estabelecido neste Termo de Adesão, o Município deverá disponibilizar a estrutura de equipamentos públicos, cenários de atenção na rede e programas
                                        de saúde existentes no município e na região de saúde, previstos no artigo 4º, da Portaria Normativa nº 5, de 1º de abril de 2015 e no item 3.2.2 do Edital nº 1/2015, além de
                                        outros que podem ser estabelecidos pela SERES:
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        a) número de leitos do SUS por aluno, maior ou igual a 5 (cinco);
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        b) existência de Equipes Multiprofissionais de Atenção Domiciliar - EMAD;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        c) número de alunos por Equipe de Atenção Básica (EAB) menor ou igual a 3 (três);
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        d) existência de leitos de urgência e emergência ou Pronto Socorro;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        e) grau de comprometimento dos leitos SUS para utilização acadêmica;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        f) existência de, pelo menos, 03 (três) Programas de Residência Médica nas especialidades prioritárias, conforme legislação de regência;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        g) adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        h) existência de Centro de Atenção Psicossocial - CAPS; e
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        i) hospital de ensino ou unidade hospitalar com mais de 80 (oitenta) leitos, com potencial para hospital de ensino, conforme legislação de regência.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        3. CLÁUSULA TERCEIRA - DAS OBRIGAÇÕES DA SECRETARIA DE REGULAÇÃO E SUPERVISÃO DO MINISTÉRIO DA EDUCAÇÃO
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">3.1. Constituem obrigações da SERES:</font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        a) selecionar os Municípios para implantação e funcionamento de cursos de medicina, por instituições de educação superior privadas;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        b) celebrar Termo de Adesão com o município selecionado, acompanhar e monitorar a implantação do curso de medicina naquela localidade;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        c) selecionar as instituições de educação superior privadas para oferta de cursos de graduação em medicina nos municípios selecionados;
                                    </font>
                                </p>
                                <p align="justify" style="margin-left: 0.98in;">
                                    <font size="3" style="font-size: 11pt">
                                        d) editar normas complementares necessárias ao cumprimento do estabelecido neste Termo de Adesão.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        4. CLÁUSULA QUARTA - DA VIGÊNCIA
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        4.1. Os compromissos assumidos pelo Município no presente Termo de Adesão estão vinculados à regularidade da oferta do curso pela instituição de educação superior.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        5. CLÁUSULA QUINTA - DA RESCISÃO
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        5.1. No caso de rescisão do presente Termo, cumpre ao Município informar à instituição de educação superior privada ofertante do curso e à Secretaria de Regulação
                                        e Supervisão da Educação Superior, com antecedência mínima de 90 (noventa) dias, a fim de preservar a continuidade da oferta do curso.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        6. CLÁUSULA SÉTIMA - DA PUBLICAÇÃO
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        6.1. O presente Termo de Adesão estará disponível no Sistema Integrado de Monitoramento e Controle <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        7. CLÁUSULA OITAVA - DAS ALTERAÇÕES
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        7.1. As eventuais alterações do presente Termo de Adesão serão realizadas por meio de termo aditivo acordado entre os partícipes.
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        8. CLÁUSULA NONA - DA SOLUÇÃO DE CONTROVÉRSIAS
                                    </font>
                                </p>
                                <p align="justify" style="text-indent: 0.59in;">
                                    <font size="3">
                                        8.1. Eventual controvérsia surgida durante a execução do presente Termo de Adesão poderá ser dirimida administrativamente entre os partícipes ou,
                                        em seguida, perante a Câmara de Conciliação e Arbitragem da Administração Federal da Advocacia - Geral da União e, se inviável, posteriormente
                                        perante o foro da Justiça Federal - Seção Judiciária do Distrito Federal.
                                    </font>
                                </p>
                                <br><br>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">Brasília-DF, ___ de ____________ de 2015.</font>
                                </p>
                                <br><br>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">____________________________________</font>
                                </p>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">
                                        <b>MARTA WENDEL ABRAMO</b>
                                    </font>
                                </p>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">SECRETÁRIA DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR</font>
                                </p>
                                <br><br>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">_____________________________________________________</font>
                                </p>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3"><b><?php echo $m['prefeito']; ?></b></font>
                                </p>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">PREFEITO MUNICIPAL DE <?php echo strtoupper($m['municipio'])."-".strtoupper($m['estuf']);?></font>
                                </p>
                                <br><br>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">_____________________________________________________</font>
                                </p>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3"><b><?php echo $gestor_sus ? $gestor_sus : 'Não Informado'; ?></b></font>
                                </p>
                                <p align="right" style="margin-right: 0.98in;">
                                    <font size="3">GESTOR LOCAL DO SISTEMA ÚNICO DE SAÚDE</font>
                                </p>
                            </div>
                        <!-- FECHAR -->
                        <?PHP
                            if( $aderiu == 'S' ){
                        ?>
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                            </fieldset>
                    <?PHP
                        }else{
                    ?>
                            </div>
                    <?PHP
                        }
                    ?>
                </td>
            </tr>
        </table>

        <table bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <?PHP
                    if( $aderiu == 'N' || $aderiu == '' ){
                ?>
                    <td align="center">
                        <div style="float: center; width: 70%;">
                             <?PHP
                                if( $habilitado == 'S' ){
                            ?>
                                    <input id="aceito" type="button" name="aceito" value="Aceito" onclick="salvarTermo('S')">
                                    <input id="naceito" type="button" name="naceito" value="Não Aceito" onclick="salvarTermo('N');">
                            <?PHP
                                }else{
                            ?>
                                    <input id="aceito" type="button" name="aceito" value="Aceito" disabled="disabled">
                                    <input id="naceito" type="button" name="naceito" value="Não Aceito" disabled="disabled">
                            <?PHP
                                }
                            ?>
                            <input id="sair" type="button" name="sair" value="Sair" onclick="sair();">
                        </div>
                    </td>
                <?PHP
                    }else{
                ?>
                    <td align="center">
                        <div style="float: center; width: 70%;">
                            <input id="imprimir" type="button" name="imprimir" value="Visualizar Impressão" onclick="imprimirAdesao();">
			</div>
                    </td>
                <?PHP
                    }
                ?>
            </tr>
	</table>
    </form>
</div>