<?php
    if($_REQUEST['indid'] && $_REQUEST['remover']){
        $key = array_search($_REQUEST['indid'], $_SESSION['cruzamento']['indicadores']);
        if($key !== false){
            unset($_SESSION['cruzamento']['indicadores'][$key]);
        }
    } else {
        $_SESSION['cruzamento']['indicadores'][] = $_REQUEST['indid'];
    }

    $_SESSION['cruzamento']['indicadores'] = array_unique($_SESSION['cruzamento']['indicadores']);

    if(!count($_SESSION['cruzamento']['indicadores'])){
        echo '<script>window.location.href=window.location.href;</script>';
        die;
    }

?>

<style>

    #div-form-cruzamento .chosen-choices{
        padding: 2px !important;
        min-height: 0;
    }

    #div-form-cruzamento #tabela_indicadores_cruzamento td{
        font-size: 13px !important;
        vertical-align: middle;
    }

    #div-form-cruzamento #tabela_indicadores_cruzamento .form-group{
        margin-top: 2px;
        margin-bottom: 2px;
    }

    .chosen-drop{
        z-index: 99 !important;
    }
</style>

<div class="row" id="div-form-cruzamento">
    <div class="col-sm-12">
        <form action="index.php?carregarRegionalizacao=1" id="form_cruzamento" name="form_cruzamento" class="form-horizontal">

            <?php
            $indid1 = current($_SESSION['cruzamento']['indicadores']);
            $sql = "select indid, indnome
            from painel.indicador
            where indid = {$indid1}
            ";
            $arrDados = $db->pegaLinha($sql);
            ?>
    
            <div class="alert alert-success">
                Indicador Principal: <i style="font-weight: bold;"><?php echo $arrDados['indnome']; ?></i>
            </div>

            <div class="row">
                <div class="col-md-12">
                    <h1 style="color:#414145; text-align: left;">Filtros do Indicador</h1>
                    <hr class="linha_titulo">
                </div>
                <div class="col-md-12 text-center" style="margin-bottom: 10px; text-align: center;" id="div_uf">
                    <?php montarUfs(); ?>
                </div>
            </div>

            <div class="row" style="font-size: 12px;">
                <div class="col-lg-6">
                    <div class="row">
                        <div class="form-group">
                            <?php
                            $sql = "select regcod, regdescricao as descricao
                                    from territorios.regiao
                                    order by descricao";
                            $regioes = $db->carregar($sql);
                            $regioes = $regioes ? $regioes : array();
                            ?>
                            <label for="regcod" class="col-sm-3 control-label">Região:</label>
                            <div class="col-sm-9">
                                <select name="regcod[]" id="regcod" class="form-control chosen" multiple data-placeholder="Selecione">
                                    <?php foreach ($regioes as $dado) { ?>
                                        <option <?php echo is_array($_POST['regcod']) && in_array($dado['regcod'], $_POST['regcod']) ? 'selected="selected"' : ''; ?> value="<?php echo trim($dado['regcod']); ?>"><?php echo $dado['descricao']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <?php
                            $ufs = isset($_POST['estuf']) && is_array($_POST['estuf']) ? " '" . implode ("', '", $_POST['estuf']) . "'" : "''";

                            $sql = "select muncod, estuf || ' - ' || mundescricao as descricao
                                        from territorios.municipio
                                        where estuf in ({$ufs})
                                        order by estuf, mundescricao";
                            $municipios = $db->carregar($sql);
                            $municipios = $municipios ? $municipios : array();
                            ?>
                            <label for="nome" class="col-sm-3 control-label">Município:</label>
                            <div class="col-sm-9"  id="div_municipio">
                                <select name="muncod[]" id="muncod" class="form-control chosen" multiple data-placeholder="Selecione">
                                    <?php foreach ($municipios as $dado) { ?>
                                        <option <?php echo is_array($_POST['muncod']) && in_array($dado['muncod'], $_POST['muncod']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['muncod']; ?>"><?php echo $dado['descricao']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-lg-6">
                    <div class="row">
                        <div class="form-group">
                            <?php
                            $ufs = isset($_POST['estuf']) && is_array($_POST['estuf']) ? " '" . implode ("', '", $_POST['estuf']) . "'" : "''";

                            $sql = "select gtmid, gtmdsc as descricao
                                    from territorios.grupotipomunicipio
                                    where gtmstatus = 'A'
                                    order by descricao";
                            $grupos = $db->carregar($sql);
                            $grupos = $grupos ? $grupos : array();
                            ?>
                            <label for="gtmid" class="col-sm-3 control-label">Grupos Município:</label>
                            <div class="col-sm-9">
                                <select name="gtmid[]" id="gtmid" class="form-control chosen" multiple data-placeholder="Selecione">
                                    <?php foreach ($grupos as $dado) { ?>
                                        <option <?php echo is_array($_POST['gtmid']) && in_array($dado['gtmid'], $_POST['gtmid']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['gtmid']; ?>"><?php echo $dado['descricao']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="tpmid" class="col-sm-3 control-label">Tipos Município:</label>
                            <div class="col-sm-9"  id="div_tipo_municipio">
                                <select name="tpmid[]" id="tpmid" class="form-control chosen" multiple data-placeholder="Selecione">
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                    <table class="table table-hover table-condensed" id="tabela_indicadores_cruzamento">
                        <?php
                        $case = ' case ';
                        foreach ($_SESSION['cruzamento']['indicadores'] as $ordem => $indid) {
                            $case .= " when indid = $indid then $ordem";
                        }
                        $case .= ' end as ordem ';

                        $sql = "select indid, indnome, $case
                                from painel.indicador
                                where indid in (" . implode(', ', $_SESSION['cruzamento']['indicadores']) . ")
                                order by ordem";
                        $arrDados = $db->carregar($sql);

                        $qtd = count($arrDados);
                        foreach ($arrDados as $count => $dados) {
                            $sql = "select t.tdiid, t.tdiordem, t.tdidsc, t.tdistatus, d.tidid, d.tiddsc, d.tidstatus
                                    from painel.indicador i
                                         INNER JOIN painel.detalhetipoindicador t ON t.indid = i.indid
                                         LEFT JOIN painel.detalhetipodadosindicador d on d.tdiid = t.tdiid
                                    where i.indid = {$dados['indid']}
                                    order by t.tdiordem, d.tiddsc";
                            $detalhes = $db->carregar($sql);
                            $detalhes = $detalhes ? $detalhes : array();

                            $detalhesAgrupados = array();
                            foreach ($detalhes as $detalhe) {
                                $detalhesAgrupados[$detalhe['tdidsc']][] = $detalhe;
                            }
                            ?>
                            <tr>
                                <td style="width: 3% !important;">
                                    <span style="color: #f00 !important; cursor: pointer;" class="glyphicon glyphicon-remove remover-indicador" aria-hidden="true" indid="<?php echo $dados['indid'];?>"></span>
                                </td>
                                <td style="width: 40% !important;" colspan="<?php echo count($detalhesAgrupados) ? 1 : 3; ?>">
                                    <input type="hidden" name="indicadores[]" value="<?php echo $dados['indid'];?>"/>
                                    <?php echo $dados['indid'] . ' - ' . $dados['indnome'];?>
                                </td>
                                <?php

                                foreach ($detalhesAgrupados as $tdidsc => $detalhes) {
                                    $ordem = $detalhes[0]['tdiordem'];
                                    ?>
                                    <td style="width: <?php echo count($detalhesAgrupados) == 1 ? 50 : 25; ?>% !important;" colspan="<?php echo count($detalhesAgrupados) == 1 ? 2 : 1; ?>">
                                        <div class="form-group">
                                            <div class="col-sm-12">
                                                <select name="tidid<?php echo $ordem ?>[<?php echo $dados['indid']; ?>][]" class="form-control chosen" multiple data-placeholder="<?php echo $tdidsc; ?>">
                                                    <?php foreach ($detalhes as $detalhe) { ?>
                                                        <option <?php echo is_array($_POST['regcod']) && in_array($detalhe['regcod'], $_POST['regcod']) ? 'selected="selected"' : ''; ?> value="<?php echo trim($detalhe['tidid']); ?>"><?php echo $detalhe['tiddsc']; ?></option>
                                                    <?php } ?>
                                                </select>
                                            </div>
                                        </div>
                                    </td>
                                <?php } ?>

                            </tr>
                        <?php } ?>
                    </table>

                <style>

                    .btn-radio{
                        padding: 4px 8px !important;
                    }

                    label.btn-todos.active{
                        background-color: #EAE38E !important;
                        border-color: #ccc !important;
                        color: black;
                    }

                    label.btn-sim.active{
                        background-color: #97FF97 !important;
                        border-color: #ccc !important;
                        color: black;
                    }

                    label.btn-nao.active{
                        background-color: #EF9B98 !important;
                        border-color: #EF9B98 !important;
                        color: black;
                    }
                </style>

                <?php
                $aModalidades = array(
                    'Ensino Regular' => array(
                        'Creche' => 'id_reg_infantil_creche',
                        'Pré-Escola' => 'id_reg_infantil_preescola',
                        'Fundamental 8 anos' => 'id_reg_fund_8_anos',
                        'Fundamental 9 anos' => 'id_reg_fund_9_anos',
                        'Médio' => 'id_reg_medio_medio',
                        'Médio Integrado' => 'id_reg_medio_integrado',
                        'Médio Normal' => 'id_reg_medio_normal',
                        'Médio Profissionalizante' => 'id_reg_medio_prof',
                    ),
                    'Ensino Especial' => array(
                        'Creche' => 'id_esp_infantil_creche',
                        'Pré-Escola' => 'id_esp_infantil_preescola',
                        'Fundamental 8 anos' => 'id_esp_fund_8_anos',
                        'Fundamental 9 anos' => 'id_esp_fund_9_anos',
                        'Médio' => 'id_esp_medio_medio',
                        'Médio Integrado' => 'id_esp_medio_integrado',
                        'Médio Normal' => 'id_esp_medio_normal',
                        'Médio Profissionalizante' => 'id_esp_medio_profissional',
                        'EJA Fundamental' => 'id_esp_eja_fundamental',
                        'EJA Médio' => 'id_esp_medio_profissional',
                    ),
                    'EJA - Educação de Jovens e Adultos' => array(
                        'EJA Fundamental' => 'id_eja_fundamental',
                        'EJA Médio' => 'id_eja_medio',
                        'EJA Fundamental Projovem' => 'id_eja_fundamental_projovem',
                    ),
                );
                ?>
                <div class="col-md-12"">
                    <h1 style="color:#414145">Filtros da Escola</h1>
                    <hr class="linha_titulo">

                <div class="text-right" style="margin-bottom: 5px;">
                    <span style="font-weight: bold;">Legenda:</span>
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default btn-todos btn-radio" title="Todas">
                            <input type="radio" class="checkbox-mod" >
                            <span class="glyphicon glyphicon-asterisk"></span> Selecionar Todas
                        </label>
                        <label class="btn btn-default btn-sim btn-radio" title="Possui">
                            <input type="radio" class="checkbox-mod" >
                            <span class="glyphicon glyphicon-thumbs-up"></span> Selecionar as que possuem
                        </label>
                        <label class="btn btn-default btn-nao btn-radio" title="Não Possui">
                            <input type="radio" class="checkbox-mod" >
                            <span class="glyphicon glyphicon-thumbs-down"></span> Selecionar as que NÃO possuem
                        </label>
                    </div>
                </div>

                    <?php foreach ($aModalidades as $modalidade => $niveis) { ?>
                        <table class="table table-bordered table-condensed text-center">
                            <thead>
                                <tr style="background-color: #ebe4f1;">
                                    <th class="text-center" colspan="<?php echo count($niveis); ?>"><?php echo $modalidade; ?></th>
                                </tr>
                                <tr style="background-color: #fdfaff;">
                                    <?php foreach ($niveis as $descricao => $campo) { ?>
                                        <th class="text-center"><?php echo $descricao; ?></th>
                                    <?php } ?>
                                </tr>
                            </thead>
                            <tr>
                                <?php foreach ($niveis as $descricao => $campo) { ?>
                                    <td>
                                        <div class="btn-group" data-toggle="buttons">
                                            <label class="btn btn-default btn-todos btn-radio" title="Todas">
                                                <input type="radio" class="checkbox-mod" name="<?php echo $campo; ?>" id="<?php echo $campo; ?>"  autocomplete="off" value="T">
                                                <span class="glyphicon glyphicon-asterisk"></span>
                                            </label>
                                            <label class="btn btn-default btn-sim btn-radio" title="Possui">
                                                <input type="radio" class="checkbox-mod" name="<?php echo $campo; ?>" id="<?php echo $campo; ?>" autocomplete="off" value="1">
                                                <span class="glyphicon glyphicon-thumbs-up"></span>
                                            </label>
                                            <label class="btn btn-default btn-nao btn-radio" title="Não Possui">
                                                <input type="radio" class="checkbox-mod" name="<?php echo $campo; ?>" id="<?php echo $campo; ?>" autocomplete="off" value="0">
                                                <span class="glyphicon glyphicon-thumbs-down"></span>
                                            </label>
                                        </div>
                                    </td>
                                <?php } ?>
                            </tr>
                        </table>
                    <?php } ?>
                </div>
            </div>

            <div class="row" style="padding-top:15px;">
            	<button type="button" class="btn btn-success" id="btn-pesquisar" ><i class="fa fa-search"></i> Pesquisar</button>
            	<button type="button" class="btn btn-danger" id="limpar-cruzamento"><i class="fa fa-minus-circle"></i> Limpar Dados</button>
            </div>
        </form>
    </div>
</div>

<div class="row" id="div-resultado-cruzamento">

</div>

<script type="text/javascript">
    $(function(){
        $('.chosen').chosen();

        $('#regcod').on('change', function(e) {
            $('#div_uf').load('?carregarUf=1&'+$('#regcod').serialize(), function(){
                $('.checkbox-uf').change();
            });
        });

        $('#gtmid').on('change', function(e) {
            $('#div_tipo_municipio').load('?carregarTipoMunicipio=1&'+$('#gtmid').serialize(), function(){
                $('#tpmid').chosen();
                carregarMunicipio();
            });
        });

        $('body').on('change', '.checkbox-uf, #tpmid', function() {
            carregarMunicipio();
        });

        $('.remover-indicador').click(function(){
            if(confirm('Deseja realmente remover este item da listagem?')){
                var indid = $(this).attr('indid');
                $('#div_cruzamento').load('index.php?carregarCruzamento=1&remover=1&indid=' + $(this).attr('indid'));
            }
        });

        function carregarMunicipio()
        {
            $('#div_municipio').load('?carregarMunicipio=1&'+$('#form_cruzamento').serialize(), function() {
                $('#muncod').chosen();
            });
        }
    });
</script>