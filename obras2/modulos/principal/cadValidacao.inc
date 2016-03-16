<?php

if (empty($_SESSION['obras2']['obrid']) && !empty($_GET['obrid'])) {
    if (!empty($_GET['obrid'])) {
        $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    }
    if (empty($_SESSION['obras2']['empid'])) {
        $o = new Obras($_SESSION['obras2']['obrid']);
        $_SESSION['obras2']['empid'] = $o->empid;
    }
}

// empreendimento || obra || orgao
verificaSessao('obra');

$obrid = $_SESSION['obras2']['obrid'];

/**
 * Para as obras de convenio, todas devem possuir os mesmos dados na aba validação
 * Essa função garante que a alteração feita em uma obra de converio, todas as outras que compatilham o mesmo convenio
 * terão a validação igual.
 * @param $obrid
 * @param $arDado
 * @param $arCamposNulo
 */
function espelhaObrasConvenio($obrid, $arDado, $arCamposNulo)
{
    $validacao = new Validacao();
    $obra = new Obras($obrid);
    if ($obra->tooid == 2) {
        $obras = $obra->pegaObrasConvenio($obra->numconvenio, $obra->obranoconvenio);
        foreach ($obras as $obra) {
            $arDadoV = $arDado;
            $validacaoObra = $validacao->pegaValidacaoObra($obra['obrid']);
            if ($validacaoObra)
                $validacao = new Validacao($validacaoObra['vldid']);
            else
                $validacao = new Validacao();
            $arDadoV['obrid'] = $obra['obrid'];
            $validacao->popularDadosObjeto($arDadoV)->salvar(true, true, $arCamposNulo);
        }
        $validacao->commit();
    }
}

