<?php

$aPrePlanoInterno = (new Proposta_Model_PrePlanoInterno)->recuperarListagem();

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
                    
                    <a class="btn btn-small btn-warning" href="?modulo=principal/preplanointerno_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">Ações</th>
                                <th>Id</th>
                                <th>Funcional</th>
                                <th>Subunidade</th>
                                <th>Título</th>
                                <th>Enquadramento de Despesa</th>
                                <th>Quantidade</th>
                                <th>Valor Custeio</th>
                                <th>Valor Capital</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aPrePlanoInterno as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=principal/preplanointerno_form&acao=A&pliid=<?php echo $dados['pliid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['pliid']; ?></td>
                                    <td><?php echo $dados['funcional'] . ' - ' . $dados['plodsc']; ?></td>
                                    <td><?php echo $dados['subunidade']; ?></td>
                                    <td><?php echo $dados['plititulo']; ?></td>
                                    <td><?php echo $dados['eqddsc']; ?></td>
                                    <td align="right"><?php echo $dados['pliquantidade']; ?></td>
                                    <td align="right"><?php echo $dados['plivalorcusteio']; ?></td>
                                    <td align="right"><?php echo $dados['plivalorcapital']; ?></td>
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