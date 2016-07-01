<?php
/**
 * Lista de Estados
 *
 * @category Lista
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 28-09-2015
 * @link     no link
 */
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';

// Municipio.
$_POST['itrid'] = array(1, 3);

echo $simec->title('Lista de Estados', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
		<?php
		$url = 'par3.php?modulo=principal/listaEstados&acao=A';
		echo $simec->tab(criaAbaPar(), $url);
		?>
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h5>Lista de Estados</h5>
				<div class="ibox-tools">
					<a class="collapse-link"> <i class="fa fa-chevron-up"></i>
					</a>
				</div>
			</div>
			<div class="ibox-content">
				<?php Par3_Controller_InstrumentoUnidade::lista(); ?>
			</div>
		</div>
	</div>
</div>
<script type="text/javascript">
function dadosUnidade( inuid ) {
	var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid;
    $(location).attr('href', url);
}
</script>