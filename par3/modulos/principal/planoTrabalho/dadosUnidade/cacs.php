<?php
/**
 * Tela CACS
 *
 * @category visao
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$inuid = $_REQUEST['inuid'];

$modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
$modelCACS = new Par3_Model_CACS();

$listaCACS = $modelCACS->listarConselheiros($modelInstrumentoUnidade->muncod);
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Conselho de Acompanhamento e Controle Social</h3>
    	</div>
	    <?php if (count($listaCACS) > 0 && is_array($listaCACS)): ?>
    		<div class="ibox-title" style="margin-bottom: 10px;">
	    	    <h3>Situação do Conselho: <?php echo $listaCACS[0]['sit_mandato']; ?></h3>
    		</div>
	    	<div class="ibox-content">
	    		<table class="table table-hover dataTable">
	    			<thead>
	    				<tr>
	    					<th width="16%">CPF</th>
	    					<th width="25%">Nome</th>
	    					<th width="25%">Email</th>
	    					<th width="12%">Situação</th>
	    					<th width="12%">Atuação</th>
	    					<th width="12%">Vinculo</th>
	    					<th width="10%">Cargo</th>
	    				</tr>
	    			</thead>
	    			<?php foreach ($listaCACS as $cacs) : ?>
	    			<tr>
	    				<td><?php echo formatar_cpf($cacs['cpf_conselheiro']); ?></td>
	    				<td><?php echo $cacs['no_conselheiro']; ?></td>
	    				<td><?php echo $cacs['email_conselheiro']; ?></td>
	    				<td><?php echo $cacs['sit_conselheiro']; ?></td>
	    				<td><?php echo $cacs['ds_segmento']; ?></td>
	    				<td><?php echo $cacs['tp_membro']; ?></td>
	    				<td><?php echo $cacs['ds_funcao']; ?></td>
	    			</tr>
	    			<?php endforeach;?>
	    		</table>
	    	</div>
	    <?php else: ?>
	    <div class="ibox-content">
	    	<div class="alert alert-warning">Nenhum conselheiro vinculado para este município</div>
	    </div>
	    <?php endif; ?>
    </div>
</form>
