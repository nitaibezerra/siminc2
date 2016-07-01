<?php
/**
 * @see Acomporc_Service_Acompanhamento
 * @var Acomporc_Service_Acompanhamento
 */
$modeloAcompanhamento;
$dadosCoordenador = reorganizaArrayDadosUsuario(explode(',', $acao['coordenador']));
$dadosValidador = reorganizaArrayDadosUsuario(explode(',', $acao['validador']));
#$apenasListar = true;
?>
<script>
    $(document).ready(function(){
        $('#workflow_body table').css('width','100%');
        calcularIndicadores();
        if($('[name=apenasListar]').val() == true){
            $.each($('#formulario input[type=text]'),function(index,value){
                //if($(this).prev().is('span')){
                    $(this).attr('disabled','disabled');
                    return;
                //}
                val = $(value).val() != '' ? $(value).val() : '-';
                $(value).parent().html('<p class="form-control-static">'+val+'</p>');
            });
            $.each($('#formulario textarea'),function(index,value){
                val = $(value).val() != '' ? $(value).val() : '-';
                $(value).parent().html('<p class="form-control-static">'+val+'</p>');
            });
            $('#coordenador-acao').attr('disabled','disabled');
            $('#validador-acao').attr('disabled','disabled');
        }
        $('#questionario-tooltip').tooltip();
        $('#rap-tooltip').tooltip();
        $('#subacoes-tooltip').tooltip();
        $('#coordenador-acao').tooltip({placement: 'right'});
        $('#validador-acao').tooltip({placement: 'right'});
        $('#buttonSave').on('click',function(){
            verificaFormularioAcompanhamento();
        });
        $('#periodo').on('change',function(){
           if($('#periodo').val() != ''){
               window.location.href = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A&requisicao=acessar&periodo='+$('#periodo').val()+'&acacod='+$('[name=acacod]').val()+'&unicod='+$('[name=unicod]').val();
           }
        });
        $('#comboAcao').on('change',function(){
           if($('#comboAcao').val() != ''){
               window.location.href = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A&requisicao=acessar&periodo='+$('#periodo').val()+'&acacod='+$('#comboAcao').val()+'&unicod='+$('[name=unicod]').val();
           }
        });

        // -- botão de repetição de valores
        $('#btn-repetir-valores').click(function(){
            $('#acoreprogramadofisico').val(
                $('#inpt_quantidade_fisico').val()
            ).blur();

            $('#acoreprogramadofinanceiro').val(
                $('#inpt_valor_financeiro').val()
            ).blur();
        });
    });

    function verificaFormularioAcompanhamento()
    {
        var retorno = true;
        $.each($('#formulario input[type="text"]'),function(index,body){
            if($(body).val() == ''){
                retorno = false;
                return false;
            }
        });
        if(!retorno){
            bootbox.alert('Preencha todos campos do formulário.');
            return false;
        }
        $.each($('#formulario textarea'),function(index,body){
            $(body).parent().removeClass('has-error');
            if($(body).val() == ''){
                if($(body).is(':required')){
                    $(body).parent().addClass('has-error');
                    retorno = false;
                    return false;
                }
            }
        });

        if(!retorno){
            bootbox.alert('Existem uma ou mais questões obrigatórias que não foram respondidas.');
            return false;
        }
        $('#formulario').submit();
    }

    function alterarResponsavelCoordenador(obj)
    {
        var url = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A';
        var params = { requisicao: 'carregarCoordenadorAcao', acacod: $('[name=acacod]').val(), rpuid: $(obj).attr('data-rpuid'), prfid: $('[name=prfid]').val(), usucpf: $(obj).attr('data-usucpf'), unicod: $('[name=unicod]').val()};
        $.post(url, params, function(response){
            $('#modal-confirm .modal-body').html(response);
            $('.modal-dialog').css('width', '60%');
            $('#modal-confirm .modal-title').html('Alteração de Responsável: Coordenador de Ação');
            $('#modal-confirm .btn-primary').attr('id','salvar-responsavel').html('<span class="glyphicon glyphicon-ok"></span> Salvar');
            $('#modal-confirm .btn-default').html('Cancelar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function alterarResponsavelValidador(obj)
    {
        var url = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A';
        var params = { requisicao: 'carregarValidador', acacod: $('[name=acacod]').val(), rpuid: $(obj).attr('data-rpuid'), prfid: $('[name=prfid]').val(), usucpf: $(obj).attr('data-usucpf'), unicod: $('[name=unicod]').val()};
        $.post(url, params, function(response){
            $('#modal-confirm .modal-body').html(response);
            $('.modal-dialog').css('width', '60%');
            $('#modal-confirm .modal-title').html('Alteração de Responsável: Validador de Ação');
            $('#modal-confirm .btn-primary').attr('id','salvar-responsavel').html('<span class="glyphicon glyphicon-ok"></span> Salvar');
            $('#modal-confirm .btn-default').html('Cancelar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function calcularIndicadores()
    {
    	// Valor da Dotação Atual
        var valordotacaoatual = parseFloat($('#dfsdotacaoatual').val());
        // Meta Física na LOA
        var metafisicaloa = parseInt($('[name=quantidade_fisico]').val());
        // Valor Liquidado
        var valorliquidado = parseFloat($('#dfsliquidado').val());

        var metafisicarealizada = 0;
        var valorreprogramacaofinanceiro = 0;
        var metafisicareprogramacao = 0;

        // Meta Física Realizada
        if ($('#acoexecutadofisico').val() != '') {
            metafisicarealizada = parseInt(replaceAll($('#acoexecutadofisico').val(),".",""));
        }
        // Valor da Reprogramação Fin.
        if ($('#acoreprogramadofinanceiro').val() != '') {
            valorreprogramacaofinanceiro = parseFloat(replaceAll(replaceAll($('#acoreprogramadofinanceiro').val(), '.', ''), ',', '.'));
        }
        // Meta Física Reprogr.
        if ($('#acoreprogramadofisico').val() != '') {
            metafisicareprogramacao = parseInt(replaceAll($('#acoreprogramadofisico').val(),".",""));
        }
        var EFLOA = 0;

        if (metafisicaloa > 0 && valorliquidado > 0 && metafisicarealizada > 0) {
            EFLOA = ((valordotacaoatual / metafisicaloa) / (valorliquidado / metafisicarealizada)) * 100;
            saida = EFLOA.toFixed(2);
            $('#EFLOA').html(saida + '%');
        } else {
            $('#EFLOA').html('-');
        }

        var EFREP = 0;
        if (metafisicareprogramacao > 0 && valorliquidado > 0 && metafisicarealizada > 0) {
            EFREP = ((valorreprogramacaofinanceiro / metafisicareprogramacao) / (valorliquidado / metafisicarealizada)) * 100;
            saida = EFREP.toFixed(2);
            $('#EFREP').html(saida + '%');
        } else {
            $('#EFREP').html('-');
        }

        var ECLOA = 0;
        if (metafisicaloa > 0) {
            ECLOA = (metafisicarealizada / metafisicaloa) * 100;
            saida = ECLOA.toFixed(2);
            $('#ECLOA').html(saida + '%');
        } else {
            $('#ECLOA').html('-');
        }

        var ECREP = 0;
        if (metafisicareprogramacao > 0) {
            ECREP = (metafisicarealizada / metafisicareprogramacao) * 100;
            saida = ECREP.toFixed(2);
            $('#ECREP').html(saida + '%');
        } else {
            $('#ECREP').html('-');
        }
    }

    function replaceAll(string, find, rep)
    {
        return string.replace(new RegExp(escapeRegExp(find), 'g'), rep);
    }

    function escapeRegExp(string)
    {
        return string.replace(/([.*+?^=!:${}()|\[\]\/\\])/g, "\\$1");
    }

    function exibirSubacao(sbacod, codSubacao)
    {
        var url = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A';
        var params = { requisicao: 'carregaSubacao', sbacod: sbacod, prfid: $('[name=prfid]').val()};
        $.post(url, params, function(response){
            $('#modal-confirm .modal-body p').empty();
            $('#modal-confirm .modal-body p').html(response);
            $('.modal-dialog').css('width', '90%');
            $('#modal-confirm .modal-title').html('Dados da Subação - '+codSubacao);
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .modal-footer .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }
</script>
<section class="row">
    <div class="col-md-1">
    </div>
    <section class="col-md-8">
        <section class="panel panel-primary">
            <section class="panel-heading"><strong>Ação e Responsáveis</strong></section>
            <table class="table">
                <thead>
                    <tr>
                        <th colspan="5" class="text-left">Ação</th>
                    </tr>
                    <tr>
                        <td colspan="5"><?php echo $acao['ptres'] . " - " . $acao['descricao']; ?></td>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <th></th>
                        <th class="text-left">Nome</th>
                        <th class="text-left">Telefone</th>
                        <th class="text-left">E-mail</th>
                        <th></th>
                    </tr>
                    <tr>
                        <th class="text-left" width="15%">Coordenador</th>
                        <td><?= $dadosCoordenador['usucpf'] ? formatar_cpf($dadosCoordenador['usucpf']). ' - '. $dadosCoordenador['usunome'] : '<span class="label label-info">Não encontrado</span>'; ?></td>
                        <td><?= $dadosCoordenador['foneddd'] && $dadosCoordenador['fonenum'] ? "({$dadosCoordenador['foneddd']}) {$dadosCoordenador['fonenum']}" : '<span class="label label-info">N/A</span>'; ?></td>
                        <td><a href="mailto:<?= $dadosCoordenador['usuemail'];?>"><?= $dadosCoordenador['usuemail'] ? $dadosCoordenador['usuemail'] : '<span class="label label-info">N/A</span>'; ?></a></td>
                        <td style="width:20px;">
                            <button id="coordenador-acao" class="btn btn-warning btn-xs" data-usucpf="<?= $dadosCoordenador['usucpf']; ?>" data-rpuid="<?= $dadosCoordenador['rpuid']; ?>" onclick="alterarResponsavelCoordenador(this);" title="Alterar Coordenador">
                                <span class="glyphicon glyphicon-user"></span>
                            </button>
                        </td>
                    </tr>
                    <tr>
                        <th class="text-left" width="15%">Validador</th>
                        <td><?= $dadosValidador['usucpf'] ? formatar_cpf($dadosValidador['usucpf']). ' - '. $dadosValidador['usunome'] : '<span class="label label-info">Não encontrado</span>'; ?></td>
                        <td><?= $dadosValidador['foneddd'] && $dadosValidador['fonenum'] ? "({$dadosValidador['foneddd']}) {$dadosValidador['fonenum']}" : '<span class="label label-info">N/A</span>'; ?></td>
                        <td><a href="mailto:<?= $dadosValidador['usuemail'];?>"><?= $dadosValidador['usuemail'] ? $dadosValidador['usuemail'] : '<span class="label label-info">N/A</span>'; ?></a></td>
                        <td style="width:20px;">
                            <button id="validador-acao" class="btn btn-warning btn-xs" data-usucpf="<?= $dadosValidador['usucpf']; ?>" data-rpuid="<?= $dadosValidador['rpuid']; ?>" onclick="alterarResponsavelValidador(this);" title="Alterar Validador">
                                <span class="glyphicon glyphicon-user"></span>
                            </button>
                        </td>
                    </tr>
                    <tr class="active">
                        <th colspan="5" class="text-left">Filtros</th>
                    </tr>
                    <tr>
                        <td colspan="1">
                            <label for="periodo">Período de Referência:</label>
                        </td>
                        <td colspan="3">
                            <?php
                            inputCombo('prfid', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo',array('mantemSelecaoParaUm' => false));
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="1">
                            <label for="periodo">Ação:</label>
                        </td>
                        <td colspan="3">
                            <?php
                            inputCombo('comboAcao', sprintf(retornaQueryComboAcaoSnapshot(),$acao['unicod'],$prfid), $acao['acacod'], 'comboAcao',array('mantemSelecaoParaUm' => false));
                            ?>
                        </td>
                    </tr>
                </tbody>
            </table>
        </section>
    </section>
    <section class="col-md-2">
        <section class="panel panel-info">
            <section class="panel-heading">
                <strong>Tramitação</strong>
            </section>
            <section class="panel-body" id="workflow_body" style="padding-bottom:0;">
            <?
            wf_desenhaBarraNavegacao($acao['docid'],array());
            ?>
            </section>
            <section class="panel-body" style="border-top: 1px solid #bce8f1; ">
                <?= statusNoSIOPIn($acao['ultimoretornosiop'],$acao['aclid']);?>
            </section>
        </section>
    </section>
</section>

<form method="post" class="form-horizontal" name="formulario" id="formulario" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" value="salvarAcompanhamento">
    <input type="hidden" name="apenasListar" value="<?=$apenasListar?>">
    <input type="hidden" name="docid" value="<?=$acao['docid']?>">
    <input type="hidden" name="aclid" value="<?=$acao['aclid']?>">
    <input type="hidden" name="acaid" value="<?=$acao['acaid']?>">
    <input type="hidden" name="prfid" value="<?=$acao['prfid']?>">
    <input type="hidden" name="loccod" value="<?=$acao['loccod']?>">
    <input type="hidden" name="unicod" value="<?=$acao['unicod']?>">
    <input type="hidden" name="acacod" value="<?=$acao['acacod']?>">
    <section class="well">
        <section class="panel panel-default">
            <section class="panel-heading">
                <strong>Dados Financeiros</strong>
                <strong style="float:right;clear:both">Data de Referência: <?= $acao['dataultimatualizacao'];?></strong>
            </section>
            <table class="table">
                <thead>
                    <tr>
                        <th>Dotação inicial</th>
                        <th>Dotação(L+C)</th>
                        <th>Empenhado</th>
                        <th>Liquidado</th>
                        <th>Pago</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td class="text-center">
                            <?= simec_number_format($acao['dotacaoinicial']) ?>
                            <input type="hidden" name="dfsdotacaoinicial" value="<?= $acao['dotacaoinicial'] ?>">
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['dotacaoatual']) ?>
                            <input type="hidden" name="dfsdotacaoatual" id="dfsdotacaoatual" value="<?= $acao['dotacaoatual'] ?>">
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['empenhado']) ?>
                            <input type="hidden" name="dfsempenhado" id="dfsempenhado" value="<?= $acao['empenhado'] ?>">
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['liquidado']) ?>
                            <input type="hidden" name="dfsliquidado" id="dfsliquidado" value="<?= $acao['liquidado'] ?>">
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['pago']) ?>
                            <input type="hidden" name="dfspago" value="<?= $acao['pago'] ?>">
                        </td>
                    </tr>
                </tbody>
                <thead>
                    <tr class="active">
                        <th colspan="5">RAP não processado <button type="button" class="btn btn-default btn-xs" id="rap-tooltip" title="A partir de 2013, será realizado o acompanhamento da execução física referente aos Restos a Pagar (RAP), cujos dados também são trazidos na tela de dados financeiros."><span class="glyphicon glyphicon-info-sign"></span></button></th>
                    </tr>
                    <tr>
                        <th>Inscrito líquido</th>
                        <th>Liquidado a pagar</th>
                        <th>Pago</th>
                        <th>Liquidado efetivo</th>
                        <th></th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td class="text-center">
                            <input type="hidden" id="rapinscritoliquido" value="<?= $acao['rapinscritoliquido'] ?>" />
                            <?= simec_number_format($acao['rapinscritoliquido']) ?>
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['rapliquidadoapagar']) ?>
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['rappago']) ?>
                        </td>
                        <td class="text-center">
                            <?= simec_number_format($acao['rapliquidadoefetivo']) ?>
                            <input type="hidden" id="rapliquidadoefetivo" value="<?= $acao['rapliquidadoefetivo'] ?>" />
                        </td>
                        <td></td>
                    </tr>
                </tbody>
            </table>
        </section>
        <!-- Subações Financiadas pela Ação -->
        <section class="panel panel-default" style="clear:both; margin-bottom: 10px;">
            <section class="panel-heading">
                <strong>Subações Financiadas pela Ação</strong> <button class="btn btn-default btn-xs" id="subacoes-tooltip" title="Para a Administração Direta, CAPES, INEP, FNDE e EBSERH, este campo trará o físico e o financeiro executados nas subações , como forma de facilitar o preenchimento dos dados de acompanhamento da ação orçamentária e a elaboração do relatório de gestão. Para as demais Unidades aparecerá a mensagem “Sem subações financiadas por esta ação”, mesmo que haja subações cadastradas no módulo PPA – Monitoramento e Avaliação."><span class="glyphicon glyphicon-info-sign"></span></button>
            </section>
            <? $modeloAcompanhamento->listarSubacaoPorPlocod($prfid,$acao['prgcod'],$acao['acacod'],$acao['loccod'],$acao['unicod']); ?>
        </section>

        <section class="alert alert-warning alert-dismissible" role="alert">
            <button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <strong>Acompanhamento Físico da Ação </strong><span class="glyphicon glyphicon-warning-sign"></span>
            Físico Executado em 2014: Informar o quantitativo físico do produto executado até a data de apuração, no período de 01/01/2014 a 31/12/2014.<br>
            *** Atenção***
            <br>
            Por orientação da Secretaria de Orçamento Federal, a execução física deve ser informada com base no FINANCEIRO LIQUIDADO.
        </section>
        <section class="row">
            <section class="col-md-6" style="padding-left: 0;">
                <section class="panel panel-default">
                    <section class="panel-heading"><strong>Acompanhamento Físico da Ação</strong></section>
                    <table class="table">
                        <thead>
                            <tr>
                                <th>Produto</th>
                                <th>Unidade</th>
                                <th>Meta Física</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td class="text-center"><?=(($acao['produto']) ? $acao['produto'] : "-") ?></td>
                                <td class="text-center"><?=(($acao['unidade_medida']) ? $acao['unidade_medida'] : "-") ?></td>
                                <td class="text-center"><?=mascaraNumero($acao['metafisica']); ?></td>
                            </tr>
                        </tbody>
                    </table>
                    <section class="panel-body">
                        <section class="form-group">
                            <label class="control-label col-md-6" for="acoexecutadofisico">Físico executado em <?= $_SESSION['exercicio'] ?>: </label>
                            <section class="col-md-6">
                                <?php #['fisicoexecutado'] = $acao['fisicoexecutado'] != 0 ? $acao['fisicoexecutado'] : '0'; ?>
                                <?php inputTexto('acoexecutadofisico', $acao['fisicoexecutado'], 'acoexecutadofisico', 11,false,array('masc'=>"###.###.###",'evtkeyup'=>'calcularIndicadores();','label'=>'Físico executado')); ?>
                            </section>
                        </section>
                        <section class="form-group">
                            <label class="control-label col-md-6" for="acoexecutadorapfisico">Físico executado com RAP do exercício anterior: </label>
                            <section class="col-md-6">
                                <?php #$acao['fisicoexecutadorap'] = $acao['fisicoexecutadorap'] != 0 ? $acao['fisicoexecutadorap'] : '0'; ?>
                                <?php inputTexto('acoexecutadorapfisico', $acao['fisicoexecutadorap'], 'acoexecutadorapfisico', 11,false,array('masc'=>'###.###.###','label'=>'Físico executado com Restos a Pagar (RAP): Neste campo deverá ser registrada a execução física realizada com Restos a Pagar (RAP), caso tenha ocorrido.')); ?>
                            </section>
                        </section>
                        <section class="form-group">
                            <label class="control-label col-md-6" for="acodataapuracao">Data de apuração: </label>
                            <section class="col-md-6">
                                <?php
                                $metaDataCombo = array(
                                    'size' => 10,
                                    'classe' => 'data',
                                    'label' => 'Informar a data de apuração dos dados da execução física, que deve ser IGUAL OU ANTERIOR a 31/01/2014.'
                                );
                                inputData('acodataapuracao', $acao['dataapuracao'], 'acodataapuracao');
                                ?>
                            </section>
                        </section>
                    </section>
                </section>
            </section>

            <section class="col-md-6" style="padding-right:0;">
                <section class="panel panel-default">
                    <section class="panel-heading"><strong>Indicadores</strong></section>
                    <table class="table" <?php echo (($perexibirindicadores == 't') ? 'style="display:none;"' : "") ?>>
                        <tr>
                            <td style="width:80%;">Eficiência em relação à meta na LOA (EFLOA)</td>
                            <td id="EFLOA"></td>
                        </tr>
                        <tr>
                            <td style="width:80%;">Eficiência em relação à meta após a reprogramação (EFREP)</td>
                            <td id="EFREP"></td>
                        </tr>
                        <tr>
                            <td style="width:80%;">Eficácia em relação à meta da LOA (ECLOA)</td>
                            <td id="ECLOA"></td>
                        </tr>
                        <tr>
                            <td style="width:80%;">Eficácia em relação à meta após a reprogramação (ECREP)</td>
                            <td id="ECREP"></td>
                        </tr>
                    </table>
                </section>
            </section>
        </section>

        <section class="panel panel-default">
            <section class="panel-heading"><strong>Reprogramação</strong></section>
            <section class="panel-body">
                <section class="col-md-6">
                    <table class="table">
                        <thead>
                            <tr>
                                <th>Físico inicial</th>
                                <th>Financeiro inicial</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td class="text-center">
                                    <?= $acao['metafisica'] ? mascaraNumero($acao['metafisica']) : '-'; ?>
                                    <input type="hidden" id="inpt_quantidade_fisico" name="quantidade_fisico"
                                           value="<?php echo $acao['metafisica'] ?>">
                                </td>
                                <td class="text-center">
                                    <?= simec_number_format($acao['dotacaoatual']) ?>
                                    <input type="hidden" id="inpt_valor_financeiro" name="valor_financeiro"
                                           value="<?php echo $acao['dotacaoatual']; ?>" />
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </section>
                <section class="col-md-6">
                    <section class="form-group">
                        <label class="control-label col-md-4" for="acoreprogramadofisico">Físico reprogramado:</label>
                        <section class="col-md-8">
                            <?php inputTexto('acoreprogramadofisico', $acao['reprogramacaofisica'], 'acoreprogramadofisico', 11,false,array('label'=>'Reprogramado FÍSICO','masc'=>'###.###.###','evtkeyup'=>'calcularIndicadores();'));?>
                        </section>
                    </section>
                    <section class="form-group">
                        <label class="control-label col-md-4" for="acoreprogramadofinanceiro">Financeiro reprogramado:</label>
                        <section class="col-md-8">
                            <?php
                            $value = ($acao['reprogramacaofinanceira']) ? number_format($acao['reprogramacaofinanceira'], 2, ',', '.') : '';
                            inputTexto('acoreprogramadofinanceiro', $value, 'acoreprogramadofinanceiro', 18,false, array('masc'=>'###.###.###.###,##','evtblur'=>'calcularIndicadores();'));?>
                        </section>
                    </section>
                </section>
                <section class="col-md-2 col-md-offset-10">
                    <button class="btn btn-success" id="btn-repetir-valores" type="button">Repetir valores</button>
                </section>
            </section>
        </section>

        <section class="panel panel-default">
            <section class="panel-heading"><strong>Acompanhamento Físico dos Planos Orçamentários</strong></section>
            <section class="panel-body">
                <?php require_once 'view/po.inc'; ?>
            </section>
        </section>

        <section class="panel panel-default">
            <section class="panel-heading"><strong>Questionário de Acompanhamento das Ações </strong><button type="button" class="btn btn-default btn-xs" id="questionario-tooltip" data-toggle="tooltip" data-placement="top" title="O questionário tem por objetivo orientar o preenchimento do acompanhamento orçamentário, para que este gere informações que expliquem de forma clara, objetiva e completa, os resultados de execução física apurados no período."><span class="glyphicon glyphicon-info-sign"></span></button></section>
            <section class="panel-body">
                <?php require_once 'view/questionario.inc'; ?>
            </section>
        </section>
        <? if($apenasListar): ?>
        <? gravacaoDesabilitada('Periodo de preenchimento expirado.');?>
        <? endif;?>
        <section class="form-group">
            <section class="col-lg-12">
                <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/acompanhamento/acao&acao=A&periodo=<?=$prfid?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar ao Acompanhamento</button>
                <? if(!$apenasListar): ?>
                <button type="button" class="btn btn-primary" id="buttonSave" <?= $apenasListar ? 'disabled' : ''?>><span class="glyphicon glyphicon-ok"></span> Salvar</button>
                <? endif; ?>
            </section>
        </section>
    </section>
</form>
