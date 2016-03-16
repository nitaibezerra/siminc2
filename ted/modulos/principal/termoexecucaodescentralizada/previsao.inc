<?php
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de Objetos
$prevOrcamentaria = new Ted_Model_PrevisaoOrcamentaria();
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$fm = new Simec_Helper_FlashMessage('ted/previsao');

//Url base
$urlBse = 'ted.php?modulo=principal/termoexecucaodescentralizada';
$queryString = array(
    'acao' => 'A',
    'ted' => Ted_Utils_Model::capturaTcpid()
);

//Url base para chamadas assincronas
$urlPage = $urlBse.'/previsao&acao=A';

Ted_Utils_Model::userIsAllowed();

/**
 * Salva a previsão orçamentária
 */
if ($_POST['proid'] && !Ted_Utils_Model::isAjax()) {
    $lote = $prevOrcamentaria->prepareData($_POST);
    $prevOrcamentaria->updateMonths($lote[0]['crdmesexecucao']);
    foreach ($lote as $item) {
        if (array_key_exists('notacredito', $item) && empty($item['notacredito'])) {
            $prevOrcamentaria->salvarDados($item);
        }
    }

    $fm->addMensagem('Previsão Orçamentária salva com sucesso!!', Simec_Helper_FlashMessage::SUCESSO);

    //Faz o redirecionamento após o processamento do formulário
    if (isset($_POST['formacao']) && ($_POST['formacao'] == 'submitcontinue')) {
        echo Ted_Utils_Model::redirect($urlBse.'/parecer', $queryString);
    } else {
        echo Ted_Utils_Model::redirect($urlBse.'/previsao', $queryString);
    }
}

/**
 * Salva a Nota de Crédito adicionada
 */
if (Ted_Utils_Model::isAjax() && ($_GET['action'] == 'salvarNotaCredito')) {
    if ($prevOrcamentaria->salvarNotaCredito()) {
        $fm->addMensagem('Nota de Crédito adicionada com sucesso!!', Simec_Helper_FlashMessage::SUCESSO);
        echo true;
    } else {
        $fm->addMensagem('Falha ao tentar adicionar Nota de Crédito!', Simec_Helper_FlashMessage::ERRO);
        echo false;
    }
    die;
}

/**
 * Registra a transação de transferencia de crédito
 */
if (Ted_Utils_Model::isAjax() && ($_GET['action'] == 'transferencia')) {
    if ($prevOrcamentaria->transfereCredito($_POST)) {
        $fm->addMensagem('Crédito transferido com sucesso!!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha ao tentar transferir crédito!', Simec_Helper_FlashMessage::ERRO);
    }
    die;
}

if (Ted_Utils_Model::isAjax() && ($_GET['action'] == 'checkSaldo')) {
    $haveSaldo = $prevOrcamentaria->haveCash(Ted_Utils_Model::capturaTcpid(), true);
    $valor = str_replace('.', '', $_GET['valor']);
    $valor = str_replace(',', '.', $valor);

    header("Content-Type: application/json");
    if ($haveSaldo >= $valor) {
        echo simec_json_encode(array('success' => true));
    } else {
        echo simec_json_encode(array('fail' => true));
    }
    die;
}

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

