
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
				<select multiple="multiple" id="estado" name="estado" onchange="javascript:Mapas.buscaEstadoForm( this, 'questoespontuais' );Mapas.atualizaLegenda( this, 'questoespontuais-legenda' )" class="multiselect">
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
						s.sqpcod,
						s.sqpdsc,
						s.sqpcor,
						count(qptid) as total
					from sase.situacaoquestoespontuais  s
					left join sase.questoespontuaispar q on q.sqpcod = s.sqpcod
					group by 1,2,3
					order by sqpcod ASC ";
				$lista = $db->carregar( $sql );
				foreach ($lista as $key => $value) {
					echo "<li ><table><tr><td><span style='background:" . $value['sqpcor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . $value['total'] . "</b>&nbsp;&nbsp;</td><td>" . $value['sqpdsc'] . "</td></tr></table></li>";
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
			Mapas.buscaEstadoForm( jQuery("#estado"), 'questoespontuais' );
			Mapas.atualizaLegenda( jQuery("#estado"), 'questoespontuais-legenda' );
		});
	});

</script>
<!-- /js -->


