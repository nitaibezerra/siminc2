<script>
    $(document).ready(function () {
        $('#executarSemCoordenadores').on('click', function () {
            if ($('#periodo').val() == '' || $('#periodo').val() == null || $('#periodo').val() == undefined) {
                bootbox.alert('Você deve selecionar um período.');
                $('#periodo').parent().parent().addClass('has-error');
                return false;
            }
            bootbox.confirm('Você realmente deseja apagar os dados do snapshot para o perído ' + $('#periodo option[value=' + $('#periodo').val() + ']').text() + ' e inserir novamente?', function (re) {
                if (re) {
                    $('#formAcompanhamentoSubacao').submit();
                }
            });
        });

        $('#executarComCoordenadores').on('click', function () {
            if ($('#periodo').val() == '' || $('#periodo').val() == null || $('#periodo').val() == undefined) {
                bootbox.alert('Você deve selecionar um período.');
                $('#periodo').parent().parent().addClass('has-error');
                return false;
            }
            bootbox.confirm('Você realmente deseja apagar os dados do snapshot para o perído ' + $('#periodo option[value=' + $('#periodo').val() + ']').text() + ' e inserir novamente?', function (re) {
                if (re) {
                    bootbox.confirm('TODOS os COORDENADORES DE SUBAÇÕES serão APAGADOS, tem CERTEZA??? ', function (re2) {
                        if (re2) {
                            $('#comCoordenadores').val('true');
                            $('#formAcompanhamentoSubacao').submit();
                        }
                    })
                }
            });
        });

        $('#voltar').on('click', function () {
            window.location = $(this).attr('data-url');
        });
        $('#comCoordenadores').hide();
    });
</script>
<?php
$form = new Simec_View_Form('formAcompanhamentoSubacao', Simec_View_Form::POST);
$form->setRequisicao('cargaSubacao');
$valor = <<<HTML
    <div> Ao clicar em executar você estará <b>APAGANDO</b> os dados do snapshot para o período selecionado. <br/><br/>
    </div>
HTML;
$form->addInputTextarea('html', $valor, 'html', '', array('somentetexto' => true));
$form->addInputTextarea('comCoordenadores', '', 'comCoordenadores', '', array(''));

$query = <<<DML
    SELECT prfid AS codigo, prftitulo AS descricao FROM acomporc.periodoreferencia WHERE prftipo = 'S' AND prsano = '{$_SESSION['exercicio']}'
DML;
$form->addInputCombo('prfid', $query, $valor, 'periodo', array('flabel' => 'Período'));
$form->addBotao('novo', array('id' => 'executarSemCoordenadores', 'label' => 'Executar somente Subações'));
$form->addBotao('avancado', array('id' => 'executarComCoordenadores', 'label' => 'Executar com Coordenadores', 'class' => 'btn-danger', 'extra' => 'data-url="spo.php?modulo=inicio&acao=C"', 'span' => 'glyphicon glyphicon-fire'));
$form->render();
