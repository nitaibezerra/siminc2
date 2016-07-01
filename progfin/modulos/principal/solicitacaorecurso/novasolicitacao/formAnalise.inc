<?php
/**
 * Formulário de análise de pedido de alteração.
 *
 * Este arquivo é incluído dentro dos métodos de geração de abas, para
 * acessar variáveis daquele escopo, elas precisam ser declaradas como globais.
 *
 * $Id: formAnalise.inc 102139 2015-09-04 18:42:55Z maykelbraz $
 */
// -- ver comentários no topo do arquivo
global $result;

/* Recuperar dados da Análise PEDIDO */
$dadosAnalise = carregarDadosAnalise($_REQUEST['id']);
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript">
    $(document).ready(function () {
        $('#confirmar-envio-siafi').on('show.bs.modal', function (e) {
            var $form = $('#form-envio-siafi');
            $('input.pf', $form).remove();

            // -- Validando os valores informados, não pode deixar passar valor igual a 0
            var todosPreenchidos = true
                    , totalAprovados = [];

            $('input[type="checkbox"]:checked').each(function () {
                var aprovado = $("#aprovado_" + $(this).val()).val();
                //atendido = $("#atendido_"+$(this).val()).val();

                aprovado = parseFloat(aprovado.replace('.', '').replace(',', '.'));
                //atendido = parseFloat(atendido.replace('.', '').replace(',', '.'));
                totalAprovados.push($("#aprovado_" + $(this).val()).val());

                if ((0 === aprovado)) {  //|| (0 === atendido)
                    todosPreenchidos = false;
                }
            });

            if (!todosPreenchidos) {
                bootbox.alert("Antes de prosseguir com o envio, os valores 'Aprovado' e 'Atendido' dos pedidos selecionados devem ser preenchidos.");
                return false;
            }

            $('input[type="checkbox"]:checked').each(function () {
                $form.append('<input type="hidden" name="lfnid[]" value="' + $(this).val() + '" class="pf pf_" />');
                $form.append('<input type="hidden" name="aprovado[' + $(this).val() + ']" value="' + $("#aprovado_" + $(this).val()).val() + '" class="pf" />');
                //$form.append('<input type="hidden" name="atendido['+$(this).val()+']" value="' + $("#atendido_"+$(this).val()).val() + '" class="pf" />');
            });
            var qtdPedidos = $form.children('input.pf_').length;
            $('#qtd-pedidos', $form).text(qtdPedidos);

            if (totalAprovados.length) {
                $.ajax({
                    url: "progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A",
                    data: {somaLiberacoesAnalise: 'true', totalAprovados: totalAprovados.join("|")},
                    type: "post",
                    beforeSend: function () {
                        $('#total-aprovado', $form).text("carregando...");
                    },
                    success: function (data) {
                        $('#total-aprovado', $form).text(data);
                    }
                });
            }

            if (qtdPedidos <= 0) {
                bootbox.alert('Selecione ao menos uma liberação para realizar o envio.');
                return false;
            }
        });

        $('#form-envio-siafi').submit(function () {
            if (!$('#siafi_usuario', $(this)).val()
                    || !$('#siafi_password', $(this)).val()
                    || !$('#siafi_ug', $(this)).val()) {
                bootbox.alert('Todos os dados do usuário, e a observação, devem ser preenchidos para realizar o envio das liberações.');
                return false;
            }
        });

        $('#btn-salvar').click(function () {
            bootbox.confirm('Confirma a alteração dos valores financeiros?', function (result) {
                if (result) {
                    $('#requisicao').val('salvarValoresFinanceiros');
                    $('#recurso').submit();
                }
            });
        });



        $('[data-toggle="popover"]').popover({trigger: 'hover', placement: 'left'});
        somarEnvio();

        $('.somarTotalEnvio').focus(function () {
            if($(this).val() == '0,00'){
                $(this).val('');
            }
        });
        $('.somarTotalEnvio').blur(function (){
            if($(this).val() == ''){
                $(this).val('0,00');
            }
             somarEnvio();
        });

        $('.somarTotalEnvio').keyup(function (){
            somarEnvio();
        });


    });

    function moedaParaFloat(numero) {
            var num = numero;
            num = num.replace('.', '');
            num = num.replace('.', '');
            num = num.replace('.', '');
            num = num.replace('.', '');
            num = num.replace(',', '.');
            return parseFloat(num).toFixed(2);
    }

    function floatParaMoeda(num) {
            var num = parseFloat(num).toFixed(2);
            x = 0;
            if (num < 0) {
                num = Math.abs(num);
                x = 1;
            }
            if (isNaN(num))
                num = "0";
            cents = Math.floor((num * 100 + 0.5) % 100);

            num = Math.floor((num * 100 + 0.5) / 100).toString();

            if (cents < 10)
                cents = "0" + cents;
            for (var i = 0; i < Math.floor((num.length - (1 + i)) / 3); i++)
                num = num.substring(0, num.length - (4 * i + 3)) + '.'
                        + num.substring(num.length - (4 * i + 3));
            ret = num + ',' + cents;
            if (x == 1)
                ret = ' - ' + ret;
            return ret;
    }
     function somarEnvio(){
        var total = 0;
        $('.somarTotalEnvio').each(function(){
            var numero = moedaParaFloat($(this).val());
            var valor = Number(numero);
            if (!isNaN(valor)) total += valor;
        });
        $('#total_para_envio').html(floatParaMoeda(total));
    }
