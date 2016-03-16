<?php
/*
 * INICIALMENTE A TELA VINCULAVA A ETAPA A AÇÃO, PORÉM POR SOLICITAÇÃO DO HENRIQUE, AGORA A ETAPA É POR INICIATIVA.
 */


if ( $_POST['op'] == 'salvar' ){
	$ictid = $_POST['ictid'];
	$arAno = (array) $_POST['ano'];
	
	foreach( $arAno as $ano ){
		$etiid 		= $_POST['etiid'][$ano];
		$fisico 	= ($_POST['fisico'][$ano] ? desformata_valor( $_POST['fisico'][$ano] ) : 'NULL');
		$financeiro = ($_POST['financeiro'][$ano] ? desformata_valor( $_POST['financeiro'][$ano] ) : 'NULL');;
		
		if ( $etiid ){
			$sql = "UPDATE monitora.etapainiciativa
				    SET etivalorfisico={$fisico}, etivalorfinanceiro={$financeiro}
				 	WHERE 
				 		etiid={$etiid};";
		}else{
			$sql = "INSERT INTO monitora.etapainiciativa(
				            ictid, exeano, etivalorfisico, etivalorfinanceiro, usucpf
				    ) VALUES (
				    		{$ictid}, '{$ano}', {$fisico}, {$financeiro}, '". $_SESSION['usucpf'] ."'
				    );";
		}
			    		
		$db->executar( $sql );
		$db->commit();	    		
	}
	die("<script>
			alert('Operação realizada com sucesso!');
			location.href = '?modulo=principal/etapaAcao&acao=A';
		 </script>");
}

switch ($_POST['ajax']){
	case 'iniciativa':
		$sql = "SELECT 
					i.ictid AS codigo, 
					i.ictdsc AS descricao 
				FROM 
					monitora.iniciativa i
				JOIN monitora.objetivoiniciativa oi ON oi.ictid = i.ictid 
				WHERE 
					ictstatus = 'A' AND 
					oi.objid = '" . $_POST['objid'] . "'
				ORDER BY
					descricao";
		
		echo $db->monta_combo("ictid", $sql, 'S','Selecione...','carregaEtapa', '', '',400,'N','ictid');
		die;
	case 'objetivo':
		$sql = "SELECT 
					objid AS codigo, 
					objcod || ' - ' || objdsc AS descricao 
				FROM 
					monitora.objetivo 
				WHERE 
					prgano = '" . $_SESSION['exercicio'] . "' AND 
					objstatus = 'A' AND 
					prgid = '" . $_POST['prgid'] . "'
				ORDER BY
					descricao";
		
		echo $db->monta_combo("objid", $sql, 'S','Selecione...','carregaIniciativa', '', '',400,'N','objid');
		die;
	case 'etapa':
		$ictid = $_POST['ictid'];
		
		$sql = "SELECT 
					exeano
				FROM 
					monitora.exercicio 
				WHERE 
					exestatus ='A' 
				ORDER BY
					exeano";
		$dados = $db->carregarColuna( $sql );
		$dados = ($dados ? $dados : array());
		
		$td1 = "";			
		$td2 = "";			
		$td3 = "";			
		foreach ( $dados as $ano ){
			$sql = "SELECT
						etiid,
						etivalorfisico,
						etivalorfinanceiro
					FROM
						monitora.etapainiciativa
					WHERE
						ictid = {$ictid} AND
						exeano = '" . $ano . "'";
			
			$dados = $db->pegaLinha( $sql );
			
			$td1 .= '<th>
						' . $ano . '
						<input type="hidden" name="ano[]" id="ano_' . $ano . '" value="' . $ano . '">
						<input type="hidden" name="etiid[' . $ano . ']" id="etiid_' . $ano . '" value="' . $dados['etiid'] . '">
					 </th>';			
			$td2 .= '<td align="center" bgcolor="#F0F0F0">
						<input type="text" name="fisico[' . $ano . ']" id="fisico_' . $ano . '" value="' . $dados['etivalorfisico'] . '" style="text-align:right; width:100px;" onkeyup="this.value=mascaraglobal(\'#######################\',this.value);">
					 </td>';			
			$td3 .= '<td align="center" bgcolor="#F0F0F0">
						<input type="text" name="financeiro[' . $ano . ']" id="financeiro_' . $ano . '" value="' . ($dados['etivalorfinanceiro'] ? number_format($dados['etivalorfinanceiro'],2,',','.') : "") . '" style="text-align:right; width:100px;" onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);">
					 </td>';			
		}
		
		echo '<table class="tabela" align="center">
				<tr>
					<th align="right">Cronograma</th>
					' . $td1 . '
				</tr>
				<tr>
					<th align="right">Físico</th>
					' . $td2 . '
				</tr>
				<tr>
					<th align="right">Financeiro</th>
					' . $td3 . '
				</tr>
			  </table>';
		die;
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
	
