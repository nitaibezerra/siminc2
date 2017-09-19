<?php

    /**
     * Funções de apoio ao cadastro de PIs.
     * @see _funcoespi.php
     */
    include_once '_funcoespi.php';

    require_once APPRAIZ. 'includes/workflow.php';
    include_once APPRAIZ. "planacomorc/classes/Cronograma.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Sniic.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Localizacao.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Convenio.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Responsavel.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Acao.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Cronograma.class.inc";
    include_once APPRAIZ. "planacomorc/classes/Pi_Anexo.class.inc";
    include_once APPRAIZ. "monitora/classes/Pi_PlanoInterno.class.inc";

    $dadosPI = array();
    $perfis = pegaPerfilGeral();
    $listaDeAnexos = $aSniic = $aSei = $aPronac = $aConvenio = $aLocalizacao = $aResponsavel = array();

    $pliid = $_REQUEST['pliid'];

    // -- Carrega dados do PI e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPiComDetalhes((object) array('pliid' => $pliid));
    $enquadramento_pi = carregarEnquadramentoPI($pliid);

    $aSniic = (new Pi_Sniic())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aSei = (new Planacomorc_Model_PiSei())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aPronac = (new Planacomorc_Model_PiPronac())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aConvenio = (new Pi_Convenio())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aLocalizacao = (new Pi_Localizacao())->recuperarPorPlanoInterno($pliid, $dadosPI['esfid']);
    $aResponsavel = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    $listaDeAnexos = (new Pi_Anexo())->recuperarPorPlanoInterno($pliid);
    $listaSubUnidadesDelegadas = (new Planacomorc_Model_PiDelegacao())->recuperarIdPorPlanoInterno($pliid, $_SESSION['exercicio']);
//    if ($dadosPI['pliid'] && !$dadosPI['docid']) {
//        $dadosPI['docid'] = pegarDocidPi($dadosPI['pliid']);
//    }
    $estadoAtual = wf_pegarEstadoAtual($dadosPI['docid']);
//    $codigoEnquadramentoFinalistico = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'F');
//    $codigoEnquadramentoNaoOrcamentario = buscarCodigoEnquadramento((int)$_SESSION['exercicio'], 'N');
//    $codigoProdutoNaoAplica = buscarCodigoProdutoNaoAplica((int)$_SESSION['exercicio']);
//    $listaEqdidReduzido = simec_json_encode(buscarListaCodigoEnquadramentoReduzido((int)$_SESSION['exercicio']));

    # Desativa Mudança de exercicio, menus, opções de usuário e etc do Cabeçalho padrão do SIMEC.
    $cabecalhoPopup = TRUE;
//    include APPRAIZ . "includes/cabecalho.inc";
//    include APPRAIZ . "includes/cabecalho_bootstrap_v3.inc";
    include APPRAIZ . "includes/cabecalho_bootstrap_v3_popup.inc";

//ver($dadosPI);

?>

