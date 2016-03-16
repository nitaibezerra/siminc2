<?php

$oCriterioM = new Par3_Model_Criterio();
$dadoCriterio = $oCriterioM->recuperarCriterio($_POST['crtid']);

$oCriterioC = new Par3_Controller_Criterio();
$dados = $oCriterioC->recuperarDadosListaVinculacao($_POST['crtid']);

if( is_array($dados) ){
    foreach( $dados as $d ){
        if( $d['crvvinculo'] ){
            $vinculo = $d['crvvinculo'];
        }
    }

}

?>

<div class="ibox-content">
	<form method="post" name="formulario" id="formulario" class="form form-horizontal">
		<input type='hidden' value='<?php echo $_POST['crtid'];?>' id='crtid' name='crtid' />
		<input type='hidden' value='' id='vinculaCriterio' name='vinculaCriterio' />

		<?php
		echo $simec->title('Cadastrar regras de vinculação '.$stTitulo);

        echo "<br>";

		echo $simec->input('crtdsc', 'Critério', $dadoCriterio['crtdsc'], array('placeHolder' => 'Critério', 'maxlengh' => 1000, 'disabled'));

	    $arrAcao     = Array(  1=>"O componente é pré-requisito para o preenchimento dos selecionados",
	                           2=>"O componente bloqueia o preenchimento dos selecionados"
	                   );

	    echo $simec->select('crtvinc', 'Tipo de Vinculação', $vinculo, $arrAcao, array(''));
		?>

		<div class="ibox float-e-margins">
        	<div class="ibox-content">

        		<table class="table table-hover">
        			<thead>
        			<tr>
        				<th>Ação</th>
        				<th>Critério</th>
        			</tr>
        			</thead>
        			<tbody>
        			<?php foreach( $dados as $dado ){ ?>
						<tr>
    						<td><input type="checkbox" name="criterioVinculo[]" value="<?php echo $dado['crtid']; ?>" <?php echo $dado['checked'] ? 'checked' : ''; ?> ></td>
    						<td><?php echo $dado['crtdsc']; ?></td>
    					</tr>
					<?php } ?>
        			</tbody>
        		</table>

        	</div>

			<div class="form-group">
				<div class="col-lg-12 text-center">
					<button type="submit" class="btn btn-sm btn-success btn-lg">
						<span class="glyphicon glyphicon-search"> </span> Salvar
					</button>
				</div>
			</div>
		</div>
	</form>
</div>

<script>

$(document).ready(function (){

	$('#vinculaCriterio').val('salvar');

});

</script>
<?php die(); ?>