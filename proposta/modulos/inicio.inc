<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Lista de perfis vinculados ao usuário logado
$perfis = pegaPerfilGeral();
$filtroprepi = $_SESSION['filtroprepi'] = ($_POST? $_POST: $_SESSION['filtroprepi']);
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
    case 'lista-inicio-xls':
        $filtros = array(
            "suo.prsano = '". $_SESSION['exercicio']. "'",
            'suo.unofundo = FALSE',
            "esdid" => $_REQUEST['esdid']
        );
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
        if($listaSubUnidadeUsuario){
            $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
        }

        $tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
        $sql = $mPrePlanoInterno->montarSqlExecucaoOrcamentaria($filtros, $tipo);
        $listaFuncional = $db->carregar($sql);
//ver($listaFuncional, d);
        include_once APPRAIZ. "proposta/modulos/lista-inicio-xls.inc";
    case 'carregarGraficoUnidade':
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='null'){
            $filtros = ["esdid" => $_REQUEST['esdid']];
        }else{
            $filtros = [];
        }
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], $filtros, true);
        die;
    case 'carregarGraficoDireta':
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='null'){
            $filtros = ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')", "esdid" => $_REQUEST['esdid']];
        }else{
            $filtros = ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')"];
        }
//        ver($_REQUEST, $filtros,d);
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], $filtros, true);
        die;
    case 'carregarGraficoCgconCogep':
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='null'){
            $filtros = ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')", "esdid" => $_REQUEST['esdid']];
        }else{
            $filtros = ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"];
        }
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], $filtros, true);
        die;
    case 'trocaAbaGrafico':
        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;        
    case 'carregarListaAcompanhamentoPis':
        $unidades='';
        if ($_REQUEST['tab']==2){
            $unidades="suo.suocod not in ('420009', '420008')";
        }else if ($_REQUEST['tab']==3){
            $unidades="suo.suocod in ('420009', '420008')";
        }
        $filtros = array(
            "suo.prsano = '". $_SESSION['exercicio']. "'",
            'suo.unofundo = FALSE',
            "suo.unosigla = 'MINC'"
        );
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='null'){
            $filtros["esdid"] = $_REQUEST['esdid'];
        }
        if ($unidades!=''){
            $filtros[]=$unidades;
        }
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
        if($listaSubUnidadeUsuario){
            $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
        }
        
        $tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
        $aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo);
        include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");
        die;
}

$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE'
);
if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='null'){
    $filtros["esdid"] = $_REQUEST['esdid'];
}
//ver($filtros,d);
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
    <div class="col-lg-12 ibox-title">
        <h5>Filtro</h5>
        <div class="ibox-content">
            <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                <input name="requisicao" id="requisicao" value="" type="hidden">

                <?php 
                echo $simec->select('esdid[]', 'Situação', $filtroprepi['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => 268)));
                ?>

                <div class="form-group">
                    <div class="text-center">
                        <button class="btn btn-warning btn-limpar" type="button">
                            <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                        </button>
                        <button class="btn btn-primary btn-buscar" type="submit">
                            <span class="glyphicon glyphicon-search"></span> Buscar
                        </button>
                        <!--<a class="btn btn-small btn-warning" href="?modulo=principal/preplanointerno_form&acao=A">Novo</a>-->
                    </div>
                </div>

            </form>
        </div>        
    </div>
</div>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4 ibox-title">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-8 ibox-title">
        <div style="padding-top: 10px;" class="text-right">
            <?php if(!array_intersect($perfis, [PFL_CONSULTA])){ ?>
                <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                    <i class="fa fa-file-excel-o"></i> Exportar XLS
                </button>
            <?php }?>
        </div>
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

<div class="clearfix" style="margin-top: 10px;">&nbsp;</div>

<div id="listaAcompanhamentoPis">
    <?php include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");?>
</div>

<script>
    $(document).ready(function(){
        $(".btn-limpar").click(function(){
            $('#requisicao').val('limpar');
            $('#filtropi').submit();            
        });
        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");
            var tipo = $(this).attr("tipo");
            $("#tipo").val(tipo);
            var tab = $(this).data("tab");
            $('#listaAcompanhamentoPis').load('?modulo=inicio&acao=C&req=carregarListaAcompanhamentoPis&tipo='+tipo+'&esdid='+$("#esdid").val()+"&tab="+tab);
            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;
                url += "&esdid="+$("#esdid").val();
                // ajax load from data-url
                $(href).load(url, function(result){
                    pane.tab('show');
                });
            } else {
                $(this).tab('show');
            }
        });
        
        $("#btn-exportar-xls").click(function(){
            window.open('?modulo=inicio&acao=C&req=lista-inicio-xls&tipo='+ $("#tipo").val());
        });
        
    });

</script>

