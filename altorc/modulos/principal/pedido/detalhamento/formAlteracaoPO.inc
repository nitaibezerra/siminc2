<?php
/**
 * Forma de alteração / visualização de dados do PO.
 * $Id: formAlteracaoPO.inc 97087 2015-05-11 17:45:11Z werteralmeida $
 */
?>
<style>
    #natcod_chosen,#idusocod_chosen,#foncod_chosen,#idoccod_chosen,#rpcod_chosen,#rpleicod_chosen{width:100%!important}
    #modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
    @media (min-width: 992px) {.modal-lg {width:900px}}
</style>
<script type="text/javascript" language="javascript">
    $(document).ready(function() {
        $('#salvar-alteracao-po').click(function() {
            var requisicao = $('#requisicao').val();
            if ('criarPO' == requisicao) {
                // -- Validando os itens do formulario obrigatorios para criar um novo pedido
                var msg = new Array();
                var itemsParaValidacao = new Array('#natcod', '#idusocod', '#foncod', '#idoccod', '#rpcod', '#rpleicod');
                for (var x in itemsParaValidacao) {
                    // -- Selecionando o input
                    var $item = $(itemsParaValidacao[x]);
                    if (undefined == $item.attr('name')) {
                        continue;
                    }

                    if (!$item.val()) { // -- validando o conteúdo do input e selecionando o label para montar msg de erro
                        msg.push($item.parent().prev().children('label').text().replace(':', ''));
                        $item.parent().parent().addClass('has-error');
                    }
                }
                // -- Se existir alguma mensagem, exibe para o usuário
                if (msg.length > 0) {
                    var htmlMsg = '<div class="bs-callout bs-callout-danger">Antes de prosseguir, os seguintes campos devem ser preenchidos:<ul>';
                    for (var x in msg) {
                        if ('function' == typeof (msg[x])) {
                            continue;
                        }

                        htmlMsg += '<li>' + msg[x];
                        if (x == msg.length - 1) {
                            htmlMsg += '.';
                        } else {
                            htmlMsg += ';';
                        }
                        htmlMsg += '</li>';
                    }
                    htmlMsg += '</ul></div>';
                    $('#modal-alert .modal-body').html(htmlMsg);
                    $('#modal-alert').modal({backdrop: 'static'});
                    return;
                }
            }

            $('#alteracaoPO').submit();
        });
    });
    function foncodChanged(foncod)
    {
        toggleSuplementacao('-', foncod, false);
        $('#modal-alteracao-po #valsuplementacao').val('0').attr('data-default-value', '0').blur();
        $('#modal-alteracao-po #valsuperavit').val('0').attr('data-default-value', '0').blur();
        $('#modal-alteracao-po #valexcesso').val('0').attr('data-default-value', '0').blur();
        $('#modal-alteracao-po #valopcredito').val('0').attr('data-default-value', '0').blur();
    }
</script>
<div class="modal fade" id="modal-alteracao-po">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Pedido de alteração orçamentária</h4>
            </div>
            <div class="modal-body">
                <div class="panel panel-default">
                    <div class="panel-heading">Dados do P.O.</div>
                    <table class="table">
                        <tr>
                            <td><strong>Funcional programática:</strong></td>
                            <td colspan="3"><?php echo $locdados['programatica']; ?></td>
                        </tr>
                        <tr>
                            <td><strong>P.O.:</strong></td>
                            <td colspan="3"><span id="plocod_span"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Dotação:</strong></td>
                            <td>R$ <span id="dotatual"></span></td>
                            <td><strong>Dotação aprovada:</strong></td>
                            <td>R$ <span id="dotaprovada"></span></td>
                        </tr>
                    </table>
                </div>
                <?php // -- formulário --> ?>
                <div class="well well-sm row">
                    <form name="alteracaoPO" id="alteracaoPO" method="POST" role="form">
                        <input type="hidden" name="dados[spoid]" id="spoid" />
                        <input type="hidden" name="dados[snaid]" id="snaid_popup" />
                        <input type="hidden" name="dados[plocod]" id="plocod" />
                        <input type="hidden" name="requisicao" id="requisicao" />
                        <input type="hidden" name="dados[tfrcod]" id="tfrcod" />
                        <div class="form-group row">
                            <div class="col-md-4">
                                <label class="control-label" for="natcod">Natureza despesa:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $query = <<<DML
