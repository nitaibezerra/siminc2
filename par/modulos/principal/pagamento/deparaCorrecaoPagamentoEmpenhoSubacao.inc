<?php 

function salvarAjustes(){
	
	global $db;
	
	if( is_array($_REQUEST['valor_ajustado']) ){
		$arrAjustes = Array();
		$strErro = '';
		foreach( $_REQUEST['valor_ajustado'] as $empid => $valoresObras ){
			foreach( $valoresObras as $arr => $ajustes ){
				$sbaid 	= 0;
				$pobano = 0;
				$arr 	= explode('_', $arr);
				$sbaid 	= $arr[0];
				$pobano = $arr[1];
				$vlrAjustesLinha = 0;
				foreach( $ajustes as $pagid => $ajuste ){
					$arrAjustesColuna[$pagid] += str_replace(Array('.',','), Array('','.'), trim($ajuste));
					$vlrAjustesLinha += str_replace(Array('.',','), Array('','.'), trim($ajuste));
				}
				if( trim($vlrAjustesLinha) > trim($_REQUEST['valor_empenhado'][$empid][$sbaid.'_'.$pobano]) ){
					$sql = "SELECT par.retornacodigosubacao($sbaid)";
					$sbanumero = $db->pegaUm( $sql );
					$strErro .= " - Ajuste de pagamento da subação $sbanumero ano $pobano excede o empenho em R$ ".number_format( ($vlrAjustesLinha - $_REQUEST['valor_empenhado'][$empid][$sbaid.'_'.$pobano]) ,2,',','.').".<Br>";
				}
			}
			foreach( $_REQUEST['pagid'][$empid] as $pagid => $valorPagamento ){
				if( $arrAjustesColuna[$pagid] != $valorPagamento ){
					$strErro .= " - O(s) ajuste(s) na {$_REQUEST['ob'][$empid][$pagid]} é diferente do valor da mesma.<Br>";
				}
			}
		}
	}
	if( $strErro != '' ){
		echo "<label style=color:red; >$strErro</label>";
	}else{
		$sql = "";
		if( is_array($_REQUEST['valor_ajustado']) ){
			foreach( $_REQUEST['valor_ajustado'] as $empid => $valoresObras ){
				foreach( $valoresObras as $arr => $ajustes ){
					$sbaid 	= 0;
					$pobano = 0;
					$arr 	= explode('_', $arr);
					$sbaid 	= $arr[0];
					$pobano = $arr[1];
					foreach( $ajustes as $pagid => $ajuste ){
						$ajuste = str_replace(Array('.',','), Array('','.'), trim($ajuste));
						$sqlPag = "SELECT true FROM par.pagamentosubacao WHERE sbaid = $sbaid AND pobano = $pobano AND pagid = $pagid;";
						$temPagamento = $db->pegaUm($sqlPag);
						if( $temPagamento == 't' ){
							$sql .= "UPDATE par.pagamentosubacao SET pobvalorpagamento = $ajuste WHERE sbaid = $sbaid AND pobano = $pobano AND pagid = $pagid;";
						}else{
							if( $ajuste > 0 ){
								$sql .= "INSERT INTO par.pagamentosubacao(sbaid, pobano, pagid, pobvalorpagamento, pobpercentualpag) VALUES($sbaid, $pobano, $pagid, $ajuste, 1);";
							}
						}
					}
				}
			}	
		}
		if( $sql != '' ){
			$db->executar($sql);
			if( $db->commit() ){
				echo 'sucesso';
			}else{
				echo 'Erro inesperadoao salvar.';
			}
		}else{
			echo 'sucesso';
		}
	}
}

