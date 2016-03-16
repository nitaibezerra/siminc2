<?php


// Implementação futura, código sem uso a princípio, porém será implementado em futura demanda
if( $_REQUEST['requisicao'] == 'gerar_xls' )
{
	$excel = true;
	$file_name="pagamentoDesembolso.xls";
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Content-Type: application/force-download");
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Disposition: attachment;filename=".$file_name);
	header("Content-Transfer-Encoding: binary ");
	$abreCenter = '';
	$fechaCenter = '';
}
else
{
	$excel = false;
	$abreCenter = '<center>';
	$fechaCenter = '</center>';
}

if( $_REQUEST['requisicao'] == 'carregaMunicipios' )
{
	echo carregaMunicipios( $_REQUEST );
	exit();
}

if( ! $excel )
{

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

monta_titulo('Pagamento Desembolso', '');
?>
	<script type="text/javascript" src="./js/par.js"></script>
	<form method="post" name="formulario" id="formulario" action="" class="formulario" >
	    <input type="hidden" name="requisicao" id="requisicao" value=""/>
	
	    <table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	        <tr bgcolor="#cccccc">
	            <td style="text-align: center;" colspan="6"><strong>Filtros</strong></td>
	        </tr>
	          <tr>
				<td class="subtituloDireita">UF:</td>
				<td colspan="3">
					<?php
						$estuf = $_POST['estuf'];
						$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao FROM territorios.estado e ORDER BY e.estdescricao ASC";
						$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
					?>
				</td>
			</tr>
			<tr id="tr_muncod">
				<td class="subtituloDireita"> Município:</td>
				<td id="td_muncod" colspan="3">
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
	            <td class="SubTituloDireita" style="width: 15%">Processo:</td>
	            <td style="width: 35%"><?php
	            $processo = $_REQUEST['processo'];
	            echo campo_texto('processo', 'N', $habilitado, 'Processo', 30, 20, '#####.######/####-##', '', '', '', '', 'id="processo"', '',$processo ); ?></td>
	        </tr>
	        <tr>
	        	<td class="SubTituloDireita">Preid:</td>
	            <td style="width: 35%"><?php
	            	$preid = $_REQUEST['preid'];
	            	echo campo_texto('preid', 'N', 'S', 'Id da Ação', 15, 15, '[#]', '', '', '', '', 'id="dncrn"', '',$preid); ?></td>
	        </tr>
	        <tr>
	        	<td class="SubTituloDireita">Obrid:</td>
	            <td style="width: 35%"><?php
	            	$obrid = $_REQUEST['obrid'];
	            	echo campo_texto('obrid', 'N', 'S', 'Id da Ação', 15, 15, '[#]', '', '', '', '', 'id="obrid"', '',$obrid); ?></td>
	        </tr>
	         <tr>
				<td class="subtituloDireita">Tipo de obra:</td>
				<td colspan="3">
					<?php
						$ptoid = $_POST['ptoid'];
						$sql = "select ptoid as codigo, ptodescricao as descricao from obras.pretipoobra  where ptostatus = 'A' ORDER BY descricao";
						$db->monta_combo( "ptoid", $sql, 'S', 'Selecione', '', '','','','',$ptoid );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Situação da Obra:</td>
				<td colspan="3">
					<?php
						$situacao = $_POST['situacao'];
						$sql = "select distinct esddsc as codigo, esddsc as descricao from workflow.estadodocumento  where esdstatus = 'A' and tpdid in (45, 37) order by esddsc";
						$db->monta_combo( "situacao", $sql, 'S', 'Selecione', '', '','','','',$situacao );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Situação da Obras 2:</td>
				<td colspan="3">
					<?php
						$strid = $_POST['strid'];
						$sql = "SELECT strid as codigo, strdsc as descricao FROM obras2.situacao_registro str where str.strstatus = 'A' order by descricao";
						$db->monta_combo( "strid", $sql, 'S', 'Selecione', '', '','','','',$strid );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Itens Pendentes para Pagar:</td>
				<td colspan="3">
					<input <?php echo ($_REQUEST['itenspendente'] || empty($_REQUEST['itenspendente'])) == "t" ? "checked='checked'" : "" ?> type="radio" name="itenspendente" value="t" >Sim
					<input <?php echo $_REQUEST['itenspendente'] == "f" ? "checked='checked'" : "" ?> type="radio" name="itenspendente" value="f" >Não
					<input <?php echo $_REQUEST['itenspendente'] == "todos" ? "checked='checked'" : "" ?> type="radio" name="itenspendente" value="todos" >Ver Todos
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Obras 100% Paga:</td>
				<td colspan="3">
					<input <?php echo $_REQUEST['obra100paga'] == "t" ? "checked='checked'" : "" ?> type="radio" name="obra100paga" value="t" >Sim
					<input <?php echo ($_REQUEST['obra100paga'] == "f" || empty($_REQUEST['obra100paga']) ) ? "checked='checked'" : "" ?> type="radio" name="obra100paga" value="f" >Não
					<input <?php echo $_REQUEST['obra100paga'] == "todos" ? "checked='checked'" : "" ?> type="radio" name="obra100paga" value="todos" >Ver Todos
				</td>
			</tr>	        
			<tr>
				<td class="subtituloDireita"> </td>
				<td>
					<input type="button" class="pesquisar" value="Pesquisar"/>
					<input type="button" value="Todos" onclick="window.location.href = window.location"/>
					<input type="button" class="excel" value="Gerar Excel" onclick=""/>
				</td>
			</tr>
		</table>
	</form>
	<div id="div_nutricionista" title="Dados Nutricionista" style="display: none; text-align: center;">
		<div style="padding:5px;text-align:justify;" id="mostra_nutricionista"></div>
	</div>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#cccccc">
	    	<td style="text-align: center;" colspan="4"><strong>Lista</strong></td>
	    </tr>
	</table>

<?php 
}