SELECT DISTINCT spo.natcod AS codigo,
                spo.natcod AS descricao
  FROM altorc.snapshotplanoorcamentario spo
  WHERE EXISTS (SELECT 1
                  FROM altorc.snapshotacao sna
                  WHERE sna.snaexercicio = '%s'
                    AND sna.snaid = spo.snaid)
  ORDER BY natcod
DML;
                                $complementoSelect = 'class="form-control chosen-select" style="width:100%"';
                                $stmt = sprintf($query, $_SESSION['exercicio']);
                                $db->monta_combo('dados[natcod]', $stmt, 'S', 'Selecione uma Natureza', null, null, null, null, 'N', 'natcod', null, '', null, $complementoSelect);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row">
                            <div class="col-md-4">
                                <label class="control-label" for="idusocod">IDUSO:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $query = <<<DML
SELECT DISTINCT spo.idusocod AS codigo,
                spo.idusocod AS descricao
  FROM altorc.snapshotplanoorcamentario spo
  WHERE EXISTS (SELECT 1
                  FROM altorc.snapshotacao sna
                  WHERE sna.snaexercicio = '%s'
                    AND sna.snaid = spo.snaid)
  ORDER BY idusocod
DML;
                                $complementoSelect = 'class="form-control chosen-select" style="width:100%"';
                                $stmt = sprintf($query, $_SESSION['exercicio']);
                                $db->monta_combo('dados[idusocod]', $stmt, 'S', 'Selecione um IDUSO', null, null, null, null, 'N', 'idusocod', null, '', null, $complementoSelect);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row">
                            <div class="col-md-4">
                                <label class="control-label" for="foncod">Fonte:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $query = <<<DML
SELECT DISTINCT spo.foncod AS codigo,
                spo.foncod AS descricao
  FROM altorc.snapshotplanoorcamentario spo
  WHERE EXISTS (SELECT 1
                  FROM altorc.snapshotacao sna
                  WHERE sna.snaexercicio = '%s'
                    AND sna.snaid = spo.snaid)
UNION SELECT '350', '350'
UNION SELECT '650', '650'
UNION SELECT '680', '680'
UNION SELECT '681', '681'
UNION SELECT '313', '313'
UNION SELECT '105', '105'
/* Mais Fontes  196, 296,696*/
UNION SELECT '196', '196'
UNION SELECT '296', '296'                                        
UNION SELECT '696', '696'
   ORDER BY codigo
DML;
                                $complementoSelect = 'class="form-control chosen-select" style="width:100%"';
                                $stmt = sprintf($query, $_SESSION['exercicio']);
                                $db->monta_combo('dados[foncod]', $stmt, 'S', 'Selecione uma Fonte', 'foncodChanged', null, null, null, 'N', 'foncod', null, '', null, $complementoSelect);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row">
                            <div class="col-md-4">
                                <label class="control-label" for="idoccod">IDOC:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $query = <<<DML
SELECT DISTINCT spo.idoccod AS codigo,
                spo.idoccod AS descricao
  FROM altorc.snapshotplanoorcamentario spo
  WHERE EXISTS (SELECT 1
                  FROM altorc.snapshotacao sna
                  WHERE sna.snaexercicio = '%s'
                    AND sna.snaid = spo.snaid)
  ORDER BY idoccod
DML;
                                $stmt = sprintf($query, $_SESSION['exercicio']);
                                $db->monta_combo('dados[idoccod]', $stmt, 'S', 'Selecione um IDOC', null, null, null, null, 'N', 'idoccod', null, '', null, $complementoSelect);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row">
                            <div class="col-md-4">
                                <label class="control-label" for="rpcod">RP atual:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $query = <<<DML
SELECT DISTINCT spo.rpcod AS codigo,
                spo.rpcod AS descricao
  FROM altorc.snapshotplanoorcamentario spo
  WHERE EXISTS (SELECT 1
                  FROM altorc.snapshotacao sna
                  WHERE sna.snaexercicio = '%s'
                    AND sna.snaid = spo.snaid
                    AND sna.acacod = '%s')
  ORDER BY rpcod
