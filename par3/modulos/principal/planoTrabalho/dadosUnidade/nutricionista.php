<?php
global $simec;

$renderDirigente = new Par3_Controller_Entidade();
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();



switch ($_REQUEST['requisicao']) {
    case 'cadastrar_nutricionista_responsavel':

        $controllerInstrumentoUnidadeEntidade->salvarInformacoesNutricionistaReponsavel($_POST);
        break;
    case 'verifica_cpf':
        ob_clean();
        $duncpf = str_replace("/", "", str_replace("-", "", str_replace(".", "", $_POST['cpf'])));
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $dados = $instrumentoUnidadeEntidade->recuperarNutricionistaResponsavelPorCpf($duncpf);

        if (!empty($dados)) {
            $dados2 = simec_json_encode($dados);
            echo $dados2;
            //Retornar restrição de acesso
        } else {
            $usuario = new Seguranca_Model_Usuario();
            $dados = $usuario->recuperarPorCPF($duncpf);
            if (!empty($dados)) {
                $dados = array(array('usunome' => $dados["usunome"], 'entemail' => $dados["usuemail"], 'origem' => "seguranca"));
                echo simec_json_encode($dados);

            } else {
                //webservice_receita
                $resp = recuperarUsuarioReceita($duncpf);
                $dados = array(array('usunome' => $resp['dados']['no_pessoa_rf'], 'entemail' => "", 'origem' => "receita"));
                echo simec_json_encode($dados);
            }
        }
        die();
        break;
    default:

        break;

}





?>



<script>

    $('.ibox').on('change', 'input[name=usucpf1]', function () {
        if (!validar_cpf($(this).val())) {
            alert("CPF inválido!\nFavor informar um cpf válido!");
            $(this).val('');
            $(this).parent().parent().find('input[name=entnome1]').val('');
            $(this).parent().parent().find("label").html('');
            return false;
        }
        var param = new Array();
        param.push({name: 'requisicao', value: 'verifica_cpf'},
            {name: 'cpf', value: $(this).val()});

        var t = $(this);

        $.ajax({
            type: "POST",
            dataType: "json",
            url: window.location.href,
            data: param,
            success: function (data) {

                if (data[0].origem == "instrumentounidade_entidade") {
                    var locais = [];
                    for (i = 0; data.length > i; i++) {
                        locais.push(data[i].inudescricao);
                    }
                    var lista_locais = locais.join(',');

                    swal({
                            title: "Você tem certeza?",
                            text: "O nutricionista <b>(" + data[0].usunome + ")</b> selecionado está vinculado aos município(s): <b>" + lista_locais + "</b>. Deseja fazer o cadastro dele em um novo município!",
                            type: "warning", showCancelButton: true,
                            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                            closeOnConfirm: "on",
                            cancelButtonText: "Cancelar",
                            html: true
                        },
                        function (isConfirm) {
                            if (isConfirm) {
                                t.closest(".ibox-content").find('input[name=entnome1]').val(data[0].usunome);
                                t.closest(".ibox-content").find('input[name=entemail1]').val(data[0].entemail);
                            }
                            else {
                                t.closest(".ibox-content").find('input[name=usucpf1]').val("");
                                t.closest(".ibox-content").find('input[name=entemail1]').val("");
                                t.closest(".ibox-content").find('input[name=entnome1]').val("");
                            }
                        });

                }
                if (data[0].origem == "receita") {
                    t.closest(".ibox-content").find('input[name=entnome1]').val(data[0].usunome);
                    //t.closest(".ibox-content").find('input[name=entemail1]').val(data[0].entemail);
                }
                if (data[0].origem == "seguranca") {
                    t.closest(".ibox-content").find('input[name=entnome1]').val(data[0].usunome);
                    t.closest(".ibox-content").find('input[name=entemail1]').val(data[0].entemail);
                }
            }
        });

    })
    ;
</script>

<div class="ibox float-e-margins">
    <div class="ibox-title esconde" tipo="integrantes">
        <h3>Responsável Técnico / Nutricionista</h3>
    </div>
    <form method="post" name="formulario" id="formulario" class="form form-horizontal">
        <div class="ibox-content">
            <?php

            //ver($_SESSION,d);

            $idNutri = $modelInstrumentoUnidadeEntidade->pegarEntidAtivoPorTipo($_REQUEST['inuid'],7);
            if(!empty($idNutri)){
            $modelInstrumentoUnidadeEntidade->carregarPorId($idNutri);
            $nutrucionistaResponsavel = $modelInstrumentoUnidadeEntidade->getDados();
            }

            ?>
            <input type="hidden" name="requisicao" value="cadastrar_nutricionista_responsavel">
            <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']?>">
            <input type="hidden" name="entid1" value="<?php echo  $nutrucionistaResponsavel['entid']?>">

            <?php
            echo $simec->cpf('usucpf1', 'CPF', $nutrucionistaResponsavel['entcpf'], array($disabled, 'data-pessoa' => true), array('label-size' => 3, 'input-size' => 9));
            echo $simec->input('entnome1', 'Nome', $nutrucionistaResponsavel['entnome'], array('maxlength' => '255', true, 'readonly' => 'readonly'), array('label-size' => 3, 'input-size' => 9));
            echo $simec->email('entemail1', 'E-mail', $nutrucionistaResponsavel['entemail'], array('class' => 'email'), array('label-size' => 3, 'input-size' => 9));
            ?>


        </div>
        <div class="ibox-footer">
            <button type="submit" class="btn btn-success novo pull-right">
                <i class="fa fa-plus-square-o"></i>
                Salvar
            </button>
            <div class="clearfix"></div>
        </div>
    </form>

</div>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3 class="pull-left;" style="margin-bottom: 10px">Quadro Técnico/Nutricionistas</h3>
            </div>
            <div class="col-md-6">
                <button class="btn btn-success novo pull-right" data-toggle="modal" data-target="#modal">
                    <i class="fa fa-plus-square-o"></i>
                    Inserir Nutricionista
                </button>
            </div>
        </div>
    </div>

        <div class="ibox-content">
            <?php $controllerInstrumentoUnidadeEntidade->recuperarNutricionistasQuadroTecnico(); ?>
        </div>

</div>


<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content animated flipInY">
            <div class="ibox-title esconde " tipo="integrantes">
                <h3>Quadro Técnico/Nutricionistas</h3>
            </div>
            <div class="ibox-content">
                <form method="post" name="formulario" id="formulario" class="form form-horizontal">

                    <?php
                    echo $simec->input('usucpf2', 'CPF', "", array('class' => 'cpf', true, 'data-pessoa' => true, 'data-pessoa-campos' => '{"entnome": "no_pessoa_rf"}'), array('label-size' => 3, 'input-size' => 9));
                    echo $simec->input('entnome2', 'Nome', "", array('maxlength' => '255', true, 'readonly' => 'readonly'), array('label-size' => 3, 'input-size' => 9));
                    echo $simec->email('entemail2', 'E-mail', "", array('class' => 'email'), array('label-size' => 3, 'input-size' => 9));
                    ?>
                </form>
            </div>

            <div class="ibox-footer">
                <button type="button" class="btn btn-success novo" <?php echo $disabled; ?>><i
                        class="fa fa-plus-square-o"></i>
                    Inserir
                </button>
            </div>
        </div>
    </div>
</div>