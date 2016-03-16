<?php
// empreendimento || obra || orgao
verificaSessao('obra');


//ver($_REQUEST, d);
/**
 * Verifica se a última supervisão foi de paralisação e to tipo OS
 * @param $obrid
 */
function ultVistoriaOS($obrid)
{
    global $db;
    $sql = "
            SELECT s.*, h.tplid
            FROM obras2.supervisao s
            LEFT JOIN obras2.historicoparalisacao h  ON h.supidparalisacao = s.supid
            WHERE s.obrid = {$obrid} AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
            AND s.rsuid = 1
            --AND s.staid = 2
            --AND h.tplid = 5
            ORDER BY s.supdata DESC LIMIT 1";
    $sup = $db->pegaLinha($sql);

    if ($sup['staid'] == 2 AND $sup['tplid'] == 5)
        return true;
    else
        return false;
}

$copy_sup = false;
$smiid = '';
if (!empty($_GET['smiid'])) {
    $smiid = $_GET['smiid'];
    $sql = 'SELECT supid FROM obras2.supervisao WHERE smiid = ' . $smiid;
    $supid = $db->pegaUm($sql);
    $copy_sup = true;
    $_SESSION['obras2']['copy_sup'] = $copy_sup;
}

// captura dados do projeto da requisição
$obrid = $_SESSION['obras2']['obrid'];

$somenteLeitura = 'S';
$habilitado = true;

//$somenteLeitura = 'N';
//$habilitado     = false;

if ($copy_sup) {
    $_SESSION['obras2']['supid'] = $supid;
} else {
    $supid = $_REQUEST['supid'];
    $_SESSION['obras2']['supid'] = $supid;
}

//$docid = pegarDocidSupervisao($supid);

$param = array();
$param['not(emsid)'] = true;
$param['not(sueid)'] = true;
$param['not(smiid)'] = true;
$supervisao = new Supervisao();
$ultimoSupid = $supervisao->pegaUltSupidByObra($obrid, $param);

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

/* function uploadArquivosArtVistoria() {
  //
  //    $arrRetornoIdsArquivos = array();
  ////    ver($_FILES,d);
  //    $arquivo_art_exec = (isset($_FILES['arquivo_art_exec']) ? $_FILES['arquivo_art_exec'] : array());
  //    $arquivo_art_fisc = (isset($_FILES['arquivo_art_fisc']) ? $_FILES['arquivo_art_fisc'] : array());
  //
  //    // Arquivo do Art de Execução
  //    if ($arquivo_art_exec["name"] != '' && $arquivo_art_exec["type"] != '' && $arquivo_art_exec["size"] != '' && $arquivo_art_exec["error"] == 0) {
  //        $file = new FilesSimec();
  //        $file->setPasta('obras2');
  //        $nome_arq = $arquivo_art_exec["name"];
  //        $file->setUpload($nome_arq, 'arquivo_art_exec', false, false);
  //        $arqid = $file->getIdArquivo();
  //        $arrRetornoIdsArquivos['arquivo_art_exec'] = $arqid;
  //    } else {
  //        $arrRetornoIdsArquivos['arquivo_art_exec'] = '';
  //    }
  //    // Arquivo do Art de Fiscalização
  //    if ($arquivo_art_fisc["name"] != '' && $arquivo_art_fisc["type"] != '' && $arquivo_art_fisc["size"] != '' && $arquivo_art_fisc["error"] == 0) {
  //        $file = new FilesSimec();
  //        $file->setPasta('obras2');
  //        $nome_arq = $arquivo_art_exec["name"];
  //        $file->setUpload($nome_arq, 'arquivo_art_fisc', false, false);
  //        $arqid = $file->getIdArquivo();
  //        $arrRetornoIdsArquivos['arquivo_art_fisc'] = $arqid;
  //    } else {
  //        $arrRetornoIdsArquivos['arquivo_art_fisc'] = '';
  //    }
  //
  //    return $arrRetornoIdsArquivos;
  //} */