DML;
                                $stmt = sprintf($query, $_SESSION['exercicio'], $locdados['acacod']);
                                $db->monta_combo('dados[rpcod]', $stmt, 'S', 'Selecione um RP', null, null, null, null, 'N', 'rpcod', null, '', null, $complementoSelect);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row">
                            <div class="col-md-4">
                                <label class="control-label" for="rpcod">RP lei:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $rpleidados = array(
                                    array('codigo' => '-', 'descricao' => '- Ação não programada inicialmente na LOA.'),
                                    array('codigo' => '0', 'descricao' => '0 Financeira'),
                                    array('codigo' => '1', 'descricao' => '1 Primária e considerada na apuração do resultado primário para cumprimento da meta, sendo obrigatória quando constar do Anexo V'),
                                    array('codigo' => '2', 'descricao' => '2 Primária e considerada na apuração do resultado primário para cumprimento da meta, sendo discricionária e não abrangida pelo PAC'),
                                    array('codigo' => '3', 'descricao' => '3 Primária e considerada na apuração do resultado primário para cumprimento da meta, sendo discricionária e abrangida pelo PAC'),
                                    array('codigo' => '4', 'descricao' => '4 Primária, constante do Orçamento de Investimento, e não considerada na apuração do resultado primário para cumprimento da meta, sendo discricionária e não abrangida pelo PAC'),
                                    array('codigo' => '5', 'descricao' => '5 Primária, constante do Orçamento de Investimento, e não considerada na apuração do resultado primário para cumprimento da meta, sendo discricionária e abrangida pelo PAC'),
                                    array('codigo' => '6', 'descricao' => '6 Discricionária e decorrente de despesas individuais.'),
                                    array('codigo' => 'X', 'descricao' => 'X'),
                                );
                                $db->monta_combo('dados[rpleicod]', $rpleidados, 'S', 'Selecione um RP Lei', null, null, null, null, 'N', 'rpleicod', null, '', null, $complementoSelect);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row fonte-recurso" id="frec_1">
                            <div class="col-md-4">
                                <label class="control-label" for="valsuplementacao">Sup. por Cancelamento:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $onBlur = "resetValor(this)";
                                $complemento = <<<HTML
id="valsuplementacao" class="form-control" data-default-value=""
HTML;
                                echo campo_texto('dados[valsuplementacao]', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row fonte-recurso" id="frec_3">
                            <div class="col-md-4">
                                <label class="control-label" for="valsuplementacao">Sup. por Superávit:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $onBlur = "resetValor(this)";
                                $complemento = <<<HTML
id="valsuperavit" class="form-control" data-default-value=""
HTML;
                                echo campo_texto('dados[valsuperavit]', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row fonte-recurso" id="frec_2">
                            <div class="col-md-4">
                                <label class="control-label" for="valsuplementacao">Sup. por Excesso:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $onBlur = "resetValor(this)";
                                $complemento = <<<HTML
id="valexcesso" class="form-control" data-default-value=""
HTML;
                                echo campo_texto('dados[valexcesso]', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row fonte-recurso" id="frec_4">
                            <div class="col-md-4">
                                <label class="control-label" for="valopcredito">Sup. por Operação Crédito:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $onBlur = "resetValor(this)";
                                $complemento = <<<HTML
id="valopcredito" class="form-control" data-default-value=""
HTML;
                                echo campo_texto('dados[valopcredito]', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                                ?>
                            </div>
                        </div>
                        <div class="form-group row" id="frcancelamento">
                            <div class="col-md-4">
                                <label class="control-label" for="valcancelamento">Cancelamento:</label>
                            </div>
                            <div class="col-md-8">
                                <?php
                                $complemento = <<<HTML
id="valcancelamento" class="form-control" data-default-value=""
HTML;
                                echo campo_texto('dados[valcancelamento]', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                                ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal">Cancelar</button>
                <?php if ($podeSalvar) { ?>
                    <button type="button" class="btn btn-success" id="salvar-alteracao-po"></button>
                <?php
                } else {
                    echo ' <span class="glyphicon glyphicon-exclamation-sign" style="color:red;"></span>  Desabilitado para Editar.';
                }
                ?>
            </div>
        </div>
    </div>
</div>