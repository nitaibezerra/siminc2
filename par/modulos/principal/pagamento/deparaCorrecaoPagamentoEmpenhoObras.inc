<?php 

//include_once APPRAIZ."par/modulos/principal/pagamento/deparaCorrecaoPagamentoEmpenho.inc";
function salvarAjustes(){
	
	global $db;
	
	if( is_array($_REQUEST['valor_ajustado']) ){
		$arrAjustes = Array();
		$strErro = '';
		foreach( $_REQUEST['valor_ajustado'] as $empid => $valoresObras ){
			foreach( $valoresObras as $preid => $ajustes ){
				$vlrAjustesLinha = 0;
				foreach( $ajustes as $pagid => $ajuste ){
					$arrAjustesColuna[$pagid] += str_replace(Array('.',','), Array('','.'), trim($ajuste));
					$vlrAjustesLinha += str_replace(Array('.',','), Array('','.'), trim($ajuste));
				}
				
				if( trim($vlrAjustesLinha) > trim($_REQUEST['valor_empenhado'][$empid][$preid]+0.01) ){
// 				if( ($vlrAjustesLinha - $_REQUEST['valor_empenhado'][$empid][$preid]) > 0.01 ){
					$strErro .= " - Ajuste de pagamento da obra n° $preid excede o empenho em R$ ".number_format( ($vlrAjustesLinha - $_REQUEST['valor_empenhado'][$empid][$preid]) ,2,',','.').".<Br>";
				}
			}
			foreach( $_REQUEST['pagid'][$empid] as $pagid => $valorPagamento ){
				if( strpos($arrAjustesColuna[$pagid], '.') !== false ){
					$arrAjustesColuna[$pagid] 	= rtrim($arrAjustesColuna[$pagid],'0');
				}
				if( strpos($valorPagamento, '.') !== false ){
					$valorPagamento 			= rtrim($valorPagamento,'0');
				}
				if( $arrAjustesColuna[$pagid] < $valorPagamento || $arrAjustesColuna[$pagid] > $valorPagamento ){
					$strErro .= " - O(s) ajuste(s) na OB {$_REQUEST['ob'][$empid][$pagid]} é diferente do valor da mesma.<Br>";
				}
			}
		}
	}
	if( $strErro != '' ){
		echo "<label style=color:red; >$strErro</label>";
	}elseif( is_array($_REQUEST['valor_ajustado']) ){
		$tblPar = '';
		$colPar = 'pob';
		if( $_REQUEST['tooid'] != 1 ){
			$tblPar = 'par';
			$colPar = 'pop';
		}
		$sql = "";
		foreach( $_REQUEST['valor_ajustado'] as $empid => $valoresObras ){
			foreach( $valoresObras as $preid => $ajustes ){
				foreach( $ajustes as $pagid => $ajuste ){
					$ajuste = str_replace(Array('.',','), Array('','.'), trim($ajuste));
					$sqlPag = "SELECT true FROM par.pagamentoobra$tblPar WHERE preid = $preid AND pagid = $pagid;";
					$temPagamento = $db->pegaUm($sqlPag);
					if( $temPagamento == 't' ){
						$sql .= "UPDATE par.pagamentoobra$tblPar SET {$colPar}valorpagamento = ".($ajuste ? $ajuste : '0')." WHERE preid = $preid AND pagid = $pagid;";
					}else{
						if( $ajuste > 0 ){
							$sql .= "INSERT INTO par.pagamentoobra$tblPar(preid, pagid, {$colPar}valorpagamento, {$colPar}percentualpag) VALUES($preid, $pagid, $ajuste, 1);";
						}
					}
					unset($temPagamento);
					unset($ajuste);
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
	}else{
		echo 'Favor preencher os valores a serem ajustados.';
	}
}

function listaObrasEmpenho(){
	
	global $db;
	
	$tblPar = '';
	$colPar = 'pob';
	if( $_REQUEST['tooid'] != 1 ){
		$tblPar = 'par';
		$colPar = 'pop';
	}
	
	$sql = "SELECT DISTINCT
				emp.empid, eob.preid, 
				(SELECT obrid FROM obras2.obras obr WHERE obr.preid = eob.preid and obr.obrstatus = 'A' and obr.obridpai is null) as obrid, 
				eob.eobvalorempenho as valor_empenhado, sum( pob.{$colPar}valorpagamento ) as valor_pago, 
				(SELECT prevalorobra FROM obras.preobra pre WHERE pre.preid = eob.preid and pre.prestatus = 'A') as prevalorobra
			FROM 
				par.empenho emp 
			INNER JOIN par.empenhoobra$tblPar 	eob ON eob.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
			INNER JOIN par.pagamento 			pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' and parnumseqob is not null
			LEFT  JOIN par.pagamentoobra$tblPar pob ON pob.preid = eob.preid AND pag.pagid = pob.pagid
			WHERE 
				emp.empid = {$_REQUEST['empid']}
				AND emp.empsituacao <> 'CANCELADO'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				AND emp.empstatus = 'A'
			GROUP BY
				emp.empid, eob.preid, eob.eobvalorempenho
			ORDER BY
				2";
	
	$dados = $db->carregar($sql);
	$dados = is_array($dados) ? $dados : Array();
	
	$sql = "SELECT DISTINCT
				pag.pagid, pag.pagnumeroob, pag.pagvalorparcela
			FROM
				par.empenho emp
			INNER JOIN par.empenhoobra$tblPar 	eob ON eob.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
			INNER JOIN par.pagamento 			pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' and parnumseqob is not null
			INNER JOIN par.pagamentoobra$tblPar pob ON pob.preid = eob.preid AND pag.pagid = pob.pagid
			INNER JOIN obras.preobra 			pre ON pre.preid = pob.preid
			WHERE
				emp.empid = {$_REQUEST['empid']}
				AND emp.empsituacao <> 'CANCELADO'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				AND emp.empstatus = 'A'
			ORDER BY
				1";
	
	$pagamentos = $db->carregar($sql);
	$pagamentos = is_array($pagamentos) ? $pagamentos : Array();
?>
	<table width=100% >
		<tr>
			<th rowspan="2" >ID <br>Pré-Obra</th>
			<th rowspan="2" >ID <br>Obra</th>
			<th rowspan="2" >Valor <br>Obra</th>
			<th rowspan="2" >Execução <br>Física</th>
			<th rowspan="2" >Valor empenhado</th>
			<th rowspan="2" >(Valor empenhado<br> - Valor Pago) %</th>
			<th colspan="<?=count($pagamentos)+1 ?>" > Pagamentos </th>
		</tr>
		<tr>
<?php 
	foreach( $pagamentos as $k => $pagamento ){
?>
			<th>
				<?=$pagamento['pagnumeroob'] ?><br>
				<label style="cursor:pointer" title="Valor do pagamento." > (R$ <?=number_format($pagamento['pagvalorparcela'],2,',','.') ?>)</label><br>
				<input type="hidden" name="pagid[<?=$_REQUEST['empid'] ?>][<?=$pagamento['pagid'] ?>]" 	value="<?=$pagamento['pagvalorparcela'] ?>" id="pagid_<?=$_REQUEST['empid'] ?>_<?=$pagamento['pagid'] ?>" />
				<input type="hidden" name="ob[<?=$_REQUEST['empid'] ?>][<?=$pagamento['pagid'] ?>]" 	value="<?=$pagamento['pagnumeroob'] ?>"/>
			</th>
<?php
	}
?> 	
			<th>
				Total:
			</th>
		</tr>
<?
	$totPago = 0;
	$totVlrObra = 0;
	$totVlrEmpenhadoObra = 0;
	foreach( $dados as $dado ){
		$vrldivergente = ($dado['valor_empenhado']-$dado['valor_pago']);
		//ver($vrldivergente);
		//valorempenhado*100/valorsubacao
		if( $dado['valor_pago'] > 0 ){
			$percent = (($vrldivergente*100) / $dado['valor_pago']);
		}else{
			$percent = 0;
		}
		if($dado['obrid'] != ''){
			
			$sql = "select 
						coalesce(v.vldstatushomologacao, 'N') as homologacao, 
						coalesce(v.vldstatus25exec, 'N') as validacao25, 
						coalesce(v.vldstatus50exec, 'N') as validacao50 
					from obras2.validacao v 
					where v.obrid = {$dado['obrid']}";
			$arValidacao = $db->pegaLinha($sql);
		}
		$execusaoFisica = (int)1;
		if( $arValidacao['homologacao'] == 'S' ) $execusaoFisica++;
		if( $arValidacao['validacao25'] == 'S' ) $execusaoFisica++;
		if( $arValidacao['validacao50'] == 'S' ) $execusaoFisica++;
?>
		<tr>
			<td><?=$dado['preid'] ?></td>
			<td><?=$dado['obrid'] ?></td>
			<td>R$ <?=number_format( $dado['prevalorobra'],2,',','.') ?></td>
			<td align="center"><? echo $execusaoFisica.'/4'; ?></td>
			<td>
				R$ <?=number_format($dado['valor_empenhado'],2,',','.') ?><br>
				<input type=hidden name=valor_empenhado[<?=$dado['empid'] ?>][<?=$dado['preid'] ?>] id=valor_empenhado_<?=$dado['empid'] ?>_<?=$dado['preid'] ?> value="<?=$dado['valor_empenhado'] ?>"/>
				<input type=hidden name=valor_pago_emp[<?=$dado['empid'] ?>][<?=$dado['preid'] ?>] id=valor_pago_emp_<?=$dado['empid'] ?>_<?=$dado['preid'] ?> value="<?=($dado['valor_pago']?$dado['valor_pago']:'0.00') ?>"/>
			</td>
			<td>
				<?php $cor = 'style="color:blue"';?> 
				<?php if( $vrldivergente < 0 ){ $cor = 'style="color:red"'; }?>
				<div  id="dif_<?=$dado['empid'] ?>_<?=$dado['preid'] ?>" title="Diferença entre o valor empenhado a obra e as parcelas pagas a mesma.">
					<label <?=$cor ?> >(R$ <?=number_format( $vrldivergente,2,',','.') ?>)</label>
				</div>
			</td>
<?php 
		$totPagoObra = 0;
		foreach( $pagamentos as $k => $pagamento ){

			$valor_pago = '';
			$sql = "SELECT
						pob.{$colPar}valorpagamento
					FROM
						par.empenho emp
					INNER JOIN par.empenhoobra$tblPar 	eob ON eob.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
					INNER JOIN par.pagamento 			pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' and parnumseqob is not null
					INNER JOIN par.pagamentoobra$tblPar pob ON pob.preid = eob.preid AND pag.pagid = pob.pagid
					INNER JOIN obras.preobra 			pre ON pre.preid = pob.preid
					WHERE
						emp.empid = {$_REQUEST['empid']}
						AND pob.pagid = {$pagamento['pagid']}
						AND pob.preid = {$dado['preid']}
						AND emp.empsituacao <> 'CANCELADO'
						AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
						AND emp.empstatus = 'A'";
					
			$valor_pago = $db->pegaUm($sql);
			$valor_pago = $valor_pago ? $valor_pago : '0.00';
?>
			<td>
				<input type=hidden value="<?=$valor_pago ?>" name="valor_pago[<?=$dado['empid'] ?>][<?=$dado['preid'] ?>][<?=$pagamento['pagid'] ?>]" id="valor_pago_<?=$dado['empid'] ?>_<?=$dado['preid'] ?>_<?=$pagamento['pagid'] ?>" />
				R$ <?=number_format($valor_pago,2,',','.') ?><br>
				<?=campo_texto("valor_ajustado[{$dado['empid']}][{$dado['preid']}][{$pagamento['pagid']}]","S","S","Usuário","10","20","[.###],##","","","","",
							   "id='valor_ajustado_{$dado['empid']}_{$dado['preid']}_{$pagamento['pagid']}'", "", $valor_pago) ?>
			</td>
<?php 
			$totPagoObra += $valor_pago;
		}
		$totPago 				+= $totPagoObra;
		$totVlrObra 			+= $dado['prevalorobra'];
		$totVlrEmpenhadoObra 	+= $dado['valor_empenhado'];
?>
			<td>
				<div id="div_tot_pag_obr_<?=$dado['empid'] ?>_<?=$dado['preid'] ?>" >R$ <?=number_format($totPagoObra,2,',','.') ?><br></div>
				<input type="hidden" id="tot_pag_obr_<?=$dado['empid'] ?>_<?=$dado['preid'] ?>"/>
			</td>
		</tr>		
<?	
	}
?>
		<tr>
			<th colspan="2" style="text-align:right" >Total:</th>
			<th style="text-align:left" >R$ <?=number_format($totVlrObra,2,',','.') ?></th>
			<th></th>
			<th style="text-align:left" >R$ <?=number_format($totVlrEmpenhadoObra,2,',','.') ?></th>
			<th></th>
<?php 
	foreach( $pagamentos as $k => $pagamento ){
?>
			<th>
				<div id="tot_pag_<?=$_REQUEST['empid'] ?>_<?=$pagamento['pagid'] ?>" > ( R$ 0,00 ) </div>
			</th>
<?php
	}
?> 	
			<th>
				<div id="tot_pag_<?=$_REQUEST['empid'] ?>"> R$ <?=number_format($totPago,2,',','.') ?> </div>
			</th>
		</tr>
		<tr>
			<th colspan="6" >Diferença entre valor do pagamento e os valores distribuidos nas obras.</th>
<?php 
	foreach( $pagamentos as $k => $pagamento ){
?>
			<th>
				<div style="cursor:pointer;color:blue;" id="dif_pag_<?=$_REQUEST['empid'] ?>_<?=$pagamento['pagid'] ?>" 
					   title="Diferença entre valor do pagamento e os valores distribuidos nas obras." > ( R$ 0,00 ) </div>
			</th>
<?php
	}
?> 	
			<th>
			</th>
		</tr>
	</table>
<?
}

function listaEmpenhosEmpenhoObra(){
	
	global $db;
	
	$tblPar = '';
	$colPar = 'pob';
	if( $_REQUEST['tooid'] != 1 ){
		$tblPar = 'par';
		$colPar = 'pop';
	}
	
	$sql = "SELECT DISTINCT es.processo, emp.empid, emp.empnumero, emp.saldo AS empenho, sum(es.saldo) as subacao 
			FROM par.v_dadosempenhoobra$tblPar es
			INNER JOIN par.v_dadosempenhos emp on emp.empid = es.empid
			WHERE es.processo = '{$_REQUEST['processo']}' $filtro  
			GROUP BY es.processo, emp.empnumero, emp.empid, emp.saldo
			HAVING sum(es.saldo) <> emp.saldo";
	
	$dados = $db->carregar($sql);
	$dados = is_array($dados) ? $dados : Array();
?>
<form id=formAjustePagamento >
	<label style="color:red;font-size:12px;"> Por favor ajuste os pagamentos de acordo com o que foi empenhado em cada obra. </label>
	<table width=100% >
		<tr>
			<th>Empenho / Vlr Empenho</th>
			<th>Vlr Empenhado<br> em Obras</th>
		</tr>
<?
	foreach( $dados as $dado ){
?>
		<tr>
			<td><?=$dado['empnumero'] ?> / R$ <?=number_format($dado['empenho'],2,',','.') ?></td>
			<td>R$ <?=number_format($dado['subacao'],2,',','.') ?></td>
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
	
	$tblPar = '';
	$colPar = 'pob';
	if( $_REQUEST['tooid'] != 1 ){
		$tblPar = 'par';
		$colPar = 'pop';
	}
	
	$sql = "SELECT DISTINCT
				emp.empnumero,
				emp.empid,
				vem.saldo as empvalorempenho,
				emp.empnumeroprocesso,
				valor_empenhado,
				sum(valor_pago) as valor_pago
			FROM 
				par.empenho emp
			INNER JOIN par.v_dadosempenhos	vem ON vem.empid = emp.empid
			INNER JOIN par.pagamento 		pag ON pagsituacaopagamento not ilike '%CANCELADO%' AND pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento not ilike '%CANCELADO%' and parnumseqob is not null
			INNER JOIN (
				SELECT
					empid,
					sum(eobvalorempenho) as valor_empenhado
				FROM
					par.empenhoobra$tblPar 
				WHERE
					eobstatus = 'A'
				GROUP BY
					empid ) eob ON eob.empid = emp.empid 
			INNER JOIN (
				SELECT
					pagid,
					sum({$colPar}valorpagamento) as valor_pago
				FROM
					par.pagamentoobra$tblPar 
				GROUP BY
					pagid ) pob ON pob.pagid = pag.pagid 
			WHERE 
				empnumeroprocesso = '{$_REQUEST['processo']}'
				AND emp.empsituacao <> 'CANCELADO'
				AND emp.empstatus = 'A'
			GROUP BY
				emp.empnumero,
				emp.empid,
				emp.empvalorempenho,
				emp.empnumeroprocesso,
				valor_empenhado,
				vem.saldo";

	$dados = $db->carregar($sql);
	$dados = is_array($dados) ? $dados : Array();
?>
<form id=formAjustePagamento >
	<label style="color:red;font-size:12px;"> Por favor ajuste os pagamentos de acordo com o que foi empenhado em cada obra. </label>
	<table width=100% >
		<tr>
			<th>Empenho/Empenhado</th>
			<th>Pago</th>
		</tr>
<?
	foreach( $dados as $dado ){

		$icon = '';
		if( verificaPagamentoMaiorQueEmpenhoObra( $_REQUEST['processo'], "AND emp.empid = {$dado['empid']}" ) ){ ?>
			<tr>
				<td>
					<img border=0 style=cursor:pointer title="Redistribuir valores pagos às obras." class=listaObrasEmpenho src=../imagens/refresh2.gif empid=<?=$dado['empid'] ?> >
					<?=$icon ?> <?=$dado['empnumero'] ?> / R$ <?=number_format($dado['valor_empenhado'],2,',','.') ?></td>
				<td>R$ <?=number_format($dado['valor_pago'],2,',','.') ?></td>
			</tr><?	
		}
	}
?>
	</table>
</form>
<?
}

function verificaEmpenhoDiferenteQueEmpenhoObra( $processo, $filtro = '' ){
	
	global $db;
	
	$tblPar = '';
	if( $_REQUEST['tooid'] != 1 ){
		$tblPar = 'par';
	}
	
	$sql = "SELECT es.processo, emp.empid, emp.empnumero, emp.saldo AS empenho, sum(es.saldo) as subacao 
			FROM par.v_dadosempenhoobra$tblPar es
			INNER JOIN par.v_dadosempenhos emp on emp.empid = es.empid
			WHERE es.processo = '{$_REQUEST['processo']}' $filtro  
			GROUP BY es.processo, emp.empnumero, emp.empid, emp.saldo
			HAVING  sum(es.saldo) <> emp.saldo";
// ver($sql,d);
	$pagamentoMaiorQueEmpenho = $db->pegaUm($sql);
	
	return ($pagamentoMaiorQueEmpenho != '');
}

function verificaPagamentoMaiorQueEmpenhoObra( $processo, $filtro = '' ){
	
	global $db;
	
	$sql = sql_verificaPagamentoMaiorQueEmpenhoObra();
	
	$sql = str_replace(Array('%processo%', '%filtro%'), Array("'$processo'",$filtro), $sql);

	$pagamentoMaiorQueEmpenho = $db->pegaUm($sql);
	
	return ($pagamentoMaiorQueEmpenho != '');
}

if( $_REQUEST['reqAjaxDCPE'] != '' ){
	ob_clean();
	$_REQUEST['reqAjaxDCPE']();
	die();
}

/*if( verificaEmpenhoDiferenteQueEmpenhoObra( $_REQUEST['processo'] ) ){
?>
<div id="div_dialog" style="display:none">
</div>
<script>
$(document).ready(function(){

	$.ajax({
		type: 	"POST",
		url: 	window.location.href,
		data: 	'reqAjaxDCPE=listaEmpenhosEmpenhoObra&processo=<?=$_REQUEST['processo'] ?>',
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
				width: 900,
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
}else*/ if( verificaPagamentoMaiorQueEmpenhoObra( $_REQUEST['processo'] ) ){
?>
<div id="div_dialog" style="display:none"></div>
<div id="div_alert_dialog"  style="display:none"></div>
<script>
$(document).ready(function(){

	$('input[type="text"][id*="valor_ajustado_"]').live('blur',function(){
		var arrDados = $(this).attr('id').split('_');
		var empid = arrDados[2];
		var preid = arrDados[3];
		var pagid = arrDados[4];
		var vlrLinha 	= 0;
		var vlrColuna 	= 0;
		var vlr = '';
		var cor = '';
		$('input[type="text"][id*="valor_ajustado_'+empid+'_'+preid+'"]').each(function(){
			if( $(this).val() != '' ){
				vlrLinha += parseFloat($(this).val().replace('.','').replace('.','').replace(',','.'));
			}
		});
		var valor_pago_emp  = $('input[id="valor_pago_emp_'+empid+'_'+preid+'"]').val();
		var valor_empenhado = $('input[id="valor_empenhado_'+empid+'_'+preid+'"]').val();
		
		vlr = mascaraglobal('[.###],##', ( parseFloat(valor_empenhado)-parseFloat(vlrLinha) ).toFixed(2) );
		if( parseFloat( parseFloat($('input[id="valor_empenhado_'+empid+'_'+preid+'"]').val())-parseFloat(vlrLinha) ) < 0 ){
			cor = 'red';
		}else{
			cor = 'blue';
		}
		var valor = vlr.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");

		if( valor_pago_emp > 0 ){
			var percent = ((valor*100) / valor_pago_emp);
		}else{
			var percent = 0;
		}
				
		$('#div_tot_pag_obr_'+empid+'_'+preid).html('R$ '+mascaraglobal('[.###],##', ( parseFloat(vlrLinha) ).toFixed(2) ));
		$('#tot_pag_obr_'+empid+'_'+preid).val( parseFloat(vlrLinha) );
		$('#dif_'+empid+'_'+preid).html('<label style="cursor:pointer;color:'+cor+'">(R$ '+vlr+') '+parseFloat(percent).toFixed(2)+'%</label>');

		var tot_pag = 0;
		$('input[id*=tot_pag_obr_]').each(function(){
			tot_pag += parseFloat($(this).val());
		});
		$('#tot_pag_'+empid).html('R$ '+mascaraglobal('[.###],##', ( parseFloat(tot_pag) ).toFixed(2) ));
		
		$('input[type="text"][id*="valor_ajustado_'+empid+'"][id*="_'+pagid+'"]').each(function(){
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
		$('#tot_pag_'+empid+'_'+pagid).html('R$ '+mascaraglobal('[.###],##', ( parseFloat(vlrColuna) ).toFixed(2) )+'');
		$('#dif_pag_'+empid+'_'+pagid).html('<label style="cursor:pointer;color:'+cor+'">(R$ '+vlr+') '+parseFloat(percent).toFixed(2)+'%</label>');
	});
	
	$('.listaObrasEmpenho').live('click', function(){
		
		var este = $(this);
		var empid = $(this).attr('empid');
		
		if( $('#obras'+empid).html() != ''  ){
			
			$('#obras'+empid).remove();
		}
		
		$.ajax({
			type: 	"POST",
			url: 	window.location.href,
			data: 	'reqAjaxDCPE=listaObrasEmpenho&processo=<?=$_REQUEST['processo'] ?>&tooid=<?=$_REQUEST['tooid'] ?>&empid='+empid,
			async: 	false,
			success: function(msg){
				$( este ).parent().parent().after('<tr id=obras'+empid+' ><td colspan=2 >'+msg+'</td></tr>');	
				$('input[type="text"]').blur();
			}
		});
		jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '12px');
	});
	
	$.ajax({
		type: 	"POST",
		url: 	window.location.href,
		data: 	'reqAjaxDCPE=listaempenhos&processo=<?=$_REQUEST['processo'] ?>&tooid=<?=$_REQUEST['tooid'] ?>',
		async: 	false,
		success: function(msg){
			$( "#div_dialog" ).attr('title', 'Redristribuição de pagamentos.');
			$( "#div_dialog" ).html(msg);		
			$( "#div_dialog" ).show();		
			$( "#div_dialog" ).dialog({
				resizable: true,
				width: 1300,
				modal: true,
				closeOnEscape: false,
				show: { effect: 'drop', direction: "up" },
				open: function(event, ui) { $(".ui-dialog-titlebar-close").hide(); },
				buttons: {
					"Salvar": function() {
						$.ajax({
					   		type: 	"POST",
					   		url: 	window.location.href,
					   		data: 	'reqAjaxDCPE=salvarAjustes&tooid=<?=$_REQUEST['tooid'] ?>&'+$('#formAjustePagamento').serialize(),
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