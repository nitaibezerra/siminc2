
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<script>
    function atualizaOrgaos(estuf){
        jQuery.ajax({
            url:'',
            type:'POST',
            data:{acao:'carrega_orgaos',estuf:estuf},
            success: function( resposta ){
                jQuery('#divOrgao').html( resposta );
                filtrar();
            }
        });
    }
</script>
<style>
.legendas_linhas{
	display:none;
}
</style>
<!-- /dependencias -->

<div style="clear:both"></div>

<div id="container">
	
	<div id="local-mapa">
		<div id="menu">
			<div style="float:left">
				<div style='float:left;width:85px;'>Estados:</div>
				<div style='float:left;margin-left:15px;'>
					<button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
					<select multiple="multiple" id="estado" name="estado" onchange="javascript:atualizaOrgaos(this.value);" class="multiselect">
						<?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ORDER BY estdescricao ASC ";$estados = $db->carregar($sql); 

						foreach ($estados as $key => $value) {
							echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
						}?>

					</select>
				</div>

				<div style="clear:both"></div>

				<div style='float:left;width:85px;'>Orgãos:</div>
				<div id="divOrgao" style='float:left;margin-left:15px;'>
					<select id="orgao" name="orgao" onchange="javascript:filtrar();" class="multiselect">
						<?php $sql = "SELECT orgid, orgdsc FROM sase.orgao ORDER BY orgid ASC ";$orgaos = $db->carregar($sql); 

						foreach ($orgaos as $key => $value) {
							echo "<option value='".$value['orgid']."'>".$value['orgdsc']."</option>";
						}?>

					</select>
				</div>

			</div>
			<div style="clear:both"></div>

			<div id="map_canvastxt" style="font-size:14px;heigth:90px;">
				<div style="float:left">&nbsp;</div>
			</div>
		</div>
		<div style="clear:both;height:10px;"></div>

		<div class="panel panel-default">
			<!-- <div class="panel-heading">Mapa</div> -->
			<div class="panel-body">
				<div id="directionsPanel" style="width: 300px"></div>
				<div id="map_canvas"></div>
			</div>
		</div>
	</div>

	<div id="legendaMapa" style="padding-top:130px;">
		<div id="legendaMapaContainer">
			<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
			<ul>
				<?php
//				$sql = "
//					select
//						m.mesdsc as mesdsc,
//						m.mescor as mescor,
//						mun.estuf as estuf,
//						o.orgdsc as orgdsc,
//						oes.orgid as orgid,
//						count(m.mesid) as total
//					from sase.mesoregiao m
//					left join sase.orgaoestado oes on oes.oesid = m.oesid
//					left join sase.orgao o on o.orgid = oes.orgid
//					left join sase.territorio t on t.mesid = m.mesid
//					join territorios.municipio mun on mun.muncod = t.muncod
//					where messtatus = 'A' and o.orgid = '1'
//					group by 1,2,3,4,5
//					order by o.orgdsc ASC
//				";
//				$lista = $db->carregar( $sql );
//				foreach ($lista as $key => $value) {
//					//echo "<li class='legendas_linhas legenda_" . $value['estuf'] . "_" . $value['orgid'] . "'><table><tr><td><span style='background:" . $value['mescor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . $value['total'] . "</b>&nbsp;&nbsp;</td><td>" . $value['mesdsc'] . "</td></tr></table></li>";
//                    echo "<li><table><tr><td><span style='background:" . $value['mescor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (int)$value['total'] . "</b>&nbsp;&nbsp;</td><td>" . $value['mesdsc'] . "</td></tr></table></li>";
//				}
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

	// function atualizaLegendas(){
	// 	jQuery('.legendas_linhas').css('display','none');
		
	// 	var arrayEstuf = jQuery('#estado').val();
	// 	var valueOrgao = jQuery('#orgao').val();

	// 	if( arrayEstuf != null ){
	// 		jQuery.each( arrayEstuf, function( indexEstuf, valueEstuf ){

	// 			jQuery('.legenda_'+valueEstuf+'_'+valueOrgao).css('display','block');

	// 		});
	// 	}
	// }

	function filtrar(){
		// atualizaLegendas();
            Mapas.buscaEstadoForm(jQuery('#estado'), 'organizacoesterritoriais');
        if(jQuery('#orgao').val() != null && jQuery('#estado').val() != null) {
            Mapas.atualizaLegenda(jQuery("#estado"), 'organizacoesterritoriais-legenda');
        }
	}

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
            if(jQuery('#orgao').val() != '' && jQuery('#estado').val() != null) {
                Mapas.buscaEstadoForm(jQuery('#estado'), 'organizacoesterritoriais');
                Mapas.atualizaLegenda(jQuery("#estado"), 'organizacoesterritoriais-legenda');
            }
		});
	});

</script>
<!-- /js -->