switch ($_REQUEST['requisicao']) {
    case "resetchecklist":
        unset($_SESSION['obras2']['checklistfnde']);
        die('
                  <script>
                    window.close();
                  </script>
                ');
        break;
    case "checklist":
        $chkFnde = new ChecklistFnde();
        $tipo = $_GET['tipo'];                                 // tec/adm/2p
        $obrid = $_SESSION['obras2']['obrid'];                  // ID obrid
        $ckfid = (!empty($_GET['ckfid'])) ? $_GET['ckfid'] : 0; // ID ckfid
        $acao = $_GET['acao'];                                 // duplicar/editar/cadastrar/excluir;
        $chkFnde->mostraPopupChecklistQuestionario($tipo, $obrid, $ckfid, $acao);
        die();
        break;
    case "salvar":
        $arCamposNulo = array();

        extract($_POST);

        $obra = new Obras($obrid);
        $percentual_de_comparacao = $obra->calculaPercentualValidacaoParcela();

        $habilita25 = ($percentual_de_comparacao >= '25') ? true : false;
        $habilita50 = ($percentual_de_comparacao >= '50') ? true : false;

        if (obraMi($obrid)) {
            $habilita25 = true;
            $habilita50 = true;
        }

        if ($vldstatushomologacao) {
            $vldstatushomologacao = $vldstatushomologacao;
            $usucpf_homo = $usucpf_homo ? $usucpf_homo : $_SESSION['usucpf'];
            $vlddtinclusaosthomo = $vlddtinclusaosthomo ? $vlddtinclusaosthomo : "now()";
        } else {
            $vldstatushomologacao = ' ';
            $usucpf_homo = null;
            $vlddtinclusaosthomo = null;
            $arCamposNulo[] = 'vldstatushomologacao';
            $arCamposNulo[] = 'usucpf_homo';
            $arCamposNulo[] = 'vlddtinclusaosthomo';
        }

        if ($vldstatus25exec && $habilita25) {
            $vldstatus25exec = $vldstatus25exec;
            $usucpf_25 = $usucpf_25 ? $usucpf_25 : $_SESSION['usucpf'];
            $vlddtinclusaost25exec = $vlddtinclusaost25exec ? $vlddtinclusaost25exec : "now()";
        } else {
            $vldstatus25exec = ' ';
            $usucpf_25 = null;
            $vlddtinclusaost25exec = null;
            $arCamposNulo[] = 'vldstatus25exec';
            $arCamposNulo[] = 'vlddtinclusaost25exec';
            $arCamposNulo[] = 'vlddtinclusaost25exec';
        }

        if ($vldstatus50exec && $habilita50) {
            $vldstatus50exec = $vldstatus50exec;
            $usucpf_50 = $usucpf_50 ? $usucpf_50 : $_SESSION['usucpf'];
            $vlddtinclusaost50exec = $vlddtinclusaost50exec ? $vlddtinclusaost50exec : "now()";
        } else {
            $vldstatus50exec = ' ';
            $usucpf_50 = null;
            $vlddtinclusaost50exec = null;
            $arCamposNulo[] = 'vldstatus50exec';
            $arCamposNulo[] = 'usucpf_50';
            $arCamposNulo[] = 'vlddtinclusaost50exec';
        }

        $validacao = new Validacao($_POST['vldid']);

        $arDado = array(
            'obrid' => $obrid,
            'usucpf_homo' => $usucpf_homo,
            'usucpf_25' => $usucpf_25,
            'usucpf_50' => $usucpf_50,
            'vldstatushomologacao' => $vldstatushomologacao,
            'vlddtinclusaosthomo' => $vlddtinclusaosthomo,
            'vldstatus25exec' => $vldstatus25exec,
            'vlddtinclusaost25exec' => $vlddtinclusaost25exec,
            'vldstatus50exec' => $vldstatus50exec,
            'vlddtinclusaost50exec' => $vlddtinclusaost50exec,
            'vldobshomologacao' => $vldobshomologacao,
            'vldobs25exec' => $vldobs25exec,
            'vldobs50exec' => $vldobs50exec
        );

        $validacao->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);

        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);

        $db->commit();
        $db->sucesso('principal/cadValidacao');
        break;
    case 'btnCancelaHmo':
        $validacao = new Validacao($_POST['vldid']);
        $arDado = array('vldstatushomologacao' => null, 'vldobshomologacao' => null);
        $arCamposNulo = array('vldstatushomologacao', 'vldobshomologacao');

        $validacao->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo);

        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);

        $db->commit();
        die('true');
        break;
    case 'btnCancela25':
        $validacao = new Validacao($_POST['vldid']);
        $arDado = array('vldstatus25exec' => null, 'vldobs25exec' => null);
        $arCamposNulo = array('vldstatus25exec', 'vldobs25exec');

        $validacao->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo);
        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);
        $db->commit();
        die('true');
        break;
    case 'btnCancela50':
        $validacao = new Validacao($_POST['vldid']);
        $arDado = array('vldstatus50exec' => null,
            'vldobs50exec' => null);
        $arCamposNulo = array('vldstatus50exec', 'vldobs50exec');

        $validacao->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo);
        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);
        $db->commit();
        die('true');
        break;
    case "download":
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
        die();
}

$habilitado = true;
$habilita = 'S';

if (possui_perfil(array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) {
    $habilitado = false;
    $habilita = 'N';
}
//retirando permissão de escrita para o perfil CALL CENTER
if (possui_perfil(PFLCOD_CALL_CENTER)){
    $semPermissao = 1;
}
include_once APPRAIZ . "includes/cabecalho.inc";

// Monta as abas e o título da tela
print "<br/>";
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
}
echo cabecalhoObra($obrid);
monta_titulo("Validação da Obra", "");