$uf 		= $_REQUEST['estuf'];
$muncod 	= $_REQUEST['muncod'];
$processo 	= str_replace(Array('-', '/', '.'), '', $_REQUEST['processo']);
$preid 		= $_REQUEST['preid'];
$obrid 		= $_REQUEST['obrid'];
$ptoid 		= $_REQUEST['ptoid'];
$situacao	= $_REQUEST['situacao'];
$strid		= $_REQUEST['strid'];
$itenspendente = ($_REQUEST['itenspendente'] ? $_REQUEST['itenspendente'] : 't');
$obra100paga = ($_REQUEST['obra100paga'] ? $_REQUEST['obra100paga'] : 'f');
    
if( $uf ) 		$filtro .= " AND pre.estuf = '{$uf}'";
if( $muncod ) 	$filtro .= " AND pre.muncod = '{$muncod}'";
if( $processo )	$filtro .= " AND 
							(
								CASE WHEN pro.proid IS NOT NULL THEN
									pro.pronumeroprocesso = '{$processo}'
								WHEN pop.proid IS NOT NULL THEN
									pop.pronumeroprocesso = '{$processo}'
								END
							)";
if( $preid )   	$filtro .= "  AND pre.preid 	= {$preid}";
if( $obrid )   	$filtro .= "  AND o.obrid 		= {$obrid}";
if( $ptoid )   	$filtro .= "  AND pto.ptoid 	= {$ptoid}";
if( $situacao )	$filtro .= "  AND esd.esddsc 	= '{$situacao}'";
if( $strid )	$filtro .= "  AND str.strid 	= '{$strid}'";
if( $strid )	$filtro .= "  AND str.strid 	= '{$strid}'";

$filtroFoo = '';
if( $itenspendente == 't' ){
	$filtroFoo = "  where (round((coalesce(foo.liberado_pagamento,0) - (((coalesce(foo.valor_pago,0)) * 100) / foo.prevalorobra )),2)) > 0 ";
}
if( $itenspendente == 'f' ){
	$filtroFoo = "  where (round((coalesce(foo.liberado_pagamento,0) - (((coalesce(foo.valor_pago,0)) * 100) / foo.prevalorobra )),2)) <= 0 ";
}

