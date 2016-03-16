<?php
include_once APPRAIZ . 'includes/workflow.php';

if($_REQUEST['requisicao'] == 'mostraDistrato'){
	//ver($_REQUEST,d);
	
	$sql = "SELECT
			    ta.tpadesc,
			    COALESCE(oardesc, '') as tipo,
			    '<a onclick=\"donwloadDistrato( '||oa.obrid||','||oa.arqid||', 1)\" style=\"color: blue;\">'||a.arqnome || '.' || a.arqextensao||'</a>' as descricao,
			    to_char(oardtinclusao,'DD/MM/YYYY') AS data,
			    usu.usunome
			FROM
			    obras2.obras_arquivos oa
			    JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
			    JOIN public.arquivo      a ON a.arqid = oa.arqid
			    JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
			WHERE
				oarstatus = 'A' 
				AND (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')
				and oa.tpaid = 30 
				and oa.oarstatus = 'A'
				AND oa.obrid = '{$_REQUEST['obrid']}' 
			ORDER BY 2";
	
	$cabecalho 	= array("Tipo Arquivo", "Descrição", "Arquivo", "Data de Inclusão", "Gravado por");
	$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','100%','N', true, false, false, true);
	
	exit();
}

if($_REQUEST['requisicao'] == 'download'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
	$sql = "select oa.arqid
					from obras2.obras_arquivos oa 
						 JOIN public.arquivo      a ON a.arqid = oa.arqid
					WHERE
						oa.tpaid = 30 
					    and oa.oarstatus = 'A'
                        and oa.obrid = {$_REQUEST['obrid']}
						and (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')
                    order by oa.oardata desc";
	$arqid = (empty($_REQUEST['arqid']) ? $db->pegaUm($sql) : $_REQUEST['arqid']);
	//ver($arqid, $_REQUEST);
	
	ob_clean();
	$file = new FilesSimec('', '', 'obras2');
	$arquivo = $file->getDownloadArquivo($arqid);
	
	//echo"<script>document.location.href = 'par.php?modulo=principal/analiseReformulacaoObrasMITecnico&acao=A';</script>";
	exit;
}

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraHistoricoWorkflow' ){
	echo mostraHistoricoWorkflowObraMI( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao_'.$_POST['proid']] == 'validaAnaliseMI' ){
	ob_clean();
	
	$retorno = validaAnaliseObrasMIConvencional($_POST);
	
	if( $retorno ){
		$db->commit();
		$db->sucesso('principal/analiseReformulacaoObrasMITecnico');
	} else {
		echo "<script>
					alert('Operação não pode ser realizada');
					window.location.href = window.location;
				</script>";
	}
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraAnaliseParecer' ){
	ob_clean();
	
	$boContraPartida = 'N';
	if( $_POST['esdiddestino'] == WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL ) $titulo = 'Parecer de Aprovação';
	if( $_POST['esdiddestino'] == WF_TIPO_OBRA_APROVADA ) $titulo = 'Parecer de Recusa';
	if( $_POST['esdiddestino'] == WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL ){
		$titulo = 'Parecer de Diligência';
		$boContraPartida = 'S';
	}
	
	if( $_POST['esdiddestino'] == WF_TIPO_EM_ANALISE_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA ) $titulo = 'Parecer de Retorno Análise da Solicitação Reformulação MI para Convencional';
	if( $_POST['esdiddestino'] == WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL ) $titulo = 'Parecer de Análise da Reformulação MI para Convencional';
	if( $_POST['esdiddestino'] == WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO ) $titulo = 'Parecer de Aprovação de Prorrogação';
	?>
	<form name="formulario_parecer_<?php echo $_POST['proid']?>" id="formulario_parecer_<?php echo $_POST['proid']?>" method="post">   
    	<input type="hidden" name="requisicao_<?php echo $_POST['proid']; ?>" id="requisicao" value="">
    	<input type="hidden" name="esdiddestino_<?php echo $_POST['proid']; ?>" id="esdiddestino" value="<?php echo $_POST['esdiddestino']; ?>">
    	<input type="hidden" name="proid" id="proid" value="<?php echo $_POST['proid']; ?>">
    	<input type="hidden" name="prmtipo" id="prmtipo" value="<?php echo $prmtipo; ?>">
    	<input type="hidden" name="boContraPartida_<?php echo $_POST['proid']?>" id="boContraPartida_<?php echo $_POST['proid']?>" value="<?php echo $boContraPartida; ?>">
    	
		<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" style="width: 100%" cellspacing="1" cellpadding="3">
			<tr>
				<th colspan="4" width="25%"><b><?php echo $titulo; ?></b></th>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>Parecer:</b></td>
				<td colspan="3"><?=campo_textarea('prmparecer_'.$_POST['proid'], 'N', 'A', 'Parecer', 90, 15, 4000, '', '', '', '', 'Parecer', $prmparecer);?></td>
			</tr>
<?php 	if ( $boContraPartida == 'S' ){?>
			<tr>
				<td class="subtitulodireita" colspan="4" style="text-align: left"><b>Deverá Informar a Contrapartida?</b>
					<input type="radio" name="prmanalisecontrapartida_<?php echo $_POST['proid']?>" value="S">Sim
					<input type="radio" name="prmanalisecontrapartida_<?php echo $_POST['proid']?>" value="N">Não
				</td>
			</tr>
<?php 	}?>
		</table>
	</form>
	<?
	exit();
}

if( $_REQUEST['requisicao'] == 'carregarListaProcesso' ){
	ob_clean();
	
	carregarListaProcessoObrasMI( $_REQUEST, 'T' );
		
	exit();
}

if( $_REQUEST['requisicao'] == 'carregarListaObras' ){
	ob_clean();
	
	carregarListaObrasMiConvencional($_POST, 'T');
	exit();
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, '' );

/* $abaAtiva = 'par.php?modulo=principal/analiseReformulacaoObrasMITecnico&acao=A&tipoaba='.( empty($_REQUEST['tipoaba']) ? 'pendente' : $_REQUEST['tipoaba']);

$menu = array (
		0 => array (
				"id" => 0,
				"descricao" => "Pendente de Análise",
				"link" => "par.php?modulo=principal/analiseReformulacaoObrasMITecnico&acao=A&tipoaba=pendente"
		),
		1 => array (
				"id" => 1,
				"descricao" => "Analisado",
				"link" => "par.php?modulo=principal/analiseReformulacaoObrasMITecnico&acao=A&tipoaba=analisado"
		)
);

echo montarAbasArray ( $menu, $abaAtiva ); */

?>
<form name="formularioAnalise" id="formularioAnalise" method="post" action="" >
	<input type="hidden" name="reqs" id="reqs" value="">
	<input type="hidden" name="tipoaba" id="tipoaba" value="<?php echo ( empty($_REQUEST['tipoaba']) ? 'pendente' : $_REQUEST['tipoaba']); ?>">
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="subtituloDireita" width="30%">UF:</td>
			<td>
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao 
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município:</td>
			<td id="td_muncod">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Quantidade de Obras no Município:</td>
			<td id="td_muncod">
				<?php
				$qtdobramunicipio = $_REQUEST['qtdobramunicipio'];
				echo campo_texto( 'qtdobramunicipio', 'N', 'S', '', 10, 10, '[#]', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Quantidade de Obras no Processo:</td>
			<td id="td_muncod">
				<?php
				$qtdobraprocesso = $_REQUEST['qtdobraprocesso'];
				echo campo_texto( 'qtdobraprocesso', 'N', 'S', '', 10, 10, '[#]', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">WorkFlow:</td>
			<td>
				<?php
				$esdid = $_POST['esdid'];
				$sql = "SELECT distinct
							es.esdid as codigo,
							es.esddsc as descricao
						FROM obras.preobra p
							inner join workflow.documento d on d.docid = p.docid
						    inner join workflow.estadodocumento es on es.esdid = d.esdid
						WHERE d.tpdid = 37
						 	and es.esddsc ilike '%RMC - %'
						order by es.esddsc";
				$db->monta_combo( "esdid", $sql, 'S', 'Todos as Ações', '', '', '', '400' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" onclick="pesquisarAnalise();" value="Pesquisar"/>
				<input type="button" onclick="window.location.href = window.location" value="Todos"/>
			</td>
		</tr>
	</table>
</form>

<div id="dialog_jquery" title="" style="display: none" >
	<div style="padding:5px;text-align:justify;" id="retornoDialog_jquery"></div>
</div>

<div id="debug"></div>
<?php
$arWere = array();
if( $_REQUEST['reqs'] == 'pesquisar' ){
	if( !empty($_REQUEST['muncod']) ){
		array_push($arWere, "po.muncod = '{$_REQUEST['muncod']}'");
	} else {
		if( !empty($_REQUEST['estuf']) ){
			array_push($arWere, "po.estuf = '{$_REQUEST['estuf']}'");
		}
	}
}

if( $_REQUEST['qtdobraprocesso'] ) $filtro = " having count(p.preid) = {$_REQUEST['qtdobraprocesso']} ";

if( !empty($_REQUEST['esdid']) ){
	array_push($arWere, "d.esdid = '{$_REQUEST['esdid']}'");
}

if( $_REQUEST['qtdobramunicipio'] ){
	array_push($arWere, "iu.inuid in (select 
										    iu.inuid
										from
										    obras.preobra p 
										    inner join obras.pretipoobra pt on pt.ptoid = p.ptoid and pt.ptoid in (42, 43, 44, 45, 73, 74)
											inner join par.processoobraspaccomposicao pp on pp.preid = p.preid
											inner join par.processoobra po on po.proid = pp.proid
											inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null )
										where p.prestatus = 'A'
										group by iu.inuid
										 having count(p.preid) = {$_REQUEST['qtdobramunicipio']})");
}

$imgMais = "<span style=\"font-size:16px; cursor:pointer;\" title=\"mais\" id=\"image_'||iu.inuid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaProcesso(\''||iu.inuid||'\', \''||(CASE WHEN mun.muncod IS NOT NULL THEN mun.estuf ELSE est.estuf end)||'\', this);\"></span>";
$sql = "select distinct
			'<center>
				$imgMais
			</center>' as mais,
		    mun.muncod,
            CASE WHEN mun.muncod IS NOT NULL THEN mun.mundescricao ELSE est.estdescricao end as municipio,
		    CASE WHEN mun.muncod IS NOT NULL THEN mun.estuf ELSE est.estuf end as estado
		from par.processoobra po
		    inner join par.processoobraspaccomposicao pc on pc.proid = po.proid and pc.pocstatus = 'A'
		    inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null )
		    inner join obras.preobra p on p.preid = pc.preid and p.prestatus = 'A'
		    inner join workflow.documento d on d.docid = p.docid
    		inner join par.solicitacaoreformulacaoobras sr on sr.preid = pc.preid and sr.sfostatus = 'A' and sr.sfoacaosolicitada = 'RE'
    		left join territorios.municipio mun on mun.muncod = po.muncod
		    left join territorios.estado est on est.estuf = po.estuf
		where
		    po.prostatus = 'A' 		    
			".($arWere ? ' and '.implode(' and ', $arWere) : '')."
		order by 3";

$cabecalho = array('Abrir/Fechar', 'IBGE', 'Municípios', 'UF');
//$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','100%','S', true, false, false, true);
$db->monta_lista($sql,$cabecalho,50,10,'N','center','N','formprocesso', '', '', '');
?>

<script>
	function pesquisarAnalise(){
		jQuery('[name="reqs"]').val('pesquisar');
		jQuery('[name="formularioAnalise"]').submit();
	}
	
	function tipoAnaliseParecerMI( esdiddestino, proid ){
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=mostraAnaliseParecer&proid="+proid+"&esdiddestino="+esdiddestino,
			async: false,
			success: function(msg){
				jQuery( "#dialog_acoes_"+proid ).attr('title', 'Analise de Reformulação MI para Convencional');
				
				jQuery( "#dialog_acoes_"+proid ).show();
				jQuery( "#mostraRetorno_"+proid ).html(msg);
				jQuery( '#dialog_acoes_'+proid ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Enviar': function() {
								
								if(jQuery('[name="prmparecer_'+proid+'"]').val() == ''){
									alert('O campo parecer é obrigatório.');
									return false;
								} else if(jQuery('[name="boContraPartida_'+proid+'"]').val() == 'S' && jQuery('[name="prmanalisecontrapartida_'+proid+'"]:checked').length < 1 ){
									alert('E necessário informar se o município deverá distribuir a contrapartida entre as obras.');
									return false;
								} else {
									jQuery('[name="requisicao_'+proid+'"]').val('validaAnaliseMI');
									jQuery('[name="formulario_parecer_'+proid+'"]').submit();
									jQuery( this ).dialog( 'close' );
								}
							},
							'Cancelar': function() {
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
	}

	function abreObraAnalise( preid ){
		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid, 
		    	   			'ProInfancia', 
		    	   			"height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
	}
	
	function mostraHistoricoWorkflow( docid, proid ){

		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=mostraHistoricoWorkflow&docid="+docid,
			async: false,
			success: function(msg){
				jQuery( "#dialog_workflow_"+proid ).attr('title', 'Histórico de Tramitações');
				jQuery( "#dialog_workflow_"+proid ).show();
				jQuery( "#mostraWorkflow_"+proid ).html(msg);
				jQuery( '#dialog_workflow_'+proid ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Fechar': function() {
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
	}
	
	function carregarListaProcesso(inuid, estuf, obj) {
		
		var linha = obj.parentNode.parentNode.parentNode;
		var tabela = obj.parentNode.parentNode.parentNode.parentNode;

		if(obj.title == 'mais') {  			
			obj.title='menos';
			jQuery('#image_'+inuid).attr('class', 'glyphicon glyphicon-upload');
			var nlinha = tabela.insertRow(linha.rowIndex);
			var ncolbranco = nlinha.insertCell(0);
			ncolbranco.innerHTML = '&nbsp;';
			var ncol = nlinha.insertCell(1);
			ncol.colSpan=12;
			ncol.innerHTML="Carregando...";
			ncol.innerHTML="<div id='listaobraprocesso_" + inuid + "' ></div>";
			
			carregarListaDivProcesso( inuid, estuf );
		} else {
			obj.title='mais';
			jQuery('#image_'+inuid).attr('class', 'glyphicon glyphicon-download');
			var nlinha = tabela.deleteRow(linha.rowIndex);
		}
	}

	function carregarListaDivProcesso( inuid, estuf ){
		
		var qtdobraprocesso = jQuery('[name="qtdobraprocesso"]').val();
		
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregarListaProcesso&inuid="+inuid+"&estuf="+estuf+"&qtdobraprocesso="+qtdobraprocesso,
	   		async: false,
	   		success: function(msg){
	   	   		jQuery('#listaobraprocesso_'+inuid).html(msg);
	   		}
		});
	}
	
	function carregarListaObras(processo, proid, estuf, obj) {
		
		var linha = obj.parentNode.parentNode.parentNode;
		var tabela = obj.parentNode.parentNode.parentNode.parentNode;

		if(obj.title == 'mais') {  			
			obj.title='menos';
			jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-upload');
			var nlinha = tabela.insertRow(linha.rowIndex);
			var ncolbranco = nlinha.insertCell(0);
			ncolbranco.innerHTML = '&nbsp;';
			var ncol = nlinha.insertCell(1);
			ncol.colSpan=12;
			ncol.innerHTML="Carregando...";
			ncol.innerHTML="<div id='listaobraprocesso_" + processo + "' ></div>";
			
			carregarListaDivObras( processo, proid, estuf);
		} else {
			obj.title='mais';
			jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-download');
			var nlinha = tabela.deleteRow(linha.rowIndex);
		}

		var boHabilitaContPartida = false;

		var prmanalisecontrapartida	= jQuery('[name="prmanalisecontrapartidainformada_'+proid+'"]').val();
				
		jQuery('[name="preid['+proid+'][]"]').each(function(){
			var titulo = '';
			var preid 				= jQuery(this).val();
			var ptoid_atual 		= jQuery('[name="ptoid_atual['+proid+']['+preid+']"]').val();
			var situacaopreobra 	= jQuery('[name="situacaopreobra['+proid+']['+preid+']"]').val();
			var boSfoacaosolicitada = jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').length;
			var sfocontrapartidainformada = jQuery('#div_projeto_contrapartida_'+proid+'_'+preid).html();
			var acaosolicitada 		= jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').val();
			
			if( (sfocontrapartidainformada != '' && parseFloat(sfocontrapartidainformada) != parseFloat('0,00') || prmanalisecontrapartida == 'S' ) && boHabilitaContPartida == false ) boHabilitaContPartida = true;

			if( acaosolicitada == 'SA' ){
				var valorObra = jQuery('#div_valorobra_'+preid).html();
				
				jQuery('#div_valorprojeto_'+preid).html( valorObra );
			}

			jQuery('#sfoacaosolicitada_re_'+proid+'_'+preid).attr('disabled', true);
			jQuery('#sfoacaosolicitada_co_'+proid+'_'+preid).attr('disabled', true);
			jQuery('#sfoacaosolicitada_sa_'+proid+'_'+preid).attr('disabled', true);
			jQuery('[name="ptoid['+proid+']['+preid+']"]').attr('disabled', true);
			
			if( titulo != '' ){
				var texto = '<b>Esta obra não pode ser Reformulada / Cancelada:</b><br>'+titulo;
				jQuery('.pendeciaobra_'+preid).val(texto);
			} else {
				jQuery('#span_'+preid).css('display', 'none');
			}
		});

		//var prmanalisecontrapartida	= jQuery('[name="prmanalisecontrapartida_'+proid+'"]').val();
		//alert(boHabilitaContPartida);
		if( boHabilitaContPartida == false ){
			jQuery('.cab_projeto_analise_'+proid).css('display', 'none');
			jQuery('.td_projeto_analise_'+proid).css('display', 'none');
		} else {
			jQuery('.cab_projeto_analise_'+proid).css('display', '');
			jQuery('.td_projeto_analise_'+proid).css('display', '');
		}

		AddTableRow(proid);
	}

	function carregarListaDivObras( processo, proid, estuf ){
		
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregarListaObras&processo="+processo+'&proid='+proid+'&estuf='+estuf,
	   		async: false,
	   		success: function(msg){
	   	   		jQuery('#listaobraprocesso_'+processo).html(msg);
	   		}
		});
	}
	
	function retiraPontosPedido(v){
		if( v != 0 ){
			var valor = v.replace(/\./gi,"");
			valor = valor.replace(/\,/gi,".");
		} else {
			var valor = v;
		}
		
		return valor;
	}

	function donwloadDistrato( obrid, arqid, total ){
		if( total > 1 ){
			jQuery.ajax({
				type: "POST",
				url: window.location.href,
				data: "requisicao=mostraDistrato&obrid="+obrid+'&arqid='+arqid,
				async: false,
				success: function(msg){
					jQuery( "#dialog_jquery").attr('title', 'Anexos de Distrato');
					jQuery( "#dialog_jquery").show();
					jQuery( "#retornoDialog_jquery").html(msg);
					jQuery( '#dialog_jquery').dialog({
							resizable: false,
							width: 800,
							modal: true,
							show: { effect: 'drop', direction: "up" },
							buttons: {
								'Fechar': function() {
									jQuery( this ).dialog( 'close' );
								}
							}
					});
				}
			});
		} else {
			window.location.href = "par.php?modulo=principal/analiseReformulacaoObrasMITecnico&acao=A&requisicao=download&obrid="+obrid+'&arqid='+arqid;
		}
	}

	(function(jQuery) {

		/* Carrega Municpios de acordo com o estado
		*/
		jQuery('[name="estuf"]').change(function(){
			if( jQuery('[name="esfera"]').val() != 'E' ){
				if( jQuery(this).val() != '' ){
					jQuery('#aguardando').show();
					jQuery.ajax({
				   		type: "POST",
				   		url: window.location.href,
				   		data: "requisicao=carregaMunicipios&estuf="+jQuery(this).val(),
				   		async: false,
				   		success: function(resp){
				   			jQuery('#td_muncod').html(resp);
				   			jQuery('#aguardando').hide();
				   		}
				 	});
				}else{
					jQuery('#td_muncod').html('Escolha uma UF.');
				}
			}
		});
		//jQuery('[type="radio"]').
		
		  /*RemoveTableRow = function(handler) {
		    var tr = jQuery(handler).closest('tr');

		    tr.fadeOut(400, function(){ 
		      tr.remove(); 
		    }); 

		    return false;
		  };*/
		  
		  AddTableRow = function( proid ) {
			//jQuery('#td_products').show();
			
			var boSelect = false;
			var totalObra = 0;
			var totalProjeto = 0;
			var totAlunos = 0;
			var totAlunosAlt = 0;
			var totLinha = 0;
			var boJustificativa = false;

			var sfocontrapartidainformada =  0;
			var estadoAtual 		= '';
			
			jQuery('[name="preid['+proid+'][]"]').each(function(){
				var preid 				= jQuery(this).val();
				var boSfoacaosolicitada = jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').length;
				var ptoid 				= jQuery('[name="ptoid['+proid+']['+preid+']"]').val();
				var predescricao 		= jQuery('[name="predescricao['+proid+']['+preid+']"]').val();
				var ptodescricao 		= jQuery('[name="ptodescricao['+proid+']['+preid+']"]').val();
				//var sfojustificativa 	= jQuery('[name="sfojustificativa['+proid+']['+preid+']"]').val();
				var tipoAlteradoText	= jQuery("[name='ptoid["+proid+"]["+preid+"]'] :selected").text();
				var tipoAlterado		= jQuery("[name='ptoid["+proid+"]["+preid+"]']").val();
				var alunosatendido 		= jQuery('[name="alunosatendido['+proid+']['+preid+']"]').val();
				var valorProjeto		= jQuery('#div_valorprojeto_'+preid).html();
				var valorObra			= jQuery('#div_valorobra_'+preid).html();

				sfocontrapartidainformada = jQuery('#div_projeto_contrapartida_'+proid+'_'+preid).html();
				estadoAtual 		= jQuery('[name="estadoAtual['+proid+']['+preid+']"]').val();
				
				//jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]').tooltip();
				
				if( valorProjeto == '' ) valorProjeto = 0;
				
				var alunosatendidoAlt = 0;
				if( tipoAlterado == 73 ){//Projeto 1 Convencional
					alunosatendidoAlt = 188;
				}
				if( tipoAlterado == 74 ){//Projeto 2 Convencional
					alunosatendidoAlt = 94;
				}
				
				//if( sfojustificativa != '' ) tipoAlteradoText = '<center>-</center>';

				//if( boSfoacaosolicitada > 0 ){
					boSelect = true;
					jQuery('#td_products_'+proid).css('display', '');
					jQuery('#'+preid).remove();

					var acaosolicitada = jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').val();

					//if( acaosolicitada == 'RE' ) jQuery('[name="sfojustificativa['+proid+']['+preid+']"]').val('');
					if( acaosolicitada == 'RE' ){
						var vrlProjeto = retiraPontosPedido(valorProjeto);
						var vrlObra = retiraPontosPedido(valorObra);
						if( parseFloat(vrlProjeto) > parseFloat(vrlObra) ){
							var contrapartida = (parseFloat(vrlProjeto) - parseFloat(vrlObra));
							jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val( number_format(contrapartida, 2, ',', '.' ) );
						} else {
							jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val( '0,00' );
							/*jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').addClass('disabled');
							jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').attr('readonly', true);*/
						}
						boJustificativa = true;
					}
					
					if( acaosolicitada == 'SA' ){
						if( alunosatendido == '' ) alunosatendido = 0;

						var vrlObra = retiraPontosPedido(valorObra);

						valorProjeto = valorObra;
						tipoAlteradoText = ptodescricao;
						alunosatendidoAlt = alunosatendido;
					}
					//var sfocontrapartida 	= jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val();
					
					if( tipoAlteradoText == 'Selecione' ) tipoAlteradoText = '<center>-</center>'; 

					var cor = (totLinha % 2) ? 'style="background-color: rgb(191, 213, 239);"': '';
					
					var newRow = jQuery('<tr '+cor+' id="'+preid+'">');
					var cols = "";
	      			
	      			cols += '<td height="20px"><b>'+preid+'</b></td>';
	      			cols += '<td><b>'+predescricao+'</b></td>';
	      			cols += '<td><b>'+ptodescricao+'</b></td>';
	      			cols += '<td><b>'+valorObra+'</b></td>';
	      			cols += '<td align=center><b>'+alunosatendido+'</b></td>';
	      			cols += '<td height="20px"><b>'+tipoAlteradoText+'</b></td>';
	      			cols += '<td><b>'+valorProjeto+'</b></td>';
					cols += '<td align=center><b>'+alunosatendidoAlt+'</b></td>';
	      
	      			newRow.append(cols);	      
	      			jQuery("#products-table_"+proid).append(newRow);
	      			if( valorProjeto == '' ) valorProjeto = 0;
	      			totalObra = parseFloat(totalObra) + parseFloat(retiraPontosPedido(valorObra));
	      			totalProjeto = parseFloat(totalProjeto) + parseFloat(retiraPontosPedido(valorProjeto));

	      			totAlunos += parseFloat(alunosatendido);
	    			totAlunosAlt += parseFloat(alunosatendidoAlt);
				//}

				totLinha++;
			});
			
			if( boJustificativa == true && jQuery('[name="pprjustificativa['+proid+']"]').val() != '' ){
				jQuery('#tr_label_just_'+proid).css('display', '');
				jQuery('#tr_campo_just_'+proid).css('display', '');
			} else {
				jQuery('#tr_label_just_'+proid).css('display', 'none');
				jQuery('#tr_campo_just_'+proid).css('display', 'none');
			}

			jQuery('#div_total_vrlprojeto_'+proid).html(number_format(totalProjeto, 2, ',', '.' ));
			
			//Insere tabela de Totais 
			jQuery('#total_'+proid).remove();
			var newRowTot = jQuery('<tr id="total_'+proid+'">');
			var colsTot = "";
			colsTot += '<th class="titulo_resumo" height="20px">&nbsp;</th>';
			colsTot += '<th class="titulo_resumo" style="text-align: right; "><b>Totais:</b></th>';
			colsTot += '<th class="titulo_resumo">&nbsp;</th>';
			colsTot += '<th class="titulo_resumo" style="text-align: left;"><b>'+number_format(totalObra, 2, ',', '.' )+'</b></th>';
			colsTot += '<th class="titulo_resumo" align=center>'+totAlunos+'</th>';
			colsTot += '<th class="titulo_resumo" style="text-align: right; "><b>Totais:</b></th>';
			colsTot += '<th class="titulo_resumo" style="text-align: left;"><b>'+number_format(totalProjeto, 2, ',', '.' )+'</b></th>';
			colsTot += '<th class="titulo_resumo" align=center>'+totAlunosAlt+'</th>';  
			newRowTot.append(colsTot);	      
  			jQuery("#products-table_"+proid).append(newRowTot);


  			var contrapartidaTot = 0;  			
  			if( parseFloat(totalProjeto) > parseFloat(totalObra) ){
				contrapartidaTot = (parseFloat(totalProjeto) - parseFloat(totalObra));				
			}
			/*
			1561 - WF_TIPO_EM_AGUARDANDO_VALIDACAO_REFORMULACAO_MI_PARA_CONVENCIONAL
			1579 - WF_TIPO_EM_ANALISE_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA
			1578 - WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL 
			*/
			if( (sfocontrapartidainformada == '' || parseFloat(sfocontrapartidainformada) == parseFloat('0,00')) && 
					(estadoAtual != 1561 && estadoAtual != 1579 && estadoAtual != 1578 && estadoAtual != 228) ){
				contrapartidaTot = '0,00';
			}
			
  			//Insere Tabela de Contrapartida
  			jQuery('#contra_'+proid).remove();
			var newRowContra = jQuery('<tr id="contra_'+proid+'">');
			var colsContra = '<th style="text-align: center;" colspan=15 height="25px" class="titulo_resumo"><b>Valor Geral de Contrapartida: '+number_format(contrapartidaTot, 2, ',', '.' )+'</b></th>';  
			newRowContra.append(colsContra);	      
  			jQuery("#products-table_"+proid).append(newRowContra);

			var totalTR = jQuery('#products-table_'+proid).find('tr').length;

			jQuery('#td_row_'+proid).attr('rowspan', (totalTR - 2));
			
			if( boSelect == false ){
				jQuery('#td_products_'+proid).css('display', 'none');
			}
		  };
		  
		})(jQuery);
</script>
<?php 
function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
	FROM territorios.municipio
	WHERE estuf = '$estuf'
	ORDER BY 2 ASC";

	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}

?>