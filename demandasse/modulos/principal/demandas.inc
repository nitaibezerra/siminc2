<?
include_once APPRAIZ . "demandasse/classes/Demanda.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
?>
<div class="col-lg-12">
    <div class="page-header">
        <h3 id="forms">Listar Demandas</h3>
    </div>

    <div>
        <?php
        $sql = "select dmdid, dmdtitulo, dmddsc from demandasse.demanda";

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(array('Título', 'Descrição'));
        $listagem->addAcao('edit', 'editarDemanda');
        $listagem->setQuery($sql);
        $listagem->render();
        ?>
    </div>
</div>

<script type="text/javascript">
    function editarDemanda(dmdid)
    {
        window.location = 'demandasse.php?modulo=principal/demandasformulario&acao=A&dmdid=' + dmdid;
    }
</script>