function listaSubacoesEmpenho(){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				emp.empid, pob.sbaid, pob.pobano, eob.eobvalorempenho as valor_empenhado, sum( pob.pobvalorpagamento ) as valor_pago
			FROM 
				par.empenho emp 
			INNER JOIN par.empenhosubacao 		eob ON eob.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
			INNER JOIN par.pagamento 			pag ON pag.empid = emp.empid AND pag.pagstatus = 'A'
			INNER JOIN par.pagamentosubacao 	pob ON pob.sbaid = eob.sbaid AND pob.pobano = eob.eobano AND pag.pagid = pob.pagid
			WHERE 
				emp.empid = {$_REQUEST['empid']}
				AND emp.empsituacao <> 'CANCELADO'
				AND emp.empstatus = 'A'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
			GROUP BY
				emp.empid, pob.sbaid, pob.pobano, eob.eobvalorempenho
			ORDER BY
				2";

	$dados = $db->carregar($sql);
	$dados = is_array($dados) ? $dados : Array();
	
	$sql = "SELECT DISTINCT
				pag.pagid, pag.pagnumeroob, pag.pagvalorparcela
			FROM
				par.empenho emp
			INNER JOIN par.empenhosubacao 		eob ON eob.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
			INNER JOIN par.pagamento 			pag ON pag.empid = emp.empid AND pag.pagstatus = 'A'
			INNER JOIN par.pagamentosubacao 	pob ON pob.sbaid = eob.sbaid AND pob.pobano = eob.eobano AND pag.pagid = pob.pagid
			WHERE
				emp.empid = {$_REQUEST['empid']}
				AND emp.empsituacao <> 'CANCELADO'
				AND emp.empstatus = 'A'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
			ORDER BY
				1";
	
	$pagamentos = $db->carregar($sql);
	$pagamentos = is_array($pagamentos) ? $pagamentos : Array();
?>
	<table width=100% >
		<tr>
			<th rowspan="2" >Subação</th>
			<th rowspan="2" >Ano</th>
			<th rowspan="2" >Valor empenhado</th>
			<th rowspan="2" >(Valor empenhado<br> - Valor Pago) %</th>
			<th colspan="<?=count($pagamentos) ?>" > Pagamentos </th>
		</tr>
		<tr>
<?php 
	foreach( $pagamentos as $k => $pagamento ){
?>
			<th>
				<?=$pagamento['pagnumeroob'] ?><br> (R$ <?=number_format($pagamento['pagvalorparcela'],2,',','.') ?>)<br>
				<input type="hidden" name="pagid[<?=$_REQUEST['empid'] ?>][<?=$pagamento['pagid'] ?>]" id="pagid_<?=$_REQUEST['empid'] ?>_<?=$pagamento['pagid'] ?>" value="<?=$pagamento['pagvalorparcela'] ?>" />
				<input type="hidden" name="ob[<?=$_REQUEST['empid'] ?>][<?=$pagamento['pagid'] ?>]" value="<?=$pagamento['pagnumeroob'] ?>"/>
			</th>
<?php
	}
?> 	
		</tr>
<?
	foreach( $dados as $dado ){
		$sql = "SELECT par.retornacodigosubacao({$dado['sbaid']})";
		$sbanumero = $db->pegaUm( $sql );
?>
		<tr>
			<td><?=$sbanumero ?></td>
			<td><?=$dado['pobano'] ?></td>
			<td>
				R$ <?=number_format($dado['valor_empenhado'],2,',','.') ?>
				<input type=hidden name="valor_empenhado[<?=$dado['empid'] ?>][<?=$dado['sbaid'] ?>_<?=$dado['ano'] ?>]" id="valor_empenhado_<?=$dado['empid'] ?>_<?=$dado['sbaid'] ?>_<?=$dado['pobano'] ?>" value="<?=$dado['valor_empenhado'] ?>"/>
			</td>
			<td>
				<?php $cor = 'style="color:blue"';?> 
				<?php if( ($dado['valor_empenhado']-$dado['valor_pago']) < 0 ){ $cor = 'style="color:red"'; }?>
				<div  id="dif_<?=$dado['empid'] ?>_<?=$dado['sbaid'] ?>_<?=$dado['pobano'] ?>" title="Diferença entre o valor empenhado a subação e as parcelas pagas a mesma.">
					<label <?=$cor ?> >(R$ <?=number_format(($dado['valor_empenhado']-$dado['valor_pago']),2,',','.') ?>)</label>
				</div>
			</td>
<?php 
		foreach( $pagamentos as $k => $pagamento ){

			$valor_pago = '';
			$sql = "SELECT
						pob.pobvalorpagamento
					FROM
						par.empenho emp
					INNER JOIN par.empenhosubacao 		eob ON eob.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
					INNER JOIN par.pagamento 			pag ON pag.empid = emp.empid AND pagstatus = 'A'
					INNER JOIN par.pagamentosubacao 	pob ON pob.sbaid = eob.sbaid AND pob.pobano = eob.eobano AND pag.pagid = pob.pagid
					WHERE
						emp.empid = {$_REQUEST['empid']}
						AND pob.pagid = {$pagamento['pagid']}
						AND pob.sbaid = {$dado['sbaid']}
						AND pob.pobano = {$dado['pobano']}
						AND emp.empsituacao <> 'CANCELADO'
						AND emp.empstatus = 'A'
						AND pag.pagsituacaopagamento not ilike '%CANCELADO%'";
					
			$valor_pago = $db->pegaUm($sql);
?>
			<td>
				<input type=hidden value="<?=$valor_pago ?>" 
					   name="valor_pago[<?=$dado['empid'] ?>][<?=$dado['sbaid'] ?>_<?=$dado['pobano'] ?>][<?=$pagamento['pagid'] ?>]"
					   id="valor_pago_<?=$dado['empid'] ?>_<?=$dado['sbaid'] ?>_<?=$dado['pobano'] ?>_<?=$pagamento['pagid'] ?>" />
				R$ <?=number_format($valor_pago,2,',','.') ?><br>
				<?=campo_texto("valor_ajustado[{$dado['empid']}][{$dado['sbaid']}_{$dado['pobano']}][{$pagamento['pagid']}]","S","S","Usuário","10","20","[.###],##","","","","",
							   "id='valor_ajustado_{$dado['empid']}_{$dado['sbaid']}_{$dado['pobano']}_{$pagamento['pagid']}'", "", $valor_pago) ?>
			</td>
<?php 
		}
?>
		</tr>
<?	
	}
?>
		<tr>
			<th colspan="4" >Diferença entre valor do pagamento e os valores distribuidos nas subações.</th>
<?php 
	foreach( $pagamentos as $k => $pagamento ){
?>
			<th>
				<label 	style="color:blue" id="dif_pag_<?=$_REQUEST['empid'] ?>_<?=$pagamento['pagid'] ?>"
						title="Diferença entre valor do pagamento e os valores distribuidos nas subações." >(R$ 0,00)</label>
			</th>
<?php
	}
?> 	
		</tr>
	</table>
<?
}

