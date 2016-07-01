<?php
if(!$_POST['sbacod'] || !$_POST['prfid']){
    die();
}
$perfil = pegaPerfilGeral();
$apenasListar = true;
$tipo = 'S';
$modeloPeriodo = new Acomporc_Service_Periodo();
$prfid = $modeloPeriodo->convertePeriodo($_POST['prfid'], 'S');
$sbacod = $_POST['sbacod'];
$modeloAcompanhamento = new Acomporc_Service_Acompanhamento();
$subacao = $modeloAcompanhamento->recuperaDadosSubacao($sbacod,$prfid);
$acoes = $modeloAcompanhamento->listarAcaoSubacao($sbacod,$prfid);

$acompanhamentoQuestionario = new Acomporc_Service_Questionario();
$acompanhamentoQuestionario->__set('tipoAcesso', $tipo);
$questionarios = $acompanhamentoQuestionario->listaJustificativas(array('prfid' => $prfid,'sbacod' => $subacao['sbacod'],'asaid'=> $subacao['asaid']));
?>
<script>
    function mascara(value){
        var tam = value.length;
        var temp = '';
        for (i = (tam - 1); i >= 0; i--){
            temp = value.charAt(i) + temp;
            if ((value.substr(i).length % 3 == 0) && (i > 0)){
                temp = '.' + temp;
            }
        }
        return temp;
    }
    function verificaPermissao()
    {
        if($('input[name="apenasListar"]').val() == true){
            $.each($('table textarea'),function(index,value){
                $(value).parent().html('<p class="form-control-static">'+$(value).val()+'</p>');
            });
            $.each($('table input'),function(index,value){
                $(value).parent().html('<p style="text-align:right!important">'+mascara($(value).val())+'</p>');
            });
        }
    }
    $(function(){
        $('#mensagemErro').hide();
        if(parseInt($('input[name="existe"]').val()) == 0){
            $('#mensagemErro').show();
            $('#corpo').hide()
        }
        verificaPermissao();
        $('#collapseButton').on('click',function(){
            if ($(this).attr('data-acao') == '0') {
                $('.collapsandeando').collapse('show');
                $(this).attr('data-acao','1');
                $(this).html('<span class="glyphicon glyphicon-resize-small"></span> Retrair Ações');

            } else {
                $('.collapsandeando').collapse('hide');
                $(this).attr('data-acao','0');
                $(this).html('<span class="glyphicon glyphicon-resize-full"></span> Expandir Ações');
            }
        });
        $('table').css('margin-bottom','0');

        $('#periodo').on('change',function(){
           if($('#periodo').val() != ''){
                var url = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A';
                var params = { requisicao: 'carregaSubacao', sbacod: $('[name=sbacod]').val(), prfid: $('#periodo').val()};
                console.log(params);
                $.post(url, params, function(response){
                    $('#modal-confirm .modal-body p').empty();
                    $('#modal-confirm .modal-body p').html(response);
                });
           }
        });
        setTimeout(function(){
            $('#periodo').chosen()
        },1000);

    });

</script>
<?php
/**
 * Arquivo de informações da ação.
 * $Id: inicio.inc 95729 2015-03-24 17:43:21Z lindalbertofilho $
 */
?>
<div class="row">
    <div class="col-md-12">
        <div class="col-md-8 col-md-offset-2">
            <div class="panel panel-primary">
                <div class="panel-heading">Coordenador da Subação</div>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Nome</th>
                            <th>CPF</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td style="width:50%"><?php echo $subacao['usunome']?$subacao['usunome']:'<center>-</center>'; ?></td>
                            <td class="text-center"><?php echo $subacao['usucpf']?formatar_cpf($subacao['usucpf']):'<center>-</center>'; ?></td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>
