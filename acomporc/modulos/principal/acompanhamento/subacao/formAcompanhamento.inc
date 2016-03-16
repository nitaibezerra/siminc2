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
    $(document).ready(function(){
        $('#workflow_body table').css('width','100%');
        $('#mensagemErro').hide();
        if(parseInt($('input[name="existe"]').val()) == 0){
            $('#mensagemErro').show();
            $('#corpo').hide()
        }
        if($('input[name="apenasListar"]').val() == true){
            $.each($('table textarea'),function(index,value){
                $(value).parent().html('<p class="form-control-static">'+$(value).val()+'</p>');
            });
            $.each($('table input'),function(index,value){
                $(value).parent().html('<p style="text-align:right!important">'+mascara($(value).val())+'</p>');
            });
            $('#definir_responsavel').attr('disabled','disabled');
        }
        $('table').css('margin-bottom','0');

        $('#periodo').on('change',function(){
           if($('#periodo').val() != ''){
               window.location.href = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&requisicao=acessar&periodo='+$('#periodo').val()+'&sbacod='+$('[name=sbacod]').val();
           }
        });

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

        $('#buttonSave').on('click',function(){
            if($('input[name="apenasListar"]').val() == true){
                bootbox.alert('Preenchimento de preechimento expirado.');
                return;
            }
            var error = 0;
            $.each($('section[class="panel panel-success"] input[class="form-control"]'),function(index,value){
                if($(this).val() == '0'){
                    //bootbox.alert('Meta Física não pode ser 0');
                    $(this).focus();
                    //return false;
                }
            });
            $.each($('section[class="panel panel-success"] textarea'),function(index,value){
                if(trim($(this).val()) == ""){
                    error += 1;
                    $(this).focus();
                }
            });

            $.each($('form textarea[required]'), function(index,value){
                 if(trim($(this).val()) == ""){
                    error += 1;
                    $(this).focus();
                }
            });

            if(error != 0){
                bootbox.confirm('O acompanhamento não foi preenchido completamente. Deseja prosseguir mesmo assim ?',function(result){
                    if(result){
                        $('#formulario').submit();
                    }
                });
            }else{
                $('#formulario').submit();
            }
        });
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
        <div class="col-md-1">
        </div>
        <div class="col-md-8">
            <div class="panel panel-primary">
                <div class="panel-heading"><strong>Subação e Responsáveis</strong></div>
                <table class="table">
                    <thead>
                        <tr>
                            <th class='text-left' colspan='4'>Subação</th>
                        </tr>
                        <tr>
                            <td colspan='4'><?= $subacao['sbacod'] . ' - '. $subacao['sbatitulo']?></td>
                        </tr>
                        <tr>
                            <th class='text-left' colspan='4'>Unidade Responsável</th>
                        </tr>
                        <tr>
                            <td colspan='4'><?= $subacao['pigdsc']?></td>
                        </tr>

                        <tr>
                            <th></th>
                            <th class='text-left'>Nome</th>
                            <th class='text-left'>CPF</th>
                            <?php if (!in_array(PFL_RELATORIO_TCU, $perfil)): ?>
                            <th>&nbsp;</th>
                            <?php endif; ?>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td><strong>Coordenador</strong></td>
                            <td style="width:50%"><?php echo $subacao['usunome']?$subacao['usunome']:'<center>-</center>'; ?></td>
                            <td class=""><?php echo $subacao['usucpf']?formatar_cpf($subacao['usucpf']):'<center>-</center>'; ?></td>
                            <?php if (!in_array(PFL_RELATORIO_TCU, $perfil)): ?>
                            <td style="width:20px;">
                                <button id="definir_responsavel" class="btn btn-warning btn-xs" onclick="alterarResponsavel($('[name=asaid]').val(),$('[name=sbacod]').val(),$('[name=prfid]').val(),'<?=$subacao['usucpf']?>');">
                                    <span class="glyphicon glyphicon-user"></span>
                                </button>
                            </td>
                            <?php endif; ?>
                        </tr>
                        <tr class="active">
                            <th colspan="4" class="text-left">Filtros</th>
                        </tr>
                        <tr>
                        <td colspan="1">
                            <label for="periodo">Período de Referência:</label>
                        </td>
                        <td colspan="3">
                            <?php
                            inputCombo('prfid', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo');
                            ?>
                        </td>
                    </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <section class="col-md-2">
            <section class="panel panel-info">
                <section class="panel-heading">
                    <strong>Tramitação</strong>
                </section>
                <section class="panel-body" id="workflow_body">
                <?
                wf_desenhaBarraNavegacao($subacao['docid'],array());
                ?>
                </section>
            </section>
        </section>
    </div>
</div>
<section class='row'>
<section id="mensagemErro">
    <section class="alert alert-warning text-center col-md-6 col-md-offset-3">Não Foram encontrados dados para a Subação <b><?=$_GET['sbacod']?></b> no período selecionado.</section>
</section>
</section>
<section class="well">
    <form class="form-horizontal" method="post" action="" id="formulario">
        <input type="hidden" name="apenasListar" value="<?=$apenasListar?>">
        <input type="hidden" name="tp" value="<?=$tipo?>">
        <input type="hidden" name="asaid" value="<?=$subacao['asaid']?>">
        <input type="hidden" name="sbacod" value="<?=$subacao['sbacod'] ? $subacao['sbacod'] : $_GET['sbacod'] ?>">
        <input type="hidden" name="existe" value="<?=$subacao['sbacod'] ? 1 : 0 ?>">
        <input type="hidden" name="requisicao" value="gravarAcompanhamento">

        <section id="corpo">
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
            <section class="alert alert-warning alert-dismissible fade in text-center" role="alert">
                <button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <span class="glyphicon glyphicon-exclamation-sign"></span> <strong>Ações não encontradas!</strong>
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
        <? if($apenasListar): ?>
        <? gravacaoDesabilitada('Periodo de preenchimento expirado.');?>
        <? endif;?>
        </section>
        <section class="form-group">
            <section class="col-lg-2">
            </section>
            <section class="col-lg-8">
                <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&periodo=<?=$prfid?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar ao Acompanhamento</button>
                <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/acompanhamento/central&acao=A&tipo=<?=$tipo?>"><span class="glyphicon glyphicon-arrow-left"></span> Ir à Central</button>
                <? if(!$apenasListar): ?>
                <button type="button" class="btn btn-primary" id="buttonSave" <?= $apenasListar ? 'disabled' : ''?>><span class="glyphicon glyphicon-ok"></span> Salvar</button>
                <? endif; ?>
            </section>
        </section>
    </form>
</section>