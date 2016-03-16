<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<div id="fake-body">
    <div>
        <img src="/SPO_-_MPN_-_Asis_-_Descentralizar_-_MEC_CAPES_INEP.png" border="0" />
    </div>
</div>

<script type="text/javascript">
jQuery(document).ready(function(){
    setTimeout(function(){
        jQuery("#main").next("table").attr("style", "width:"+jQuery("#fake-body").width()+"px; height:45px; color:#667; background-color:#000;");
        jQuery("#menubackmenu1").attr("style", "position:absolute; top:83px; width:"+jQuery("#fake-body").width()+"px; height:28px;");
        jQuery(".barraAmbienteDivPrincipal").css("width", jQuery("#fake-body").width()+"px");
    },200);
});
</script>