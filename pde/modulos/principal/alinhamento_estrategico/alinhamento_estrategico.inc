<?php
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once 'AlinhamentoEstrategico.class.php';

require APPRAIZ . 'includes/cabecalho.inc';
$alinhamento = new AlinhamentoEstrategico();
if ($_REQUEST['requisicao'] == 'carregaTema') {
    ob_clean();
    $alinhamento->carregarTemaPormeta($_REQUEST['mpneid']);
    die;
}

if ($_REQUEST['requisicao'] == 'carregaRoll') {
    ob_clean();
    $alinhamento->carregarMetas($_REQUEST['area'],$_REQUEST['temid']);
    die;
}
if ($_REQUEST['requisicao'] == 'carregarRollAE') {
    ob_clean();
    $alinhamento->carregarRollAE($_REQUEST['temid']);
    die;
}
if($_REQUEST['requisicao'] == 'carregarRollORC') {
    ob_clean();
    $alinhamento->carregarRollORC($_REQUEST['temid']);
    die;
}

if ($_REQUEST['requisicao'] == 'carregaTable') {
    ob_clean();
    $arId = explode('_', $_REQUEST['id']);
    if (is_array($arId)) {
        switch ($arId[0]) {
            case 'pne':
                $alinhamento->montaGridPNE($_REQUEST['id']);
                break;
            case 'ppa':
                $alinhamento->montaGridPPA($_REQUEST['id']);
                break;
            case 'pe':
                $alinhamento->montaGridPE($_REQUEST['id']);
                break;
            case 'obj':
                $alinhamento->montaGridOBJ($_REQUEST['id']);
                break;
            case 'ae':
                $alinhamento->montaGridAE($_REQUEST['id'],$_REQUEST['temid']);
                break;
            case 'orc':
                $alinhamento->montaGridORC($_REQUEST['id']);
                break;
            case 'pro':
                $alinhamento->montaGridPRO($_REQUEST['id']);
                break;
        }
    }
    die;
}
if ($_REQUEST['requisicao'] == 'carregaEstrategiasPNE') {
    ob_clean();
    $alinhamento->carregaEstrategiasPNE($_REQUEST['id']);
    die;
}
if ($_REQUEST['requisicao'] == 'carregaMetaPPA') {
    ob_clean();
    $alinhamento->carregaMetaPPA($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregarDesafiosPNE') {
    ob_clean();
    $alinhamento->carregarDesafiosPNE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregarObjetivosEstrategicosPNE') {
    ob_clean();
    $alinhamento->carregarObjetivosEstrategicosPNE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregaAcoesPDE') {
    ob_clean();
    $alinhamento->carregaAcoesPDE($_REQUEST['id']);
    die;
}
if  ($_REQUEST['requisicao'] == 'carregaOrcamentos') {
    ob_clean();
    $alinhamento->carregaOrcamentos($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregaProjetosPNE') {
    ob_clean();
    $alinhamento->carregaProjetosPNE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'buscarMetas') {
    ob_clean();
    $temid = $_REQUEST['temid'] == 0 ? null :$_REQUEST['temid'];
    $alinhamento->montarMetas($temid);
    die;
}
if($_REQUEST['requisicao'] == 'carregaDesafiosPE') {
    ob_clean();
    $alinhamento->carregaDesafiosPE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregaObjetivosEstrategicosPE') {
    ob_clean();
    $alinhamento->carregaObjetivosEstrategicosPE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregaMetasPNEPPA') {
    ob_clean();
    $alinhamento->carregaMetasPNEPPA($_REQUEST['id']);
    die;
}
if ($_REQUEST['requisicao'] == 'carregaAcoesPEPPA') {
    ob_clean();
    $alinhamento->carregaAcoesPEPPA($_REQUEST['id']);
    die;
}
//ORÇAMENTO PLANO PLURIANUAL
if ($_REQUEST['requisicao'] == 'carregaOrcamentosPPA') {
    ob_clean();
    $alinhamento->carregaOrcamentosPPA($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'carregaProjetosPPA') {
    ob_clean();
    $alinhamento->carregaProjetosPPA($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaGridDetalheAcaoAE') {
    ob_clean();
    $alinhamento->montaGridDetalheAcaoAE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaMetasAE') {
    ob_clean();
    $alinhamento->montaMetasAE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaPPAAE') {
    ob_clean();
    $alinhamento->montaPPAAE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaOEAE') {
    ob_clean();
    $alinhamento->montaOEAE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaOrcamentosAE') {
    ob_clean();
    $alinhamento->montaOrcamentosAE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaProjetosAE') {
    ob_clean();
    $alinhamento->montaProjetosAE($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaProjetosORC') {
    ob_clean();
    $alinhamento->montaProjetosORC($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaOrcamentosORC') {
    ob_clean();
    $alinhamento->montaOrcamentosORC($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaOEORC') {
    ob_clean();
    $alinhamento->montaOEORC($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaPPAORC') {
    ob_clean();
    $alinhamento->montaPPAORC($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaMetasORC') {
    ob_clean();
    $alinhamento->montaMetasORC($_REQUEST['id']);
    die;
}
if($_REQUEST['requisicao'] == 'montaGridDetalheAcaoORC') {
    ob_clean();
    $alinhamento->montaGridDetalheAcaoORC($_REQUEST['id']);
    die;
}


?>
<style>
    .titulo{
        font-size: 12px; text-align:center; font-weight:bold;background-color: #E4D779 !important;
    }
    .tituloAno{
        text-align:center; font-weight:bold; background-color: #E4D779;
    }
    .goal-item:hover{opacity:1;border:1px solid #273A4A;color: #FFFFFF;background-color: #273A4A;}
    .goal-item{
        border: 1px solid #273A4A;
        color: #273A4A;
    }
    .btn.active{
        background-color: #273A4A;
        border: 1px solid #273A4A;
        color: #FFFFFF;
    }
    .wrapper-content{
        padding: 20px 10px 0px;
    }
    .ibox-content{
        padding: 15px 20px 0px 20px;
    }
    .textoDescricao{
        font-size: 12px;
        text-align: justify;
    }
    .tile4{
        text-align: justify;
    }
    .lazur-bg{
        background-color: #273A4A;
    }
    .clickMetasPNE{
        cursor: pointer;
    }
    .clickMetasPPA{
        cursor: pointer;
    }
    .clickObjetivoEstrategico{
        cursor: pointer;
    }
    .clickAcoesEstrategicas{
        cursor: pointer;
    }
    .roxo{
        cursor: pointer;
    }
    .marromClaro{
        cursor: pointer;
    }
</style>
<input type="hidden" id="areah" name="areah" value=""/>
<div class="wrapper wrapper-content">
    <div class="row noprint">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title" style="border-top: 3px solid #f8ac59">
                    <h5>ALINHAMENTO ESTRATÉGICO</h5>
                </div>
                <div class="ibox-content">
                    <div class="row" style="text-align: center !important;">
                        <?php $alinhamento->montarArea();?>
                        <?php $alinhamento->montarTema();?>
                        <div id="botao-metas">
                            <?php $alinhamento->montarMetas();?>
                        </div>
                        <br/>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div id="div_roll">
    
</div>
<div class="row" id="content_table" style="padding-bottom: 50px;">
    
</div>
<script>
jQuery(function ($) {

    //esconde botao "suporte"
    $('#temas-label-6').hide();

    //carrega meta1, caso area esteja vazio
    if (!$("#areah").val()) {
        $('#area-pne').addClass('active').click();
        $('#temas-label-1').addClass('active').click();
        $.ajax({
            url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
            type	: 'post',
            data	: 'requisicao=buscarMetas&temid=1',
            success	: function(e){
                $("#botao-metas").html(e);
                $('#metas_label_1').addClass('active').click();
            }
        });
    }

    $('body').on('click', '.metas' ,function(){
        desabilitarareas();
        $("#content_table").html('');
        $("#div_roll").html('');
        mpneid = $(this).find('input').attr('value');
        $('[id*="radio-meta"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
        $('#area-pne').parent().addClass('active');
        var idArea = 'pne';
        $("#areah").val(idArea);
        $.ajax({
            url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
            type	: 'post',
            data	: 'requisicao=carregaTema&mpneid='+mpneid,
            success	: function(e){
                $('[id*="temid-"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
                $('#temid-'+e).parent().addClass('active');
                idTema = e;
                //carregartema(idTema,idArea);
                carregarmeta(idArea+"_"+mpneid);
            }
        });
    });
    $('body').on('click', '.clickMetasPNE' ,function(){
        desabilitarareas();
        $("#content_table").html('');
        $("#div_roll").html('');
        param = this.id;
        mpneid = param.split('_')[1];
        $('[id*="radio-meta"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
        $('#area-pne').parent().addClass('active');
        var idArea = 'pne';
        $("#areah").val(idArea);
        $.ajax({
            url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
            type	: 'post',
            data	: 'requisicao=carregaTema&mpneid='+mpneid,
            success	: function(e){
                $('[id*="temid-"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
                $('.temid').addClass('nao_selecionado').removeClass('lazur-bg');
                $('#temid-'+e).parent().addClass('active');
                $('#metas_label_'+mpneid).addClass('active');                
                idTema = e;
                //carregartema(idTema,idArea);
                carregarmeta(idArea+"_"+mpneid);
            }
        });
    });
    $("body").on('click', '.temid', function(){
        $("#content_table").html('');
        $('[id*="radio-meta"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
        temid = $(this).find('input').attr('value');

        if ($("#areah").val() == 'pne'){
            $("#botao-metas").html('');
            $.ajax({
                url	: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                type	: 'post',
                data	: 'requisicao=buscarMetas&temid='+temid,
                success	: function(e){
                        $("#botao-metas").html(e);
                }
            });
        }else if ($("#areah").val() == 'ae'){
            $('#content_table').html('');
            $('#objetivos_metas_todos').val('ae');
            $('.objetivos_metas_todos').addClass('nao_selecionado').removeClass('marcado');
            $.ajax({
                    url     : 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregarRollAE&temid='+temid,
                    success	: function(e){
                            $('#div_roll').html(e);
                    }
            });
            $('.temid').addClass('nao_selecionado').removeClass('lazur-bg');
            $('.temid').addClass('nao_selecionado').removeClass('active');
            $(this).removeClass('nao_selecionado').addClass('lazur-bg');
        }else if ($("#areah").val() == 'orc'){
            $('#content_table').html('');
            $('#objetivos_metas_todos').val('ae');
            $('.objetivos_metas_todos').addClass('nao_selecionado').removeClass('marcado');
            $.ajax({
                    url     : 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregarRollORC&temid='+temid,
                    success	: function(e){
                            $('#div_roll').html(e);
                    }
            });
            $('.temid').addClass('nao_selecionado').removeClass('lazur-bg');
            $('.temid').addClass('nao_selecionado').removeClass('active');
            $(this).removeClass('nao_selecionado').addClass('lazur-bg');
        }else{
            if ($("#areah").val() == ""){
                alert('Selecione a área.');
                return false;
            }else{
                $.ajax({
                    url     : 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaRoll&temid='+temid+'&area='+$("#areah").val(),
                    success	: function(e){
                            $("#div_roll").html(e);
                    }
                });
            }
        } 
    });

    $("body").on('click', '.area', function(){
        desabilitarareas();
        $("#botao-metas").html('');
        $("#content_table").html('');
        $("#div_roll").html('');
        area = $(this).find('input').attr('value');
        $("#areah").val(area);
        $('[id*="radio-meta"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
        $('[id*="temid-"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
        $('[id*="temid-"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('lazur-bg');

        //mostra esconde botao "suporte"
        if ($("#areah").val() == 'obj') {
            $('#temas-label-6').show();
        }else{
            $('#temas-label-6').hide();
        }

        var dat = 'requisicao=buscarMetas&temid=1';
        var area_click = $(this).find('input').attr('id');
        $('#temas-label-1').addClass('active');
        $("#temas-label-1").click();
        /********************************************/
        $.ajax({
            url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
            type	: 'post',
            data	: dat,
            success	: function(e){
                $("#botao-metas").html(e);
                if( area_click  == 'area-pne'  ){
                    $('#metas_label_1').addClass('active').click();
                }
                if( area_click  == 'area-ppa'  ) {
                    $('#ppa_10').addClass('active').click();
                    setTimeout(function(){
                        $('#metas_189').addClass('active').click();
                    }, 500);
                }
                if( area_click  == 'area-obj'  ) {
                    $('#obj_3').addClass('active').click();
                }
                if( area_click  == 'area-ae'  ) {
                    $('#ae_61').addClass('active').click();
                }
                if( area_click  == 'area-orc'  ) {
                    $('#_BF01').addClass('active').click();
                }


            }
        });
    });

    $("body").on('click', '.objetivos_metas_todos', function(){
        $('#content_table').html('');
        arData = this.id.split('_');
        $('#objetivos_metas_todos').val(arData[1]);
        $('.objetivos_metas_todos').addClass('nao_selecionado').removeClass('marcado');
        $.ajax({
                url     : 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                type	: 'post',
                data	: 'requisicao=carregaTable&id='+this.id,
                success	: function(e){
                        $('#content_table').html(e);
                }
        });
        $('.objetivos_metas_todos').addClass('nao_selecionado').removeClass('lazur-bg');
        $(this).removeClass('nao_selecionado').addClass('lazur-bg');
    });
    $("body").on('click', '.objetivos_metas_ae', function(){
        $('#content_table').html('');
        arData = this.id.split('_');
        $('#objetivos_metas_ae').val(arData[1]);
        $('.objetivos_metas_ae').addClass('nao_selecionado').removeClass('marcado');
        carregarTableae(this.id);
        $('.objetivos_metas_ae').addClass('nao_selecionado').removeClass('lazur-bg');
        $(this).removeClass('nao_selecionado').addClass('lazur-bg');
    });
    $("body").on('click', '.objetivos_metas_orc', function(){
        $('#content_table').html('');
        arData = this.id.split('_');
        $('#objetivos_metas_orc').val(arData[1]);
        $('.objetivos_metas_orc').addClass('nao_selecionado').removeClass('marcado');
        carregarTableorc(this.id);
        $('.objetivos_metas_orc').addClass('nao_selecionado').removeClass('lazur-bg');
        $(this).removeClass('nao_selecionado').addClass('lazur-bg');
    });
        
    $("body").on('click', '.clickObjetivoEstrategico', function(){
        param = this.id;
        idTema = param.split('_')[0];
        idObeid = param.split('_')[1];
        $("#area-obj").click();
        $("#temas-label-"+idTema).click();
        setTimeout(function(){
            $("#obj_"+idObeid).click();
        }, 500);
    });
    $("body").on('click', '.clickAcoesEstrategicas', function(){
        param = this.id;
        idTema = param.split('_')[0];
        idAcao = param.split('_')[1];
        $("#area-ae").click();
        $("#temas-label-"+idTema).click();
        setTimeout(function(){
            $("#ae_"+idAcao).click();
        }, 500);
    });
    $("body").on('click', '.clickProjetos', function(){
        param = this.id;
        idTema = param.split('_')[0];
        idProjeto = param.split('_')[1];
        $("#area-pro").click();
        $("#temas-label-"+idTema).click();
        setTimeout(function(){
            $("#pro_"+idProjeto).click();
        }, 500);
    });
    $("body").on('click', '.clickMetasPPA', function(){
        param = this.id;
        idTema = param.split('_')[0];
        idObjetivosMetas = param.split('_')[1];
        idMeta = param.split('_')[2];
        tipo = param.split('_')[3];
        desabilitarareas();
        $("#botao-metas").html('');
        $("#content_table").html('');
        $("#div_roll").html('');
        $("#area-ppa").click();
        $("#temas-label-"+idTema).click();
        setTimeout(function(){
            $("#ppa_"+idObjetivosMetas).click();
            setTimeout(function(){
                $("#metas_"+idMeta).click();
            },500);
        }, 500);
        
    });
    $("body").on('click', '.iniciativas', function(){
            arIds = this.id.split('_');
            // Carrega Desafios PE
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaDesafiosPE&id='+arIds[1],
                    success	: function(e){
                            $('#div_desafios_pe').html(e);
                    }
            });

            // Carrega Objetivos Estratégicos PE
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaObjetivosEstrategicosPE&id='+arIds[1],
                    success	: function(e){
                            $('#div_objetivos_estrategicos_pe').html(e);
                    }
            });

            // Carrega Metas PNE e PPA
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaMetasPNEPPA&id='+arIds[1],
                    success	: function(e){
                            $('#div_metas_pne').html(e);
                    }
            });

            // Carrega Ações e Metas
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaAcoesPEPPA&id='+arIds[1],
                    success	: function(e){
                            $('#div_acoes_metas').html(e);
                    }
            });

            // Carrega Orçamentos
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaOrcamentosPPA&id='+arIds[1],
                    success	: function(e){
                            $('#div_orcamentos').html(e);
                    }
            });

            // Carrega Projetos
            $.ajax({
                url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                type	: 'post',
                data	: 'requisicao=carregaProjetosPPA&id='+arIds[1],
                success	: function(e){
                    $('#div_projetos_ppa').html(e);
                }
            });
            $('.iniciativas').addClass('nao_selecionado').removeClass('lazur-bg');
            $(this).removeClass('nao_selecionado').addClass('lazur-bg');
        });
    /*$('.objetivos_metas').live('click', function(){
        $('#td_content_table').html('');
        arData = this.id.split('_');
        $('#objetivos_metas').val(arData[1]);
        $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
        $(this).removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel3').html(' > '+delimitador($(this).html(), 25));
        $.ajax({
                url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
                type	: 'post',
                data	: 'requisicao=carregaTable&id='+this.id,
                success	: function(e){
                        $('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
                        $('#td_content_table').html(e);
                        if(arData[0] == 'pne') carregaTablePNE(arData[1]);
                }
        });
    });*/


});
function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}
function desabilitarareas(){
    $('[id*="area-pne"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
    $('[id*="area-ppa"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
    $('[id*="area-obj"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
    $('[id*="area-ae"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
    $('[id*="area-orc"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
    $('[id*="area-pro"]').not($(this)).removeAttr('checked').prop('checked', false).parent().removeClass('active');
}
function carregartema(idTema,idArea){
    $.ajax({
        url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type	: 'post',
        data	: 'requisicao=carregaRoll&temid='+idTema+'&area='+idArea,
        success	: function(e){
                $('#div_roll').html(e);
        }
    });
}
function carregarmeta(id){
    arData = id.split('_');
    $.ajax({
            url		: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
            type	: 'post',
            data	: 'requisicao=carregaTable&id='+id,
            success	: function(e){
                    $('#content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
                    $('#content_table').html(e);
                    if(arData[0] == 'pne') carregaTablePNE(arData[1]);
            }
    });
}
function carregarTableorc(id){
    //carregar as ações da AE
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaGridDetalheAcaoORC&id=' + id,
        success: function(e) {
            $('.acoesae').html(e);
        }
    });
    // Carregar metas AE
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaMetasORC&id=' + id,
        success: function(e) {
            $('#div_metas_pne').html(e);
        }
    });
    //Metas / Iniciativas PPA
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaPPAORC&id=' + id,
        success: function(e) {
            $('#div_objetivo_ppa').html(e);
        }
    });
    // carregar objetivos estrategicos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaOEORC&id=' + id,
        success: function(e) {
            $('#div_acoes_metas').html(e);
        }
    });
    // Carregar orcamentos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaOrcamentosORC&id=' + id,
        success: function(e) {
            $('#div_orcamentos').html(e);
        }
    });
    // Carregar projetos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaProjetosORC&id=' + id,
        success: function(e) {
            $('#div_projetos').html(e);
        }
    });
}
function carregarTableae(id){
    //carregar as ações da AE
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaGridDetalheAcaoAE&id=' + id,
        success: function(e) {
            $('.acoesae').html(e);
        }
    });
    // Carregar metas AE
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaMetasAE&id=' + id,
        success: function(e) {
            $('#div_metas_pne').html(e);
        }
    });
    //Metas / Iniciativas PPA
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaPPAAE&id=' + id,
        success: function(e) {
            $('#div_objetivo_ppa').html(e);
        }
    });
    // carregar objetivos estrategicos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaOEAE&id=' + id,
        success: function(e) {
            $('#div_acoes_metas').html(e);
        }
    });
    // Carregar orcamentos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaOrcamentosAE&id=' + id,
        success: function(e) {
            $('#div_orcamentos').html(e);
        }
    });
    // Carregar projetos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=montaProjetosAE&id=' + id,
        success: function(e) {
            $('#div_projetos').html(e);
        }
    });
    
}
function carregaTablePNE(id){
    // carrega estrategias (div principal)
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregaEstrategiasPNE&id=' + id,
        success: function(e) {
            $('#td_estrategias_pne').html(e);
        }
    });

    // Carrega metas PPA
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregaMetaPPA&id=' + id,
        success: function(e) {
            $('#td_objetivos_ppa').html(e);
        }
    });

    // Carrega Desafios (planejamento estrategico)
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregarDesafiosPNE&id=' + id,
        success: function(e) {
            $('#td_planejamento_estrategico').html(e);
        }
    });

    // Carrega Objetivos Estratégicos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregarObjetivosEstrategicosPNE&id=' + id,
        success: function(e) {
            $('#td_objetivo_estrategico_pne').html(e);
        }
    });

    //Carrega estrategias (açoes / metas)
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregaAcoesPDE&id=' + id,
        success: function(e) {
            $('#td_acoes_metas').html(e);
        }
    });

    // orcamentos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregaOrcamentos&id=' + id,
        success: function(e) {
            $('#td_orcamentos').html(e);
        }
    });

    // projetos
    $.ajax({
        url: 'estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A',
        type: 'post',
        data: 'requisicao=carregaProjetosPNE&id=' + id,
        success: function(e) {
            $('#td_projetos_pne').html(e);
        }
    });
}
function abreOrcamento(vacid){
    window.open('/planacomorc/planacomorc.php?modulo=sistema/tabelasapoio/popupImpressaoVinculacaoOrcamentaria&acao=A&vacid='+vacid,'blank','height=500,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}
function abreSolucao(solid){
    window.open('/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='+solid,'blank','height=500,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}
</script>