</script>
<style type="text/css">
.linha_filha td {
    background-color: #ffff55 !important;
}
</style>
<form name="recurso" id="recurso" method="POST" class="well form-horizontal" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <input type="hidden" name="unicod" value="<?php echo $dados['unicod']; ?>" />
    <input type="hidden" name="dados[lfnid]" id="lfnid" value="<?php echo $result['lfnid']; ?>" />

    <div class="form-group ">
        <label class="control-label col-md-2" for="">Total para o envio (R$):</label>
        <div id="total_para_envio" class="col-md-4" style="color:green; padding:8px; font-weight: bold">
            0,00
        </div>
    </div>
    <div class="form-group row">
        <?php
        $cabecalho = array(
            '<span class="glyphicon glyphicon-send"></span>',
            'Criação',
            'Unidade Gestora (UG)',
            'Classificação',
            'Situação Contábil',
            'Fonte',
            'Cat. Gasto',
            'Vinc.',
            'Observação',
            'Liberação de Recursos (R$)' => array('Solicitado', 'Aprovado', 'Atendido'),
            'PF',
            'Situação',
            'Data Liberação'
        );
        $listagem = new Simec_Listagem();
        $listagem->addCallbackDeCampo(array('solicitado', 'aprovado', 'atendido'), 'mascaraMoeda')
                ->addCallbackDeCampo('lfnobservacao', 'diminuirFonte')
                ->addCallbackDeCampo('numpedido', 'formatarPedido')
                ->addCallbackDeCampo('esddsc', 'colocaIcone')
                ->addCallbackDeCampo('enviarpedido', 'checkboxEnviar')
                ->addCallbackDeCampo('autorizado', 'campoEditavelAutorizado')
                ->addRegraDeLinha(
                    array('campo' => 'origem', 'op' => 'diferente', 'valor' => 'id', 'classe' => 'linha_filha', 'valorComoCampo' => true)
                )
                ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('solicitado', 'autorizado', 'atendido'))
                ->turnOnPesquisator()
                ->setQuery($dadosAnalise['liberacoes']);

        $colunasOcultas = array('llfid', 'esdid', 'lfnmensagem', 'docid', 'plfid', 'lfnid2', 'lfnidorigem', 'origem');
        if (array_intersect(pegaPerfilGeral(), array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA))) {
            $listagem->addAcao('workflow', array(
                'func' => 'drawWorkflow',
                'extra-params' => array('docid', 'plfid')
            ));
        } else {
            $colunasOcultas[] = 'lfnid';
            $colunasOcultas[] = 'enviarpedido';
            array_shift($cabecalho);
        }

        $listagem->esconderColunas($colunasOcultas)
            ->setCabecalho($cabecalho);

        /* Passando todas para em análise para poder enviar para o SIAFI */
        if (false === $listagem->render()):
            ?>
            <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
        <?php elseif (array_intersect(pegaPerfilGeral($_SESSION['usucpf']), array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA))): ?>
            <button type="button" class="btn btn-primary" id="btn-salvar">Salvar Alterações</button>
            <button type="button" class="btn btn-success" id="btn-enviar"
                    data-toggle="modal" data-target="#confirmar-envio-siafi">Enviar selecionados para o SIAFI</button>
                <?php endif;
        ?>
    </div>
    <br />
</form>
<div class="modal fade" id="confirmar-envio-siafi">
    <div class="modal-dialog">
        <div class="modal-content">
            <form id="form-envio-siafi" method="POST" role="form" class="form-horizontal">
                <input type="hidden" value="enviar-liberacoes" name="requisicao" />
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">
                        <span aria-hidden="true">&times;</span><span class="sr-only">Fechar</span>
                    </button>
                    <h4 class="modal-title">Confirmação de envio</h4>
                </div>
                <div class="modal-body">
                    <p>Tem certeza que deseja proceder com o envio dos pedidos selecionados ao SIAFI? <br/><br/>
                        Será enviado um total de <b style="color:green;"><span id="qtd-pedidos"></span></b> registro(s).<br /><br/>
                        Total aprovado a ser enviado: <b style="color:green;"><span id="total-aprovado"></span></b></p>
                    <br />
                    <div class="well">
                        <div class="form-group">
                            <label for="siafi_usuario" class="control-label col-md-4">Usuário SIAFI:</label>
                            <div class="col-md-8">
                                <?php inputTexto('siafi_usuario', null, 'siafi_usuario', 14, false, array('masc' => '###.###.###-##')); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="siafi_password" class="control-label col-md-4">Senha SIAFI:</label>
                            <div class="col-md-8">
                                <input type="password" name="siafi_password" id="siafi_password" maxlength="50"
                                       class="form-control" />
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="siafi_ug" class="control-label col-md-4">UG (do usuário):</label>
                            <div class="col-md-8">
                                <?php inputTexto('siafi_ug', null, 'siafi_ug', 6, false, array('masc' => '######')); ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
                    <button type="submit" class="btn btn-success">Confirmar</button>
                </div>
            </form>
        </div>
    </div>
</div>