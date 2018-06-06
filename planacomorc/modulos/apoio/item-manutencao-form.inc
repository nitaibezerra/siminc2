<?php

$cManutencaoItem= new Public_Controller_ManutencaoItem();
switch ($_REQUEST['req']) {
	case 'salvar':
		$cManutencaoItem->salvar($_REQUEST);
		die;
	case 'excluir':
		$cManutencaoItem->excluir($_REQUEST['maiid']);
		die;
	case 'formulario-subitem':
		$cManutencaoItem->montarFormulario($_REQUEST['maiid'], $_REQUEST['masid']);
		die;
	case 'listar-subitem':
		$cManutencaoItem->montarListagem($_REQUEST['maiid']);
		die;
	case 'salvar-subitem':
		$cManutencaoSubItem = new Public_Controller_ManutencaoSubItem();
		$cManutencaoSubItem->salvar($_REQUEST);
		die;
	case 'excluir-subitem':
		$cManutencaoSubItem = new Public_Controller_ManutencaoSubItem();
		$cManutencaoSubItem->excluir($_REQUEST);
		die;
}

$mManutencaoItem = new Planacomorc_Model_ManutencaoItem($_REQUEST['maiid']);
include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Item</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="maiid" id="maiid" type="hidden" value="<?php echo $mManutencaoItem->maiid; ?>">

						<?php
						echo $simec->input('mainome', 'Nome', $mManutencaoItem->mainome, ['required','maxlength' => 200]);
						echo $simec->input('maidescricao', 'Descrição', $mManutencaoItem->maidescricao, ['maxlength' => 500]);
						echo $simec->radio('maistatus', 'Status', ($mManutencaoItem->maistatus ? $mManutencaoItem->maistatus : 'A'), ['A' => 'Ativo', 'I' => 'Inativo']);
						?>
                        <div class="form-group">
                            <label for="eqdid" class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">Enquadramento da Despesa:</label>
                            <div class="col-sm-9 col-md-9 col-lg-9 carregaEnq">
                                <?php
                                $sql = "
                                    SELECT
                                        eqdid AS codigo,
                                        eqddsc AS descricao
                                    FROM monitora.pi_enquadramentodespesa
                                    WHERE
                                        eqdstatus = 'A'
                                        AND eqdano = '{$_SESSION['exercicio']}'
                                    ORDER BY
                                        eqddsc
                                ";
                                $eqdid = $mManutencaoItem->eqdid;
                                $db->monta_combo('eqdid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'eqdid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($eqdid) ? $eqdid : null));
                                ?>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" id="btn-salvar" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/item-manutencao&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
								<?php if($mManutencaoItem->maiid){ ?>
                                    <a href="?modulo=apoio/item-manutencao-form&acao=A&req=excluir&maiid=<?php echo $mManutencaoItem->maiid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
								<?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>

        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Subitem</h5>
                </div>
                <div class="ibox-content">
                    <div id="div_formulario_subitem">
						<?php
						if($mManutencaoItem->maiid){
							$cManutencaoItem->montarFormulario($mManutencaoItem->maiid);
						}
						?>
                    </div>
                    <div id="div_listagem_subitem">
						<?php $cManutencaoItem->montarListagem($mManutencaoItem->maiid); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
