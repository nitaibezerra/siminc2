<div class="container">
<?php echo montarAbasArray( $_SESSION['demandasse']['abas_array'] , $_SESSION['demandasse']['url'], ''); ?>

<!--    <div class="col-lg-12">-->
<!--        <div class="page-header">-->
<!--            <h1 id="forms">-->
<!--                Procedência-->
                <!--            <small>-->
                <!--                Lista de Procedência-->
                <!--            </small>-->
<!--            </h1>-->
<!--        </div>-->
<!--    </div>-->

    <form id="formulario_pesquisar" method="post" class="form-horizontal">
        <input name="controller" value="procedencia" type="hidden" />
        <input name="action" value="listar" type="hidden" />
        <div class="col-md-12">
            <div class="well">
                <fieldset>
                    <legend>Pesquisa</legend>
                    <div class="col-md-1"></div>
                    <div class="col-md-10">
                        <div class="form-group">
                            <label for="prcsigla" class="col-lg-2 col-md-2 control-label">Sigla</label>
                            <div class="col-lg-10 col-md-10 ">
                                <input id="prcsigla" name="prcsigla" type="text" class="form-control" placeholder="" value="">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="prcdsc" class="col-lg-2 col-md-2  control-label">Descrição</label>

                            <div class="col-lg-10 col-md-10 ">
                                <input id="prcdsc" name="prcdsc" type="text" class="form-control" placeholder="" value="">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="prcresponsavel" class="col-lg-2 col-md-2  control-label">Responsável</label>

                            <div class="col-lg-10 col-md-10  ">
                                <input id="prcresponsavel" name="prcresponsavel" type="text" class="form-control" placeholder="" value="">
                            </div>
                        </div>
                        <div class="text-right">
                            <button onclick="javascript:window.location.reload();" id="bt_limpar"  title="Limpar pesquisa" class="btn btn-warning" type="button" ><span
                                    class="glyphicon glyphicon-remove"></span> Limpar
                            </button>
                            <button id="bt_pesquisar"  title="Pesquisar" class="btn btn-primary" type="button" ><span
                                    class="glyphicon glyphicon-search"></span> Pesquisar
                            </button>
                        </div>
<!--                    </div>-->
                    <div class="col-md-1"></div>
                </fieldset>
            </div>
        </div>

        <br>
    </form>

    <div class="col-lg-12">
        <div class="page-header">
            <h1 id="forms">
                <!--                Dados da universidade --->
                <small>
                    Listagem
                </small>
            </h1>
        </div>
    </div>
    <div  class="col-lg-12">
        <button class="bt-inserir btn btn-success" > <span class="glyphicon glyphicon-plus"></span> Inserir</button>
    </div>
        <br />
    <div class="row">
        <div class="col-md-12" id="container_listar">
            <?php $this->listarAction(); ?>
        </div>
    </div>
</div>
<script language="JavaScript">

    /**
     * Exibe a listagem de acordo com os campos para pesquisa.
     */
    $('#bt_pesquisar').click(function () { $('#formulario_pesquisar').ajaxSubmit({target: $('#container_listar').hide().fadeIn()}); });

    /**
     * Exibe uma modal para realizar cadastro.
     */
    $('.bt-inserir').click(function() {
        $('#container_formulario').fadeIn();
        $.post(window.location.href, {'controller': 'procedencia', 'action': 'formulario'}, function(html) {
            $('#modal').html(html).modal('show');
        });
    });


    function fecharModal() {
        $('#modal').modal('hide');
        var data = {controller: 'procedencia', action: 'listar'};
        $.post(window.location.href, data, function (html) {
            $('#container_listar').hide().fadeIn().html(html);
        });
    }


</script>