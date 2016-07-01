<br>
<div class="well notprint">
    <form class="form-horizontal" id="form_pesquisar_solucao">
        <fieldset>
            <legend>Pesquisar</legend>

            <div class="form-group">
                <label class="col-lg-2 control-label" for="soldsc">PTO</label>

                <div class="col-lg-10">
                    <input type="text" placeholder="Número, Projeto, Apelido, Etapa, Atividade" class="form-control" name="diverso" id="diverso">
                </div>
            </div>

			<div class="form-group">
				<label class="col-lg-2 control-label" for="solprazo"><?= $this->solucao->getAttributeLabel('solprazo'); ?></label>

				<div class="col-lg-10">
					<input type="text" class="form-control" name="solprazo" id="solprazo_pesquisa">
				</div>
			</div>

            <div class="form-group">
                <label class="col-lg-2 control-label" for="acaid_pesquisa_solucao">Ações Estratégicas</label>

                <div class="col-lg-10">
                    <select class="form-control chosen" name="acaid[]" id="acaid_pesquisa" multiple="multiple">
                        <?= $this->acaoSolucao->getOptionsAcao(); ?>
                    </select>
                </div>
            </div>

            <div class="form-group">
                <label class="col-lg-2 control-label" for="temid_pesquisa_solucao"><?= $this->tema->getAttributeLabel('temdsc'); ?></label>

                <div class="col-lg-10">
                    <select class="form-control chosen" name="temid[]" id="temid_pesquisa" multiple="multiple">
                        <?= $this->tema->getOptionsTema(); ?>
                    </select>
                </div>
            </div>


            <div class="form-group">
                <label class="col-lg-2 control-label" for="temid_pesquisa_solucao">Dispositivos PNE</label>

                <div class="col-lg-10">
                    <select class="form-control chosen" name="mpneid[]" id="mpneid_meta" multiple="multiple">
                        <?= $this->metaSolucao->getOptionsMeta(); ?>
                    </select>
                </div>
            </div>


            <div class="text-right">
                <button type="button" class="btn btn-success" title="Pequisar" id="btn_pesquisar">
                    <span class="glyphicon glyphicon-search"></span> Pequisar
                </button>
                <button type="button" class="btn btn-default" title="Limpar" id="btn_limpar_pesquisa">
                    <span class="glyphicon glyphicon-repeat"></span> limpar
                </button>
				<button type="button" id="bt_imprimir_lista123" class="btn btn-primary btn-sm">
					<span class="glyphicon glyphicon-print" aria-hidden="true"></span> imprimir
				</button>
            </div>
        </fieldset>
    </form>
</div>
<script type="text/javascript">
    $(function () {
    	$('#solprazo_pesquisa').datepicker();
        $("#solprazo_pesquisa").mask("99/99/9999");

        $('#btn_pesquisar').on('click', function (event) {
            $.post(window.location.href, {'controller': 'default', 'action': 'listar', 'parans': $('#form_pesquisar_solucao').serialize() }, function (html) {
                $('#div_listar').html(html);
            });
        });

        $('#btn_limpar_pesquisa').on('click', function (event) {
            location.href = '/pto/pto.php?modulo=inicio&acao=C';
        });

        $(".chosen").chosen({
            no_results_text: 'Nenhum item encontrado!',
            placeholder_text_multiple: 'Selecione algum item',
            placeholder_text_single: 'Selecione um item'
        });

		$('#bt_imprimir_lista123').on('click', function () {
			window.print();
		});
    })
</script>