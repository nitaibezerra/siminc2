<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Lista de perfis vinculados ao usuário logado
$perfis = pegaPerfilGeral();

if(count(array_intersect($perfis, [PFL_SUBUNIDADE]))){
    # Perfil unidade são redirecionados para a tela de lista de Pré-Pis
    $urlPainel = 'proposta.php?modulo=principal/preplanointerno&acao=A';
    if($_REQUEST['exercicio']){
        $urlPainel .= '&exercicio='. $_REQUEST['exercicio'];
    }
    if($_REQUEST['req']){
        $urlPainel .= '&req='. $_REQUEST['req'];
    }
    if($_REQUEST['pliid']){
        $urlPainel .= '&pliid='. $_REQUEST['pliid'];
    }
    simec_redirecionar($urlPainel);
}

$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno();

switch ($_REQUEST['req']) {
    case 'carregarGraficoUnidade':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], null, true);
        die;
    case 'carregarGraficoDireta':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')"], true);
        die;
    case 'carregarGraficoCgconCogep':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"], true);
        die;
    case 'trocaAbaGrafico':
        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;        
    case 'carregarListaAcompanhamentoPis':
        if ($_REQUEST['exportar_xls']){
            header( 'Content-Type: application/vnd.ms-excel' );
            header( 'Expires: 0' );
            header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
            header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );
        }
        $filtros = array(
            "suo.prsano = '". $_SESSION['exercicio']. "'",
            'suo.unofundo = FALSE',
        );
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
        if($listaSubUnidadeUsuario){
            $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
        }

        $tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
        $aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo);
        include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");
        die;  
    case 'exportar-xls':
        header( 'Content-Type: application/vnd.ms-excel' );
        header( 'Expires: 0' );
        header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
        header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );
        echo $_REQUEST['tabela'];
        die;
}

$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}

$tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
$aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>
<style>
.gray-bg {
    background-color: #ffffff;
}
.wrapper.wrapper-content.animated.fadeInRight {
    background: #e5e5e5;
}
</style>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
</div>
<input type="hidden" id="tipo" value="Normal">
<div class="tabs-container tabs">
    <ul class="nav nav-tabs prodTabs">
        <li class="active"><a data-toggle="tab" href="#tab-normal" tipo="Normal" data-url="?modulo=inicio&acao=C&req=trocaAbaGrafico&tipo=Normal"> Normal</a></li>
        <li class=""><a data-toggle="tab" href="#tab-expansao" tipo="Expansao" data-url="?modulo=inicio&acao=C&req=trocaAbaGrafico&tipo=Expansao"> Expansão</a></li>
    </ul>
    <div class="tab-content">
        <div id="tab-normal" class="tab-pane active">
            <?php include_once(APPRAIZ . "proposta/modulos/principal/montaGraficoNormal.inc");?>                      
        </div>
        <div id="tab-expansao" class="tab-pane">
            <?php include_once(APPRAIZ . "proposta/modulos/principal/montaGraficoExpansao.inc");?>   
        </div>
    </div>
</div>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
            <i class="fa fa-file-excel-o"></i> Exportar XLS
        </button>
    </div>
</div>
<div id="listaAcompanhamentoPis">
    <?php include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");?>
</div>

<script>
    $(document).ready(function(){

        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");
            var tipo = $(this).attr("tipo");
            $("#tipo").val(tipo);
            $('#listaAcompanhamentoPis').load('?modulo=inicio&acao=C&req=carregarListaAcompanhamentoPis&tipo='+tipo);
            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;

                // ajax load from data-url
                $(href).load(url, function(result){
                    pane.tab('show');
                });
            } else {
                $(this).tab('show');
            }
        });
        
        $("#btn-exportar-xls").click(function(){
            window.open('?modulo=inicio&acao=C&req=carregarListaAcompanhamentoPis&tipo='+$("#tipo").val()+'&exportar_xls=1');
        });
        
    });

</script>

