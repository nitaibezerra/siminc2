<?php

    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

    $cAcompanhamento = new Acompanhamento_Controller_Acompanhamento();
    
    $mpnid = $_REQUEST['mpnid'];
    
    switch ($_REQUEST['req']) {
        case 'salvar':
            $cAcompanhamento->salvar($_REQUEST);
        die;
        case 'indicador-form':
            include_once APPRAIZ. 'acompanhamento/modulos/principal/indicador_form.inc';
            die;
        die;
        case 'excluir':
            $cAcompanhamento->excluir($_REQUEST['acoid']);
        die;
        case 'detalhe-pi':
            $cAcompanhamento->recuperarPiPorMetaPncESubunidade($_REQUEST['mpnid'], $_REQUEST['suoid']);
        die;
        case 'exibir-grafico':
//            $cAcompanhamento->exibirGrafico($_REQUEST['mpnid'], $_REQUEST['suoid']);
        die;
        case 'ver-historico':
            $cAcompanhamento->recuperarHistorico($_REQUEST['acoid']);
            die;
        case 'carregarMedida':
            $mClassificacao = new Acompanhamento_Model_Classificacao($_REQUEST['claid']);
            $retorno['medida'] = $mClassificacao->clamedida;
            $retorno['select'] = $simec->select('medidas[]', 'Medidas a serem adotadas', null, (new Acompanhamento_Model_Medida())->recuperarSqlCombo(null, ['claid = ' . (int)$_REQUEST['claid']]));

            echo json_encode($retorno);
        die;
        case 'uploadAnexo':
            $file = new FilesSimec();
            if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
                echo simec_json_encode(array(
                    'arqid' => $file->getIdArquivo(),
                    'arqnome' => $_FILES['file']['name'],
                    'arqdescricao' => $_REQUEST['arqdescricao']
                ));
            }else{
                echo simec_json_encode(array(
                    'error' => 1,
                    'errorMensage' => 'Não foi possível enviar o arquivo!'
                ));
            }
        die;
    }

    $acompanhamento = new Acompanhamento_Model_Acompanhamento($_REQUEST['acoid']);

    // Recuperando Janela Ativa
    $janela = (new Acompanhamento_Model_Janela())->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
    $janid = $acompanhamento->janid = $acompanhamento->janid? $acompanhamento->janid: $janela['janid'];

    // Recuperando SubUnidade
    $suoid = $acompanhamento->suoid? $acompanhamento->suoid: $_REQUEST['suoid'];
    
    $subunidade = new Public_Model_SubUnidadeOrcamentaria($suoid);
    if($subunidade){
        $unidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
    }
    
    $listaDeAnexos = (new Acompanhamento_Model_AcompanhamentoArquivo())->buscarPorAcompanhamento($acompanhamento->acoid);

    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/principal/acompanhamento.js?v=1"></script>

<script src="../library/liquify/snap.svg.js"></script>
<script src="../library/liquify/liquid.meter.js"></script>

