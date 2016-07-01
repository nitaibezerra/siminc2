<?php
require_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentos.class.inc";
require_once APPRAIZ . "gestaodocumentos/classes/Solicitante.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";

if($_REQUEST['requisicao']=='pesquisar'){
    header('Content-Type: text/html; charset=iso-8859-1');
    pesquisarTarefa($_POST);
    exit();
}

#Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '');

unset($_SESSION['arSolicitante']);
unset($_SESSION['tarid']);
unset($_SESSION['_tartarefa']);
unset($_SESSION['dados_tarefa']);

if($_GET['taridExcluir']) {
    $obTarefa = new GestaoDocumentos();
    $obTarefa->excluir($_GET['taridExcluir']);
    $obTarefa->commit();
    unset($_GET['taridExcluir']);
    unset($obTarefa);
    $db->sucesso("principal/listaTarefas");
    die;
}

if( $_REQUEST['vListFss'] == 'S' ){
    $_REQUEST = $_SESSION['GD_filtro'];
}

?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/slider/slider.css"></link>
<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="./js/gestaodocumentos.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

$1_11(document).ready(function(){
    //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
    $1_11(window).unload(
        $1_11('.chosen-select').chosen()
    );
});

jQuery.noConflict();

function abrirDemandasRecebidas(){
    document.getElementById('id_popup_alerta').style.display='';
}

function excluirTarefaAtividade(tarid) {
    if(confirm('Deseja excluir este registro')) {
    	window.location.href = "/gestaodocumentos/gestaodocumentos.php?modulo=principal/listaTarefas&acao=A&taridExcluir=" + tarid;
        return true;
    } else {
        return false;
    }
}

function exibirHistorico( docid ){
    var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid='+docid;
    window.open( url, 'alterarEstado', 'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no' );
}

function pesquisarTarefa(){
    divCarregando();
    jQuery.ajax({
        type: 'POST',
        url: 'gestaodocumentos.php?modulo=principal/listaTarefas&acao=A&requisicao=pesquisar',
        data: jQuery('#formulario').serialize(),
        async: false,
        success: function(data) {
            jQuery("#div_tarefas").html(data);
            divCarregado();
        }
    });
}

function slicerSubmitSituacao() {
    var objSliderValor	= document.getElementById( 'valorSlider' );
    var objSliderStatus	= document.getElementById( 'situacaoSlider' );
    var strIdSpan       = objSliderStatus.id_tarefa;
    var objSpan		= document.getElementById( strIdSpan );

    var strStatus	= document.getElementById( "situacaoSlider" ).options[ objSliderStatus.value - 1 ].innerHTML;
    var intPercentual	= objSliderValor.value;

    atualizaBarraStatusSituacao( strIdSpan , strStatus , objSliderStatus.value  , intPercentual, 'S' )
    removeSlider();
}

function atualizaBarraStatusSituacao(intBarraStatusId, strStatus, intStatus, intPercentual, boMontaShowModal ) {
    var id = intBarraStatusId.replace('td_', '');
    var data = 'tipo=atualiza_barra_status&id='+id+'&codstatus='+intStatus+'&percentual='+intPercentual+'';
    var aj = new Ajax.Request('ajax.php',  {
        method: 'post',
        parameters: data,
        onComplete: function(r){
            if(r.responseText){
                jQuery('#sit'+id).load('gestaodocumentos.php?modulo=principal/listaTarefas&acao=A #sit'+id);
            } else {
                alert("Erro ao atualizar a Situação.");
            }
        }
    });
}

