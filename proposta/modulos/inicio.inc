<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Lista de perfis vinculados ao usuário logado
$perfis = pegaPerfilGeral();
$filtros =$filtroacompanhamento = $_SESSION['filtroacompanhamento'] = ($_POST? $_POST: $_SESSION['filtroacompanhamento']);
$filtroacompanhamento['esdid'] = $filtroacompanhamento['esdid']?$filtroacompanhamento['esdid']:0;

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
        $unidades='';
        if ($_REQUEST['tab']==2 || $_REQUEST['tab']==5){
            $unidades="suo.suocod not in ('420009', '420008')";
            $filtro[]="suo.unosigla = 'MINC'";
        }else if ($_REQUEST['tab']==3 || $_REQUEST['tab']==6){
            $unidades="suo.suocod in ('420009', '420008')";
            $filtro[]="suo.unosigla = 'MINC'";
        }
        $filtro[]="suo.prsano = '". $_SESSION['exercicio']. "'";
        $filtro[]='suo.unofundo = FALSE';
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='0'){
            $filtro["esdid"] = $_REQUEST['esdid'];
        }
        if ($unidades!=''){
            $filtro[]=$unidades;
        }
        $filtros =$filtro;
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
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='0'){
            $filtro = ["esdid" => $_REQUEST['esdid']];
        }else{
            $filtro = [];
        }
        $filtros =$filtro;
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], $filtros, true);
        die;
    case 'carregarGraficoDireta':
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='0'){            
            $filtro = ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')", "esdid" => $_REQUEST['esdid']];
        }else{
            $filtro = ['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')"];
        }
        $filtros =$filtro;
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], $filtros, true);
        die;
    case 'carregarGraficoCgconCogep':
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='0'){
            $filtro = ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')", "esdid" => $_REQUEST['esdid']];
        }else{
            $filtro = ['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"];
        }
        $filtros =$filtro;
        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], $filtros, true);
        die;
    case 'trocaAbaGrafico':
        include_once(APPRAIZ . "proposta/modulos/principal/montaGrafico".$_REQUEST['tipo'].".inc");
        die;        
    case 'carregarListaAcompanhamentoPis':
        $unidades='';
        if ($_REQUEST['tab']==2 || $_REQUEST['tab']==5){
            $unidades="suo.suocod not in ('420009', '420008')";
            $filtro[]="suo.unosigla = 'MINC'";
        }else if ($_REQUEST['tab']==3 || $_REQUEST['tab']==6){
            $unidades="suo.suocod in ('420009', '420008')";
            $filtro[]="suo.unosigla = 'MINC'";
        }
        $filtro[]="suo.prsano = '". $_SESSION['exercicio']. "'";
        $filtro[]='suo.unofundo = FALSE';
        if ($_REQUEST['esdid'] && $_REQUEST['esdid']!='0'){
            $filtro["esdid"] = $_REQUEST['esdid'];
        }
        if ($unidades!=''){
            $filtro[]=$unidades;
        }
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
        if($listaSubUnidadeUsuario){
            $filtro[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
        }
        $filtros =$filtro;
        $tipo = $_REQUEST['tipo']?$_REQUEST['tipo']:'Normal';
        $aPropostas = $mPrePlanoInterno->recuperarExecucaoOrcamentaria($filtros, $tipo);
        include_once(APPRAIZ . "proposta/modulos/principal/listaAcompanhamentoPI.inc");
        die;
}
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
$filtro[] = "suo.prsano = '". $_SESSION['exercicio']. "'";
$filtro[] = 'suo.unofundo = FALSE';
$filtro['esdid'] = $filtros['esdid'];
$filtros=null;
$filtros=$filtro;
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
<form id="filtropi" name="filtroacompanhamento" method="POST" role="form" class="form-horizontal">
<input name="requisicao" id="requisicao" value="" type="hidden">
<input name="tabAtiva" id="tabAtiva" value="Normal" type="hidden">    
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10 ibox-title">
        <h2>Painel de Acompanhamento <?php echo $simec->radio('esdid', null, $filtroacompanhamento['esdid'], array("0"=>"Todos", "1787" => "Aprovados"));?></h2> 
    </div>
    <div class="col-lg-2 ibox-title">
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
        <li class="<?php echo $_REQUEST['tabAtiva']<4?'active':'';?>"><a data-toggle="tab" href="#tab-normal" tipo="Normal" data-url="?modulo=inicio&acao=C&req=trocaAbaGrafico&tipo=Normal"> Normal</a></li>
        <li class="<?php echo $_REQUEST['tabAtiva']>3?'active':'';?>"><a data-toggle="tab" href="#tab-expansao" tipo="Expansao" data-url="?modulo=inicio&acao=C&req=trocaAbaGrafico&tipo=Expansao"> Expansão</a></li>
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
</form>
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
            var tab = $(this).data("tab");
            if ($(this).attr("tipo")!=$("#tipo").val()){
                if ($(this).attr("tipo")=='Expansao'){
                    tab=4;
                }else{
                    tab=1;
                }
            }
            $("#tipo").val(tipo);
            $("#tabAtiva").val(tab);
            $('#listaAcompanhamentoPis').load('?modulo=inicio&acao=C&req=carregarListaAcompanhamentoPis&tipo='+tipo+'&esdid='+$("input[name='esdid']").val()+"&tab="+tab);
            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;
                url += "&esdid="+$("input[name='esdid']").val();
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
        
        $("input[name='esdid']").change(function(){
            window.document.filtroacompanhamento.submit();
        });
        
    });

</script>