// Executa as funções da tela de acordo com suas ações
if ($_REQUEST["requisicao"] == 'salvar') {


    if ($_REQUEST['smiid']) {
        $_REQUEST['supid'] = '';
        $_POST['supid'] = '';
        $_SESSION['obras2']['supid'] = '';
        $copy_sup = true;
        $_SESSION['obras2']['copy_sup'] = $copy_sup;
    }

    /**
     * Verifica se existe uma Trava no Cronograma. Caso o campo obras2.obras.obrtravaedicaocronograma = FALSE, deve-se bloquear as vistorias
     * para que o cronograma seja atualizado.
     */
    $obr = new Obras($obrid);

    if ($obr->getTravaCronograma($obrid) == 'f' && empty($_GET['croid'])) {
        $str_trava_cronograma_vistoria = 'A edição da vistoria está boqueada até que o Cronograma da Obra seja atualizado.';
        die("<script>
                alert('" . $str_trava_cronograma_vistoria . "');
                history.back(-1);
             </script>");
    }

    $supervisao = new Supervisao();
    $supervisao->arquid_art_exec = null;
    $supervisao->arquid_art_fisc = null;


    $estadoobra = $obr->getEstadoObra();
    if (!empty($estadoobra)) {
        if ($estadoobra['esdid'] == ESDID_OBJ_INACABADA) {
            $supervisao->posvigencia = 't';
            wf_alterarEstado($obr->docid,AEDID_INACABADA_PARA_CONCLUIDA,"Obra concluída pós vigência",array());
        }
    }



    $supid = $supervisao->salvar();
    $supervisao->commit();

    if ($_REQUEST['smiid']) {
        $resp = atualizarFotosVistoria($supid);
    }

    $obras = new Obras($obrid);

    $email = new Email();
    $email->enviaEmailEmpenhoMobiliario($obrid);
    $email = new Email();
    $email->enviaEmailPagamentoMobiliario($obrid);

    if ($_REQUEST['staid'] == 2) {
        $email = new Email();
        $email->enviaEmailVistoriaParalisacao($obrid);
    }

    if ($_REQUEST['staid'] == 1 && $supervisao->percentExec > 85) {
        $email = new Email();
        $email->enviaEmailFaseDeConclusao($obrid);
    }

    if ($_REQUEST[tplid] == 3) {
        // Cria restrição para informar a solicitação da obra vinculada
        $sv = new SolicitacaoVinculada();
        $sv->criarRestricaoObraVinculada($obrid);

        $email = new Email();
        $email->paralisacaoPorContratoRescindido($obrid);
    }

    if (possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) {
        $supervisao->validaSupervisao($supid);
        if (!$_REQUEST['supid']) {
            $obras->staid = $_REQUEST['staid'];
            $obras->obrpercentultvistoria = $supervisao->percentExec;
            $obras->obrdtultvistoria = $supervisao->supdata;
        } elseif ($ultimoSupid == $_REQUEST['supid']) {
            $obras->staid = $_REQUEST['staid'];
            $obras->obrpercentultvistoria = $supervisao->percentExec;
            $obras->obrdtultvistoria = $supervisao->supdata;
        }
    }


    $arObra = array();
    $arCamposNulo = array();
    if ($_POST['obrcronogramaservicocontratado']) {
        $arObra['obrcronogramaservicocontratado'] = $_POST['obrcronogramaservicocontratado'];
    } else {
        $arCamposNulo[] = 'obrcronogramaservicocontratado';
        $arObra['obrcronogramaservicocontratado'] = null;
    }

    if ($_POST['obrcronogramaservicocontratadojustificativa']) {
        $arObra['obrcronogramaservicocontratadojustificativa'] = $_POST['obrcronogramaservicocontratadojustificativa'];
    } else {
        $arCamposNulo[] = 'obrcronogramaservicocontratadojustificativa';
        $arObra['obrcronogramaservicocontratadojustificativa'] = null;
    }

    // O cronograma da obra convencional deve ser travado para modificações pelo supervisor unidade após a primeira vistoria.
    $arObra['obrtravaedicaocronograma'] = 'TRUE';

    $obras->popularDadosObjeto($arObra)
        ->salvar(true, true, $arCamposNulo);

    $_POST['ftiid'] = ($_POST['ftiid'] ? $_POST['ftiid'] : array());
    foreach ($_POST['ftiid'] as $ftiid) {
        $arquivo = $_FILES["anexomi_{$ftiid}"];
        if ($arquivo["name"] && $arquivo["type"] && $arquivo["size"] && $arquivo["error"] == 0) {
            $campos = array(
                "ftiid" => $ftiid,
                "supid" => $supid
            );

            $file = new FilesSimec("arquivosupervisaoformulariotecnologiami", $campos, "obras2");
            $file->setPasta('obras2');
            $file->setUpload(null, "anexomi_{$ftiid}", true);
            $arqid = $file->getIdArquivo();
        }
    }


    /**
     * A vistoria do Gestor Unidade, quando se tratar de uma obra  na situação paralisada,
     * pode atualizar diretamente o percentual e a data de atualização da obra sem necessidade de validação;
     */
    $estado = $obras->getEstadoObra($obrid);
    $esdid = $estado['esdid'];

    if (possui_perfil(PFLCOD_GESTOR_UNIDADE) && $esdid == ESDID_OBJ_PARALISADO) {
        $supervisao = new Supervisao($supid);
        //--$supervisao->validaSupervisao($supervisao->supid); -- Não precisa da validação
        $obr = new Obras($obrid);
        $obr->obrpercentultvistoria = $supervisao->pegaPercentSupervisao($supid);
        $obr->obrdtultvistoria = $supervisao->supdata;
        $obr->salvar();
    } else if (possui_perfil(PFLCOD_GESTOR_UNIDADE) && $esdid == ESDID_OBJ_CONCLUIDO) {
        $supervisao = new Supervisao($supid);
        $supervisao->validaSupervisao($supervisao->supid);
        $obr = new Obras($obrid);
        $obr->obrpercentultvistoria = $supervisao->pegaPercentSupervisao($supid);
        $obr->obrdtultvistoria = $supervisao->supdata;
        $obr->salvar();
    }

    $db->commit();

    $obras->enviaEmailEvolucao();

    // Quando a inclusão vier da pagina de ediçao de cronograma, ativar o novo cronograma e registrar nas atividades
    if (!empty($_GET['croid'])) {

        $obra = new Obras($obrid);
        $obra->obrtravaedicaocronograma = true;
        $obra->salvar();

        $cronograma = new Cronograma();
        $arDadosCronograma = $cronograma->getCronogramaEdicao($obrid);
        $cronograma->popularDadosObjeto($arDadosCronograma);

        $cronogramaAntigo = $cronograma->getCronogramaObra($obrid);
        $cronogramaAntigo = new Cronograma($cronogramaAntigo['croid']);
        $cronogramaAntigo->crostatus = 'H';
        $cronogramaAntigo->salvar();

        $cronograma->crostatus = 'A';
        $cronograma->salvar();
        $cronograma->commit();

        $regAtividade = new RegistroAtividade();
        $arDado = array();
        $arDado['arqid'] = null;
        $arDado['obrid'] = $obrid;
        $arDado['usucpf'] = $_SESSION['usucpf'];
        $arDado['rgadscsimplificada'] = 'Edição do cronograma.';
        $arDado['rgadsccompleta'] = "O cronograma da obra foi editado sob a justificativa: " . $cronograma->croobs;
        $arDado['rgaautomatica'] = true;
        $arDado['rgastatus'] = 'A';
        $arCamposNulo2 = array('empid');

        if (empty($arDado['arqid'])) {
            $arCamposNulo2[] = 'arqid';
        }

        $rga = $regAtividade->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo2);
        $regAtividade->commit();


        die("<script>
          alert('Cronograma editado com sucesso!');
            window.location = '?modulo=principal/etapas_da_obra&acao=A&obrid={$obrid}';
         </script>");

    } else {
        $db->sucesso("principal/inserir_vistoria", "&supid=" . $supid);
    }
    unset($_SESSION['obras2']['copy_sup']);
} elseif ($_POST['requisicao'] == 'carregaSubTipoParalisacao') {
    $subTipoParalisacao = new SubTipoParalisacao();
    $sql = $subTipoParalisacao->listaCombo(array("tplid" => $_POST['tplid']));
    $db->monta_combo("stpid", $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'S', "stpid");
    die;
} elseif ($_POST['requisicao'] == 'download') {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqiddownload'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    die();
} elseif ($_POST['requisicao'] == 'apagarArquivoFormularioTecnologia') {
    $arqid = $_REQUEST['arqid'];
    $arquivoSupervisaoFormularioTecnologiaMi = new ArquivoSupervisaoFormularioTecnologiaMi();
    $arquivoSupervisaoFormularioTecnologiaMi->carregaPorArqid($arqid);

    if ($arquivoSupervisaoFormularioTecnologiaMi->asfid) {
        $arquivoSupervisaoFormularioTecnologiaMi->asfstatus = 'I';
        $arquivoSupervisaoFormularioTecnologiaMi->salvar();

        $db->commit();
    }
    die();
} elseif ($_POST['requisicao'] == 'validaSupervisao') {

    $supervisao = new Supervisao($_POST['supid']);
    $resposta = $supervisao->validaSupervisao($supervisao->supid);

    $obras = new Obras($obrid);
    $obras->obrpercentultvistoria = $supervisao->pegaPercentSupervisao($_POST['supid']);
    $obras->obrdtultvistoria = $supervisao->supdata;

    $obras->enviaEmailEvolucao();
    $obras->salvar();
    $obras->commit();

    if ($resposta) {
        exit("Supervisão Validada com sucesso!");
    } else {
        exit("Houve um erro ao validar supervisão, tente novamente.");
    }
}

// recuperando a situação da obra
$param = array();
$param['not(emsid)'] = true;
$param['not(smiid)'] = true;
$situacao_obra = $supervisao->pegaSituacaoVistoria($obrid, $param);

//recuperando os dados da vistoria
$vistoria = new Supervisao($supid);
$vistoriaEstrutura = $vistoria->getDados();

$obra = new Obras($obrid);

// Verifica se a obra é do tipo MI
$obraMi = ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ? true : false;
$obrcronogramaservicocontratado = $obra->obrcronogramaservicocontratado;
$obrcronogramaservicocontratadojustificativa = $obra->obrcronogramaservicocontratadojustificativa;

//recuperando os dados da paralização ( se tiver )
$paralisacao = new HistoricoParalisacao();

$historicoParalisacao = $paralisacao->getHistoricoParalisacao($supid);

$arquivo = new Arquivo($historicoParalisacao['arqid']);
$arquivoBoletim = new Arquivo($historicoParalisacao['arqidboletim']);
$arquivoOSparalisacao = new Arquivo($historicoParalisacao['arqidosparalisacao']);
$arquivoplanilha = new Arquivo($vistoriaEstrutura['arqidplanilha']);
$arquivotermo = new Arquivo($vistoriaEstrutura['arqidtermo']);
$arqidosexecucao = new Arquivo($vistoriaEstrutura['arqidosexecucao']);
$arqiddocumentoembargo = new Arquivo($historicoParalisacao['arqiddocumentoembargo']);

if ($_REQUEST['ajaxFotosGaleria'] && ($_REQUEST['fotosSupervisao'] || $_REQUEST['fotosGaleria'])) {
    if (atualizarFotosVistoria()) {
        echo "true";
    } else {
        echo "false";
    }
    exit();
}


// Realiza as ações do componente de fotos
if ($_REQUEST["AJAX"]) {

    switch ($_REQUEST["subacao"]) {
        case "ShowImage":
            ShowImage($_REQUEST["img"], $_REQUEST["dir"]);
            break;
        case "UpdateListFoto":
            UpdateListFoto();
            break;
        case "FotosVistoria":
            AtualizarFotosVistoria2($_REQUEST);
            break;
        case "editar":
            AtualizarFotosVistoria2($_REQUEST);
            break;
        case "deletar":
            DeletarFotoVistoria($_REQUEST);
            break;
        case "galeria":
            EnviarGaleria($_REQUEST["imagens"]);
            break;
    }
    die;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if ($_GET['acao'] != 'V') {
    // Inclusão de arquivos padrão do sistema
    include APPRAIZ . 'includes/cabecalho.inc';

    echo '<br>';
    if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $db->cria_aba(ID_ABA_CADASTRO_VISTORIA_FNDE, $url, $parametros);
    } else {
        $db->cria_aba(ID_ABA_CADASTRO_VISTORIA, $url, $parametros);
    }
} else {
    echo '
            <script language="JavaScript" src="../includes/funcoes.js"></script>
            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
         ';
    $db->cria_aba($abacod_tela, $url, $parametros);
}

if ((($vistoriaEstrutura["usucpf"] == $_SESSION['usucpf'] || possui_perfil(array(PFLCOD_SUPER_USUARIO))) &&
        $vistoriaEstrutura['supid'] == $ultimoSupid &&
        $_GET['acao'] != 'V') ||
    empty($vistoriaEstrutura['supid'])
) {
    $somenteLeitura = 'S';
    $habilitado = true;
} else {
    $somenteLeitura = 'N';
    $habilitado = false;
}

if ($copy_sup) {
    $somenteLeitura = 'S';
    $habilitado = true;
}

//RN - Não permitir a edição de vistorias pelo supervisor unidade ou gestor unidade 7 dias após a data de inclusão.
if ($supid) {
    $dataInicial = explode(' ', $vistoriaEstrutura['supdtinclusao']);
    $dataFinal = date('Y-m-d');

    $diffDatas = ((((strtotime($dataFinal) - strtotime($dataInicial[0])) / 60) / 60) / 24);
    $habilitaMods = (($diffDatas >= 7) && (possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) || (possui_perfil(PFLCOD_CALL_CENTER))) ? false : true;
    if (!$habilitaMods) {
        $somenteLeitura = 'N';
        $habilitado = false;
    }
} elseif (possui_perfil(PFLCOD_CALL_CENTER)) {//demanda 311298 - perfil call_center não pode editar.
    $somenteLeitura = 'N';
    $habilitado = false;
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

echo cabecalhoObra($obrid);
echo alertaObraMi($obrid);
monta_titulo($titulo_modulo, 'Acompanhamento da Obra');
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script src="/obras2/js/vistoria.js"></script>
    <style type="text/css">
        <!--
        .divItemComposicao {
            width: 100%;
            font-weight: bold;
        }

        .divDivisao {
            width: 100%;
            margin-left: 5px;
            font-weight: bold;
            padding: 5px;
        }

        .divQuestao {
            width: 100%;
            margin-left: 10px;
            padding: 5px;
        }

        .divQuestaoNivel1 {
            width: 100%;
        }

        .divQuestaoNivel2 {
            width: 100%;
        }

        .divQuestaoNivel3 {
            margin-top: 3px;
        }

        .divQuestaoNivel4 {
            margin-top: 3px;
        }

        .divQuestaoNivel4_none {
            margin-top: 3px;
            display: none;
        }

        .divQuestaoNivel4_fieldset {
            width: 98%
        }

        .divQuestaoNivel4_1 {
            margin-top: 3px;
        }

        .divQuestaoNivel4_2 {
            margin-top: 3px;
        }

        .divQuestaoNivel4_3 {
            margin-top: 3px;
            padding: 2px 2px 2px 2px;
            border: 1px dashed #ccc;
            height: 70px;
        }

        .divQuestaoNivel3 {
            margin-top: 3px;
        }

        .divQuestaoNivel3_none {
            display: none;
            margin-top: 3px;
        }

        .divQuestaoNivel3_1 {
            font-weight: bold;
        }

        input.disabled {
            FONT-SIZE: 10px;
            BORDER: none;
            COLOR: #404040;
            FONT-FAMILY: verdana;
            BACKGROUND-COLOR: #ffffff;
        }

        -->
    </style>
    <form id="formulario" name="formulario" method="post" enctype="multipart/form-data"
          action="?modulo=principal/inserir_vistoria&acao=A<?= ($_GET['croid'] ? "&croid=" . $_GET['croid'] : "") ?>">

    <input type="hidden" name="smiid" id="smiid" value="<?php echo $smiid; ?>"/>
    <input type="hidden" name="situacaoatual" id="situacaoatual" value="<?php echo $situacao_obra; ?>"/>
    <input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
    <input type="hidden" name="arqiddownload" id="arqiddownload" value=""/>
    <input type="hidden" name="obrdtinicio" id="obtdtinicio"
           value="<?php echo formata_data($datasValidacao["obrdtinicio"]); ?>"/>
    <input type="hidden" name="obrdttermino" id="obrdttermino"
           value="<?php echo formata_data($datasValidacao["obrdttermino"]); ?>"/>
    <input type="hidden" name="dtiniciolicitacao" id="dtiniciolicitacao"
           value="<?php echo formata_data($datasValidacao["dtiniciolicitacao"]); ?>"/>
    <input type="hidden" name="dtfinallicitacao" id="dtfinallicitacao"
           value="<?php echo formata_data($datasValidacao["dtfinallicitacao"]); ?>"/>
    <input type="hidden" name="fprdtiniciofaseprojeto" id="fprdtiniciofaseprojeto"
           value="<?php echo formata_data($datasValidacao["fprdtiniciofaseprojeto"]); ?>"/>
    <input type="hidden" name="fprdtconclusaofaseprojeto" id="fprdtconclusaofaseprojeto"
           value="<?php echo formata_data($datasValidacao["fprdtconclusaofaseprojeto"]); ?>"/>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="3">
            Dados do Acompanhamento&nbsp;&nbsp;&nbsp;&nbsp;
            (<a href="javascript:abreEvolucaoFinan( <?php echo $obrid ?> )">Ver Gráfico de Evolução da Execução da
                Obra</a>)
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Data do Acompanhamento</td>
        <td>
            <?php
            $supdata = $vistoriaEstrutura['supdata'];
            echo campo_data2('supdata', 'S', $somenteLeitura, '', 'S', '', 'validaDataVistoria();', null, 'validaDataVistoria();');
            ?>
        </td>
        <td rowspan="8" align="right" valign="top" width="1">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Data Prevista de Conclusão</td>
        <td>
            <?php
            $supdatafimobra = $vistoriaEstrutura['supdatafimobra'];
            echo campo_data2('supdatafimobra', 'S', $somenteLeitura, '', 'S', '', '');
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Nome do Responsável</td>
        <td>
            <?php
            $vistoriador = new Entidade($vistoriaEstrutura['supvistoriador']);
            $entnomevistoriador = $vistoriador->entnome;
            $entidvistoriador = $vistoriador->getPrimaryKey();
            ?>
            <span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
            <input type="hidden" name="entidvistoriador" id="entidvistoriador"
                   value="<?php if (isset($obrid)) echo $entidvistoriador; ?>">
            <?php if ($somenteLeitura == 'S' && $entnomevistoriador == '') { ?>
                <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;"
                       onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
            <?php } ?>
            <?php print obrigatorio(); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Situação</td>
        <td>
            <?php
            $percentual = 0;
            $staid = ($dado_situacao ? $dado_situacao : $vistoriaEstrutura['staid']);

            $sitAtividade = new SituacaoAtividade();

            $param = array();
            $esdid = pegaEstadoObra($obra->docid);

            if ($esdid == ESDID_OBJ_CONCLUIDO) {
                $param['not(staid)'][] = STAID_PARALISADO;
                if (!possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO)))
                    $param['not(staid)'][] = STAID_EXECUCAO;
            }
            if ($esdid == ESDID_OBJ_INACABADA) {
                $param['not(staid)'][] = STAID_PARALISADO;
                $param['not(staid)'] [] = STAID_EXECUCAO;

            }

            if ($esdid == ESDID_OBJ_PARALISADO) {
                $param['not(staid)'] = STAID_CONCLUIDO;
            }

            $sql = $sitAtividade->situacaoPossivelSql($param);
            $staid = $vistoriaEstrutura['staid'];
            $db->monta_combo("staid", $sql, $somenteLeitura, "Selecione...", "validasituacao(this.value); verificasituacao", '', '', '', 'S', 'staid');
            ?>
        </td>
    </tr>
    <tr style="display:none" id="tr_elaboracao">
        <td class="SubTituloDireita">Necessita de Licenciamento Ambiental</td>
        <td>
            <?php
            $obrlincambiental = $vistoriaEstrutura[''];

            if ($supid) {
                if ($obrlincambiental == "t") {
                    echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"1\"> Sim";
                    echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"0\"> Não";
                } else {
                    echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"1\"> Sim";
                    echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"0\"> Não";
                }
            } else {
                echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"1\"> Sim";
                echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"0\"> Não";
            }
            ?>
        </td>
    </tr>
    <tr style="display:none" id="tr_elaboracao1">
        <td class="SubTituloDireita"> Necessita de aprovação junto ao patrimônio histórico</td>
        <td>
            <?php
            $obraprovpatrhist = $vistoriaEstrutura[''];

            if ($supid) {
                if ($obraprovpatrhist == "t") {
                    echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
                    echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"0\"> Não";
                } else {
                    echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
                    echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"0\"> Não";
                }
            } else {
                echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
                echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"0\"> Não";
            }
            ?>
        </td>
    </tr>
    <tr style="display:none" id="tr_elaboracao2">
        <td class="SubTituloDireita">Previsão de entrega do(s) projeto(s)</td>
        <td>
            <?php
            $obrdtprevprojetos = $vistoriaEstrutura['obrdtprevprojetos'];
            echo campo_data2('obrdtprevprojetos', 'S', $somenteLeitura, '', 'S', '', '');
            ?>
        </td>
    </tr>
    <tr style="display:none" id="tr_tplid">
        <td class="SubTituloDireita">Tipo de Paralisação</td>
        <td>
            <?php
            $tipoParalisacao = new TipoParalisacao();
            $sql = $tipoParalisacao->listaCombo();
            $tplid = $historicoParalisacao['tplid'];

            $db->monta_combo("tplid", $sql, $somenteLeitura, "Selecione...", "ctrlTipoParalisacao(this.value);", '', '', '', 'S', "tplid");
            ?>
        </td>
    </tr>
    <? if (obraMI($obrid) && (ultVistoriaOS($obrid) || !empty($vistoriaEstrutura['arqidosexecucao']))): ?>

        <tr style="display:none" id="tr_osexecucao">
            <td class="SubTituloDireita">OS de Execução</td>
            <td>
                <input type="hidden" name="arqidosexecucao" id="arqid" value="<?php echo $arqidosexecucao->arqid; ?>"/>
                <? if ($somenteLeitura != 'N'): ?>
                    <input type="file" name="arqidosexecucao" id="arquivo"/>
                <? endif; ?>
                <?php echo obrigatorio(); ?>
                <br>
                <?php
                if ($arqidosexecucao->arqid) {
                    echo "<a href='javascript:DownloadArquivo(" . $arqidosexecucao->arqid . ");'>(" . $arqidosexecucao->arqnome . "." . $arqidosexecucao->arqextensao . ")</a>";
                }
                ?>
            </td>
        </tr>

    <? endif; ?>

    <tr style="display:none" id="tr_documentoembargo">
        <td class="SubTituloDireita">Documento de Embargo</td>
        <td>
            <input type="hidden" name="arqiddocumentoembargo" id="arqid"
                   value="<?php echo $arqiddocumentoembargo->arqid; ?>"/>
            <? if ($somenteLeitura != 'N'): ?>
                <input type="file" name="documentoembargo" id="arquivo"/>
            <? endif; ?>
            <?php echo obrigatorio(); ?>
            <br>
            <?php
            if ($arqiddocumentoembargo->arqid) {
                echo "<a href='javascript:DownloadArquivo(" . $arqiddocumentoembargo->arqid . ");'>(" . $arqiddocumentoembargo->arqnome . "." . $arqiddocumentoembargo->arqextensao . ")</a>";
            }
            ?>
        </td>
    </tr>


    <tr style="display:none" id="tr_stpid">
        <td class="SubTituloDireita">Subtipo de Paralisação</td>
        <td id="td_stpid">
            <?php
            if ($tplid) {
                $subTipoParalisacao = new SubTipoParalisacao();
                $sql = $subTipoParalisacao->listaCombo(array("tplid" => $tplid));
                $stpid = $historicoParalisacao['stpid'];
                $db->monta_combo("stpid", $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'S', "stpid");

            }
            ?>
        </td>
    </tr>
    <tr style="display:none" id="tr_hprobs">
        <td class="SubTituloDireita">Observações da Paralisação</td>
        <td>
            <?php
            $hprobs1 = $historicoParalisacao['hprobs'];
            echo campo_textarea('hprobs1', 'S', 'S', '', '70', '8', '1000', '', 0, '', '', false, '', '');
            ?>
        </td>
    </tr>

    <tr id="tr_osparalisacao" style="display: none;">
        <td class="SubTituloDireita" style="vertical-align:top;">OS de Paralisação:</td>
        <td>
            <input type="hidden" name="arqidosparalisacao" id="arqid"
                   value="<?php echo $arquivoOSparalisacao->arqid; ?>"/>
            <input type="file" name="arqidosparalisacao" id="arquivo"/>
            <?php echo obrigatorio(); ?>
            <br>
            <?php
            if ($arquivoOSparalisacao->arqid) {
                echo "<a href='javascript:DownloadArquivo(" . $arquivoOSparalisacao->arqid . ");'>(" . $arquivoOSparalisacao->arqnome . "." . $arquivoOSparalisacao->arqextensao . ")</a>";
            }
            ?>
        </td>
    </tr>

    <tr id="tr_arqid" style="display: none;">
        <td class="SubTituloDireita" style="vertical-align:top;">Recisão do Contrato:</td>
        <td>
            <input type="hidden" name="arqid" id="arqid" value="<?php echo $arquivo->arqid; ?>"/>
            <input type="file" name="arquivo" id="arquivo"/>
            <?php echo obrigatorio(); ?>
            <br>
            <?php
            if ($arquivo->arqid) {
                echo "<a href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
            }
            ?>
        </td>
    </tr>

    <tr id="tr_arqid_boletim" style="display: none;">
        <td class="SubTituloDireita" style="vertical-align:top;">Boletim de medição acumulado:</td>
        <td>
            <input type="hidden" name="arqidboletim" id="arqid" value="<?php echo $arquivoBoletim->arqid; ?>"/>
            <input type="file" name="arquivoboletim" id="arquivo"/>
            <?php echo obrigatorio(); ?>
            <br>
            <?php
            if ($arquivoBoletim->arqid) {
                echo "<a href='javascript:DownloadArquivo(" . $arquivoBoletim->arqid . ");'>(" . $arquivoBoletim->arqnome . "." . $arquivo->arqextensao . ")</a>";
            }
            ?>
        </td>
    </tr>

    <tr id="tr_arqid_planilha" style="display: none;">
        <td class="SubTituloDireita" style="vertical-align:top;">Planilha Acumulada da Medição:</td>
        <td>
            <input type="hidden" name="arqidplanilha" id="arqidplanilha"
                   value="<?php echo $arquivoplanilha->arqid; ?>"/>
            <input type="file" name="arquivo_planilha" id="arquivo_planilha"/>
            <br>
            <?php
            if ($arquivoplanilha->arqid) {
                echo "<a href='javascript:DownloadArquivo(" . $arquivoplanilha->arqid . ");'>(" . $arquivoplanilha->arqnome . "." . $arquivoplanilha->arqextensao . ")</a>";
            }
            ?>
        </td>
    </tr>

    <tr id="tr_arqid_termo" style="display: none;">
        <td class="SubTituloDireita" style="vertical-align:top;">Termo de Recebimento da Obra:</td>
        <td>
            <input type="hidden" name="arqidtermo" id="arqidtermo" value="<?php echo $arquivotermo->arqid; ?>"/>
            <input type="file" name="arquivo_termo" id="arquivo_termo"/>
            <br>
            <?php
            if ($arquivotermo->arqid) {
                echo "<a href='javascript:DownloadArquivo(" . $arquivotermo->arqid . ");'>(" . $arquivotermo->arqnome . "." . $arquivotermo->arqextensao . ")</a>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="30%" nowrap="nowrap">TODOS OS SERVIÇOS QUE COMPOEM A PLANILHA PACTUADA COM O
            FNDE FORAM CONTRATADOS?
        </td>
        <td>
            <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_s" value="S"
                <?php echo(!$habilitado ? 'disabled="disabled"' : '') ?>
                <?php echo $obrcronogramaservicocontratado == 'S' ? 'checked="checked"' : '' ?>
                   onclick="managerServicoContratado('S');">
            <label for="obrcronogramaservicocontratado_s">Sim</label>
            <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_n" value="N"
                <?php echo(!$habilitado ? 'disabled="disabled"' : '') ?>
                <?php echo $obrcronogramaservicocontratado == 'N' ? 'checked="checked"' : '' ?>
                   onclick="managerServicoContratado('N');">
            <label for="obrcronogramaservicocontratado_n">Não</label>
        </td>
    </tr>
    <tr id="tr_servico_contratado_justificativa"
        style="display: <?php echo($obrcronogramaservicocontratado == 'N' ? '' : 'none') ?>">
        <td class="SubTituloDireita" width="30%" nowrap="nowrap" valign="top">Justificativa:</td>
        <td>
            <?php
            echo campo_textarea('obrcronogramaservicocontratadojustificativa', 'S', 'S', '', '70', '8', '2000', '', 0, '', '', false, '', '');
            ?>
        </td>
    </tr>
    <tr id="tr8">
        <td colspan="3">Detalhamento de Supervisão e Acompanhamento</td>
    </tr>
    <tr id="tr9">
    <td colspan="3">
    <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
    <thead>
    <tr>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Descrição</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Valor (R$)</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>(%) Sobre a Obra</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Quantidade</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Unidade de Medida</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Data de Início</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Data de Término</b></td>
        <td colspan="2" rowspan="1" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Última Supervisão</b></td>
        <td colspan="4" rowspan="1" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Supervisão Atual</b></td>
        <td colspan="2" rowspan="1" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Diferença para a Supervisão anterior</b></td>
    </tr>
    <tr>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>(%) do Item já Executado</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>(%) do Item já Executado sobre a Obra</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>(%) Supervisão</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Valor Executado<br></b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>(%) do Item já Executado sobre a Obra após Supervisão</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Quantidade Executada<br></b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>Valor Executado</b></td>
        <td colspan="1" rowspan="2" valign="middle" align="center"
            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
            class="title"><b>(%) Executado<br></b></td>
    </tr>
    </thead>

    <?php
    $supervisaoItem = new SupervisaoItem();

    $total['vlrProjeto'] = 0;
    $total['vlrObra'] = 0;
    $total['vlrExecSobreObra'] = 0;
    $total['vlrExecSobreObraUlt'] = 0;
    $total['percExecSobreObraUlt'] = 0;


    if ($_GET['croid']) {
        $cronograma = new Cronograma();
        $arDadosCronograma = $cronograma->getCronogramaEdicao($obrid);
        $cronograma->popularDadosObjeto($arDadosCronograma);

        $dadoEtapa = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid), 'D', $cronograma->croid);
        $dadoEtapaF = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid), 'F', $cronograma->croid);

    } else {
        // Busca filhos da Edificação
        $cronograma = new Cronograma();
        $croid = $cronograma->getIdCronogramaObra($obrid);
        $dadoEtapa = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid), 'D', ($supid ? null : $croid));
        $dadoEtapaF = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid), 'F', ($supid ? null : $croid));
    }


    $habil = (count($dadoEtapa) ? 'N' : 'S');

    foreach ($dadoEtapa as $etapa) {

        $total['vlrProjeto'] = $etapa['ocrvalorexecucao'];

        $total['vlrObra'] += $etapa['icovlritem'];
        if ($etapa['ocrvalorexecucao'] > 0) {
            $total['percObra'] = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;
        } else {
            $total['percObra'] = 0;
        }
        $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];

        if ($etapa['ocrvalorexecucao'] > 0) {
            $total['percExecSobreObra'] = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;
        } else {
            $total['percExecSobreObra'] = 0;
        }

        $total['vlrExecSobreObraUlt'] += $etapa['spivlrfinanceiroanterior'];

        if ($etapa['ocrvalorexecucao'] > 0) {
            $total['percExecSobreObraUlt'] = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
        } else {
            $total['percExecSobreObraUlt'] = 0;
        }
