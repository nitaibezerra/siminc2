<?php
/**
 * Arquivo da aba detalhamento.
 * $Id: inicio.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

// -- Dados do localizador
$locdados = carregarDadosLocalizador($dadospao['snaid'], $dadospao['paoid']);
?>
<style type="text/css">
</style>
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>
<div class="row col-md-12">
    <div class="col-md-10 col-md-offset-1">
        <div class="panel panel-primary">
            <div class="panel-heading">Informações do localizador</div>
            <table class="table">
                <tr>
                    <td><strong>Ação:</strong></td>
                    <td colspan="3"><?php echo "{$locdados['acacod']} - {$locdados['acadsc']}"; ?></td>
                </tr>
                <tr>
                    <td><strong>Programa:</strong></td>
                    <td width="35%"><?php echo "{$locdados['prgcod']} - {$locdados['prgdsc']}"; ?></td>
                    <td><strong>Funcional Programática:</strong></td>
                    <td><?php echo $locdados['programatica']; ?></td>
                </tr>
                <tr>
                    <td><strong>Localizador:</strong></td>
                    <td><?php echo "{$locdados['loccod']} - {$locdados['locdsc']}"; ?></td>
                    <td>&nbsp;</td>
                    <td>&nbsp;</td>
                </tr>
            </table>
        </div>
    </div>
    <br style="clear:both" />
    <?php require(dirname(__FILE__) . '/formFisico.inc'); ?>
    <br style="clear:both" />
    <?php require(dirname(__FILE__) . '/listarAlteracoesPO.inc'); ?>
</div>