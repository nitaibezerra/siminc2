<?php

$perfis = pegaPerfilGeral();
$mEmenda = new Emendas_Model_Emenda($_REQUEST['emeid']);
$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
//ver($listaSubUnidadeUsuario);

$aEmenda = (new Emendas_Model_Emenda)->recuperarListagem($_SESSION['exercicio'], $listaSubUnidadeUsuario);

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

                    <?php if($podeEditar): ?>
                        <a class="btn btn-small btn-warning" href="?modulo=principal/emenda_form&acao=A">Novo</a>
                    <?php endif; ?>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">Ações</th>
                                <th>Número</th>
                                <th>Autor</th>
                                <th>Beneficiário</th>
                                <th>Unidade</th>
                                <th>Situação</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aEmenda as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Emenda" href="?modulo=principal/emenda_form&acao=A&emeid=<?php echo $dados['emeid']; ?>"><i class="fa fa-file-o"></i></a>
                                        <?php if($dados['benid']): ?>
                                            <a title="Beneficiário" href="?modulo=principal/beneficiario_form&acao=A&benid=<?php echo $dados['benid']; ?>" style="margin-left: 5px;"><i class="fa fa-usd"></i></a>
                                        <?php else: ?>
                                            <a title="Beneficiário" href="?modulo=principal/beneficiario_form&acao=A&emeid=<?php echo $dados['emeid']; ?>" style="margin-left: 5px;"><i class="fa fa-usd"></i></a>
                                        <?php endif; ?>
                                    </td>
                                    <td><?php echo $dados['emenumero']; ?></td>
                                    <td><?php echo $dados['autnome']; ?></td>
                                    <td><?php echo $dados['pronome']; ?></td>
                                    <td><?php echo $dados['unidade']; ?></td>
                                    <td><?php echo $dados['sitdsc']; ?></td>
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