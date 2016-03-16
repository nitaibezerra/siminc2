<?php
// empreendimento || obra || orgao
verificaSessao( 'empreendimento' );

$empid = $_SESSION['obras2']['empid'];
$empreendimento = new Empreendimento( $empid );

function testaSelecionado( $arqid, $sfndeid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				CASE WHEN aqs.arqid IS NOT NULL OR ass.arqid IS NOT NULL THEN 'f_selected' ELSE '' END
			FROM
				obras2.arquivosupervisao ars
			INNER JOIN public.arquivo 				   		a 	ON a.arqid 	   = ars.arqid AND 
																   a.arqstatus = 'A'
																   
			LEFT JOIN obras2.questaosupervisao 				qs ON qs.sfndeid 	   = ars.sfndeid AND 
																  qs.qtsstatus = 'A'
																  
	    	LEFT JOIN obras2.arquivoquestaosupervisao 		aqs ON aqs.arqid 	 = ars.arqid AND 
	    														   aqs.aqsstatus = 'A' AND
	    														   aqs.qtsid 	 = qs.qtsid
	    														   
			LEFT JOIN obras2.respostasubquestao rsq ON rsq.qtsid 	 = qs.qtsid AND
													   rsq.rsqstatus = 'A'
													   
	    	LEFT JOIN obras2.arquivorespostasubquestao 	ass ON ass.arqid 	 = ars.arqid AND 
	    													   ass.arsstatus = 'A' AND
	    													   ass.rsqid 	 = rsq.rsqid
			WHERE
				(aqs.arqid IS NOT NULL OR ass.arqid IS NOT NULL) AND
				ars.aqsstatus = 'A' AND
				ars.sfndeid = {$sfndeid} AND
				ars.arqid = {$arqid}";
	$teste = $db->pegaUm($sql);
	
	return $teste;
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
	$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
//	$sosidHabil = ($sosid ? 'N' : 'S');
	$sfndeid 						 = '';
	$_SESSION['obras2']['sfndeid'] = $sfndeid;
//	$endereco 					 = new Endereco( $empreendimento->endid );
}else{
	$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
//	$sosidHabil = ($sosid ? 'N' : 'S');
	$sfndeid = ($_REQUEST['sfndeid'] != '' ? $_REQUEST['sfndeid'] : $_SESSION['obras2']['sfndeid']);
	$_SESSION['obras2']['sfndeid'] = ($_REQUEST['sfndeid'] != '' ? $_REQUEST['sfndeid'] : $_SESSION['obras2']['sfndeid']);
	
	if ( empty($sfndeid) ){
		die("<script>
				alert('Faltam parametros para acessar a tela!'); 
				window.location = '?modulo=principal/listVistoriaEmp&acao=A';
			 </script>");
	}
	
	$supervisaoFNDE = new SupervisaoFNDE( $sfndeid );
	extract( $supervisaoFNDE->getDados() );
//	$endereco 		   = new Endereco( $endid );
	$docid = pegaDocidSupervisaoFNDE( $supervisaoFNDE->sfndeid );
}
$sosidHabil = ($sosid ? 'N' : 'S');
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

$somenteLeitura 	= 'S';
$habilitado     	= true;
$habilitadoPerfil 	= true;
if( possui_perfil( array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL) ) ){
    if($entidvistoriador){
        $vistoriador = new Entidade( $entidvistoriador );
        if($_SESSION['usucpf'] != $vistoriador->entnumcpfcnpj){
            $habilitadoPerfil = false;
        }
    }
}
// Regra inválida
//elseif( possui_perfil( array(PFLCOD_GESTOR_MEC) ) ){
//	$somenteLeitura 	= 'N';
//	$habilitado     	= false;
//	$habilitadoPerfil 	= false;
//}


