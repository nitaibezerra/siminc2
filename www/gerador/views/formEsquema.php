<?php $dml = <<<DML
    SELECT DISTINCT schemaname AS schema
    FROM pg_catalog.pg_tables
    WHERE schemaname NOT IN ('pg_catalog', 'information_schema', 'pg_toast')
    ORDER BY schemaname
DML;
$data = $db->carregar($dml);
?>

<div class="container">
    <div class="row vertical-offset-100">
        <div class="col-lg-4 col-lg-offset-4">
            <div class="panel panel-default">
                <div class="panel-heading">
                    <h3>Selecione o Esquema</h3>
                </div>
                <div class="panel-body">
                    <form method="get">
                        <div class="form-group">
                            <label for="schema">Esquema</label>
                            <select name="schema" id="schema" class="form-control" data-placeholder="selecione">
                                <?php foreach ($data as $schema): ?>
                                    <option><?= $schema['schema'] ?></option>
                                <?php endforeach; ?>
                            </select>
                        </div>
                        <button class="btn btn-lg btn-success btn-block" type="submit">avançar »</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
