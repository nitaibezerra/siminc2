
<?php

$sql = " SELECT  ";

?>

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
				<select multiple="multiple" id="estado" name="estado" onchange="javascript:Mapas.buscaEstadoForm( this, 'assessoramento' );Mapas.atualizaLegenda( this, 'assessoramento-legenda' )" class="multiselect">
					<?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql); 
					
					foreach ($estados as $key => $value) {
						echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
					} ?>

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
					SELECT 
						s.stacod,
						s.stadsc,
						s.stacor,
						count(a.assid) as total 
					FROM sase.situacaoassessoramento s
					LEFT JOIN sase.assessoramento a on a.stacod = s.stacod
					WHERE stastatus = 'A'
					GROUP BY 1,2,3
					ORDER BY stacod ASC ";
				$lista = $db->carregar( $sql );
				foreach ($lista as $key => $value) {

					$sql = "
						SELECT count(d.docid) AS total
						FROM workflow.documento d
						INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = " . $value['stacod'] . "
						INNER JOIN sase.assessoramento a ON a.docid = d.docid
						" . (($estuf!=''&&count($estuf)>0)?" WHERE a.estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " ";
					$total = $db->pegaUm( $sql );

					echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($total!='')?$total:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
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
			Mapas.buscaEstadoForm( jQuery("#estado"), 'assessoramento' );
			Mapas.atualizaLegenda( jQuery("#estado"), 'assessoramento-legenda' )
		});
	});

</script>
<!-- /js -->


