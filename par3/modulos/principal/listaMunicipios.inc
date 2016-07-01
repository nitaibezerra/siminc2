<?php
/**
 * Lista de Municipios
 *
 * @category Lista
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 10000
 * @link     no link
 */
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';

// Municipio.
$_POST['itrid'] = 2;

echo $simec->title('Lista Municípios', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
		<?php
		$url = 'par3.php?modulo=principal/listaMunicipios&acao=A';
		echo $simec->tab(criaAbaPar(), $url);
		?>
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h5>Filtros de pesquisa</h5>
			</div>
			<div class="ibox-content">
				<form method="post" name="formulario" id="formulario">
					<input type="hidden" name="pesquisa" value="1"> <input
						type="hidden" id="exporta" name="exporta" value="false">
					<div class="form-group">
                        <div class="row">
                            <div class="col-md-6">
    						<?php
                            $mundescricao = simec_htmlentities($_POST['mundescricao']);

                            $arrAttr = array(
                                        'placeHolder' => 'Municipio',
                                        'maxlength'   => '100',
                                       );
                            echo $simec->input(
                                'mundescricao', null, $mundescricao, $arrAttr
                            );
                            ?>
                            </div>
                            <div class="col-md-6">
    						<?php
                            $sql = Territorios_Model_Estado::pegarSQLSelect($_POST);

                            $arrAttr = array('data-placeholder' => 'Estado');
                            echo $simec->select(
                                'estuf', null, $_POST['estuf'], $sql, $arrAttr
                            );
                            ?>
                            </div>
                        </div>
					</div>
					<div class="form-group">
						<input class="btn btn-primary" type="submit" name="pesquisar"
							value="Pesquisar" />
					</div>
				</form>
			</div>
		</div>
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h5>Lista de municípios</h5>
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
$(document).ready( function(){
	$( "input[name='municipio']" ).focus();
	$('#formulario').submit(function() {
// 		$("#grupo option").attr("selected", "selected").trigger('liszt:updated');
    });

});

function dadosUnidade( inuid ) {
	var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid;
    $(location).attr('href', url);
}
</script>