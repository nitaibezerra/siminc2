<?php
$dados = $modelo->listaPeriodoRelatorioTCU();
?>
<script src="/spo/js/spo.js" type="text/javascript"></script>

<button class="btn btn-warning" id="voltar" style="margin-bottom: 15px;" data-url="spo.php?modulo=inicio&acao=C"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>

<table class="table table-bordered">
    <thead>
        <tr class="active">
            <th colspan="12">SPO <?=$_SESSION['exercicio']?><br>Calendário de Período de Acompanhamento de Relatório do TCU</th>
        </tr>
        <tr id="meses">
            <? for($i = 1; $i <= 12; $i++): ?>
            <th data-col="<?=$i?>"><?=$i?></th>
            <? endfor; ?>
        </tr>
    </thead>
    <tbody id="calendario">
        <?php
        foreach($dados as $data => $value):
        ?>
        <tr>
        <?
            $data ++;
            for($i = 1; $i <= 12; $i++):
        ?>
            <td data-col="<?=$i?>" style="text-align:center;">
        <?
                if($i == $value['mes_inicio']){
                    echo "<span data-pr=\"{$data}\" data-pos=\"init\">{$value['prftitulo']}</span> ";
                }
                if($i == $value['mes_fim']){
                    echo "<span data-pr=\"{$data}\" data-pos=\"final\"></span> ";
                }
        ?>
            </td>
        <?
            endfor;
        ?>
        </tr>
        <?
        endforeach;
        ?>
    </tbody>
</table>
<section class="panel panel-default">
    <section class="panel-heading"><strong>Detalhe Período</strong></section>
    <section class="panel-body">
        <section class="row">
        <? foreach ($dados as $data): ?>
            <section class="col-md-6">
                <table class="table table-bordered table-condensed">
                    <thead>
                        <tr class="active">
                            <th colspan="2">Período <?= $data['prftitulo']?></th>
                        </tr>
                        <tr class="active">
                            <th>Data Início</th>
                            <th>Data Fim</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td class="text-center"><?=$data['prfinicio']?></td>
                            <td class="text-center"><?=$data['prffim']?></td>
                        </tr>
                    </tbody>
                </table>
            </section>
        <? endforeach; ?>
        <? if(count($dados) < 1): ?>
            <section class="col-md-4 col-md-offset-4 alert alert-info text-center" style="margin-bottom: 0;">
                Não existem Períodos cadastros para este exercício.
            </section>
        <? endif; ?>
        </section>
    </section>
</section>
<script>
    $(document).ready(function(){
        $.each($('#meses th[data-col]'),function(index,val){
            $($('#meses th[data-col]')[index]).html(retornaMes($($('#meses th[data-col]')[index]).attr('data-col')));
        });

        $.each($('[data-pr]'),function(index,val){
            $(val).css('background-color',retornaCor($(val).attr('data-pr'))).css('color','white');
        });

        $.each($('#calendario tr td'),function(index,val){
            var span = $(val).find('span');
            if($(span).html() != undefined){
                var cor = $(span).css('background-color');
                if($(span).attr('data-pos') == 'init'){
                    verificaIndexTable(index,cor);
                }
            }
        });

        $('#voltar').on('click',function(){
            window.location = $(this).attr('data-url');
        });
    });
</script>