<style>
    .no-margin-bottom .form-group{
        margin-bottom: 5px;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2>MONITORAMENTO DAS METAS PNC</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
                    <div class="row">
                        <div class="col-sm-2">
                            <h5>Indicadores</h5>
                        </div>
                        <div class="col-sm-10">
                            <div style="float: right;">
                                <div class="box-legenda">
                                    Legenda:
                                </div>
                                <div class="box-legenda red-bg">
                                    Pendente
                                </div>
                                <div class="box-legenda navy-bg">
                                    Preenchida
                                </div>
                                <div class="box-legenda blue-bg">
                                    Selecionada
                                </div>
                            </div>
                        </div>
            	    </div>
            	</div>
            	<div class="ibox-content">
                    <div class="row form-horizontal">

                        <div class="col-sm-6">
                            <?php
                                $listaUnidades = (new Public_Model_SubUnidadeOrcamentaria())->recuperarPorUsuario();
                                echo $simec->select('suoid', 'Unidade', $suoid, $listaUnidades, null, ['label-size'=>2]);
                            ?>
                        </div>

                        <div class="clearfix"></div>

                        <?php
                            $listaMetas = $acompanhamento->getMetasPncComPi((object) array('suoid' => $suoid, 'janid' => $acompanhamento->janid, 'exercicio' => $_SESSION['exercicio']));
                        
                            foreach($listaMetas as $meta){
                                $cor = $meta['acoid']? 'navy-bg': 'red-bg';
                                $cor = $mpnid == $meta['mpnid']? 'blue-bg': $cor;
                                $urlAcompanhamento = $meta['acoid']? "?modulo=principal/monitorar-pnc&acao=A&acoid={$meta['acoid']}": "?modulo=principal/monitorar-pnc&acao=A&mpnid={$meta['mpnid']}&suoid={$suoid}";
                            ?>
                                <a class="selecionar-indicador" href="<?php echo $urlAcompanhamento; ?>">
                                    <div class="col-xs-1" title="<?php echo $meta['mpncod'] . ' - ' . $meta['mpndsc']; ?>">
                                        <div data-cor="<?php echo $cor ?>" class="widget div-indicador <?php echo $cor ?>">
                                            <div class="row">
                                                <div class="col-xs-12">
                                                    <h2><?php echo $meta['mpncod']; ?></h2>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </a>
                            <?php } ?>

                        <div class="clearfix"></div>

                        <?php
                            if($mpnid){
                                $simec->setPodeEditar(0);
                                
                                $valoresOrcamentarios = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
                        ?>
                            <hr />
                            
                            <div class="col-sm-12">
                                <div style="float: right;">
                                    <?php echo $simec->input('Referência', 'Referência', "{$janela['jandsc']} ({$janela['janinicio']} - {$janela['janfim']})"); ?>
                                </div>
                            </div>
                            <div class="col-sm-12">
                                <table class="table">
                                    <thead>
                                        <tr>
                                            <th>
                                                <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $mpnid; ?>&suoid=<?php echo $suoid; ?>" title="Exibir Lista de Planos Internos">
                                                    Planos Internos (PI´s)
                                                </a>
                                            </th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Previsto</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Empenhado</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Liquidado</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Pago</a></th>
                                        </tr>
                                    </thead>
                                    <?php if($valoresOrcamentarios): ?>
                                        <tbody>
                                            <tr>
                                                <td>
                                                    <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $mpnid; ?>&suoid=<?php echo $suoid; ?>" title="Exibir Lista de Planos Internos">
                                                        <?php echo number_format($valoresOrcamentarios['qtd'], 0, ',', '.'); ?>
                                                    </a>
                                                </td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['previsto'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['empenhado'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['liquidado'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['pago'], 2, ',', '.'); ?></a></td>
                                            </tr>
                                        </tbody>
                                    <?php else: ?>
                                        <tfoot>
                                            <tr>
                                                <td>
                                                    <div class="alert alert-danger">Nenhum PI cadastrado para essa indicador</div>
                                                </td>
                                            </tr>
                                        </tfoot>
                                    <?php endif; ?>
                                </table>
                            </div>
                        <?php } ?>

                    </div>
            	</div>
            </div>
            
        </div>

        <?php
            if($mpnid){
                $meta = new Public_Model_MetaPnc($mpnid);
                $listaIndicadorPnc = $acompanhamento->getIndicadoresPnc((object) array('suoid' => $suoid, 'mpnid' => $meta->mpnid, 'exercicio' => (int)$_SESSION['exercicio']));
//                ver($listaIndicadorPnc);
        ?>
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <div class="row">
                            <div class="col-sm-1" style="text-align: center;">
                               <h1><?php echo $meta->mpncod; ?></h1>
                            </div>
                            <div class="col-sm-11">
                                <h2><?php echo $meta->mpndsc; ?></h2>
                            </div>
                        </div>
                    </div>
                    <div class="ibox-content">
                        <form id="form" action="#" class="wizard-big wizard clearfix" role="application" novalidate="novalidate">
                            <div class="steps clearfix">
                                <ul role="tablist">
                                    <li role="tab" class="first current" aria-disabled="false" aria-selected="true">
                                        <a id="form-t-0" href="#form-h-0" aria-controls="form-p-0">
                                            <span class="current-info audible">current step: </span>
                                            <span class="number">1.</span> INDICADORES
                                        </a>
                                    </li>
                                    <li role="tab" class="disabled" aria-disabled="true">
                                        <a id="form-t-1" href="#form-h-1" aria-controls="form-p-1">
                                            <span class="number">2.</span> ATIVIDADES
                                        </a>
                                    </li>
                                    <li role="tab" class="disabled last" aria-disabled="true">
                                        <a id="form-t-2" href="#form-h-3" aria-controls="form-p-3">
                                            <span class="number">3.</span> ANEXOS
                                        </a>
                                    </li>
                                </ul>
                            </div>
                            <div class="content clearfix">
                                <h1 id="form-h-0" tabindex="-1" class="title current">Account</h1>
                                <fieldset id="form-p-0" role="tabpanel" aria-labelledby="form-h-0" class="body current" aria-hidden="false">
                                    <div class="row">
                                        <div class="col-lg-8">
                                            <table class="table table-bordered">
                                                <thead>
                                                    <tr>
                                                        <th>DESCRIÇÃO DO INDICADOR</th>
                                                        <?php for($ano = 2010; $ano <= ((int)$_SESSION['exercicio']); $ano++): ?>
                                                            <th><?php echo $ano; ?></th>
                                                        <?php endfor; ?>
                                                    </tr>
                                                </thead>
                                                <?php foreach($listaIndicadorPnc as $indicador): ?>
                                                    <tbody>
                                                        <tr>
                                                            <td><?php echo $indicador['ipncod']. ' - '. $indicador['ipndsc']; ?></td>
                                                            <?php
                                                                for($ano = 2010; $ano <= ((int)$_SESSION['exercicio']); $ano++):
                                                                    $acompanhamentoIndicadorPnc = (new Acompanhamento_Model_Acompanhamento())->buscarIndicadorPnc((object) array(
                                                                        'janid' => $janid,
                                                                        'unocod' => $unidadeOrcamentaria->unocod,
                                                                        'suocod' => $subunidade->suocod,
                                                                        'ipncod' => $indicador['ipncod'],
                                                                        'prsano' => $ano
                                                                    ));
                                                            ?>
                                                                <td>
                                                                    <?php if($acompanhamentoIndicadorPnc->acoquantidade): ?>
                                                                        <a href="#" class="editar_indicador link" data-acoid="<?php echo $acompanhamentoIndicadorPnc->acoid; ?>" title="Editar Indicador">
                                                                            <?= $acompanhamentoIndicadorPnc->acoquantidade; ?>
                                                                        </a>
                                                                    <?php else: ?>
                                                                        <span class="editar_indicador fa fa-pencil link" data-janid="<?php echo $janid; ?>" data-unocod="<?php echo $unidadeOrcamentaria->unocod; ?>" data-suocod="<?php echo $subunidade->suocod; ?>" data-ipncod="<?php echo $indicador['ipncod']; ?>" data-prsano="<?php echo $ano; ?>" title="Editar Indicador"></span>
                                                                    <?php endif; ?>
                                                                </td>
                                                            <?php endfor; ?>
                                                        </tr>
                                                    </tbody>
                                                <?php endforeach; ?>
                                            </table>
                                        </div>
                                        <div class="col-lg-4">
                                            <div class="text-center">
                                                <div style="margin-top: 20px">
                                                    <i class="fa fa-sign-in" style="font-size: 180px;color: #e5e5e5 "></i>
                                                </div>
                                            </div>
                                        </div>
                                    </div>

                                </fieldset>
                                <h1 id="form-h-1" tabindex="-1" class="title">Profile</h1>
                                <fieldset id="form-p-1" role="tabpanel" aria-labelledby="form-h-1" class="body" style="display: none;" aria-hidden="true">
                                    <h2>Profile Information</h2>
                                    <div class="row">
                                        <div class="col-lg-6">
                                            <div class="form-group">
                                                <label>First name *</label>
                                                <input id="name" name="name" class="form-control required" aria-required="true" type="text">
                                            </div>
                                            <div class="form-group">
                                                <label>Last name *</label>
                                                <input id="surname" name="surname" class="form-control required" aria-required="true" type="text">
                                            </div>
                                        </div>
                                        <div class="col-lg-6">
                                            <div class="form-group">
                                                <label>Email *</label>
                                                <input id="email" name="email" class="form-control required email" aria-required="true" type="text">
                                            </div>
                                            <div class="form-group">
                                                <label>Address *</label>
                                                <input id="address" name="address" class="form-control" type="text">
                                            </div>
                                        </div>
                                    </div>
                                </fieldset>

                                <h1 id="form-h-2" tabindex="-1" class="title">Warning</h1>
                                <fieldset id="form-p-2" role="tabpanel" aria-labelledby="form-h-2" class="body" style="display: none;" aria-hidden="true">
                                    <div class="text-center" style="margin-top: 120px">
                                        <h2>You did it Man :-)</h2>
                                    </div>
                                </fieldset>

                                <h1 id="form-h-3" tabindex="-1" class="title">Finish</h1>
                                <fieldset id="form-p-3" role="tabpanel" aria-labelledby="form-h-3" class="body" style="display: none;" aria-hidden="true">
                                    <h2>Terms and Conditions</h2>
                                    <input id="acceptTerms" name="acceptTerms" class="required" aria-required="true" type="checkbox"> <label for="acceptTerms">I agree with the Terms and Conditions.</label>
                                </fieldset>
                            </div>
                            <div class="actions clearfix">
                                <ul role="menu" aria-label="Pagination">
                                    <li class="disabled" aria-disabled="true">
                                        <a href="#previous" role="menuitem">Voltar</a>
                                    </li>
                                    <li aria-hidden="false" aria-disabled="false">
                                        <a href="#next" role="menuitem">Próximo</a>
                                    </li>
                                    <li style="display: none;" aria-hidden="true">
                                        <a href="#finish" role="menuitem">Concluir</a>
                                    </li>
                                </ul>
                            </div>
                        </form>
                    </div>
                </div>
            </div>

        <?php } ?>
    </div>
</div>

<!-- Div do Modal de lista de PIs -->
<div class="modal fade" id="detalhe-pi" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel"></h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<!-- Div do Modal do formulário de Anexo de Arquivos -->
<div class="row">
    <div class="col-md-12">
        <div id="modal_upload" class="modal fade">
            <div class="modal_dialog_upload">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Anexar arquivos(Documentos)</h4>
                    </div>
                    <div class="modal-body">
                        <form class="dropzone" method="POST" enctype="multipart/form-data" action="acompanhamento.php?modulo=principal/monitorar-pnc&acao=A&acoid=<?php echo $acompanhamento->acoid; ?>&aba=1&req=uploadAnexo" id="formularioAnexo" name="formularioAnexo">
                            <div class="form-group">
                                <label for="arqdescricao" class="col-lg-2 control-label">Descrição:</label>
                                <div class="col-lg-10">
                                    <input type="text" value="" class="CampoEstilo normal form-control" placeholder="Insira a descrição do arquivo." title="Descrição" id="arqdescricao" name="arqdescricao" maxlength="255" size="2">
                                </div>
                            </div>
                            <div class="fallback">
                                <input name="file" type="file" multiple />
                            </div>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<!-- Div do Modal do Grafico -->
<div class="row">
    <div class="col-md-12">
        <div id="modal_grafico" class="modal fade">
            <div class="modal_dialog_grafico">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Grafico Planos Internos</h4>
                    </div>
                    <div class="modal-body">
                        <?php if($mpnid): ?>
                            <div class="col-sm-12">
                                <div class="col-md-8">
                                    <h3 class="text-center">PI's Associados à Meta</h3>
                                    <?php
                                        $listaPI = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
                                        $metaPnc = new Public_Model_MetaPnc($mpnid);
                                        $dadosGrafico = [];
                                        if(count($listaPI)){
                                            foreach ($listaPI as $key => $valor) {
                                                if(in_array($key, ['previsto', 'provisionado', 'empenhado', 'liquidado', 'pago'])){
                                                    $dadosGrafico[$key]['descricao'] = ucfirst($key);
                                                    $dadosGrafico[$key]['valor'] = $valor;
                                                    $dadosGrafico[$key]['categoria'] = $metaPnc->mpncod;
                                                }
                                            }
                                        }

                                        $colors = "'#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";
                                        $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                                        $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
                                            ->setColors($colors)
                                            ->setHeight('250px')
                                            ->gerarGrafico($dadosGrafico);
                                    ?>
                                </div>
                                <div class="col-sm-2">
                                    <?php
                                        $percentual = $valoresOrcamentarios['previsto']? $valoresOrcamentarios['empenhado'] * 100 / $valoresOrcamentarios['previsto'] : 0;
                                    ?>
                                    <h3 class="text-center">Percentual de Empenho</h3>
                                    <meter style="display: none;" max="1000" value="<?php echo $percentual; ?>" id="percentual-empenhado"></meter>
                                </div>
                            </div>
                        <?php endif; ?>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<!-- Modal de Cadastro de Indicador -->
<div class="row">
    <div class="col-md-6">
        <div id="div_modal_cadastro_indicador" class="modal fade">
            <div class="modal_dialog_upload">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Cadastro de acompanhamento do Indicador PNC</h4>
                    </div>
                    <div id="div_modal_cadastro_indicador_body" class="modal-body">
                        
                        <!-- Formulario de indicador -->
                        
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" type="button" id="Salvar-indicador">Salvar</button>
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<?php
    bootstrapPopup(
        "Cadastro de acompanhamento do Indicador PNC",
        'div_modal_acompanhamento_indicador',
        '',
        array(
            'salvar',
            'fechar'),
        array(
            'tamanho' => 'lg'
        ));
?>

<script>
    $(function(){
        
        $('#div_modal_acompanhamento_indicador').find('.btn-salvar').click(function(){
            $('#formulario-acompanhamento-indicador').find('[type="submit"]').trigger('click');
        });
        
        $('.editar_indicador').click(function(){
            var acoid = $(this).attr('data-acoid');
            var janid = $(this).attr('data-janid');
            var unocod = $(this).attr('data-unocod');
            var suocod = $(this).attr('data-suocod');
            var ipncod = $(this).attr('data-ipncod');
            var prsano = $(this).attr('data-prsano');
            var url = '?modulo=principal/monitorar-pnc&acao=A'+
                '&req=indicador-form'+
                '&acoid='+ acoid+
                '&janid='+ janid+
                '&unocod='+ unocod+
                '&suocod='+ suocod+
                '&ipncod='+ ipncod+
                '&prsano='+ prsano;
            
            $('#div_modal_acompanhamento_indicador .modal-body').empty();
            $('#div_modal_acompanhamento_indicador .modal-body').load(url);
            $('#div_modal_acompanhamento_indicador').modal();
            return false;
        });
        
        $('#claid').change(function(){
            $.ajax({
                url: '?modulo=principal/monitorar-pnc&acao=A&req=carregarMedida&claid=' + $(this).val(),
                dataType: 'json',
                success: function(retorno){
                    $('#span-medida').html(retorno.select);
                    if(retorno.medida == 't'){
                        $('#span-medida').show();
                    } else {
                        $('#span-medida').hide();
                    }
                }
            });
        });

        $('.selecionar-indicador').click(function(){
            if(!$('#suoid').val()){
                swal('', 'Por favor, escolha uma unidade.', 'warning');
                return false;
            }
        });

        $('#suoid').change(function(){
            window.location.href = '?modulo=principal/monitorar-pnc&acao=A&suoid=' + $(this).val();
        });

        $('.div-indicador').mouseenter(function(){
            var cor = $(this).data('cor');
            $(this).removeClass(cor).addClass('blue-bg');
        });

        $('.div-indicador').mouseleave(function(){
            var cor = $(this).data('cor');
            $(this).removeClass('blue-bg').addClass(cor);
        });

        $('.detalhe-pi').click(function(){
            var url = $(this).attr('href');
            var title = $(this).attr('title');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi .modal-title').html(title);
            $('#detalhe-pi').modal();
            return false;
        });

        $('#btn-salvar').click(function(){
            if($('#span-medida').css('display') != 'none' && !$('#medidas').val()){
                swal('', "O campo 'Medidas a serem adotadas' é de preenchimento obrigatório", 'warning');
                return false;
            }

            $('#formulario').submit();
        });
        
        $('#btn_inserir_anexos').click(function(){
            abrirModalUpload();
        });

        // Evento de terminar de carregar arquivos
        Dropzone.options.formularioAnexo = {
            init: function() {
                
                this.on("success", function(file, response){
                    var jsonResponse = $.parseJSON(response);
                    inserirNovoAnexo(jsonResponse);
                });

                this.on("queuecomplete", function(file){

                    // Armazena o objeto Dropzone para chamar métodos
                    objFormularioAnexo = this;
                    // Chama mensagem de sucesso
                    swal({
                      title: "",
                      text: "Arquivos salvos com sucesso!",
                      timer: 2000,
                      showConfirmButton: false,
                      type: "success"
                    }, function(){
                        // Fecha o swal alert
                        swal.close();
                        // limpa campo de upload
                        objFormularioAnexo.removeAllFiles();
                        // fecha modal após a seleção
                        $('#modal_upload').modal('hide');
                    });
                });
            }
        };
        
        $('#table_anexos').on('click', '.btnRemoverAnexos', function(){
            var id = $(this).attr('data-anexos');
            $('.tr_anexos_'+ id).remove();
        });

        $('#percentual-empenhado').liquidMeter({
            shape: 'circle',
            color: '#0088cc',
            background: '#F9F9F9',
            fontSize: '24px',
            fontWeight: '600',
            stroke: '#F2F2F2',
            textColor: '#333',
            liquidOpacity: 0.9,
            liquidPalette: ['#333'],
            speed: 3000
        });

        $('.a_grafico').click(function(){
            abrirModalGrafico();
            return false;
        });

    });

    function abrirModalUpload() {
        $('.modal_dialog_upload').show();
        $('#modal_upload').modal();
        $('#modal_upload .chosen-container').css('width', '100%');
    }
    
    function abrirModalGrafico() {
        $('.modal_dialog_grafico').show();
        $('#modal_grafico').modal();
        $('#modal_grafico .chosen-container').css('width', '100%');
    }

    function inserirNovoAnexo(json){
        var trHtml = '<tr style="height: 30px;" class="tr_anexos_'+ json.arqid +'" >'
            + '                    <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('+ json.arqid+ '); return false;" >'+ json.arqnome +'</a></td>'
            + '                    <td style="text-align: left;">'+ json.arqdescricao +'</td>'
            + '                    <td style="text-align: center;">'
            + '                         <input type="hidden" value="'+ json.arqid +'" name="listaAnexos[]">'
            + '                         <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo '+ json.arqnome+ '" data-anexos="'+ json.arqid +'" >'
            + '                    </td>'
            + '                </tr>'
        ;
        $('#table_anexos').append(trHtml);
    }

</script>