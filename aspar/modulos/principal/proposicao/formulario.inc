<?PHP
$modelProposicao = new Aspar_Model_Proposicao($_REQUEST['prpid']);

switch ($_REQUEST['action']) {
    case('salvar'):
        $modelProposicao->gravar();
        break;
}

#CHAMADA DE PROGRAMA
include  APPRAIZ."includes/cabecalho.inc";

//echo $simec->title('Bem-vindo');
?>

<div class="wrapper wrapper-content animated fadeInRight">

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Cadastro de Proposição</h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>

        <div class="ibox-content" style="padding-bottom:1px;">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                <input name="action" type="hidden" value="salvar">
                <input name="prpid" id="prpid" type="hidden" value="<?=$modelProposicao->prpid;?>">

                <?php
                echo $simec->radio('casa', 'Casa', $modelProposicao->prpnumerosenado ? 'senado' : ($modelProposicao->prpnumerocamara ? 'camara' : null), array('camara' => 'Câmara dos Deputados','senado' => 'Senado Federal'));

                echo $simec->input('prpnumerosenado', 'Número', $modelProposicao->prpnumerosenado, array('maxlength' => 7));

                echo $simec->input('prpano', 'Ano', $modelProposicao->prpano, array('maxlength' => 4));

                $sql = "SELECT  posid AS codigo, posdsc AS descricao FROM aspar.posicionamento";
                echo $simec->select( 'posid', 'Posicionamento', $modelProposicao->posid, $sql, array('placeholder' => 'Selecione') );

                $sql = "SELECT  tprid AS codigo, tprdsc AS descricao FROM aspar.tipoproposicao";
                echo $simec->select( 'tprid', 'Tipo de Proposição', $modelProposicao->tprid, $sql, array('placeholder' => 'Selecione') );

                $sql = "SELECT  priid AS codigo, pridsc AS descricao FROM aspar.prioridade";
                echo $simec->select( 'priid', 'Prioridade', $modelProposicao->priid, $sql, array('placeholder' => 'Selecione') );

                echo $simec->data('prpdtsolicitacao', 'Data da Solicitaçao', $modelProposicao->prpdtsolicitacao);

                $sql = "SELECT  tipid AS codigo, tipdsc AS descricao FROM aspar.tipoimpacto";
                echo $simec->select( 'tipid', 'Impacto', $modelProposicao->tipid, $sql, array('placeholder' => 'Selecione') );

                echo $simec->data('prpprazo', 'Prazo', $modelProposicao->prpprazo);

//                $sql = "SELECT  refid AS codigo, refdsc AS descricao FROM aspar.referencia";
//                echo $simec->select( 'refid', 'Referência', $modelProposicao->refid, $sql, array('placeholder' => 'Selecione') );

                echo $simec->textarea('prptextoementa', 'Ementa', $modelProposicao->prptextoementa);
                ?>

                <div class="form-group">
                    <div class="col-lg-6 text-right">
                        <button type="submit" class="btn btn-primary"><span class="glyphicon glyphicon-thumbs-up"> </span> Salvar</button>
                    </div>
                    <div class="col-lg-6">
                        <a class="btn btn-danger" href="aspar.php?modulo=principal/proposicao/index&acao=A"><span class="glyphicon glyphicon-hand-left"> </span> Voltar</a>
                    </div>
                </div>

            </form>
        </div>
    </div>
</div>