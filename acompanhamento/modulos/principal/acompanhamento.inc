<?php

$aAcompanhamento = (new Acompanhamento_Model_Acompanhamento)->recuperarTodos('*' , ["prsano = '{$_SESSION['exercicio']}'"]);

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
                    
                    <a class="btn btn-small btn-warning" href="?modulo=apoio/acompanhamento_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">Ações</th>
                                <th>acoquantidade</th>
                                <th>acodata</th>
                                <th>acoanalise</th>
                                <th>claid</th>
                                <th>acoprovidencias</th>
                                <th>mppid</th>
                                <th>ipnid</th>
                                <th>pliid</th>
                                <th>janid</th>
                                <th>prsano</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aAcompanhamento as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=apoio/acompanhamento_form&acao=A&acoid=<?php echo $dados['acoid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['acoquantidade']; ?></td>
                                    <td><?php echo $dados['acodata']; ?></td>
                                    <td><?php echo $dados['acoanalise']; ?></td>
                                    <td><?php echo $dados['claid']; ?></td>
                                    <td><?php echo $dados['acoprovidencias']; ?></td>
                                    <td><?php echo $dados['mppid']; ?></td>
                                    <td><?php echo $dados['ipnid']; ?></td>
                                    <td><?php echo $dados['pliid']; ?></td>
                                    <td><?php echo $dados['janid']; ?></td>
                                    <td><?php echo $dados['prsano']; ?></td>
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