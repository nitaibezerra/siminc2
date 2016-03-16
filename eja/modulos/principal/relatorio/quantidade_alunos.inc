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
    <h4 id="forms">Quantidade Alunos</h4>   
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao"value="listar">
            <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">Tipo de Gráfico:</label>
                <div class="col-lg-10">
                    <select name="tipo_rel" id="tipo_rel"class="form-control chosen-select">
                        <option value="">Selecione</option>
                        <option value="K_TIPO_BARRA" <?php
                        if ($_REQUEST['tipo_rel'] == 'K_TIPO_BARRA') {
                            echo 'selected';
                        }
                        ?>>Barra</option>
                        <option value="K_TIPO_PIZZA"  <?php
                        if (!$_REQUEST['tipo_rel'] || $_REQUEST['tipo_rel'] == 'K_TIPO_PIZZA') {
                            echo 'selected';
                        }
                        ?>>Pizza</option>
                       
                        <option value="K_TIPO_COLUNA"  <?php
                        if ($_REQUEST['tipo_rel'] == 'K_TIPO_COLUNA') {
                            echo 'selected';
                        }
                        ?>>Coluna</option>
                    </select>
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
                $anoQuery = $_REQUEST['ano'];
            } else {
                $anoQuery = '2015';
            }

            $sqlTotal = "SELECT COUNT(ate.atecpf)
                         FROM eja.alunoturmaeja ate
                         INNER JOIN eja.novaturmaeja nte ON nte.nteid=ate.nteid
                         INNER JOIN entidade.endereco ende ON ende.entid = nte.pk_cod_entidade
                         JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                         JOIN territorios.estado est ON est.estuf = mun.estuf
                         WHERE ateexercicio = '{$anoQuery}'
                         AND nteexercicio = '{$anoQuery}'";

            $total_Brasil = $db->pegaUm($sqlTotal);
            ?> <h8>Total Brasil</h8> : <?php
            echo $total_Brasil;
            $sql = "SELECT est.estuf AS descricao,
       COUNT(ate.atecpf) AS valor
FROM eja.alunoturmaeja ate
INNER JOIN eja.novaturmaeja nte ON nte.nteid=ate.nteid
INNER JOIN entidade.endereco ende ON ende.entid = nte.pk_cod_entidade
JOIN territorios.municipio mun ON mun.muncod = ende.muncod
JOIN territorios.estado est ON est.estuf = mun.estuf
WHERE ateexercicio = '{$anoQuery}'
  AND nteexercicio = '{$anoQuery}'
GROUP BY est.estuf
ORDER BY est.estuf,
         descricao";

            global $db;
            $dados = $db->carregar($sql);

            $tooltip = "function() { return '<span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }";
            
            $grafico = new Grafico(null, false);
            if ($_REQUEST['tipo_rel']) {
                if ($_REQUEST['tipo_rel'] == 'K_TIPO_BARRA') {
                    echo $grafico->setTipo(Grafico::K_TIPO_BARRA)->setHeight('600px')->setId('graficoExecucao')->setTitulo('Quantidade de Alunos - '.$anoQuery)->setFormatoTooltip($tooltip)->gerarGrafico($sql);
                }
                if ($_REQUEST['tipo_rel'] == 'K_TIPO_PIZZA') {
                    echo $grafico->setTipo(Grafico::K_TIPO_PIZZA)
                                 ->setHeight('600px')
                                 ->setId('graficoExecucao')
                                 ->setTitulo('Quantidade de Alunos - '.$anoQuery)
                                 ->setFormatoTooltip("function() { return '<b>' + this.point.name + '</b><br />Quantidade: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }")
                                 ->gerarGrafico($sql);
                }
                if ($_REQUEST['tipo_rel'] == 'K_TIPO_COLUNA') {
                    echo $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setHeight('600px')->setId('graficoExecucao')->setTitulo('Quantidade de Alunos - '.$anoQuery)->setFormatoTooltip($tooltip)->gerarGrafico($sql);
                }
            } else {
                echo $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setHeight('600px')->setId('graficoExecucao')->setTitulo('Quantidade de Alunos - '.$anoQuery)->setFormatoTooltip($tooltip)->gerarGrafico($sql);
            }
            ?>
        </section>
    </section>
</section>