if( $obra100paga == 't' ){
	$filtro .= " and pre.preid in (select preid
									from(
									    SELECT distinct
									        po.preid,
									        --sum(po.pobvalorpagamento) as vlrpagamento,
									        --cast(p.prevalorobra as numeric(20,2)) as vlrobra,
									        cast(p.prevalorobra as numeric(20,2)) - sum(po.pobvalorpagamento) as totalobra
									    FROM obras.preobra p 
									        inner join par.pagamentoobra po on po.preid = p.preid
									        inner join par.pagamento pg on pg.pagid = po.pagid
									    WHERE 
									        p.prestatus = 'A'
									        and pg.pagsituacaopagamento not ilike '%cancelado%' 
									        and pg.pagsituacaopagamento not ilike '%devolvido%'
									        and pg.pagstatus = 'A'
									    GROUP BY p.prevalorobra, po.preid
									) as foo
									where totalobra < 2)";
} elseif( $obra100paga == 'f' ){
	$filtro .= " and pre.preid not in (select preid
									from(
									    SELECT distinct
									        po.preid,
									        --sum(po.pobvalorpagamento) as vlrpagamento,
									        --cast(p.prevalorobra as numeric(20,2)) as vlrobra,
									        cast(p.prevalorobra as numeric(20,2)) - sum(po.pobvalorpagamento) as totalobra
									    FROM obras.preobra p 
									        inner join par.pagamentoobra po on po.preid = p.preid
									        inner join par.pagamento pg on pg.pagid = po.pagid
									    WHERE 
									        p.prestatus = 'A'
									        and pg.pagsituacaopagamento not ilike '%cancelado%' 
									        and pg.pagsituacaopagamento not ilike '%devolvido%'
									        and pg.pagstatus = 'A'
									    GROUP BY p.prevalorobra, po.preid
									) as foo
									where totalobra < 2)";
}

if( ! $excel )
{
	$acao = "CASE WHEN ( round(coalesce(foo.valor_liberado,0), 2) > (coalesce(foo.valor_pago,0)) ) THEN
			CASE WHEN foo.tipo = 'PAR' THEN
				'<img onclick=\"window.open(\'par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&processo='||foo.processosemformato||'&proid='||foo.proid||'&sldid='|| foo.sldid ||'&preidobrid='||foo.preid ||' \' ,\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" src=\"../imagens/money.gif\">'
			ELSE
			
				'<img onclick=\"window.open(\'par.php?modulo=principal/solicitacaoPagamento&amp;acao=A&amp;processo='||foo.processosemformato||'&proid='||foo.proid||'&sldid='|| foo.sldid ||'&preidobrid='||foo.preid||'\' ,\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" src=\"../imagens/money.gif\">'
			
			END
		
		
		ELSE
			'<span class=\'acao_bloq\' id=\"'||foo.sldid||'\"> </span>'
		END
			as acao,";
}
else
{
	$acao = "";
}