function listaEmpenhosEmpenhoSubacao(){
	
	global $db;
	
	$sql = "SELECT es.processo, e.empid, e.empnumero, e.saldo AS empenho, sum(es.saldo) as subacao 
			FROM par.v_dadosempenhosubacao es
			INNER JOIN par.v_dadosempenhos e on e.empid = es.empid
			WHERE es.processo = '{$_REQUEST['processo']}'  
			GROUP BY es.processo, e.empnumero, e.empid, e.saldo, es.saldo
			HAVING sum(es.saldo) <> e.saldo";
						
// 			SELECT
// 				emp.empid, emp.empnumero, vem.saldo as empvalorempenho, sum(ems.eobvalorempenho) as vlr_emp_sub
// 			FROM 
// 				par.empenho emp
// 			INNER JOIN par.v_dadosempenhos	vem ON vem.empid = emp.empid
// 			INNER JOIN par.empenhosubacao 	ems ON ems.empid = emp.empid
// 			WHERE
// 				emp.empnumeroprocesso = '{$_REQUEST['processo']}'
// 				AND emp.empsituacao <> 'CANCELADO'
// 				AND emp.empstatus = 'A'
// 				AND ems.eobstatus = 'A'
// 			GROUP BY
// 				emp.empid, emp.empnumero, emp.empvalorempenho, vem.saldo
// 			HAVING
// 				sum(ems.eobvalorempenho) <> vem.saldo";

	$dados = $db->carregar($sql);
	$dados = is_array($dados) ? $dados : Array();
?>
<form id=formAjustePagamento >
	<label style="color:red;font-size:12px;"> Por favor ajuste os pagamentos de acordo com o que foi empenhado em cada subação. </label>
	<table width=100% >
		<tr>
			<th>Empenho / Vlr Empenho</th>
			<th> Vlr Empenhado<br>em Subações</th>
		</tr>
<?
	foreach( $dados as $dado ){
?>
		<tr>
			<td><?=$dado['empnumero'] ?> / R$ <?=number_format($dado['empvalorempenho'],2,',','.') ?></td>
			<td>R$ <?=number_format($dado['vlr_emp_sub'],2,',','.') ?></td>
		</tr>
<?	
	}
?>
	</table>
</form>
<?
}