monta_titulo('Etapas', '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function carregaObjetivo( prgid ){
	var td	= $('#td_objetivo');
	
	var td_iniciativa  = $('#td_iniciativa');
	var tr_etapa_label = $('#tr_etapa_label');
	var tr_etapa	   = $('#tr_etapa');
	var td_etapa	   = $('#td_etapa');
	var tr_salvar	   = $('#tr_salvar');
	
	td_iniciativa.html('');
	
	tr_etapa_label.hide();
	tr_etapa.hide();
	td_etapa.hide();
	
	tr_salvar.hide();
	
	if ( prgid != '' ){
		var url = location.href;
		$.ajax({
					  url  		 : url,
					  type 		 : 'post',
					  data 		 : {ajax  : 'objetivo', 
					  		  	    prgid : prgid},
					  dataType   : "html",
					  async		 : false,
					  beforeSend : function (){
					  	divCarregando();
						td.find('select:first').val('')
											   .attr('disabled', true);
					  },
					  error 	 : function (){
					  	divCarregado();
					  },
					  success	 : function ( data ){
					  	td.html( data );
					  	divCarregado();
					  }
				});	
	}else{
		td.find('select:first').val('')
							   .attr('disabled', true);
	}			
}

function carregaIniciativa( objid ){
	var td	= $('#td_iniciativa');
	
	var tr_etapa_label = $('#tr_etapa_label');
	var tr_etapa	   = $('#tr_etapa');
	var td_etapa	   = $('#td_etapa');
	var tr_salvar	   = $('#tr_salvar');
	
	tr_etapa_label.hide();
	tr_etapa.hide();
	td_etapa.hide();
	tr_salvar.hide();
	
	if ( objid != '' ){
		var url = location.href;
		$.ajax({
					  url  		 : url,
					  type 		 : 'post',
					  data 		 : {ajax  : 'iniciativa', 
					  		  	    objid : objid},
					  dataType   : "html",
					  async		 : false,
					  beforeSend : function (){
					  	divCarregando();
						td.find('select:first').val('')
											   .attr('disabled', true);
					  },
					  error 	 : function (){
					  	divCarregado();
					  },
					  success	 : function ( data ){
					  	td.html( data );
					  	divCarregado();
					  }
				});	
	}else{
		td.find('select:first').val('')
							   .attr('disabled', true);
	}			
}

function carregaEtapa( ictid ){
	var tr_label  = $('#tr_etapa_label');
	var tr		  = $('#tr_etapa');
	var td		  = $('#td_etapa');
	var tr_salvar = $('#tr_salvar');
	
	if ( ictid != '' ){
		var url = location.href;
		$.ajax({
					  url  		 : url,
					  type 		 : 'post',
					  data 		 : {ajax  : 'etapa', 
					  		  	    ictid : ictid},
					  dataType   : "html",
					  async		 : false,
					  beforeSend : function (){
					  	divCarregando();
					  	
					  	tr_label.hide();
					  	tr.hide();
					  	td.hide();
					  	tr_salvar.hide();
					  	
					  },
					  error 	 : function (){
					  	divCarregado();
					  	
					  	tr_label.hide();
					  	tr.hide();
					  	td.hide();
					  	tr_salvar.hide();
					  },
					  success	 : function ( data ){
					  	td.html( data );
					  	
					  	tr_label.show();
					  	tr.show();
					  	td.show();
					  	tr_salvar.show();
					  	
					  	divCarregado();
					  }
				});	
	}else{
	  	tr_label.hide();
	  	tr.hide();
	  	td.hide();
	  	tr_salvar.hide();
	}			
}

function validarFormEtapa(){
	$('#formEtapa').submit();
}
//-->
</script>

<form name="formEtapa" id="formEtapa" method="post" action="">
<input name="op" id="op" value="salvar" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Programa:</td>
		<td>
		<?php 
		$sql = "SELECT  
					DISTINCT
					p.prgid AS codigo, 
					p.prgcod || ' - ' || p.prgdsc AS descricao
				FROM 
					monitora.programa p 
				JOIN monitora.acao a ON a.prgid = p.prgid 
				JOIN monitora.objetivo o ON o.prgid = p.prgid AND
											o.objstatus = 'A'  
				WHERE 
					p.prgano='".$_SESSION['exercicio']."' AND 
					acasnrap = false 
				ORDER BY 
					p.prgid";	

		echo $db->monta_combo("prgid", $sql, 'S','Selecione...','carregaObjetivo', '', '',400,'N','prgid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Objetivo:</td>
		<td id="td_objetivo">&nbsp;</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Iniciativa:</td>
		<td id="td_iniciativa">&nbsp;</td>
	</tr>
	<tr id="tr_etapa_label" style="display:none;">
		<td class="SubTituloCentro" colspan="2">Detalhamento do Cronograma</td>
	</tr>
	<tr id="tr_etapa" style="display:none;">
		<td id="td_etapa" colspan="2">&nbsp;</td>
	</tr>
	<tr bgcolor="#CCCCCC" id="tr_salvar" style="display:none;">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btalterar" value="Salvar" onclick="validarFormEtapa()" class="botao">
	   </td>
	</tr> 
</table>
</form>