// Query principal da tela
$sql = "
	SELECT 
		{$acao}
		foo.sldid as sldid,
		foo.uf,
		foo.mundsc,
		foo.preid,
 		foo.obrid,
		foo.obradsc,
		foo.tipo_obra,
		foo.tipo,
		foo.numeroprocesso,		
		foo.situacao,
		foo.situacao_obras_2,
		foo.perc,
		'{$abreCenter}' || foo.liberado_pagamento || '%{$fechaCenter}',
		'{$abreCenter}' || round((((coalesce(foo.valor_pago,0)) * 100) / foo.prevalorobra ),2) || '% {$fechaCenter}' AS percentual_pago,
		'{$abreCenter}' || round((coalesce(foo.liberado_pagamento,0) - (((coalesce(foo.valor_pago,0)) * 100) / foo.prevalorobra )),2)  || '% {$fechaCenter}'  as a_pagar,
		foo.programa as programa,
		'{$abreCenter}' || round((((coalesce(foo.total_empenho,0)) * 100) / foo.prevalorobra ),2) || '% {$fechaCenter}' AS percentual_empenhado,
		'{$abreCenter}' || round((((coalesce(foo.total_pagamento,0)) * 100) / foo.prevalorobra ),2) || '% {$fechaCenter}' AS percentual_total_pago,
		(round((((coalesce(foo.total_empenho,0)) * 100) / foo.prevalorobra ),2) - round((((coalesce(foo.total_pagamento,0)) * 100) / foo.prevalorobra ),2))||' %' as percentual_apagar
		FROM 
	(
		SELECT 
		
		sd.sldid as sldid, 
		m.estuf as uf,
		pre.prevalorobra,
		coalesce((SELECT saldo FROM par.v_saldo_empenho_por_obra WHERE preid = pre.preid limit 1),0) 
		as total_empenho,
		CASE WHEN pro.proid IS NOT NULL THEN
			(SELECT SUM(pobvalorpagamento) FROM par.pagamentoobra p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid AND pag2.pagstatus = 'A' AND pag2.pagsituacaopagamento NOT ILIKE '%CANCELADO%' WHERE p2.preid = pre.preid) 
		
		WHEN pop.proid IS NOT NULL THEN
			(SELECT SUM(popvalorpagamento) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid WHERE p2.preid = pre.preid AND pag2.pagstatus = 'A' and pagsituacaopagamento not ilike '%cancelado%')
		END as total_pagamento,
		pf.prfdesc as programa, 
		m.mundescricao as mundsc,
		pre.preid as preid,
		o.obrid as obrid,
		pre.predescricao as obradsc,
		pto.ptodescricao as tipo_obra,
		CASE WHEN pro.proid IS NOT NULL THEN
			pro.proid
		WHEN pop.proid IS NOT NULL THEN
			pop.proid
		END
			AS proid,
		CASE WHEN pro.proid IS NOT NULL THEN
			'PAC'
		WHEN pop.proid IS NOT NULL THEN
			'PAR'
		END
			AS tipo,
		CASE WHEN pro.proid IS NOT NULL THEN
			substring(pro.pronumeroprocesso from 1 for 5)||'.'||
			substring(pro.pronumeroprocesso from 6 for 6)||'/'||
			substring(pro.pronumeroprocesso from 12 for 4)||'-'||
			substring(pro.pronumeroprocesso from 16 for 2) || ''
		WHEN pop.proid IS NOT NULL THEN
			
			substring(pop.pronumeroprocesso from 1 for 5)||'.'||
			substring(pop.pronumeroprocesso from 6 for 6)||'/'||
			substring(pop.pronumeroprocesso from 12 for 4)||'-'||
			substring(pop.pronumeroprocesso from 16 for 2) || ''
		END
			AS numeroprocesso,
		CASE WHEN pro.proid IS NOT NULL THEN
			pro.pronumeroprocesso
		WHEN pop.proid IS NOT NULL THEN			
			pop.pronumeroprocesso
		END
			AS processosemformato,
		
		esd.esddsc as situacao,
		str.strdsc as situacao_obras_2,
		'{$abreCenter}'||(SELECT obrpercentultvistoria FROM obras2.obras WHERE preid = pre.preid AND obrstatus = 'A' AND obridpai IS NULL)::integer||' %{$fechaCenter}' as perc,
		sd.sldpercpagamento  as liberado_pagamento,
		(( sd.sldpercpagamento * pre.prevalorobra ) / 100) as valor_liberado,
		
		CASE WHEN pro.proid IS NOT NULL THEN
			(
				select  sum(pg.pagvalorparcela)
				 from par.pagamentodesembolsoobras pd
				INNER JOIN par.pagamentoobra pob ON pob.pobid =  pd.pobid
				INNER JOIN par.pagamento pg ON pg.pagid = pob.pagid AND pg.pagstatus = 'A'
				WHERE pd.sldid = sd.sldid
				AND pdostatus = 'A'
				
			)
				
		WHEN pop.proid IS NOT NULL THEN
		
			(
				select  sum(pg.pagvalorparcela) from par.pagamentodesembolsoobras pd
				INNER JOIN  par.pagamentoobrapar po ON po.popid = pd.popid 
				INNER JOIN  par.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A'
				WHERE pd.sldid = sd.sldid
				AND pdostatus = 'A'
			)
				
		
		END
			AS valor_pago
		
		FROM 
			obras2.solicitacao_desembolso sd
		LEFT JOIN workflow.documento d ON d.docid = sd.docid
		LEFT JOIN workflow.estadodocumento eo ON eo.esdid = d.esdid
		INNER JOIN obras2.obras o ON o.obrid = sd.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
		LEFT JOIN obras2.empreendimento ep                    ON ep.empid = o.empid
        LEFT JOIN obras2.programafonte pf                    ON pf.prfid = ep.prfid
		LEFT JOIN obras2.situacao_registro str on str.strid = o.strid and str.strstatus = 'A'
		
		INNER JOIN obras.preobra pre ON pre.preid = o.preid
		LEFT  JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptostatus = 'A' 
		INNER JOIN workflow.documento doc ON doc.docid = pre.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		
		LEFT JOIN par.processoobraspaccomposicao poc ON poc.preid = pre.preid AND poc.pocstatus = 'A'
		LEFT JOIN par.processoobra   pro ON pro.proid = poc.proid AND pro.prostatus = 'A'
		
		LEFT JOIN par.processoobrasparcomposicao popc ON popc.preid = pre.preid and popc.pocstatus = 'A'
		LEFT JOIN par.processoobraspar pop ON popc.proid = pop.proid and pop.prostatus = 'A'
		
		LEFT JOIN territorios.municipio m ON m.muncod = pre.muncod
		LEFT JOIN territorios.estado e ON pre.muncod is null AND e.estuf = pre.estuf
		
		
		where d.esdid IN (1576) 
		AND sldstatus = 'A'
					 $filtro
	) AS foo			 
		$filtroFoo		 ";

