<div class="row">
    <div class="col-lg-4 col-lg-offset-4">
        <div class="panel panel-default">
            <div class="panel-heading">
                <h3>Selecione uma Tabela</h3>

                <p>Esquema Selecionado: <i><?= $schema ?></i></p>
            </div>
            <div class="panel-body">
                <form method="get">
                    <input type="hidden" name="schema" value="<?= $_GET['schema'] ?>"/>

                    <div class="form-group">
                        <label for="tables">Tabelas</label>
                        <select class="form-control" name="tables[]" id="sel_tables" data-placeholder="selecione" multiple>
                            <option value="todas"> Todas</option>
                            <?php foreach ($tabelas as $table): ?>
                                <option> <?= $table['table'] ?></option>
                            <?php endforeach; ?>
                        </select>
                    </div>

                    <button class="btn btn-lg btn-success btn-block" type="submit">avançar »</button>
                </form>
            </div>
        </div>
    </div>
</div>
