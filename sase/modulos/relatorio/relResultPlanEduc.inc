<?php
ob_start();

global $db;

// --------------- Dependências
include_once '../../sase/classes/AvaliadorEducacional.class.inc';
include_once '../../sase/classes/RelatorioAvaliadorRedeae.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
//include APPRAIZ . 'includes/funcoes.inc';
//include APPRAIZ . 'www/sase/_funcoes.php';
// --

$meses = array(
    '01' => 'Janeiro',
    '02' => 'Fevereiro',
    '03' => 'Março',
    '04' => 'Abril',
    '05' => 'Maio',
    '06' => 'Junho',
    '07' => 'Julho',
    '08' => 'Agosto',
    '09' => 'Setembro',
    '10' => 'Outubro',
    '11' => 'Novembro',
    '12' => 'Dezembro'
);

$usucpf = $_SESSION['usucpf'];
//$usucpf = '32096208734'; // Tania - Técnica
//$usucpf = '03041053764'; // André - Supervisor Geral
//$usucpf = '';

$avaliadoreducacional = new AvaliadorEducacional();

$rel = new RelatorioAvaliadorRedeae();

$rel->arAtributos['usucpf'] = $usucpf;

$data = new Data();

$pfls = arrayPerfil();
$pflcod = 'ok';
if (is_array($pfls)) {
    if (in_array(PFLCOD_SASE_TECNICO, $pfls)) {
        $pflcod = PFLCOD_SASE_TECNICO;
    } else if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)) {
        $pflcod = PFLCOD_SASE_SUPERVISOR;
    }
}
//$pflcod = PFLCOD_SASE_TECNICO;

$titulo = 'Registro dos Resultados';

$ravid = $_GET['ravid'];

if ($ravid != '') {
    $rel->carregarPorId($ravid);
    //ver($rel, d);
    $perCampos = $rel->retornaDisCampos($_SESSION['usucpf']);

    $dados = $rel->getDados();
    $usucpf = $rel->arAtributos['usucpf'];
    $ratperiodo = $rel->arAtributos['ratperiodo'];
    $ratdata1periodo = $data->formataData($rel->arAtributos['ratdata1periodo']);
    $ratdata2periodo = $data->formataData($rel->arAtributos['ratdata2periodo']);
    $ratdata2periodo2 = str_replace(" 00:00:00", "", $rel->arAtributos['ratdata2periodo']);
    $pflcod = $rel->arAtributos['pflcod'];
}


