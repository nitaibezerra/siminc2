<?php

$formEnviarNC = new Ted_Form_EnviarNotaCredito();
echo $formEnviarNC->showForm();

?>
<script type="text/javascript">
$(function(){
    $("#enviarNC").on("click", function() {
        if (!$("[name='proid[]']:checked").length) {
            bootbox.alert("É necessário marcar ao menos um item da Programa Orçamentária");
            return false;
        }
    });
});
</script>