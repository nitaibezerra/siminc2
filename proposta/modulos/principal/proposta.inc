<?php

$aProposta = (new Proposta_Model_Proposta)->recuperarListagem();

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
                            <thead>
                            <tr class="text-center">
                                <th width="10%">Ações</th>
                                <th>Id</th>
                                <th>Subunidade</th>
                                <th>Funcional</th>
                                <th>Enquadramento de Despesa</th>
                                <th>Quantidade PO</th>
                                <th>Quantidade Expansão</th>
                                <th>Custo Total</th>
                                <th>Custo Expansão</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aProposta as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=principal/proposta_form&acao=A&proid=<?php echo $dados['proid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['proid']; ?></td>
                                    <td><?php echo $dados['subunidade']; ?></td>
                                    <td><?php echo $dados['funcional'] . ' - ' . $dados['acatitulo'] . ' - ' . $dados['plodsc']; ?></td>
                                    <td><?php echo $dados['eqddsc']; ?></td>
                                    <td align="right"><?php echo number_format($dados['proquantidade'], 0, ',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['proquantidadeexpansao'], 0, ',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['prdvalor'], 0, ',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['prdvalorexpansao'], 0, ',', '.'); ?></td>
                                </tr>
                            <?php } ?>        
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>