<?php

if ($_GET['id']) {
    $docid = pegaDocid($_GET['id']);
    require_once APPRAIZ . 'includes/workflow.php';
    echo wf_desenhaBarraNavegacao($docid , array('docid' => $docid, 'pdctipo' => $pdctipo, 'pdcid' => $id));

    $perfilPermitido = array(PERFIL_CGO, PERFIL_SUPER_USUARIO);
    $pflcod = pegarPerfilAtual($_SESSION['usucpf']);

    if (HOMOLOGADO == pegarEstadoAtual($_GET['id']) && in_array($pflcod, $perfilPermitido)) {
        echo '<button class="btn btn-primary btn-sm btn-resend">Enviar <br />e-mail <br />à UO</button>';
    }
?>

<script type="text/javascript">
$(function(){

    /**
     * Ação para botão de reenviar e-mail a SEGEP
     **/
    $(".btn-resend").on("click", function(){
        if (confirm("Deseja realmente enviar e-mail à UO?")) {
            $.post(location.href+'&_url_redirect_='+location.href+'&resend=true', function(data){
                location.reload();
            });
        }
    });

    /**
     * Mensagem de alerta para estados Analise SPO tramita para Analise SEGEP
     * intercepta a tramitação para perguntar se deseja enviar
     */
    var aedid = [2995]
        , preSeletor = '#td_acao_'
        , $seletor;

    for (var i = 0; i < aedid.length; i++) {
        $seletor = $(preSeletor+aedid[i]);

        if ($seletor.length) {

            var el = $seletor.find("a")
              , action = $(el).attr("onclick");

            $(el).attr("onclick", "");
            $seletor.click(function() {
                if (confirm("Confirma envio de e-mail para SEGEP?")) {
                    eval(action);
                    return true;
                }
                return false;
            });
        }
    }
});
</script>

<?php

}

?>