//                        $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];

        $etapa['icodtinicioitem'] = formata_data($etapa['icodtinicioitem']);
        $etapa['icodterminoitem'] = formata_data($etapa['icodterminoitem']);

        if ($etapa['ocrvalorexecucao'] > 0) {
            $etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
        } else {
            $etapa['spipercsobreobrainfsupervisor'] = 0;
        }

        // Busca Filhos da Etapa
        $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
        $habil = (count($dadoDetalhe) ? 'N' : 'S');
        ?>
        <tbody>
        <tr bgcolor="#FDF8E7" id="<?php echo "tr_item_etapa_" . $obrid . "_" . $etapa['icoid'] ?>">
            <td align="left">
                <input type="hidden" name="icoid[]" value="<?php echo $etapa['icoid'] ?>">
                <input type="hidden" name="ico_spiid[]" value="<?php echo $etapa['spiid'] ?>">
                <input type="hidden" name="ico_spitotitemsupervisor[]" value="">
                &nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?php echo $etapa['itcdesc'] ?>
            </td>
            <td align="right" style="color:#336EFF">
                <?php echo number_format($etapa['icovlritem'], 2, ',', '.') ?>
            </td>
            <td align="right" style="color:#336EFF">
                <?php echo number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
            </td>
            <td align="right" style="color:#336EFF">
                -
            </td>
            <td align="center">
                -
            </td>
            <td align="center">
                <?php echo $etapa['icodtinicioitem'] ?>
            </td>
            <td align="center">
                <?php echo $etapa['icodterminoitem'] ?>
            </td>
            <td align="center">
                <input type="hidden" name="ico_spivlrfinanceiroanterior[]"
                       value="<?php echo number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                <?php echo campo_texto('ico_spivlritemexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
            </td>
            <td align="center">
                <?php echo campo_texto('ico_spivlritemsobreobraexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
            </td>
            <td align="center">
                <?php echo campo_texto('ico_spivlrinfsupervisor[]', 'N', $somenteLeitura, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, ' . $obrid . ' )', number_format($etapa['spivlrinfsupervisor'], 2, ',', '.')); ?>


            </td>
            <td align="center">
                <?php echo campo_texto('ico_spivlrfinanceiroinfsupervisor[]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, ' . $obrid . ', \'numerico\' )', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
            </td>
            <td align="right" style="color:#336EFF">
                <input type="hidden" name="ico_spipercsobreobrainfsupervisor[]"
                       value="<?php echo number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                    <span>
    <?php echo number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                    </span>
            </td>
            <td align="right" style="color:#336EFF">
                &nbsp;
            </td>
            <td align="right" style="color:#336EFF">
                <input type="hidden"
                       value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?>"
                       name="ico_spidiferencavlr[]">
                                    <span> 
                                        <?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?> 
                                    </span>
            </td>
            <td align="right" style="color:#336EFF">
                <input type="hidden"
                       value="<?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?>"
                       name="ico_spidiferencaporcentagem[]">
                                    <span> 
    <?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?> 
                                    </span>
            </td>
        </tr>
        </tbody>
        <?php
        foreach ($dadoDetalhe as $detalhe) {
            $ditid = $detalhe['ditid'];
            $detalhe['ditdatainicio'] = formata_data($detalhe['ditdatainicio']);
            $detalhe['ditdatatermino'] = formata_data($detalhe['ditdatatermino']);
            $detalhe['spipercsobreobrainfsupervisor'] = ($detalhe['spivlrfinanceiroinfsupervisor'] / $detalhe['ocrvalorexecucao']) * 100;

            //adicionando os filhos
            $dadoDetalheFilho = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai ={$ditid}"));
            $habil = (count($dadoDetalheFilho) ? 'N' : 'S');
            ?>
            <tbody>
            <tr bgcolor="#DBF6D5"
                id="<?php echo "tr_item_detalhamento_" . $obrid . "_" . $detalhe['icoid'] . "_" . $detalhe['ditid'] ?>">
                <td align="left">
                    <input type="hidden" name="ditid[]" value="<?php echo $detalhe['ditid'] ?>">
                    <input type="hidden" name="dit_spiid[]" value="<?php echo $detalhe['spiid'] ?>">
                    <input type="hidden" name="ico_spitotitemsupervisor[]" value="">
                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif"
                                                             border="0"><?php echo $detalhe['ditdsc'] ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php echo number_format($detalhe['ditvalor'], 2, ',', '.') ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php echo number_format($detalhe['ditpercsobreobra'], 2, ',', '.') ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php echo($detalhe['ditmetafisica'] ? number_format($detalhe['ditmetafisica'], 2, ',', '.') : '-') ?>
                </td>
                <td align="center">
                    <?php echo($detalhe['umcdsc'] ? $detalhe['umcdsc'] : '-') ?>
                </td>
                <td align="center">
                    <?php echo $detalhe['ditdatainicio'] ?>
                </td>
                <td align="center">
                    <?php echo $detalhe['ditdatatermino'] ?>
                </td>
                <td align="center">
                    <input type="hidden" name="dit_spivlrfinanceiroanterior[]"
                           value="<?php echo number_format($detalhe['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                    <?php echo campo_texto('dit_spivlritemexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemexecanterior'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('dit_spivlritemsobreobraexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('dit_spivlrinfsupervisor[]', 'N', $somenteLeitura, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, ' . $obrid . ', ' . $detalhe['icoid'] . ' )', number_format($detalhe['spivlrinfsupervisor'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('dit_spivlrfinanceiroinfsupervisor[]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, ' . $obrid . ', ' . $detalhe['icoid'] . ', \'numerico\' )', number_format($detalhe['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <input type="hidden" name="dit_spipercsobreobrainfsupervisor[]"
                           value="<?php echo number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                        <span>
                                        <?php echo number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                        </span>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php
                    echo
                    ($detalhe['umcdsc'] ?
                        campo_texto('dit_spivlrmetafisicasupervisor[' . $detalhe['ditid'] . '][]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalhe['ditmetafisica'] . ' )', number_format($detalhe['spivlrmetafisicasupervisor'], 2, ',', '.')) :
                        '&nbsp;');
                    ?>
                </td>

                <td align="right" style="color:#336EFF">
                    <input type="hidden"
                           value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?>"
                           name="dit_spidiferencavlr[]">
                    <span> <?php echo number_format($detalhe['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                </td>
                <td align="right" style="color:#336EFF">
                    <input type="hidden"
                           value="<?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?>"
                           name="dit_spidiferencaporcentagem[]">
                    <span> <?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                </td>
            </tr>
            </tbody>
            <?php
            foreach ($dadoDetalheFilho as $detalheFilho) {
                $detalheFilho['ditdatainicio'] = formata_data($detalheFilho['ditdatainicio']);
                $detalheFilho['ditdatatermino'] = formata_data($detalheFilho['ditdatatermino']);

                $detalheFilho['spipercsobreobrainfsupervisor'] = ($detalheFilho['spivlrfinanceiroinfsupervisor'] / $detalheFilho['ocrvalorexecucao']) * 100;
                ?>
                <tbody>
                <tr bgcolor="#D7EAF2"
                    id="<?php echo "tr_item_detalhamento_filho_" . $obrid . "_" . $detalheFilho['icoid'] . "_" . $detalheFilho['ditidpai'] . "_" . $detalheFilho['ditid'] ?>">
                    <td align="left">
                        <input type="hidden" name="ditid_filho[]" value="<?php echo $detalheFilho['ditid'] ?>">
                        <input type="hidden" name="dtf_spiid[]" value="<?php echo $detalheFilho['spiid'] ?>">
                        <input type="hidden" name="ico_spitotitemsupervisor[]" value="">
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif"
                                                                 border="0"><?php echo $detalheFilho['ditdsc'] ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <?php echo number_format($detalheFilho['ditvalor'], 2, ',', '.') ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <?php echo number_format($detalheFilho['ditpercsobreobra'], 2, ',', '.') ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <?php echo($detalheFilho['ditmetafisica'] ? number_format($detalheFilho['ditmetafisica'], 2, ',', '.') : '-') ?>
                    </td>
                    <td align="center">
                        <?php echo($detalheFilho['umcdsc'] ? $detalheFilho['umcdsc'] : '-') ?>
                    </td>
                    <td align="center">
                        <?php echo $detalheFilho['ditdatainicio'] ?>
                    </td>
                    <td align="center">
                        <?php echo $detalheFilho['ditdatatermino'] ?>
                    </td>
                    <td align="center">
                        <input type="hidden" name="dtf_spivlrfinanceiroanterior[]"
                               value="<?php echo number_format($detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                        <?php echo campo_texto('dtf_spivlritemexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemexecanterior'], 2, ',', '.')); ?>
                    </td>
                    <td align="center">
                        <?php echo campo_texto('dtf_spivlritemsobreobraexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                    </td>
                    <td align="center">
                        <?php echo campo_texto('dtf_spivlrinfsupervisor[]', 'N', 'S', '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, ' . $obrid . ', ' . $detalheFilho['icoid'] . ', ' . $detalheFilho['ditidpai'] . ' )', number_format($detalheFilho['spivlrinfsupervisor'], 2, ',', '.')); ?>
                    </td>
                    <td align="center">
                        <?php echo campo_texto('dtf_spivlrfinanceiroinfsupervisor[]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, ' . $obrid . ', ' . $detalheFilho['icoid'] . ', ' . $detalheFilho['ditidpai'] . ', \'numerico\'  )', number_format($detalheFilho['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <input type="hidden" name="dtf_spipercsobreobrainfsupervisor[]"
                               value="<?php echo number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                            <span>
                                            <?php echo number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                            </span>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <?php
                        if ($detalheFilho['umcdsc']) {
                            echo campo_texto('dtf_spivlrmetafisicasupervisor[' . $detalheFilho['ditid'] . '][]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalheFilho['ditmetafisica'] . ' )', number_format($detalheFilho['spivlrmetafisicasupervisor'], 2, ',', '.'));
                        } else {
                            echo '&nbsp;';
                        }
                        ?>
                    </td>

                    <td align="right" style="color:#336EFF">
                        <input type="hidden"
                               value="<?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?>"
                               name="dtf_spidiferencavlr[]">
                        <span> <?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <input type="hidden"
                               value="<?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?>"
                               name="dtf_spidiferencaporcentagem[]">
                        <span> <?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                    </td>

                </tr>
                </tbody>
            <?php
            }
        }
    }
    ?>
    <tbody>
    <tr id="tr_total_supervisao" bgcolor="#E3E3E3">
        <td align="right">
            <b>Total</b>
            <input type="hidden" name="vlrProjeto" id="vlrProjeto" value="<?php echo $total['vlrProjeto'] ?>">
        </td>
        <td align="right" style="color:#336EFF">
            <?php echo number_format($total['vlrObra'], 2, ',', '.') ?>
        </td>
        <td align="right" style="color:#336EFF">
            <?php echo number_format($total['percObra'], 2, ',', '.') ?>
        </td>
        <td align="center">
            &nbsp;
        </td>
        <td align="center">
            &nbsp;
        </td>
        <td align="center">
            &nbsp;
        </td>
        <td align="center">
            &nbsp;
        </td>
        <td align="right">
            &nbsp;
            <?php // echo campo_texto( 'total_spivlritemexecanterior', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00');   ?>
        </td>
        <td align="center">
            <?php echo campo_texto('total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
        </td>
        <td align="right">
            <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00');   ?>
        </td>
        <td align="right">
            &nbsp;
        </td>
        <td align="right" style="color:#336EFF" id="td_exec_obra">
            <?php echo number_format($total['percExecSobreObra'], 2, ',', '.') ?>
        </td>
        <td align="right">
            &nbsp;
        </td>
        <td align="right">
            &nbsp;
        </td>
        <td align="right">
            &nbsp;
        </td>
    </tr>
    </tbody>
    </table>
    </td>
    </tr>

    <?php if ($obraMi): ?>
        <tr>
            <td colspan="3" bgcolor="#DCDCDC" style="text-align: center; font-weight: bold">
                Serviços externos
            </td>
        </tr>
        <tr>
        <td colspan="3">
        <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
        <thead>
        <tr>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Descrição</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Valor (R$)</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>(%) Sobre a Obra</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Quantidade</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Unidade de Medida</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Data de Início</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Data de Término</b></td>
            <td colspan="3" rowspan="1" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Última Supervisão</b></td>
            <td colspan="5" rowspan="1" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Supervisão Atual</b></td>
        </tr>
        <tr>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>(%) do Item já Executado</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>(%) do Item já Executado sobre a Obra</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Quantidade já Executada</b></td>

            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Quantidade já Executada</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>(%) Supervisão</b></td>
            <td colspan="1" rowspan="2" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>Valor Executado<br></b></td>
            <td colspan="1" rowspan="1" valign="middle" align="center"
                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
                class="title"><b>(%) do Item já Executado sobre a Obra após Supervisão</b></td>
        </tr>
        </thead>
        <?php
        $total['vlrProjeto'] = 0;
        $total['vlrObra'] = 0;
        $total['vlrExecSobreObra'] = 0;
        $total['vlrExecSobreObraUlt'] = 0;
        $total['percExecSobreObraUlt'] = 0;
        foreach ($dadoEtapaF as $etapa) {
            $total['vlrProjeto'] = 0;

            $total['vlrObra'] += ($etapa['icovlritem'] * $etapa['itcquantidade']);

            if ($etapa['ocrvalorexecucao'] > 0) {
                $total['percObra'] = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;
            } else {
                $total['percObra'] = 0;
            }
            $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];

            if ($etapa['ocrvalorexecucao'] > 0) {
                $total['percExecSobreObra'] = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;
            } else {
                $total['percExecSobreObra'] = 0;
            }

            $total['vlrExecSobreObraUlt'] += $etapa['spivlrfinanceiroanterior'];

            if ($etapa['ocrvalorexecucao'] > 0) {
                $total['percExecSobreObraUlt'] = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
            } else {
                $total['percExecSobreObraUlt'] = 0;
            }
            //                        $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];
            $etapa['icodtinicioitem'] = formata_data($etapa['icodtinicioitem']);
            $etapa['icodterminoitem'] = formata_data($etapa['icodterminoitem']);

            if ($etapa['ocrvalorexecucao'] > 0) {
//                    $etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
            } else {
                $etapa['spipercsobreobrainfsupervisor'] = 0;
            }
            // Busca Filhos da Etapa
            $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
//                    $habil = (count($dadoDetalhe) ? 'N' : 'S');
            ?>
            <tbody>
            <tr bgcolor="#FDF8E7" id="<?php echo "tr_item_etapa_" . $obrid . "_" . $etapa['icoid'] ?>">
                <td align="left">
                    <input type="hidden" name="icoid[]" value="<?php echo $etapa['icoid'] ?>">
                    <input type="hidden" name="ico_spiid[]" value="<?php echo $etapa['spiid'] ?>">
                    &nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif"
                                           border="0"><?php echo $etapa['itcdesc'] ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php echo number_format($etapa['icovlritem'], 2, ',', '.') ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php echo number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                </td>
                <td align="right" style="color:#336EFF">
                    <?php echo number_format($etapa['itcquantidade'], 2, ',', '.') ?>
                </td>
                <td align="center">
                    <?php echo $etapa['umdeesc'] ?>
                </td>
                <td align="center">
                    <?php echo $etapa['icodtinicioitem'] ?>
                </td>
                <td align="center">
                    <?php echo $etapa['icodterminoitem'] ?>
                </td>
                <td align="center">
                    <input type="hidden" name="ico_spivlrfinanceiroanterior[]"
                           value="<?php echo number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                    <?php echo campo_texto('ico_spivlritemexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('ico_spivlritemsobreobraexecanterior[]', 'N', 'N', '', 7, 9, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo $etapa['spitotitemsupervisor'] ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('ico_spitotitemsupervisor[]', 'N', $somenteLeitura, '', 7, 9, '#######,##', '', 'right', '', 0, '', '', number_format($etapa['spitotitemsupervisor'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('ico_spivlrinfsupervisor[]', 'N', 'N', '', 7, 9, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                </td>
                <td align="center">
                    <?php echo campo_texto('ico_spivlrfinanceiroinfsupervisor[]', 'N', 'N', '', 11, 15, '', '', 'right', '', 0, '', '', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                </td>
                <td align="right" class="totpercentitem" style="color:#336EFF">

                    <input type="hidden" name="ico_spipercsobreobrainfsupervisor[]"
                           value="<?php echo number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.') ?>">
                                        <span>
        <?php echo number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.') ?>
                                        </span>
                </td>

            </tr>
            </tbody>
        <?php
        }
        ?>
        <tbody>
        <tr id="tr_total_supervisao_d" bgcolor="#E3E3E3">
            <td align="right">
                <b>Total</b>
                <input type="hidden" name="vlrProjeto" id="vlrProjeto"
                       value="<?php echo $total['vlrProjeto'] ?>">
            </td>
            <td align="right" style="color:#336EFF">
                <?php echo number_format($total['vlrObra'], 2, ',', '.') ?>
            </td>
            <td align="right" style="color:#336EFF">
                <?php echo number_format($total['percObra'], 2, ',', '.') ?>
            </td>
            <td align="center">
                &nbsp;
            </td>
            <td align="center">
                &nbsp;
            </td>
            <td align="center">
                &nbsp;
            </td>
            <td align="center">
                &nbsp;
            </td>
            <td align="center">
                &nbsp;
            </td>
            <td align="center">
                <?php echo campo_texto('total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
            </td>
            <td align="right">
                &nbsp;
                <?php // echo campo_texto( 'total_spivlritemexecanterior', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00');   ?>
            </td>
            <td align="right">
                <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00');  ?>
            </td>
            <td align="right">
                &nbsp;
            </td>
            <td align="right">
                &nbsp;
            </td>
            <td align="right" style="color:#336EFF" id="td_exec_obra_d">
                <? //= number_format($total['percExecSobreObra'], 2, ',', '.')  ?>
            </td>
        </tr>
        </tbody>
        </table>
        </td>
        </tr>
    <?php endif; ?>
    <?php $supjusticativaperc = $vistoriaEstrutura['supjusticativaperc']; ?>

    <tr id="tr_justificativa" <?php if (empty($vistoriaEstrutura['supjusticativaperc'])) { ?> class="hide" <?php } ?>>
        <td class="SubTituloDireita" valign="top">Justificativa da regressão de percentual</td>
        <td><?php echo campo_textarea('supjusticativaperc', 'S', $somenteLeitura, '', '100', '8', '5000', '', '', "Justificativa"); ?></td>
    </tr>
    <tr>
        <!--<td class="SubTituloDireita">Observação da Vistoria</td>-->
        <td class="SubTituloDireita" valign="top">Relatório Técnico do Acompanhamento</td>
        <td colspan="2">
            <?php
            $supobs = $vistoriaEstrutura['supobs'];
            $desc_relatorio = "No relatório deverão constar informações pertinentes a execução da obra, tais como:<br/><br/>&nbsp;&nbsp;&nbsp;<b>a.</b> O nome do Responsável Técnico da obra e o número da ART de execução.<br/>&nbsp;&nbsp;&nbsp;<b>b.</b> A existência ou não de todos os documentos necessários para a execução da obra, tais como: projetos, especificações, memoriais, ART´s, licenças, alvarás, caderno de encargos, diários de obras (preenchido e atualizado), dentre outros.<br/>&nbsp;&nbsp;&nbsp;<b>c.</b> Informações sobre o canteiro de obra (existência, organização e segurança do Trabalho).<br/>&nbsp;&nbsp;&nbsp;<b>d.</b> Atrasos na execução da obra e a necessidade ou não de aditivos.<br/>&nbsp;&nbsp;&nbsp;<b>e.</b> Comentário sobre a situação geral da obra e/ou de cada serviço da obra.<br/>&nbsp;&nbsp;&nbsp;<b>f.</b> Outros apontamentos.";
            ?>
            <?php echo campo_textarea('supobs', 'S', $somenteLeitura, '', '100', '8', '5000', '', '', $desc_relatorio); ?>
        </td>
    </tr>
    <?php
    if ($supid) {
        $sql = "SELECT
                			fot.*, arq.arqdescricao , c.itcdesc
                		FROM 
                			obras2.fotos AS fot
                        LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
                        LEFT JOIN obras2.itenscomposicaoobra i ON i.icoid = fot.icoid
                        LEFT JOIN obras2.itenscomposicao c ON c.itcid = i.itcid
                        WHERE 
                        	fot.obrid =" . $obrid . " AND
                        	supid=" . $supid . "
                        ORDER BY fotordem;";
        $fotos = ($db->carregar($sql));
    }
    ?>
    <tr id="tr10">
        <td colspan="2" align="center">
        </td>
    </tr>
    </table>
    <input type="hidden" name="supid" value="<?php echo $supid; ?>"/>
    <input type="hidden" name="hdn_fotos_galeria" id="hdn_fotos_galeria" value=""/>
    <input type="hidden" name="hdn_fotos_supervisao" id="hdn_fotos_supervisao" value=""/>
    <?php
    if (!$supid) {
        $sql = "SELECT
                arq.arqid
            FROM
                public.arquivo arq
            INNER JOIN
                obras2.arquivosobra oar ON oar.arqid = arq.arqid AND
                						   oar.aqostatus = 'A'
            INNER JOIN
                seguranca.usuario seg ON seg.usucpf = oar.usucpf
            INNER JOIN
                obras2.supervisao supv ON supv.obrid = oar.obrid
            WHERE
                oar.obrid = {$_SESSION['obras2']["obrid"]} AND
                tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . "
            AND
                (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
            ORDER BY
                arq.arqid";
    } else {
        $sql = "SELECT DISTINCT
                arq.arqid
            FROM
                obras2.fotos fot
            INNER JOIN
                public.arquivo arq ON arq.arqid = fot.arqid
            INNER JOIN
                obras2.obras_arquivos oar ON arq.arqid = oar.arqid
            WHERE
                fot.obrid = {$_SESSION['obras2']["obrid"]}
                AND fot.supid = {$supid}
                AND oar.tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . "
                AND oar.oarstatus = 'A'
            ORDER BY
                arq.arqid";
    }
    $arrFotosGaleria = $db->carregar($sql);

    // array que armazenará o arqid da Galeria
    $arrArqidGaleria = array();
    if (is_array($arrFotosGaleria)) {
        foreach ($arrFotosGaleria as $foto) {
            $arrArqidGaleria[] = $foto['arqid'];
        }
    }
    ?>
    <table class="tabela" id="tbl_graggable" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td bgcolor="#DCDCDC" colspan="2">
            <b>Fotos do Acompanhamento</b>
            <?php if ($supid): ?>
                *Arraste as imagens para a área de 'Fotos da Galeria' para adicioná-las à galeria. Após adicioná-las, clique no botão 'Salvar' para confirmar as alterações.
            <?php else: ?>
                *Após adicionar as fotos, clique no botão 'Salvar' para confirmar as alterações.
            <?php endif; ?>
        </td>
    </tr>
    <tr>
        <td <?php echo $supid ? "width=\"50%\"" : "colspan='2'" ?> >
            <fieldset class="field_fotos">
                <legend>Fotos do Acompanhamento</legend>
                <ul id="fotos_supervisao" class="div_fotos">
                    <?php if (is_array($fotos)): $arrFotos = $fotos; ?>
                        <?php $n = 1 ?>
                        <?php foreach ($arrFotos as $foto): ?>
                            <?php $pagina = floor($n / 16); ?>

                            <?php
                            $sql = "SELECT arqtipo, arqid  FROM public.arquivo
                                        WHERE arqid = '" . $foto['arqid'] . "'";
                            $dados = $db->pegaLinha($sql);

                            $imgend = APPRAIZ . 'arquivos/obras2/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                            if (is_file($imgend)) {
                                $img_max_dimX = 100;
                                $img_max_dimY = 85;

                                $imginfo = getimagesize($imgend);

                                $width = $imginfo[0];
                                $height = $imginfo[1];

                                if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
                                    if ($width > $height) {
                                        $w = $width * 0.9;
                                        while ($w > $img_max_dimX) {
                                            $w = $w * 0.9;
                                        }
                                        $w = round($w);
                                        $h = ($w * $height) / $width;
                                    } else {
                                        $h = $height * 0.9;
                                        while ($h > $img_max_dimY) {
                                            $h = $h * 0.9;
                                        }
                                        $h = round($h);
                                        $w = ($h * $width) / $height;
                                    }
                                } else {
                                    $w = $width;
                                    $h = $height;
                                }

                                $tamanho = " width=\"$w\" height=\"$h\" ";
                            } else {
                                $tamanho = "";
                            }
                            $eschema = ($foto['obrid_1'] ? 'obras' : 'obras2');
                            ?>
                            <li id="foto_<?php echo $foto['arqid'] ?>"
                                class="draggable<?php echo in_array($foto['arqid'], $arrArqidGaleria) ? " f_selected" : "" ?>">
                                <img title="<?= $foto['itcdesc'] ?>" <?php echo $tamanho; ?> class="img_foto"
                                     ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>', '<?php echo $pagina ?>')"
                                     src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?php echo $eschema ?>"/>
                            </li>
                            <?php $n++ ?>
                        <?php endforeach; ?>
                    <?php endif; ?>
                </ul>
            </fieldset>
        </td>
        <?php if ($supid): ?>
            <td>
                <fieldset class="field_fotos">
                    <legend>Fotos da Galeria</legend>
                    <ul id="fotos_galeria" class="div_fotos">
                        <?php if (is_array($arrFotosGaleria)): ?>
                            <?php $n = 1 ?>
                            <?php foreach ($arrFotosGaleria as $foto): ?>
                                <?php $pagina = floor($n / 16); ?>

                                <?php
                                $sql = "SELECT arqtipo, arqid  FROM public.arquivo
                                            WHERE arqid = '" . $foto['arqid'] . "'";
                                $dados = $db->pegaLinha($sql);

                                $imgend = APPRAIZ . 'arquivos/' . (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) . '/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                                if (is_file($imgend)) {
                                    $img_max_dimX = 100;
                                    $img_max_dimY = 85;

                                    $imginfo = getimagesize($imgend);

                                    $width = $imginfo[0];
                                    $height = $imginfo[1];

                                    if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
                                        if ($width > $height) {
                                            $w = $width * 0.9;
                                            while ($w > $img_max_dimX) {
                                                $w = $w * 0.9;
                                            }
                                            $w = round($w);
                                            $h = ($w * $height) / $width;
                                        } else {
                                            $h = $height * 0.9;
                                            while ($h > $img_max_dimY) {
                                                $h = $h * 0.9;
                                            }
                                            $h = round($h);
                                            $w = ($h * $width) / $height;
                                        }
                                    } else {
                                        $w = $width;
                                        $h = $height;
                                    }

                                    $tamanho = " width=\"$w\" height=\"$h\" ";
                                } else {
                                    $tamanho = "";
                                }
                                ?>

                                <li id="s_foto_<?php echo $foto['arqid'] ?>" class="nodraggable">
                                    <img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar"
                                         onclick="removerFotoGaleria('foto_<?php echo $foto['arqid'] ?>')"/>
                                    <img <?php echo $tamanho; ?> class="img_class"
                                                                 ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>', '<?php echo $pagina ?>')"
                                                                 src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85"/>
                                </li>
                                <?php $n++ ?>
                            <?php endforeach; ?>
                        <?php endif; ?>
                    </ul>
                </fieldset>
            </td>
        <?php endif; ?>
    </tr>
    <tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
            <?php
            //if($habilitado && $somenteLeitura == 'S' || possuiPerfil(PERFIL_SUPERVISORMEC)){
            ?>
            <input <?php echo(($habilitado) ? "" : "disabled=\"disabled\""); ?>
                type="button" name="add_fotos"
                onmouseover="return escape('Gerenciamento de fotos do acompanhamento (inserir, alterar e remover).');"
                value="<?php echo($supid ? 'Alterar Fotos' : 'Inserir Fotos') ?>"
                onclick="ImageComponent(<?php if (isset($supid)) {
                    echo "'?funcao=AtualizaFotos&supid=" . $supid . "'";
                } else {
                    echo "' '";
                } ?>);">
            <?php
            if ($supid):
                ?>
                <input <?php echo(($habilitado) ? "" : "disabled=\"disabled\""); ?>
                    type="button"
                    name="btn_salvar_fotos"
                    onmouseover="return escape('Salvar as alterações das fotos de acompanhamento e galeria.');"
                    value="Salvar Fotos"
                    onclick="salvarFotos()">
            <?php
            endif;
            ?>
        </td>
    </tr>
    <tr>
        <td colspan="2">
        </td>
    </tr>
    <?php
    if ($obraMi):
        $tecnologiaMi = new TecnologiaMi();
        $formularioTecnologiaMi = new FormularioTecnologiaMi();

        $tecnologiaMi->carregaPorObrid($obrid);
        $dadosFormTec = $formularioTecnologiaMi->listaComArquivoPorObridSupid($obrid, ($supid ? $supid : 0));

        if ($tecnologiaMi->tmiid && count($dadosFormTec) > 0):
            ?>
            <tr>
                <td bgcolor="#DCDCDC" colspan="2">
                    <b>Tecnologia: </b><?php echo $tecnologiaMi->tminome ?>
                </td>
            </tr>
            <?php
            foreach ($dadosFormTec as $formTec):
                ?>
                <tr>
                    <td>
                        <div>
                                <span id="ftiidTxt_<?php echo $formTec['ftiid'] ?>" style="font-weight: bold;">
            <?php echo $formTec['ftinome'] ?>
                                </span>
                            <?php
                            if ($formTec['arqid']):
                                ?>
                                <span style="padding-left: 30px;">
                                        Arquivo de exemplo:&nbsp;&nbsp;
                                        <img src="/imagens/principal.gif"
                                             onclick="DownloadArquivo(<?php echo $formTec['arqid'] ?>);"
                                             title="Fazer download do arquivo" border="0" style="cursor:pointer;">
                                    </span>
                            <?php
                            endif;
                            ?>
                        </div>
                        <div>
                            <input type="file" name="anexomi_<?php echo $formTec['ftiid'] ?>"
                                   id="anexomi_<?php echo $formTec['ftiid'] ?>">
                            <input type="hidden" name="arqidmi_<?php echo $formTec['ftiid'] ?>"
                                   id="arqidmi_<?php echo $formTec['ftiid'] ?>"
                                   value="<?php echo $formTec['arqidup'] ?>">
                            <input type="hidden" name="ftiid[]" id="ftiid_<?php echo $formTec['ftiid'] ?>"
                                   value="<?php echo $formTec['ftiid'] ?>">
                            <input type="hidden" name="ftiidObrig[]" id="ftiidObrig_<?php echo $formTec['ftiid'] ?>"
                                   value="<?php echo $formTec['ftisnobrigatorio'] ?>">
                        </div>
                        <?php
                        if ($formTec['arqidup']):
                            ?>
                            <div id="div_arqidmi_<?php echo $formTec['arqidup'] ?>"
                                 style="border: 1px dashed #000000; width: 50%; padding: 5px; padding-top: 2px; padding-right: 1px;">
                                <img src="/imagens/sair.gif"
                                     onclick="deletarAquivoFormularioTecnologia(<?php echo $formTec['arqidup'] ?>, <?php echo $formTec['ftiid'] ?>)"
                                     style="float: right; cursor: pointer;" title="Clique para deletar o anexo">
                                    <span onclick="DownloadArquivo(<?php echo $formTec['arqidup'] ?>)"
                                          style="cursor: pointer;" title="Clique para fazer download do anexo">
                                        <img src="/imagens/salvar.png">
                                        <?php
                                        echo $formTec['arqnomeup'] . '.' . $formTec['arqextensaoup']
                                        ?>
                                    </span>
                            </div>
                        <?php
                        endif;
                        ?>
                        <hr>
                    </td>
                </tr>
            <?php
            endforeach;
        endif;
    endif;
    ?>
    <tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
            <?php if ($habilitado && $somenteLeitura == 'S' && !$copy_sup) { ?>
                <input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer"
                       onclick="enviaFormulario('1<?php echo possuiPerfil(array(PFLCOD_SUPERVISOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_ADMINISTRADOR, PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_GESTOR_MEC)); ?>', <?php if (isset($supid)) {
                           echo "'?funcao=AtualizaFotos&supid=" . $supid . "'";
                       } else {
                           echo "' '";
                       } ?>);">
            <?php
            }
            if ($habilitado && $somenteLeitura == 'S' && $copy_sup) {
                ?>
                <input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer"
                       onclick="enviaFormulario('1<?php echo possuiPerfil(array(PFLCOD_SUPERVISOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_ADMINISTRADOR, PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_GESTOR_MEC)); ?>', '');">

            <?php
            }
            ?>

            <?php
            if ($_SESSION['obras2']['supid']):
                if ($supervisao->verificaValidacaoSupervisorUnidae($_SESSION['obras2']['supid']) == 'N' &&
                    (possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) &&
                    !$copy_sup
                ):
                    ?>
                    <input type="button" title="Validar Supervisão" value="Validar Supervisão" style="cursor: pointer"
                           onclick="validaSupervisao('<?php echo $_SESSION['obras2']['supid'] ?>');"/>
                <?php
                endif;
            endif;
            ?>
            <input type="button" value="Voltar" id="btnVoltar" style="cursor: pointer" onclick="history.go(-1);">
        </td>
    </tr>
    </table>
    </form>

    <div id="dialog_paralizadas2" title="Observação de obras paralisadas" style="width: 600px;display:none">
        <h4>Prezado(a),</h4>

        <p style="margin: 10px 0px;">

            Caso a razão da paralisação da obra seja falta de repasse por parte do FNDE, antes de cadastrar a vistoria
            de paralisação, entre em contato com a nossa equipe para verificação das razões do não pagamento.
            <br>
            Atente para os seguintes pontos:
            <br><br>
            1) Paralisação menor que 30 dias: Entre em contato com a nossa equipe para orientações por meio dos
            telefones: (61) 2022-5200 /2022-5402 / 2022-5199 /2022-5201, ou através do e-mail
            atendimento.monitora@fnde.gov.br
            <br><br>
            2) Paralisação maior que 30 dias: Cadastre a vistoria de paralisação de obra selecionando a razão da
            paralisação que melhor se aplica ( descrições abaixo) e descreva a justificativa. Informe no relatório de
            vistoria que ações estão sendo tomadas para a retomada da obra.
            <br><br>
            Motivos de paralisação:
            <br>
            <br>

        <li> Atraso no Pagamento Construtora: Paralisação por falta de pagamento à empresa executora.</li>
        <li> Descumprimento de Contrato: apuração de descumprimento de contrato</li>
        <li> Rescisão contratual: Contrato rescindido ou em processo de rescisão.</li>
        <li> Irregularidades na gestão anterior: Apuração de irregularidades ocorridas na gestão anterior.</li>
        <li> Embargo: Decisão judicial que determina a paralisação da obra. Anexar documentos comprobatórios na aba
            documentos.
        </li>
        <li> Medidas Administrativas do Estado/Município: Ausência de documentos legais (procedimentos administrativos
            internos) que impedem a continuidade da obra. Exemplo; Elaboração de aditivo
        </li>
        <li> Falha na execução de serviços: Há serviços não executados conforme o projeto ou as boas praticas de
            construção, que necessitam de correção para garantir a segurança e funcionalidade da edificação.
        </li>
        <li> Problemas de Infraestrutura: Execução incorreta ou insuficiente de serviços de infraestrutura para o inicio
            da obra. Exemplo: Terraplanagem mal executada, falta de infraestrutura de energia elétrica e água, dimensões
            divergentes de terreno, etc.
        </li>
        <li> Questões Climáticas: Atraso ou impedimentos na execução dos serviços causados por condições climáticas
            excepcionais.
        </li>


        </p>

        <p>Atenciosamente.<br/>
            Equipe PAR MEC/FNDE</p>
    </div>
