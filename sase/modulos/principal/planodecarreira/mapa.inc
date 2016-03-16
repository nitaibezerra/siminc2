<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 15:35
 */

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

<div id="container">

    <div id="menu">
        <div style='float:left'>Estados:</div>
        <div style='float:left;margin-left:15px;'>
            <button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
            <select multiple="multiple" id="estado" name="estado" class="multiselect">
                <?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql);

                foreach ($estados as $key => $value) {
                    echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
                } ?>

            </select>
        </div>

        <div style='float:left; margin-left: 20px;'>Situação:</div>
        <div style='float:left;margin-left:15px;'>
            <select id="tpdid" name="tpdid" class="form-control">
                <option value="238">Plano de Carreira</option>
                <option value="240">Adequaçao</option>
            </select>
        </div>
        <div id="map_canvastxt"></div>
    </div>

    <div id="local-mapa">
<!--        <div class="col-lg-12" id="menu">-->
<!--            <div class=form-group>-->
<!--                <label for="estado" class="control-label col-lg-1">Estados:</label>-->
<!--                <div class="col-lg-4">-->
<!--                    <button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>-->
<!--                    <select multiple="multiple" id="estado" name="estado" class="multiselect form-control">-->
<!--                        --><?php //$sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql);
//
//                        foreach ($estados as $key => $value) {
//                            echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
//                        } ?>
<!--                    </select>-->
<!--                </div>-->
<!---->
<!--            </div>-->
<!--            <div class=form-group>-->
<!--                <label for="estado" class="control-label col-lg-2">Tipo de documentos:</label>-->
<!--                <div class="col-lg-2">-->
<!--                    <select id="tpdid" name="tpdid" class="form-control">-->
<!--                        <option value="238">Plano de Carreira</option>-->
<!--                        <option value="240">Adequaçao</option>-->
<!--                    </select>-->
<!--                </div>-->
<!---->
<!--            </div>-->
<!--        </div>-->

        <div class="panel panel-default">
            <div class="panel-body">
                <div id="directionsPanel" style="width: 300px"></div>
                <div id="map_canvas"></div>
            </div>
        </div>
    </div>
    <div id="legendaMapa" style="padding: 0 !important;">
        <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <ul>

            </ul>
        </div>
    </div>


</div>

<div style="clear:both"></div>

<div id="footer"></div>

<script>
    jQuery('documento').ready(function(){
        Mapas.inicializar('#map_canvas');
        Mapas.atualizaLegenda( jQuery("#estado"), 'planocarreira-municipio-legenda' );

        jQuery('#estado')
            .multiselect({
                numberDisplayed: 2,
                id: 'estado'
            })
            .change(function(){
                Mapas.buscaEstadoForm( jQuery("#estado"), 'planocarreira-municipio' );
                Mapas.atualizaLegenda( jQuery("#estado"), 'planocarreira-municipio-legenda' );
            });

        jQuery('#tpdid').change(function(){
            Mapas.buscaEstadoForm( jQuery("#estado"), 'planocarreira-municipio' );
            Mapas.atualizaLegenda( jQuery("#estado"), 'planocarreira-municipio-legenda' );
        });

        jQuery("#estado-toggle").click(function(e) {
            e.preventDefault();
            multiselect_toggle(jQuery("#estado"), jQuery(this));
            Mapas.buscaEstadoForm( jQuery("#estado"), 'planocarreira-municipio' );
            Mapas.atualizaLegenda( jQuery("#estado"), 'planocarreira-municipio-legenda' );
        });

    });
</script>