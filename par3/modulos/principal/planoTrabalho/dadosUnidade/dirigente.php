<?php
/**
 * Tela de dados da dirigente
 *
 * @category visao
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderDirigente                      = new Par3_Controller_Entidade();
$controleUnidade         			  = new Par3_Controller_InstrumentoUnidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$itrid = $controleUnidade->pegarItrid($inuid);

if ($itrid === '2') {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;
} else {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
}

switch ($_REQUEST['req']) {
	case 'salvarDirigente':
		$_POST['inuid'] = $inuid;
		$_POST['tenid'] = $tenid;
	    $controllerInstrumentoUnidadeEntidade->salvarInformacoesDirigente($_POST);
	    break;
	default:
        $objPessoaFisica      = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
        $arrPost              = array();
        $arrPost['inuid']     = $inuid;
        $arrPost['tenid']     = $tenid;
        $arrPost['entstatus'] = 'I';
        $listaHistoricoDirigentes = $modelInstrumentoUnidadeEntidade->carregaArrayHistoricoEntidade($arrPost);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid"   id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req"     value="salvarDirigente"/>
    <input type="hidden" name="tenid"   value="<?php echo $tenid; ?>"/>

    <div class="ibox">
    	<div class="ibox-title">
    	    <h3><?php echo ($itrid === '2') ? 'Dirigente' : 'Secretário '; echo $esfera; ?> de Educação</h3>
    	</div>
    	<div class="ibox-content">
    		<?php $renderDirigente->formPessoaFisica($disabled, $objPessoaFisica);?>
    		<?php $renderDirigente->formDirigente($disabled, $objPessoaFisica);?>
    	</div>
    	<div class="ibox-footer">
    		<div class="col-sm-offset-3 col-md-offset-3 col-lg-offset-3">
    	    	<button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar dirigente</button>
    		</div>
    	</div>
    </div>

	<?php if (count($listaHistoricoDirigentes) > 0 && is_array($listaHistoricoDirigentes)): ?>
    <div class="ibox">
    	<div class="ibox-title">
    	    <h3><?php echo ($itrid === '2') ? 'Dirigente' : 'Secretário '; echo $esfera; ?> de Educação - Histórico de Modificações</h3>
    	</div>
    	<div class="ibox-content">
    		<table class="table table-hover dataTable">
    			<thead>
    				<tr>
    					<th>CPF</th>
    					<th>Nome</th>
    					<th>Email</th>
    					<th>Data</th>
    				</tr>
    			</thead>
    			<?php foreach ($listaHistoricoDirigentes as $historico) : ?>
    			<tr>
    				<td><?php echo formatar_cpf($historico['entcpf']); ?></td>
    				<td><?php echo $historico['entnome']; ?></td>
    				<td><?php echo $historico['entemail']; ?></td>
    				<td><?php echo $historico['entdtinativacao']; ?></td>
    			</tr>
    			<?php endforeach;?>
    		</table>
    	</div>
    </div>
    <?php endif; ?>
</form>
<script>
	$(document).ready(function() {
		$('input[name="entcursomec"]').change(function() {
			if ($(this).val() == 't') {
				$('#entcursomecdescricao').closest('.form-group').removeClass('hidden');
			} else {
				$('#entcursomecdescricao').closest('.form-group').addClass('hidden');
				$('#entcursomecdescricao').val('');
			}	
		})
	})
</script>