<?php
// empreendimento || obra || orgao

$str = "<script>
                alert('A funcionalidade de Supervisão MI foi desativada. Voce será redirecionado para a tela de Evolução MI.'); 
                document.location.href = 'obras2.php?modulo=principal/listaEvolucaoMi&acao=A';
         </script>";

die($str);

verificaSessao( 'obra' );
$_SESSION['obras2']['sueid'] = '';

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];
$empreendimento = new Empreendimento( $empid );

function gravarFoto(){
	global $db;	
	extract($_REQUEST);	
	$sql = "INSERT INTO obras2.$tabela($campo, arqid) VALUES($id, $arqid)";	
	$db->executar($sql);
	$db->commit();
}

function excluirFoto(){
	global $db;
	extract($_REQUEST);	
	$status = $tabela == 'arquivoquestaosupervisao' ? 'aqsstatus' : 'arsstatus';
	$sql = "UPDATE obras2.$tabela SET 
                    $status = 'I'
                WHERE
                    $campo = $id
                    AND arqid = $arqid";
	$db->executar($sql);
	$db->commit();
}

if( $_REQUEST['ajax'] ){
    switch ( $_REQUEST['ajax'] ){
        case 'buscaEndereco':
            require_once APPRAIZ . "adodb/adodb.inc.php";
            require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
            require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
            require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

            $endereco = new Endereco( $empreendimento->endid );

            $arEnd = array(
                'endcep' => $endereco->endcep,
                'endlogradouro' => $endereco->endlog,
                'endnum' => $endereco->endnum,
                'endcom' => $endereco->endcom,
                'endbai' => $endereco->endbai,
                'mundescricao' => $endereco->getMunDescricao(),
                'muncod' => $endereco->muncod,
                'estuf' => $endereco->estuf
            );

            $arEnd = array_map('utf8_encode',$arEnd);					  
            echo simec_json_encode( $arEnd );					  
            die;
        default:
            $_REQUEST['ajax']();
            die;
    }
}

if ($_GET['acao'] == 'A') {
    $smiid = '';
    $_SESSION['obras2']['smiid'] = $smiid;
} else {

    $smiid = ($_REQUEST['smiid'] != '' ? $_REQUEST['smiid'] : $_SESSION['obras2']['smiid']);
    $_SESSION['obras2']['smiid'] = ($_REQUEST['smiid'] != '' ? $_REQUEST['smiid'] : $_SESSION['obras2']['smiid']);

    if (empty($smiid)) {
        die("<script>
            alert('Faltam parametros para acessar a tela!'); 
            window.location = '?modulo=principal/listaSupervisaoMI&acao=A';
         </script>");
    }

    $supervisaoMi = new SupervisaoMi($smiid);
    extract($supervisaoMi->getDados());
//    $endereco = new Endereco( $endid );
    $docid = pegaDocidSupervisaoMI( $supervisaoMi->smiid );

    function disabledEditAction($usucpf = null) {
        
        if (possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
            return false;
        }
        
        if ($usucpf != $_SESSION['usucpf']) {
            echo '<script type="text/javascript"> disabledEditAction(); </script>';
            return true;
        }
        return false;
    }
}

// Executa as funções da tela de acordo com suas ações
if ($_REQUEST["requisicao"] == 'salvar'){
    salvarSupervisaoMI();
    die();
}

if ($_REQUEST['ajaxFotosGaleria'] && ($_REQUEST['fotosSupervisao'] || $_REQUEST['fotosGaleria'])) {
    if (atualizarFotosVistoria()) {
        echo 'true';
    } else {
        echo 'false';
    }
    exit();
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$tecnologiaMi = new TecnologiaMi();
$tecnologiaMi->carregaPorObrid( $obrid );
$endereco = new Endereco($endid ? $endid : $empreendimento->endid);

$somenteLeitura = 'S';
$habilitado     = true;
if(possui_perfil(array(PFLCOD_CALL_CENTER))){
//    $somenteLeitura = 'S';
    $habilitado     = false;
}

$supMi = new SupervisaoMi();
$estado_doc_sup = $supMi->getDadosTramitacaoSupervisaoMi($docid, array('esd.esdid','esd.esddsc'));

//Estado da Supervisão: Validação - Quando sob Validação, bloqueia edição pra empresa mi gestor e empresa mi fiscal
if($estado_doc_sup['esdid'] == ESDID_MI_VALIDACAO && possuiPerfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL)) && !possuiPerfil(array(PFLCOD_SUPER_USUARIO))){
    echo '<script type="text/javascript"> disabledEditAction(); </script>';
}

