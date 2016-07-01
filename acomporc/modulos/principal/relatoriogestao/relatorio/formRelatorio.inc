<? if(!$dadosAcao): ?>
    <section class="alert alert-danger text-center">Relatório não encontrado!</section>
    <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
<? else: ?>
<style type="text/css">
.form-group img{display:none}
</style>
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
$(document).ready(function() {
    $('textarea').addClass('form-control');
    $('.radio_acaprioritaria').on("change", function () {
        var escolha = $(this).val();
        if ('t' === escolha) {
            $('#casoPositivo').show();
        } else {
            $('#casoPositivo').hide();
        }
        $('#rldacaoprioritaria').val(escolha);
    });
    $('.radio_acaprioritipo').on("change", function () {
        var escolha = $(this).val();
        $('#rldacaoprioritariatipo').val(escolha);
    });

    $('#relGestao').submit(function(){
        if ('t' === $('#rldacaoprioritaria').val()) {
            var acaprioritipo = $('#rldacaoprioritariatipo').val();

            if (('' == acaprioritipo) || ('undefined' === acaprioritipo)) {
                $('#modal-alert .modal-body').html("Ao definir uma ação como <u>prioritária</u>, você deve classificá-la no campo 'Caso Positivo'.");
                $('#modal-alert').modal();
                return false;
            }
        }
    });

    // -- Controlando os valores da ação prioritária
    var valAcaoPrioritaria = $('#rldacaoprioritaria').val();
    if ('t' == valAcaoPrioritaria) {
        $('#acaoprioritaria_t').click();

        var valAcaoPrioritariaTipo = $('#rldacaoprioritariatipo').val();
        if ('' != valAcaoPrioritariaTipo) {
            $('#acaprioritariatipo_'+valAcaoPrioritariaTipo.toLowerCase()).click();
        }
    }

    // -- Ativando a máscara dos campos numéricos
    $('.numerico').blur();

    // -- Gerando o PDF em outra janela
    $('#gerar-pdf').click(function(e){
        e.stopPropagation();
        window.open(window.location+'&pdf=true&rldid='+$('#rldid').val(), 'simec-pdf');
    });
	// -- Gerando o XLS em outra janela
    $('#gerar-xls').click(function(e){
        e.stopPropagation();
        window.open(window.location+'&xls=true&rldid='+$('#rldid').val(), 'simec-xls');
    });

    // -- Salvar responsável
    $('.modal-dialog').on('click', '#salvar-responsavel', function(){
        $('#form-responsavel').submit();
    });

    // -- Carregar conteúdo da modal
    $('#mostar-modal-alterar-responsavel').click(function(){
        var url = 'acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&aba=acao',
        params = {rldid: $(this).attr('data-rldid'), usucpf: $(this).attr('data-usucpf'), requisicao: 'carregarUsuarios', rlgid: $('input[type="hidden"][name="dados[rlgid]"]').val()};
        $.post(url, params, function(response){
            $('#modal-confirm .modal-body').html(response);
            $('.modal-dialog').css('width', '60%');
            $('#modal-confirm .modal-title').html('Alteração de Responsável');
            $('#modal-confirm .btn-primary').attr('id','salvar-responsavel').html('<span class="glyphicon glyphicon-ok"></span> Salvar');
            //$('#modal-confirm .btn-primary').html('Salvar');
            $('#modal-confirm .btn-default').html('Cancelar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    });
});
</script>
<div class="row">
    <div class="col-md-2">
        <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
    </div>
    <div class="col-md-8">
        <div class="panel panel-info">
            <div class="panel-heading">Informações da Ação</div>

            <table class="table">
                <tr>
                    <td style="width:25%"><strong>Unidade Orçamentária:</strong></td>
                    <td colspan="5"><?php echo "{$dadosAcao['unicod']} - {$dadosAcao['unidsc']}"; ?></td>
                </tr>
                <tr>
                    <td><strong>Ação:</strong></td>
                    <td><?php echo $dadosAcao['acacod']; ?></td>
                    <td><strong>Tipo:</strong></td>
                    <td><?php echo formatarAcaTipo($dadosAcao['acatipo']); ?></td>
                    <td><strong>Período de Referência:</strong></td>
                    <td><?php echo $dadosAcao['prftitulo']; ?></td>
                </tr>
            </table>
        </div>
    </div>
</div>
    <?
    // -- URL base das abas
    $urlBaseDasAbas = '/acomporc/acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&requisicao=relatorio&periodo='.$prfid.'&rlgid='.$rlgid.'&aba=';

    // -- Gerenciamento de abas
    $listaAbas = array();
    if ('N' == $dadosAcao['acatipo']) {
        if (!$usuarioTCU) {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'acao');
        } else {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'justificativaacao');
        }
        if (!$usuarioTCU) {
            $listaAbas[] = array("id" => 1, "descricao" => "Ação", "link" => "{$urlBaseDasAbas}acao");
        }
        $listaAbas[] = array('id' => 2, 'descricao' => 'Justificativa da Ação', 'link' => "{$urlBaseDasAbas}justificativaacao");

        // -- Aba de POs só é exibida para usuário TCU, se ele tiver alguma permissão cadastrada
        if ((!$usuarioTCU) || ($usuarioTCU && in_array('po', $permissoesRld))) {
            $listaAbas[] = array("id" => 3, "descricao" => "Planos Orçamentários", "link" => "{$urlBaseDasAbas}po");
        }

        // -- Abas exibidas apenas qdo solicitadas
        if ('detalhamentopo' == $abaAtiva) {
            $complemento = "&rldid={$_REQUEST['rldid']}";
            $listaAbas[] = array(
                'id' => 4,
                'descricao' => 'Detalhamento Plano Orçamentário',
                'link' => "{$urlBaseDasAbas}detalhamentopo{$complemento}"
            );
        }
        if ('justificativapo' == $abaAtiva) {
            $complemento = "&rldid={$_REQUEST['rldid']}";
            $listaAbas[] = array(
                'id' => 5,
                'descricao' => 'Justificativa do Plano Orçamentário',
                'link' => "{$urlBaseDasAbas}justificativapo{$complemento}"
            );
        }
    } else { // -- 'R' == $dadosacao['acatipo']
        if (!$usuarioTCU) {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'acaorap');
        } else {
            $abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'justificativaacaorap');
        }
        if (!$usuarioTCU) {
            $listaAbas[] = array("id" => 1, "descricao" => "Ação RAP", "link" => "{$urlBaseDasAbas}acaorap");
        }
        $listaAbas[] = array('id' => 2, 'descricao' => 'Justificativa da Ação RAP', 'link' => "{$urlBaseDasAbas}justificativaacaorap");
    }

    echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}{$complemento}");

    /**
     * Incluíndo o arquivo de acordo com a aba selecionada.
     * Para cada aba, é chamado o arquivo início dentro do diretório
     * com o mesmo nome da aba selecionada.
     */
    require_once(dirname(__FILE__) . "/aba/{$abaAtiva}/inicio.inc");
    ?>
<? endif;