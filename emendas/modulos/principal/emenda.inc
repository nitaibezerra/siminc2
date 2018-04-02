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
                                      <!-- se Situação SICONV for vazio, executa o if abaixo -->
                                    <?php if($dados['sitdsc'] == null){ ?>
                                      <span class="editar_situacao fa fa-plus detalhar-linha link" data-benid="<?php echo $dado['benid']; ?>" title="Adicionar Proposta">
                                      <?php echo "Proposta"; ?>
                                      </span>
                                    <?php } else{?>
                                      <span class="editar_situacao fa fa-plus detalhar-linha link" data-benid="<?php echo $dado['benid']; ?>" title="Adicionar Proposta">
                                    <?php echo $dados['sitdsc']; ?>
                                      </span>
                                    <?php  } ?>
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
  bootstrapPopup("Proposta", 'modal-proposta-modal', '', array('fechar'), array('tamanho' => 'lg'));

  bootstrapPopup("PI's da Unidade", 'detalhe-subunidade', '', array('fechar'), array('tamanho' => 'lg'));
?>
<script>
    $(document).ready(function(){

        $('.dataTablesSP').DataTable({
            bPaginate: false,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });

        $('.editar_situacao').click(function(){
            $('#proposta-modal .modal-body').load('emendas.php?modulo=principal/unidade/painel&acao=A&req=proposta_modal&benid='+ $(this).data('benid'));
            $('#proposta-modal').modal();
        });

        $('.detalhar-linha').click(function(){

            var benid = $(this).data('benid');

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.tr_' + benid).hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.tr_' + benid).show('slow');
            }
        });
        $('.detalhar-todos').click(function(){

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.detalhar-linha').removeClass('fa-minus').addClass('fa-plus');
                $('.tr_detalhe').hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.detalhar-linha').removeClass('fa-plus').addClass('fa-minus');
                $('.tr_detalhe').show('slow');
            }
        });
    });
</script>