if (isset($_GET['action']) && Ted_Utils_Model::isAjax()) {
    switch ($_GET['action']) {
        case 'getPrevisao':
            $dados = $prevOrcamentaria->getPrevisao((int) $_GET['proid']);
            header("Content-Type: application/json");
            echo simec_json_encode($dados);
            die();
        case 'getMesesExecucao':
            $dados = $prevOrcamentaria->getIntervaloMeses();
            echo $dados;
            die();
        case 'getPlanoInterno':
            $dados = $prevOrcamentaria->getPlanoInterno((int) $_GET['ptrid']);
            echo $dados;
            die();
        case 'getAcaoPtrid':
            $dados = $prevOrcamentaria->getAcaoPtrid((int) $_GET['ptrid']);
            echo $dados;
            die();
        case 'getDescricaoAcao':
            $dados = $prevOrcamentaria->getDescricaoAcao((int) $_GET['ptrid']);
            echo $dados;
            die();
        case 'getNaturezaDespesa':
            $dados = $prevOrcamentaria->getNaturezaDespesa();
            echo $dados;
            die();
        case 'searchPtres':
            $dados = $prevOrcamentaria->searchPtres((string) $_GET['query']);
            echo $dados;
            die();
        case 'getPtres':
            $currentYear = (isset($_GET['onlypopulate'])) ? false : true;
            $dados = $prevOrcamentaria->getPtres($currentYear);
            echo $dados;
            die();
        case 'pegaExtratoNotaCredito':
            $dados = $prevOrcamentaria->pegaExtratoNotaCredito($_GET['nc']);
            header("Content-Type: application/json");
            echo $dados;
            die();
        case 'deletePrevisao':
            header("Content-Type: application/json");
            $transferencia = isset($_GET['deletatransferencia']) ? true : false;
            if ($prevOrcamentaria->deletePrevisao((int) $_GET['proid'], $transferencia)) {
                echo simec_json_encode(array('success' => true));
            } else {
                echo simec_json_encode(array('fail' => true));
            }
            die();
    }
}

$situacao = Ted_Utils_Model::pegaSituacaoTed();
$templateSet = ($situacao['esdid'] == EM_ANALISE_OU_PENDENTE) ? '#table-row-all-privileges' : '#table-row';

