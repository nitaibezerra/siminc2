<?php
    include_once '_funcoes_maismedicos_2015.php';
    include_once APPRAIZ . "includes/workflow.php";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if( !$_SESSION['par']['prgid'] || $_SESSION['par']['muncod'] == '' ){
        $db->sucesso( 'principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    if ($_REQUEST['requisicao'] == 'salvarInformacaoMum') {
        salvarInformacaoMum($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'salvarTermoResidencia') {
        salvarTermoResidencia($_POST);
        exit();
    }

    $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

    $infoMun    = informacaoMunicipios_2015();
    $dadosVagas = verificarVagas();

    $dadosm = verificarDadosMunicipios();
    if( $dadosm ){
        extract($dadosm);
    }

    if( $_SESSION['par']['rqmid'] ){
        $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);

        $respoMun = verificaInformacaoMunicipio($_SESSION['par']['rqmid']);
        extract($respoMun);
    }

    $habilitado = verificaDataFechamento();

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/informacao_municipio&acao=A');

    monta_titulo("Mais Médicos - Edital nº 1/2015", $titulo_modulo.' <br> '.recuperaMunicipioEstado());
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">

    function salvarTermoResidencia() {
        jQuery.ajax({
            url: 'par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/informacao_municipio&acao=A',
            data: {
                requisicao: 'salvarTermoResidencia',
                rqmid: jQuery("#rqmid").val(),
                rqmaceitetermoresidencia: 't'
            },
            async: false,
            type: 'POST',
            success: function(data) {
                if (trim(data) == 'S') {
                    alert('Termo de Compromisso Residência Médica aceito com sucesso!');
                    jQuery("#termoresidencia").attr('disabled', 'true');
                } else {
                    alert('Não foi possível aceitar o Termo de Compromisso Residência Médica!');
                }
            }
        });
    }


</script>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
    <tr class="subtituloEsquerda">
        <td height="10" width="45%">
            <input type="button" value="Voltar" onclick="window.location.href='?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" id="btnVoltar" />
        </td>
    </tr>
</table>

<br>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value="salvarInformacaoMum"/>

    <input type="hidden" id="rqmid" name="rqmid" value="<?=$_SESSION['par']['rqmid'];?>"/>
    <input type="hidden" id="rqmquestao01" name="rqmquestao01" value="<?=$dadosVagas['vgmnumvagashab'];?>"/>
    <input type="hidden" id="rqmquestao02" name="rqmquestao02" value="<?=$dadosVagas['vgmmedicohab'];?>"/>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0">
        <tr>
            <td width="95%">
                <table class="tabela" align="center"bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Somente poderão participar desta pré-seleção municípios localizados em Unidades da Federação (UF) que, conforme tabela constante do Anexo I, apresentem:</td>
                    </tr>
                </table>
                <br>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0" style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Na primeira etapa desta pré-seleção, serão analisadas a relevância e a necessidade social da oferta de curso de medicina no município. O município deverá atender, obrigatoriamente, aos seguintes critérios:</td>
                    </tr>
                </table>
                <br>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" border="0"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloCentro">Item</td>
                        <td class="SubTituloCentro">Informação Município</td>
                        <td class="SubTituloCentro">Valor Oficial</td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Não se constituir capital do Estado.</td>
                        <td class="SubTituloCentro"><?php echo $dadosMun['rqmquestao04'] == 't' ? 'Sim' : 'Não'; ?></td>
                        <td class="SubTituloCentro"><?php echo $dadosMun['rqmquestao04'] == 't' || $capital == 't' ? '<img src="../imagens/check_checklist_vermelho.png">' : '<img src="../imagens/check_checklist.png">'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Possuir oferta de curso de medicina em seu território ou na região de saúde.</td>
                        <td class="SubTituloCentro"><?php echo $dadosMun['rqmquestao05'] == 't' ? 'Sim' : 'Não'; ?></td>
                        <td class="SubTituloCentro"><?php echo $dadosMun['rqmquestao05'] == 't' ? '<img src="../imagens/check_checklist_vermelho.png">' : '<img src="../imagens/check_checklist.png">'; ?></td>
                    </tr>
                </table>
                <br>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" border="0"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloCentro" width="75%">Item</td>
                        <td class="SubTituloCentro">Valor Oficial</td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda" width="65%">Número de leitos disponíveis SUS por aluno maior ou igual a 5 (cinco), ou seja, para um curso com 50 vagas, o município deverá possuir, no mínimo, 250 leitos disponíveis SUS.</td>
                        <td class="SubTituloCentro"><?PHP echo $infoMun['ifmtotalsus']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Número de alunos por equipe de atenção básica menor ou igual a 3 (três).</td>
                        <td class="SubTituloCentro">
                            <?php
                                if( $infoMun['ifmeab'] ){
                                    $ifmeab = number_format($infoMun['ifmeab'], 2);
                                }
                                echo $ifmeab;
                            ?>
                            <input type="hidden" id="rqmquestao07" name="rqmquestao07" value="<?php echo $ifmeab; ?>"/>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Existência de leitos de urgência e emergência ou Pronto Socorro.</td>
                        <td class="SubTituloCentro">
                            <?php echo $pronto_socorro; ?>
                            <input type="hidden" id="rqmquestao08" name="rqmquestao08" value="<?php echo $pronto_socorro == 'SIM' ? 't' : 'f'; ?>"/>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Existência de, pelo menos 3 (três), Programas de Residência Médica nas especialidades prioritárias (Clínica Médica, Cirurgia, Ginecologia-Obstetrícia, Pediatria e Medicina de Família e Comunidade).</td>
                        <td class="SubTituloCentro">
                            <?php echo $residencia_medica > 3 ? 'SIM' : 'NÃO'; ?>
                            <input type="hidden" id="rqmquestao10" name="rqmquestao10" value="<?php echo $residencia_medica > 3 ? 't' : 'f'; ?>"/>
                        </td>
                    </tr>
                    <?php if($residencia_medica < 3) { ?>
                    <tr>
                        <td>
                            <table width="95%" align="center" border="0" cellpadding="3" cellspacing="1" border="2">
                                <tr>
                                    <td height="15"></td>
                                </tr>
                                <tr>
                                    <td align="center" style="font-size: 15px; font-family:arial;"><b>Termo de Compromisso Residência Médica</b>
                                    </td>
                                </tr>
                                <tr>
                                    <td height="15"></td>
                                </tr>
                                <?php
                                    $municipio = recuperarMunicipio();
                                    $municipio = explode("-", $municipio);
                                ?>
                                <tr>
                                    <td style="font-family:arial;">
                                        <p style="text-align: justify; font-size: 15px; margin-left:20%; margin-right:20%">
                                            O Município <?php echo $municipio[0]; ?> do Estado <?php echo $municipio[1]; ?> se compromete, juntamente com a Instituição de Educação Superior privada vencedora do chamamento
                                            público para oferta de cursos de graduação em medicina, a apoiar o desenvolvimento de Programas de Residência Médica em seu território
                                            de modo a que, até 1 (um) ano após o inicio das atividades do curso de medicina, no mínimo 03 (três) programas de residência médica estejam
                                            implementados nas áreas prioritárias nos termos da Portaria Normativa n° 13/2013.</p>
                                    </td>
                                </tr>
                                <tr>
                                    <td height="15"></td>
                                </tr>
                                <tr>
                                    <td id="div_termoresidencia" align="center">
                                    	<?PHP
                                            if($rqmaceitetermoresidencia == 'f' || $rqmaceitetermoresidencia == ''){
                                                if( $habilitado == 'S' ){
                                        ?>
                                                    <input type="submit" name="termoresidencia" id="termoresidencia" value="Aceito">
                                        <?PHP
                                                }else{
                                        ?>
                                                    <input type="submit" name="termoresidencia_disabled" id="termoresidencia_disabled" value="Aceito" disabled="disabled">
                                        <?PHP
                                                }
                                            } else {
                                        ?>
                                            <font style="font-size: 15px; color: #FF0000;"><b>Termo Aderido.</b></font>
                                    	<?php } ?>
                                    </td>
                                </tr>
                            </table>
                        </td>
                        <td>&nbsp;</td>
                    </tr>
                <?php } ?>
                <tr>
                    <td class="SubTituloEsquerda">Adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica – PMAQ.</td>
                    <td class="SubTituloCentro">
                        <?php echo $infoMun['ifmpmaq'] == 't' ? 'SIM' : 'NÃO'; ?>
                        <input type="hidden" id="rqmquestao11" name="rqmquestao11" value="<?=$infoMun['ifmpmaq'];?>"/>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloEsquerda">Existência de Centro de Atenção Psicossocial – CAPS.</td>
                    <td class="SubTituloCentro">
                        <?php echo $infoMun['ifmtotalcaps']; ?>
                        <input type="hidden" id="rqmquestao12" name="rqmquestao12" value="<?php echo $infoMun['ifmtotalcaps'] == 'SIM' ? 't' : 'f'; ?>"/>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloEsquerda">Hospital de ensino ou unidade hospitalar com mais de 80 (oitenta) leitos, com potencial de ensino, conforme legislação de regência.</td>
                    <td class="SubTituloCentro">
                        <?php echo $infoMun['ifmhospleito']; ?>
                        <input type="hidden" id="rqmquestao13" name="rqmquestao13" value="<?php echo $hospital_ensino == 'SIM' ? 't' : 'f'; ?>"/>
                    </td>
                </tr>
            </table>
            <br>
        </td>
        <?php if($docid){ ?>
        <td width="5%" align="center" valign="top" class ="SubTituloCentro">
                <?php wf_desenhaBarraNavegacao( $docid , array('rqmid' => $_SESSION['cap']['rqmid']), ''); ?>
        </td>
        <?php } ?>
    </tr>
</table>
</form>