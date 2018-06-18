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

$mProposta = new Proposta_Model_Proposta();
$mPtres = new Monitora_Model_Ptres();
$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno();

$momento = $_POST['momento']? $_POST['momento']: 1000;

switch ($_REQUEST['req']) {
    case 'importar-siop':
        $mPtres->importarSiop($_SESSION['exercicio'], $momento);
        simec_redirecionar('?modulo=inicio&acao=C', 'success');
        die;
    case 'gerar-planilha':
        $mProposta->gerarPlanilhaImportacao($momento);
        die;
    case 'carregarGraficoUnidade':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], null, true);
//        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;
    case 'carregarGraficoDireta':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')"], true);
//        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;
    case 'carregarGraficoCgconCogep':
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"], true);
//        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;
    case 'trocaAbaGrafico':
//        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"], true);
        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;        
    case 'carregarListaAcompanhamentoPis':
        $filtros = array(
            "suo.prsano = '". $_SESSION['exercicio']. "'",
            'suo.unofundo = FALSE',
        );
        if($listaSubUnidadeUsuario){
            $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
        }
        $oPlanoInterno = new Pi_PlanoInterno();
        $tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
        $aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo);
        include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");
        die;        
}

$quantidadeSiop = $mPtres->recuperarQuantidadesSiop($momento);
$funcionaisSiop = $mPtres->recuperarFuncionaisSiop($momento);
$comparacaoSiop = $mPtres->recuperarComparacaoSiop($momento);
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
$oPlanoInterno = new Pi_PlanoInterno();
$tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
$aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
</div>
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
<div id="listaAcompanhamentoPis">
    <?php include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");?>
</div>

<script>
    $(document).ready(function(){

        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");
            var tipo = $(this).attr("tipo");
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
        
    });

</script>