<section class="well">
    <section class="form-horizontal" method="post" action="" id="formulario">
        <input type="hidden" name="apenasListar" value="<?=$apenasListar?>">
        <input type="hidden" name="sbacod" value="<?=$subacao['sbacod'] ? $subacao['sbacod'] : $_POST['sbacod'] ?>">
        <input type="hidden" name="existe" value="<?=$subacao['sbacod'] ? 1 : 0 ?>">
        <input type="hidden" name="tp" value="<?=$tipo?>">
        <section class="form-group">
            <label for="periodo" class="col-lg-2 control-label">Período de Referência:</label>
            <section class="col-lg-10">
                <?php
                inputCombo('prfidIn', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo');
                ?>
            </section>
        </section>
        <section id="mensagemErro">
            <section class="alert alert-info text-center">Não Foram encontrados dados para a Subação <b><?=$_GET['sbacod']?></b> no período selecionado.</section>
        </section>
        <section id="corpo">
        <section class="form-group">
            <label class="control-label col-lg-2">
                Subação:
            </label>
            <section class="col-lg-8">
                <p class="form-control-static"><?= $subacao['sbacod'] . ' - '. $subacao['sbatitulo']?>.</p>
            </section>
        </section>
        <section class="form-group">
            <label class="control-label col-lg-2">
                Unidade Responsável pela subação:
            </label>
            <section class="col-lg-8">
                <p class="form-control-static"><?= $subacao['pigdsc']?>.</p>
            </section>
        </section>
        <section class="form-group">
            <label class="control-label col-lg-2">
                Ações:
            </label>
            <section class="col-lg-10">
                <button type="button" id="collapseButton" data-acao="1" class="btn btn-default"><span class="glyphicon glyphicon-resize-small"></span> Retrair Ações</button>
            </section>
        </section>
        <!-- Ações -->
        <section class="col-md-12">
            <section class="panel-group form-group" id="accordion" role="tablist" aria-multiselectable="true">
        <?php
        foreach($acoes as $acao):
        ?>
            <section class="panel panel-default">
                <section class="panel-heading" role="tab" style="overflow: auto;">
                    <small style="float:right;clear:both;"><b>Data de Referência: <?= $acao['datareferencia'];?></b></small>
                    <h4 class="panel-title" style="width: 80%; float:left;">
                        <span >
                        <a data-toggle="collapse" style="color:#333333;" class="text-uppercase" data-parent="#accordion" aria-controls="<?= $acao['acacod'] ?>" href="#<?= $acao['acacod'] ?>">
                            Ação <?= $acao['acacod'] ." - ". $acao['acadsc']; ?>
                        </a>
                        </span>
                    </h4>
                </section>
                <section id="<?= $acao['acacod'] ?>" class="panel-collapse collapsandeando in" style="overflow-x: auto;">
                <?
                $cabecalho = array(
                    'PTRES',
                    'Plano Orçamentário',
                    'Produto',
                    'Meta do PTRES',
                    'Análise da execução da subação no PTRES',
                    'Contribuição da subação na meta física do PTRES',
                    'Orçamento do PTRES nesta subação',
                    'Empenhado do PTRES nesta subação',
                    'Liquidado do PTRES nesta subação',
                    'Pago do PTRES neste subação'
                );
                $listagem = new Simec_Listagem();
                $listagem->esconderColunas('aspid');
                $listagem->setFormOff();
                $listagem->setCabecalho($cabecalho);
                $listagem->setDados($acao['dadosPtres']);
                $listagem->addCallbackDeCampo('plocod', 'alinhaParaEsquerda');
                $listagem->addCallbackDeCampo('analiseexecucao', 'callbackCampoAnalise');
                $listagem->addCallbackDeCampo('metafisicareprogramada', 'callbackCampoMetaFisica');
                $listagem->addCallbackDeCampo(array('vlrdotacao', 'vlrempenhado', 'vlrliquidado', 'vlrpago'), 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('vlrdotacao', 'vlrempenhado', 'vlrliquidado', 'vlrpago'));
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
                </section>
            </section>
        <?
        endforeach;
        ?>
            </section>
        </section>
        <? if(count($acoes) < 1): ?>
            <section class="alert alert-warning col-md-4 col-md-offset-4 text-center" role="alert">
                <span class="glyphicon glyphicon-exclamation-sign"></span> Nenhum registro encontrado.
            </section>
        <? endif; ?>
        <!-- Questionários -->
        <section class="panel panel-default">
            <section class="panel-heading"><strong>Questionário de Acompanhamento das Subações</strong></section>
            <section class="panel-body">
                <? foreach($questionarios as $questionario): ?>
                <? $cont = 0; ?>
                <section class="form-group">
                    <section class="col-lg-12">
                        <h4 class="text-primary">Questionário - <small><b><?=$questionario['qstnome']?></b></small></h4>
                    </section>
                </section>
                    <? foreach($questionario['questoes'] as $questao): ?>
                    <?php
                    $cont++;
                    $required = $questao['qprobrigatorio'] == 't' ? 'required' : '';
                    $requiredText = $questao['qprobrigatorio'] == 't' ? 'Campo com preenchimento obrigatório*' : '' ;
                    ?>
                <section class="form-group">
                    <section class="col-lg-11 col-lg-offset-1">
                        <p style="line-height: 25px;text-align:justify;"><b><?=$questao['qprordem'] ? $questao['qprordem'] : $cont?>)</b> <?=$questao['qprpergunta']?></p>
                        <input type="hidden" name="qpridHidden[<?=$questao['qprid']?>]" value="<?=$questao['qsrid']?>">
                        <? if($apenasListar): ?>
                        <p class="form-control-static" style="text-align:justify;line-height: 150%;"><strong>R:</strong> <?=$questao['resposta'] ? $questao['resposta'] : 'N/A'?></p>
                        <? else: ?>
                        <textarea name="qprid[<?=$questao['qprid']?>]" rows="5" class="form-control" <?=$required?> data-id="<?=$questao['qprid']?>" maxlength="<?=$questao['qprnumcaracteres']?>"><?= $questao['resposta'] ? $questao['resposta'] : $questao['qprrespostapadrao']?></textarea>
                        <p class="help-block"><?=$requiredText?></p>
                        <? endif; ?>
                    </section>
                </section>
                    <? endforeach; ?>
                <!-- FIM Questionários -->
                <? endforeach; ?>
                <? if(count($questionarios) < 1): ?>
                    <section class="alert alert-info text-center col-md-4 col-md-offset-4" role="alert">
                        <span class="glyphicon glyphicon-exclamation-sign"></span> Nenhum registro encontrado.
                    </section>
                <? endif; ?>
            </section>
        </section>
        </section>
    </section>
</section>