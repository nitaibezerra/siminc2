<?php

$_REQUEST['preid'] = simec_strip_tags($_REQUEST['preid']);

if($_REQUEST['requisicaoJson']) {
    $_REQUEST['requisicaoJson']();
}

?>
<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<?php

function aprovarReformulacao(){

	require_once APPRAIZ . 'includes/workflow.php';
	$teste = wf_alterarEstado( $_POST['docid'], 1016, $_POST['comentario'], array( 'preid' => $_SESSION['par']['preid'] ) );
	echo "<script>window.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analise&preid={$_SESSION['par']['preid']}';</script>";
}

function popupAprovacaoReformulacao(){

	global $db;

	$sql = "SELECT
				pre.docid,
				obr.obrid,
				max(hst.hstid) as hstid
			FROM
				obras.preobra pre
			LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid
			LEFT  JOIN workflow.documento doc ON doc.docid = obr.docid
			LEFT  JOIN workflow.historicodocumento hst ON hst.docid = obr.docid
			WHERE
				pre.preid =  {$_SESSION['par']['preid']}
			GROUP BY
					pre.docid, obr.obrid ";

		$obra = $db->pegaLinha($sql);

	if( $obra['hstid'] != '' ){
		$sql = "SELECT
					esd.esdid,
					esd.esddsc
				FROM
					workflow.historicodocumento hst
				INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem
				WHERE
					hstid = {$obra['hstid']}";

		$estadoObra = $db->pegaLinha($sql);
	}

	$arrTipoObrasMI = Array(42,43,44,45);

	$sql = "SELECT ptoid FROM obras.preobra WHERE preid = {$_SESSION['par']['preid']}";

	$ptoid = $db->pegaUm($sql);

	?>
	<style>
	.texto{
		font-size:				12px;
	}
	.butaoHorizontalGrande{
		align:					center;
		text-align:				center;
		vertical-align:			middle;
		width:					550px;
		height:					50px;
		background-color: 		#FFF8DC;
		-moz-border-radius: 	5px;
	    -webkit-border-radius: 	5px;
	    -khtml-border-radius: 	5px;
	    border-radius: 			5px;
	    display: 				table-cell;
	    cursor:					pointer;
	}
	.butaoHorizontalGrande:hover{
		font-weight:			bold;
		background-color: 		#FFFFF0;
	}
	</style>
	<script>
	function travaTela( trava ){
		if( trava ){
			$('#aguardando').show();
		}else{
			$('#aguardando').hide();
		}
	}
	
	$('.retorna').click(function(){
		travaTela( true );
		if( $('#comentario').val() == '' ){
			travaTela( false );
			alert('Preencha a justificativa.');
			$('#comentario').focus();
			return false;
		}
		$('#esdid').val($(this).attr('id'));
		$('#formAprova').submit();
	});
	$('.envia').click(function(){
		travaTela( true );
		if( $('#comentario').val() == '' ){
			travaTela( false );
			alert('Preencha a justificativa.');
			$('#comentario').focus();
			return false;
		}
		if( $('#esdid_envia').val() == '' ){
			travaTela( false );
			alert('Selecione uma situação.');
			$('#esdid_envia').focus();
			return false;
		}
		$('#esdid').val($('#esdid_envia').val());
		$('#formAprova').submit();
	});
	</script>
	<center>
		<div id="aguardando" style="display:none; position: absolute; background-color: white; height:98%; width:95%; opacity:0.4; filter:alpha(opacity=40)" >
			<div style="margin-top:250px; align:center;">
				<img border="0" title="Aguardando" src="../imagens/carregando.gif">
				Carregando...
			</div>
		</div>
	</center>
	<form method="POST" id="formAprova" name="formAprova">
		<input type="hidden" name="esdid" 	id="esdid"/>
		<input type="hidden" name="docid" 	id="docid"	value="<?=$obra['docid'] ?>"/>
		<input type="hidden" name="req" 	id="req"	value="aprovarReformulacao"/>
		<div class="texto"><br>Justificativa:</div>
		<?=campo_textarea('comentario', 'S', 'S', '', 60, 5, 500)?>
		<br>
	</form>
	<?php
	if( $obra['obrid'] == '' ){
	?>
	<div class="texto"> Esta obra ainda não possui registro no Obras 2 e a aprovação da reformulação só terá efeito no sistema atual. <br></div><br>
	<div class="butaoHorizontalGrande retorna" >Enviar para obra aprovada.</div><br>
	<?php  
	}elseif( in_array($ptoid, $arrTipoObrasMI) ){
	?>
	<div class="texto"> Esta é uma obra do tipo MI.<br> Neste caso ele só pode ser encaminhada para 'Aguardando Adesão do Município.'</div><br>
	<div class="butaoHorizontalGrande retorna" id="<?=OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO ?>">Tramitar para 'Aguardando Adesão do Município.'</div><br>
	<?php 
	}else{
		if( $estadoObra['esdid'] != '' ){
	?>
		<div class="texto"> A obra se encontrava no módulo de Obras 2 na situação: '<?=$estadoObra['esddsc'] ?>'</div><br>
		<div class="butaoHorizontalGrande retorna" id="<?=$estadoObra['esdid'] ?>">Retornar para '<?=$estadoObra['esddsc'] ?>'</div><br>
		<div class="texto"> ou </div>
		<?php 
		}else{
		?>
		<div class="texto"> A obra não possui histórico de tramitação no Obras 2. <br> Escolha o novo estado da Obra no sistema Obras 2.</div><br>
		<?php 
		}
		?>
	<div class="texto">Situação:</div>
	<?php 
		$sql = "SELECT
					esdid as codigo,
					esddsc as descricao
				FROM
					workflow.estadodocumento
				WHERE
					tpdid = ".TPDID_OBJETO;
	?>
	<?=$db->monta_combo( "esdid_envia", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'esdid_envia','','','Estado Documento', ''); ?><br><br>
	<div class="butaoHorizontalGrande envia">Tramitar para situação escolhida.</div><br>
	<?php 
	}
	?>
<?php 
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

/**
 *  Verifica se pode emitir o relatório para o lote de escolas
 *  Em caso afirmativo
 */
if( $_POST["ajaxFinalizarAnalise"] )
{
	/*** Inicializa o retorno do AJAX ***/
	$oPreObraControle = new PreObraControle();
	$retorno = array();
	$retorno['valida'] = false;
	
	$sql = "
		Select	itptitulo,
				pertitulo 
		From questionario.resposta r 
		Inner Join questionario.itempergunta it on it.itpid = r.itpid
		Inner Join questionario.pergunta p on p.perid = it.perid
		Where r.itpid in (
			Select itpid 
			From questionario.itempergunta 
			Where perid in (
				Select perid 
				From questionario.pergunta 
				Where grpid in ( 
					Select grpid 
					From questionario.grupopergunta 
					Where queid = 49 
				)
			)
		) and qrpid = {$_POST["qrpid"]}
	";
	
	$respostas = $db->carregar($sql);

	//if( count($respostas) == 13 )
	if( $oPreObraControle->validaPermissaoFinalizarQuestionario( $_POST["qrpid"] ) ){
		$aedid = WF_AEDID_ANALISE_ENVIAR_VALIDACAO_DEFERIMENTO;
		for($i=0; $i<count($respostas); $i++){
			if( $respostas[$i]['itptitulo'] == 'Não.' ){
				$aedid = WF_AEDID_ANALISE_ENVIAR_VALIDACAO_DILIGENCIA;
				
				if( substr($respostas[$i]['pertitulo'],0,3) == '1.2' ){
					include_once APPRAIZ . 'includes/workflow.php';
					$retorno['valida'] 	= true;
					$aedid = WF_AEDID_ANALISE_ENVIAR_VALIDACAO_INDEFERIMENTO;
				
					// Sem comentário
					// Sem dados no array já que não há chamada de função após a ação.
					$dados = array("qrpid" => $_POST['qrpid']);
					
					// Realiza a alteração do estado da entidade no ano corrente.
					wf_alterarEstado( $_POST["docid"], $aedid, '', $dados );
					die( simec_json_encode($retorno) );
				}
			}
		}
	}
	/*** Retorna o objeto JSON e finaliza o script ***/
	die( simec_json_encode($retorno) );
}

$boData = true;
//if(!(date('YmdHis') <= DATA_EXPIRA_PROINFANCIA ) && !$_REQUEST['preid']){
//	echo "<label style=\"color: red\">Acesso negado!</label>";
//	exit;
//}

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "par/classes/WSSigarp.class.inc";

$obMunicipio = new Municipio();

$anoref = $_GET['ano'] ? $_GET['ano'] : '2011';
$preid 	= ($_REQUEST['preid']) ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

$_SESSION['par']['prog'] = $_REQUEST['prog'] != $_SESSION['par']['prog'] ? $_REQUEST['prog'] : 'nulo';

if($_REQUEST['preid'] && !$_GET['muncod']){
	$_SESSION['par']['preid'] 	= $_REQUEST['preid'];
	$_SESSION['par']['estuf'] 	= $_SESSION['par']['estuf'] ? $_SESSION['par']['estuf'] : $obMunicipio->recuperarUF($_SESSION['par']['muncod']);
}

if($_GET['muncod']){
	$_SESSION['par']['preid'] 	= $_GET['preid'];
	$_SESSION['par']['muncod'] 	= $_GET['muncod'];
	$_SESSION['par']['estuf'] 	= $obMunicipio->recuperarUF($_SESSION['par']['muncod']);
}

if(!$_REQUEST['preid']){
	unset($_SESSION['par']['preid']);
}

if($_REQUEST['download'] == 's'){
	unset($_REQUEST['download']);
	$arqid = $_REQUEST['arqid'];
	ob_get_clean();
	$file = new FilesSimec(null,null,'obras');
	$file->getDownloadArquivo($arqid);
	die;
} 


if($_REQUEST['preid']){
	$acesso = verificaResponsabilidadeUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);
	$_SESSION['par']['esfera'] = retornaEsfera( $_REQUEST['preid'] );
}

