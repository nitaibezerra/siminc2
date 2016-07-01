<?php
$controllerQuestoes = new Par3_Controller_QuestoEsestrategicasEscolasCe();
?>
<div class="ibox-content">
<form method="post" name="formularioEscolas" id="formularioEscolas" class="form form-horizontal">

    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req" value="salvarEscolasCE"/>

    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Selecione as Escolas que tem CE implantado</h3>
    	</div>
    </div>

	<div class="row">
        <div class="col-md-12">
        	<div class="ibox-content">
            <?php $controllerQuestoes->listaEscolasCe($_REQUEST); ?>
            </div>
        </div>
	</div>
</form>
</div>
<script>
$(document).ready(function()
{
	$('.js-switch').change(function()
	{
		var qrpid = $(this).attr('qrpid');
		var perid = $(this).attr('perid');
		var entid = $(this).attr('entid');
		var check = $(this).attr('checked');
		if (check == 'checked') {
			$(this).removeAttr('checked');
    		check = null;
		} else {
			$(this).attr('checked', 'checked');
    		check = 'checked';
		}
		var param = '&req=salvarEscolaCE&qrpid='+qrpid+'&perid='+perid+'&entid='+entid+'&check='+check;
		$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: param,
       		async: false,
       		success: function(resp){
           		$('#total_escolas_ce').html(resp);
       		}
     	});
	});

});
</script>