<?php
IF ($copy_sup) {
//    $_SESSION['obras2']['supid'] = '';
//    $supid                       = '';
}
?>
    <script type="text/javascript">
    function validaSupervisao(supid) {
        $.ajax({
            type: 'POST',
            data: {requisicao: 'validaSupervisao', supid: supid},
            success: function (resposta) {
                alert(resposta);
                window.location.reload();
            }
        });
    }
    ;

    function managerServicoContratado(valor) {
        if (valor == 'N') {
            $('#tr_servico_contratado_justificativa').show();
        } else {
            $('#tr_servico_contratado_justificativa').hide();
            $('#obrcronogramaservicocontratadojustificativa').val('');
        }
    }
    ;

    $('.download').click(function () {
        $('#requisicao').val('download');
        $('#arqiddownload').val($(this).attr('id'));
        $('#formulario').submit();
    });

    DownloadArquivo = function (id) {
        $('#arqiddownload').val(id);
        $('#requisicao').val('download');
        $('#formulario').submit();
    };

    function deletarAquivoFormularioTecnologia(arqid, ftiid) {
        if (confirm('Deseja apagar este arquivo?')) {
            var url = window.location;
            $.ajax({
                url: url,
                async: false,
                type: 'post',
                data: {arqid: arqid, requisicao: 'apagarArquivoFormularioTecnologia'},
                success: function (dado) {
                    $('#arqidmi_' + ftiid).val('');
                    $('#div_arqidmi_' + arqid).remove();
                    alert('Operação realizada com sucesso!');
                }
            });
        }
    }

    function enviaFormulario(superuser, imagem) {

        var staid = window.document.getElementById('staid');
        if ($('#staid').val() == <?php echo STAID_PARALISADO ?>) {
            if ($('#tplid').val() == "") {
                alert("O preenchimento do campo tipo de paralisação é obrigatório");
                return false;
            }
        }

        if ($('#tplid').val() == <?php echo TPLID_CONTRATO_RESCINDIDO ?>) {
            if ($('input[name=arquivo]').val() == "" && $('input[name=arqid]').val() == "") {
                alert("O preenchimento do campo recisão do contrato é obrigatório.");
                return false;
            }
            if ($('input[name=arquivoboletim]').val() == "" && $('input[name=arqidboletim]').val() == "") {
                alert("O preenchimento do campo Boletim de medição acumulado é obrigatório.");
                return false;
            }
        }

        if ($('#tplid').val() == <?php echo TPLID_EMBARGO ?>) {
            if ($('input[name=documentoembargo]').val() == "" && $('input[name=arqiddocumentoembargo]').val() == "") {
                alert("O preenchimento do campo documento de embargo é obrigatório.");
                return false;
            }
        }

        if ($('#tplid').val() == <?php echo TPLID_OUTROS ?>) {
            if ($('[name=hprobs1]').val() == "") {
                alert("O preenchimento do campo observações da paralisação é obrigatório.");
                return false;
            }
        }

        if ($('#staid').val() == <?php echo STAID_CONCLUIDO ?>) {
            var obCalc = new Calculo();
            var percExecObra = $('#td_exec_obra').html();
            if (obCalc.comparar(percExecObra, '80,00', '<')) {
                alert('Para concluir o percentual de execução deve ser superior a 80%.');
                return false;
            }

        }

        if (!$('[name=supobs]').val()) {
            alert("O preenchimento do campo relatório técnico do acompanhamento é obrigatório.");
            return false;
        }

        console.log($('[name=supjusticativaperc]').hasClass("hide"));

        if (!$('[name=supjusticativaperc]').val() && !$('#tr_justificativa').hasClass("hide")) {
            alert("O preenchimento da justificativa é obrigatória.");
            return false;
        }


        if ($('[name=obrcronogramaservicocontratado]:checked').val() == 'N') {
            if ($('#obrcronogramaservicocontratadojustificativa').val() == '') {
                alert('O campo justificativa é obrigatório.\n');
                return false;
            }
        }

        if (parseFloat($('#td_exec_obra').text()) >= 100) {

            if ($('#staid').val() != 3) {
                alert("Você informou que a obra está com 100% de execução, nesse caso, a situação deve ser concluída.\n");
                return false;
            }
        }

        if (parseFloat($('[name=total_spivlritemsobreobraexecanterior]').val()) == 0) {

            if ($('#staid').val() == 2) {
                alert("Só é possível paralisar uma obra que tenha um percentual de execução maior que zero.\n");
                return false;
            }

        }

        <?php
        if ($tecnologiaMi->tmiid && count($dadosFormTec) > 0):
            ?>
        var msgFormTec = new Array();
        $('[id*=ftiid_]').each(function (i, obj) {
            if ($('#ftiidObrig_' + obj.value).val() == 't' && $('#anexomi_' + obj.value).val() == '' && $('#arqidmi_' + obj.value).val() == '') {
                msgFormTec.push($('#ftiidTxt_' + obj.value).html());
            }
        });
        if (msgFormTec.length > 0) {
            msgFormTec = 'Os seguintes anexos de tecnologia são de preenchimento obrigatório:\n\n' + msgFormTec.join('\n');
            alert(msgFormTec);
            return false;
        }
        <?php
    endif;
    ?>
        var num = $('#fotos_supervisao li');
        if (num.size() <= 0) {
            <?php if ($esdid == ESDID_OBJ_PARALISADO): ?>
            if (confirm("Deseja inserir ao menos uma foto antes de salvar?")) {
                ImageComponent(imagem);
                return false;
            }
            <?php else: ?>

            <?php
            if (possui_perfil(PFLCOD_GESTOR_UNIDADE)):
                ?>
            if ($('#staid').val() != 3) {
                alert('É necessário ao menos uma foto para a supervisão');
                return false;
            }
            <?php
        else:
            ?>
            alert('É necessário ao menos uma foto para a supervisão');
            return false;
            <?php
            endif;
            ?>


            <?php endif; ?>
//alert("Para cadastrar um acompanhamento, é necessário anexar ao menos uma foto!");
//return false;
        }


        if ($('#fotos_supervisao li').size() <= 0) {
            var arrFotosSupervisao = "";
        } else {
            var arrFotosSupervisao = $("#fotos_supervisao").sortable("serialize");
        }

        $("#hdn_fotos_supervisao").val(arrFotosSupervisao);
        if ($("#fotos_galeria")[0]) {
            var arrFotosGaleria = $("#fotos_galeria").sortable("serialize");
            $("#hdn_fotos_galeria").val(arrFotosGaleria);
        }

        if (validaVistoria("formulario", superuser) && validasituacao(staid.value, superuser)) {
            $("#salva_vistoria").attr('disabled', 'disabled');
            $('#requisicao').val('salvar');
            $('#formulario').submit();
//		document.getElementById("formulario").submit();
        }

    }

    function abreEvolucaoFinan(obrid) {
        janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=' + obrid, 800, 650);
    }


    function ImageComponent(params) {
        var a = window.open("plugins/component_foto.php" + params, "inserir_fotos", "scrollbars=yes,height=540,width=630");
        a.focus();
    }

    function inserirVistoriador(entid) {
        var caminho_atual = '/obras2/obras2.php';
        if (entid) {
            return windowOpen(caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&busca=entnumcpfcnpj&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen(caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A', 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }


    <?php
    if ($vistoriaEstrutura['staid'] == STAID_PARALISADO) {
        ?>
    $('#tr_tplid').show();
    <?php
    if ($historicoParalisacao['tplid'] == TPLID_OUTROS) {
        ?>
    $('#tr_hprobs').show();
    <?php
}if ($historicoParalisacao['tplid'] == TPLID_OUTROS) {
    ?>
    $('#tr_hprobs').show();
    <?php
}
if ($historicoParalisacao['tplid'] == TPLID_EMBARGO) {
    ?>
    $('#tr_stpid').show();
    $('#tr_hprobs').show();
    $('#tr_documentoembargo').show();
    $('#tr_hprobs .SubTituloDireita').html('INFORME AS AÇÕES ADOTADAS PARA RESOLUÇÃO DO EMBARGO');
    <?php
}
if ($historicoParalisacao['tplid'] == TPLID_CONTRATO_RESCINDIDO) {
    ?>
    $('#tr_arqid_boletim').show();
    $('#tr_arqid').show();
    $('#tr_hprobs').show();
    $('#tr_hprobs .SubTituloDireita').html('Medidas Administrativas e Judiciais Adotadas');
    <?php
}
 if ($historicoParalisacao['tplid'] == TPLID_ABANDONO_EMPRESA) {
    ?>
    $('#tr_hprobs').show();
    $('#tr_hprobs .SubTituloDireita').html('Medidas Administrativas e Judiciais Adotadas');
    <?php
  }



if ($historicoParalisacao['tplid'] == TPLID_OS_PARALISACAO) {
  ?>
    $('#tr_osparalisacao').show();
    <?php
}
if ($historicoParalisacao['tplid'] == TPLID_PARALISACAO) {
    ?>
    $('#tr_osexecucao').show();
    <?php
}
} elseif ($vistoriaEstrutura['staid'] == STAID_CONCLUIDO) {
?>
    $('#tr_arqid_planilha').show();
    $('#tr_arqid_termo').show();
    <?php
} elseif ($vistoriaEstrutura['staid'] == STAID_EXECUCAO) {
    ?>
    $('#tr_osexecucao').show();
    <?php
}
?>

    function ctrlTipoParalisacao(tplid) {
        $('#tr_hprobs').hide();
        $("#hprobs1").parent().find('img').show();


        if (tplid == <?php echo TPLID_EMBARGO ?>) {
            var url = window.location;
            $.ajax({
                url: url,
                async: false,
                type: 'post',
                data: {tplid: tplid, requisicao: 'carregaSubTipoParalisacao'},
                success: function (dado) {
                    $('#td_stpid').html(dado);
                }
            });
            $('#tr_stpid').show();
            $('#tr_arqid_boletim').hide();
            $('#tr_hprobs').show();
            $('[name=hprobs1]').val('');
            $('#tr_arqid').hide();
            $('#arquivo').val('');
            $('#tr_arqid_boletim').hide();
            $('#tr_osparalisacao').hide();
            $('#tr_documentoembargo').show();
            $('#tr_hprobs .SubTituloDireita').html('INFORME AS AÇÕES ADOTADAS PARA RESOLUÇÃO DO EMBARGO');

        } else if (tplid == <?php echo TPLID_OS_PARALISACAO ?>) {
            $('#tr_stpid').hide();
            $('#tr_arqid_boletim').hide();
            $('#td_stpid').html('');
            $('#tr_osparalisacao').show();
            $('#tr_arqid').hide();
            $('#arquivo').val('');
            $('#tr_documentoembargo').hide();
        } else if (tplid == <?php echo TPLID_OUTROS ?>) {
            $('#tr_stpid').hide();
            $('#tr_arqid_boletim').hide();
            $('#td_stpid').html('');
            $('#tr_hprobs').show();
            $('#tr_arqid').hide();
            $('#arquivo').val('');
            $('#tr_osparalisacao').hide();
            $('#tr_documentoembargo').hide();
            $('#tr_hprobs .SubTituloDireita').html('Observações da Paralisação');


        } else if (tplid == <?php echo TPLID_CONTRATO_RESCINDIDO ?>) {
            $('#tr_stpid').hide();
            $('#td_stpid').html('');
            $('#tr_hprobs').show();
            $('[name=hprobs1]').html('');
            $('#tr_arqid').show();
            $('#arquivo').val('');
            $('#tr_arqid_boletim').show();
            $('#tr_osparalisacao').hide();
            $('#tr_documentoembargo').hide();
            $('#tr_hprobs .SubTituloDireita').html('Medidas Administrativas e Judiciais Adotadas');
        } else if (tplid == <?php echo TPLID_ABANDONO_EMPRESA ?>) {
            $('#tr_stpid').hide();
            $('#td_stpid').html('');
            $('#tr_hprobs').show();
            $('[name=hprobs1]').html('');
            $('#tr_arqid').hide();
            $('#arquivo').val('');
            $('#tr_arqid_boletim').hide();
            $('#tr_osparalisacao').hide();
            $('#tr_documentoembargo').hide();
            $('#tr_hprobs .SubTituloDireita').html('Medidas Administrativas e Judiciais Adotadas');
        } else {
            $('#tr_arqid_boletim').hide();
            $('#tr_stpid').hide();
            $('#td_stpid').html('');
            $('#tr_hprobs').hide();
            $('[name=hprobs1]').val('');
            $('#tr_arqid').hide();
            $('#arquivo').val('');
            $('#tr_osparalisacao').hide();
            $('#tr_documentoembargo').hide();
        }


    }


    function verificasituacao(id) {

        var staid = window.document.getElementById('staid');
        $('#tr_arqid').hide();
        $('#tr_tplid').hide();
        $('#tr_osexecucao').hide();
        if (id == <?php echo STAID_PARALISADO ?>) {

            $('#tr_tplid').show();
            $('#tr_hprobs').hide();
            $('#tr1').show();
            $('#tr2').show();
            $('#tr3').show();
            $('#tr6').show();
            $('#tr7').show();
            $('#tr8').show();
            $('#tr9').show();
            $('#tr10').show();
            $('#tr_arqid_planilha').hide();
            $('#tr_arqid_termo').hide();
        } else if (id == 4) {

            $('#tr_elaboracao').show();
            $('#tr_elaboracao1').show();
            $('#tr_elaboracao2').show();
            $('#tr1').hide();
            $('#tr2').hide();
            $('#tr3').hide();
            $('#tr6').hide();
            $('#tr7').hide();
            $('#tr8').hide();
            $('#tr9').hide();
            $('#tr10').hide();
            $('#tr_arqid_planilha').hide();
            $('#tr_arqid_termo').hide();
        } else if (id == <?php echo STAID_LICITACAO ?>) {

            $('#tr_tplid').hide();
            $('#tr_hprobs').hide();
            $('#tr_arqid').hide();
            $('#tr_elaboracao').hide();
            $('#tr_elaboracao1').hide();
            $('#tr_elaboracao2').hide();
            $('#tr1').hide();
            $('#tr2').hide();
            $('#tr3').hide();
            $('#tr6').hide();
            $('#tr7').hide();
            $('#tr8').hide();
            $('#tr9').hide();
            $('#tr10').hide();
            $('#tr_arqid_planilha').hide();
            $('#tr_arqid_termo').hide();
            $('[name=obrlincambiental]').val('');
            $('[name=obraprovpatrhist]').val('');
            $('[name=obrdtprevprojetos]').val('');
            $('[name=tplid]').val('');
            $('[name=hprobs1]').val('');
        } else if (id == <?php echo STAID_PREPARATORIA ?>) {

            $('#tr1').hide();
            $('#tr2').hide();
            $('#tr3').hide();
            $('#tr6').hide();
            $('#tr7').hide();
            $('#tr8').hide();
            $('#tr9').hide();
            $('#tr10').hide();
            $('#tr_arqid_planilha').hide();
            $('#tr_arqid_termo').hide();
        } else if (id == <?php echo STAID_CONCLUIDO ?>) {

            $('#tr1').hide();
            $('#tr2').hide();
            $('#tr3').hide();
            $('#tr6').hide();
            $('#tr7').hide();
            $('#tr8').hide();
            $('#tr9').show();
            $('#tr10').show();
            $('#tr_arqid_planilha').show();
            $('#tr_arqid_termo').show();
        } else {


            $('#tr_tplid').hide();
            $('#tr_hprobs').hide();
            $('#tr_arqid').hide();
            $('#tr_elaboracao').hide();
            $('#tr_elaboracao1').hide();
            $('#tr_elaboracao2').hide();
            $('#tr1').show();
            $('#tr2').show();
            $('#tr3').show();
            $('#tr6').show();
            $('#tr7').show();
            $('#tr8').show();
            $('#tr9').show();
            $('#tr10').show();
            $('#tr_arqid_planilha').hide();
            $('#tr_arqid_termo').hide();
            $('[name=obrlincambiental]').val('');
            $('[name=obraprovpatrhist]').val('');
            $('[name=obrdtprevprojetos]').val('');
            $('[name=tplid]').val('');
            $('[name=hprobs1]').val('');
        }
        if (id == <?php echo STAID_EXECUCAO ?>) {
            $('#tr_osexecucao').show();
        }


        if ($("select[name=staid]").val() == <?php echo STAID_PARALISADO ?>) {
            $("#dialog_paralizadas2").dialog({width: 600});
        }


    }


    <?php
    $arPflcod = array(PFLCOD_SUPERVISOR_MEC,
        PFLCOD_ADMINISTRADOR,
        PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
        PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
        PFLCOD_SUPERVISOR_UNIDADE,
        PFLCOD_GESTOR_UNIDADE,
        PFLCOD_GESTOR_MEC);
    if (possuiPerfil($arPflcod)) {
        ?>
    $(function () {
        $("#fotos_supervisao").sortable({
            placeholder: "draggable_space",
            connectWith: "#fotos_supervisao",
            cancel: ".nodraggable"
        });
        $("#fotos_galeria").droppable({
            drop: function (event, ui) {
                if (!$("#s_" + ui.draggable.attr("id")).html()) {
                    $("<li id=\"s_" + ui.draggable.attr("id") + "\" class=\"nodraggable\" ><img src=\"../imagens/fechar.jpeg\" title=\"Remover Foto\" class=\"fechar\" onclick=\"removerFotoGaleria('" + ui.draggable.attr("id") + "')\" />" + ui.draggable.html() + "</li>").appendTo(this);
                    $("#s_" + ui.draggable.attr("id") + " .img_foto").attr("style", "margin-top:-15px");
                    $("#" + ui.draggable.attr("id")).attr("class", "draggable f_selected");
                }
            }
        }).sortable({
            cancel: ".nodraggable"
        });
    });
    function removerFotoGaleria(foto) {
        $("#s_" + foto).remove();
        $("#" + foto).attr("class", "draggable");
    }
    <?php } ?>

    function abrirGaleria(arqid, pagina) {
        window.open("../slideshow/slideshow/obras2_galeriaGaleriaFotos.php?&arqid=" + arqid + "&supid=<?php echo $supid ?>", "imagem", "width=850,height=600,resizable=yes");
//        window.open("../slideshow/slideshow/index.php?pagina=" + pagina + "&arqid=" + arqid,"imagem","width=850,height=600,resizable=yes");
    }

    function salvarFotos() {
        $("[name=btn_salvar_fotos]").val("Carregando...");
        $("[name=btn_salvar_fotos]").attr("disabled", "disabled");
        var arrFotosSupervisao = $("#fotos_supervisao").sortable("serialize", {key: 'fotosSupervisao[]'});
        var arrFotosGaleria = $("#fotos_galeria").sortable("serialize", {key: 'fotosGaleria[]'});

        var fotos = arrFotosSupervisao.split('&');
        var arqids = [];
        for (var x = 0; x < fotos.length; x++) {
            arqids.push(fotos[x].split('=')[1]);
        }

        var fotos2 = arrFotosGaleria.split('&');
        var arqids2 = [];
        for (var x = 0; x < fotos2.length; x++) {
            arqids2.push(fotos2[x].split('=')[1]);
        }

        var url = window.location + "&ajaxFotosGaleria=true";
        $.ajax({
            type: "POST",
            url: url,
            data: {'fotosSupervisao': arqids, 'fotosGaleria': arqids2},
            success: function (data) {
                if (data == "true") {
                    alert("Operação realizada com sucesso!");
                    $("[name=btn_salvar_fotos]").val("Salvar Fotos");
                    $("[name=btn_salvar_fotos]").attr("disabled", "");
                } else {
                    alert("Não foi possível realizar a operação!");
                    $("[name=btn_salvar_fotos]").val("Salvar Fotos");
                    $("[name=btn_salvar_fotos]").attr("disabled", "");
                }
            }
        });
    }

    </script>
    <style>
        .div_fotos {
            list-style-type: none;
            margin: 0;
            padding: 0;
            padding-top: 3px
        }

        .div_fotos li {
            font-size: 1.2em;
            height: 110px;
            height: 90px;
            padding: 1px
        }

        html > body .div_fotos li {
            height: 80px;
            line-height: 1.2em;
        }

        .field_fotos {
            padding: 10px;
        }

        .div_fotos {
            height: 300px;
            overflow: auto;
        }

        .draggable {
            width: 110px;
            height: 90px;
            margin: 3px;
            border: solid 1px black;
            float: left;
            cursor: move;
            text-align: center;
            background-color: #FFFFFF
        }

        .nodraggable {
            width: 110px;
            height: 90px;
            margin: 3px;
            border: solid 1px black;
            float: left;
            text-align: center;
            background-color: #FFFFFF
        }

        .draggable_space {
            line-height: 1.2em;
            width: 110px;
            height: 90px;
            margin: 3px;
            float: left;
            cursor: pointer;
            background-color: #CCCCCC
        }

        .f_selected {
            border: solid 1px red;
        }

        .fechar {
            position: relative;
            margin-left: 105px;
            top: -8px;
            cursor: pointer
        }

        .img_foto {
            z-index: 2;
        }

        .img_class {
            margin-top: -15px;
        }
    </style>

    <script type="text/javascript">
    function ctrlObs(qstid, val) {
        if (val == true) {
            $('#div_obs_' + qstid).show();
        } else {
            $('#div_obs_' + qstid).hide()
                .find('textarea')
                .val('');
        }
    }

    function ctrlImg(qstid, val) {
        if (val == true) {
            $('#div_arq_' + qstid).show();
        } else {
            $('#div_arq_' + qstid).hide()
                .find('input:file')
                .val('');
        }
    }

    var totalArq = new Array();
    function addArquivo(qstid) {
        totalArq[qstid] = (totalArq[qstid] ? totalArq[qstid] + 1 : 1);
        $('<br>').appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
        $('#div_arq_' + qstid).find('[type=file]:eq(0)')
            .clone()
            .val('')
            .attr('name', 'img_' + qstid + '_' + totalArq[qstid])
            .appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
        $('#totImg_' + qstid).val(totalArq[qstid]);
    }

    function calculoSupervisao() {
        var objClass = this;
        var obCalc = new Calculo();
        var percentTotal = new Number(0);
        var itemEdicao = '';
        var vlrItemAnterior = '';
        this.managerEdificacao = function (obj, tipo) {
            obj = obj || '';
            tipo = tipo || 'percentual';
            var vlrItemObra = $(obj).parents('tr:eq(0)')
                .find('td:eq(1)')
                .html();
            var vlrObj = mascaraglobal("[###.]###,##", obj.value);
            if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>')) {
                obj.value = '100,00';
            } else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>')) {
                obj.value = vlrItemObra;
            }

            this.formatarNumeroMonetario(obj);
            this.calculaPercentLinha(obj, tipo);
            calculaTotalSupervisao();
        }

        this.managerEtapa = function (obj, eobid, tipo) {
            obj = obj || '';
            tipo = tipo || 'percentual';
            var vlrItemObra = $(obj).parents('tr:eq(0)')
                .find('td:eq(1)')
                .html();
            var vlrObj = mascaraglobal("[###.]###,##", obj.value);
            if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>')) {
                obj.value = '100,00';
            } else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') && tipo != 'percentual') {
                obj.value = vlrItemObra;
            }

            this.formatarNumeroMonetario(obj);
            this.calculaPercentLinha(obj, tipo);
//        calculaNivelEdificacao( eobid );
            calculaTotalSupervisao();
        }

        this.managerDetalhamento = function (obj, eobid, icoid, tipo) {

            obj = obj || '';
            tipo = tipo || 'percentual';
            var vlrItemObra = $(obj).parents('tr:eq(0)')
                .find('td:eq(1)')
                .html();
            var vlrObj = mascaraglobal("[###.]###,##", obj.value);
            if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>')) {
                obj.value = '100,00';
            } else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>')) {
                obj.value = vlrItemObra;
            }

            this.formatarNumeroMonetario(obj);
            this.calculaPercentLinha(obj, tipo);
            calculaNivelEtapa(eobid, icoid);
