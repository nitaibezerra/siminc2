<?php
$modelProposicao = new Aspar_Model_Proposicao($_REQUEST['prpid']);
switch ($_REQUEST['action']) {
    case('excluir'):
        $url = 'aspar.php?modulo=principal/proposicao/index&acao=A';
        try{
            $modelProposicao->excluir();
            $modelProposicao->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        break;
}

include  APPRAIZ."includes/cabecalho.inc";
?>

<script src="/aspar/js/funcoes.js"></script>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5> Listagem Pauta </h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>
        <div class="ibox-content">
            <div>
                <a class="btn btn-warning" href="aspar.php?modulo=principal/proposicao/formulario&acao=A"><span class="fa fa-plus-circle"> </span> Adicionar</a>
            </div>
            <div>
                <?PHP
                #GERA LISTAGEM DAS TAREFAS. EMCAMINHAMENTO.
                $sql = "
                        SELECT  prpid, prpano,
                                to_char(prpdtsolicitacao, 'DD/MM/YYYY') AS prpdtsolicitacao,
                                to_char(prpprazo, 'DD/MM/YYYY') AS prpprazo
                        FROM aspar.proposicao
                        ORDER BY prpid
                    ";
                $cabecalho = array("Ano", "Data da Solicitação", "Prazo");

                $listagem = new Simec_Listagem();
                $listagem->setCabecalho($cabecalho);
                $listagem->setQuery($sql);
                $listagem->addAcao('edit', 'editar');
                $listagem->addAcao('delete', 'excluir');

                $listagem->turnOffForm();
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>
</div>