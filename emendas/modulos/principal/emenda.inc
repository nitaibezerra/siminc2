<?php

$perfis = pegaPerfilGeral();
$mEmenda = new Emendas_Model_Emenda($_REQUEST['emeid']);
$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

//ver($listaSubUnidadeUsuario);
switch ($_REQUEST['req']) {
    case 'proposta_modal':
        include 'proposta_modal.inc';
    die;
    case 'salvar-proposta':
        $cProposta = new Emendas_Controller_SiconvBeneficiario();
        $cProposta->salvar($_REQUEST);
    die;
}

$aEmenda = (new Emendas_Model_Emenda)->recuperarListagem($_SESSION['exercicio'], $listaSubUnidadeUsuario);

include APPRAIZ . "includes/cabecalho.inc";

?>

<script src="js/principal/emenda.js?v=4"></script>

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
                    <h5>Pesquisa avançada por PI</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
                            $sqlSubunidade = (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlComboUnidade((object)['exercicio' => $_SESSION['exercicio']]);
                            echo $simec->select('suoid', 'SubUnidade', $filtropi['suoid'], $sqlSubunidade);
                            echo $simec->select('suoid_delegada', 'SubUnidade Delegada', $filtropi['suoid_delegada'], $sqlSubunidade);
                            echo $simec->select('lista_esdid', 'Situação', $filtropi['lista_esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_BENEFICIARIO)), array('multiple' => 'true'));
                            $modelSiconvSituacao = new Emendas_Model_SiconvSituacao();
                            $sqlSituacaoSiconv = $modelSiconvSituacao->recuperarSqlCombo(['sitid', 'sitdsc'], NULL, 'sitid');
                            echo $simec->select('lista_sitid', 'Situação SICONV', $filtropi['lista_sitid'], $sqlSituacaoSiconv, array('multiple' => 'true'));
                            echo $simec->radio('benimpedimento', 'Impedimento', $filtropi['benimpedimento'], ['t' => 'Sim', 'f' => 'Não']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-success btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                                    <i class="fa fa-file-excel-o"></i> Exportar XLS
                                </button>
                                <?php if($podeEditar): ?>
                                    <button id="btn_novo" class="btn btn-warning" type="button">
                                        <span class="glyphicon glyphicon-plus"></span>
                                        Novo
                                    </button>
                                <?php endif; ?>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>

    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover dataTablesSP" >
                            <thead>
                            <tr class="text-center">
                                <th width="5%">Ações</th>
                                <th>Número</th>
                                <th width="10%">Unidade</th>
                                <th width="10%">Autor</th>
                                <th>UF</th>
                                <th>Id Beneficiário</th>
                                <th>Beneficiário</th>
                                <th width="10%">Subunidade</th>
                                <th>Impedimento</th>
                                <th>Situação</th>
                                <th>Situação SICONV</th>
                                <th>Custeio</th>
                                <th>Capital</th>
                                <th>Empenhado</th>
                                <th>Pago</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aEmenda as $dados){?>
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
                                    <td><?php echo $dados['unidade']; ?></td>
                                    <td><?php echo $dados['autnome']; ?></td>
                                    <td><?php echo $dados['estuf']; ?></td>
                                    <td><?php echo $dados['benid']; ?></td>
                                    <td><?php echo $dados['pronome']; ?></td>
                                    <td><?php echo $dados['subunidade']; ?></td>
                                    <td><?php echo $dados['empedimento']; ?></td>
                                    <td><?php echo $dados['esddsc']; ?></td>
                                    <td>
                                      <!-- se Situação SICONV tiver mais de uma proposta, executa o if abaixo -->
                                    <?php if($dados['qtdrepetida']!=0){?>
                                      <span class="editar_situacao fa fa-pencil detalhar-linha link" data-benid="<?php echo $dados['benid'];?>" title="Selecionar Proposta"></span>
                                    <?php } echo $dados['sitdsc']; ?>
                                      </span>
                                    </td>
                                    <td><?php echo number_format($dados['custeio'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($dados['capital'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($dados['empenhado'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($dados['pago'], 2, ',', '.'); ?></td>
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

<?php

    bootstrapPopup("Selecionar Proposta", 'proposta_modal', '', array('salvar'), array('tamanho' => 'lg'));

?>
<script>

    $(document).ready(function(){
        initListaEmendas();
    });

</script>