//        calculaNivelEdificacao( eobid );
            calculaTotalSupervisao();
        }


        this.managerDetalhamentoFilho = function (obj, eobid, icoid, ditidPai, tipo) {
            obj = obj || '';
            tipo = tipo || 'percentual';
            var vlrItemObra = $(obj).parents('tr:eq(0)')
                .find('td:eq(1)')
                .html();
            var vlrObj = mascaraglobal("[###.]###,##", obj.value);
            if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>')) {
                obj.value = '100,00';
            } else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>')) {
                obj.value = vlrItemObra;
            }

            this.formatarNumeroMonetario(obj);
            this.calculaPercentLinha(obj, tipo);
            calculaNivelDetalhamento(eobid, icoid, ditidPai);
//        calculaNivelEtapa( eobid, icoid );
//        calculaNivelEdificacao( eobid );
            calculaTotalSupervisao();
        }


        this.calculaPercentLinha = function (obj, tipo) {
            var vlrItemObra = $(obj).parents('tr:eq(0)')
                .find('td:eq(1)')
                .html();
            // CALCULA: "Valor Executado" a partir do "(%) Supervisão"
            if (tipo == 'percentual') {
                var calcVlrItemSup = obCalc.operacao(vlrItemObra, obj.value, '*', 20);
                calcVlrItemSup = obCalc.operacao(calcVlrItemSup, 100, '/', 20);
                $(obj).parents('tr:eq(0)')
                    .find('td:eq(10) input')
                    .val(this.formatarNumeroMonetario({value: new Number(calcVlrItemSup).toFixed(2)}));
                // CALCULA: "(%) Supervisão" a partir do "Valor Executado"
            } else {
                var calcPercItemSup = obCalc.operacao(obj.value, vlrItemObra, '/', 20);
                calcPercItemSup = obCalc.operacao(calcPercItemSup, 100, '*', 20);
                $(obj).parents('tr:eq(0)')
                    .find('td:eq(9) input')
                    .val(this.formatarNumeroMonetario({value: new Number(calcPercItemSup).toFixed(2)}));
            }

// CALCULA: "(%) do Item já Executado sobre a Obra após Supervisão" a partir do "Valor Executado"
            var vlrItemSup = $(obj).parents('tr:eq(0)')
                .find('td:eq(10) input')
                .val();
            var vlrProjeto = $('#vlrProjeto').val();
            var calcPercItemSupSobreObra = obCalc.operacao(vlrItemSup, vlrProjeto, '/', 20);
            calcPercItemSupSobreObra = obCalc.operacao(calcPercItemSupSobreObra, 100, '*', 20);
            $(obj).parents('tr:eq(0)')
                .find('td:eq(11) span')
                .html(this.formatarNumeroMonetario({value: new Number(calcPercItemSupSobreObra).toFixed(2)}));
            $(obj).parents('tr:eq(0)')
                .find('td:eq(11) input')
                .val(this.formatarNumeroMonetario({value: new Number(calcPercItemSupSobreObra).toFixed(2)}));
            // CALCULA: "diferença Valor Executado
            var vlrItemSup = $(obj).parents('tr:eq(0)')
                .find('td:eq(10) input')
                .val();
            var vlrAnterior = $(obj).parents('tr:eq(0)')
                .find('td:eq(7) input[type="hidden"]')
                .val();
            var calcDiferencaValor = obCalc.operacao(vlrItemSup, vlrAnterior, '-', 20);
            var sinal = "";
            if (new Number(calcDiferencaValor) < 0) {
                sinal = "-";
            } else {
                sinal = "";
            }

            $(obj).parents('tr:eq(0)')
                .find('td:eq(13) span')
                .html(sinal + "" + this.formatarNumeroMonetario({value: new Number(calcDiferencaValor).toFixed(2)}));
            $(obj).parents('tr:eq(0)')
                .find('td:eq(13) input')
                .val(sinal + "" + this.formatarNumeroMonetario({value: new Number(calcDiferencaValor).toFixed(2)}));
            // CALCULA: "diferença % Executado
            var percItemSup = $(obj).parents('tr:eq(0)')
                .find('td:eq(9) input')
                .val();
            var percAnterior = $(obj).parents('tr:eq(0)')
                .find('td:eq(7) input[type="text"]')
                .val();
            var calcDiferencaPerc = obCalc.operacao(percItemSup, percAnterior, '-', 20);
            $(obj).parents('tr:eq(0)')
                .find('td:eq(14) span')
                .html((new Number(calcDiferencaPerc).toFixed(2)).replace(".", ","));
            $(obj).parents('tr:eq(0)')
                .find('td:eq(14) input')
                .val((new Number(calcDiferencaPerc).toFixed(2)).replace(".", ","));
        }

        this.formatarNumeroMonetario = function (obj) {
            var valor = new String(obCalc.retiraZeroEsquerda(new String(obj.value)));
            if (valor.length == 2) {
                valor = '0' + valor;
            } else if (valor.length == 1) {
                valor = '00' + valor;
            } else if (valor.length == 0) {
                valor = '000';
            }

            obj.value = mascaraglobal("[###.]###,##", valor);
            return obj.value;
        }

        this.managerMetaFisica = function (obj, limiteMetaFisica) {
            obj = obj || '';
            var vlrObj = this.formatarNumeroMonetario({value: new String(obj.value)});
            obj.value = vlrObj;
            if (obCalc.comparar(new String(vlrObj), limiteMetaFisica, '>')) {
                obj.value = this.formatarNumeroMonetario({value: new Number(limiteMetaFisica).toFixed(2)});
            }

        }

        function calculaNivelEdificacao(eobid) {
            var idTrEdicacao = 'tr_item_edificacao_';
            var idTrEtapa = 'tr_item_etapa_';
            var vlrEtapaSupTot = 0;
            $('[id^=' + idTrEtapa + eobid + '_]').each(function (i, obj) {
                vlrEtapaSupTot = obCalc.operacao(vlrEtapaSupTot, $(obj).find('td:eq(10) input').val(), '+', 20);
            });
            $('#' + idTrEdicacao + eobid).find('td:eq(10) input')
                .val(objClass.formatarNumeroMonetario({value: new Number(vlrEtapaSupTot).toFixed(2)}));
            objClass.managerEdificacao($('#' + idTrEdicacao + eobid).find('td:eq(10) input')[0], 'numerico');
        }

        function calculaNivelEtapa(eobid, icoid) {
            var idTrEtapa = 'tr_item_etapa_';
            var idTrDetalhamento = 'tr_item_detalhamento_';
            var vlrDetalhamentoSupTot = 0;
            $('[id^=' + idTrDetalhamento + eobid + '_' + icoid + '_]').each(function (i, obj) {
                vlrDetalhamentoSupTot = obCalc.operacao(vlrDetalhamentoSupTot, $(obj).find('td:eq(10) input').val(), '+', 20);
            });
            $('#' + idTrEtapa + eobid + '_' + icoid).find('td:eq(10) input')
                .val(objClass.formatarNumeroMonetario({value: new Number(vlrDetalhamentoSupTot).toFixed(2)}));
            objClass.managerEtapa($('#' + idTrEtapa + eobid + '_' + icoid).find('td:eq(10) input')[0], eobid, 'numerico');
        }

        function calculaNivelDetalhamento(eobid, icoid, ditidPai) {
            var idTrDetalhamento = 'tr_item_detalhamento_';
            var idTrDetalhamentoFilho = 'tr_item_detalhamento_filho_';
            var vlrDetalhamentoFilhoSupTot = 0;
            $('[id^=' + idTrDetalhamentoFilho + eobid + '_' + icoid + '_' + ditidPai + '_]').each(function (i, obj) {
                vlrDetalhamentoFilhoSupTot = obCalc.operacao(vlrDetalhamentoFilhoSupTot, $(obj).find('td:eq(10) input').val(), '+', 20);
            });
            $('#' + idTrDetalhamento + eobid + '_' + icoid + '_' + ditidPai).find('td:eq(10) input')
                .val(objClass.formatarNumeroMonetario({value: new Number(vlrDetalhamentoFilhoSupTot).toFixed(2)}));
            objClass.managerDetalhamento($('#' + idTrDetalhamento + eobid + '_' + icoid + '_' + ditidPai).find('td:eq(10) input')[0], eobid, icoid, 'numerico');
        }

        function calculaTotalSupervisao() {
//        var idTrEdificacao      = 'tr_item_edificacao_';
            var idTrEtapa = 'tr_item_etapa';
            var idTrTotal = 'tr_total_supervisao';
            var vlrProjeto = $('#vlrProjeto').val();
            var vlrEdificacaoSupTot = 0;
            var percSupTot = 0;
//        $('[id^=' + idTrEdificacao + '_]').each(function (i, obj){
            $('[id^=' + idTrEtapa + '_]').each(function (i, obj) {
                vlrEdificacaoSupTot = obCalc.operacao(vlrEdificacaoSupTot, $(obj).find('td:eq(10) input').val(), '+', 20);
            });
            percSupTot = obCalc.operacao(vlrEdificacaoSupTot, vlrProjeto, '/', 20);
            percSupTot = obCalc.operacao(percSupTot, 100, '*', 20);
            $('#' + idTrTotal).find('td:eq(11)')
                .html(objClass.formatarNumeroMonetario({value: new Number(percSupTot).toFixed(2)}));

            var valor_inicial = $('input[name=total_spivlritemsobreobraexecanterior]').val();
            var valor_final = $('#td_exec_obra').html();

            if (valor_inicial > valor_final) {
                $("#tr_justificativa").removeClass('hide');
            } else {
                $("#tr_justificativa").addClass('hide');
            }


        }

    }

    calculoSupervisao = new calculoSupervisao();
    $(function () {

        strVlToFloat = function (vl) {
            if (typeof (vl) === 'string') {
                var valorFloat = 0;
                var strVl = vl.replace(/\./g, '');
                strVl = strVl.replace(/\,/g, '.');
                valorFloat = parseFloat(strVl);
            } else {
                valorFloat = parseFloat(vl);
            }
            return valorFloat;
        };

        floatToStr = function (vl) {
            if (typeof (vl) === 'number') {
                vl = vl.toFixed(2);
                var floatAtualizado = vl.toString().replace(/\./g, ',');
                if (floatAtualizado.search(',') === -1) {
                    floatAtualizado = floatAtualizado + ',00';
                }
            } else {
                floatAtualizado = vl;
            }

            return floatAtualizado;
        };

        calculoQtdTotal = function (obj) {

            var obCalc = new Calculo();
            var qtd = $(obj).val().replace(',', '.');
            var maxQtd = $(obj).parents('tr:eq(0)').find('td:eq(3)').text().trim().replace(',', '.');

            var total = $('#tr_total_supervisao_d td:eq(1)').text().trim();
            var valorItem = $(obj).parents('tr:eq(0)').find('td:eq(1)').text().trim();

            // Total de acordo com o total
            valor = obCalc.operacao(valorItem, qtd, '*', 20);
            valor = obCalc.operacao(valor, 100, '*', 20);
            valor = obCalc.operacao(valor, total, '/', 20);
            valor = calculoSupervisao.formatarNumeroMonetario({value: new Number(valor).toFixed(2)});
            valor = valor + '<input type="hidden" name="ico_spipercsobreobrainfsupervisor[]" value="' + valor + '">';
            $(obj).parents('td:eq(0)').next().next().next().html(valor);
            // Total do item
            valor = (qtd * 100) / maxQtd;
            $(obj).parents('td:eq(0)').next().find('input').val(calculoSupervisao.formatarNumeroMonetario({value: new Number(valor).toFixed(2)}));
            valor = obCalc.operacao(qtd, valorItem, '*', 2);
            $(obj).parents('td:eq(0)').next().next().find('input').val(calculoSupervisao.formatarNumeroMonetario({value: new Number(valor).toFixed(2)}));
            valor = 0;
            $('.totpercentitem').each(function (i) {
                valor = obCalc.operacao(valor, $(obj).text().trim(), '+', 2);
            });
            $('#td_exec_obra_d').html(valor);
        }

        calculoMaxQtd = function (obj) {
            var qtd = strVlToFloat($(obj).val());
            var maxQtd = strVlToFloat($(obj).parents('tr:eq(0)').find('td:eq(3)').text().trim());

            if (qtd > maxQtd) {
                qtd = floatToStr(maxQtd);
                $(obj).val(qtd);
            }
        };

        $("input[name='ico_spitotitemsupervisor\[\]']").blur(function () {
            calculoMaxQtd(this);
            calculoQtdTotal(this);
        });

        $("input[name='ico_spitotitemsupervisor\[\]']").keyup(function () {
            calculoMaxQtd(this);
            calculoQtdTotal(this);
        });
    });
    </script>


    <script type="text/javascript">
        $(function () {
            $('#staid').change(function (e) {
                $('#tplid').change();
            });

            $('#stpid').live("change", function () {


                if ($(this).val() == 2) {
                    $('textarea[name=hprobs1]').addClass("obrigatorio");
                    $("#hprobs1").parent().find('img').show();
                } else {
                    $('textarea[name=hprobs1]').removeClass("obrigatorio");
                    $("#hprobs1").parent().find('img').hide();
                }
            });
        })

        $(function () {
            $('#td_exec_obra').change(function (e) {
                alert($(this).html);
            })
        })
    </script>




<?php
$objObras = new Obras($obrid);
$blockEdicao = $objObras->verificaObraVinculada();
if ($blockEdicao) {
    echo '<script type="text/javascript">';
    echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#btnVoltar').attr('disabled', false);
                   }
                 ";
    echo '</script>';
}
?>