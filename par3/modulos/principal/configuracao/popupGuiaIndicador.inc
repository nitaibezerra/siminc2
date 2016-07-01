<?php

$sql = "SELECT
            i.indid,
        	i.indnome as nome,
        	tpi.tpidsc as tipo,
        	reg.regdescricao as regionalizacao,
        	e.exodsc as eixo,
        	sec.secdsc as autarquia,
            ip.indidpainel as checked
        FROM
            painel.indicador  i
        INNER JOIN painel.eixo e ON e.exoid = i.exoid
        INNER JOIN painel.secretaria sec ON sec.secid = i.secid
        INNER JOIN painel.tipoindicador tpi ON tpi.tpiid = i.tpiid
        INNER JOIN painel.regionalizacao reg ON reg.regid = i.regid
        LEFT  JOIN par3.indicadorpainel ip ON ip.indidpainel = i.indid AND ip.indid = ".$_POST['indid']."
        WHERE
        --	i.indid = 6 AND
        	i.indstatus = 'A'
        ORDER BY
        	1,2,3,4,5";

global $db;
$dados = $db->carregar($sql);

$arr = Array();

?>

<div class="ibox-content">
	<form method="post" name="formulario" id="formulario"
		class="form form-horizontal">
		<input type='hidden' value='<?php echo $_POST['indid'];?>' id='indid' name='indid' />
		<input type='hidden' value='' id='indices' name='indices' />
		<input type='hidden' value='' id='vinculaIndicador' name='vinculaIndicador' />
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h5>Vincular Indicadores para <?php echo $_POST['indicador']; ?></h5>
			</div>
			<div class="ibox-content">
				<div id="editable_wrapper"
					class="dataTables_wrapper form-inline dt-bootstrap">
					<div class="row">
						<div class="col-sm-12">
							<div id="editable_filter" class="dataTables_filter"></div>
						</div>
					</div>
					<div class="row">
						<div class="col-sm-12">
							<table
								class="table table-striped table-bordered table-hover  dataTable"
								id="editable" role="grid" aria-describedby="editable_info">
								<thead>
									<tr>
										<th>Ação</th>
										<th>Nome do Indicador</th>
										<th>Tipo de Indicador</th>
										<th>Regionalização</th>
										<th>Eixo</th>
										<th>Autarquia Executora</th>
									</tr>
								</thead>
								<tbody>
        							<?php foreach( $dados as $dado ){ if($dado['checked']){ $arr[] = $dado['checked']; }?>
            							<tr class="gradeA odd" role="row">
										<td class="center"><input type="checkbox" name="indicadorPainel[]" value="<?php echo $dado['indid']; ?>" <?php echo $dado['checked'] ? 'checked' : ''; ?> ></td>
										<td class="sorting_1"><?php echo $dado['nome']; ?></td>
										<td><?php echo $dado['tipo']; ?></td>
										<td><?php echo $dado['regionalizacao']; ?></td>
										<td><?php echo $dado['eixo']; ?></td>
										<td><?php echo $dado['autarquia']; ?></td>
									</tr>
        							<?php } ?>
        						</tbody>
							</table>
						</div>
					</div>
				</div>
				<div class="form-group">
					<div class="col-lg-12 text-center">
						<button type="submit" class="btn btn-sm btn-success btn-lg">
							<span class="glyphicon glyphicon-search"> </span> Salvar
						</button>
					</div>
				</div>
			</div>
		</div>
	</form>
</div>

<script>

$(document).ready(function (){

	$('#vinculaIndicador').val('salvar');

	var linhas_selecionadas = [];

    $('#editable tbody').on('click', 'input[type="checkbox"]', function(e){

    	// Determino onde do array está esse id
    	var index = $.inArray(this.value, linhas_selecionadas);

    	// Se marcou o checkbox e ele não está no array, adiciona.
		if(this.checked && index === -1){
			linhas_selecionadas.push(this.value);
		// Se desmarcou e ele está no array, remove.
		} else if (!this.checked && index !== -1){
			linhas_selecionadas.splice(index, 1);
		}

		$('#indices').val( linhas_selecionadas );

		// Daí vai e funfa bonito!!! By McBenzi

    });
});
</script>
<?php die(); ?>