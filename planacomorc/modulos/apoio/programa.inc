<?php

$listaPrograma = (new Monitora_Model_Programa)->buscarTodos((object)array('exercicio' => $_SESSION['exercicio']));;

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2><?= $titulo_modulo; ?></h2>
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
                    
                    <a class="btn btn-small btn-warning" href="?modulo=apoio/programa_form&acao=A">Novo</a>
                    
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="10%">Ações</th>
                                    <th>Código</th>
                                    <th>Descrição</th>
                                    <!--<th>Objetivo</th>
                                    <th>Publico Alvo</th>
                                    <th>Jutificativa</th>
                                    <th>Estratégia</th>-->
                                </tr>
                            </thead>
                            <tbody>
                            <?= "<pre>";
                            print_r($listaPrograma);
                            die;
                            ?>
                                <?php foreach($listaPrograma as $dados){ ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="?modulo=apoio/programa_form&acao=A&prgid=<?php echo $dados['prgid']; ?>">
                                                <i class="fa fa-pencil"></i>
                                            </a>
                                        </td>
                                        <td><?= $dados['prgcod']; ?></td>
                                        <td><?= $dados['prgdsc']; ?></td>
                                        <!--<td><?/*= $dados['prgdscobjetivo']; */?></td>
                                        <td><?/*= $dados['prgdscpublicoalvo']; */?></td>
                                        <td><?/*= $dados['prgdscjustificativa']; */?></td>
                                        <td><?/*= $dados['prgdscestrategia']; */?></td>-->
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