switch ($_POST['acao']) {
    case 'carregar':
        if ($pflcod != '') {
            if ($rel->validaRelatorio()) {
                $ratdata1periodo = $_POST['ratdata1periodo'];
                $ratdata2periodo = $_POST['ratdata2periodo'];
                $ratperiodo = $_POST['ratperiodo'];
                if ($ratperiodo == '') {
                    echo "
					<script>
						alert('Informe o período do relatório.');
						window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A';
					</script>";
                    exit;
                }

                if ($ratdata1periodo == '' || $ratdata2periodo == '' || $ratdata1periodo == '//' || $ratdata2periodo == '//') {
                    echo "
					<script>
						alert('Informe o período em data.');
						window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A';
					</script>";
                    exit;
                }

                $ravid = $rel->salvarRelatorio();

                if ($ravid) {
                    //$rel->salvaMunicipiosAssistidos();
                    echo "
					<script>
						alert('Relatório criado com sucesso.');
						window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
					</script>";
                    exit;
                } else {
                    echo "
					<script>
						alert('{$rel->msg}');
						window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A';
					</script>";
                }
            } else {
                echo "
					<script>
						alert('" . $rel->msg . "');
						window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A';
					</script>";
            }
        } else {
            echo "
					<script>
						alert('Perfil do usuário não possui os dados necessários para o relatório.');
						window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A';
					</script>";
        }
        break;

    case 'salvar_atividades':

        if ($rel->salvaAtividades()) {
            echo "
			<script>
				alert('Atividades Desenvolvidas salvas com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        } else {
            if ($rel->msg != '') {
                $msg = $rel->msg;
                echo "
                    <script>
                        alert('" . $msg . "');
                        window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                    </script>";
                exit;
            } else {
                echo "
                    <script>
                        alert('Erro ao salvar as atividades desenvolvidas.');
                        window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                    </script>";
                exit;
            }
        }

        break;

    case 'salvar_resultados_consolidados':
        if ($rel->salvaResultadosConsolidades()) {
            echo "
			<script>
				alert('Resultados Consolidados salvos com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
        } else {
            if ($rel->msg != '') {
                echo "
                <script>
                    alert('" . $rel->msg . "');
                    window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                </script>";
                exit;
            } else {
                echo "
                <script>
                    alert('Erro ao salvar os Resultados Consolidados.');
                    window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                </script>";
                exit;
            }
        }
        break;

    case 'salvar_municipio_com_alteracao':
        if ($rel->salvaMunicipiosComAlteracao()) {
            echo "
			<script>
				alert('Municípios Assistidos com Alteração salvos com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        } else {
            echo "
			<script>
				alert('Erro ao salvar os Municípios Assistidos com Alteração.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        }
        break;

    case 'salvar_municipio_sem_alteracao':
        if ($rel->salvarMunicipiosSemAlteracao()) {
            echo "
			<script>
				alert('Municípios Assistidos sem Alteração salvos com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        } else {
            echo "
			<script>
				alert('Erro ao salvar os Municípios Assistidos sem Alteração.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        }
        break;

    case 'salvar_municipio_sem_informacao':
        if ($rel->savlarMunicipiosSemInformacao()) {
            echo "
			<script>
				alert('Municípios Assistidos sem Alteração salvos com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        } else {
            echo "
			<script>
				alert('Erro ao salvar os Municípios Assistidos sem Alteração.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        }
        break;

    case 'salvar_q6':
        if ($rel->salvaQuadro6()) {
            echo <<<HTML
			<script>
				alert('Ações Propostas salvas com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>
HTML;
            exit;
        } else {
            if ($rel->msg != '') {
                echo <<<HTML
                <script>
                    alert('{$rel->msg}');
                    window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                </script>
HTML;
            } else {
                echo <<<HTML
                <script>
                    alert('Erro ao salvar as Ações Propostas.');
                    window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                </script>
HTML;
                exit;
            }
        }
        break;

    case 'salvar_informacao_complementar_q6':
        if ($rel->salvaInformacoesComplementaresQ6()) {
            echo "
			<script>
				alert('Comentários Gerais salvos com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        } else {
            if ($rel->msg != '') {
                echo "
                    <script>
                        alert('{$rel->msg}');
                        window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                    </script>";
                exit;
            } else {
                echo "
                    <script>
                        alert('Erro ao salvar os Comentários Gerais.');
                        window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                    </script>";
                exit;
            }
        }
        break;

    case 'salvar_informacao_complementar':

        if ($rel->salvaInformacoesComplementares()) {
            echo "
			<script>
				alert('Informações Complementares salvas com sucesso.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        } else {
            if ($rel->msg != '') {
                echo "
                    <script>
                        alert('{$rel->msg}');
                        window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                    </script>";
                exit;
            } else {
                echo "
                    <script>
                        alert('Erro ao salvar as Informações Complementares.');
                        window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                    </script>";
                exit;
            }
        }
        break;

    case 'gera_relatorio':
        ob_clean();

        $html = $rel->geraRelatorio();
        $nome = $rel->pegaAvaliador();

        $content = http_build_query(array(
            'conteudoHtml' => utf8_encode($html)
        ));

        $context = stream_context_create(array(
            'http' => array(
                'method' => 'POST',
                'content' => $content
            )
        ));

        $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
        header('Content-Type: application/pdf');
        header('Content-Disposition: attachment; filename=Relatorio_AE_' . $nome . '_' . $_REQUEST['ravid'] . '.pdf');
        echo $contents;
        exit();
        break;

    case 'apaga_registro_quadro6':
        if (!$rel->apagaRegQuadro6($_REQUEST['msaid'])) {
            echo "
			<script>
				alert('Erro ao apagar a ação proposta.');
				window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
			</script>";
            exit;
        }
        break;

    case 'carregar_municipios_assistidos':
        if ($pflcod) {
            switch ($pflcod) {
                case PFLCOD_SASE_TECNICO:
                    $rel->salvaMunicipiosAssistidos();
                    break;

                case PFLCOD_SASE_SUPERVISOR:
                    $rel->salvaMunicipiosAssistidos();
                    $rel->salvaMunicipiosAssistidosPorTecnicos();
                    break;

                default:
                    $rel->salvaMunicipiosAssistidos();
                    break;
            }
        } else {
            echo "
                <script>
                    alert('Perfil não selecionado.');
                    window.location.href = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}';
                </script>";
            exit;
        }
        break;
}

include APPRAIZ . 'includes/cabecalho.inc';

$prsano = trim($_SESSION['exercicio']);
?>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>
    function salvar(request, quadro, esdid, msg, pflcod) {
        quadro = typeof quadro !== 'undefined' ? quadro : '';
        esdid = typeof esdid !== 'undefined' ? esdid : '';
        msg = typeof msg !== 'undefined' ? msg : '';
        pflcod = typeof pflcod !== 'undefined' ? pflcod : '';

        if (request !== 'undefined') {
            jQuery('[name=acao]').val(request);
            jQuery('[name=quadro]').val(quadro);
            jQuery('[name=esdid]').val(esdid);
            jQuery('[name=msg]').val(msg);

            if (request == 'salvar_informacao_complementar') {
                jQuery('[name=ravinfcomplementar2]').val(document.formCadastroLista.ravinfcomplementar.value);
            }

            jQuery('[name=formCadastroLista]').submit();
        } else {
            alert('Ação não informada.');
        }

        ravid = jQuery('[name=ravid]').val();
//console.log(jQuery('[name=formAtivDesen]').serializeArray());

//        var a = Array();
//        var b = Array();
//        var f = jQuery('[name=formAtivDesen]').serializeArray();
//        console.log(f);
//        $.each(f, function() {
//            //console.log(a[this.name.replace('[]','')]);
//            if (a[this.name.replace('[]','')] !== undefined){
//                a[this.name.replace('[]','')].push(this.value || '');
//            } else {
//                a[this.name.replace('[]','')] = Array();
//                a[this.name.replace('[]','')].push(this.value || '');
//            }
//        });
//console.log(a);

//        jQuery.ajax({
//            url: 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&ravid={$ravid}',
//            type: 'POST',
//            data: {
//                acao: request,
//                quadro: quadro,
//                esdid: esdid,
//                msg: msg,
//
//            }
//        });

    }

    function delQuadro6(msaid) {
        if (msaid != '') {
            jQuery('[name=acao]').val('apaga_registro_quadro6');
            jQuery('[name=msaid]').val(msaid);
            jQuery('[name=formCadastroLista]').submit();
        } else {
            console.error('Código do município, msaid, não informado.');
        }
    }

    function delMunicipioAssistido() {
        if (confirm("Esta ação apagará os municípios assistidos, que não possuem associação com o avaliador educacional, e incluirá os municípios associados que ainda não são contemplados pelo relatório. Deseja continuar?")) {
            jQuery('[name=acao]').val('carregar_municipios_assistidos');
            jQuery('[name=formCadastroLista]').submit();
        }
    }

    function montaPeriodo() {

        switch (jQuery('#ratperiodo').val()) {
            case '1':
                jQuery('#ratdata1periodo').val('01/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val('15/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
            case '2':
                jQuery('#ratdata1periodo').val('16/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val((new Date(2015, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
            case '3':
                jQuery('#ratdata1periodo').val('01/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val((new Date(2015, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
        }
        // Informa, no console do browser, o período em data referente ao Período e Mês slecionados.
        //console.info('Período em data: '+jQuery('#ratdata1periodo').val()+' à '+jQuery('#ratdata2periodo').val());
    }

</script>

<form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">

    <input type="hidden" id="requisicao" name="requisicao" value="" />
    <input name="formulario" value="1" type="hidden"/>
    <input name="acao" value="carregar" type="hidden"/>
    <input name="aveid" value="<?= @$_GET['aveid'] ?>" type="hidden"/>
    <input name="ravid" value="<?= @$_GET['ravid'] ?>" type="hidden"/>
    <input name="usucpf" value="<?= @$usucpf ?>" type="hidden"/>
    <input name="quadro" value="" type="hidden"/>
    <input name="esdid" value="" type="hidden"/>
    <input name="msaid" value="" type="hidden"/>
    <input name="msg" value="" type="hidden"/>
    <input name="ravinfcomplementar2" value="" type="hidden"/>


    <div id='cabecalhoSistema'>
        <center><h3><?= $titulo ?></h3></center>
    </div>

    <!-- html -->
    <div id="container">
        <div class="row">
            <div class="row col-md-12">

                <div class="row col-md-11">
                    <div class="well">
                        <fieldset>
                            <legend>1. Informações gerais do relatório</legend>

                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Avaliador Educacional</label>
                                <div class="col-lg-8 col-md-8">
                                    <label style="font-weight: normal; margin-top: 8px;">
<?php
echo $rel->pegaAvaliador();
?>
                                    </label>
                                </div>
                            </div>
<?php if ($ratdata1periodo != '' && $ratdata2periodo != '') { ?>
                                <div class="form-group">
                                    <label class="col-lg-4 col-md-4 control-label">Período em Data</label>
                                    <div class="col-lg-8 col-md-8">
                                        <label style="font-weight: normal; margin-top: 8px;">
    <?php
    echo $ratdata1periodo . ' à ' . $ratdata2periodo;
    ?>
                                        </label>
                                    </div>
                                </div>
<?php } ?>
<?php if ($pflcod != PFLCOD_SASE_TECNICO) { ?>
                                <div class="form-group">
                                    <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Perfil</label>
                                    <div class="col-lg-8 col-md-8">
    <?php
    $sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflcod in (" . PFLCOD_SASE_TECNICO . "," . PFLCOD_SASE_SUPERVISOR . "," . PFLCOD_SASE_SUPERVISOR_GERAL . "," . PFLCOD_SASE_EXECUTIVO . PFLCOD_SASE_EXECUTIVO . ") order by pfldsc";
    $dados = $db->carregar($sql);
    echo getoptions($dados, array('prompt' => ' Selecione...'), 'pflcod', null, $pflcod);
    ?>
                                    </div>
                                </div>
                                    <?php } else { ?>
                                <input name="pflcod" value="<?= @$pflcod ?>" type="hidden"/>
                            <?php } ?>

                            <div class="form-group">
                                <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label>

                                <div class="col-lg-8 col-md-8 ">
                                    <select class="form-control chosen" id="ratperiodo" name="ratperiodo" onchange="montaPeriodo()">
                                        <option value="">Selecione...</option>
                                        <option value="1" <?= $ratperiodo == 1 ? 'selected="true"' : '' ?>>1° Quinzena</option>
                                        <option value="2" <?= $ratperiodo == 2 ? 'selected="true"' : '' ?>>2° Quinzena</option>
                                        <option value="3" <?= $ratperiodo == 3 ? 'selected="true"' : '' ?>>Mensal</option>
                                    </select>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Mês:</label>

                                <div class="col-lg-8 col-md-8 ">
                                    <input type="hidden" id="ratdata1periodo" name="ratdata1periodo" value="<?= $ratdata1periodo ?>"/>
                                    <input type="hidden" id="ratdata2periodo" name="ratdata2periodo" value="<?= $ratdata2periodo ?>"/>
                                    <select class="form-control chosen" id="mes" name="mes" onchange="montaPeriodo()">
<?php
foreach ($meses as $c => $v) {
    if ($ratdata2periodo != '') {
        $mr = explode("/", $ratdata2periodo);
        $s = $mr[1] == $c ? 'selected="true"' : '';
    } else {
        $s = $c == date('m') ? 'selected="true"' : '';
    }

    echo <<<HTML
                                                <option {$s} value="{$c}">$v</option>
HTML;
}
?>
                                    </select>
                                </div>
                            </div>
<?php
if ($ravid != '') {
    $sqlLiberaBotao = "select esddsc
           
                from sase.relatorioavaliadorredeae rav
                inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                left join workflow.documento doc on rav.docid = doc.docid
                left join workflow.estadodocumento esd on esd.esdid = doc.esdid
               where ravid = '{$ravid}'";

    $estadoRegistro = $db->carregar($sqlLiberaBotao);
    $habilitadoBGR = "";
    $perfis = pegaPerfil($_SESSION['usucpf']);
//if (in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis)) {
    if ($perfis[0][pflcod] == PFLCOD_SASE_ADMINISTRADOR || $perfis[0][pflcod] == PFLCOD_SASE_SUPER_USUARIO) {
        $habilitadoBGR = "";
    } else
        if ($estadoRegistro[0][esddsc] != 'Relatório Finalizado') {
            $habilitadoBGR = "disabled = 'disabled'";
        }
} else {
    $habilitadoBGR = "disabled = 'disabled'";
}
?>
                            <div class="text-right">
                            <?php if (empty($ravid)) { ?>
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('carregar', null, null, null, $('#pflcod').val())">Carregar
                                    </button>
                                <?php } ?>
<?php if (!empty($ravid)) { ?>
                                    <button title="Gerar Relatorio" <?php echo $habilitadoBGR; ?> class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('gera_relatorio')">Gerar Relatório
                                    </button>
                                <?php } ?>
                            </div>

                        </fieldset>
                    </div>
                </div>
<?php if (!empty($ravid)) { ?>
                    <div class="col-md-1">
                    <?php
                    if (!$rel->docid && $_GET['ravid']) {
                        //$docid = wf_cadastrarDocumento( TPDID_SASE_AVALIADOREDUCACIONAL, 'Documento Avaliador Educacional' );
                        $docid = cadastraDocumentoAE($pflcod, 'Documento Avaliador Educacional');
                        $rel->docid = $docid;
                        // ver($Assessoramento,d);
                        $rel->alterar();
                        $rel->commit();
                    }

                    if ($_GET['ravid'] && $rel->docid)
                        wf_desenhaBarraNavegacao($rel->docid, array('docid' => $rel->docid, 'ravid' => $rel->ravid, 'usucpf' => $rel->arAtributos['usucpf'], 'ratdata1periodo' => $data->formataData($rel->arAtributos['ratdata1periodo']), 'ratdata2periodo' => $data->formataData($rel->arAtributos['ratdata2periodo'])));
                    ?>

                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <?php
                                switch ($pflcod) {
                                    case PFLCOD_SASE_TECNICO:
                                        $t2 = "Municípios Assistidos";
                                        break;
                                    case PFLCOD_SASE_SUPERVISOR:
                                        $t2 = "Técnicos Supervisionados";
                                        break;
                                    default:
                                        $t2 = "Municípios Assistidos";
                                        break;
                                }
                                ?>
                                <legend>2. <?php echo $t2; ?></legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php
                                        switch ($pflcod) {
                                            case PFLCOD_SASE_TECNICO:
                                                echo $rel->getListaMunicipiosAssistidos(true);
                                                break;
                                            case PFLCOD_SASE_SUPERVISOR:
                                                echo $rel->getListaTecnicosSupervisionados(true);
                                                break;
                                            default:
                                                echo $rel->getListaMunicipiosAssistidos();
                                                break;
                                        }
                                        ?>
                                    </div>
                                </div>
                                <div class="text-right">
                                    <button title="Carregar" class="btn btn-success" type="button" id="btnPesquisar" onclick="delMunicipioAssistido()">Carregar
                                    </button>
                                </div>
                            </fieldset>
                        </div>
                    </div>

                    <!--form id="form-save" method="post" name="formAtivDesen" role="form" class="form-horizontal"-->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>3. Atividades Desenvolvidas</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <?php echo $rel->getListaAtividadesDesenvolvidas($perCampos, true, $pflcod); ?>
                                    </div>

                                    <?php if ($pflcod != PFLCOD_SASE_TECNICO) { ?>
                                        <label for="dmdprazo" class="col-lg-12 col-md-12 control-label" style="text-align: left !important; margin-top: 10px;">Comentários gerais sobre avanços e dificuldades relacionados às atividades realizadas no período:</label>
                                        <div class="col-lg-12 col-md-12">
                                            <textarea class="form-control" maxlength="200" id="ravinfcomplqdo1" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplqdo1" rows="5"><?= $rel->arAtributos['ravinfcomplqdo1'] ?></textarea>
                                        </div>
                                    <?php } ?>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_atividades')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>4. Resultados Consolidados Alcançados</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <?php echo $rel->getListaResultadosConsolidados($perCampos); ?>
                                    </div>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_resultados_consolidados')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>5. Municípios assistidos com alteração</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <?php echo $rel->getMunicipiosComAlteracaoNaData($perCampos); ?>
                                    </div>

                                    <?php if ($pflcod != PFLCOD_SASE_TECNICO) { ?>
                                        <label for="dmdprazo" class="col-lg-12 col-md-12 control-label" style="text-align: left !important; margin-top: 10px;">Comentários gerais sobre avanços e dificuldades relacionados à alteração das etapas de trabalho pelos Municípios no período:</label>
                                        <div class="col-lg-12 col-md-12">
                                            <textarea class="form-control" maxlength="200" id="ravinfcomplqdo3" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplqdo3" rows="5"><?= $rel->arAtributos['ravinfcomplqdo3'] ?></textarea>
                                        </div>
                                    <?php } ?>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_municipio_com_alteracao')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>6. Municípios assistidos sem alteração de etapa</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <?php echo $rel->getMunicipioSemAlteracao($perCampos, true); ?>
                                    </div>

                                    <?php if ($pflcod != PFLCOD_SASE_TECNICO) { ?>
                                        <label for="dmdprazo" class="col-lg-12 col-md-12 control-label" style="text-align: left !important; margin-top: 10px;">Comentários gerais sobre avanços e dificuldades relacionados à permanência dos Municípios na mesma etapa de trabalho no período:</label>
                                        <div class="col-lg-12 col-md-12">
                                            <textarea class="form-control" maxlength="200" id="ravinfcomplqdo4" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplqdo4" rows="5"><?= $rel->arAtributos['ravinfcomplqdo4'] ?></textarea>
                                        </div>
                                    <?php } ?>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_municipio_sem_alteracao')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>7. Municípios assistidos sem informação ou sem Comissão Instituída</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <?php echo $rel->getMunicipiosSemInfo($perCampos); ?>
                                    </div>

                                    <?php if ($pflcod != PFLCOD_SASE_TECNICO) { ?>
                                        <label for="dmdprazo" class="col-lg-12 col-md-12 control-label" style="text-align: left !important; margin-top: 10px;">Comentários gerais sobre avanços e dificuldades relacionados à permanência dos Municípios na mesma etapa de trabalho no período:</label>
                                        <div class="col-lg-12 col-md-12">
                                            <textarea class="form-control" maxlength="200" id="ravinfcomplqdo5" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplqdo5" rows="5"><?= $rel->arAtributos['ravinfcomplqdo5'] ?></textarea>
                                        </div>
                                    <?php } ?>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_municipio_sem_informacao')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>8. Município sem informação</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <?php
                                        $sql = "select distinct
													mun.muncod as codigo,
													mun.mundescricao as descricao
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												and sta.esdid in (" . ESDID_SASE_SEM_INFORMACAO . ")
												order by mun.mundescricao";
                                        /*
                                          $sqlTab = "select distinct
                                          msa.msaid as codigo,
                                          msa.msaacaoproposta,
                                          mun.mundescricao,
                                          mun.muncod as chaveVal
                                          from sase.assessoramento ass
                                          inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
                                          inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                                          inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                                          inner join territorios.municipio mun on mun.muncod = ass.muncod
                                          left join sase.municipiossemacoesdesenvstecredeaee msa on msa.marid = mar.marid and msa.msaquadro = 1
                                          where ur.usucpf = '{$usucpf}'
                                          and ravid = {$ravid}
                                          and sta.esdid in (".ESDID_SASE_SEM_INFORMACAO.")
                                          order by mun.mundescricao";
                                         */
                                        $sqlTab = "select distinct
                                                        msa.msaid as codigo,
                                                        msa.msaacaoproposta,
                                                        mun.mundescricao,
                                                        mun.muncod as chaveVal
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												inner join workflow.documento doc on doc.docid = ass.docid and docdatainclusao <= '{$ratdata2periodo2} 23:59:59'
                                                inner join workflow.estadodocumento esd on esd.esdid = doc.esdid and esd.esdid = 1117
												left join sase.municipiossemacoesdesenvstecredeaee msa on msa.marid = mar.marid and msa.msaquadro = 1
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												order by mun.mundescricao";

                                        $chave = "muncod";

                                        CampoDif($sql, $sqlTab, array('Lista de municípios', 'Ações propostas'), '', $chave, "msaacaoproposta", 200, "q6_1", $perCampos, 'delQuadro6');
                                        ?>
                                    </div>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_q6', 'q6_1', '<?= ESDID_SASE_SEM_INFORMACAO ?>')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>9. Município sem Comissão instituída</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
    <?php
    $sql = "select distinct
													mun.muncod as codigo,
													mun.mundescricao as descricao
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												and sta.esdid in (" . ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA . ")
												order by mun.mundescricao";
    /*
      $sqlTab = "select distinct
      msa.msaid as codigo,
      msa.msaacaoproposta,
      mun.mundescricao,
      mun.muncod as chaveVal
      from sase.assessoramento ass
      inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
      inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
      inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
      inner join territorios.municipio mun on mun.muncod = ass.muncod
      left join sase.municipiossemacoesdesenvstecredeaee msa on msa.marid = mar.marid and msa.msaquadro = 2
      where ur.usucpf = '{$usucpf}'
      and ravid = {$ravid}
      and sta.esdid in (".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
      order by mun.mundescricao";
     */

    $sqlTab = "select distinct
                                                        msa.msaid as codigo,
                                                        msa.msaacaoproposta,
                                                        mun.mundescricao,
                                                        mun.muncod as chaveVal
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												inner join workflow.historicodocumento hst on hst.docid = ass.docid and htddata <= '{$ratdata2periodo2} 23:59:59'
                                                inner join workflow.acaoestadodoc aed on aed.aedid = hst.aedid and aed.esdiddestino = 1118
												left join sase.municipiossemacoesdesenvstecredeaee msa on msa.marid = mar.marid and msa.msaquadro = 2
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												order by mun.mundescricao";

    $chave = "muncod";

    CampoDif($sql, $sqlTab, array('Lista de municípios', 'Ações propostas'), '', $chave, "msaacaoproposta", 200, "q6_2", $perCampos, 'delQuadro6');
    ?>
                                    </div>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_q6', 'q6_2', '<?= ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA ?>')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>10. Municípios sem alteração de etapa de trabalho, mas com demora justificável</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
    <?php
    $sql = "select distinct
													mun.muncod as codigo,
													mun.mundescricao as descricao
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												and not exists (
													select
														1
													from workflow.historicodocumento doc
													where doc.docid = ass.docid
													and to_char(doc.htddata, 'dd/mm/yyyy') between '{$ratdata1periodo}' and '{$ratdata2periodo}'
													--and doc.htddata between '$ratdata1periodo' and '$ratdata2periodo'
												)
												and not exists (
													select
														1
													from sase.municipiossemacoesdesenvstecredeaee msa
													inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
													inner join sase.assessoramento ass on ass.assid = mar.assid
													inner join territorios.municipio mun2 on mun.muncod = ass.muncod
													where msaquadro = 4 and ravid = {$ravid} and mun2.muncod = mun.muncod
												)
												order by mun.mundescricao";

    $sqlTab = "select distinct
                                                        msa.msaid as codigo,
                                                        msa.msaacaoproposta,
                                                        mun.mundescricao,
                                                        mun.muncod as chaveVal
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												left join sase.municipiossemacoesdesenvstecredeaee msa on mar.marid = msa.marid and msa.msaquadro = 3
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												and not exists (
													select
														1
													from workflow.historicodocumento doc
													where doc.docid = ass.docid
													and date(doc.htddata) between date('{$ratdata1periodo}') and date('{$ratdata2periodo}')
													--and doc.htddata between '$ratdata1periodo' and '$ratdata2periodo'
												)
												and not exists (
													select
														1
													from sase.municipiossemacoesdesenvstecredeaee msa
													inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
													inner join sase.assessoramento ass on ass.assid = mar.assid
													inner join territorios.municipio mun2 on mun.muncod = ass.muncod
													where msaquadro = 4 and ravid = {$ravid} and mun2.muncod = mun.muncod
												)
												order by mun.mundescricao";

    $chave = "muncod";

    CampoDif($sql, $sqlTab, array('Lista de municípios', 'Ações propostas'), '', $chave, "msaacaoproposta", 200, "q6_3", $perCampos, 'delQuadro6');
    ?>
                                    </div>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_q6', 'q6_3', '')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>11. Municípios sem alteração de etapa de trabalho com demora maior do que a esperada</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
    <?php
    $sql = "select distinct
													mun.muncod as codigo,
													mun.mundescricao as descricao
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												and not exists (
													select
														1
													from workflow.historicodocumento doc
													where doc.docid = ass.docid
													and to_char(doc.htddata, 'dd/mm/yyyy') between '{$ratdata1periodo}' and '{$ratdata2periodo}'
													--and doc.htddata between '$ratdata1periodo' and '$ratdata2periodo'
												)
												and not exists (
													select
														1
													from sase.municipiossemacoesdesenvstecredeaee msa
													inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
													inner join sase.assessoramento ass on ass.assid = mar.assid
													inner join territorios.municipio mun2 on mun.muncod = ass.muncod
													where msaquadro = 3 and ravid = {$ravid} and mun2.muncod = mun.muncod
												)
												order by mun.mundescricao";

    $sqlTab = "select distinct
                                                        msa.msaid as codigo,
                                                        msa.msaacaoproposta,
                                                        mun.mundescricao,
                                                        mun.muncod as chaveVal
												from sase.assessoramento ass
												inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
												inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
												inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
												inner join territorios.municipio mun on mun.muncod = ass.muncod
												left join sase.municipiossemacoesdesenvstecredeaee msa on msa.marid = mar.marid and msa.msaquadro = 4
												where ur.usucpf = '{$usucpf}'
												and ravid = {$ravid}
												and not exists (
													select
														1
													from workflow.historicodocumento doc
													where doc.docid = ass.docid
													and date(doc.htddata) between date('{$ratdata1periodo}') and date('{$ratdata2periodo}')
													--and doc.htddata between '$ratdata1periodo' and '$ratdata2periodo'
												)
												and not exists (
													select
														1
													from sase.municipiossemacoesdesenvstecredeaee msa
													inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
													inner join sase.assessoramento ass on ass.assid = mar.assid
													inner join territorios.municipio mun2 on mun.muncod = ass.muncod
													where msaquadro = 3 and ravid = {$ravid} and mun2.muncod = mun.muncod
												)
												order by mun.mundescricao";

    $chave = "muncod";

    CampoDif($sql, $sqlTab, array('Lista de municípios', 'Ações propostas'), '', $chave, "msaacaoproposta", 200, "q6_4", $perCampos, 'delQuadro6');
    ?>
                                    </div>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_q6', 'q6_4', '')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

    <?php $numInfComp = "12";
    if ($pflcod != PFLCOD_SASE_TECNICO) { ?>
                        <div class="row col-md-11">
                            <div class="well">
                                <fieldset>
                                    <legend>12. Comentários gerais sobre avanços e dificuldades relacionados às ações propostas para os municípios no período</legend>

                                    <div class="form-group">
                                        <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                        <div class="col-lg-12 col-md-12 ">
                                            <textarea class="form-control" maxlength="500" id="ravinfcomplqdo6" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplqdo6" rows="5"><?= $rel->arAtributos['ravinfcomplqdo6'] ?></textarea>
                                        </div>
                                    </div>

                                    <div class="text-right">
                                        <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_informacao_complementar_q6')">Salvar
                                        </button>
                                    </div>

                                </fieldset>
                            </div>
                        </div>
        <?php $numInfComp = "13";
    } ?>

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend><?= $numInfComp ?>. Informações complementares sobre o período avaliado</legend>

                                <div class="form-group">
                                    <!-- label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label -->

                                    <div class="col-lg-12 col-md-12 ">
                                        <textarea class="form-control" maxlength="500" id="ravinfcomplementar" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplementar" rows="5"><?= $rel->arAtributos['ravinfcomplementar'] ?></textarea>
                                    </div>
                                </div>

                                <div class="text-right">
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar_informacao_complementar')">Salvar
                                    </button>
                                </div>

                            </fieldset>
                        </div>
                    </div>

<?php } ?>


            </div>
        </div>
    </div>

</form>

<script>
    $(document).ready(function() {
        $('.campoCpf').mask("999.999.999-99");
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();

        $('.obrigatorio').append('<span style="color: red;">*</span>');

        jQuery.fn.maxLength = function(maxlength) {
            return this.each(function() {
                $(this).bind('keypress keyup change mouseover', function(e) {
                    var key,
                            max = maxlength ? maxlength : $(this).attr('maxlength'),
                            tam = this.value.length,
                            ie = (typeof window.ActiveXObject != 'undefined');
                    key = (ie) ? e.keyCode : e.which;
                    if (key >= 33 || key == 13 || key == 32) {
                        if (tam >= max) {
                            //e.preventDefault();
                            this.value = this.value.substring(0, max);
                        }
                    }
                    else if (tam > max) {
                        this.value = this.value.substring(0, max);
                    }
                });
            });
        };

        $(".maxRC").maxLength(200);

    });
</script>