function listaempenhos(){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				emp.empnumero, emp.empid, vem.saldo as empvalorempenho, emp.empnumeroprocesso,
				valor_empenhado, sum(valor_pago) as valor_pago
			FROM 
				par.empenho emp
			INNER JOIN par.v_dadosempenhos	vem ON vem.empid = emp.empid
			INNER JOIN par.pagamento 		pag ON pagsituacaopagamento not ilike '%CANCELADO%' AND pag.empid = emp.empid AND pag.pagstatus = 'A'
			INNER JOIN (
				SELECT
					empid,
					sum(eobvalorempenho) as valor_empenhado
				FROM
					par.empenhosubacao
				WHERE
					eobstatus = 'A'
				GROUP BY
					empid ) eob ON eob.empid = emp.empid 
			INNER JOIN (
				SELECT
					pagid,
					sum(pobvalorpagamento) as valor_pago
				FROM
					par.pagamentosubacao
				GROUP BY
					pagid ) pob ON pob.pagid = pag.pagid 
			WHERE 
				empnumeroprocesso = '{$_REQUEST['processo']}'
				AND emp.empsituacao <> 'CANCELADO'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				AND emp.empstatus = 'A'
			GROUP BY
				emp.empnumero, emp.empid, emp.empvalorempenho, emp.empnumeroprocesso, valor_empenhado, vem.saldo";

	$dados = $db->carregar($sql);
	$dados = is_array($dados) ? $dados : Array();
?>
<form id=formAjustePagamento >
	<label style="color:red;font-size:12px;"> Por favor ajuste os pagamentos de acordo com o que foi empenhado em cada subação. </label>
	<table width=100% >
		<tr>
			<th>Empenho/Empenhado</th>
			<th>Pago</th>
		</tr>
<?
	foreach( $dados as $dado ){

		$icon = '';
		if( verificaPagamentoMaiorQueEmpenhoSubacao( $_REQUEST['processo'], "AND emp.empid = {$dado['empid']}" ) ){
?>
		<tr>
			<td>
			<?php if( !$_REQUEST['nlistar'] ){?>
				<img border=0 style=cursor:pointer title="Redistribuir valores pagos às subações." class=listaSubacoesEmpenho src=../imagens/refresh2.gif empid=<?=$dado['empid'] ?> >
			<?php }?>
				<?=$icon ?> <?=$dado['empnumero'] ?> / R$ <?=number_format($dado['valor_empenhado'],2,',','.') ?></td>
			<td>R$ <?=number_format($dado['valor_pago'],2,',','.') ?></td>
		</tr>
<?	
		}
	}
?>
	</table>
</form>
<?
}

function verificaPagamentoMaiorQueEmpenhoSubacao( $processo, $filtro = '' ){
	
	global $db;
	
	$sql = sql_verificaPagamentoMaiorQueEmpenhoSubacao();
	
	$sql = str_replace(Array('%processo%', '%filtro%'), Array("'".$_REQUEST['processo']."'",$filtro), $sql);

	$pagamentoMaiorQueEmpenho = $db->pegaUm($sql);
	
	return ($pagamentoMaiorQueEmpenho != '');
}

function verificaEmpenhoDifereEmpenhoSubacao( $processo, $filtro = '' ){
	
	global $db;
	
	$sql = "SELECT
				emp.empid, vem.saldo as empvalorempenho, sum(ems.eobvalorempenho)
			FROM 
				par.empenho emp
			INNER JOIN par.v_dadosempenhos	vem ON vem.empid = emp.empid
			INNER JOIN par.empenhosubacao 	ems ON ems.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
			WHERE
				emp.empnumeroprocesso = '{$_REQUEST['processo']}'
				AND emp.empsituacao <> 'CANCELADO'
				AND emp.empstatus = 'A'
				$filtro
			GROUP BY
				emp.empid, emp.empvalorempenho, vem.saldo
			HAVING
				sum(ems.eobvalorempenho) <> vem.saldo";

	$pagamentoMaiorQueEmpenho = $db->pegaUm($sql);
	
	return ($pagamentoMaiorQueEmpenho != '');
}