$validacao = new Validacao();
$arrFases = $validacao->pegaDadosComFaseLicitacao(array('obrid' => $obrid));
//dbg($arrFases, d);
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript">

        jQuery.noConflict();

        function DownloadArquivoFase(arqid) {
            window.location = '?modulo=principal/cadValidacao&acao=A&requisicao=download&arqid=' + arqid;
        }

        function salvarValidacao() {
            var form = document.getElementById('formulario');
            var msg = true;
            for (i = 0; i < form.length; i++) {
                if (formulario.elements[i].type == 'radio') {
                    if (formulario.elements[i].checked == true) {
                        msg = false;
                    }
                }
            }

            if (msg) {
                alert('Selecione uma validação!');
                return false;
            }
            if (jQuery('#vldstatushomologacao_n').attr('checked') && jQuery('#vldobshomologacao').val() == '') {
                alert('É obrigatório a justificativa da validação da homologação!');
                jQuery('#vldobshomologacao').focus();
                return false;
            }
            if (jQuery('#vldstatus25exec_n').attr('checked') && jQuery('#vldobs25exec').val() == '') {
                alert('É obrigatório a justificativa da <?=$label25?>!');
                jQuery('#vldobs25exec').focus();
                return false;
            }
            if (jQuery('#vldstatus50exec_n').attr('checked') && jQuery('#vldobs50exec').val() == '') {
                alert('É obrigatório a justificativa da <?=$label50?>!');
                jQuery('#vldobs50exec').focus();
                return false;
            }

            document.getElementById('requisicao').value = 'salvar';
            document.getElementById('formulario').submit();
        }

        function popupChecklist(tipo, ckfid, acao) {
            var url = "/obras2/obras2.php?modulo=principal/cadValidacao&acao=A";
            var requisicao = 'checklist';
            var obrid = <?php echo $_SESSION['obras2']['obrid']?>;
            if (acao.trim() === '') {
                acao = 'cadastro';
            }

            url = url + '&requisicao=' + requisicao + '&tipo=' + tipo + '&obrid=' + obrid + '&ckfid=' + ckfid + '&acao=' + acao;

            popup1 = window.open(
                url,
                "_blank",
                "width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
            );
            // Seta-se no atributo "notclose" o obj instanciado "popup1", a fim de que não se feche a popup
            popup1.opener.notclose = popup1;
        }


        function editarChecklist(tipo, ckfid) {
            var acao = 'editar';
            popupChecklist(tipo, ckfid, acao);
        }

        function duplicarChecklist(tipo, ckfid) {
            var acao = 'duplicar';
            popupChecklist(tipo, ckfid, acao);
        }

        function excluirChecklist(tipo, ckfid) {
            var acao = 'excluir';
            var r = window.confirm('Você realmente deseja excluir o Checklist ' + ckfid + ' ?');
            if (r) {
                popupChecklist(tipo, ckfid, acao);
                reload();
            }
        }

        function reload() {
            window.location.reload();
        }

        jQuery(function () {

            jQuery('#vldobshomologacao, #vldobs25exec, #vldobs50exec').parent().hide();

            // Validação da Homologação
            jQuery('#vldstatushomologacao_n').click(function () {
                jQuery('#vldobshomologacao').parent().show();
            });
            jQuery('#vldstatushomologacao_s').click(function () {
                jQuery('#vldobshomologacao').val('').parent().hide();
            });

            // Validação dos 25%
            jQuery('#vldstatus25exec_n').click(function () {
                jQuery('#vldobs25exec').parent().show();
            });
            jQuery('#vldstatus25exec_s').click(function () {
                jQuery('#vldobs25exec').val('').parent().hide();
            });

            // Validação dos 50%
            jQuery('#vldstatus50exec_n').click(function () {
                jQuery('#vldobs50exec').parent().show();
            });
            jQuery('#vldstatus50exec_s').click(function () {
                jQuery('#vldobs50exec').val('').parent().hide();
            });

            jQuery('.Cancelar').click(function () {
                id = this.id.replace('btnCancela', '');
                validacao = id == 'Hmo' ? 'Homologação' : id == '25' ? '<?=$label25?>' : '<?=$label50?>';
                if (confirm('Deseja cancelar a validação da ' + validacao + '?')) {
                    jQuery.ajax({
                        url: '?modulo=principal/cadValidacao&acao=A',
                        type: 'post',
                        data: 'requisicao=' + this.id + '&vldid=' + jQuery('input[name=vldid]').val(),
                        success: function (e) {
                            if (e == 'true')
                                document.location.href = '?modulo=principal/cadValidacao&acao=A';
                        }
                    });
                }
            });

            jQuery('#cad_chk_adm').click(function () {
                popupChecklist('adm', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_adm_sp').click(function () {
                popupChecklist('adm_sp', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_adm_2015').click(function () {
                popupChecklist('adm_2015', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_tec').click(function () {
                popupChecklist('tec', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_tec_2015').click(function () {
                popupChecklist('tec_2015', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_2p').click(function () {
                popupChecklist('2p', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_obr_vinc').click(function () {
                popupChecklist('obr_vinc', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_obr_mi').click(function () {
                popupChecklist('obr_mi', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_solicitacoes').click(function () {
                popupChecklist('solicitacoes', 0, acao = 'cadastro');
            });

        });

    </script>
    <table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <!-- Solicitação do Adonias para comentar o código no dia 28/03/2014 -->
            <!--<td style="width: 50%">
        <td style="width: 50%; height: 240px; vertical-align: top;">

            <table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="subtituloesquerda" colspan="2">Fase de Homologação</td>
                </tr>
                <tr>
                    <td class="subtitulodireita">Fase:</td>
                    <td>
                        <?php
            //                        $tfldesc = $arrFases['tfldesc'];
            //                        echo campo_texto('tfldesc', 'N', 'N', '', 80, 60, '', '', 'left', '', 0);
            ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Data:</td>
                    <td>
                        <?php // $flchomlicdtprev = $arrFases['flchomlicdtprev'] ?>
                        <?php //echo campo_data('flchomlicdtprev', 'N', 'N', '', 'S') ?>

                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Meio de Publicação:</td>
                    <td>
                        <?php //$flcmeiopublichomol = $arrFases['flcmeiopublichomol'] ?>
                        <?php //echo campo_texto('flcmeiopublichomol', 'N', 'N', '', 80, 60, '', '', 'left', '', 0); ?>			
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Observação:</td>
                    <td>
                        <?php //$flcobshomol = $arrFases['flcobshomol'] ?>
                        <?php //echo campo_textarea('flcobshomol', 'N', 'N', '', '84', '4', '500'); ?>			
                    </td>
                </tr>
            </table>
        </td> -->
            <td style="width: 50%; vertical-align: top; ">
                <div style=" height: 240px; max-height: 240px; overflow: auto;">
                    <table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
                        <tr>
                            <td class="subtituloesquerda" colspan="8">Checklist</td>
                        </tr>
                        <tr>
                            <td class="subtituloesquerda" colspan="8">

                            <span id="cad_chk_adm_sp" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist Administrativo Simplificado"
                                            title="Inserir Checklist Administrativo Simplificado" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist Administrativo Simplificado
                            </span>

<!--                            <span id="cad_chk_adm" style="color: #0000CC; cursor: pointer;">-->
<!--                                &nbsp; <img alt="Inserir Checklist Administrativo"-->
<!--                                            title="Inserir Checklist Administrativo" src="/imagens/gif_inclui.gif">-->
<!--                                &nbsp; Inserir Checklist Administrativo-->
<!--                            </span>-->

                            <span id="cad_chk_adm_2015" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist Administrativo"
                                            title="Inserir Checklist Administrativo" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist Administrativo
                            </span> 
<!--                            <span id="cad_chk_tec" style="color: #0000CC; cursor: pointer;"> -->
<!--                                &nbsp; <img alt="Inserir Checklist Técnico" title="Inserir Checklist Técnico"-->
<!--                                            src="/imagens/gif_inclui.gif">-->
<!--                                &nbsp; Inserir Checklist Técnico -->
<!--                            </span>-->

                                <span id="cad_chk_tec_2015" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist Técnico" title="Inserir Checklist Técnico"
                                            src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist Técnico
                            </span>
                            <span id="cad_chk_2p" style="color: #0000CC; cursor: pointer;"> 
                                &nbsp; <img alt="Inserir Checklist da 1° Parcela"
                                            title="Inserir Checklist  da 1° Parcela" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist da 1° Parcela
                            </span> 
                            <span id="cad_chk_obr_vinc" style="color: #0000CC; cursor: pointer;"> 
                                &nbsp; <img alt="Inserir Checklist de Obra Vinculada"
                                            title="Inserir Checklist de Obra Vinculada" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist de Obra Vinculada
                            </span>
                                <?php if (obraMi($obrid)) { ?>
                                    <span id="cad_chk_obr_mi" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist de Obra MI" title="Inserir Checklist de Obra MI"
                                            src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist de Obra MI
                                <?php } ?>
                            </span>

                            </td>
                        </tr>
                        <tr>
                            <td class="subtituloesquerda" style="width: 65px;"> Ação</td>
                            <td class="subtituloesquerda"> ID</td>
                            <td class="subtituloesquerda"> Tipo</td>
                            <td class="subtituloesquerda"> Responsável</td>
                            <td class="subtituloesquerda"> Data de Inclusão</td>
                            <td class="subtituloesquerda"> Situação</td>
                            <td class="subtituloesquerda"> Data da Situação</td>
                            <td class="subtituloesquerda"> Possui Pendências</td>
                        </tr>
                        <?php
                        $chkFnde = new ChecklistFnde();
                        $lista = $chkFnde->getListaCheck('lista');
                        $lista = $chkFnde->montaColunaPendenciasChkLst($lista);
                        if (!empty($lista)) {
                            $c = 0;
                            foreach ($lista as $key => $value) {
                                $c++;
                                switch ($value['queid']) {
                                    case QUEID_QUEST_CHKLST_ADM:
                                        $tipo = 'Administrativo';
                                        $tipo_abrev = 'adm';
                                        break;
                                    case QUEID_QUEST_CHKLST_ADM_SP:
                                        $tipo = 'Administrativo Simplificado';
                                        $tipo_abrev = 'adm_sp';
                                        break;
                                    case QUEID_QUEST_CHKLST_ADM_2015:
                                        $tipo = 'Administrativo';
                                        $tipo_abrev = 'adm_2015';
                                        break;
                                    case QUEID_QUEST_CHKLST_TEC:
                                        $tipo = 'Técnico';
                                        $tipo_abrev = 'tec';
                                        break;
                                    case QUEID_QUEST_CHKLST_TEC_2015:
                                        $tipo = 'Técnico';
                                        $tipo_abrev = 'tec_2015';
                                        break;
                                    case QUEID_QUEST_CHKLST_2P:
                                        $tipo = '1° Parcela';
                                        $tipo_abrev = '2p';
                                        break;
                                    case QUEID_QUEST_CHKLST_OBR_VINC:
                                        $tipo = 'Obra Vinculada';
                                        $tipo_abrev = 'obr_vinc';
                                        break;
                                    case QUEID_QUEST_CHKLST_OBR_MI:
                                        $tipo = 'Obra MI';
                                        $tipo_abrev = 'obr_mi';
                                        break;
                                    case QUEID_QUEST_CHKLST_SOLICITACOES:
                                        $tipo = 'Checklist Analise de Solicitação';
                                        $tipo_abrev = 'solicitacoes';
                                        break;
                                    default:
                                        $tipo = 'Checklist - Modelo Antigo';
                                        $tipo_abrev = 'old';
                                        break;
                                }
                                $data = (!empty($value['htddata'])) ? $value['htddata'] : $value['ckfdatainclusao'];
                                $iconExcluir = '<img src="/imagens/excluir.gif" title="Excluir Questionário"  onclick="javascript: excluirChecklist(\'' . $tipo_abrev . '\', ' . $value['ckfid'] . ')" style="cursor: pointer;" > <!-- Excluir -->';
                                if(possui_perfil(PFLCOD_CALL_CENTER)){
                                    $iconExcluir = '';
                                }
                                echo '<tr>';
                                echo '  <td> 
                                            <img src="/imagens/alterar.gif" title="Editar Questionário"   onclick="javascript: editarChecklist(\'' . $tipo_abrev . '\', ' . $value['ckfid'] . ')" style="cursor: pointer;"  > <!-- Editar -->
                                            <img src="/imagens/send.png"    title="Duplicar Questionário" onclick="javascript: duplicarChecklist(\'' . $tipo_abrev . '\', ' . $value['ckfid'] . ')" style="cursor: pointer;"> <!-- Duplicar -->
                                            '.$iconExcluir.'
                                          </td>';
                                echo '  <td> ' . $value['ckfid'] . ' </td>';
                                echo '  <td> ' . $tipo . ' </td>';
                                echo '  <td> ' . $value['usunome'] . ' </td>';
                                echo '  <td> ' . $value['ckfdatainclusao'] . ' </td>';
                                echo '  <td> ' . $value['esddsc'] . ' </td>';
                                echo '  <td> ' . $data . ' </td>';
                                echo '  <td> ' . $value['pendencias'] . ' </td>';
                                echo '</tr>';
                            }
                            echo '<tr> <td colspan="8" style="color: black; text-align: left; border-top: #000 2px solid"> <strong> Total de Registros: ' . $c . ' </strong> </td> </tr>';
                        } else {
                            echo '<tr> <td colspan="8" style="color: red; text-align: center;"> Não foram encontrados Registros. </td> </tr>';
                        }
                        ?>
                    </table>
                </div>
            </td>
        </tr>
    </table>
    <br>

<?php

$cabecalho = array(
    "Data Inclusão",
    "Nome Arquivo",
    "Tamanho (Mb)",
    "Descrição Arquivo",
    "Responsável");
$arquivoLicitacao = new ArquivoLicitacao();
$sql = $arquivoLicitacao->listaHomologadoSql(array('obrid' => $obrid));

$obrasContrato = new ObrasContrato();
$arqs = $obrasContrato->listaArqidOsPorObra($obrid);

$arqsHomologados = $db->carregar($sql);
$arqsHomologados = ($arqsHomologados ? $arqsHomologados : array());

if (isset($arqs[0])) {
    $arqsHomologados[] = $arqs[0];
}

echo '<table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloesquerda" style="text-align: center">Anexos da 2ª parcela</td>
        </tr>
      </table>
     ';
$db->monta_lista($arqsHomologados, $cabecalho, 50, 10, 'N', '', '');

/*
 * Regra alterada para habilitar a validação atraves do arquivo da ordem de serviço
 * $habilitaHo = (!empty($arrFases['tflid']) && $habilitado) ? '': 'disabled="disabled"';
 */

$obra = new Obras($obrid);

// Para Obras MI a regra para liberar é a empresa ter aceitado a OS
if ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) {
    $docid = pegaDocidObra($obrid);
    $acaoAceite = wf_acaoFoiExecutada($docid, ESDID_OBJ_AGUARDANDO_ACEITE_OS, ESDID_OBJ_EXECUCAO);
    $arquivoOS = ($acaoAceite) ? true : false;
} else {
    $arquivoOS = $db->pegaUm("SELECT DISTINCT arqidos FROM obras2.obrascontrato WHERE ocrstatus = 'A' AND obrid = $obrid AND arqidos IS NOT NULL");
}

// Calcula o Percentual de Validacao da Parcela com ajustes dos casos com obras vinculadas
$percentual_de_comparacao = $obra->calculaPercentualValidacaoParcela();
$habilitaHo = (!empty($arquivoOS) && $habilitado) ? '' : 'disabled="disabled"';
$habilita25 = ($percentual_de_comparacao >= '25' && $habilitado) ? '' : 'disabled="disabled"';
$habilita50 = ($percentual_de_comparacao >= '50' && $habilitado) ? '' : 'disabled="disabled"';

$label25 = 'Execução Física 25%';
$label50 = 'Execução Física 50%';

if ($obra->tooid == 2) {
    $habilita25 = '';
    $habilita50 = '';
}

// Quando MI, libera sem checar o percentul
if (obraMi($obrid)) {
    $habilitaHo = '';
    $habilita25 = '';
    $habilita50 = '';

    $label25 = 'Execução Física 3º Parcela';
    $label50 = 'Execução Física 4º Parcela ';
}

//Forma Antiga
//$habilitaHo = (!empty($arquivoOS) && $habilitado) ? '': 'disabled="disabled"';
//$habilita25 = ($arrFases['obrpercentultvistoria'] >= '25' && $habilitado) ? '' : 'disabled="disabled"';
//$habilita50 = ($arrFases['obrpercentultvistoria'] >= '50' && $habilitado) ? '' : 'disabled="disabled"';

$docid = pegaDocidObra($obrid);
$doc = wf_pegarDocumento($docid);
if ($doc['esdid'] == ESDID_OBJ_INACABADA) {
    $habilitado = false;
    $habilita = 'N';
    $habilitaHo = 'disabled="disabled"';
    $habilita25 = 'disabled="disabled"';
    $habilita50 = 'disabled="disabled"';
}

?>
    <form name="formulario" id="formulario" method="post">
    <input type="hidden" id="requisicao" name="requisicao" value="">
    <input type="hidden" id="obrpercentultvistoria" name="obrpercentultvistoria"
           value="<?= $arrFases['obrpercentultvistoria']; ?>">
    <input type="hidden" id="vldid" name="vldid" value="<?= $arrFases['vldid']; ?>">

    <table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

        <?php if ($arrFases['vldstatushomologacao'] == 'S') { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Ordem de serviço inserida em:</td>
                <td><?= $arrFases['vlddtinclusaosthomo1']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usuhomo']; ?></td>
            </tr>
            <tr>
                <td><input type="hidden" name="vldstatushomologacao" id="vldstatushomologacao"
                           value="<?= $arrFases['vldstatushomologacao']; ?>">
                    <input type="hidden" name="vlddtinclusaosthomo" id="vlddtinclusaosthomo"
                           value="<?= $arrFases['vlddtinclusaosthomo']; ?>">
                    <input type="hidden" name="usucpf_homo" id="usucpf_homo" value="<?= $arrFases['usucpf_homo']; ?>">
                </td>
                <td><input type="button" name="btnCancelaHmo" id="btnCancelaHmo" class="Cancelar" value="Cancelar"></td>
            </tr>
        <?php } else { ?>
            <tr>
                <td class="subtitulodireita" width="20%" valign="top">Documento de Ordem de Serviço Inserido?</td>
                <td>
                    <?php if ($arrFases['vldstatushomologacao'] == 'N'): ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobshomologacao').parent().show();
                            });
                        </script>
                    <?php else: ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobshomologacao').val('').parent().hide();
                            });
                        </script>
                    <?php endif; ?>
                    <input type="radio" name="vldstatushomologacao" id="vldstatushomologacao_s" <?= $habilitaHo; ?>
                           value="S" <?php echo $arrFases['vldstatushomologacao'] == 'S' ? 'checked' : '' ?>>Sim
                    <input type="radio" name="vldstatushomologacao" id="vldstatushomologacao_n" <?= $habilitaHo; ?>
                           value="N" <?php echo $arrFases['vldstatushomologacao'] == 'N' ? 'checked' : '' ?>>Não
                    <?php
                    $var = 'vldobshomologacao';
                    $obrig = 'N';
                    $habil = 'S';
                    $label = '';
                    $cols = 80;
                    $rows = 5;
                    $max = 5000;
                    $funcao = '';
                    $acao = 0;
                    $txtdica = '';
                    $tab = false;
                    $title = NULL;
                    $value = $arrFases['vldobshomologacao'];

                    echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value);
                    ?>
                </td>
            </tr>
        <?php } ?>
        <tr>
            <td>&nbsp;</td>
        </tr>
    </table>
    <table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloesquerda" colspan="2"><?= $label25 ?></td>
        </tr>
        <?php if ($arrFases['vldstatus25exec'] == 'S') { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Valor do percentual executado validado em:</td>
                <td><?= $arrFases['vlddtinclusaost25exec1']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu25']; ?></td>
            </tr>
            <tr>
                <td><input type="hidden" name="vldstatus25exec" id="vldstatus25exec"
                           value="<?= $arrFases['vldstatus25exec']; ?>">
                    <input type="hidden" name="vlddtinclusaost25exec" id="vlddtinclusaost25exec"
                           value="<?= $arrFases['vlddtinclusaost25exec']; ?>">
                    <input type="hidden" name="usucpf_25" id="usucpf_25" value="<?= $arrFases['usucpf_25']; ?>"></td>
                <td><input type="button" name="btnCancela25" id="btnCancela25" class="Cancelar" value="Cancelar"></td>
            </tr>
        <?php } else { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Validar Percentual Executado:</td>
                <td>
                    <input type="radio" name="vldstatus25exec" id="vldstatus25exec_s" <?php echo $habilita25; ?>
                           value="S" <?php echo $arrFases['vldstatus25exec'] == 'S' ? 'checked' : '' ?>>Sim
                    <input type="radio" name="vldstatus25exec" id="vldstatus25exec_n" <?php echo $habilita25; ?>
                           value="N" <?php echo $arrFases['vldstatus25exec'] == 'N' ? 'checked' : '' ?>>Não
                    <?php if ($arrFases['vldstatus25exec'] == 'N'): ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs25exec').parent().show();
                                jQuery('#responsavel_vldobs25exec').parent().show();
                            });
                        </script>
                    <?php else: ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs25exec').val('').parent().hide();
                                jQuery('#responsavel_vldobs25exec').val('').parent().hide();
                            });
                        </script>
                    <?php endif; ?>
                    <?php
                    $var = 'vldobs25exec';
                    $obrig = 'N';
                    $habil = 'S';
                    $label = '';
                    $cols = 80;
                    $rows = 5;
                    $max = 5000;
                    $funcao = '';
                    $acao = 0;
                    $txtdica = '';
                    $tab = false;
                    $title = NULL;
                    $value = $arrFases['vldobs25exec'];

                    echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value);
                    ?>
                </td>
            </tr>
            <tr>
                <td id="responsavel_vldobs25exec" class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu25']; ?></td>
            </tr>
        <?php } ?>
        <tr>
            <td>&nbsp;</td>
        </tr>
    </table>
    <table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloesquerda" colspan="2"><?= $label50 ?></td>
        </tr>
        <?php if ($arrFases['vldstatus50exec'] == 'S') { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Homologação Validade em:</td>
                <td><?= $arrFases['vlddtinclusaost50exec1']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu50']; ?></td>
            </tr>
            <tr>
                <td><input type="hidden" name="vldstatus50exec" id="vldstatus50exec"
                           value="<?= $arrFases['vldstatus50exec']; ?>">
                    <input type="hidden" name="vlddtinclusaost50exec" id="vlddtinclusaost50exec"
                           value="<?= $arrFases['vlddtinclusaost50exec']; ?>">
                    <input type="hidden" name="usucpf_50" id="usucpf_50" value="<?= $arrFases['usucpf_50']; ?>"></td>
                <td><input type="button" name="btnCancela50" id="btnCancela50" class="Cancelar" value="Cancelar"></td>
            </tr>
        <?php } else { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Validar Percentual Executado:</td>
                <td>
                    <input type="radio" name="vldstatus50exec" id="vldstatus50exec_s" <?php echo $habilita50; ?>
                           value="S" <?php echo $arrFases['vldstatus50exec'] == 'S' ? 'checked' : '' ?>>Sim
                    <input type="radio" name="vldstatus50exec" id="vldstatus50exec_n" <?php echo $habilita50; ?>
                           value="N" <?php echo $arrFases['vldstatus50exec'] == 'N' ? 'checked' : '' ?>>Não
                    <?php if ($arrFases['vldstatus50exec'] == 'N'): ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs50exec').parent().show();
                                jQuery('#responsavel_vldobs50exec').parent().show();
                            });
                        </script>
                    <?php else: ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs50exec').val('').parent().hide();
                                jQuery('#responsavel_vldobs50exec').val('').parent().hide();
                            });
                        </script>
                    <?php endif; ?>
                    <?php
                    $var = 'vldobs50exec';
                    $obrig = 'N';
                    $habil = 'S';
                    $label = '';
                    $cols = 80;
                    $rows = 5;
                    $max = 5000;
                    $funcao = '';
                    $acao = 0;
                    $txtdica = '';
                    $tab = false;
                    $title = NULL;
                    $value = $arrFases['vldobs50exec'];

                    echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value);
                    ?>
                </td>
            </tr>
            <tr>
                <td id="responsavel_vldobs50exec" class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu50']; ?></td>
            </tr>
        <?php } ?>
        <tr bgcolor="#C0C0C0">
            <td class="subtitulocentro" colspan="2">
                <?php if(empty($semPermissao)){
                        if (empty($habilitaHo) || empty($habilita25) || empty($habilita50)) { ?>
                            <input type="button" value="Salvar" onclick="salvarValidacao();">
                <?php   } 
                      }else { ?>
                        <input type="button" value="Salvar" disabled="disabled">
                <?php } ?>
            </td>
        </tr>
    </table>
    </form>


<?php
$objObras = new Obras($obrid);
$blockEdicao = $objObras->verificaObraVinculada();
if ($blockEdicao) {
    echo '<script type="text/javascript">';
    echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
    echo '</script>';
}
?>