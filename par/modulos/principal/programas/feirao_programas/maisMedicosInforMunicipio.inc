<?php
include_once '_funcoes_maismedicos.php';
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if ($_REQUEST['requisicao'] == 'alterarInfor') {
    alterarInfor($_POST);
    exit();
}

if ($_REQUEST['requisicao'] == 'anexarTermoPerceria') {
    header('Content-Type: text/html; charset=iso-8859-1');
    anexarTermoPerceria($_POST, $_FILES);
    exit();
}

if ($_REQUEST['requisicao'] == 'excluirParceiro') {
    excluirParceiro($_REQUEST['pmmid']);
    listarParceiro();
    exit();
}

if ($_REQUEST['requisicao'] == 'salvarTermoResidencia') {
    salvarTermoResidencia($_POST);
    exit();
}

if ($_REQUEST['download'] == 'S') {
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_REQUEST['arqid']);
    echo "<script>window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A';</script>";
    exit();
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">
    function cadastrarParceiro() {
        return window.open('par.php?modulo=principal/programas/feirao_programas/maisMedicosRegiaoSaude&acao=A', 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }

    function alterarParceiro(pmmid) {
        return window.open('par.php?modulo=principal/programas/feirao_programas/maisMedicosRegiaoSaude&acao=A&pmmid=' + pmmid, 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }

    function excluirParceiro(pmmid) {
        if (confirm("Deseja realmente excluir o Parceiro?")) {
            jQuery.ajax({
                url: 'par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A',
                data: {
                    requisicao: 'excluirParceiro', pmmid: pmmid
                },
                async: false,
                success: function(data) {
                    alert('Parceiro excluído com sucesso!');
                    $("#div_parceiro").html(data);
                }
            });
        }
    }

    function abrirTermo(pmmid) {
        return window.open('par.php?modulo=principal/programas/feirao_programas/maisMedicosTermoParceria&acao=A&pmmid=' + pmmid, 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }

    function salvarTermoResidencia() {
        jQuery.ajax({
            url: 'par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A',
            data: {requisicao: 'salvarTermoResidencia', rqmid: jQuery("#rqmid").val(), rqmaceitetermoresidencia: 't'},
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

    function anexarTermoPerceria(muncod) {
        var arquivo = $('#arquivo_' + muncod);

        var erro;

        if (!arquivo.val()) {
            alert('É necessário selecionar um arquivo antes de anexar!');
            arquivo.focus();
            erro = 1;
            return false;
        }

        if (!erro) {
            $('#requisicao').val('anexarTermoPerceria');
            $('#muncod_anexo').val(muncod);
            $('#formulario').submit();
        }
    }

</script>

<?php
if ($_REQUEST['rqmid']) {
    $_SESSION['par']['rqmid'] = $_REQUEST['rqmid'];
    $_SESSION['par']['muncod'] = $_REQUEST['muncod'];
    $_SESSION['par']['prgid'] = 228;
    $_SESSION['abaMaisMedicosIM'] = 'S';
}

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
$dadosm = verificarDadosMunicipios();
if ($_SESSION['par']['rqmid']) {
    $infor = visualizarInfor($_SESSION['par']['rqmid']);
    extract($infor);
}

$vagas = verificarVagas();

if ($dadosm) {
    extract($dadosm);
}

if ($vagas) {
    extract($vagas);
}

if (trim($rqmid)) {
    $_SESSION['abaMaisMedicos'] = 'S';
    $_SESSION['abaMaisMedicosIM'] = 'S';
} else {
    $_SESSION['abaMaisMedicos'] = 'N';
    $_SESSION['abaMaisMedicosIM'] = 'N';
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A');

monta_titulo($titulo_modulo, recuperaMunicipioEstado());
$_SESSION['maismedicos'] = true;

if ($_SESSION['par']['rqmid']) {
    $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);
}
?>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
    <tr class="subtituloEsquerda">
        <td height="10" width="45%">
            <input type="button" value="Voltar" onclick="window.location.href='?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" id="btnVoltar" />
        </td>
    </tr>
</table>

<br>
<!--
<form id="formulario" method="post" name="formulario" enctype="multipart/form-data" action="par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A">
-->

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value="alterarInfor"/>
    <input type="hidden" id="muncod_anexo" name="muncod_anexo" value=""/>
    <input type="hidden" id="rqmid" name="rqmid" value="<?php echo $_SESSION['par']['rqmid']; ?>"/>
    <input type="hidden" id="rqmquestao01" name="rqmquestao01" value="<?php echo $vgmnumvagashab; ?>"/>
    <input type="hidden" id="rqmquestao02" name="rqmquestao02" value="<?php echo $vgmmedicohab; ?>"/>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0">
        <tr>
            <td width="95%">
                <table class="tabela" align="center"bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Somente poderão participar desta pré-seleção municípios localizados em Unidades da Federação (UF) que, conforme tabela constante do Anexo I, apresentem:</td>
                    </tr>
                </table>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0" style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Na primeira etapa desta pré-seleção, serão analisadas a relevância e a necessidade social da oferta de curso de medicina no município. O município deverá atender, obrigatoriamente, aos seguintes critérios:</td>
                    </tr>
                </table>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" border="0"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloCentro">Item</td>
                        <td class="SubTituloCentro">Informação Município</td>
                        <td class="SubTituloCentro">Valor Oficial</td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda" width="65%">Município com mais de 70 mil habitantes, conforme dados do Instituto Brasileiro de Geografia e Estatística (IBGE), Censo 2012.</td>
                        <td class="SubTituloCentro"><?php echo formata_numero($rqmquestao03); ?></td>
                        <td class="SubTituloCentro"><?php echo $rqmquestao03 > 70000 ? '<img src="../imagens/check_checklist.png">' : '<img src="../imagens/check_checklist_vermelho.png">'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Não se constituir capital do Estado.</td>
                        <td class="SubTituloCentro"><?php echo $rqmquestao04 == 't' ? 'Sim' : 'Não'; ?></td>
                        <td class="SubTituloCentro"><?php echo $rqmquestao04 == 't' || $capital == 't' ? '<img src="../imagens/check_checklist_vermelho.png">' : '<img src="../imagens/check_checklist.png">'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda">Possuir oferta de curso de medicina em seu território.</td>
                        <td class="SubTituloCentro"><?php echo $rqmquestao05 == 't' ? 'Sim' : 'Não'; ?></td>
                        <td class="SubTituloCentro"><?php echo $rqmquestao05 == 't' ? '<img src="../imagens/check_checklist_vermelho.png">' : '<img src="../imagens/check_checklist.png">'; ?></td>
                    </tr>
                </table>
                <br>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="5" border="0" width="75%"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Na segunda etapa desta pré-seleção, serão analisados a estrutura de equipamentos públicos e os programas de saúde existentes no município, segundo dados do Ministério da Saúde. O município deverá atender, obrigatoriamente, aos seguintes critérios:</td>
                    </tr>
                </table>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" border="0"  style="margin: 0px; width: 100%;">
                    <tr>
                        <td class="SubTituloCentro" width="75%">Item</td>
                        <td class="SubTituloCentro">Valor Oficial</td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda" width="65%">Número de leitos disponíveis SUS por aluno maior ou igual a 5 (cinco), ou seja, para um curso com 50 vagas, o município deverá possuir, no mínimo, 250 leitos disponíveis SUS.</td>
                        <td class="SubTituloCentro"><?php echo $rqmquestao06; ?></td>
                    </tr>
                    <?php if($leitos_sus < 250){ ?>
                    <tr>
                        <td bgcolor="#f0f0f0" colspan="3">
                            <?php if( !in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) && !in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){ ?>
                                    <span class="adicionar_parceiro" style="cursor:pointer" onclick="cadastrarParceiro();"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">&nbsp;<b>Adicionar Parceiro</b></span><br>
                            <?php }else{ ?>
                                    <span class="adicionar"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui_d.gif">&nbsp; Adicionar Parceiro </span><br>
                            <?php } ?>
                            <br>
                            <table class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" border="2" width="95%">
                                <tr bgcolor="#ffffff">
                                    <?php
                                        $total_leitos = totalLeitos();
                                        $total_geral = $total_leitos + $leitos_sus;
                                    ?>
                                    <td><b>Total de Leitos da(s) Parceria(s):&nbsp;<?php echo $total_leitos ? $total_leitos : '0'; ?></b></td>
                                    <td><b>Total Geral de Leitos:&nbsp;<?php echo $total_geral; ?></b></td>
                                </tr>
                            </table>
                            <div id="div_parceiro">
                                <?php
                                    listarParceiro();
                                ?>
                            </div>
                        </td>
                    </tr>
                    <?php } ?>
                    <tr>
                        <td class="SubTituloEsquerda">Número de alunos por equipe de atenção básica menor ou igual a 3 (três).</td>
                        <td class="SubTituloCentro">
                            <?php
                                if ($equipe_atencao_basica) {
                                    $equipe_atencao_basica = number_format($equipe_atencao_basica, 2);
                                }
                                echo $equipe_atencao_basica;
                            ?>
                            <input type="hidden" id="rqmquestao07" name="rqmquestao07" value="<?php echo $equipe_atencao_basica; ?>"/>
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
                        <td class="SubTituloEsquerda">Comprometimento dos leitos dos SUS para utilização acadêmica.</td>
                        <td class="SubTituloCentro">
                            <?php echo number_format($grau_comprometimento, 2); ?>
                            <input type="hidden" id="rqmquestao09" name="rqmquestao09" value="<?php echo number_format($grau_comprometimento, 2); ?>"/>
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
                                    	<?php if($rqmaceitetermoresidencia == 'f' || $rqmaceitetermoresidencia == ''){ ?>
                                    	<input type="button" name="termoresidencia" id="termoresidencia" value="Aceito" onclick="salvarTermoResidencia();" />
                                    	<?php } else { ?>
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
                        <?php echo $adesao_pmaq; ?>
                        <input type="hidden" id="rqmquestao11" name="rqmquestao11" value="<?php echo $adesao_pmaq == 'SIM' ? 't' : 'f'; ?>"/>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloEsquerda">Existência de Centro de Atenção Psicossocial – CAPS.</td>
                    <td class="SubTituloCentro">
                        <?php echo $caps; ?>
                        <input type="hidden" id="rqmquestao12" name="rqmquestao12" value="<?php echo $caps == 'SIM' ? 't' : 'f'; ?>"/>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloEsquerda">Hospital de ensino ou unidade hospitalar com potencial para hospital de ensino, conforme legislação de regência.</td>
                    <td class="SubTituloCentro">
                        <?php echo $hospital_ensino; ?>
                        <input type="hidden" id="rqmquestao13" name="rqmquestao13" value="<?php echo $hospital_ensino == 'SIM' ? 't' : 'f'; ?>"/>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloEsquerda">Existência de hospital com mais de 100 (cem) leitos exclusivos para o curso</td>
                    <td class="SubTituloCentro">
                        <?php echo $hospital_100_leitos; ?>
                        <input type="hidden" id="rqmquestao14" name="rqmquestao14" value="<?php echo $hospital_100_leitos == 'SIM' ? 't' : 'f'; ?>"/>
                    </td>
                </tr>
            </table>
            <br>
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" border="0" style="margin: 0px; width: 100%;">
                <tr>
                    <td align="center" bgcolor="#CCCCCC" colspan="2">
                        <input type="submit" value="Salvar" id="btnSalvar"/>
                        <input type="button" value="Cancelar" id="btnCancelar"/>
                    </td>
                </tr>
            </table>
        </td>
        <?php if($docid){ ?>
        <td width="5%" align="center" valign="top" class ="SubTituloCentro">
                <?php wf_desenhaBarraNavegacao( $docid , array('rqmid' => $_SESSION['cap']['rqmid']), ''); ?>
        </td>
        <?php } ?>
    </tr>
</table>
</form>

<script type="text/javascript" language="javascript">
	<?php if($rqmaceitetermoresidencia == 't'){ ?>
		jQuery("#termoresidencia").attr('disabled','true');
	<?php } ?>
	<?php if(!empty($rqmquestao05)){ ?>
		jQuery("#rqmquestao05").attr('disabled','true');
	<?php } ?>
	<?php if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) ||  in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){ ?>
		jQuery("#btnSalvar").attr('disabled','true');
		jQuery("#rqmparecermectexto").attr('disabled','true');
		jQuery("#rqmparecermec").attr('disabled','true');
	<?php } ?>
</script>