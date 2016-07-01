<?php
/**
 * Tela de dados do prefeito
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01/10/2015
 * @link     no link
 */
$renderEntidade                       = new Par3_Controller_Entidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$tenid = Par3_Model_InstrumentoUnidadeEntidade::PREFEITO;

switch($_REQUEST['req']){
	case 'salvar':
	    $controllerInstrumentoUnidadeEntidade->salvarInformacoesPrefeito($_POST);
	    break;
	default:
        $objPessoaFisica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco     = new Par3_Model_Endereco($objPessoaFisica->endid);
        $arrPost                  = array();
        $arrPost['inuid']         = $inuid;
        $arrPost['tenid']         = $tenid;
        $arrPost['entstatus']     = 'I';
        $listaHistoricoPrefeitura = $modelInstrumentoUnidadeEntidade->carregaArrayHistoricoEntidade($arrPost);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req" value="salvar"/>
    <input type="hidden" name="tenid" value="<?php echo $tenid; ?>"/>

    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Dados da Prefeito</h3>
    	</div>
    	<div class="ibox-content">
    		<?php $renderEntidade->formPessoaFisica($disabled, $objPessoaFisica);?>
    	</div>
    	<div class="ibox-title">
        	<h3>Endereço da Prefeito</h3>
    	</div>
    	<div class="ibox-content">
    		<?php $renderEntidade->formEnderecoEntidade($disabled, $objEndereco);?>
    	</div>
    	<div class="ibox-footer">
    		<div class="col-sm-offset-3 col-md-offset-3 col-lg-offset-3">
        		<button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar prefeito</button>
    		</div>
    	</div>
    </div>
</form>
<div class="ibox">
	<div class="ibox-title">
	    <h3>Prefeito - Histórico Modificações</h3>
	</div>
<?php
if (count($listaHistoricoPrefeitura) > 0 && is_array($listaHistoricoPrefeitura)) { ?>
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
		<?php foreach ($listaHistoricoPrefeitura as $historico) : ?>
		<tr>
			<td><?php echo formatar_cpf($historico['cadastro']); ?></td>
			<td><?php echo $historico['entnome']; ?></td>
			<td><?php echo $historico['entemail']; ?></td>
			<td><?php echo formata_data($historico['entdtinativacao']); ?></td>
		</tr>
		<?php endforeach;?>
	</table>
</div>
<?php }; ?>
</div>