<?php

$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$aProposta = (new Proposta_Model_Proposta)->recuperarListagem((object) array(
    'exercicio' => $_SESSION['exercicio'],
    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
));

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    
                    <a class="btn btn-small btn-warning" href="?modulo=principal/proposta_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <tbody>
                            <?php
                                # Inicialização das variáveis do totalizador dos dados.
                                $totalQtdPo = 0;
                                $totalQtdExp = 0;
                                $totalCustoT = 0;
                                $totalCustoExp = 0;
                                
                            foreach($aProposta as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=principal/proposta_form&acao=A&proid=<?php echo $dados['proid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?= $dados['proid']; ?></td>
                                    <td><?= $dados['subunidade']; ?></td>
                                    <td><?= $dados['funcional'] . ' - ' . $dados['acatitulo'] . ' - ' . $dados['plodsc']; ?></td>
                                    <td><?= $dados['eqddsc']; ?></td>
                                    <td align="right"><?= number_format($dados['proquantidade'], 0, ',', '.'); ?></td>
                                    <td align="right"><?= number_format($dados['proquantidadeexpansao'], 0, ',', '.'); ?></td>
                                    <td align="right"><?= number_format($dados['prdvalor'], 0, ',', '.'); ?></td>
                                    <td align="right"><?= number_format($dados['prdvalorexpansao'], 0, ',', '.'); ?></td>
                                </tr>
                            <?php
                                # Soma dos Totalizadores.
                                $totalQtdPo += (int)$dados['proquantidade'];
                                $totalQtdExp += (int)$dados['proquantidadeexpansao'];
                                $totalCustoT += (int)$dados['prdvalor'];
                                $totalCustoExp += (int)$dados['prdvalorexpansao'];
                            ?>
                            <?php } ?>
                            </tbody>
                            <thead>
                            <tr class="text-center">
                                <th width="4%">Ações</th>
                                <th>Id</th>
                                <th>Subunidade</th>
                                <th>Funcional</th>
                                <th>Enquadramento de Despesa</th>
                                <th>Quantidade PO</th>
                                <th>Quantidade Expansão</th>
                                <th>Custo Total</th>
                                <th>Custo Expansão</th>
                            </tr>
                            <tr style="vertical-align: middle" class="text-right">
                                <th colspan="5" style="text-align: left"><h5>Total</h5></th>
                                <th><?= number_format($totalQtdPo , 0, ',', '.'); ?></th>
                                <th><?= number_format($totalQtdExp , 0, ',', '.'); ?></th>
                                <th><?= number_format($totalCustoT,0,',','.'); ?></th>
                                <th><?= number_format($totalCustoExp,0,',','.'); ?></th>
                            </tr>
                            </thead>
                            <tfoot>
                                <tr style="vertical-align: middle" class="text-right">
                                    <th colspan="5" style="text-align: left"><h5>Total</h5></th>
                                    <th><?= number_format($totalQtdPo , 0, ',', '.'); ?></th>
                                    <th><?= number_format($totalQtdExp , 0, ',', '.'); ?></th>
                                    <th><?= number_format($totalCustoT,0,',','.'); ?></th>
                                    <th><?= number_format($totalCustoExp,0,',','.'); ?></th>
                                </tr>
                            </tfoot>
                        </table>

                    </div>
                </div>
            </div>
        </div>
    </div>
</div>