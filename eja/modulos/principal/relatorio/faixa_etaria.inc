<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
<script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
<?php
$perfis = pegaPerfilGeral();

   /**
     * Componente para Graficos.
     */
    include_once APPRAIZ . "includes/library/simec/Grafico.php";
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
    $(document).ready(function() {
        $('#buscar').click(function() {
            $('#requisicao').val('listar');
            $("#formPrincipal").submit();
        });
    });

</script>

<div class="page-header">
    <h4 id="forms">Faixa Etária</h4>   
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao"value="listar">
            <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">UF:</label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['uf']) {

                        $uf = $_REQUEST['uf'];
                    }
                    $sql = "
                                        SELECT DISTINCT
                                            e.estuf AS codigo,
                                            e.estuf || ' - ' || e.estdescricao AS descricao
                                        FROM
                                            territorios.estado e where 1 = 1  {$whereGestorEst} {$whereMunUf}
                                        ORDER BY
                                            codigo ASC
                                    ";
//                                            ver($sql,d);
                    $db->monta_combo('uf', $sql, 'S', 'Todos', 'carregaMuni', null, null, null, 'N', 'uf', null, isset($uf) ? $uf : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>
             <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">Ano:</label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['ano']) {

                        $ano = $_REQUEST['ano'];
                    }
                    $sql = "
                                        SELECT DISTINCT ateexercicio codigo,
                                                        ateexercicio AS descricao
                                        FROM eja.alunoturmaeja
                                        WHERE ateexercicio IS NOT NULL
                                        ORDER BY codigo ASC
                                    ";
                    $db->monta_combo('ano', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'uf', null, isset($uf) ? $uf : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <button class="btn btn-primary" id="buscar" type="button">Pesquisar</button>
                </div>
            </div>
        </form>
    </fieldset>
</div>
<section class="col-md-12">
    <section class="panel panel-primary">
        <section class="panel-heading"><strong>GRÁFICO</strong></section>
        <section class="panel-body" style="background-color: royalblue;">
            <?php
            if ($_REQUEST['ano']) {
                $_REQUEST['ano'] = $_REQUEST['ano'];
            } else {
                $_REQUEST['ano'] = '2015';
            }
            
            if ($_REQUEST['uf']) {
                $sql = "SELECT est.estuf AS uf,
       CASE
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 0
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 14 THEN 'Menores de 14 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 15
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 19 THEN '15 aos 19'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 20
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 24 THEN '20 aos 24'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 25
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 29 THEN '25 aos 29'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 30
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 34 THEN '30 aos 34'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 35
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 39 THEN '35 aos 39'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 40
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 44 THEN '40 aos 44'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 45
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 49 THEN '45 aos 49'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 50
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 54 THEN '50 aos 54'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 55
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 59 THEN '55 aos 59'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 60
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 64 THEN '60 aos 64'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 65
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 999 THEN '65 em diante'
       END AS descricao,
       COUNT(0) AS valor
FROM eja.alunoturmaeja ate
INNER JOIN eja.novaturmaeja nte ON nte.nteid=ate.nteid
INNER JOIN entidade.endereco ende ON ende.entid = nte.pk_cod_entidade
JOIN territorios.municipio mun ON mun.muncod = ende.muncod
JOIN territorios.estado est ON est.estuf = mun.estuf
WHERE est.estuf = '{$_REQUEST['uf']}'
  AND ateexercicio = '{$_REQUEST['ano']}'
  AND nteexercicio = '{$_REQUEST['ano']}'
GROUP BY est.estuf,
         descricao
ORDER BY est.estuf,
         descricao;";

                global $db;
                $dados = $db->carregar($sql);
                
             
              $grafico = new Grafico(null, false);
            echo $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setHeight('600px')->setId('graficoExecucao')->setTitulo('Faixa Etária - '.$_REQUEST['uf'].'-'.$_REQUEST['ano'])->gerarGrafico($dados);
            } else {
                $sql = " 
                   SELECT CASE
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 0
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 14 THEN 'Menores de 14 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 15
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 19 THEN '15 aos 19 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 20
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 24 THEN '20 aos 24 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 25
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 29 THEN '25 aos 29 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 30
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 34 THEN '30 aos 34 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 35
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 39 THEN '35 aos 39 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 40
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 44 THEN '40 aos 44 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 45
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 49 THEN '45 aos 49 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 50
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 54 THEN '50 aos 54 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 55
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 59 THEN '55 aos 59 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 60
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 64 THEN '60 aos 64 anos'
           WHEN extract(YEAR
                        FROM age(ate.atedatanasc)) >= 65
                AND extract(YEAR
                            FROM age(ate.atedatanasc)) <= 999 THEN '65 aos 999 anos'
       END AS descricao,
       COUNT(0) AS valor
FROM eja.alunoturmaeja ate
INNER JOIN eja.novaturmaeja nte ON nte.nteid=ate.nteid
WHERE ateexercicio = '{$_REQUEST['ano']}'
  AND nteexercicio = '{$_REQUEST['ano']}'
GROUP BY descricao
ORDER BY descricao";
                
                global $db;
                $dados = $db->carregar($sql);
                
               
                            $grafico = new Grafico(null, false);
            echo $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setHeight('600px')->setId('graficoExecucao')->setTitulo('Faixa Etária - '.$_REQUEST['ano'])->gerarGrafico($dados);
            }


            ?>
