<?php
// empreendimento || obra || orgao
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

if ( $_GET['acao'] == 'A' ){
//	$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
//	$sosidHabil = ($sosid ? 'N' : 'S');
	$sfndeid 						 = '';
	$_SESSION['obras2']['sfndeid'] = $sfndeid;
//	$endereco 					 = new Endereco( $empreendimento->endid );
}
else{
//	$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
//	$sosidHabil = ($sosid ? 'N' : 'S');
	$sfndeid = ($_REQUEST['sfndeid'] != '' ? $_REQUEST['sfndeid'] : $_SESSION['obras2']['sfndeid']);
	$_SESSION['obras2']['sfndeid'] = ($_REQUEST['sfndeid'] != '' ? $_REQUEST['sfndeid'] : $_SESSION['obras2']['sfndeid']);
	
	if ( empty($sfndeid) ){
		die("<script>
				alert('Faltam parametros para acessar a tela!'); 
				window.location = '?modulo=principal/listaSupervisaoFNDE&acao=A';
			 </script>");
	}
	
	$supervisaoFNDE = new SupervisaoFNDE( $sfndeid );
	extract( $supervisaoFNDE->getDados() );
//	$endereco 		   = new Endereco( $endid );

	// $docid = pegaDocidSupervisaoFNDE( $supervisaoFNDE->sfndeid );
}
//$sosidHabil = ($sosid ? 'N' : 'S');
// o EXTRACT limpa o empid, assim o recarrega
$empid = $_SESSION['obras2']['empid'];

// Executa as funções da tela de acordo com suas ações
if ($_REQUEST["requisicao"] == 'salvar'){
	salvarSupervisaoFNDE();
	die();
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$endereco 		   = new Endereco( $endid ? $endid : $empreendimento->endid );

$somenteLeitura = 'S';
$habilitado     = true;
if (possui_perfil(PFLCOD_CALL_CENTER) ){
    $habilitado = false;
}
if( $_REQUEST['abaAjax'] ){
	$_SESSION['obras2']['abaAjax'] = $_REQUEST['abaAjax'];
	switch ( $_POST['abaAjax'] ){
		case 'dadosSupervisao':
			require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoFNDE_dado.inc";
			die;
		case 'localObra':
			require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoFNDE_local.inc";
			die;
		case 'cronograma':
			require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoFNDE_cronograma.inc";
			die;
		case 'questionario':
			require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoFNDE_questionario.inc";
			die;
		case 'fotos':
			require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoFNDE_foto.inc";
   			die;
   		break;
		case 'tramitacao':
			require_once APPRAIZ . "obras2/modulos/principal/cadSupervisaoFNDE_tramitacao.inc";
			die;
	}
}else{
	$_SESSION['obras2']['abaAjax'] = $_SESSION['obras2']['abaAjax'] ? $_SESSION['obras2']['abaAjax'] : 'dadosSupervisao';
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

//if( $_SESSION['obras2']['sfndeid'] ){
//	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMPRESA_EDICAO,$url,$parametros);
//}else{
//	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMPRESA,$url,$parametros);
//}

//$empreendimento = new Empreendimento( $empid );
//$empreendimento->montaCabecalho();
echo cabecalhoObra($obrid);

monta_titulo( 'Cadastro de Supervisão FNDE', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/obras2/js/vistoria.js"></script>
<style>
.div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
.div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
html>body .div_fotos li { height: 80px; line-height: 1.2em; }
.field_fotos{padding:10px;width:90%}
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
	
	<input type="hidden" name="sfndeid"               id="sfndeid"         value="<?php echo $sfndeid ?>" />
	<input type="hidden" name="requisicao"          id="requisicao"    value="salvar" />
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
		<tr>
			<td colspan="2" style="heigth:600px;">
				<div style="width:100%;height:600px;overflow:auto;">
			 	<?php 
			 	include(APPRAIZ."/includes/classes/AbaAjax.class.inc");
			 	
			 	$abaAjax = new AbaAjax("abasAjax",false,false,false,false);
			 	
		 		$arAba = array(
	 		  			array(	 "descricao" => "Dados da Supervisão", 
		 	   				 	 "padrao" 	 =>  ($_SESSION['obras2']['abaAjax'] == 'dadosSupervisao' ? true : false)  ,
		 		   				 "url" 		 => "obras2.php?modulo=principal/cadSupervisaoFNDE&acao={$_GET['acao']}",
		 		   				 "parametro" => "abaAjax=dadosSupervisao"
	 		  			),
		 				array(	 "descricao" => "Local da Obra",
		 						"padrao" 	 => ($_SESSION['obras2']['abaAjax'] == 'localObra' ? true : false) ,
		 						"url" 		 => "obras2.php?modulo=principal/cadSupervisaoFNDE&acao={$_GET['acao']}",
		 						"parametro"  => "abaAjax=localObra"
		 				),
		 				array(	 "descricao" => "Cronograma",
		 						"padrao" 	 => ($_SESSION['obras2']['abaAjax'] == 'cronograma' ? true : false) ,
		 						"url" 		 => "obras2.php?modulo=principal/cadSupervisaoFNDE&acao={$_GET['acao']}",
		 						"parametro"  => "abaAjax=cronograma"
		 				),
		 				array(	 "descricao" => "Questionário",
		 						"padrao" 	 => ($_SESSION['obras2']['abaAjax'] == 'questionario' ? true : false) ,
		 						"url" 		 => "obras2.php?modulo=principal/cadSupervisaoFNDE&acao={$_GET['acao']}",
		 						"parametro"  => "abaAjax=questionario"
		 				),
		 				array(	 "descricao" => "Fotos",
		 						"padrao" 	 => ($_SESSION['obras2']['abaAjax'] == 'fotos' ? true : false) ,
		 						"url" 		 => "obras2.php?modulo=principal/cadSupervisaoFNDE&acao={$_GET['acao']}",
		 						"parametro"  => "abaAjax=fotos"
		 				),
		 				array(	 "descricao" => "Tramitação",
		 						"padrao" 	 => ($_SESSION['obras2']['abaAjax'] == 'tramitacao' ? true : false) ,
		 						"url" 		 => "obras2.php?modulo=principal/cadSupervisaoFNDE&acao={$_GET['acao']}",
		 						"parametro"  => "abaAjax=tramitacao"
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
		                <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/listaSupervisaoFNDE&acao=A';">
		        </td>
		    </tr>
	</table>    
</form>
<script type="text/javascript">

jQuery('#endcep').val( mascaraglobal('##.###-###', jQuery('#endcep').val()) );

function ctrlSituacaoObra( sobid ){
	if ( sobid == <?php echo SITUACAO_OBRA_EM_EXECUCAO ?> ){
		jQuery('#situacaoObraSubPergunta').show();
	}else{
		jQuery('[name=sueacordo]:checked').attr('checked', false);
		jQuery('#situacaoObraSubPergunta').hide();
	}
}


function imprimirQuestionario(){
	return windowOpen( '?modulo=principal/cadVistoriaEmpresaImpressao&acao=A','blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
function imprimirQuestionarioRespondido(){
	return windowOpen( '?modulo=principal/cadVistoriaEmpresaImpressaoPreenchido&acao=A','blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
function ctrlImgObs(obj, sqtid){

	if ( jQuery(obj).attr('checked') ){
		jQuery('[id*=div_obs_sub_' + sqtid + ']').attr('class', 'divSubQuestaoNivel3');
	}else{
		jQuery('[id*=div_obs_sub_' + sqtid + ']').attr('class', 'divSubQuestaoNivel3_none');
	}
}

function ctrlObs(qstid, val){
	if ( jQuery('[name*=qstid[' + qstid + ']]:checked').val() == 'f' 
		&& jQuery('[name*=sqtid[' + qstid + ']]').size() > 0 ){
		jQuery('#div_obs_' + qstid).hide()
							  .find('textarea')
							  .val('');
		return;	
	}
	
	if ( val == true ){
		jQuery('#div_obs_' + qstid).show();
	}else{
		jQuery('#div_obs_' + qstid).hide()
							  .find('textarea')
							  .val('');
	}
}

function ctrlSubPergunta(qstid, val){
	if ( val == true ){
		jQuery('[id*=div_subquestao_' + qstid + ']').attr('class', 'divSubQuestao');
	}else{
		jQuery('[id*=div_subquestao_' + qstid + ']').attr('class', 'divSubQuestao_none')
										  .find('input:file')
										  .val('')
										  .find('textarea')
										  .val('');
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
        // return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&validaCPFProfissionalEmrpesa=1&','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
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

//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
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
    }

}

calculoSupervisao = new calculoSupervisao();

function enviaFormulario(){
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	if ( jQuery('#sosid').val() == '' ){
		mensagem += 'OS \n';
		validacao = false;
	}	
	
	if (!validacao){
		alert(mensagem);
	}else{
		jQuery('#formulario_supervisao_empresa').submit();
	}
	
}

function removerFotoGaleria(foto, pk)
{
	var tableta = jQuery(foto).parent().parent().attr('id').split('_');
	var campo 	= tableta[1];
	tableta		= tableta[0]
	var id 		= jQuery(foto).parent().parent().attr('name');
	var arqid  	= jQuery(foto).parent().attr('name');
	
	if( id && arqid ){
		var url = window.location + "&ajax=excluirFoto&tabela="+tableta+"&campo="+campo+"&id="+id+"&arqid="+arqid+"&pk="+pk;
		jQuery.ajax({
			url: url,
			success: function(data) {
		  }
		});
	}
	id = jQuery(foto).parent().attr('id');
	if( jQuery('[name="'+arqid+'"]').size() < 3 ){
		jQuery('#fotos_galeria').find('[id="'+id+'"]').removeClass("f_selected");
	}
	jQuery(foto).parent().remove();
}

function incluirFotos( sfndeid ){
	janela = window.open('?modulo=principal/popupFotosSupervisaoEmpresa&acao=A&sfndeid=' + sfndeid, 'inserirFotosSupervisao', 'menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=750,height=700' ); 
	janela.focus();
} 
 
function abrirGaleria(arqid,pagina,obj)
{
	var tableta = jQuery(obj).parent().parent().attr('id').split('_');
	var campo 	= tableta[1];
//	alert(campo);
//	tabela		= tableta[0]
	var id 		= jQuery(obj).parent().parent().attr('name');
//	if( tabela == 'fotos' ){
//		tabela = '';
//	}
	window.open("../slideshow/slideshow/obras2_galeriaSupervisao.php?&arqid=" + arqid + "&" + campo + "=" + id,"imagem","width=850,height=600,resizable=yes");
	//window.open("../slideshow/slideshow/verimagem.php?arqid=" + arqid,"imagem","width=850,height=600,resizable=yes");
}
</script>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario_supervisao_empresa'), 1500);
                   function bloqueiaForm(idForm){
                      jQuery(document).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#gerarXls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>