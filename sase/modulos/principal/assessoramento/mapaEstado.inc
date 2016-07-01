
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->


<div style="clear:both"></div>

<div id="container">
	
	<div id="local-mapa">
		<div id="menu">
			<div style='float:left'>Estados:</div>
			<div style='float:left;margin-left:15px;'>
				<button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
				<select multiple="multiple" id="estado" name="estado" 
				onchange="javascript:Mapas.buscaEstadoForm( this, 'assessoramento-estado' );
				Mapas.atualizaLegenda( this, 'assessoramento-legenda-estado' )" class="multiselect">
					<?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql); 

					foreach ($estados as $key => $value) {
						echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
					}?>

				</select>
			</div>
			<div id="map_canvastxt"></div>
		</div>

		<div class="panel panel-default">
			<!-- <div class="panel-heading">Mapa</div> -->
			<div class="panel-body">
				<div id="directionsPanel" style="width: 300px"></div>
				<div id="map_canvas"></div>
			</div>
		</div>
	</div>

	<div id="legendaMapa">
		<div id="legendaMapaContainer">
			<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
			<ul>
				<?php
				$sql = "
					select 
						s.stacod,
						s.stadsc,
						s.stacor,
						count(a.aseid) as total 
					from sase.situacaoassessoramento s
					left join sase.assessoramentoestado a on a.stacod = s.stacod
					where stastatus = 'A'
					group by 1,2,3
					order by stacod ASC ";
				$lista = $db->carregar( $sql );
				foreach ($lista as $key => $value) {
					echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . $value['total'] . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
				}
				?>
			</ul>
		</div>
	</div>

</div>

<div style="clear:both"></div>

<div id="footer"></div>
<!-- /html -->


<!-- js -->
<script>

	jQuery('documento').ready(function(){
		Mapas.inicializar( '#map_canvas' );
	});

	jQuery('document').ready(function(){
		jQuery('#estado').multiselect({
	      numberDisplayed: 14,
	      id: 'estado'
	    });
	    jQuery("#estado-toggle").click(function(e) {
			e.preventDefault();
			multiselect_toggle(jQuery("#estado"), jQuery(this));
			Mapas.buscaEstadoForm( jQuery("#estado"), 'assessoramento-estado' );
			Mapas.atualizaLegenda( jQuery("#estado"), 'assessoramento-legenda-estado' )
		});
	});

</script>
<!-- /js -->