function posicionaSliderSituacao(tarid) {
    var resp;
    var data = 'funcao=verificaSeConcluido&tarid='+tarid;
    var aj = new Ajax.Request('_funcoes.php',{
        method: 'POST',
        parameters: data,
        asynchronous: false,
        onComplete: function(r){
            resp = pegaRetornoAjax('<resp>', '</resp>', r.responseText, true);
        }
    });

    var verificaPermicao = verificaSePermitido();
    var boExibeSituacao = true;

    if( verificaPermicao == 'S' ){
        boExibeSituacao = true;
    } else {
        alert('Usuario sem permissão para alterar a Situação');
        boExibeSituacao = false;
        return false;
    }

    if( resp == 'S' ){
        alert('A Demanda está em Processo finalizão/Arquivamento não pode ser alterada!');
        boExibeSituacao = false;
        return false;
    }

    if(boExibeSituacao){
        var left = jQuery("#sit"+tarid).offset().left;
        var top  = jQuery("#sit"+tarid).offset().top;

        var objSlider       = document.getElementById('sliderDiv');
        var objSliderValor  = document.getElementById('valorSlider');
        var objSliderStatus = document.getElementById('situacaoSlider');

        var intValor        = jQuery("#percentual"+tarid).val();
        var intSelectValue  = jQuery("#status"+tarid).val();
        var strIdSpan       = tarid;

        objSlider.style.position = "absolute";
       	objSlider.style.left = left;
        objSlider.style.top = top;
        objSlider.style.display = "block";

        objSliderValor.value = intValor;
        objSliderStatus.value = intSelectValue;
        objSliderStatus.status = intSelectValue;
        objSliderStatus.id_tarefa = strIdSpan;
        objSliderValor.onchange();
    }
}

function resetFromulario(){
    jQuery.each(jQuery("input[type=text], select, textarea"), function(i,v){
        if( jQuery(this).attr('type') != 'radio' ){
            jQuery(v).val('');
        }
    });
}


function verificaSePermitido(){
    return '<?php echo $_SESSION['gt_doc']['perfilHabilitado']; ?>';
}
</script>
<style>
    .popup_alerta{
        width:900px;
        height:500;
        position:absolute;
        z-index:999;
        top:54%;
        left:50%;
        margin-top:-285;
        margin-left:-450;
        border: solid 2px black;
    	background-color:#FFFFFF;
	}
</style>

