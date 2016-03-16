<?php
/**
 * Tela de dados do conselho
 *
 * @category visao
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderConselho                       = new Par3_Controller_Entidade();
$controleUnidade         			  = new Par3_Controller_InstrumentoUnidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$itrid = $controleUnidade->pegarItrid($inuid);

if ($itrid === '2') {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_MUNICIPAL;
} else {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_ESTADUAL;
}

switch ($_REQUEST['req']) {
	case 'salvarConselho':
	    $controllerInstrumentoUnidadeEntidade->salvarInformacoesConselho($_REQUEST);
	    break;
	case 'removerConselheiro':
	   	$controllerInstrumentoUnidadeEntidade->inativarConselheiro($_REQUEST);
	   	break;	    
	default:
        $listaHistoricoConselho = $modelInstrumentoUnidadeEntidade->carregarConselheiros($inuid);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <input type="hidden" name="inuid"   id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="tenid"   id="tenid" value="<?php echo $tenid; ?>"/>
    <input type="hidden" name="req"     value="salvarConselho"/>
    
    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Conselho <?php echo $esfera; ?> de Educação</h3>
    	</div>
    	<div class="ibox-content">
    		<?php $renderConselho->formConselho($disabled, $objPessoaFisica);?>
    	</div>
    	<div class="ibox-footer">
    		<div class="col-sm-offset-3 col-md-offset-3 col-lg-offset-3">
    	    	<button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa fa-plus-square-o"></i> Incluir conselheiro</button>
    		</div>
    	</div>
    </div>
    
	<?php if (count($listaHistoricoConselho) > 0 && is_array($listaHistoricoConselho)): ?>
    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Conselheiros <?php echo $esfera; ?> de Educação</h3>
    	</div>
    	<div class="ibox-content">
    		<table class="table table-hover dataTable">
    			<thead>
    				<tr>
    					<th></th>
    					<th>CPF</th>
    					<th>Nome</th>
    					<th>Email</th>
    					<th>Atuação</th>
    					<th>Cargo</th>
    				</tr>
    			</thead>
    			<?php foreach ($listaHistoricoConselho as $historico) : ?>
    			<tr>
    				<td>
    					<a href="javascript:inativaConselheiro('<?php echo $historico['entid']; ?>');" title="Remover conselheiro">
    						<span class="btn btn-danger btn-xs glyphicon glyphicon-trash"></span>
    					</a>
    				</td>
    				<td><?php echo formatar_cpf($historico['entcpf']); ?></td>
    				<td><?php echo $historico['entnome']; ?></td>
    				<td><?php echo $historico['entemail']; ?></td>
    				<td><?php echo $historico['entatuacao']; ?></td>
    				<td><?php echo $historico['entcargo']; ?></td>
    			</tr>
    			<?php endforeach;?>
    		</table>
    	</div>
    </div>
    <?php endif; ?>
</form>
<script>
function inativaConselheiro(id)
{
    var inuid  = $('#inuid').val();
    var url    = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid+'&menu=conselho';
    var action = '&req=removerConselheiro';
    var param  = '&entid=' + id + '&tenid=' + $('#tenid').val();
    window.location.href = url+action+param;
}
</script>