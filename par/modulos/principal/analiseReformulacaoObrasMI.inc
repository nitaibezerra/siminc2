<?php

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraHistoricoWorkflow' ){
	
	$_REQUEST['tipoaba'] = 'analisado';
	echo mostraHistoricoWorkflowObraMI( $_REQUEST );
	exit();
}

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ."includes/cabecalho.inc";

if( $_REQUEST['requisicao_'.$_POST['proid']] == 'validaAnaliseMI' ){
	ob_clean();
	
	$_REQUEST['tipoaba'] = 'analisado';
	$retorno = validaAnaliseObrasMIConvencional($_REQUEST);
	
	if( $retorno ){
		$db->commit();
		$db->sucesso('principal/analiseReformulacaoObrasMI');
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
		
	if( $_POST['esdiddestino'] == WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL ) $titulo = 'Parecer de Aprovação';
	if( $_POST['esdiddestino'] == WF_TIPO_OBRA_APROVADA ) $titulo = 'Parecer de Recusa';
	if( $_POST['esdiddestino'] == WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL ) $titulo = 'Parecer de Diligência';
	?>
	<form name="formulario_parecer_<?php echo $_POST['proid']?>" id="formulario_parecer_<?php echo $_POST['proid']?>" method="post">   
    	<input type="hidden" name="requisicao_<?php echo $_POST['proid']; ?>" id="requisicao" value="">
    	<input type="hidden" name="proid" id="proid" value="<?php echo $_POST['proid']; ?>">
    	<input type="hidden" name="prmtipo" id="prmtipo" value="<?php echo $_POST['tipo']; ?>">
    	
		<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" style="width: 100%" cellspacing="1" cellpadding="3">
			<tr>
				<th colspan="4" width="25%"><b><?php echo $titulo; ?></b></th>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>Parecer:</b></td>
				<td colspan="3"><?=campo_textarea('prmparecer_'.$_POST['proid'], 'N', 'A', 'Parecer', 90, 15, 4000, '', '', '', '', 'Parecer', $prmparecer);?></td>
			</tr>
		</table>
	</form>
	<?
	exit();
}

if( $_REQUEST['requisicao'] == 'carregarListaObras' ){
	ob_clean();

	$_REQUEST['tipoaba'] = 'analisado';
	carregarListaObrasMiConvencional($_REQUEST, 'M');
	exit();
}
 

/*** DECLARAÇÃO DE VARIAVEIS E FUNCOES ***/
if( !$_SESSION['par']['inuid'] ){
	echo "<script>
                    alert('Faltam dados na sessão!');
                    history.back(-1);
              </script>";
	die;
}

$estuf = $db->pegaUm("select case when estuf is null then mun_estuf else estuf end from par.instrumentounidade where inuid = ".$_SESSION['par']['inuid']);
$_SESSION['par']['estuf'] = ( $_SESSION['par']['estuf'] ? $_SESSION['par']['estuf'] :  $estuf);

/*** INSTANCIA AS DE CLASSES ***/
$obPreObraControle 		= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();
 
monta_titulo( $titulo_modulo, '' );
$db->cria_aba( $abacod_tela, $url, '' );

if(empty($_SESSION['par']['muncod']) && !empty($_SESSION['par']['estuf'])) {
	$descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['estuf'], 1);
} else {
	$descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);
}
?>
<form name="formularioAnalise" id="formularioAnalise" method="post" action="" >
	<input type="hidden" name="tipoaba" id="tipoaba" value="<?php echo ( empty($_REQUEST['tipoaba']) ? 'pendente' : $_REQUEST['tipoaba']); ?>">
</form>
<div class="tabela" style="margin-left: 7px; margin-right: 5px; width: 97.5% !important;">
    <span class="TituloTela" style="text-align: center;width: 100%">OBRAS - <?= $descricao;?></span>
    <span style="float: right; color: blue; font-size: 12px;">
        <a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
        <a href="par.php?modulo=principal/orgaoEducacao&acao=A">Dados da Unidade |</a>
        <a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
    </span>
</div>
<div style="clear: both;"></div>

<?php
if( $_SESSION['par']['itrid'] == 2 ){
	$filtro = " and po.muncod = '{$_SESSION['par']['muncod']}' ";
} else {
	$filtro = " and po.estuf = '{$_SESSION['par']['estuf']}' and po.muncod is null ";
}

/* Array com os itens da aba de identificação */
$menu = array (
		0 => array (
				"id" => 0,
				"descricao" => "Solicitações",
				"link" => "par.php?modulo=principal/pedidoReformulacaoObras&acao=A"
		),
		1 => array (
				"id" => 1,
				"descricao" => "Acompanhamento",
				"link" => "par.php?modulo=principal/analiseReformulacaoObrasMI&acao=A"
		)
);

echo montarAbasArray ( $menu, $url );

	$arWere = array();
	
	if( $_SESSION['par']['itrid'] == 2 ){
		array_push($arWere, "po.muncod = '{$_SESSION['par']['muncod']}' ");
	} else {
		array_push($arWere, "po.estuf = '{$_SESSION['par']['estuf']}' and po.muncod is null");
	}
	$_REQUEST['muncod'] = $_SESSION['par']['muncod'];
	$_REQUEST['tipoaba'] = 'analisado';
	
	
	carregarListaProcessoObrasMI($_REQUEST, 'M');
?>

<script>	
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
		
		jQuery('[name="preid['+proid+'][]"]').each(function(){
			var titulo = '';
			var preid 				= jQuery(this).val();
			var distrato 			= jQuery('[name="distrato['+proid+']['+preid+']"]').val();
			var ptoid_atual 		= jQuery('[name="ptoid_atual['+proid+']['+preid+']"]').val();
			var situacaopreobra 	= jQuery('[name="situacaopreobra['+proid+']['+preid+']"]').val();
			var boSfoacaosolicitada = jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').length;

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

				if( boSfoacaosolicitada > 0 ){
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
				}

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