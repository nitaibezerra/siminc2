<?php


include "config.inc";
include_once APPRAIZ . "educriativa/autoload.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
$db = new cls_banco();
$configuracao = new Educriativa_Model_Configuracao();

$sql = "select to_char(dataexpiracao, 'YYYY-MM-DD') data_termino,
               to_char(dataexpiracao, 'YYYYMMDDHH24MISS') data_expiracao,
               to_char(dataexpiracao, 'DD/MM/YYYY \à\s HH24:MI:SS') as data_formatada
          from criatividadeeducacao.configuracao";

$dados = $configuracao->pegaLinha($sql);

$dataTermino = $dados['data_termino'];
$dataExpiracao = $dados['data_expiracao'];
$dataFormatada = $dados['data_formatada'];
$dias = floor((strtotime($dataTermino) - strtotime(date('Y-m-d'))) / ((60*60*24)+1));

$regioes = array(
    1 =>
    array(
        'titulo' => 'São Paulo',
        'valor' => array(
            'SP'
        )
    ),
    2 =>
    array(
        'titulo' => 'Norte',
        'valor' => array(
            'PA',
            'AM',
            'RO',
            'AP'
        )
    ),
    3 =>
    array(
        'titulo' => 'Nordeste1',
        'valor' => array(
            'CE',
            'RN',
            'PI',
            'MA'
        )
    ),
    4 =>
    array(
        'titulo' => 'Nordeste2',
        'valor' => array(
            'BA',
            'AL',
            'SE'
        )
    ),
    5 =>
    array(
        'titulo' => 'Centro-Oeste',
        'valor' => array(
            'DF',
            'MT'
        )
    ),
    6 =>
    array(
        'titulo' => 'Sul',
        'valor' => array(
            'PR',
            'RS',
            'SC'
        )
    ),
    7 =>
    array(
        'titulo' => 'RJ-ES',
        'valor' => array(
            'RJ',
            'ES'
        )
    ),
    8 =>
    array(
        'titulo' => 'Minas',
        'valor' => array(
            'MG'
        )
    )
);
$regWhere = '';

if($_POST['acao']){
    switch($_POST['acao']){
        case 'carrega_estado':
            ob_clean();
            header ('Content-type: text/html; charset=iso-8859-1');
            foreach ($regioes as $k => $v) {
                if(is_array($_POST['regiao']) && in_array($k, $_POST['regiao'])){
                    $regWhere .= $regWhere != '' ? ','."'".implode("','", $v['valor'])."'" : "'".implode("','", $v['valor'])."'";
                }
            }
            $v = rawurlencode($_POST['regiao'][0]);
            if($regWhere != ''){
                $sql = "select estuf, estdescricao from territorios.estado where estuf in ($regWhere) order by estuf";
            } else {
                $sql = "select estuf, estdescricao from territorios.estado order by estuf";
            }
            $est = $db->carregar($sql);

            print simec_json_encode($est);

            exit;
    }
}

?>
<!DOCTYPE html>
<html lang="pt-BR">
<?php require "header.php"; ?>
<body class="menubar-hoverable header-fixed ">

<!-- BEGIN HEADER-->
<!-- barra do governo -->
<div id="barra-brasil">
    <a href="http://brasil.gov.br" class="barraGoverno">Portal do Governo Brasileiro</a>
</div>
<!-- fim barra do governo -->

<div class="container">
    <div id="topo">
        <div class="row">
            <div class="col-md-3 col-sm-6 col-xs-6 logo">
                <img src="img/logo.png" alt="">
            </div>
        </div> <!--  row -->
    </div> <!--  topo -->
</div> <!--  container -->
<!-- END HEADER-->

