<?php include APPRAIZ . "includes/cabecalho.inc"; ?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
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

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/item-manutencao-form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables">
                            <thead>
                            <tr class="text-center">
                                <th>Ação</th>
                                <th>Nome</th>
                                <th>Descrição</th>
                                <th>Enquadramento</th>
                                <th>Status</th>
                            </tr>
                            </thead>
                            <tbody>
							<?php
							$sql = "
                                SELECT 
                                  maiid AS codigo, 
                                  mainome AS nome,
                                  maidescricao AS descricao,
                                  maistatus AS status,
                                  eqdid
                                FROM planacomorc.manutencaoitem
                                WHERE prsano = '{$_SESSION['exercicio']}'
                                --AND maistatus = 'A'
                                --AND eqdid = " . (int)$eqdid . "
                                ORDER BY nome
                            ";
							$manutencao_itens = $db->carregar($sql);

							if ($manutencao_itens):
								foreach ($manutencao_itens as $manutencao_item): ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="planacomorc.php?modulo=apoio/item-manutencao-form&acao=A&maiid=<?php echo $manutencao_item['codigo']; ?>"><i class="fa fa-pencil"></i></a>
                                        </td>
                                        <td><?php echo $manutencao_item['nome']; ?></td>
                                        <td><?php echo $manutencao_item['descricao']; ?></td>
                                        <td><?php
                                            $sql = "
                                                SELECT
                                                    eqddsc AS descricao
                                                FROM monitora.pi_enquadramentodespesa
                                                WHERE
                                                    eqdstatus = 'A'
                                                    AND eqdano = '{$_SESSION['exercicio']}'
                                                    AND eqdid = {$manutencao_item['eqdid']}
                                            "; echo $db->pegaUm($sql); ?></td>
                                        <td><?php echo $manutencao_item['status'] == 'A' ? 'Ativo' : 'Inativo'; ?></td>
                                    </tr>
								<?php endforeach;
							endif; ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
