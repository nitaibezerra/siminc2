<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

class Simec_Indicadores_Renderer_Bootstrap {

    protected $grafico;

    public function __construct($incluirBiblioteca = true)
    {
        $this->grafico = new Grafico(null, $incluirBiblioteca);
    }

    public function gerarGraficoLinha($sql)
    {
        echo <<<HTML
            <div class="row">
                <div class="col-md-12">
HTML;
                    $this->grafico->setTitulo('Quantidade por ano')
                                  ->setTipo(Grafico::K_TIPO_LINHA)
                                  ->gerarGrafico($sql);
                echo <<<HTML
            </div>
        </div>
HTML;

    }

    public function gerarGraficoDetalhes($sqls)
    {
        echo <<<HTML
            <div id="tab-2">
HTML;
        foreach( $sqls as $indid => $dados ){
        echo <<<HTML
                <div class="panel-group" role="tablist" aria-multiselectable="true">
                    <div class="panel panel-default">
                        <div class="panel-heading" role="tab" id="headinge{$indid}">
                            <h4 class="panel-title">
                                <a role="button" data-toggle="collapse" data-parent="#accordion" href="#collapse{$indid}" aria-expanded="true" aria-controls="collapse{$indid}">
                                    <i class="fa fa-random"></i>{$dados['indnome']}
                                </a>
                            </h4>
                        </div>
                        <div id="collapse{$indid}" class="panel-collapse collapse in" role="tabpanel" aria-labelledby="heading{$indid}">
                            <div class="panel-body">

                                <div class="row">
HTML;
                                    foreach($dados['sql'] as $indice => $sql) {
                                        echo '<div class="col-md-12">';
                                        $this->grafico->setTitulo('Quantidade por ' . $dados['tdidsc'][$indice])
                                                      ->setTipo(Grafico::K_TIPO_COLUNA)
                                                      ->gerarGrafico($sql);
                                        echo '</div>';
                                    }
                                    echo <<<HTML
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
HTML;
        }
    }

    public function gerarResumoIndicador($dados)
    {
        echo <<<HTML
            <style>
                .div-resumo p.p-static{
                    padding-top: 5px !important;
                }

                .div-resumo .form-group{
                    margin-bottom: 0;
                }

                .div-resumo{
                    color: #333;
                }
            </style>

            <div class="div-resumo alert alert-warning">
                <div class="row">
                    <div class="col-md-12">
                        <form class="form-horizontal">
                            <div class="form-group">
                                <label class="col-sm-3 control-label">Indicador: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['indnome']}</p>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="inputPassword" class="col-sm-3 control-label">Objetivo: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['indobjetivo']}</p>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="inputPassword" class="col-sm-3 control-label">Ação Estratégica: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['acadsc']}</p>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="inputPassword" class="col-sm-3 control-label">Regionalização: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['regdescricao']}</p>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="inputPassword" class="col-sm-3 control-label">Periodicidade: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['perdsc']}</p>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="inputPassword" class="col-sm-3 control-label">Eixo: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['exodsc']}</p>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="inputPassword" class="col-sm-3 control-label">Secretaria/Autarquia Executora: </label>
                                <div class="col-sm-9">
                                    <p class="form-control-static p-static">{$dados['secdsc']}</p>
                                </div>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
HTML;

    }

    public function gerarTabelaRelatorio($periodos, $dadosAgrupados)
    {
        $totalAno = array();
        $totalAgrupador = array();
        ?>
        <style>
            .div-tabela-resumo{
                font-size: 11px;
            }
        </style>

        <div class="div-tabela-resumo">
            <table class="table table-hover table-striped table-condensed table-bordered">
                <?php foreach($dadosAgrupados as $nivel1 => $dados){ ?>
                    <tr id="tr_<?php echo $nivel1; ?>">
                        <td>
                            <span class="glyphicon glyphicon-plus" aria-hidden="true" id="uf_<?php echo $nivel1; ?>"></span>
                            <?php echo $nivel1; ?>
                        </td>
                        <?php foreach($periodos as $periodo){ ?>
                            <td class="text-right">
                                <?php
                                if(isset($dados[$periodo])){
                                    $totalAno[$periodo] += $dados[$periodo];
                                    $totalAgrupador[$nivel1] += $dados[$periodo];
                                    echo number_format($dados[$periodo], 2, ',', '.');
                                } else {
                                    echo '-';
                                }
                                ?>
                            </td>
                        <?php } ?>
                        <th class="text-right"><?php echo number_format($totalAgrupador[$nivel1], 2, ',', '.'); ?></th>
                    </tr>
                <?php } ?>

                <thead>
                    <tr>
                        <th>Descrição</th>
                        <?php foreach($periodos as $periodo){ ?>
                            <th class="text-center"><?php echo $periodo; ?></th>
                        <?php } ?>
                        <th class="text-center">Total</th>
                    </tr>
                    <tr>
                        <th>Totais</th>
                        <?php foreach($periodos as $periodo){ ?>
                            <th class="text-right"><?php echo number_format($totalAno[$periodo], 2, ',', '.'); ?></th>
                        <?php } ?>
                        <th class="text-right"><?php echo number_format(array_sum($totalAgrupador), 2, ',', '.'); ?></th>
                    </tr>
                </thead>
                <tfoot>
                    <tr>
                        <th>Totais</th>
                        <?php foreach($periodos as $periodo){ ?>
                            <th class="text-right"><?php echo number_format($totalAno[$periodo], 2, ',', '.'); ?></th>
                        <?php } ?>
                        <th class="text-right"><?php echo number_format(array_sum($totalAgrupador), 2, ',', '.'); ?></th>
                    </tr>
                    <tr>
                        <th>Descrição</th>
                        <?php foreach($periodos as $periodo){ ?>
                            <th class="text-center"><?php echo $periodo; ?></th>
                        <?php } ?>
                        <th class="text-center">Total</th>
                    </tr>
                </tfoot>
            </table>
        </div>
        <?php
    }
}