<!-- BEGIN BASE-->
<div id="base">

    <!-- BEGIN CONTENT-->
    <div id="content" class="section-body contain-lg shadow">

        <div class="row">
            <div class="col-md-12">
                <form action="" name="formPesq" id="formPesq" class="form-horizontal" role="form" method="post">
                    <div class="form-group">
                        <label for="regiao" class="col-md-3 control-label">Regiao:</label>
                        <div class="col-md-6">
                            <select name="regiao[]" id="regiao" class="form-control select uf select2-list" multiple="multiple" data-placeholder="Selecione">
                                <?php foreach ($regioes as $k => $v) { ?>
                                    <option value="<?= $k ?>" <?= is_array($_POST['regiao']) && in_array($k, $_POST['regiao']) ? 'selected="selected"' : '' ?>><?= $v['titulo'] ?></option>
                                    <?php
                                    if(is_array($_POST['regiao']) && in_array($k, $_POST['regiao'])){
                                        $regWhere .= $regWhere != '' ? ','."'".implode("','", $v['valor'])."'" : "'".implode("','", $v['valor'])."'";
                                    }
                                    ?>
                                <?php } ?>
                            </select>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="estuf" class="col-md-3 control-label">UF:</label>
                        <div class="col-md-6" id="divEstado">
                            <?php
                            if($regWhere != ''){
                                $sql = "select estuf, estdescricao from territorios.estado where estuf in ($regWhere) order by estuf";
                            } else {
                                $sql = "select estuf, estdescricao from territorios.estado order by estuf";
                            }
                            $est = $db->carregar($sql);
                            ?>
                            <select name="estuf[]" id="estuf" class="form-control select uf select2-list" multiple="multiple" data-placeholder="Selecione">
                                <?php foreach ($est as $e) { ?>
                                    <option value="<?= $e['estuf'] ?>" <?= is_array($_POST['estuf']) && in_array($e['estuf'], $_POST['estuf']) ? 'selected="selected"' : '' ?> ><?= $e['estdescricao'] ?></option>
                                <?php } ?>
                            </select>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="situacao" class="col-md-3 control-label">Situação:</label>
                        <div class="col-md-6">
                            <div class="btn-group" data-toggle="buttons">
                                <label class="btn btn-raty btn-default <?= is_array($_POST['situacao']) && in_array('A', $_POST['situacao']) ? 'active' : '' ?>">
                                    <input type="checkbox" name="situacao[]"  value="A" <?= is_array($_POST['situacao']) && in_array('A', $_POST['situacao']) ? 'checked="checked"' : '' ?>/> Não Finalizada
                                </label>
                                <label class="btn btn-raty btn-default <?= is_array($_POST['situacao']) && in_array('F', $_POST['situacao']) ? 'active' : '' ?>">
                                    <input type="checkbox" name="situacao[]"  value="F" <?= is_array($_POST['situacao']) && in_array('F', $_POST['situacao']) ? 'checked="checked"' : '' ?>/> Finalizada
                                </label>
                            </div>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-md-12 text-center">
                            <input type="submit" class="btn btn-success" value="Pesquisar"/>
                        </div>
                    </div>
                </form>
            </div>
        </div>

        <section>
            <!-- BEGIN VALIDATION FORM WIZARD -->
            <div class="row">
                <div class="row">
                    <div class="col-md-12">
                        <?php

                        //            $sql = "select distinct q.queid, p.parcpf, p.parnome, m.mundescricao, p.estuf, p.parcnpj, p.parreprazaosocial,
                        //                            case
                        //                                when p.parrepresentacao = 1 then '<span style=\"color: green;\">Pública</span>'
                        //                                when p.parrepresentacao = 2 then '<span style=\"color: red;\">Privada</span>'
                        //                                else 'Pessoa Física'
                        //                            end as representacao,
                        //                            case
                        //                                when q.quesituacao = 'F' then '<span style=\"color: green;\">FINALIZADO</span>'
                        //                                else '<span style=\"color: red;\">EM PREENCHIMENTO</span>'
                        //                            end as situacao
                        //                    from consultapnf.questionario q
                        //                        inner join consultapnf.participante p on p.parid = q.parid
                        //                        " . implode(' ', $join).  "
                        //                        left join territorios.municipio m on m.muncod = p.muncod
                        //                    {$where}
                        //                    order by p.estuf, m.mundescricao, parnome
                        //                     -- limit 10
                        //                    "
                        //
                        //                    ;
                        $where = 'where true';
                        if(is_array($_POST['estuf'])){
                            $where .= " and o.estuf in ('".implode("','", $_POST['estuf'])."')";
                        } else {
                            if($regWhere != '') {
                                $where .= " and o.estuf in ({$regWhere})";
                            }
                        }
                        if(is_array($_POST['situacao'])){
                            $where .= ' and q.quesituacao in (\''.implode("','", $_POST['situacao']).'\')';
                        }
                        $sql = "select distinct q.queid, p.parcpf, p.parnome, o.estuf, o.muncod, m.mundescricao, o.orgcnpj, o.orgrazaosocial,
                              case when q.quesituacao = 'F' then 'Finalizado' else 'Não Finalizado' end as quesituacao
                        from criatividadeeducacao.questionario q
                            inner join criatividadeeducacao.participante p on p.parid = q.parid
                            inner join criatividadeeducacao.organizacao o on o.orgid = q.orgid
                            left join territorios.municipio m on m.muncod = o.muncod
                            {$where}
                            order by o.estuf, o.muncod
                        "
                        ;

                        $dados  = $db->carregar($sql);
                        $dados = $dados ? $dados : array();
                        $total = is_array($dados) ? count($dados ) : 0;
                        ?>

                        <h2>Total de Contribuições: <span style="color: red;"><?php echo count($dados); ?></span></h2>

                        <table class="table table-hover table-striped table-condensed table-bordered">
                            <tr>
                                <th>Açoes</th>
                                <th>UF</th>
                                <th>Município</th>
                                <th>Situação</th>
                                <th>CPF</th>
                                <th>Nome</th>
                                <th>CNPJ</th>
                                <th>Razão Social</th>
                            </tr>
                            <?php foreach($dados as $dado){
                                $info = 'acompanhamento=1&q=' . $dado['queid'];
                                ?>
                                <tr>
                                    <td><a target="_blank" href="formularioAcompanhamento.php?i=<?php echo base64_encode($info); ?>" title="Visualizar"><i class="glyphicon glyphicon-search"></i></a></td>
                                    <td><?php echo $dado['estuf']; ?></td>
                                    <td><?php echo $dado['mundescricao']; ?></td>
                                    <td><?php echo $dado['quesituacao']; ?></td>
                                    <td><?php echo $dado['parcpf']; ?></td>
                                    <td><?php echo $dado['parnome']; ?></td>
                                    <td><?php echo $dado['orgcnpj']; ?></td>
                                    <td><?php echo $dado['orgrazaosocial']; ?></td>
                                </tr>
                            <?php } ?>
                        </table>

                        <hr>

                        <footer class="well well-sm">
                            <p style="text-align: center;">&copy; 2015 Ministério da Educação. Todos os direitos reservados.</p>
                        </footer>

                </div>
            </div><!--end .row -->
            <!-- END VALIDATION FORM WIZARD -->
        </section>
    </div><!--end #content-->
    <!-- END CONTENT -->

</div><!--end #base-->
<!-- END BASE -->

<?php require_once "footer.php"; ?>
<script>
    $(function(){
        $('#regiao').change(function(e) {
            e.preventDefault();
            var options = $('#estuf');
            $.ajax({
                utl: '',
                method: 'POST',
                data:{
                    acao: 'carrega_estado',
                    regiao: $(this).val()
                },
                success: function(ret){
                    options.empty();
                    var res = JSON.parse(ret);
                    $.each(res, function(){
                        console.log(this.teste);
                        options.append(new Option(this.estdescricao, this.estuf));
                    });
                    options.focus();
                }
            });
        });
    });
</script>
</body>
