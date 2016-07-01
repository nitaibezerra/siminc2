<?php 

function criaGrafico($id = 1,$label = "Label",$tipo = "linha"){
	if($label){
		$grafico = "<fieldset>";
		$grafico.= "<legend>$label</legend>";
	}
	$grafico.= "<div id=\"grafico_indicador_$id\"></div>";
	if($label)
		$grafico.= "</fieldset>";
	$grafico.="<script type=\"text/javascript\">
					swfobject.embedSWF(\"/includes/open_flash_chart/open-flash-chart.swf\", \"grafico_indicador_$id\", \"100%\", \"200\", \"9.0.0\", \"expressInstall.swf\", {\"data-file\":\"geraGrafico.php?tipo=$tipo\",\"loading\":\"Carregando gráfico...\"} );
				</script>";
	
	echo $grafico;
}

function criaTabelaGraficos(){?>
	<style>
		.cursor_link{cursor:pointer;}
	</style>
	<input type="hidden" id="parametros[hdn_grafico]" name="parametros[hdn_grafico]" value="" />
	<input type="hidden" id="parametros[hdn_label]" name="parametros[hdn_label]" value="" />
	<input type="hidden" id="parametros[hdn_tipo]" name="parametros[hdn_tipo]" value="" />
	<input type="hidden" id="funcao" name="funcao" value="exibeTabelaGrafico" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width=50% class="cursor_link" onclick="exibeGrafico(1,'Demandas','barra')" >
			<?php 
			criaGrafico(1,"Demandas","barra");
			?>
		</td>
		<td class="cursor_link" onclick="exibeGrafico(2,'Demandas em Atraso','pizza')" >
			<?php 
			criaGrafico(2,"Demandas em Atraso","pizza");
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="cursor_link" onclick="exibeGrafico(3,'Demandas','linha')" >
			<?php 
			criaGrafico(3,"Demandas","linha");
			?>
		</td>
	</tr>
</table>
<script>
	d = document;
	function exibeGrafico(id,label,tipo){
		d.getElementById('parametros[hdn_grafico]').value = id;
		d.getElementById('parametros[hdn_label]').value = label;
		d.getElementById('parametros[hdn_tipo]').value = tipo;
		if(d.getElementById('parametros[hdn_grafico]').value)
			d.getElementById('form_grafico').submit(); 
	}
</script>
<?php }

function exibeTabelaGrafico($id,$label,$tipo){?>

	<input type="hidden" id="funcao" name="funcao" value="exibeTabelaGrafico" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td><h2 style="text-align: center;" ><? echo $label ?></h2>
			<fieldset>
				<legend>Filtros</legend>
				Filtros
			</fieldset>
			<fieldset>
				<legend>Gráfico</legend>
				<?php criaGrafico($id,false,$tipo); ?>
			</fieldset>
		</td>
	</tr>
</table>
<?php }
?>