<form action="" method="POST" name="formulario" id="formulario">
    <input type="hidden" name="tarid" id="tarid" value="" >
    <input type="hidden" name="tipo" id="tipo" value="filtroArvore" />
    <table align="center" border="0" class="tabela" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="30%">Número SIDOC:</td>
            <td>
                <?PHP
                    $filtrosidoc = $_SESSION['GD_filtro']['filtrosidoc'];
                    echo campo_texto('filtrosidoc', 'N', '', 'Número SIDOC', 54, 20, '#####.######/####-##', '', '', '', '', 'id="filtrosidoc"', '', $filtrosidoc);
                ?>
            </td>
            <td width="25%" rowspan="11" valign="top" align="left">
                <fieldset>
                    <legend>Legendas</legend>
                    <table align="center" border="0" class="tabela" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
                        <tr>
                            <td width="5%"><img src="../imagens/alterar.gif" style="vertical-align: middle"></td><td><b>- Altera a Demanda</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/excluir.gif" style="vertical-align: middle"></td><td><b>- Excluir a Demanda</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/anexo.gif" style="vertical-align: middle"></td><td><b>- A demanda contém Anexo</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/restricao.png" style="vertical-align: middle"></td><td><b>- A demanda contém Restrição</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/restricao_ico.png" style="vertical-align: middle"></td><td><b>- A demanda contém Reiteração</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/pd_urgente.JPG" style="vertical-align: middle"></td><td><b>- Demanda com prioridade: Urgente</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/pd_alta.JPG" style="vertical-align: middle"></td><td><b>- Demanda com prioridade: Alta</b></td>
                        </tr>
                        <tr>
                            <td width="5%"><img src="../imagens/pd_normal.JPG" style="vertical-align: middle"></td><td><b>- Demanda com prioridade: Normal</b></td>
                        </tr>
                    </table>
                </fieldset>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Identificação da Demanda (Título):</td>
            <td>
                <?PHP
                    $filtrotartitulo = $_SESSION['GD_filtro']['filtrotartitulo'];
                    echo campo_texto('filtrotartitulo', 'N', '', 'Identificação da Demanda', 54, 255, '', '', '', '', '', 'id="filtrotartitulo"', '', $filtrotartitulo);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo de Solicitante:</td>
            <td>
                <?PHP
                    $filtrotiposolicitante = $_SESSION['GD_filtro']['filtrotiposolicitante'];
                    $sql = "
                        SELECT  tpsid AS codigo,
                                tpsdsc AS descricao
                        FROM gestaodocumentos.tiposolicitante
                        ORDER BY tpsdsc
                    ";
                    $db->monta_combo("filtrotiposolicitante", $sql, 'S', 'Selecione...', '', '', '', 500, 'N', 'filtrotiposolicitante', false, $filtrotiposolicitante, 'Tipo de Solicitante');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Orgão: </td>
            <td>
                <?PHP
                    $filtroorgao = $_SESSION['GD_filtro']['filtroorgao'];
                    $sql = "
                        SELECT  ogsid AS codigo,
                                ogsdsc AS descricao
                        FROM gestaodocumentos.orgaosolicitante
                        ORDER BY ogsdsc
                    ";
                    $db->monta_combo("filtroorgao", $sql, 'S', 'Selecione...', '', '', '', 500, 'N', 'filtroorgao', false, $filtroorgao, 'Orgão', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Setor de Origem:</td>
            <td>
                <?php
                    $filtrounaidsetororigem = $_SESSION['GD_filtro']['filtrounaidsetororigem'];
                    $sql = "
                        SELECT  unaid as codigo,
                                unasigla||' - '|| unadescricao as descricao
                        FROM gestaodocumentos.unidade 
                        ORDER BY unadescricao
                    ";
                    $db->monta_combo("filtrounaidsetororigem", $sql, 'S', 'Selecione...', '', '', '', 500, 'N', 'filtrounaidsetororigem', false, $filtrounaidsetororigem, 'Setor de Origem');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Responsável pela Demanda:</td>
            <td>
                <?php
                    $filtrousucpfresponsavel = $_SESSION['GD_filtro']['filtrousucpfresponsavel'];
                    $sql = "
                        SELECT  DISTINCT u.usucpf as codigo,
                                u.usunome as descricao
                        FROM seguranca.usuario AS u

                        LEFT JOIN seguranca.perfilusuario AS p ON p.usucpf = u.usucpf

                        WHERE p.pflcod IN (" . PERFIL_GDOCUMENTO_APOIO . "," . PERFIL_GDOCUMENTO_COORDENACAO_GERAL . "," . PERFIL_GDOCUMENTO_DIRETORIA . "," . PERFIL_GDOCUMENTO_ESQUIPE_TEC . "," . PERFIL_GDOCUMENTO_ADMINISTRADOR . ")
                        ORDER BY u.usunome
                    ";
                    $db->monta_combo('filtrousucpfresponsavel', $sql, 'S', "Selecione...", '', '', '', 500, '', 'filtrousucpfresponsavel', '', $filtrousucpfresponsavel, 'Responsável pela Demanda');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Prazo de Atendimento:</td>
            <td>
                <?php
                    $filtroprazoini = $_SESSION['GD_filtro']['filtroprazoini'];
                    echo campo_data2('filtroprazoini','N','S','','DD/MM/YYYY','','', $filtroprazoini, '', '', 'filtroprazoini' );
                    echo " até ";
                    $filtroprazofim = $_SESSION['GD_filtro']['filtroprazofim'];
                    echo campo_data2('filtroprazofim','N','S','','DD/MM/YYYY','','', $filtroprazofim, '', '', 'filtroprazofim' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data de Inclusão:</td>
            <td>
                <?php
                    $filtrotardtinclusao_1 = $_SESSION['GD_filtro']['filtrotardtinclusao_1'];
                    echo campo_data2('filtrotardtinclusao_1','N','S','','DD/MM/YYYY','','', $filtrotardtinclusao_1, '', '', 'filtrotardtinclusao_1' );
                    echo " até ";
                    $filtrotardtinclusao_2 = $_SESSION['GD_filtro']['filtrotardtinclusao_2'];
                    echo campo_data2('filtrotardtinclusao_2','N','S','','DD/MM/YYYY','','', $filtrotardtinclusao_2, '', '', 'filtrotardtinclusao_2' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação:</td>
            <td>
                <?php
                    $filtrosituacao = $_SESSION['GD_filtro']['filtrosituacao'];
                    $sql = "
                        SELECT  sitid AS codigo,
                                sitdsc AS descricao
                        FROM gestaodocumentos.situacaotarefa
                        ORDER BY codigo
                    ";
                    $arSituacao = $db->carregar($sql);

                    if( $filtrosituacao != ''){
                        $filtrosituacao = $filtrosituacao;
                    }else{
                        $filtrosituacao = array(1 => '1', 2 => '2', 3 => '5');
                    }

                    foreach ($arSituacao as $situacao) {
                        if (in_array($situacao['codigo'], $filtrosituacao)) {
                            $ckecked = "checked=\"checked\"";
                        }
                        echo "<input type=\"checkbox\" $ckecked name=\"filtrosituacao[]\" value=\"{$situacao['codigo']}\"> " . $situacao['descricao'];
                        $ckecked = "";
                    }
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" align="right">Assunto:</td>
            <td colspan="4">
                <?PHP
                    $filtrotemid = $_SESSION['GD_filtro']['filtrotemid'];
                    $sql = "
                        SELECT  temid AS codigo,
                                temdescricao AS descricao
                        FROM gestaodocumentos.tema
                        ORDER BY descricao
                    ";
                    $temid = $obTarefa->temid;
                    $db->monta_combo('filtrotemid', $sql, 'S', "Selecione...", '', '', '', 500, 'N', 'filtrotemid', false, $filtrotemid, 'Tema');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" align="right">Expediente:</td>
            <td>
                <?php
                    $filtrotpeid = $_SESSION['GD_filtro']['filtrotpeid'];
                    $sql = "
                        SELECT  tpeid AS codigo,
                                tpedsc AS descricao
                        FROM gestaodocumentos.tipoexpediente
                        ORDER BY tpedsc
                    ";
                    $tpeid = $obTarefa->tpeid;
                    $db->monta_combo('filtrotpeid', $sql, 'S', "Selecione...", '', '', '', 500, 'N', 'filtrotpeid', false, $filtrotpeid, 'Expediente');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo:</td>
            <td>
                <?php
                    $filtrotmdid = $_SESSION['GD_filtro']['filtrotmdid'];
                    $sql = "
                        SELECT  tmdid AS codigo,
                                tmddescricao AS descricao
                        FROM gestaodocumentos.tipomodalidade
                        ORDER BY tmddescricao
                    ";
                    $tmdid = $obTarefa->tmdid;
                    $db->monta_combo('filtrotmdid', $sql, 'S', "Selecione...", '', '', '', 500, 'N', 'filtrotmdid', false, $filtrotmdid, 'Tipo');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Expressão Chave:</td>
            <td>
                <?php
                    $filtroexpressaochave = $_SESSION['GD_filtro']['filtroexpressaochave'];
                    echo campo_texto('filtroexpressaochave', 'N', '', 'Expressão Chave', 54, 255, '', '', '', '', '', 'id="filtroexpressaochave"', '', $filtroexpressaochave);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Status Workflow:</td>
            <td>
                <?php
                    $filtrostatusworkflow = $_SESSION['GD_filtro']['filtrostatusworkflow'];
                    $sql = "
                        SELECT  esdid AS codigo,
                                esddsc AS descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = " . FUXO_GESTAO_DOCUMENTOS . "
                    ";
                    $db->monta_combo('filtrostatusworkflow', $sql, 'S', "Selecione...", '', '', '', 500, 'N', 'filtrostatusworkflow', false, $filtrostatusworkflow, 'Status Workflow');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="3">
                <input type="button" name="pesquisar" id="pesquisar" value="Pesquisar" onclick="pesquisarTarefa();" />
                <!-- <input type="reset" name="limpar" id="limpar" value="Limpar Filtros" /> -->
                <input type="button" name="limpar" id="limpar" value="Limpar Filtros" onclick="resetFromulario();"/>
                <input type="button" name="mostrar" id="mostrar" value="Mostrar Demandas Recebidas" onclick="abrirDemandasRecebidas();" />
            </td>
        </tr>
    </table>
</form>

<br>

<div id="sliderDiv" style="z-index: 1000; width: 200px; *height: 97px; left: 2px; top: 2px; visibility: visible; display: none;">
    <div class="monthYearPicker" style="left: 167px; width: 35px;" id="minuteDropDown"></div>
    <div class="topBar" id="topBar" style="top: 150px;">
        <img onclick="removeSlider()" onmouseover="this.src = '../includes/JsLibrary/slider/images/close_over.gif'" onmouseout="this.src = '../includes/JsLibrary/slider/images/close.gif'" src="../includes/JsLibrary/slider/images/close.gif" style="position: absolute; right: 2px;" />
    </div>
    <div>
        <table cellspacing="1" width="100%">
            <tr>
                <td style="background-color: #E2EBED">Situação:</td>
                <td style="text-align:left">
                    <select onchange="alteraStatus(this)" id="situacaoSlider" style="border:1px; border-style:solid; border-color:black; width:100%; font-size:10px;">
                        <option value='1'>Não Iniciada</option>
                        <option value='2'>Em Andamento</option>
                        <option value='3'>Suspensa</option>
                        <option value='4'>Cancelada</option>
                        <option value='5'>Concluída</option>
                    </select>
                </td>
            </tr>
            <tr>
                <td style="background-color: #E2EBED">Andamento:</td>
                <td>
                    <span id="slider_target" style="width: 40px; position: relative; *left: -41px; top: 5px; height: 3px;"></span>&nbsp;
                    <input type="text" name="valor" id="valorSlider" value="200" style="font-size: 10px; width: 30px" readonly />%
                </td>
            	<script type="text/javascript" language="javascript">
               		form_widget_amount_slider('slider_target',document.getElementById( "valorSlider" ),70,0,100,"arredonda(document.getElementById( 'valorSlider' ))");
            	</script>
            </tr>
            <tr>
                <td colspan="2" style="background-color: #E2EBED">
                    <input type="button" onclick="slicerSubmitSituacao()" value="Ok" style="height:18px; font-size: 10px;"/>
                </td>
            </tr>
        </table>
    </div>
</div>

<?PHP
    $aryPerfil = pegarPerfil($_SESSION['usucpf']);

    if(in_array(PERFIL_GDOCUMENTO_ESQUIPE_TEC,$aryPerfil) || in_array(PERFIL_GDOCUMENTO_COORDENACAO_GERAL,$aryPerfil)){
        $aryWhereD[] = "usucpfresponsavel = '{$_SESSION['usucpf']}'";
        $aryWhereD[] = "tarrecebimento = 't'";

        $aryWhereC[] = "usucpfresponsavel = '{$_SESSION['usucpf']}'";
        $aryWhereC[] = "tarcomplexidade = 't'";

        $sqld = "
            SELECT  CASE WHEN char_length(trim(tarnumsidoc)) = 12 THEN  substr(tarnumsidoc,0,7) || '.' || substr(tarnumsidoc,7,4) || '-' || substr(tarnumsidoc,11,2)
                         WHEN char_length(trim(tarnumsidoc)) = 17 THEN substring(tarnumsidoc,0,6) || '.' || substr(tarnumsidoc,6,6) || '/' || substr(tarnumsidoc,12,4) || '-' || substr(tarnumsidoc,16,2)
                        ELSE ''
                    END AS tarnumsidoc,
                    tartitulo,
                    to_char(tardtinclusao,'DD/MM/YYYY') AS tardtinclusao,
                    CASE WHEN tarstatus = 'A' THEN 'Ativo' ELSE 'Inativo' END AS tarstatus,
                    CASE WHEN nvcid = 1 THEN 'Alto' WHEN nvcid = 2 THEN 'Médio' WHEN nvcid = 3 THEN 'Baixo' END AS nvcid
            FROM gestaodocumentos.tarefa

            ".(is_array($aryWhereD) ? ' WHERE '.implode(' AND ', $aryWhereD) : '')."
        ";
        $aryD = $db->carregar($sqld);

        $sqlc = "
            SELECT  CASE WHEN char_length(trim(tarnumsidoc)) = 12 THEN  substr(tarnumsidoc,0,7) || '.' || substr(tarnumsidoc,7,4) || '-' || substr(tarnumsidoc,11,2)
                         WHEN char_length(trim(tarnumsidoc)) = 17 THEN substring(tarnumsidoc,0,6) || '.' || substr(tarnumsidoc,6,6) || '/' || substr(tarnumsidoc,12,4) || '-' || substr(tarnumsidoc,16,2)
                        ELSE ''
                    END AS tarnumsidoc,
                    tartitulo,
                    to_char(tardtinclusao,'DD/MM/YYYY') AS tardtinclusao,
                    CASE WHEN tarstatus = 'A' THEN 'Ativo' ELSE 'Inativo' END AS tarstatus,
                    CASE WHEN nvcid = 1 THEN 'Alto' WHEN nvcid = 2 THEN 'Médio' WHEN nvcid = 3 THEN 'Baixo' END AS nvcid
            FROM gestaodocumentos.tarefa
            ".(is_array($aryWhereC) ? ' WHERE '.implode(' AND ', $aryWhereC) : '')."
        ";
        $aryC = $db->carregar($sqlc);
        
        $cabecalho = array("Nº SIDOC", "Identificação", "Data de Inclusão", "Status", "Nível");
        $alinhamento = array('','','center','center','center');
        $tamanho = array('10%','55%','5%','5%','5%');
?>
        <div id="id_popup_alerta" class="popup_alerta" style="height:530px; display: none;">
            
            <div id="fecha" style="width:100%; text-align:right; position: absolute; margin-top:0px;">
                <img onclick="document.getElementById('id_popup_alerta').style.display='none'" style="margin-top:5px; margin-right:5px; cursor:pointer" title="Fechar" src="../imagens/fechar.jpeg">
            </div>
            
            <div id="t_demanda" style="width:100%; text-align:center; text-decoration: underline; font-weight: bold; position: absolute; margin-top:8px; border-bottom: solid 1px black;"> DEMANDAS RECEBIDAS </div>

            <div id="g_demanda" style="width:100%; height:42%; margin-top:30px; border-bottom: solid 1px #E2EBED; overflow:auto;">
                <?php
                    monta_grid_demanda( $aryD, 'grid_demandas' );
                ?>
            </div>
            
            <div id="t_complexo" style="width:100%; height:15px; text-align:center; text-decoration: underline; font-weight: bold; position:absolute; top:265px; border-top:solid 1px black; border-bottom:solid 1px black;"> NÍVEL DE COMPLEXIDADE ALTERADO </div>

            <div id="g_complexo" style="width:100%; height:37%; position: absolute; top:290px; border-bottom: solid 1px #E2EBED; overflow:auto;">
                <?php
                    monta_grid_demanda( $aryC, 'grid_complexo' );
                ?>
            </div>

            <div id="botao_ok" style="width:100%; height:7%; text-align:center; position:absolute; top:495px; border-top:solid 1px black;">
                <input style="margin-top:5px; width:150px;" id="ok" type="button" onclick="document.getElementById('id_popup_alerta').style.display='none'" value="OK" name="ok">
            </div>

        </div>
<?PHP
    }
?>

<div id="div_tarefas">

    <?PHP pesquisarTarefa( $_REQUEST ); ?>

</div>
