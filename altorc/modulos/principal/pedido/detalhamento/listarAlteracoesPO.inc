<?php

if ( empty($dadospao['mcrid']) ) {
        die("<script>
                alert('Faltam parâmetros para acessar esta tela!');
                location.href = '?modulo=inicio&acao=C';
             </script>");
}
/**
 * Listagem das alteraçoes solicitadas para o PO.
 * Este arquivo também é utilizado na chamada ajax de dados do pedido na aba resumo/tramite.
 * No caso de ser utilizada neste contexto, são feitas algumas modificações na página, como
 * a remoção das ações da listagem, o ocultamente de algumas colunas e a exibição de itens
 * que só tem utilidade no contexto principal da chamada deste arquivo (filtros e formulários).
 * $Id: listarAlteracoesPO.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

// -- O título da sessão, formulário de filtro e formulário de criação não precisam
// -- ser criados/exibidos durante um detalhamento.
if (!isset($detalharPO)): ?>
<script>
    function toggleSuplementacao(fonrecurso, foncod, retornaAntesDeMostrar)
    {
        var tcrcod = $('#tcrcod').val();
        $('.fonte-recurso').css('display', 'none');
        if (('103' != tcrcod && '103a' != tcrcod)) {
            $('#frcancelamento').css('display', 'block');
        }
        if ('-' != fonrecurso) {
            $('#frec_' + fonrecurso).css('display', 'block');
            $('#tfrcod').val(fonrecurso);
            if (retornaAntesDeMostrar) {
                return;
            }
        } else {
            $('#tfrcod').val('');
            if ('100' == tcrcod) {
                $('.fonte-recurso').css('display', 'block');
            } else if ('103' == tcrcod || '103a' == tcrcod) {
                $('#frcancelamento').css('display', 'none');
                var grupofonte = foncod.substr(0, 1);
                if ('350' == foncod || '650' == foncod || '680' == foncod || '681' == foncod || '696' == foncod) {
                    $('#frec_3').css('display', 'block');
                } else {
                    if ('1' == grupofonte || '2' == grupofonte) {
                        $('#frec_2').css('display', 'block');
                    } else if ('3' == grupofonte || '6' == grupofonte) {
                        $('#frec_3').css('display', 'block');
                    }
                }
            } else if ('120' == tcrcod) {
                $('#frec_1').css('display', 'block');
                $('#frec_2').css('display', 'block');
                $('#frec_3').css('display', 'block');
                $('#frec_4').css('display', 'block');
            } else if ('156' == tcrcod) {
                $('#frec_1').css('display', 'block');
                $('#frec_2').css('display', 'block');
                $('#frec_3').css('display', 'block');
            } else if ('192' == tcrcod) {
                $('#frec_3').css('display', 'block');
            } else if ('175a' == tcrcod) {
                $('#frec_3').css('display', 'block');
            } else {
                $('#frec_1').css('display', 'block');
            }
        }

        var spoid = $('#spoid').val();
        if (!spoid) {
            $('#frcancelamento').css('display', 'none');
        }
    }

    function editarAlteracaoPO(spoid, plocod, dotatual, natcod, valsuplementacao, valcancelamento, idusocod, foncod, idoccod, fonrecurso, rpcod, rpleicod, plodsc)
    {
        $('#modal-alteracao-po .has-error').removeClass('has-error');

        $('#modal-alteracao-po #spoid').val(spoid);
        $('#modal-alteracao-po #plocod').val(plocod);
        $('#modal-alteracao-po #plocod_span').text(plocod + ' - ' + plodsc);
        $('#modal-alteracao-po #dotatual').text(number_format(dotatual, 2, ',', '.'));
        $('#modal-alteracao-po #dotaprovada').text(number_format(dotatual, 2, ',', '.'));

        $('#modal-alteracao-po #valsuplementacao').val('0').attr('data-default-value', '0').blur();
        $('#modal-alteracao-po #valexcesso').val('0').attr('data-default-value', '0').blur();
        $('#modal-alteracao-po #valsuperavit').val('0').attr('data-default-value', '0').blur();
        $('#modal-alteracao-po #valopcredito').val('0').attr('data-default-value', '0').blur();

        if ('2' == fonrecurso) {
            $('#modal-alteracao-po #valexcesso').val(valsuplementacao).attr('data-default-value', valsuplementacao).blur();
        } else if ('3' == fonrecurso) {
            $('#modal-alteracao-po #valsuperavit').val(valsuplementacao).attr('data-default-value', valsuplementacao).blur();
        } else if ('4' == fonrecurso) {
            $('#modal-alteracao-po #valopcredito').val(valsuplementacao).attr('data-default-value', valsuplementacao).blur();
        } else { // '1' == fonrecurso
            $('#modal-alteracao-po #valsuplementacao').val(valsuplementacao).attr('data-default-value', valsuplementacao).blur();
        }

        $('#modal-alteracao-po #valcancelamento').val(valcancelamento).attr('data-default-value', valcancelamento).blur();
        $('#modal-alteracao-po #natcod').val(natcod).attr('disabled', true).trigger('chosen:updated');
        $('#modal-alteracao-po #idusocod').val(idusocod).attr('disabled', true).trigger('chosen:updated');
        $('#modal-alteracao-po #foncod').val(foncod).attr('disabled', true).trigger('chosen:updated');
        $('#modal-alteracao-po #idoccod').val(idoccod).attr('disabled', true).trigger('chosen:updated');
        $('#modal-alteracao-po #rpcod').val(rpcod).attr('disabled', true).trigger('chosen:updated');
        $('#modal-alteracao-po #rpleicod').val(rpleicod).attr('disabled', true).trigger('chosen:updated');
        $('#modal-alteracao-po #requisicao').val('alterarPO');
        $('#modal-alteracao-po #salvar-alteracao-po').text('Confirmar alteração');
        $('#modal-alteracao-po #snaid_popup').val('');

        toggleSuplementacao(fonrecurso, foncod, false);

        $('#modal-alteracao-po').modal();
    }

    function excluirAlteracaoPO(spoid, pasid)
    {
        $('#modal-confirm .modal-title').html('Exclusão de crédito');
        $('#modal-confirm .modal-body p').html('Tem certeza que deseja excluir o crédito selecionado?');

        $('#spoid_exclusao').val(spoid);
        $('#pasid_exclusao').val(pasid);
        $('#excluir_requisicao').val('excluirCredito');
        $('#modal-confirm').modal();
    }

    function limparAlteracaoPO(spoid, pasid)
    {
        $('#modal-confirm .modal-title').html('Exclusão de crédito');
        $('#modal-confirm .modal-body p').html('Tem certeza que deseja limpar o crédito selecionado?');

        $('#spoid_exclusao').val(spoid);
        $('#pasid_exclusao').val(pasid);
        $('#excluir_requisicao').val('limparCredito');
        $('#modal-confirm').modal();
    }

    $(document).ready(function() {
        $('#novoPO').click(function() {
            var plocod;
            if (!(plocod = $('#plocod').val())) {
                $('#modal-alert .modal-body').text('Antes de prosseguir, você deve escolher um plano orçamentário.');
                $('#modal-alert').modal();
                return;
            }
            var plodsc = $('#plocod_chosen .chosen-single span').html();
            // -- Configurando a janela de criação de um novo PO
            $('#modal-alteracao-po .has-error').removeClass('has-error');
            $('#modal-alteracao-po #spoid').val('');
            $('#modal-alteracao-po #plocod').val(plocod);
            $('#modal-alteracao-po #plocod_span').text(plodsc);
            $('#modal-alteracao-po #dotatual').text('0');
            $('#modal-alteracao-po #dotaprovada').text('0');
            $('#modal-alteracao-po #valsuplementacao').val('0').attr('data-default-value', '0').blur();
            $('#modal-alteracao-po #valsuperavit').val('0').attr('data-default-value', '0').blur();
            $('#modal-alteracao-po #valexcesso').val('0').attr('data-default-value', '0').blur();
            $('#modal-alteracao-po #valopcredito').val('0').attr('data-default-value', '0').blur();
            $('#modal-alteracao-po #valcancelamento').val('0').attr('data-default-value', '0').blur();
            $('#modal-alteracao-po #natcod').val('').removeAttr('disabled').trigger('chosen:updated');
            $('#modal-alteracao-po #idusocod').val('0').removeAttr('disabled').trigger('chosen:updated');
            $('#modal-alteracao-po #foncod').val('').removeAttr('disabled').trigger('chosen:updated');
            $('#modal-alteracao-po #idoccod').val('9999').removeAttr('disabled').trigger('chosen:updated');
            $('#modal-alteracao-po #rpcod').val('').removeAttr('disabled').trigger('chosen:updated');
            $('#modal-alteracao-po #rpleicod').val('2').removeAttr('disabled').trigger('chosen:updated');
            $('#modal-alteracao-po #requisicao').val('criarPO');
            $('#modal-alteracao-po #salvar-alteracao-po').text('Criar novo');
            $('#modal-alteracao-po #snaid_popup').val($('#snaid').val());

            var tcrcod = $('#tcrcod').val();
            if ('103' == tcrcod || '103a' == tcrcod) {
                $('#frcancelamento').css('display', 'none');
            }

            toggleSuplementacao('', '', false);

            // -- Quando estiver criando um novo credito, nao exibe o campo de cancelamento
            var spoid = $('#spoid').val();
            if (!spoid) {
                $('#frcancelamento').css('display', 'none');
            }

            $('#modal-alteracao-po').modal();
        });

        // -- Inicializando a modal de confirmacao
        $('#modal-confirm .btn-primary').click(function() {
            $('#excluirCredito').submit();
        });
    });
</script>
<div class="page-header">Dotações orçamentárias</div>
<input type="hidden" id="snaid" value="<?php echo $dadospao['snaid']; ?>" />
<input type="hidden" id="tcrcod" value="<?php echo $dadospao['tcrcod']; ?>" />
<form name="excluirCredito" id="excluirCredito" method="POST">
    <input type="hidden" name="dados[spoid]" id="spoid_exclusao" />
    <input type="hidden" name="dados[pasid]" id="pasid_exclusao" />
    <input type="hidden" name="requisicao" id="excluir_requisicao" />
</form>
<div class="col-md-10 col-md-offset-1">
    <div class="well well-sm row form-group">
        <div class="col-md-2">
            <label class="control-label" for="ploid">Plano orçamentário:</label>
        </div>
        <div class="col-md-9">
                <?php
                $query = <<<DML
SELECT DISTINCT spo.plocod AS codigo,
                spo.plocod || ' - ' || spo.plodsc AS descricao
  FROM altorc.snapshotplanoorcamentario spo
  WHERE spo.snaid = %d
DML;
                $stmt = sprintf($query, $dadospao['snaid']);
                $db->monta_combo('plocod', $stmt, 'S', 'Todos', null, null, null, null, 'N', 'plocod', null, '', null, 'class="form-control chosen-select" style="width=100%;""');
                ?>
        </div>
        <div class="col-md-1">
            <button type="submit" class="btn btn-default">Buscar</button>
        </div>
    </div>
</div>
<br style="clear:both" />
    <?php
endif;
$listagem = new Simec_Listagem();

// -- Se não for um detalhamento, então devemos incluir as ações da listagem.
if (!isset($detalharPO)) {
    if ($podeSalvar && $dadospao['mcrid'] == $mcridAtual) {
        $listagem->addAcao(
            'edit',
            array(
                'func' => 'editarAlteracaoPO',
                'extra-params' => array(
                    'plocod',
                    'dotatual',
                    'natcod',
                    'valsuplementacao',
                    'valcancelamento',
                    'idusocod',
                    'foncod',
                    'idoccod',
                    'tfrcod',
                    'rpcod',
                    'rpleicod',
                    'plodsc'
                )
            )
        );
        $listagem->addAcao('delete', array('func' => 'excluirAlteracaoPO', 'extra-params' => array('pasid')));
        $listagem->addAcao('clear', array('func' => 'limparAlteracaoPO', 'extra-params' => array('pasid')));
    } else {
        $listagem->esconderColuna('spoid');
    }

    $listagem->setAcaoComoCondicional('delete', array(array('campo' => 'spoalterado', 'valor' => 'N', 'op' => 'igual')))
            ->setAcaoComoCondicional('clear', array(array('campo' => 'spoalterado', 'valor' => 'M', 'op' => 'igual')));
} else { // -- Se for um detalhamento, a coluna spoid não precisa ser listada
    $listagem->esconderColuna('spoid');
}

// -- Formatando as moedas da consulta
$listagem->addCallbackDeCampo(array('dotatual', 'valsuplementacao', 'valcancelamento', 'dotprovavel', 'dotinicial'), 'mascaraMoeda')
        ->addCallbackDeCampo('spoalterado', 'statusAlteracaoPO')
        ->addCallbackDeCampo('tfrcod', 'fonteRecurso');

// -- Escondendo a coluna 'plocod'
$listagem->esconderColunas(
    array('plodsc', 'pasid', 'tfrcod', 'idusocod', 'idoccod', 'rpleicod', 'rpcod')
);

$listagem->setTotalizador(
    Simec_Listagem::TOTAL_SOMATORIO_COLUNA,
    array('dotatual', 'valsuplementacao', 'valcancelamento', 'dotprovavel', 'dotinicial')
);

$listagem->setCabecalho(
        array(
            'P.O.',
            'Natureza',
            'Fonte',
            'Dotação (R$)' => array('Inicial', 'Atual'),
            'Suplementação (R$)',
            'Cancelamento (R$)',
            'Dotação provável (R$)',
            'Condição'
        )
);

// -- No caso de um detalhamento, só é necessário mostrar os registros com modificação, então,
// -- na query abaixo, o LEFT join é trocado por um INNER join.
if (isset($detalharPO) || $mcridAtual != $dadospao['mcrid']) {
    $join = 'INNER';
} else {
    $join = 'LEFT';
}

$query = <<<DML
SELECT spo.spoid,
       spo.plocod,
       spo.plodsc,
       spo.natcod,
       idusocod,
       foncod,
       idoccod,
       rpleicod,
       rpcod,
       COALESCE(spo.dotinicial, 0.00)::numeric(15,0) AS dotinicial,
       dotatual,
       COALESCE(pas.valsuplementacao, 0.00)::numeric(15,0) AS valsuplementacao,
       COALESCE(pas.tfrcod::varchar, '-') AS tfrcod,
       COALESCE(pas.valcancelamento, 0.00)::numeric(15,0) AS valcancelamento,
       (dotatual + COALESCE(pas.valsuplementacao, 0.00) - COALESCE(pas.valcancelamento, 0.00))::numeric(15,0) AS dotprovavel,
       pas.pastip AS spoalterado,
       pas.pasid
  FROM altorc.snapshotplanoorcamentario spo
    {$join} JOIN altorc.pedidoalteracaofinanceiro pas ON (spo.spoid = pas.spoid AND pas.paoid = %d)
  WHERE spo.snaid = %d
    AND snaid IN (SELECT snaid FROM altorc.snapshotacao WHERE mcrid =  {$dadospao['mcrid']})
  ORDER BY pas.pastip
DML;
$stmt = sprintf($query, $dadospao['paoid'], $dadospao['snaid']);

$listagem->setQuery($stmt)
    ->setId('dados-po')
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

if (!isset($detalharPO)):
    if ($podeSalvar && $mcridAtual == $dadospao['mcrid']): ?>
    <button type="button" class="btn btn-success" id="novoPO">Adicionar</button>
    <br style="clear:both" />
    <br />
    <?php
        require(dirname(__FILE__) . '/formAlteracaoPO.inc');
    endif;
endif;