if( ! $excel )
{			 
	$cabecalho = array('Ação', 'Nº da solicitação de desembolso','UF', 'Município', 'PREID', 'OBRID', 'Obra', 'Tipo Obra', 'Tipo','Processo', 'Situação', 'Situação Obras 2','% de execução da obra', 'Percentual deferido', '% pago do deferimento', 'Percentual a solicitar',
						 'Programa','Percentual empenhado', 'Percentual total pago da obra', 'Percentual a Pagar'); 
	$db->monta_lista($sql, $cabecalho, 50, 10, '', 'center', 'N', 'formulario_lista', '', '', '', '');
}
else
{
	$cabecalho = array( 'Nº da solicitação de desembolso','UF', 'Município', 'PREID', 'OBRID', 'Obra', 'Tipo Obra', 'Tipo','Processo', 'Situação', 'Situação Obras 2','% de execução da obra', 'Percentual deferido', '% pago do deferimento', 'Percentual a solicitar',
			'Programa','Percentual empenhado', 'Percentual total pago da obra', 'Percentual a Pagar');
	$db->monta_lista_tabulado($sql, $cabecalho, 5, 100000000, '', 'center', 'N', 'formulario_lista', '', '', '', '');	
	die();
}
?>
<div id="salvar_validacao" style="display: none" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
    	<td style="text-align: center;"><input type="button" class="salvar" value="Salvar Validação"/></td>
    </tr>
</table>
</div>
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

<script type="text/javascript">

	
	jQuery(document).ready(function(){

		
		jQuery( ".acao_bloq" ).each(function( index, element ) {
					
		    		jQuery(element).parent().parent().css( "color", "#6B6B6B", 'important');
		    		jQuery(element).parent().parent().css( "color", "#6B6B6B", 'important');
		    		
			  });

		if(jQuery('#mostra_salvar').val() == "mostrar")
		{
			jQuery('#salvar_validacao').show();
		}
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

		jQuery('.pesquisar').click(function(){
			jQuery('#requisicao').val('');
			jQuery('[name="formulario"]').submit();
		});
		jQuery('.excel').click(function(){
			jQuery('#requisicao').val('gerar_xls');
			jQuery('[name="formulario"]').submit();
		});
		
	});
</script>