//Estado da Supervisão: Concluído - Quando concluído, bloqueia edição para qqer usuário
if($estado_doc_sup['esdid'] == ESDID_MI_CONCLUIDO){
    echo '<script type="text/javascript"> disabledEditAction(); </script>';
}

if( $_REQUEST['abaAjax'] ){
    
    if (function_exists('disabledEditAction')) {
        disabledEditAction($usucpf);
    }

    $_SESSION['obras2']['abaAjax'] = $_REQUEST['abaAjax'];
    switch ($_POST['abaAjax']) {
        case 'dadosSupervisao':
            require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoMI_dado.inc";
            die;
        case 'localObra':
            require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoMI_local.inc";
            die;
        case 'cronograma':
            require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoMI_cronograma.inc";
            die;
        case 'questionario':
            require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoMI_questionario.inc";
            die;
        case 'fotos':
            require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoMI_foto.inc";
            die;
        break;
        case 'tramitacao':
            require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoMI_tramitacao.inc";
            die;
    }
} else {
    $_SESSION['obras2']['abaAjax'] = $_SESSION['obras2']['abaAjax'] ? $_SESSION['obras2']['abaAjax'] : 'dadosSupervisao';
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

echo cabecalhoObra($obrid);

monta_titulo( 'Cadastro de Medição MI', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<style type="text/css">
.div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
.div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
html>body .div_fotos li { height: 80px; line-height: 1.2em; }
.div_fotos{height:499px;overflow:auto;}
.div_fotos2{height:90px;overflow:auto;}
.draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
.nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
.draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
.f_selected{border: solid 1px lightgreen;background-color: lightgreen;}
.fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
.img_foto{z-index:2;}
.img_class{margin-top:5px;}
.img_class2{margin-top:-10px;}

.div_abas_ajax{width:99%;background-color: #FFFFFF;overflow:fixed;}

.divItemComposicao{
	width:100%;
	font-weight:bold;
}

.divDivisao{
	width:100%;
	margin-left:5px;
	font-weight:bold;
	padding:5px;
}

.divQuestao{
	width:100%;
	margin-left:10px;
	padding:5px;
}
.divQuestaoNivel1{
	width:100%;
}
.divQuestaoNivel2{
	width:100%;
}
.divQuestaoNivel3{
	margin-top:3px;
}
.divQuestaoNivel4{
	margin-top:3px;
}
.divQuestaoNivel4_none{
	margin-top:3px;
	display:none;
}
.divQuestaoNivel4_fieldset{
	width:98%
}
.divQuestaoNivel4_1{
	margin-top:3px;
}
.divQuestaoNivel4_2{
	margin-top:3px;
}
.divQuestaoNivel4_3{
	margin-top:3px;
	padding:2px 2px 2px 2px;
	border: 1px dashed #ccc;
	height: 70px;
}
.divQuestaoNivel3{
	margin-top:3px;
}
.divQuestaoNivel3_none{
	display:none;
	margin-top:3px;
}
.divQuestaoNivel3_1{
	font-weight:bold;
}

.divSubQuestao{
	padding-left: 20px;
	padding-top: 10px;
	padding-bottom: 5px;
}
.divSubQuestao_none{
	display: none;
}

.divSubItemQuestao{
	padding: 5px, 5px, 5px, 5px;
}
.divSubItemQuestao_none{
	display: none;
}

.divSubQuestaoNivel1{
	font-weight: bold;	
}
.divSubQuestaoNivel1_none{}

.divSubQuestaoNivel2{
	font-weight: bold;
}
.divSubQuestaoNivel2_none{}

.divSubQuestaoNivel3{
	padding-left: 30px;
}
.divSubQuestaoNivel3_none{
	display: none;	
}

.divSubQuestaoNivel4{
	padding-left: 30px;
	width: 90% 
}
.divSubQuestaoNivel4_none{
	display: none;	
}
</style>
<form id="formulario_supervisao_empresa" name="formulario_supervisao_empresa" method="post" enctype="multipart/form-data" action="">
	
	<input type="hidden" name="smiid"      id="smiid"      value="<?php echo $smiid ?>" />
	<input type="hidden" name="requisicao" id="requisicao" value="salvar" />
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
            <tr>
                <td colspan="2" style="heigth:600px;">
                    <div style="width:100%;height:600px;overflow:auto;">
                    <?php 
                    include(APPRAIZ."/includes/classes/AbaAjax.class.inc");

                    $abaAjax = new AbaAjax("abasAjax",false,false,false,false);

                    $arAba = array(
                                    array(  "descricao" => "Dados da Medição",
                                            "padrao"    =>  ($_SESSION['obras2']['abaAjax'] == 'dadosSupervisao' ? true : false)  ,
                                            "url"       => "obras2.php?modulo=principal/cadSupervisaoMI&acao={$_GET['acao']}",
                                            "parametro" => "abaAjax=dadosSupervisao"
                                    ),
                                    array(  "descricao" => "Local da Obra",
                                            "padrao"    => ($_SESSION['obras2']['abaAjax'] == 'localObra' ? true : false) ,
                                            "url" 	    => "obras2.php?modulo=principal/cadSupervisaoMI&acao={$_GET['acao']}",
                                            "parametro" => "abaAjax=localObra"
                                    ),
                                    array(  "descricao" => "Cronograma",
                                            "padrao"    => ($_SESSION['obras2']['abaAjax'] == 'cronograma' ? true : false) ,
                                            "url"       => "obras2.php?modulo=principal/cadSupervisaoMI&acao={$_GET['acao']}",
                                            "parametro" => "abaAjax=cronograma"
                                    ),
//                                    array(	 "descricao" => "Questionário",
//                                                    "padrao" 	 => ($_SESSION['obras2']['abaAjax'] == 'questionario' ? true : false) ,
//                                                    "url" 		 => "obras2.php?modulo=principal/cadSupervisaoMI&acao={$_GET['acao']}",
//                                                    "parametro"  => "abaAjax=questionario"
//                                    ),
                                    array(  "descricao" => "Fotos",
                                            "padrao"    => ($_SESSION['obras2']['abaAjax'] == 'fotos' ? true : false) ,
                                            "url"       => "obras2.php?modulo=principal/cadSupervisaoMI&acao={$_GET['acao']}",
                                            "parametro" => "abaAjax=fotos"
                                    )
                                    ,array(	"descricao" => "Tramitação",
                                            "padrao"    => ($_SESSION['obras2']['abaAjax'] == 'tramitacao' ? true : false) ,
                                            "url"       => "obras2.php?modulo=principal/cadSupervisaoMI&acao={$_GET['acao']}",
                                            "parametro" => "abaAjax=tramitacao"
                                    )
                             );

                    $abaAjax->criaAba($arAba,'div_abas_ajax');
                    ?>
                    </div>
                </td>
            </tr>
            <tr>
                <td bgcolor="#c0c0c0" colspan="2" align="center">
                    <?php if($habilitado && $somenteLeitura == 'S'){ ?>
                        <input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="enviaFormulario();">                        
                    <?php } ?>
                    <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/listaSupervisaoMI&acao=A';">
                </td>
            </tr>
	</table>    
</form>
<script type="text/javascript">

function disabledEditAction() {
    jQuery("select, input:text, input:radio, textarea, [value='Pesquisar'], [value='Salvar'], [name='add_fotos'], [name='btn_salvar_fotos']").attr("disabled", true);
    jQuery("#formulario_supervisao_empresa [title='Escolha uma Data']").attr("onclick", "");
    console.log("executed!!");
    return false;
}
    
jQuery('#endcep').val( mascaraglobal('##.###-###', jQuery('#endcep').val()) );

function ctrlSituacaoObra( sobid ){
    if ( sobid == <?php echo SITUACAO_OBRA_EM_EXECUCAO ?> ){
            jQuery('#situacaoObraSubPergunta').show();
    }else{
            jQuery('[name=sueacordo]:checked').attr('checked', false);
            jQuery('#situacaoObraSubPergunta').hide();
    }
}

var totalArq = new Array();

function addArquivo(qstid){
	totalArq[qstid] = (totalArq[qstid] ? totalArq[qstid] + 1 : 1);
	jQuery('<br>').appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
	jQuery('#div_arq_' + qstid).find('[type=file]:eq(0)')
                                   .clone()
                                   .val('')
                                   .attr('name', 'img_'+ qstid + '_' + totalArq[qstid])
                                   .appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
	jQuery('#totImg_' + qstid).val( totalArq[qstid] );
}

function inserirVistoriador(entid){
    var caminho_atual = '/obras2/obras2.php';
    if (entid){
        return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&validaCPFProfissionalEmrpesa=1&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }else{
        return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
}

function verificaEnd(opcao){
	if (opcao == 'S'){
		jQuery('[name*=endereco]').attr('readonly', 'readonly')
                                          .addClass('disabled')
                                          .val('');
		var endid = jQuery('#emp_endid').val();
		if ( endid ){
			var url = location.href;
			jQuery.ajax({
                            type	: "POST",
                            url		: url,
                            data	: {endid : endid, ajax : 'buscaEndereco'},
                            dataType: 'json',
                            success	: function (data){
                                        jQuery('#endcep').val( mascaraglobal('##.###-###', data.endcep) );
                                        jQuery('#endlogradouro').val( data.endlogradouro );
                                        jQuery('#endnum').val( data.endnum );
                                        jQuery('#endcom').val( data.endcom );
                                        jQuery('#endbai').val( data.endbai );
                                        jQuery('#mundescricao').val( data.mundescricao );
                                        jQuery('#muncod').val( data.muncod );
                                        jQuery('#estuf').val( data.estuf );
                                    }
                        });
		}
	}else{
		jQuery('[name*=endereco]').attr('readonly', '')
                                          .removeClass('disabled')
                                          .val('');
	}
}

function calculoSupervisao(){
    var objClass     = this;
    var obCalc       = new Calculo();
    var percentTotal = new Number(0);
    var itemEdicao        = '';
    var vlrItemAnterior   = '';
    var primeira_td       = '';

    this.managerEdificacao = function (obj, tipo, obrid){
        obj  = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaTotalSupervisao( obrid );
    }

    this.managerEtapa = function (obj, obrid, tipo){
        obj = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                                       .find('td:eq(1)')
                                       .html();
                               
        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );
        
        <?php 
            if($estado_doc_sup['esdid'] == ESDID_MI_VALIDACAO && possuiPerfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE))){
                echo 'this.managerJustificativa(obj, obrid);';
            }
            //this.managerJustificativa(obj, obrid);
        ?>
//        calculaNivelEdificacao( obrid );

        calculaTotalSupervisao( obrid );
    }
    
    this.managerJustificativa= function (obj, obrid){
        
        var primeira_td        = jQuery( obj ).parents('tr:eq(0)').find('td:eq(0)');
        var nome_item_edtitado  = primeira_td.text().trim();
        var input_item_spi      = 'undefined';
        var input_justificativa = 'undefined';
        var id_spiid            = 'undefined';
        
        primeira_td.find("input").each(function() {            
            if(this.name === 'ico_spiid['+obrid+'][]'){
                input_item_spi = this;
                id_spiid       = this.value;
            }            
            if(this.name === 'spiid_just['+id_spiid+'][]'){
                input_justificativa = this;
            }
        });
        
        var html_input_justificativa  = '<input type="hidden" name="spiid_just['+id_spiid+'][]" value="">';        
        var img_inserir_justificativa = '<img id="inserir_justificativa_'+id_spiid+'" title="Inserir Justificativa"    style="cursor:pointer; width:15px;" onclick="calculoSupervisao.mostraModalJustificativa(this, \'MostraModal\',\''+id_spiid+'\')" src="/imagens/obras/atencao_vermelho.png">'; 
        var img_ok_justificativa      = '<img id="ok_justificativa_'+id_spiid+'"      title="Justificativa Preenchida" style="cursor:pointer; width:15px;" onclick="calculoSupervisao.mostraModalJustificativa(this, \'EditaModal\',\''+id_spiid+'\')" src="/imagens/obras/atencao.png">';

        if(input_justificativa === 'undefined'){
            primeira_td.append(html_input_justificativa);
            if(jQuery('#inserir_justificativa_'+id_spiid).length === 0){
                primeira_td.append(img_inserir_justificativa); 
            }
            
            primeira_td.find("input").each(function() {
                if(this.name === 'spiid_just['+id_spiid+'][]'){
                    input_justificativa = this;
                }
            });
        }
        
        if(input_justificativa.value.trim() === '' && input_justificativa.length !== 0){
            alert('Preencha a justificativa da mudança dos valores do Item: \n' + nome_item_edtitado);
        }else{
            var tag_remove = '#inserir_justificativa_'+id_spiid;
            primeira_td.remove(tag_remove);
            if(jQuery('#ok_justificativa_'+id_spiid).length === 0){
                primeira_td.append(img_ok_justificativa);
            }
        }
        
    }
    
    this.mostraModalJustificativa = function (obj, acao, spiid){
        
        var primeira_td        = jQuery( obj ).parents('tr:eq(0)').find('td:eq(0)');
        var textarea_jus_tag   = '';
        var textarea_jus_text  = '';
        var nome_item_edtitado = primeira_td.text().trim();
        
        switch(acao){
            case 'MostraModal':
                textarea_jus_tag = '<center><textarea id="txt_just" cols="5" rows="3" id="text_jus_'+spiid+'" style="width:95%; margin-top: 10px;" > </textarea></center>';                
                break;
            case 'EditaModal':
                $(primeira_td).find("input").each(function() {
                    if(this.name === 'spiid_just['+spiid+'][]'){
                        textarea_jus_text = this.value;
                    }
                });
                textarea_jus_tag = '<center><textarea id="txt_just" cols="5" rows="3" id="text_jus_'+spiid+'" style="width:95%; margin-top: 10px;"> '+textarea_jus_text+' </textarea> </center>';
                break;
        }
        
        var div_close = '<div id="span_dialog_justificativa_titulo"    '+
                               '   style=" position: static;           '+
                               '           margin-right: 2;            '+
                               '           width: 100%;                '+
                               '           height: 25px;               '+
                               '           text-align: right;          '+
                               '           font-color: #fff;           '+
                               '           background-color:#5C9CCC" > '+
                               ' <center> <b>Justificativa de Mudança de Valores</b> </center> '+
                               '</div>';

        var div = '<div id="div_dialog_justificativa"            '+
                  '     style="display:  none;                   '+
                  '            background-color: #5C9CCC;        '+
                  '            width:    500;                    '+
                  '            height:   300;                    '+
                  '            position: absolute;               '+
                  '            box-shadow: 7px 7px 5px #888888;  '+
                  '            opacity: none;                    '+ 
                  '            filter:alpha(opacity=none);       '+
                  '           -khtml-opacity:1;                  '+ 
                  '            -moz-opacity:1;                   '+ 
                  '            -ms-filter:?alpha(opacity=none)?; '+
                  '            filter: progid:DXImageTransform.Microsoft.Alpha(opacity=none); '+
                  '            padding-top: 0;                   '+
                  '            overflow: auto;                   '+
                  '            z-index:  900; ">&nbsp;</div>     ';
          
        var div_background = '<div id="div_dialog_justificativa_bg"       '+
                             '     style=" background-color: #000;        '+ 
                             '             bottom: 0;                     '+ 
                             '             display: none;                 '+ 
                             '             left: 0;                       '+ 
                             '             opacity: 0.5;                  '+ 
                             '             filter:alpha(opacity=50);       '+
                             '            -khtml-opacity:.50;              '+ 
                             '            -moz-opacity:.50;               '+ 
                             '            -ms-filter:?alpha(opacity=50)?; '+
                             '             filter: progid:DXImageTransform.Microsoft.Alpha(opacity=0.5); '+
                             '             position: fixed; '+ 
                             '             right: 0;        '+ 
                             '             top: 0;          '+ 
                             '             z-index: 99;     '+ 
                             '            ">&nbsp;</div>    ';
        
        
        var img_ok_justificativa = '<img id="ok_justificativa_'+spiid+'" title="Justificativa Preenchida" style="cursor:pointer; width:15px;" onclick="calculoSupervisao.mostraModalJustificativa(this, \'EditaModal\',\''+spiid+'\')" src="/imagens/obras/atencao.png">';
        
        var btn_close  = '<input type="button" id="close_div_dialog_justificativa"  class="close_div_dialog_justificativa"  value="Fechar">';
        var btn_salvar = '<input type="button" id="salvar_div_dialog_justificativa" class="salvar_div_dialog_justificativa" value="Salvar">';
        var content    = '<div id="div_dialog_justificativa_conteudo" style="background-color: #fff; height: 80%; width: 100%;">'+
                         '<p style="margin-left: 3px"><b>Preencha a justificativa da mudança dos valores do Item:</b><br/>' + nome_item_edtitado + '</p>'+
                         ''+textarea_jus_tag+
                         '<br /><center>'+btn_salvar+'   '+btn_close+'</center></div>';
        
        if(jQuery("#div_dialog_justificativa_bg").length === 0){
            jQuery('body').append(div_background);
        }

        jQuery("#div_dialog_justificativa_bg").show();
        jQuery('body').append(div);
        
        jQuery("#div_dialog_justificativa").append( div_close );
        jQuery("#div_dialog_justificativa").append(content);
        jQuery("#div_dialog_justificativa").show();
        
        var divWidth  = 500;
        x = (window.innerWidth)/2 - (divWidth)/2;
        y = 50; 

        floatDiv("div_dialog_justificativa", x,y).floatIt();

        jQuery(".close_div_dialog_justificativa").click(function(){
            jQuery("#div_dialog_justificativa").remove();
            jQuery("#div_dialog_justificativa_bg").hide();
        });
        
        jQuery(".salvar_div_dialog_justificativa").click(function(){
            
            var txt_just = jQuery('#txt_just').val().trim();
            
            if(txt_just === ''){
                alert('Para salvar, preencha o campo.');
                return false;
            }

            primeira_td.find("input").each(function() {
                if(this.name == 'spiid_just['+spiid+'][]'){
                    this.value = txt_just;
                    var tag_remove = jQuery('#inserir_justificativa_'+spiid);
                    
                    tag_remove.remove();

                    if(jQuery('#ok_justificativa_'+spiid).length === 0){
                        primeira_td.append(img_ok_justificativa);
                    }
                    
                    jQuery("#div_dialog_justificativa").remove();
                    jQuery("#div_dialog_justificativa_bg").hide();
                }
            });
        });
    }

    this.managerDetalhamento = function (obj, obrid, icoid, tipo){

        obj = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                                       .find('td:eq(1)')
                                       .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);

        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaNivelEtapa( obrid, icoid );
//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }

    this.managerDetalhamentoFilho = function (obj, obrid, icoid, ditidPai, tipo){
        obj       = obj || '';
        tipo      = tipo || 'percentual';

        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }
        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaNivelDetalhamento( obrid, icoid, ditidPai );
//        calculaNivelEtapa( obrid, icoid );
//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }

    this.calculaPercentLinha = function ( obj, tipo, obrid ){
        var vlrItemObra = jQuery( obj ).parents('tr:eq(0)')
                                  .find('td:eq(1)')
                                  .html();
        // CALCULA: "Valor Executado" a partir do "(%) Supervisão"
        if ( tipo == 'percentual' ){
            var calcVlrItemSup = obCalc.operacao(vlrItemObra, obj.value, '*', 20);
            calcVlrItemSup = obCalc.operacao(calcVlrItemSup, 100, '/', 20);
            jQuery( obj ).parents('tr:eq(0)')
                    .find('td:eq(10) input')
                    .val( this.formatarNumeroMonetario( {value : new Number( calcVlrItemSup ).toFixed(2)} ) );
        // CALCULA: "(%) Supervisão" a partir do "Valor Executado"
        }else{
            var calcPercItemSup = obCalc.operacao(obj.value, vlrItemObra, '/', 20);
            calcPercItemSup = obCalc.operacao(calcPercItemSup, 100, '*', 20);
            jQuery( obj ).parents('tr:eq(0)')
                    .find('td:eq(9) input')
                    .val( this.formatarNumeroMonetario( {value : new Number( calcPercItemSup ).toFixed(2)} ) );
        }

        // CALCULA: "(%) do Item já Executado sobre a Obra após Supervisão" a partir do "Valor Executado"
        var vlrItemSup = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(10) input')
                                   .val();
        var vlrProjeto = jQuery('#vlrProjeto_' + obrid).val();
        var calcPercItemSupSobreObra = obCalc.operacao(vlrItemSup, vlrProjeto, '/', 20);
        calcPercItemSupSobreObra = obCalc.operacao(calcPercItemSupSobreObra, 100, '*', 20);
        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(11) span')
                .html( this.formatarNumeroMonetario( {value : new Number( calcPercItemSupSobreObra ).toFixed(2)} ) );

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(11) input')
                .val( this.formatarNumeroMonetario( {value : new Number( calcPercItemSupSobreObra ).toFixed(2)} ) );



        // CALCULA: "diferença Valor Executado
        var vlrItemSup  = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(10) input')
                                   .val();

        var vlrAnterior = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(7) input[type="hidden"]')
                                   .val();


       	var calcDiferencaValor = obCalc.operacao(vlrItemSup, vlrAnterior, '-', 20);

       	var sinal = "";
       	if( new Number( calcDiferencaValor ) < 0 ){
	       	sinal = "-";
       	}else{
	       	sinal = "";
        }

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(13) span')
                .html( sinal+""+this.formatarNumeroMonetario( {value : new Number( calcDiferencaValor ).toFixed(2)} ) );

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(13) input')
                .val( sinal+""+this.formatarNumeroMonetario( {value : new Number( calcDiferencaValor ).toFixed(2)} ) );


        // CALCULA: "diferença % Executado
        var percItemSup  = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(9) input')
                                   .val();

        var percAnterior = jQuery( obj ).parents('tr:eq(0)')
                                   .find('td:eq(7) input[type="text"]')
                                   .val();

        var calcDiferencaPerc = obCalc.operacao(percItemSup, percAnterior, '-', 20);

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(14) span')
                .html(  (new Number( calcDiferencaPerc ).toFixed(2)).replace(".",",") );

        jQuery( obj ).parents('tr:eq(0)')
                .find('td:eq(14) input')
                .val(  (new Number( calcDiferencaPerc ).toFixed(2)).replace(".",",") );


    }

    this.formatarNumeroMonetario = function ( obj ){
        var valor = new String( obCalc.retiraZeroEsquerda( new String(obj.value) ) );

        if ( valor.length == 2 ){
            valor = '0' + valor;
        }else if ( valor.length == 1 ){
            valor = '00' + valor;
        }else if ( valor.length == 0 ){
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }

    this.managerMetaFisica = function ( obj, limiteMetaFisica ){
        obj = obj || '';

        var vlrObj = this.formatarNumeroMonetario( {value : new String( obj.value )} );
        obj.value = vlrObj;
        if (obCalc.comparar(new String(vlrObj), limiteMetaFisica, '>') ){
            obj.value = this.formatarNumeroMonetario( {value : new Number( limiteMetaFisica ).toFixed(2) } );
        }
    }

    function calculaNivelEdificacao( obrid ){
        var idTrEdicacao   = 'tr_item_edificacao_';
        var idTrEtapa      = 'tr_item_etapa_';
        var vlrEtapaSupTot = 0;

        jQuery('[id^=' + idTrEtapa + obrid + '_]').each(function (i, obj){
        	vlrEtapaSupTot = obCalc.operacao(vlrEtapaSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        jQuery( '#' + idTrEdicacao + obrid ).find('td:eq(10) input')
                                       .val( objClass.formatarNumeroMonetario( {value : new Number( vlrEtapaSupTot ).toFixed(2)} ) );
        objClass.managerEdificacao(jQuery( '#' + idTrEdicacao + obrid ).find('td:eq(10) input')[0] , 'numerico');
    }

    function calculaNivelEtapa( obrid, icoid ){
        var idTrEtapa             = 'tr_item_etapa_';
        var idTrDetalhamento      = 'tr_item_detalhamento_';
        var vlrDetalhamentoSupTot = 0;
        jQuery('[id^=' + idTrDetalhamento + obrid + '_' + icoid + '_]').each(function (i, obj){
        	vlrDetalhamentoSupTot = obCalc.operacao(vlrDetalhamentoSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        jQuery( '#' + idTrEtapa + obrid + '_' + icoid ).find('td:eq(10) input')
                                                  .val( objClass.formatarNumeroMonetario( {value : new Number( vlrDetalhamentoSupTot ).toFixed(2)} ) );

        objClass.managerEtapa(jQuery( '#' + idTrEtapa + obrid + '_' + icoid ).find('td:eq(10) input')[0], obrid, 'numerico');
    }

    function calculaNivelDetalhamento( obrid, icoid, ditidPai ){
        var idTrDetalhamento      = 'tr_item_detalhamento_';
        var idTrDetalhamentoFilho = 'tr_item_detalhamento_filho_';

        var vlrDetalhamentoFilhoSupTot   = 0;

        jQuery('[id^=' + idTrDetalhamentoFilho + obrid + '_' + icoid + '_' + ditidPai + '_]').each(function (i, obj){
        	vlrDetalhamentoFilhoSupTot = obCalc.operacao(vlrDetalhamentoFilhoSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        jQuery( '#' + idTrDetalhamento + obrid + '_' + icoid + '_' + ditidPai ).find('td:eq(10) input')
                                                  .val( objClass.formatarNumeroMonetario( {value : new Number( vlrDetalhamentoFilhoSupTot ).toFixed(2)} ) );

        objClass.managerDetalhamento(jQuery( '#' + idTrDetalhamento + obrid + '_' + icoid + '_' + ditidPai ).find('td:eq(10) input')[0], obrid, icoid, 'numerico');
    }

    function calculaTotalSupervisao( obrid ){
        var idTrEtapa      		= 'tr_item_etapa_' + obrid;
        var idTrTotal           = 'tr_total_supervisao_' + obrid;
        var vlrProjeto          = jQuery('#vlrProjeto_' + obrid).val();
        var vlrEdificacaoSupTot = 0;
        var percSupTot          = 0;
        jQuery('[id^=' + idTrEtapa + '_]').each(function (i, obj){
          vlrEdificacaoSupTot = obCalc.operacao(vlrEdificacaoSupTot, jQuery( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        percSupTot = obCalc.operacao(vlrEdificacaoSupTot, vlrProjeto, '/', 20);
        percSupTot = obCalc.operacao(percSupTot, 100, '*', 20);

        jQuery('#' + idTrTotal).find('td:eq(11)')
                        .html( objClass.formatarNumeroMonetario( {value : new Number( percSupTot ).toFixed(2)} ) );

        jQuery('#' + idTrTotal).find('td:eq(13)')
                        .html( objClass.formatarNumeroMonetario( {value : new Number( vlrEdificacaoSupTot ).toFixed(2)} ) );
    }

}

calculoSupervisao = new calculoSupervisao();

function validaJustificativasMudancaValores(){

    var primeira_td         = ''; 
    var tr                  = '';
    var obrid               = <?php echo $obrid;?>;
    var nome_item_edtitado  = '';
    var retorno             = true;
    
    jQuery('#lista_supervisao > tbody  > tr').each(function() {
        tr          = this;
        primeira_td = jQuery(tr).find('td:eq(0)');
        nome_item_edtitado = primeira_td.text().trim();
        
        var txt_input_justificativa = 'undefined';
        var id_spiid                = 'undefined';
        
        jQuery(primeira_td).find("input").each(function() {
            if(this.name === 'ico_spiid['+obrid+'][]'){
                id_spiid = this.value;
            }            
            if(this.name === 'spiid_just['+id_spiid+'][]'){
                txt_input_justificativa = this.value.trim();
            }
            
            if(txt_input_justificativa === ''){
                alert('Preencha a justificativa de mudança dos valores do Item: \n'+nome_item_edtitado);
                retorno =  false;
            }
        });
    });
    
    return retorno;    
}

function enviaFormulario(){
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;
        
        if(!validaJustificativasMudancaValores()){
            mensagem += 'Preencha a(s) justificativa(s) de mudança dos valores do Itens com alerta Vermelho. \n';
            validacao = false;            
        }

	if ( jQuery('#emiid').val() == '' ){
            mensagem += 'Empresa \n';
            validacao = false;
	}

	if (!validacao){
            alert(mensagem);
	}else{
            jQuery('#formulario_supervisao_empresa').submit();
	}

}

var ns = (navigator.appName.indexOf("Netscape") != -1);
var d = document;

function floatDiv(id, sx, sy){

       var el=d.getElementById?d.getElementById(id):d.all?d.all[id]:d.layers[id];
       var px = document.layers ? "" : "px";
       window[id + "_obj"] = el;
       if(d.layers)el.style=el;
       el.cx = el.sx = sx;el.cy = el.sy = sy;
       el.sP=function(x,y){this.style.left=x+px;this.style.top=y+px;};

       el.floatIt=function()
       {
               var pX, pY;
               pX = (this.sx >= 0) ? 0 : ns ? innerWidth : 
               document.documentElement && document.documentElement.clientWidth ? 
               document.documentElement.clientWidth : document.body.clientWidth;
               pY = ns ? pageYOffset : document.documentElement && document.documentElement.scrollTop ? 
               document.documentElement.scrollTop : document.body.scrollTop;
               if(this.sy<0) 
               pY += ns ? innerHeight : document.documentElement && document.documentElement.clientHeight ? 
               document.documentElement.clientHeight : document.body.clientHeight;
               this.cx += (pX + this.sx - this.cx)/8;this.cy += (pY + this.sy - this.cy)/8;
               this.sP(this.cx, this.cy);
               setTimeout(this.id + "_obj.floatIt()", 40);
       }
       return el;
}

</script>