if( $_REQUEST['abaAjax'] ){
	$_SESSION['obras2']['abaAjax'] = $_REQUEST['abaAjax'];?>

		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
		    <tr>
				<td colspan="2">
                    <div style="float:right">
                    	<img title="Imprimir questionário" onclick="imprimirQuestionario()" src="../imagens/print.png" class="link">
                	</div>
            	</td>
            </tr>
		    <tr>
		    	<td colspan="2">
			    <?php 
//							$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras2","destino" => "obras2");
//							$_SESSION['imgparams'] = array("filtro" => " 1=1 ", "tabela" => "obras2.arquivoquestaosupervisao");
			    
			    $questao  = new Questao();
			    $arFiltro = array("qstescopo" => "SFNDE",
		    				  	  "orgid" 	  => $_SESSION['obras2']['orgid'], 
		    				  	  "sfndeid" 	  => ($sfndeid ? $sfndeid : 0));
			    $arDados  = $questao->pegaTodaEstrutura( $arFiltro );
			    
			    $etqidUlt = null;
			    $itcidUlt = null;
			    $qstidUlt = null;
			    
			    $subQuestao = new SubQuestao();
			    foreach ( $arDados as $k => $dados ){
			    	
			    	$arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid( $dados['qstid'] );
			    	
			    	$qstctrlobs = json_decode( $dados['qstctrlobs'] );
			    	switch ( $dados['qtsresposta'] ){
			    		case 't':
			    			$obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');		
			    			break;
			    		case 'f':
			    			$obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');		
			    			break;
			    		case 'n':
			    			$obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');		
			    			break;
			    		default:
			    			$obsClass = 'divQuestaoNivel3_none';		
			    			break;
			    	}
			    	
			    	$qstctrlimg = json_decode( $dados['qstctrlimagem'] );
			    	switch ( $dados['qtsresposta'] ){
			    		case 't':
			    			$imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');		
			    			break;
			    		case 'f':
			    			$imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');		
			    			break;
			    		case 'n':
			    			$imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');		
			    			break;
			    		default:
			    			$imgClass = 'divQuestaoNivel4_none';		
			    			break;
			    	}
				    	
			    	
			    	// ABRE Questionário
			    	if ( $etqidUlt != $dados['etqid'] ){
			    		if ( !is_null($etqidUlt) ){
							$html .= "</fieldset>";	    		
			    		}
		    			$etqidUlt = $dados['etqid'];
						$html .= "<fieldset>
							      	<legend>{$dados['etqdsc']}</legend>";	    		
			    	}
			    		    	
			    	// ETAPA
			    	if ( $itcidUlt != $dados['itcid'] ){
			    		$itcidNum++;
						$html .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";	    		
			    		$itcidUlt = $dados['itcid'];
			    	}
			    	
			    	// DIVISÃO
			    	if ( $dvqidUlt != $dados['dvqid'] && !empty( $dados['dvqid'] ) ){
						$html .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";	    		
			    		$dvqidUlt = $dados['dvqid'];
			    	}
			    	
			    	
			    	// QUESTÃO
			    	if ( $qstidUlt != $dados['qstid']){
			    		
				    	// Sub Questão
				    	$subClass  			= ($dados['qtsresposta'] == 'f' ? 'divSubQuestao' : 'divSubQuestao_none' );
				    	$subItemClass 		= 'divSubItemQuestao';
				    	
				    	$subImgClass 		= "divSubQuestaoNivel4_none";
			    		$htmlSubQuestao 	= "";	
			    		if ( count($arDadosSubQuestao) > 0 ){
			    			$htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">
			    									<div class=\"divSubQuestaoNivel1\">Tipo:</div>";
			    		}
			    		foreach ($arDadosSubQuestao as $dadosSubQuestao ){
			    			$arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid( $dadosSubQuestao['sqtid'], ($dados['qtsid'] ? $dados['qtsid'] : 0) );
			    			$subObsClass	= $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "divSubQuestaoNivel3" : "divSubQuestaoNivel3_none";
			    			$checked 		= $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "checked=\"checked\"" : '';
			    			$restricao 		= '';
				    		if( $dadosSubQuestao['mrqid'] != '' ){
				    			$restricao = "<input type=\"hidden\" name=\"mrqid[{$dados['qstid']}][{$dadosSubQuestao['sqtid']}]\" value=\"{$dadosSubQuestao['mrqid']}\">";
				    		}
				    		
				    		$htmlSubQuestao .= "<div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" class=\"{$subItemClass}\">
				    								<div class=\"divSubQuestaoNivel2\">
				    									<input type=\"checkbox\" $checked
				    										   value=\"{$dadosSubQuestao['sqtid']}\" 
				    										   name=\"sqtid[{$dados['qstid']}][]\" 
				    										   id=\"sqtid_{$dadosSubQuestao['sqtid']}\" 
				    										   " . ($habilitado ? '' : 'disabled="disabled"') . "
				    										   onclick=\"ctrlImgObs(this, {$dadosSubQuestao['sqtid']});\"> 
				    									<label for=\"sqtid_{$dadosSubQuestao['sqtid']}\">
				    									{$dadosSubQuestao['sqtdsc']}
				    									</label>
				    								</div>
													<div class='" . $subObsClass . "' id='div_obs_sub_{$dadosSubQuestao['sqtid']}'>
														<div class='divSubQuestaoNivel3_1'>Observaçãos:</div>
														$restricao
														<input type=\"hidden\" name=\"rsqid[{$dados['qstid']}][{$dadosSubQuestao['sqtid']}]\" value=\"{$arResultadosDadosSubQuestao[0]['rsqid']}\">
														" . campo_textarea('rsqobs_' . $dados['qstid'] . '_' . $dadosSubQuestao['sqtid'], 'S', $somenteLeitura, '', 100, 4, 1000, '', '', '', '', '', $arResultadosDadosSubQuestao[0]['rsqobs'], array('id' => "qtsobs_sub_{$dadosSubQuestao['sqtid']}")) . "
													</div>
												</div>";	
							$htmlSubQuestaoTipo = '';	
							// Quando houver subquestão e a resposta for "não" virá fechada a OBSERVAÇÃO
							if ( $dados['qtsresposta'] == 'f' ){
								$obsClass 		 = 'divQuestaoNivel3_none';
								$dados['qtsobs'] = '';
							}
				    	}
			    		if ( count($arDadosSubQuestao) > 0 ){
			    			$htmlSubQuestao .= "</div>";
			    		}
				    	
						$html .= "<div class='divQuestao'>
									<div class='divQuestaoNivel1'>{$dados['qstnumero']} - {$dados['qstdsc']}</div>
									<div class='divQuestaoNivel2'>
										<input type='hidden' name='qtsid_{$dados['qstid']}' id='qtsid_{$dados['qstid']}' value='{$dados['qtsid']}'>
										<input 
											name='qstid[{$dados['qstid']}]' 
											id='qstid_t_{$dados['qstid']}' value='t' 
											type='radio' 
											" . ($habilitado ? '' : 'disabled="disabled"') . "
											onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->S ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, false);' " . 
											($dados['qtsresposta'] == 't' ? "checked='checked'" : "") . ">
										<label for='qstid_t_{$dados['qstid']}'>Sim</label>
										&nbsp;&nbsp;
										<input 
											name='qstid[{$dados['qstid']}]' 
											id='qstid_f_{$dados['qstid']}' 
											value='f' 
											type='radio'
											" . ($habilitado ? '' : 'disabled="disabled"') . " 
											onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->N ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, true);' " . 
											($dados['qtsresposta'] == 'f' ? "checked='checked'" : "") . ">
										<label for='qstid_f_{$dados['qstid']}'>Não</label>
										&nbsp;&nbsp;
										<input 
											name='qstid[{$dados['qstid']}]' 
											id='qstid_n_{$dados['qstid']}' 
											value='n' 
											type='radio'
											" . ($habilitado ? '' : 'disabled="disabled"') . " 
											onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->NA ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, false);' " . 
											($dados['qtsresposta'] == 'n' ? "checked='checked'" : "") . ">
										<label for='qstid_n_{$dados['qstid']}'>Não se aplica</label>
									</div>
									" . $htmlSubQuestao . "
									<div class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
										<div class='divQuestaoNivel3_1'>Observaçãos:</div>
										" . campo_textarea('qtsobs_' . $dados['qstid'] , 'S', $somenteLeitura, '', 100, 4, 1000, '', '', '', '', '', $dados['qtsobs']) . "
									</div>";
						 
						$html .= "</div>";	    		
			    		$qstidUlt = $dados['qstid'];
			    	}
		
			    	echo $html;
			    	$html = null;
			    }
			    ?>
		    	</td>
		    </tr>
	   	</table>
		<?php 
		die;

}else{
	$_SESSION['obras2']['abaAjax'] = $_SESSION['obras2']['abaAjax'] ? $_SESSION['obras2']['abaAjax'] : 'dadosSupervisao';
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if( $_SESSION['obras2']['sfndeid'] ){
	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMPRESA_EDICAO,$url,$parametros);
}else{
	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMPRESA,$url,$parametros);
}