$cotaproinfancia = verificarQtdObraProinfancia(); 
$qtdtdpreobra = verificarQtdPreObraProinfancia();

?>
<html>
	<head>
		<title>PAR - Programa Pró Infância</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/entidadesn.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
		<script type="text/javascript">
			jQuery.noConflict();
			function atualizarObra(){
				window.close();
			}


			function visualizaComentariosDiligencia(docid) {
				if(docid != '') {
					if(jQuery('#btnVisualizaComentariosDiligencias').hasClass('mais')) {
			    	    jQuery.ajax({
			        	    url : 'par.php?modulo=principal/subacaoObras&acao=A&requisicaoJson=visualizarDetalheDiligencia',
			        	    type: "POST",
			    	        data: { 
			        	        docid : docid
			    	        },
			    	        success : function(retorno) {
			    	            jQuery('#comentariosDiligencia').html(retorno);
			    	            jQuery('#btnVisualizaComentariosDiligencias').addClass('menos');
			    	            jQuery('#btnVisualizaComentariosDiligencias').removeClass('mais');
			    	            jQuery('#btnVisualizaComentariosDiligencias').html('<img src="../imagens/menos.gif" alt="Menos" title="Menos" />');
			    	        }
			    	    });
					} else {
						jQuery('#comentariosDiligencia').html('');
			            jQuery('#btnVisualizaComentariosDiligencias').addClass('mais');
			            jQuery('#btnVisualizaComentariosDiligencias').removeClass('menos');
			            jQuery('#btnVisualizaComentariosDiligencias').html('<img src="../imagens/mais.gif" alt="Mais" title="Mais" />');
					}
				}

			}
		</script>			
	</head>
	<body>
	<br>
	<input type="hidden" name="cotaproinfancia" id="cotaproinfancia" value="<?php echo $cotaproinfancia;?>" /> 
	<input type="hidden" name="qtdtdpreobra" id="qtdtdpreobra" value="<?php echo $qtdtdpreobra;?>" />
	<?php 
		$preid = $_REQUEST['preid'];		
		$docid = prePegarDocid($preid);
		$esdid = prePegarEstadoAtual($docid); 

		$oPreObraControle = new PreObraControle();
		if($preid != NULL){
			$sql = "SELECT preanoselecao FROM obras.preobra WHERE preid = $preid";
			$preanoselecao = $db->pegaUm($sql);
			if( $preanoselecao == '2014' ){
				$_SESSION['proinfancia2014'] = true;
			}
		}
			
		
		$arrPerfil = Array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_ADMINISTRADOR,PAR_PERFIL_ENGENHEIRO_FNDE,PAR_PERFIL_COORDENADOR_GERAL);
		if( possuiPerfil($arrPerfil) || ( $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO && $perfil == PAR_PERFIL_ENGENHEIRO_FNDE ) ){
			if($preid){
//				echo $oPreObraControle->verificaAlertaArquivoCorrompido($preid);
			}
			if( possuiPerfil(Array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_COORDENADOR_GERAL)) ){
				if($preid){
//					echo $oPreObraControle->verificaAlertaArquivoSubstituido($preid);
				}
			}
		}
		$obItensComposicaoControle = new ItensComposicaoControle();
	 
		$tipoAba = ($_REQUEST['tipoAba']) ? $_REQUEST['tipoAba'] : 'dados';
		if($preid){
			$qrpid = pegaQrpidAnalisePAC( $preid, 49 );
			$respostas = $oPreObraControle->recuperarRespostasQuestionario($qrpid);
		}
		$arrRespSim = Array();
		if( is_array($respostas) ){
			foreach($respostas as $resposta){
				if( $resposta['resposta'] == 'Sim.' ){
					array_push($arrRespSim,$resposta['idpergunta']);
				}
			}
		}
		$chamada = 'pac'.ucwords($tipoAba);
		$arParametros = array('visao' => $chamada, 'preid' => $_REQUEST['preid'], 'respSim' => $arrRespSim);

        include_once APPRAIZ . 'www/obras2/_funcoes_obras_par.php';

        // Somente aparecerá na aba Dados do terreno
        /*
        if(empty($_REQUEST['aba'])){
        	if( $_REQUEST['preid'] ){
        		$dadoEntidade = $db->pegaLinha("SELECT CASE WHEN preesfera = 'M' THEN muncod ELSE estuf END as local, preesfera FROM obras.preobra WHERE preid = " . $_REQUEST['preid']); 
	        	if( $dadoEntidade['preesfera'] == 'M' ){
		            exibirAvisoPendencias(null, $dadoEntidade['local'], 'E', '', $dadoEntidade['preesfera']);
	        	} else {
		            exibirAvisoPendencias(null, '', 'E',  $dadoEntidade['local'], $dadoEntidade['preesfera']);
	        	}
        	}
        }
		*/

        // Monta tarja vermelha com aviso de pendência de obras
        prepararDetalhePendenciasObras();
        montarAvisoCabecalho($_SESSION['par']['esfera'], $_SESSION['par']['muncod'], $_SESSION['par']['estuf']);

        echo $avisoPendencia;
		$obItensComposicaoControle->visualizacaoItensComposicaoProInfancia($arParametros, $anoref);
		die();				
	?>
	</body>
</html>