if ($situacao['esdid'] == EM_DESCENTRALIZACAO && Ted_Utils_Model::possuiPerfil(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA))) {
    $templateSet = '#table-row-all-privileges';
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<link rel="stylesheet" href="/ted/css/previsao-orcamentaria.css" type="text/css"/>
<script type="text/javascript">
var settings = {
    endpoint:"<?=$urlPage;?>",
    ted:<?=$_GET['ted']?>,
    nc:[<?=$prevOrcamentaria->getGrupoNC();?>],
    valor:[<?=$prevOrcamentaria->getProvalorGroup();?>],
    year:"<?=date("Y");?>",
    esdid:<?=$situacao['esdid']?>,
    template:"<?=$templateSet;?>"
};
</script>
<script src="/ted/js/jquery.livequery.js" type="text/javascript"></script>
<script src="/ted/js/gestaoploa.js" type="text/javascript"></script>
<script src="/ted/js/handlebars-v2.0.0.js" type="text/javascript"></script>
<script src="/includes/funcoes.js" type="text/javascript"></script>

<script id="table-row" type="text/x-handlebars-template">
    <tr id="tr_{{proid}}" class="tr_new">
        <td class="text-center">
            <span class="glyphicon glyphicon-remove cursor-click remove-nc newLine" data-remove-prev="{{proid}}"></span>
            <input type="hidden" class="transferir-celula" name="creditoremanejado[]" id="transfer-{{proid}}" value="" />
            <input type="hidden" name="notacredito[]" id="nota-credito-{{proid}}" value="" />
        </td>
        <td class="text-center">
            <input type="hidden" name="proid[]" value="">
        </td>
        <td class="text-center" id="td_anoref_{{proid}}" width="">
            <select name="proanoreferencia[]" data-proanoreferencia-value="{{proanoreferencia}}" class="form-control chosen-select" id="proanoreferencia_{{proid}}">
                <option value="" label="-Selecione-">-Selecione-</option>
                <option value="2012" label="2012">2012</option>
                <option value="2013" label="2013">2013</option>
                <option value="2014" label="2014">2014</option>
                <option value="2015" label="2015">2015</option>
                <option value="2016" label="2016">2016</option>
                <option value="2017" label="2017">2017</option>
                <option value="2018" label="2018">2018</option>
                <option value="2019" label="2019">2019</option>
                <option value="2020" label="2020">2020</option>
                <option value="2021" label="2021">2021</option>
                <option value="2022" label="2022">2022</option>
                <option value="2023" label="2023">2023</option>
            </select>
        </td>
        <td class="text-center" id="td_acao_{{proid}}">-</td>
        <td class="text-center container-ptrid" id="td_prg_{{proid}}" width="">-</td>
        <td class="text-center container-pliid" id="td_pi_{{proid}}" width="">-</td>
        <td class="text-center" id="td_acaodsc_{{proid}}" style="font-size: 9px;">{{acatitulo}}</td>
        <td class="text-center container-ndpid" width="">
            <select name="ndpid[]" data-ndpid-value="{{ndpid}}" class="form-control chosen-select" id="ndpid_{{proid}}">
            </select>
        </td>
        <td class="text-center" width="">
            <input type="text" name="provalor[]" maxlength="17" value="{{provalor}}" id="provalor_{{proid}}" class="form-control text-right" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
        </td>
        <td class="text-center" width="">-</td>
        <td class="text-center" width="">
            <span title="Favor, preencher o novo prazo total de execução do TED, contado da data de início da vigência até o prazo final, incluindo o termo aditivo."></span>
            <select name="crdmesexecucao[]" data-crdmesexecucao-value="{{crdmesexecucao}}" class="form-control chosen-select" id="crdmesexecucao_{{proid}}">
            </select>
        </td>
    </tr>
</script>
<script id="table-row-all-privileges" type="text/x-handlebars-template">
    <tr id="tr_{{proid}}" class="tr_new">
        <td class="text-center">
            <span class="glyphicon glyphicon-remove cursor-click remove-nc newLine" data-remove-prev="{{proid}}"></span>
            <input type="hidden" class="transferir-celula" name="creditoremanejado[]" id="transfer-{{proid}}" value="" />
            <input type="hidden" name="notacredito[]" id="nota-credito-{{proid}}" value="" />
        </td>
        <td class="text-center">
            <input type="hidden" name="proid[]" value="">
        </td>
        <td class="text-center" id="td_anoref_{{proid}}" width="">
            <select name="proanoreferencia[]" class="proanoreferencia" data-proanoreferencia-value="{{proanoreferencia}}" class="form-control chosen-select" id="proanoreferencia_{{proid}}">
                <option value="" label="-Selecione-">-Selecione-</option>
                <option value="2012" label="2012">2012</option>
                <option value="2013" label="2013">2013</option>
                <option value="2014" label="2014">2014</option>
                <option value="2015" label="2015">2015</option>
                <option value="2016" label="2016">2016</option>
                <option value="2017" label="2017">2017</option>
                <option value="2018" label="2018">2018</option>
                <option value="2019" label="2019">2019</option>
                <option value="2020" label="2020">2020</option>
                <option value="2021" label="2021">2021</option>
                <option value="2022" label="2022">2022</option>
                <option value="2023" label="2023">2023</option>
            </select>
        </td>
        <td class="text-center" id="td_acao_{{proid}}"></td>
        <td class="text-center container-ptrid" id="td_prg_{{proid}}" width="">
            <select name="ptrid[]" data-proid-value="{{proid}}" data-ptrid-value="{{ptrid}}" class="form-control chosen-select" id="ptrid_{{proid}}">
            </select>
        </td>
        <td class="text-center container-pliid" id="td_pi_{{proid}}" width="">
            <select name="pliid[]" data-ptrid-value="{{ptrid}}" data-pliid-value="{{pliid}}" class="form-control chosen-select" id="pliid_{{proid}}">
            </select>
        </td>
        <td class="text-center" id="td_acaodsc_{{proid}}" style="font-size: 9px;">{{acatitulo}}</td>
        <td class="text-center container-ndpid" width="">
            <select name="ndpid[]" data-ndpid-value="{{ndpid}}" class="form-control chosen-select" id="ndpid_{{proid}}">
            </select>
        </td>
        <td class="text-center" width="">
            <input type="text" name="provalor[]" maxlength="17" value="{{provalor}}" id="provalor_{{proid}}" class="form-control text-right" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
        </td>
        <td class="text-center" width="">
            <select name="crdmesliberacao[]" data-crdmesliberacao-value="{{crdmesliberacao}}" class="form-control chosen-select" id="crdmesliberacao_{{proid}}">
                <option value="" label="-Selecione-">-Selecione-</option>
                <option value="1" label="Janeiro">Janeiro</option>
                <option value="2" label="Fevereiro">Fevereiro</option>
                <option value="3" label="Março">Março</option>
                <option value="4" label="Abril">Abril</option>
                <option value="5" label="Maio">Maio</option>
                <option value="6" label="Junho">Junho</option>
                <option value="7" label="Julho">Julho</option>
                <option value="8" label="Agosto">Agosto</option>
                <option value="9" label="Setembro">Setembro</option>
                <option value="10" label="Outubro">Outubro</option>
                <option value="11" label="Novembro">Novembro</option>
                <option value="12" label="Dezembro">Dezembro</option>
            </select>
        </td>
        <td class="text-center" width="">
            <span title="Favor, preencher o novo prazo total de execução do TED, contado da data de início da vigência até o prazo final, incluindo o termo aditivo."></span>
            <select name="crdmesexecucao[]" data-crdmesexecucao-value="{{crdmesexecucao}}" class="form-control chosen-select" id="crdmesexecucao_{{proid}}">
            </select>
        </td>
    </tr>
</script>
<script id="table-form-transfer" type="text/x-handlebars-template">
    <tr>
        <td><label for="nc_devolucao">NC Devolução:</label></td>
        <td>
            <input type="hidden" name="ghost_proid" id="ghost_proid" value="" class="form-control text-left">
            <input type="text"
                   name="nc_devolucao"
                   maxlength="12"
                   id="nc_devolucao"
                   class="form-control text-left"
                   onmouseover="MouseOver(this);"
                   onfocus="MouseClick(this);this.select();"
                   onmouseout="MouseOut(this);"
                   onblur="MouseBlur(this);">
        </td>
    </tr>
    <tr>
        <td><label for="valor_remanejar">Valor:</label></td>
        <td>
            <input type="text"
                   name="valor_remanejar"
                   maxlength="17"
                   id="valor_remanejar"
                   class="form-control text-right"
                   onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                   onmouseover="MouseOver(this);"
                   onfocus="MouseClick(this);this.select();"
                   onmouseout="MouseOut(this);"
                   onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
        </td>
    </tr>
    <tr>
        <td><label for="observacao">Observação:</label></td>
        <td>
            <textarea id="observacao"
                      name="observacao"
                      cols="65" rows="3"
                      onmouseover="MouseOver( this );"
                      onfocus="MouseClick( this );" onmouseout="MouseOut( this );"
                      onblur="MouseBlur( this );"
                      class="form-control text-left"></textarea>
        </td>
    </tr>
</script>
<script id="table-info-nc" type="text/x-handlebars-template">
    <tr>
        <th class="text-left" colspan="9" style="color:#1479B3;">Resumo da ND selecionada</th>
    </tr>
    <tr>
        <th>Ano</th>
        <th>Ação</th>
        <th>Programa de Trabalho</th>
        <th>Plano Interno</th>
        <th>Descrição da Ação</th>
        <th>Nat. da Despesa</th>
        <th>Valor original</th>
        <th>Mês Liberação</th>
        <th>Prazo cumprimento</th>
    </tr>
    <tr>
        <td>{{proanoreferencia}}</td>
        <td class="text-center">{{acacod}}</td>
        <td>{{ptrid_descricao}}</td>
        <td>{{pliid_descricao}}</td>
        <td>{{acatitulo}}</td>
        <td>{{ndp_descricao}}</td>
        <td class="text-center">{{provalor}}</td>
        <td class="text-center">{{crdmesliberacao}}</td>
        <td class="text-center">{{crdmesexecucao}} Mês(s)</td>
    </tr>
</script>

<script id="table-row-nc" type="text/x-handlebars-template">
    <tr id="tr_nc_{{lote}}">
        <td class="text-center"></td>
        <td class="text-center">&nbsp;</td>
        <td class="text-center" width="">&nbsp;</td>
        <td class="text-center">&nbsp;</td>
        <td class="text-left" style="font-weight:bold;font-size:14px;color:#1479B3;"" colspan="5" class="nc-table-row">
            <span id="{{lote}}" class="glyphicon glyphicon-search cursor-click extrato-nc"></span>&nbsp;Nota de crédito ({{lote}})
            <em style="float:right;font-weight:bold;font-size:14px;color:#000;">R$ {{provalor}}</em>
        </td>
        <td class="text-center"width="">&nbsp;</td>
        <td class="text-center" width="">&nbsp;</td>
    </tr>
</script>
<script id="table-extrato" type="text/x-handlebars-template">
    <thead>
        <tr>
            <th colspan="4">Informações Complementares</th>
        </tr>
        <tr>
            <td colspan="3">Número de Transferência</td>
            <td>{{tcpnumtransfsiafi}}</td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <th colspan="2">Nota de Crédito</th>
            <th colspan="2">Valor</th>
        </tr>
        {{#each extrato}}
            <tr>
                <td colspan="2">{{codncsiafi}}</td>
                {{#if devolucao}}
                    <td  colspan="2" class="devolucao">-{{ppavlrparcela}}</td>
                {{else}}
                    <td  colspan="2">{{ppavlrparcela}}</td>
                {{/if}}
            </tr>
        {{/each}}
    </tbody>
    <tfoot>
        <tr>
            <td colspan="3" class="text-right"><strong>Total:</strong></td>
            <td class="text-left"><strong>{{total}}</strong></td>
        </tr>
    </tfoot>
</script>
<script src="/ted/js/checkbox-checker.js" type="text/javascript"></script>
<script src="/ted/js/previsao-orcamentaria.js" type="text/javascript"></script>

<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
		<li class="active">Programação Orçamentária</li>
	</ol>

    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

	<section class="col-md-10">
		<section class="">
            <table class="table table-striped well">
                <tr>
                    <td width="20%"></td>
                    <td width="60%"><h4 class="text-center">Programação Orçamentária</h4></td>
                    <td width="20%"></td>
                </tr>
                <tr>
                    <td width="15%"></td>
                    <td width="70%">
                        <div class="alert alert-warning" role="alert">
                            <p>
                                As Programações Orçamentárias que já possuem Nota de Crédito, só poderão ser editadas,<br />
                                quando o Termo de Execução Descentralizada, estiver em situação <strong>"Termo em Solicitação de Alteração"</strong><br />
                                E por meio da funcionalidade <strong>Remanejamento de Crédito</strong>,
                                através do icone&nbsp;&nbsp;<span class="glyphicon glyphicon-refresh" aria-hidden="true"></span>
                            </p>
                        </div>
                    </td>
                    <td width="15%"></td>
                </tr>
            </table>
		</section>

        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>

        <!-- form listagem de dados -->
        <form action="" method="post" role="form" class="form-horizontal formListagem" id="formListagem">
            <input type="hidden" value="" name="formacao" id="formacao"/>
            <table class="table table-striped table-bordered table-hover" id="tb_render">
                <thead>
                    <tr id="tr_titulo">
                        <th class="text-center">&nbsp;</th>
                        <th class="text-center">
                            <input type="checkbox" class="" name="ckboxPai" id="ckboxPai">
                        </th>
                        <th class="text-center" width="">Ano</th>
                        <th class="text-center" width="">Ação</th>
                        <th class="text-center" width="">Programa <br>de Trabalho</th>
                        <th class="text-center" width="">Plano <br>Interno</th>
                        <th class="text-center" width="">Descrição <br>da Ação <br>Constante <br>da LOA</th>
                        <th class="text-center" width="">Natureza <br>da Despesa</th>
                        <th class="text-center" width="">Valor <br>(em R$ 1,00)</th>
                        <th class="text-center" width="">Previsão <br />Mês <br>da Liberação</th>
                        <th class="text-center" width="">Prazo para o<br> cumprimento<br> do objeto</th>
                    </tr>
                </thead>
                <tbody>
                <?php include_once dirname(__FILE__) . '/previsao/view_grid.inc'; ?>
                </tbody>
                <tfoot>
                    <tr>
                        <td colspan="4">
                            <?php if ($prevOrcamentaria->verificaProgramacaoEditavel()) : ?>
                                <?php $cssClass = ($prevOrcamentaria->haveCash(Ted_Utils_Model::capturaTcpid())) ? 'useBalanceAvailableOptions' : ''; ?>
                                <a href="#" class="insertNewLine <?=$cssClass;?>">
                                    <span class="glyphicon glyphicon-plus"></span>&nbsp;
                                    Inserir nova previsão
                                </a>
                            <?php endif; ?>
                        </td>
                        <td colspan="7">&nbsp;</td>
                    </tr>
                </tfoot>
            </table>
        </form>
        <!-- FIM form listage de dados -->

        <!-- modal form remanejamento de nota de credito -->
        <div class="modal fade" id="transferValor" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
            <div class="modal-dialog" style="width:80%;">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal">
                            <span aria-hidden="true">&times;</span>
                            <span class="sr-only">Close</span>
                        </button>
                        <h4 class="modal-title">Transferir Crédito</h4>
                    </div>
                    <div class="modal-body">
                        <table class="table table-striped table-hover" id="table-original-nc"></table>
                        <form class="form-horizontal" role="form" method="post" id="web-form-transfer">
                            <table class="table table-striped table-hover" id="table-form"></table>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        <button type="button" class="btn btn-success" id="salvarRm">Salvar</button>
                    </div>
                </div>
            </div>
        </div>
        <!-- FIM modal remanejamento de nota de credito -->

        <!-- modal form cadastro de nota de credito -->
        <div class="modal fade" id="registerNC" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
            <div class="modal-dialog" style="width:40%;">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal">
                            <span aria-hidden="true">&times;</span>
                            <span class="sr-only">Close</span>
                        </button>
                        <h4 class="modal-title">Cadastrar Nota de Crédito</h4>
                    </div>
                    <div class="modal-body">
                        <form class="form-horizontal" name="form_nc" id="form_nc" action="" method="post" role="form">
                            <div class="row">
                                <div class="form-group">
                                    <label class="control-label col-md-5" for="tcpnumtransfsiafi">Nº de Transferencia Siafi:</label>
                                    <div class="col-md-7">
                                        <input type="text" class="form-control" name="tcpnumtransfsiafi" id="tcpnumtransfsiafi">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="control-label col-md-5" for="codncsiafi">Nota de Crédito:</label>
                                    <div class="col-md-7">
                                        <input type="text" class="form-control" name="codncsiafi" id="codncsiafi">
                                    </div>
                                </div>
                            </div>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        <button type="button" class="btn btn-success" id="salvarNC">Salvar</button>
                    </div>
                </div>
            </div>
        </div>
        <!-- FIM modal form cadastro de nota de credito -->

        <!-- modal extrato nota de credito -->
        <div class="modal fade" id="extratoNc" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
            <div class="modal-dialog" style="width:60%;">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal">
                            <span aria-hidden="true">&times;</span>
                            <span class="sr-only">Close</span>
                        </button>
                        <h4 class="modal-title">Extrato Nota de Crédito</h4>
                    </div>
                    <div class="modal-body">
                        <table class="table table-striped table-bordered table-hover" id="tb_extrato">
                        </table>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
        <!-- fim modal extrato nota de credito -->

        <!-- container com os botões de ação -->
        <div class="form-group">
            <div class="col-md-offset-1 container-button">
                <?php if ($prevOrcamentaria->permiteInserirPrevisao()): ?>
                    <button type="submit" class="btn btn-primary" name="submit" id="submit">Gravar</button>
                    <button type="submit" class="btn btn-success" name="submitcontinue" id="submitcontinue">Gravar e Continuar</button>
                <?php else : ?>
                    <div class="alert alert-danger" role="alert">
                        <p>A Programação Orçamentária só pode ser modificada nas seguintes situações:</p>
                        <ul>
                            <li>- Termo em Cadastramento</li>
                            <li>- Termo em Análise pela Coordenação</li>
                            <li>- Termo com Solicitação de Alteração</li>
                        </ul>
                    </div>
                <?php endif; ?>
            </div>
        </div>
	</section>
</section>

<?php if ($prevOrcamentaria->haveCash(Ted_Utils_Model::capturaTcpid())): ?>
<!-- painel para informação de saldo disponível a ser remanejado -->
<div id="saldo-disponivel">
    <?php $prevOrcamentaria->showSaldo(Ted_Utils_Model::capturaTcpid()); ?>
</div>
<?php endif; ?>