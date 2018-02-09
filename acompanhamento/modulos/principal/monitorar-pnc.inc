<?php
    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

    $cAcompanhamento = new Acompanhamento_Controller_Acompanhamento();
    switch ($_REQUEST['req']) {
        case 'salvar':
            $cAcompanhamento->salvar($_REQUEST);
        die;
        case 'excluir':
            $cAcompanhamento->excluir($_REQUEST['acoid']);
        die;
        case 'detalhe-pi':
            $cAcompanhamento->recuperarPiPorIndicadorESubunidade($_REQUEST['ipnid'], $_REQUEST['suoid']);
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

    $mAcompanhamento = new Acompanhamento_Model_Acompanhamento($_REQUEST['acoid']);

    // Recuperando Janela Ativa
    $janela = (new Acompanhamento_Model_Janela())->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
    $janid = $mAcompanhamento->janid ? $mAcompanhamento->janid : $janela['janid'];

    // Recuperando Indicador Selecionada
    $ipnid = $mAcompanhamento->ipnid ? $mAcompanhamento->ipnid : $_REQUEST['ipnid'];

    // Recuperando SubUnidade
    $suoid = $mAcompanhamento->suoid ? $mAcompanhamento->suoid : $_REQUEST['suoid'];
    
    $listaDeAnexos = (new Acompanhamento_Model_AcompanhamentoArquivo())->buscarPorAcompanhamento($mAcompanhamento->acoid);

    include APPRAIZ . "includes/cabecalho.inc";
?>

<style>
    .no-margin-bottom .form-group{
        margin-bottom: 5px;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
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

                        <?php
                        $aIndicadores = $mAcompanhamento->getIndicadoresPnc($suoid);
                        $aUnidades = (new Public_Model_SubUnidadeOrcamentaria())->recuperarPorUsuario();
//                        $infoIndicador = (new Public_Model_IndicadorPnc($ipnid))->recuperarInformacoes();
//                        $infoPI = (new Pi_PlanoInterno())->recuperarResumoPorIndicadorESubunidade($ipnid, $suoid);
                        ?>

                        <div class="col-sm-6">
                            <?php echo $simec->select('suoid', 'Unidade', $suoid, $aUnidades, null, ['label-size'=>2]); ?>
                        </div>

                        <div class="clearfix"></div>

                        <?php foreach($aIndicadores as $indicador){
                            $cor = $indicador['acoid'] ? 'navy-bg' : 'red-bg';
                            $cor = $ipnid == $indicador['ipnid'] ? 'blue-bg' : $cor;
                            $urlAcompanhamento = $indicador['acoid'] ? "?modulo=principal/monitorar-pnc&acao=A&acoid={$indicador['acoid']}" : "?modulo=principal/monitorar-pnc&acao=A&ipnid={$indicador['ipnid']}&suoid={$suoid}";
                            ?>
                            <a class="selecionar-indicador" href="<?php echo $urlAcompanhamento; ?>">
                                <div class="col-sm-4 col-md-3" title="<?php echo $indicador['ipnid'] . ' - ' . $indicador['ipndsc']; ?>">
                                    <div data-cor="<?php echo $cor ?>" class="widget div-indicador <?php echo $cor ?>">
                                        <div class="row">
                                            <div class="col-xs-4">
                                                <h2><?php echo $indicador['ipncod']; ?></h2>
                                            </div>
                                            <div class="col-xs-8">
                                                <span><?php echo substr($indicador['ipndsc'], 0, 50); ?>...</span>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </a>
                        <?php } ?>

                        <div class="clearfix"></div>

                        <?php if($ipnid){ ?>
                            <hr />

                            <div class="col-sm-9 no-margin-bottom">
                                <?php
                                $simec->setPodeEditar(0);
                                echo $simec->input('Referência', 'Referência', "{$janela['jandsc']} ({$janela['janinicio']} - {$janela['janfim']})");
                                echo $simec->input('Objetivo', 'Objetivo', "{$infoIndicador['oppcod']} - {$infoIndicador['oppdsc']}");
                                echo $simec->input('Referência', 'Indicador', "{$infoIndicador['ipncod']} - {$infoIndicador['ipndsc']}");
                                echo $simec->input('Previsto', 'Previsto PNC', number_format($infoIndicador['ipnquantidade'], 2, ',', '.'));
                                echo $simec->input('Previstoa', 'Previsto Anualizado', number_format($infoIndicador['ipnquantidadeanualizada'], 2, ',', '.'));
                                $simec->setLabelSize(3);
                                $simec->setPodeEditar(1);
                                ?>
                            </div>
                            <div class="col-sm-3">
                                <?php if(!count($aBeneficiarioDetalhe)){ ?>
                                    <ul class="list-group clear-list m-t">
                                        <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&ipnid=<?php echo $ipnid; ?>&suoid=<?php echo $suoid; ?>">
                                        <li class="list-group-item fist-item">
                                            <span class="pull-right"><?php echo number_format($infoPI['qtd'], 0, ',', '.'); ?></span>
                                            Planos Internos (PI´s)
                                        </li>
                                        </a>
                                        <li class="list-group-item">
                                            Previsto
                                            <span class="pull-right">R$ <?php echo number_format($infoPI['previsto'], 2, ',', '.'); ?></span>
                                        </li>
                                        <li class="list-group-item">
                                            Empenhado
                                            <span class="pull-right">R$ <?php echo number_format($infoPI['empenhado'], 2, ',', '.'); ?></span>
                                        </li>
                                        <li class="list-group-item">
                                            Liquidado
                                            <span class="pull-right">R$ <?php echo number_format($infoPI['liquidado'], 2, ',', '.'); ?></span>
                                        </li>
                                        <li class="list-group-item">
                                            Pago
                                            <span class="pull-right">R$ <?php echo number_format($infoPI['pago'], 2, ',', '.'); ?></span>
                                        </li>
                                    </ul>
                                <?php } else {
                                    echo '<div class="alert alert-danger">Nenhum PI cadastrado para essa indicador</div>';
                                } ?>
                            </div>
                        <?php } ?>

                    </div>
            	</div>
            </div>
            
        </div>

        <?php if($ipnid){ ?>
            <form id="formulario" name="formulario" method="post" class="form-horizontal">
                <input type="hidden" name="req" id="req" value="salvar" />
                <input name="suoid" type="hidden" value="<?php echo $suoid; ?>">
                <input name="ipnid" id="ipnid" type="hidden" value="<?php echo $ipnid; ?>">
                <input name="janid" id="janid" type="hidden" value="<?php echo $janid; ?>">
                <input name="acoid" id="acoid" type="hidden" value="<?php echo $mAcompanhamento->acoid; ?>">

                <div class="col-md-8">
                    <div class="ibox float-e-margins">
                        <div class="ibox-title">
                            <h5>
                                Monitoramento do Indicador
                                <?php if($mAcompanhamento->acoid){ ?>
                                    <a style="margin-left: 10px;" class="fa fa-history detalhe-pi" title="Histórico de alterações" href="?modulo=principal/monitorar-ppa&acao=A&req=ver-historico&acoid=<?php echo $mAcompanhamento->acoid; ?>"></a>
                                <?php } ?>
                            </h5>
                        </div>
                        <div class="ibox-content">
                            <?php

                            echo $simec->valor('acoquantidade', 'Executado', $mAcompanhamento->acoquantidade, ['required']);
                            echo $simec->data('acodata', 'Dt. Referência', $mAcompanhamento->acodata, ['required']);
                            echo $simec->textarea('acoanalise', 'Análise Situacional', $mAcompanhamento->acoanalise);
                            echo $simec->select('claid', 'Classificação', $mAcompanhamento->claid, (new Acompanhamento_Model_Classificacao())->recuperarSqlCombo(null, ['tipid = ' . Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC]), ['required']);

                            $aMedidasSelecionadas = $mAcompanhamento->recuperarMedidas();
                            echo '<span id="span-medida" ' . (!count($aMedidasSelecionadas) ? 'style="display: none"' : '') . '>' . $simec->select('medidas[]', 'Medidas a serem adotadas', $aMedidasSelecionadas, (new Acompanhamento_Model_Medida())->recuperarSqlCombo(null, ['claid = ' . (int)$mAcompanhamento->claid])) . '</span>';

                            echo $simec->textarea('acoprovidencias', 'Detalhamento das providências a serem adotadas', $mAcompanhamento->acoprovidencias);
                            ?>

                            <div class="form-group">
                                <div class="text-center">
                                    <button class="btn btn-primary" type="button" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                    <a href="?modulo=inicio&acao=C" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                    <?php if($mAcompanhamento->acoid){ ?>
    <!--                                        <a href="?modulo=apoio/acompanhamento_form&acao=A&req=excluir&acoid=--><?php //echo $mAcompanhamento->acoid; ?><!--" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>-->
                                    <?php } ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
        
                <div class="col-md-4">
                    <div class="ibox float-e-margins">
                        <div class="ibox-title">
                            <h5>Anexos</h5>
                        </div>
                        <div class="ibox-content">
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table id="table_anexos" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td style="text-align: left;"><b>Nome do arquivo</b></td>
                                            <td style="text-align: left;"><b>Descrição</b></td>
                                            <td style="text-align: center;"><b>Ação</b></td>
                                        </tr>
                                        <?php 
                                            if($listaDeAnexos):
                                                foreach($listaDeAnexos as $anexo):
                                        ?>
                                            <tr style="height: 30px;" class="tr_anexos_<?php echo $anexo['arqid']; ?>" >
                                                <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('<?php echo $anexo['arqid']; ?>'); return false;" ><?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?></a></td>
                                                <td style="text-align: left;"><?php echo $anexo['arqdescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $anexo['arqid']; ?>" name="listaAnexos[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo <?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?>" data-anexos="<?php echo $anexo['arqid']; ?>" />
                                                </td>
                                            </tr>
                                        <?php
                                                endforeach;
                                            endif;
                                        ?>
                                    </table>
                                </div>
                            </div>

                            <div class="form-group">
                                <div class="text-center">
                                    <input type="button" id="btn_inserir_anexos" title="Inserir Anexos" value="Inserir Anexos" class="btn btn-info"/>
                                </div>
                            </div>

                        </div>
                    </div>
                </div>

            </form>
        <?php } ?>
    </div>
</div>

<!-- Modal -->
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
                        <form class="dropzone" method="POST" enctype="multipart/form-data" action="acompanhamento.php?modulo=principal/monitorar-pnc&acao=A&acoid=<?php echo $mAcompanhamento->acoid; ?>&aba=1&req=uploadAnexo" id="formularioAnexo" name="formularioAnexo">
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

<script>
    $(function(){
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

    });

    function abrirModalUpload() {
        $('.modal_dialog_upload').show();
        $('#modal_upload').modal();
        $('#modal_upload .chosen-container').css('width', '100%');
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