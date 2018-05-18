<?php
$requisicao = $_REQUEST['requisicao'];
$filtropi = $_SESSION['filtropi'] = ($_POST? $_POST: $_SESSION['filtropi']);

$perfis = pegaPerfilGeral();

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

# Lista de PIs
$filtropi['exercicio'] = $_SESSION['exercicio'] - 1;
$filtropi['usucpf'] = $_SESSION['usucpf'];
$filtropi['unofundo'] = 'FALSE';

$mPlanointerno = new Spo_Model_Planointerno();
$sql = $mPlanointerno->listar((object) $filtropi);
$aDados = $mPlanointerno->carregar($sql);

?>

<div class="table-responsive" style="overflow: auto;">
    <table class="table table-striped table-bordered table-hover dataTables">
        <tbody>
        <?php
        $total['custeio'] = $total['capital'] = $total['empenhado'] = $total['pago'] = $total['liquidado'] = 0;
        foreach($aDados as $dados){
            if($dados['plistatus'] == 'A'){
                $total['custeio'] += $dados['custeio'];
                $total['capital'] += $dados['capital'];
                $total['autorizado'] += $dados['autorizado'];
                $total['empenhado'] += $dados['empenhado'];
                $total['pago'] += $dados['pago'];
                $total['liquidado'] += $dados['liquidado'];
            }
            ?>
            <tr>
                <td class="text-center" nowrap="nowrap">
                    <!--<a href="#" class="pi-importer btn btn-primary btn-sm" data-pi-id="<?php /*echo $dados['pliid']; */?>" title="Selecionar"><span class="glyphicon glyphicon-ok"></span></a>-->
                    <a href="proposta.php?modulo=principal/preplanointerno_form&acao=A&req=importar-pi&pliid=<?php echo $dados['pliid']; ?>" class="btn btn-primary btn-sm" data-pi-id="<?php echo $dados['pliid']; ?>" title="Selecionar"><span class="glyphicon glyphicon-plus-sign"></span></a>
                </td>
                <td><?php echo $dados['id']; ?></td>
                <td><?php echo $dados['codigo_pi']; ?></td>
                <td><?php echo $dados['sub_unidade']; ?></td>
                <td><?php echo $dados['plititulo']; ?></td>
                <td><?php echo $dados['funcional']; ?></td>
                <td><?php echo $dados['situacao']; ?></td>
                <td class="text-right"><?php echo number_format(($dados['custeio'] + $dados['capital']), 2, ',', '.'); ?></td>
                <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['empenhado'], $dados['custeio']+ $dados['capital'], 2); ?></th>
                <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['liquidado'], $dados['custeio']+ $dados['capital'], 2); ?></th>
                <th class="text-right"><?php echo marcarVermelhoValorSuperior($dados['pago'], $dados['custeio']+ $dados['capital'], 2); ?></th>
            </tr>
        <?php } ?>
        </tbody>
        <thead>
        <tr class="text-center">
            <th rowspan="2" width="5%">Ações</th>
            <th rowspan="2">Id</th>
            <th rowspan="2">Código</th>
            <th rowspan="2">Unidade</th>
            <th rowspan="2">Título</th>
            <th rowspan="2">Funcional</th>
            <th rowspan="2">Situação</th>
            <th colspan="5">Orçamento do PI (R$)</th>
        </tr>
        <tr class="text-center">
            <th>Valor</th>
            <th>Empenhado</th>
            <th>Liquidado</th>
            <th>Pago</th>
        </tr>
        <tr>
            <th colspan="7">TOTAL</th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior(( $total['custeio'] + $total['capital'] ), $total['custeio']+ $total['capital'], 2); ?></th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['empenhado'], $total['custeio']+ $total['capital'], 2); ?></th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['liquidado'], $total['custeio']+ $total['capital'], 2); ?></th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['pago'], $total['custeio']+ $total['capital'], 2); ?></th>
        </tr>
        </thead>
        <tfoot>
        <tr>
            <th colspan="7">TOTAL</th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior(( $total['custeio'] + $total['capital'] ), $total['custeio']+ $total['capital'], 2); ?></th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['empenhado'], $total['custeio']+ $total['capital'], 2); ?></th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['liquidado'], $total['custeio']+ $total['capital'], 2); ?></th>
            <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['pago'], $total['custeio']+ $total['capital'], 2); ?></th>
        </tr>
        </tfoot>
    </table>
</div>