<div class="wrapper wrapper-content animated fadeInRight">
    <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
        <div class="row">
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados do Planejamento</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <b>ID do Projeto:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['pliid']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Código do PI:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['plicod']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Unidade Solicitante:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['unidade']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Sub-Unidade:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['sub_unidade']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Enquadramento:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['eqddsc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Metas PPA:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['meta']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Meta PNC:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['meta_pnc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Indicador PNC:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['iniciativa_pnc']; ?></span>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-2">
                <?php wf_desenhaBarraNavegacao($dadosPI['docid'], array('pliid' => $dadosPI['pliid'])); ?>
            </div>
            
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Funcional Programática</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <b>Programa:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['programa']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Objetivo:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['objetivo']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Iniciativa:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['iniciativa']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Ação:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['acao']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Localizador:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['localizador']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Plano Orçamentário:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['po']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>PTRES:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['ptres']? $dadosPI['ptres']: NULL; ?></span>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>&nbsp;</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <b>Título:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['plititulo']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Descrição / Finalidade:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['plidsc']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Produto PI:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['produto']; ?></span>
                                            <br />
                                            <strong>Unidade de medida:</strong> <span><?php echo $dadosPI['unidade_medida']; ?></span>
                                            <strong>Quantidade:</strong> <span><?php echo $dadosPI['picquantidade']; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>Modalidade de Pactuação:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['modalidade_pactuacao']; ?></span>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Forma de Execução</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <b>Edital:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['picedital'] == 't'? 'Sim': 'Não'; ?></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <b>TED:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['picted'] == 't'? 'Sim': 'Não'; ?></span>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Emenda</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <b>Edital:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['pliemenda'] == 't'? 'Sim': 'Não'; ?></span>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Localização do Projeto</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <b>Tipo de localização:</b>
                                        </td>
                                        <td align="left">
                                            <span><?php echo $dadosPI['tipo_localizacao']; ?></span>
                                        </td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_MUNICIPAL): ?>
                                    <tr>
                                        <td>
                                            <b>Localizações:</b>
                                        </td>
                                        <td align="left">
                                            <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr class="tr_head">
                                                    <td style="text-align: left;"><b>UF</b></td>
                                                    <td style="text-align: left;"><b>Município</b></td>
                                                </tr>
                                                <?php foreach($aLocalizacao as $localizacao): ?>
                                                    <tr style="height: 30px;" class="tr_localizacao_<?php echo $localizacao['muncod']; ?>" >
                                                        <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                        <td style="text-align: left;"><?php echo $localizacao['mundescricao']; ?></td>
                                                    </tr>
                                                <?php endforeach; ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php elseif($dadosPI['esfid'] == Territorios_Model_Esfera::K_ESTADUAL): ?>
                                    <tr>
                                        <td>
                                            <b>localização(ões):</b>
                                        </td>
                                        <td align="left">
                                            <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr class="tr_head">
                                                    <td style="text-align: left;"><b>UF</b></td>
                                                    <td style="text-align: left;"><b>Nome</b></td>
                                                </tr>
                                                <?php foreach($aLocalizacao as $localizacao): ?>
                                                <tr style="height: 30px;" class="tr_localizacao_estadual_<?php echo $localizacao['estuf']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                    <td style="text-align: left;"><?php echo $localizacao['estdescricao']; ?></td>
                                                </tr>
                                                <?php endforeach; ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php elseif($dadosPI['esfid'] == Territorios_Model_Esfera::K_EXTERIOR): ?>
                                    <tr>
                                        <td>
                                            <b>localização(ões):</b>
                                        </td>
                                        <td align="left">
                                            <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr class="tr_head">
                                                    <td style="text-align: left;"><b>País</b></td>
                                                </tr>
                                                <?php foreach($aLocalizacao as $localizacao): ?>
                                                <tr style="height: 30px;" class="tr_localizacao_exterior_<?php echo $localizacao['paiid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['paidescricao']; ?></td>
                                                </tr>
                                                <?php endforeach; ?>
                                            </table>
                                        </td>
                                    </tr>
                                    <?php endif; ?>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="col-sm-10">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Responsáveis do Projeto</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" class="table table-hover table-bordered table-hover" >
                                    <tr>
                                        <td>
                                            <table id="table_responsaveis" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                                <tr>
                                                    <td style="text-align: left;"><b>Nome</b></td>
                                                    <td style="text-align: left;"><b>Telefone</b></td>
                                                    <td style="text-align: left;"><b>E-mail</b></td>
                                                </tr>
                                                <?php foreach($aResponsavel as $responsavel){ ?>
                                                    <tr style="height: 30px;" class="tr_responsaveis_<?php echo $responsavel['pirid']; ?>" >
                                                        <td style="text-align: left;"><?php echo $responsavel['usunome']; ?></td>
                                                        <td style="text-align: left;"><?php echo $responsavel['usutelefone']; ?></td>
                                                        <td style="text-align: left;"><?php echo $responsavel['usuemail']; ?></td>
                                                    </tr>
                                                <?php } ?>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

        </div>
    </form>
</div>