//$empreendimento = new Empreendimento( $empid );
$empreendimento->montaCabecalho();

monta_titulo( 'Cadastro de Supervisão', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<style>
.subquestaoobs{padding-bottom: 4px;}
.div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
.div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
html>body .div_fotos li { height: 80px; line-height: 1.2em; }
.field_fotos{/*padding:10px;*/width:90%}
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
		 	   				 	 "padrao" =>  ($_SESSION['obras2']['abaAjax'] == 'dadosSupervisao' ? true : false)  ,
		 		   				 "url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 		   				 "parametro" => "abaAjax=dadosSupervisao"
	 		  			),
		 				array(	 "descricao" => "Local da Obra",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'localObra' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=localObra"
		 				),
		 				array(	 "descricao" => "Cronograma",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'cronograma' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=cronograma"
		 				),
		 				array(	 "descricao" => "Questionário",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'questionario' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=questionario"
		 				),
		 				array(	 "descricao" => "Fotos",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'fotos' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
		 						"parametro" => "abaAjax=fotos"
		 				),
		 				array(	 "descricao" => "Tramitação",
		 						"padrao" => ($_SESSION['obras2']['abaAjax'] == 'tramitacao' ? true : false) ,
		 						"url" => "obras2.php?modulo=principal/cadVistoriaEmpresa&acao={$_GET['acao']}",
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
                            <? if($habilitadoPerfil):?>
		                <?php if($habilitado && $somenteLeitura == 'S'){ ?>
		                    <input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="enviaFormulario();">
		                <?php } ?>
                            <? endif;?>
		                <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/listVistoriaEmp&acao=A';">
		                <input type="button" value="Voltar Para Lista de obras por OS" style="cursor: pointer" onclick="location.href='?modulo=principal/listaEmpreendimentoEmpresa&acao=A';">
		        </td>
		    </tr>
	</table>    
</form>
<script type="text/javascript">

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