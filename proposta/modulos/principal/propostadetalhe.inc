<?php

$aPropostaDetalhe = (new Proposta_Model_PropostaDetalhe)->recuperarTodos();

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
                    
                    <a class="btn btn-small btn-warning" href="?modulo=proposta/propostadetalhe_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">Ações</th>
                                <th>ndpid</th>
                                <th>iducod</th>
                                <th>foncod</th>
                                <th>idoid</th>
                                <th>prdvalor</th>
                                <th>prdvalorexpansao</th>
                                <th>proid</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aPropostaDetalhe as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=proposta/propostadetalhe_form&acao=A&prdid=<?php echo $dados['prdid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['ndpid']; ?></td>
                                    <td><?php echo $dados['iducod']; ?></td>
                                    <td><?php echo $dados['foncod']; ?></td>
                                    <td><?php echo $dados['idoid']; ?></td>
                                    <td><?php echo $dados['prdvalor']; ?></td>
                                    <td><?php echo $dados['prdvalorexpansao']; ?></td>
                                    <td><?php echo $dados['proid']; ?></td>
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