<?php
verificaSessao('orgao');

$orgid = $_SESSION['obras2']['orgid'];
$obridSession = $_SESSION['obras2']['obrid'];

//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if( possui_perfil(array(PFLCOD_CALL_CENTER))){
    $desabilita = true;
}
$obraLicitacao = new ObraLicitacao();
if ($obridSession) {
    $licid = $obraLicitacao->pegaLicidPorObrid($obridSession);
}

switch ($_REQUEST['op']) {
    case 'liberaAditivo':
        $obra = new Obras($obridSession);
        $esdid = pegaEstadoObra($obra->docid);
        if ($esdid == ESDID_OBJ_EXECUCAO || $esdid == ESDID_OBJ_ADITIVO) {
            if ($esdid == ESDID_OBJ_EXECUCAO) {
                wf_alterarEstado($obra->docid, AEDID_OBJ_EXECUCAO_ADITIVO, '', array('obrid' => $obra->obrid));
            }
            ?>
            <script type="text/javascript">
                <!--
            var pagina = '?modulo=principal/popUpInserirAditivo&acao=A';
                var dest = 'aditivo';
                var TW = '1200';
                var TH = '500';
                var j = window.open(pagina, dest, "toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=yes" + ",width=" + TW + ",height=" + TH);
                j.focus();
                //-->
            </script>
            <?php
        }
    case 'carregaLicitacao':
        $licid = $_POST['licid'];
        break;
    case 'salvar':
        $contrato = new Contrato($_POST['crtid']);

        $dados = $_POST;
        $_POST['crtvalorexecucao'] = (trim($_POST['crtvalorexecucao']) ? $_POST['crtvalorexecucao'] : 0);
        $_POST['crtpercentualdbi'] = (trim($_POST['crtpercentualdbi']) ? $_POST['crtpercentualdbi'] : 0);

        $dados['crtdtassinatura'] = formata_data_sql($_POST['crtdtassinatura']);
        $dados['crtdttermino'] = formata_data_sql($_POST['crtdttermino']);
        $dados['crtvalorexecucao'] = str_replace(array('.', ','), array('', '.'), (string) $_POST['crtvalorexecucao']);
        $dados['crtpercentualdbi'] = str_replace(array('.', ','), array('', '.'), (string) $_POST['crtpercentualdbi']);

        $dados['crtdtassinatura'] = empty($dados['crtdtassinatura']) ? null : $dados['crtdtassinatura'];

        $arqidcontrato = ($dados['arqidcontrato'] ? $dados['arqidcontrato'] : NULL);
        $arquivo = $_FILES["arquivo"];
        if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] && $arquivo["error"] == 0) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $file = new FilesSimec(null, null, "obras2");
            $file->setPasta('obras2');
            $file->setUpload(null, 'arquivo', false);
            $arqidcontrato = $file->getIdArquivo();
        }
        $dados['arqidcontrato'] = $arqidcontrato;

        $crtid = $contrato->popularDadosObjeto($dados)
                ->salvar();

        $obraContrato = new ObrasContrato();
        $obraContrato->apagaByContrato($crtid);
        if (count($_POST['obrid'])) {
            for ($i = 0; $i < count($_POST['obrid']); $i++) {
                $obraContrato->clearDados();

                $arqidos = ($_POST['arqidos'][$i] ? $_POST['arqidos'][$i] : NULL);
                $arquivo = $_FILES["arquivoos_" . $i];

                if (!empty($arquivo['name'])) {
                    $nome_arquivo1 = explode(".", $arquivo['name']);
                    if (strtolower(end($nome_arquivo1)) == "pdf" || empty($nome_arquivo1)) {
                        
                    } else {
                        die('<script type="text/javascript">
								alert(\'É obrigatório que esse arquivo seja um PDF.\nFavor repetir a operação.\');
								window.location.href = window.location.href; 
				</script>');
                    }
                }
                if ($_FILES["arquivoos_" . $i] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] && $arquivo["error"] == 0) {


                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $returnUpload = $file->setUpload(null, 'arquivoos_' . $i, false);

                    if ($returnUpload) {
                        $arqidos = $file->getIdArquivo();
                    } else {
                        $db->rollback();

                        enviaEmailChiavicatti($arquivo);

                        die('<script type="text/javascript">
								alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
								window.location.href = window.location.href; 
							 </script>');
                    }
                } elseif ($arquivo['error'] != 0 && $arquivo['error'] != 4) {

                    if ($arquivo['error'] == 3) {
                        echo('<script type="text/javascript">
								alert(\'Falha no upload do arquivo: ' . $arquivo['name'] . '\nAnexe-o novamente!\');
							 </script>');
                    } else {
                        $db->rollback();

                        enviaEmailChiavicatti($arquivo);

                        die('<script type="text/javascript">
								alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
								window.location.href = window.location.href; 
							 </script>');
                    }
                }

                $arqidcusto = ($_POST['arqidcusto'][$i] ? $_POST['arqidcusto'][$i] : NULL);
                $arquivo = $_FILES["arquivocusto_" . $i];

                if (!empty($arquivo['name'])) {
                    $nome_arquivo1 = explode(".", $arquivo['name']);
                    if (strtolower(end($nome_arquivo1)) == "pdf" || empty($nome_arquivo1)) {
                        
                    } else {
                        die('<script type="text/javascript">
								alert(\'É obrigatório que esse arquivo seja um PDF.\nFavor repetir a operação.\');
								window.location.href = window.location.href;
				</script>');
                    }
                }











                if ($_FILES["arquivocusto_" . $i] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] && $arquivo["error"] == 0) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $returnUpload = $file->setUpload(null, 'arquivocusto_' . $i, false);

                    if ($returnUpload) {
                        $arqidcusto = $file->getIdArquivo();
                    } else {
                        $db->rollback();

                        enviaEmailChiavicatti($arquivo);

                        die('<script type="text/javascript">
								alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
								window.location.href = window.location.href; 
							 </script>');
                    }
                } elseif ($arquivo['error'] != 0 && $arquivo['error'] != 4) {

                    if ($arquivo['error'] == 3) {
                        echo('<script type="text/javascript">
								alert(\'Falha no upload do arquivo: ' . $arquivo['name'] . '\nAnexe-o novamente!\');
							 </script>');
                    } else {
                        $db->rollback();

                        enviaEmailChiavicatti($arquivo);

                        die('<script type="text/javascript">
								alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
								window.location.href = window.location.href; 
							 </script>');
                    }
                }

                $_POST['ocrqtdconstrucao'][$i] = ($_POST['ocrqtdconstrucao'][$i] ? $_POST['ocrqtdconstrucao'][$i] : 0);
                $_POST['ocrprazoexecucao'][$i] = ($_POST['ocrprazoexecucao'][$i] ? $_POST['ocrprazoexecucao'][$i] : 0);
                $_POST['ocrvalorexecucao'][$i] = ($_POST['ocrvalorexecucao'][$i] ? $_POST['ocrvalorexecucao'][$i] : 0);
                $_POST['ocrcustounitario'][$i] = ($_POST['ocrcustounitario'][$i] ? $_POST['ocrcustounitario'][$i] : 0);
                $_POST['ocrpercentualdbi'][$i] = ($_POST['ocrpercentualdbi'][$i] ? $_POST['ocrpercentualdbi'][$i] : 0);

                // término da execução como o término do contrato
                $_POST['ocrdtterminoexecucao'][$i] = $_POST['crtdttermino'];

                // término do contrato - Início da Execução:
                $dateIni = new DateTime(formata_data_sql_null($_POST['ocrdtinicioexecucao'][$i]));
                $dateTer = new DateTime(formata_data_sql_null($_POST['crtdttermino']));
                $_POST['ocrprazoexecucao'][$i] = $dateTer->diff($dateIni)->days;

                $arDados = array(
                    'crtid' => $crtid,
                    'obrid' => $_POST['obrid'][$i],
                    'umdid' => $_POST['umdid'][$i],
                    'arqidos' => ($arqidos ? $arqidos : null),
                    'arqidcusto' => ($arqidcusto ? $arqidcusto : null),
                    'ocrqtdconstrucao' => str_replace(array('.', ','), array('', '.'), (string) $_POST['ocrqtdconstrucao'][$i]),
                    'ocrdtordemservico' => formata_data_sql_null($_POST['ocrdtordemservico'][$i]),
                    'ocrdtinicioexecucao' => formata_data_sql_null($_POST['ocrdtinicioexecucao'][$i]),
                    'ocrprazoexecucao' => $_POST['ocrprazoexecucao'][$i],
                    'ocrdtterminoexecucao' => formata_data_sql_null($_POST['ocrdtterminoexecucao'][$i]),
                    'ocrvalorexecucao' => str_replace(array('.', ','), array('', '.'), (string) $_POST['ocrvalorexecucao'][$i]),
                    'ocrcustounitario' => str_replace(array('.', ','), array('', '.'), (string) $_POST['ocrcustounitario'][$i]),
                    'ocrpercentualdbi' => str_replace(array('.', ','), array('', '.'), (string) $_POST['ocrpercentualdbi'][$i])
                );
                $obraContrato->popularDadosObjeto($arDados)
                        ->salvar();
            }
        }
        $db->commit();

        $_SESSION['obras2']['crtid'] = $crtid;

        die("<script>
				alert('Operação realizada com sucesso!');" .
                ($obridSession ? "window.location = '?modulo=principal/exibeContrato&acao=A';" : "window.location = '?modulo=principal/cadContrato&acao=E';") . "
			 </script>");
    case 'downloadArquivo':
        $arqid = $_GET['arqid'];
        if ($arqid) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec(null, null, "obras2");
            $file->getDownloadArquivo($arqid);
        }
        die('<script>
				location.href=\'?modulo=principal/cadContrato&acao=E\';
			 </script>');
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$crthabil = 'S';
$crtDisabled = '';
if ($_GET['acao'] == 'E') {
    $crtid = ($_REQUEST['crtid'] ? $_REQUEST['crtid'] : $_SESSION['obras2']['crtid']);
    $contrato = new Contrato($crtid);
    $dados = $contrato->getDados();

    if ($dados['crtid']) {
        $_SESSION['obras2']['crtid'] = $crtid;
        $dados['crtdtassinatura'] = formata_data($dados['crtdtassinatura']);
        $dados['crtdttermino'] = formata_data($dados['crtdttermino']);
        $dados['crtvalorexecucao'] = number_format($dados['crtvalorexecucao'], 2, ',', '.');
        $dados['crtpercentualdbi'] = number_format($dados['crtpercentualdbi'], 2, ',', '.');

        extract($dados);

        $empresa = new Entidade($entidempresa);
        $entnomeempresa = $empresa->entnome;
        $entidempresa = $empresa->getPrimaryKey();

        if ($contrato->bloqueiaContrato($crtid)) {
            $crthabil = 'N';
            $crtDisabled = 'disabled="disabled"';
        }
    }
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arAba = ($_GET['acao'] == 'A' ? getArAba('cadastrocontrato') : getArAba('cadastrocontratoedicao'));
echo montarAbasArray($arAba, "?modulo=principal/cadContrato&acao=" . $_GET['acao']);

monta_titulo('Cadastro de Contrato', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>

<script type="text/javascript">
<!--
                            jQuery.noConflict();

                            function dataTerminoContrato() {
                                var dtAssinaturaContrato = jQuery('#crtdtassinatura').val(); //document.formulario.obrdtassinaturacontrato.value;
                                var diaPrazoVigenciaContrato = jQuery('#crtprazovigencia').val(); //document.formulario.obrprazovigencia.value;
                                var dtTerminoContrato = "";

                                if (dtAssinaturaContrato && diaPrazoVigenciaContrato) {
                                    var obDt = new Data();
                                    dtTerminoContrato = obDt.dtAddDia(dtAssinaturaContrato, diaPrazoVigenciaContrato);
                                }
                                jQuery('#crtdttermino').val(dtTerminoContrato);
                            }

                            function dataTerminoObra(obj) {
                                var table = jQuery(obj).parents('table')[0];

                                var dtInicio = jQuery(table).find('[id*=ocrdtinicioexecucao]').val();
                                var diaPrazo = jQuery(table).find('[id*=ocrprazoexecucao]').val();
                                var dtTermino = '';

                                if (dtInicio && diaPrazo) {
                                    var obDt = new Data();
                                    dtTermino = obDt.dtAddDia(dtInicio, diaPrazo);
                                }

                                jQuery(table).find('[id*=ocrdtterminoexecucao]').val(dtTermino);
                            }

                            function inserirEmpresa(entidempresa, tipo) {
                                if (entidempresa) {
                                    return windowOpen('?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entidempresa, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
                                } else {
                                    return windowOpen('?modulo=principal/inserir_empresa&acao=A&tipo=' + tipo, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
                                }
                            }

                            function preencheValor(obj) {
                                var calc = new Calculo();
                                var table = jQuery(obj).parents('table')[0];

                                var vlrExecucao = jQuery(table).find('#ocrvalorexecucao').val();
                                var qtdArea = jQuery(table).find('#ocrqtdconstrucao').val();
                                var custoUnit = jQuery(table).find('#ocrcustounitario');
                                var vlrCustoUnitario = '';

                                if (vlrExecucao != '' && qtdArea != '') {
                                    vlrCustoUnitario = mascaraglobal('###.###.###,##', calc.operacao(vlrExecucao, qtdArea, '/'));
                                }
                                custoUnit.val(vlrCustoUnitario);
                            }

                            function validarFormContrato() {
                                var msg = new Array();

                                if (jQuery.trim(jQuery('#licid').val()) == '') {
                                    msg.push('Licitação');
                                }

                                if (jQuery.trim(jQuery('#entidempresa').val()) == '') {
                                    msg.push('Empresa Contratada');
                                }

                                if (jQuery.trim(jQuery('#crtdtassinatura').val()) == '') {
                                    msg.push('Data de Assinatura do Contrato');
                                }

                                if (jQuery.trim(jQuery('#crtprazovigencia').val()) == '') {
                                    msg.push('Prazo de Vigência do Contrato (dias)');
                                }

                                if (jQuery.trim(jQuery('#crtdttermino').val()) == '') {
                                    msg.push('Data de término do contrato');
                                }

                                if (jQuery.trim(jQuery('#crtvalorexecucao').val()) == '0,00') {
                                    msg.push('Valor do Contrato(R$)');
                                }
                                if (jQuery.trim(jQuery('#crtpercentualdbi').val()) == '') {
                                    msg.push('Percentual BDI');
                                }
                                if (jQuery.trim(jQuery('input[name="ocrdtordemservico[\]"]').val()) == '') {
                                    msg.push('Data de assinatura da ordem de serviço');
                                }
                                if (jQuery.trim(jQuery('input[name="ocrdtinicioexecucao[\]"]').val()) == '') {
                                    msg.push('Início da Execução');
                                }
//                                if (jQuery.trim(jQuery('input[name="ocrprazoexecucao\[\]"]').val()) == '') {
//                                    msg.push('Prazo de Execução');
//                                }
                                if (jQuery.trim(jQuery('input[name="ocrvalorexecucao[\]"]').val()) == '0,00') {
                                    msg.push('Total da planilha contratada (R$)');
                                }
                                if (jQuery.trim(jQuery('input[name="ocrqtdconstrucao[\]"]').val()) == '') {
                                    msg.push('Área/Quantidade a ser Construída');
                                }
                                if (jQuery.trim(jQuery('input[name="ocrpercentualdbi[\]"]').val()) == '') {
                                    msg.push('Planilha de Custos');
                                }
<?php
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA):
    ?>
    //	if ( jQuery.trim( jQuery('[id=arqidcontrato]').val() ) == '' && jQuery.trim( jQuery('[id=arquivo]').val() ) == '' ){
    //		msg.push('Arquivo do Contrato');
    //	}

                                    jQuery('[id*=table_obra_]').each(function() {
                                        if (jQuery.trim(jQuery(this).find('[id*=arqidos]').val()) == '' && jQuery.trim(jQuery(this).find('[id*=arquivoos_]').val()) == '') {
                                            msg.push('Anexo da Ordem de Serviço');
                                        }
                                        if (jQuery.trim(jQuery(this).find('[id*=arqidcusto]').val()) == '' && jQuery.trim(jQuery(this).find('[id*=arquivocusto_]').val()) == '') {
                                            msg.push('Planilha de custos');
                                        }
                                    });
    <?php
endif;
?>
                                if (msg.length > 0) {
                                    msg = msg.join("\n");
                                    alert('O(s) campo(s) listado(s) abaixo são de preenchimento obrigatório:\n\n' + msg);
                                    return false;
                                }

                                var calc = new Calculo();
                                var vlrContrato = jQuery('#crtvalorexecucao').val();
                                var somaVlrObra = '0,00';
                                jQuery('[name*=ocrvalorexecucao]').each(function() {
                                    somaVlrObra = calc.operacao(somaVlrObra, jQuery(this).val(), '+');
                                    somaVlrObra = mascaraglobal('###.###.###,##', somaVlrObra);
                                    ;
                                });

                                if (calc.comparar(somaVlrObra, vlrContrato, '>')) {
                                    alert('A soma do Total da planilha contratada da Obra (Obras do contrato) não deve ser maior que o valor do contrato.');
                                    return false;
                                }

                                var data = new Data();
                                var dataIniContrato = jQuery('#crtdtassinatura').val();
                                var retorno = true;
                                jQuery('[name*=ocrdtinicioexecucao]').each(function() {
                                    obrDtIni = jQuery(this).val();
                                    if (data.comparaData(obrDtIni, dataIniContrato, '<')) {
                                        alert('Nas obras vinculadas ao contrato a data de início de execução da obra não deve ser menor que a data de assinatura do contrato.');
                                        jQuery(this).focus();
                                        retorno = false;
                                        return false;
                                    }
                                });
                                if (retorno == false)
                                    return false;

                                var dataFimContrato = jQuery('[name*=crtdttermino]').val();
                                var retorno = true;
                                jQuery('[name*=ocrdtterminoexecucao]').each(function() {
                                    obrDtFim = jQuery(this).val();
                                    if (data.comparaData(obrDtFim, dataFimContrato, '>')) {
                                        alert('Nas obras vinculadas ao contrato a data de término de execução da obra não deve ser maior que a data de término de execução do contrato.');
                                        jQuery(this).focus();
                                        retorno = false;
                                        return false;
                                    }
                                });
                                if (retorno == false)
                                    return false;

                                jQuery('#op').val('salvar');
                                jQuery('#formulario').submit();
                            }

                            function liberaLicitacao(licid) {
                                if (licid) {
                                    jQuery('#op').val('carregaLicitacao');
                                    jQuery('#formulario').submit();
//		jQuery('#tr_obra_contrato').show();
                                } else {
                                    jQuery('#tr_obra_contrato').hide();
                                    jQuery('[id*=table_obra_]').remove();
                                }
                            }
//-->
</script>

<form method="post" name="formulario" id="formulario" action="" enctype="multipart/form-data">
    <input type="hidden" name="op" id="op" value="">
    <input type="hidden" name="crtid" id="crtid" value="<?php echo $crtid; ?>">
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr>
            <td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">
                Contratação da Obra
                <br>
                (Para gravar qualquer alteração que se faça, clique em salvar)
            </td>
        </tr>
        <tr>
            <td width="265" class="subtitulodireita">Órgão:</td>
            <td>
<?php
$orgao = new Orgao();
echo $orgao->pegaDescricao($orgid);
?>
                <input type="hidden" name="orgid" id="orgid" value="<?php echo $orgid; ?>">
            </td>
        </tr>
                <?php
//		$crthabil = 'S';
                if ($ttaid):
                    $crthabil = 'N';
                    ?>
            <tr>
                <td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
                <td>
            <?php
            $tipoTermo = new TipoTermoAditivo($ttaid);
            echo $tipoTermo->ttadsc;
            ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Denominação:</td>
                <td>
                    <?php
                    echo $crtdenominacao;
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
                <td>
                    <?php
                    echo formata_data($crtdtassinaturaaditivo);
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Aditivo de Supressão:</td>
                <td>
                    <?php
                    echo ($crtsupressao == 't' ? 'Sim' : 'Não');
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Justificativa:</td>
                <td>
                    <?php
                    echo nl2br($crtjustificativa);
                    ?>
                </td>
            </tr>
    <?php
endif;
?>
        <tr>
            <td width="265" class="subtitulodireita">Licitação:</td>
            <td>
                <?php
                    $licitacao = new Licitacao();
                    $filtro = array('orgid' => $orgid,'obrid' => $obridSession);
                    $db->monta_combo('licid', $licitacao->listaCombo($filtro), $crthabil, "Selecione...", 'liberaLicitacao', '', '', '', 'S', 'licid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Empresa Contratada</td>
            <td>
                <span id="entnomeempresa"><?php echo $entnomeempresa; ?></span>
                <input type="hidden" name="entidempresa" id="entidempresa" value="<?php echo $entidempresa; ?>">
<?php
if ($crthabil == 'S'):
    ?>
                    <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEmpresa(document.getElementById('entidempresa').value);">
    <?php
endif;
?>
                <?php echo obrigatorio(); ?>

            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Data de Assinatura do Contrato:</td>
            <td><?= campo_data2('crtdtassinatura', 'S', $crthabil, 'Data de Assinatura do Contrato', '', '', 'dataTerminoContrato(); //verDataHomologadao();', $crtdtassinatura, '', '', 'crtdtassinatura'); ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
            <td><?= campo_texto('crtprazovigencia', 'S', $crthabil, '', 5, 10, '[#]', '', 'right', '', 0, 'id="crtprazovigencia" onchange="dataTerminoContrato();"', 'dataTerminoContrato();', $crtprazovigencia, ''); ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Data de término do contrato:</td>
            <td><?= campo_data2('crtdttermino', 'N', 'N', 'Data de término do contrato', '', '', '', $crtdttermino, '', '', 'crtdttermino'); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Valor do Contrato(R$):</td>
            <td><?= campo_texto('crtvalorexecucao', 'S', $crthabil, '', 20, 18, '[###.]###,##', '', 'right', '', 0, 'id="crtvalorexecucao"', '', $crtvalorexecucao, ''); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Percentual BDI:</td>
            <td><?= campo_texto('crtpercentualdbi', 'S', $crthabil, '', 10, 6, '[###.]###,##', '', 'right', '', 0, 'id="crtpercentualdbi"', '', $crtpercentualdbi, ''); ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
        </tr>

        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Contrato Digitalizado:</td>
            <td>
<?php
$arquivo = new Arquivo($arqidcontrato);
?>
                <input type="hidden" name="arqidcontrato" id="arqidcontrato" value="<?php echo $arquivo->arqid; ?>"/>
                <input type="file" name="arquivo" id="arquivo" <?= $crtDisabled ?>/>
                <br>
                <?php
//				$arquivo = new Arquivo( $arqidContrato );
                if ($arquivo->arqid) {
                    echo "<a href='?modulo=principal/cadContrato&acao=E&op=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                }
                ?>
            </td>
        </tr>

                <?php
                if ($arqid):
                    $arquivo = new Arquivo($arqid);
                    ?>
            <tr>
                <td class="SubTituloDireita">Anexo do Aditivo:</td>
                <td>
                    <a href="?modulo=principal/cadContrato&acao=E&op=downloadArquivo&arqid=<?php echo $arqid ?>">
                        &nbsp;&nbsp;
                        <img src="/imagens/anexo.gif">
            <?php echo $arquivo->arqnome . '.' . $arquivo->arqextensao ?>
                    </a>
                </td>
            </tr>
    <?php
endif;
?>		
        <tr>
            <td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">
                Obra(s)
            </td>
        </tr>
        <tr bgcolor="#FFFFFF" id="tr_obra_contrato" style="display: <?= $licid ? '' : 'none' ?>;">
            <td colspan="2" valign="top" id="td_obra_contrato">
                <div style="margin-bottom: 7px; <?= ($crthabil == 'N' ? 'display:none;' : '') ?>">
                    <a href="javascript:windowOpen( '?modulo=principal/vinculaObraContrato&acao=A&crtid=<?php echo $crtid; ?>&licid=' + jQuery('#licid').val(),'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );">[Vincular Obra(s) ao Contrato]</a>
                </div>
<?php
if ($crtid || $licid):
    $obraContrato = new ObrasContrato();
    $dadosObraContrato = $obraContrato->listaByContrato($crtid);

    if ($licid && empty($crtid)) {
        $dadosObraContrato = $obraLicitacao->listaObraByLicid($licid);
    }

    $i = 0;
    foreach ($dadosObraContrato as $dados):
        $umdid = $dados['umdid'];
        $ocrqtdconstrucao = number_format($dados['ocrqtdconstrucao'], 2, ',', '.');
        $ocrdtordemservico = formata_data($dados['ocrdtordemservico']);
        $ocrdtinicioexecucao = formata_data($dados['ocrdtinicioexecucao']);
        $ocrprazoexecucao = $dados['ocrprazoexecucao'];
        $ocrdtterminoexecucao = formata_data($dados['ocrdtterminoexecucao']);
        $ocrvalorexecucao = number_format($dados['ocrvalorexecucao'], 2, ',', '.');
        $ocrcustounitario = number_format($dados['ocrcustounitario'], 2, ',', '.');
        $ocrpercentualdbi = number_format($dados['ocrpercentualdbi'], 2, ',', '.');
        $obrid = $dados['obrid'];
        $obrnome = $dados['obrnome'];
        $arqidos = $dados['arqidos'];
        $arqidcusto = $dados['arqidcusto'];
//$ttaid = 2;
        if (empty($ttaid)) {
            $ocrHabilGeral = 'S';
            $ocrHabilPrazo = 'S';
            $ocrHabilValor = 'S';
        } elseif ($dados['ocraditivado'] != 't') {
            $ocrHabilGeral = 'N';
            $ocrHabilPrazo = 'N';
            $ocrHabilValor = 'N';
        } elseif ($ttaid == TIPO_ADV_PRAZO) {
            $ocrHabilGeral = 'N';
            $ocrHabilPrazo = 'S';
            $ocrHabilValor = 'N';
        } elseif ($ttaid == TIPO_ADV_VALOR) {
            $ocrHabilGeral = 'N';
            $ocrHabilPrazo = 'N';
            $ocrHabilValor = 'S';
        } elseif ($ttaid == TIPO_ADV_PRAZOVALOR) {
            $ocrHabilGeral = 'N';
            $ocrHabilPrazo = 'S';
            $ocrHabilValor = 'S';
        }

        if ($crthabil == 'N') {
            $ocrHabilGeral = 'N';
            $ocrHabilPrazo = 'N';
            $ocrHabilValor = 'N';
        }
//						$ocrHabilGeral 			= ($dados['ocraditivado'] == 't' ? 'S' : 'N');
//						$ocrHabilPrazo			= ();
//						$ocrHabilValor			= ();
        ?>
                        <table width="95%" cellspacing="0" cellpadding="2" border="1" align="center" class="listagem" id="table_obra_<?php echo $obrid ?>" style="margin-bottom: 10px;">
                            <tr>
                                <td class="subtitulodireita" width="20%">
                                    Obra:
                                    <input name="obrid[]" id="obrid" value="<?php echo $obrid ?>" type="hidden">
                                </td>
                                <td id="txt_obra" width="30%">
        <?php echo $obrid . ' - ' . $obrnome ?>
                                </td>
                                <td id="" width="50%" colspan="2">
        <?php
        if ($crtid):
            ?>
                                        <input type="button" value="Cronograma Atual" id="btn_cronograma_atual_<?= $obrid ?>" onclick="location.href = '?modulo=principal/etapas_da_obra&acao=A&obrid=<?= $obrid ?>';">
                                        <?php
                                    endif;
                                    ?>
                                    <?php
                                    if (!empty($ttaid)):
                                        ?>
                                        &nbsp;
                                        <input type="button" value="Histórico do Cronograma" id="btn_historico_cronograma_<?= $obrid ?>" onclick="janela('?modulo=principal/historicoCronograma&acao=A&obrid=<?= $obrid ?>', 800, 800, 'historicoCronograma')">
                                        <?php
                                    endif;
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subtitulodireita" width="20%">Data de assinatura da ordem de serviço:</td>
                                <td width="30%"><?= campo_data2('ocrdtordemservico[]', 'S', $ocrHabilGeral, 'Data de assinatura da ordem de serviço', '', '', '', $ocrdtordemservico, '', '', 'ocrdtordemservico_' . $obrid); ?></td>
                                <td class="SubTituloDireita" width="20%">Total da planilha contratada (R$):</td>
                                <td width="30%"><?= campo_texto('ocrvalorexecucao[]', 'S', $ocrHabilValor, '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrvalorexecucao"', 'preencheValor( this );', $ocrvalorexecucao, ''); ?></td>
                            </tr>
                            <tr>
                                <td class="subtitulodireita">Início da Execução:</td>
                                <td><?= campo_data2('ocrdtinicioexecucao[]', 'S', $ocrHabilPrazo, 'Início de Execução da Obra', '', '', 'dataTerminoObra(this);', $ocrdtinicioexecucao, '', '', 'ocrdtinicioexecucao_' . $obrid); ?></td>
                                <td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
                                <td>
        <?php
        $unidadeMedida = new UnidadeMedida();
        echo campo_texto('ocrqtdconstrucao[]', 'S', $ocrHabilValor, '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrqtdconstrucao"', 'preencheValor( this );', $ocrqtdconstrucao, '');
        echo '&nbsp;&nbsp;Unidade de Medida:&nbsp;' . $db->monta_combo("umdid[]", $unidadeMedida->listaCombo(), $ocrHabilValor, '', '', '', '', '100', 'N', '', true, $umdid);
        ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="subtitulodireita"></td>
                                <td></td>
                                <td class="SubTituloDireita">Custo Unitário R$:</td>
                                <td><?= campo_texto('ocrcustounitario[]', 'N', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrcustounitario"', '', $ocrcustounitario, ''); ?> (R$ / Unidade de Medida)</td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"></td>
                                <td></td>
                                <td class="SubTituloDireita">Percentual BDI:</td>
                                <td><?= campo_texto('ocrpercentualdbi[]', 'S', $ocrHabilValor, '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrpercentualdbi"', '', $ocrpercentualdbi, ''); ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
                            </tr>

                            <tr>
                                <td align='right' class="SubTituloDireita" style="vertical-align:top;">Ordem de Serviço:</td>
                                <td>
        <?php
        $arquivo = new Arquivo($arqidos);
        ?>
                                    <input type="hidden" name="arqidos[]" id="arqidos" value="<?php echo $arquivo->arqid; ?>"/>
                                    <input type="file" name="arquivoos_<?php echo $i ?>" id="arquivoos_<?php echo $i ?>" <?= $crtDisabled ?>/>
                                    <br>
                                    <?php
                                    if ($arquivo->arqid) {
                                        echo "<a href='?modulo=principal/cadContrato&acao=E&op=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                                    }
                                    ?>
                                </td>
                                <td align='right' class="SubTituloDireita" style="vertical-align:top;">Planilha de Custos:</td>
                                <td>
                                    <?php
                                    $arquivo = new Arquivo($arqidcusto);
                                    ?>
                                    <input type="hidden" name="arqidcusto[]" id="arqidcusto" value="<?php echo $arquivo->arqid; ?>"/>
                                    <input type="file" name="arquivocusto_<?php echo $i ?>" id="arquivocusto_<?php echo $i ?>" <?= $crtDisabled ?>/>
                                    <br>
                                    <?php
                                    if ($arquivo->arqid) {
                                        echo "<a href='?modulo=principal/cadContrato&acao=E&op=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                                    }
                                    ?>
                                </td>
                            </tr>

                        </table>				
                                    <?php
                                    $i++;
                                endforeach;
                            endif;
                            unset($umdid, $ocrqtdconstrucao, $ocrdtordemservico, $ocrdtinicioexecucao, $ocrprazoexecucao, $ocrdtterminoexecucao, $ocrvalorexecucao, $ocrcustounitario, $ocrpercentualdbi, $obrid, $obrnome);
                            ?>				
            </td>
        </tr>
        <tr bgcolor="#CCCCCC">
            <td align="left">
                <?php
                $docid = pegaDocidObra($_SESSION['obras2']['obrid']);
                $esdid = pegaEstadoObra($docid);
                if ($_REQUEST['acao'] == 'E' && $esdid == ESDID_OBJ_ADITIVO):
                    ?>
                    <a href="javascript: janela('?modulo=principal/popUpInserirAditivo&acao=A','1200','500','aditivo');">
                        <img border="0" src="/imagens/gif_inclui.gif">
                        Adicionar Aditivo ao Contrato
                    </a>
                    <?php
                endif;
                ?>
            </td>
            <td>
                <?php if ($obridSession): ?>
                        <input type="button" name="botao" value="Salvar e Voltar" onclick="validarFormContrato();" <?= $crtDisabled ?>/>
                        <input type="button" name="botao" value="Voltar" onclick="window.location = '?modulo=principal/exibeContrato&acao=A';"/>
                <?php else: ?>
                        <input type="button" name="btsalvar" value="Salvar" onclick="validarFormContrato()" class="botao" <?= $crtDisabled ?>>
                        <input type="button" name="botao" value="Voltar" id="voltar" onclick="window.location = '?modulo=principal/listaContrato&acao=A';"/>
                            <?php if ($_REQUEST['acao'] == 'E'): ?>
                                <input type="button" name="btnovo" value="Novo Contrato" onclick="location.href = '?modulo=principal/cadContrato&acao=A'" class="botao">
                            <?php endif; ?>
                <?php endif; ?>
            </td>
        </tr> 
    </table>
</form>

                <?php
                $objObras = new Obras($obrid);
                $blockEdicao = $objObras->verificaObraVinculada();
                if ($blockEdicao || $desabilita) {
                    echo '<script type="text/javascript">';
                    echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#voltar').attr('disabled', false);
                   }
                 ";
                    echo '</script>';
                }
                ?>

<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" id="vinculo_obra_modelo" style="display: none; margin-bottom: 10px;">
    <tr>
        <!--
                        <td class="subtitulodireita" width="20%">Empreendimento:</td>
                        <td id="txt_empreendimento" width="25%">
                        </td>
        -->		
        <td class="subtitulodireita" width="20%">
            Obra:
            <input name="obrid[]" id="obrid" value="" type="hidden">
        </td>
        <td id="txt_obra" width="35%" colspan="3">
        </td>
    </tr>
    <tr>
        <td class="subtitulodireita">Data de assinatura da ordem de serviço:</td>
        <td><?= campo_data2('ocrdtordemservico[]', 'S', 'S', 'Data de assinatura da ordem de serviço', '', '', '', $ocrdtordemservico, '', '', 'ocrdtordemservico'); ?></td>
        <td class="SubTituloDireita">Total da planilha contratada (R$):</td>
        <td><?= campo_texto('ocrvalorexecucao[]', 'S', 'S', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrvalorexecucao"', 'preencheValor( this );', $ocrvalorexecucao, ''); ?></td>
    </tr>
    <tr>
        <td class="subtitulodireita">Início da Execução:</td>
        <td><?= campo_data2('ocrdtinicioexecucao[]', 'S', 'S', 'Início da Execução', '', '', 'dataTerminoObra(this);', $ocrdtinicioexecucao, '', '', 'ocrdtinicioexecucao'); ?></td>
        <td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
        <td>
<?php
$unidadeMedida = new UnidadeMedida();
echo campo_texto('ocrqtdconstrucao[]', 'S', 'S', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrqtdconstrucao"', 'preencheValor( this );', $ocrcustounitario, '');
echo '&nbsp;&nbsp;Unidade de Medida:&nbsp;' . $db->monta_combo("umdid[]", $unidadeMedida->listaCombo(), 'S', '', '', '', '', '100', 'N', '', true);
?>
        </td>
    </tr>
    <tr>
        <td class="subtitulodireita"></td>
        <td></td>
        <td class="SubTituloDireita">Custo Unitário R$:</td>
        <td><?= campo_texto('ocrcustounitario[]', 'N', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrcustounitario"', '', $ocrcustounitario, ''); ?> (R$ / Unidade de Medida)</td>
    </tr>
    <tr>
        <td class="SubTituloDireita"></td>
        <td></td>
        <td class="SubTituloDireita">Percentual BDI:</td>
        <td><?= campo_texto('ocrpercentualdbi[]', 'S', 'S', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrpercentualdbi"', '', $ocrpercentualdbi, ''); ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Ordem de Serviço:</td>
        <td>
            <input type="hidden" name="arqidos[]" id="arqidos" value=""/>
            <input type="file" name="arquivoos" id="arquivoos"/>
            <br>
        </td>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Planilha de Custos:</td>
        <td>
            <input type="hidden" name="arqidcusto[]" id="arqidcusto" value=""/>
            <input type="file" name="arquivocusto" id="arquivocusto"/>
            <br>
        </td>
    </tr>
</table>