if( $_REQUEST['reqAjaxDCPE'] != '' ){
	ob_clean();
	$_REQUEST['reqAjaxDCPE']();
	die();
}

$sql = "select count(p.pagid)
		from par.pagamento p
		  left join par.pagamentosubacao o on p.pagid = o.pagid
		  inner join par.empenho e on e.empid = p.empid and e.empstatus = 'A'
		where p.pagstatus = 'A' and e.empnumeroprocesso = '{$_REQUEST['processo']}'
		group by e.empnumeroprocesso
		having count(o.pobid) = 0";
$totPagSemComp = $db->pegaUm($sql);

/*if( verificaEmpenhoDifereEmpenhoSubacao( $_REQUEST['processo'] ) ){
?>
<div id="div_dialog" style="display:none">
</div>
<script>
$(document).ready(function(){

	$.ajax({
		type: 	"POST",
		url: 	window.location.href,
		data: 	'reqAjaxDCPE=listaEmpenhosEmpenhoSubacao&processo=<?=$_REQUEST['processo'] ?>',
		async: 	false,
		success: function(msg){
			$( "#div_dialog" ).attr('title', 'Redristribuição de pagamentos.');
			$( "#div_dialog" ).html('<label style="color:red;font-size:12px;">'+ 
										'Esse pagamento possui inconsistências em seus empenhos.<br>'+
										'Favor acessar a tela dos respectivos empenhos e ajusta-los.'+ 
									'</label>'+msg);		
			$( "#div_dialog" ).show();		
			$( "#div_dialog" ).dialog({
				resizable: true,
				width: 750,
				modal: true,
				closeOnEscape: false,
				show: { effect: 'drop', direction: "up" },
				open: function(event, ui) { $(".ui-dialog-titlebar-close").hide(); },
				buttons: {}
			});
		}
	});
});
</script>
<?php 
}else*/ if( verificaPagamentoMaiorQueEmpenhoSubacao( $_REQUEST['processo'] ) || (int)$totPagSemComp > 0 ){
?>
<div id="div_dialog" style="display:none"></div>
<div id="div_alert_dialog"  style="display:none"></div>
<script>
$(document).ready(function(){

	$('input[type="text"][id*="valor_ajustado_"]').live('blur',function(){
		var arrDados = $(this).attr('id').split('_');
		var empid 	= arrDados[2];
		var sbaid 	= arrDados[3];
		var pobano 	= arrDados[4];
		var pagid 	= arrDados[5];
		var vlrLinha 	= 0;
		var vlrColuna 	= 0;
		var vlr = '';
		var cor = '';
		$('input[type="text"][id*="valor_ajustado_'+empid+'_'+sbaid+'_'+pobano+'"]').each(function(){
			if( $(this).val() != '' ){
				vlrLinha += parseFloat($(this).val().replace('.','').replace('.','').replace(',','.'));
			}
		});
		
		vlr = mascaraglobal('[.###],##', ( parseFloat($('input[id="valor_empenhado_'+empid+'_'+sbaid+'_'+pobano+'"]').val())-parseFloat(vlrLinha) ).toFixed(2) );
		if( parseFloat( parseFloat($('input[id="valor_empenhado_'+empid+'_'+sbaid+'_'+pobano+'"]').val())-parseFloat(vlrLinha) ) < 0 ){
			cor = 'red';
		}else{
			cor = 'blue';
		}
		var valor = vlr.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");

		var percent = ((valor*100) / parseFloat($('input[id="valor_empenhado_'+empid+'_'+sbaid+'_'+pobano+'"]').val())  );
				
		$('#dif_'+empid+'_'+sbaid+'_'+pobano).html('<label style="cursor:pointer;color:'+cor+'" >(R$ '+vlr+') '+parseFloat(percent).toFixed(2)+'%</label>');

		$('input[type="text"][id^="valor_ajustado_'+empid+'"][id*="_'+pagid+'"]').each(function(){
			if( $(this).val() != '' ){
				vlrColuna += parseFloat($(this).val().replace('.','').replace('.','').replace(',','.'));
			}
		});
		if( parseFloat( parseFloat($('input[id="pagid_'+empid+'_'+pagid+'"]').val())-parseFloat(vlrColuna) ) < 0 ){
			cor = 'red';
		}else{
			cor = 'blue';
		}
		percent = ( (( parseFloat($('input[id="pagid_'+empid+'_'+pagid+'"]').val())-parseFloat(vlrColuna) )*100) / parseFloat($('input[id="pagid_'+empid+'_'+pagid+'"]').val())  );
		vlr = mascaraglobal('[.###],##', ( parseFloat($('input[id="pagid_'+empid+'_'+pagid+'"]').val())-parseFloat(vlrColuna) ).toFixed(2) );
		$('#dif_pag_'+empid+'_'+pagid).html('<label style="cursor:pointer;color:'+cor+'">(R$ '+vlr+') '+parseFloat(percent).toFixed(2)+'%</label>');
	});
	
	$('.listaSubacoesEmpenho').live('click', function(){
		
		var este = $(this);
		var empid = $(this).attr('empid');
		
		if( $('#obras'+empid).html() != '' ){
			
			$('#obras'+empid).remove();
		}
			
		$.ajax({
			type: 	"POST",
			url: 	window.location.href,
			data: 	'reqAjaxDCPE=listaSubacoesEmpenho&processo=<?=$_REQUEST['processo'] ?>&empid='+empid,
			async: 	false,
			success: function(msg){
				$( este ).parent().parent().after('<tr id=obras'+empid+' ><td colspan=2 >'+msg+'</td></tr>');	
				$('input[type="text"]').blur();
			}
		});
	});
	
	$.ajax({
		type: 	"POST",
		url: 	window.location.href,
		data: 	'reqAjaxDCPE=listaempenhos&processo=<?=$_REQUEST['processo'] ?>',
		async: 	false,
		success: function(msg){
			$( "#div_dialog" ).attr('title', 'Redristribuição de pagamentos.');
			$( "#div_dialog" ).html(msg);		
			$( "#div_dialog" ).show();		
			$( "#div_dialog" ).dialog({
				resizable: true,
				width: 1200,
				modal: true,
				closeOnEscape: false,
				show: { effect: 'drop', direction: "up" },
				open: function(event, ui) { $(".ui-dialog-titlebar-close").hide(); },
				buttons: {
					"Salvar": function() {
						$.ajax({
					   		type: 	"POST",
					   		url: 	window.location.href,
					   		data: 	'reqAjaxDCPE=salvarAjustes&'+$('#formAjustePagamento').serialize(),
					   		async: 	false,
					   		success: function(msg){
						   		if( msg == 'sucesso' ){
							   		$( "#div_alert_dialog" ).attr('title', 'Sucesso');
						   			$( "#div_alert_dialog" ).css('font-size', '12px');
						   			$( "#div_alert_dialog" ).html('<center><b>Ajuste(s) de pagamento salvo(s) com sucesso!</b></center>');		
						   			$( "#div_alert_dialog" ).show();		
						   			$( "#div_alert_dialog" ).dialog({
										resizable: true,
										width: 600,
										modal: true,
										show: { effect: 'drop', direction: "up" },
										buttons: {
											"OK": function() {
												$( this ).dialog( "close" );
									   			$( '#div_dialog' ).dialog( "close" );
									   			window.location.reload();
											}
										}
									});
							   	}else{
						   			$( "#div_alert_dialog" ).attr('title', 'Erro ao salvar ajustes de pagamento');
						   			$( "#div_alert_dialog" ).css('font-size', '11px');
						   			$( "#div_alert_dialog" ).html(msg);		
						   			$( "#div_alert_dialog" ).show();		
						   			$( "#div_alert_dialog" ).dialog({
										resizable: true,
										width: 700,
										modal: true,
										show: { effect: 'drop', direction: "up" },
										buttons: {
											"Close": function() {
												$( this ).dialog( "close" );
											}
										}
									});
								}
					   		}
						});
					}
				}
			});
		}
	});
});
</script>
<?php 
}
?>