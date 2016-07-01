<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
//ini_set("memory_limit", "1024M");
//set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if( $_POST['requisicao'] == 'xls'){
	relatorio();
}


if ($_POST['ajaxUf']){
	
	if($_POST['ajaxUf'] != 'XX'){
		echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
		$stAnd = "AND regcod in ('".str_replace(",","','",$_POST['ajaxUf'])."')";
	}
	
	$sql_combo = "SELECT
					muncod AS codigo,
					regcod ||' - '|| mundsc AS descricao
				FROM
					public.municipio
				Where munstatus = 'A'
				$stAnd
				ORDER BY 2 ASC;";
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);

	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . "includes/cabecalho.inc";

echo'<br>';

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

monta_titulo( 'Relatório de Prazo de Obras', '' );

?>

<script src="../includes/calendario.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
 
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
jQuery(document).ready(function(){

	jQuery('.programa').click(function(){
		if( jQuery(this).val() == 'PAR' ){
			jQuery('.PAC').hide();
			jQuery('.PAR').show();
		}else{
			jQuery('.PAR').hide();
			jQuery('.PAC').show();
		}
	});
});
</script> 
<form name="formulario" id="formulario" method="post">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<input type="hidden" id="requisicao" name="requisicao" value="">
	<tr> 
		<td style="width: 30%;" class="SubTituloDireita">PreId:</td>
		<td>
			<?$preid = $_POST['preid'];?>
			<?= campo_texto( 'preid', 'N', 'S', '', 11 , 11, '##########', '', 'left', '', 0, 'id="preid"'); ?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita">Obrid:</td>
		<td>
			<?$obrid = $_POST['obrid'];?>
			<?= campo_texto( 'obrid', 'N', 'S', '', 11 , 11, '##########', '', 'left', '', 0, 'id="obrid"'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Esfera:</td>	
		<td >
			<?php

				if($_POST['esfera'][0]){
					$sql = "";
					foreach( $_POST['esfera'] as $k => $esfera ){
						$sql .= "SELECT '$esfera' as codigo,";
						if($esfera == 'E'){
							$sql .= " 'ESTADUAL' as descricao";
						}else{
							$sql .= " 'MUNICIPAL' as descricao";
						}
						if( (count($_POST['esfera'])-1) != $k ){
							$sql .= " UNION ";
						}
					}
					
					$esfera = $db->carregar($sql);
				}
				
			
				$sql_combo = "SELECT
									'E' AS codigo,
									'ESTADUAL' AS descricao
							  union
							  SELECT
									'M' AS codigo,
									'MUNICIPAL' AS descricao
						ORDER BY 2 ASC;";
				combo_popup( 'esfera', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
			
	    	?>	
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Estado:</td>	
		<td >
			<?php
			
				if($_POST['uf'][0]){
					$sql = "SELECT
							regcod AS codigo,
							descricaouf AS descricao
						FROM
							public.uf
						WHERE idpais = 1
						and regcod in ('".implode("','",$_POST['uf'])."')
						ORDER BY 2 ASC;";
					$uf = $db->carregar($sql);
				}
				
				
				$sql_combo = "SELECT
							regcod AS codigo,
							descricaouf AS descricao
						FROM
							public.uf
						WHERE idpais = 1
						ORDER BY 2 ASC;";
				combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
			
	    	?>	
		</td>
	</tr>
	<tr id="tr_municipio" >
		<td class="SubTituloDireita" valign="top">Município:</td>	
		<td id="td_municipio">
			<?php 
			
				
				if($_POST['muncod'][0]){
					$sql = "SELECT
								muncod AS codigo,
								regcod ||' - '|| mundsc AS descricao
							FROM
								public.municipio
							Where munstatus = 'A'
							and muncod in ('".implode("','",$_POST['muncod'])."')
							ORDER BY 2 ASC;";
					$muncod = $db->carregar($sql);
					
					if($_POST['uf'][0]){
						$sql_combo = "SELECT
									muncod AS codigo,
									regcod ||' - '|| mundsc AS descricao
								FROM
									public.municipio
								Where munstatus = 'A'
								and regcod in ('".implode("','",$_POST['uf'])."')
								ORDER BY 2 ASC;";
					}
				}else{
					
					$sql_combo = "SELECT
								muncod AS codigo,
								regcod ||' - '|| mundsc AS descricao
							FROM
								public.municipio
							Where munstatus = 'A'
							and regcod = 'XX'
							ORDER BY 2 ASC;";
					
				}
				
				
				combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);			
			
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Obras vencendo:</td>
		<td>
			<?$dtobra_inicio 	= $_POST['dtobra_inicio'];?>
			<?$dtobra_fim 		= $_POST['dtobra_fim'];?>
			De: <?=campo_data2('dtobra_inicio', 'N', 'S', '', '' ); ?>
			Até: <?=campo_data2('dtobra_fim', 'N', 'S', '', '' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Programa:</td>
		<td>
			<input type="radio" class="programa" name="programa" value="PAC" <?=($_POST['programa']=='PAR'?'':'checked="checked"') ?>/> PAC
			<input type="radio" class="programa" name="programa" value="PAR" <?=($_POST['programa']=='PAR'?'checked="checked"':'') ?>/> PAR
		</td>
	</tr>
	<tr class="PAR" <?=($_POST['programa']=='PAR'?'':'style="display:none"') ?> >
		<td class="SubTituloDireita" valign="top">Situação da Obra:</td>	
		<td>
			<?php
				if( $_POST['esdid_par'][0] != '' ){
					$sql = "SELECT
								esdid as codigo,
								esddsc as descricao
							FROM
								workflow.estadodocumento
							WHERE 
								esdid IN (".implode(',',$_POST['esdid_par']).") AND tpdid = ".WF_FLUXO_OBRAS_PAR;
					$esdid_par = $db->carregar($sql);
				}
				
				$sql_combo = "SELECT
									esdid as codigo,
									esddsc as descricao
								FROM
									workflow.estadodocumento
								WHERE tpdid = ".WF_FLUXO_OBRAS_PAR;
				combo_popup( 'esdid_par', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
	    	?>	
		</td>
	</tr>
	<tr class="PAR" <?=($_POST['programa']=='PAR'?'':'style="display:none"') ?> >
		<td class="SubTituloDireita" valign="top">Tipo de Obra:</td>	
		<td>
			<?php
			
				if( $_POST['ptoid_par'][0] != '' ){

					$sql = "SELECT
								ptoid as codigo,
								ptodescricao as descricao
							FROM
								obras.pretipoobra
							WHERE
								ptoid IN (".implode(',',$_POST['ptoid_par']).") AND tooid <> 1";
					
					$ptoid_par = $db->carregar($sql);
				}
			
				$sql_combo = "SELECT 
									ptoid as codigo, 
									ptodescricao as descricao
								FROM 
									obras.pretipoobra
								WHERE
									tooid <> 1";
				combo_popup( 'ptoid_par', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
	    	?>	
		</td>
	</tr>
	<tr class="PAC" <?=($_POST['programa']=='PAR'?'style="display:none"':'') ?> >
		<td class="SubTituloDireita" valign="top">Situação da Obra:</td>	
		<td>
			<?php
				if( $_POST['esdid_pac'][0] != '' ){
					$sql = "SELECT
								esdid as codigo,
								esddsc as descricao
							FROM
								workflow.estadodocumento
							WHERE 
								esdid IN (".implode(',',$_POST['esdid_pac']).") AND tpdid = ".WF_TPDID_FLUXO_OBRA_PAC;
					$esdid_pac = $db->carregar($sql);
				}
				
				$sql_combo = "SELECT
									esdid as codigo,
									esddsc as descricao
								FROM
									workflow.estadodocumento
								WHERE tpdid = ".WF_TPDID_FLUXO_OBRA_PAC;
				combo_popup( 'esdid_pac', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
	    	?>	
		</td>
	</tr>
	<tr class="PAC" <?=($_POST['programa']=='PAR'?'style="display:none"':'') ?> >
		<td class="SubTituloDireita" valign="top">Tipo de Obra:</td>	
		<td>
			<?php
			
				if( $_POST['ptoid_pac'][0] != '' ){
					$sql = "SELECT
								ptoid as codigo,
								ptodescricao as descricao
							FROM
								obras.pretipoobra
							WHERE
								ptoid IN (".implode(',',$_POST['ptoid_pac']).") AND tooid = 1";
					
					$ptoid_pac = $db->carregar($sql);
				}
				
				$sql_combo = "SELECT 
									ptoid as codigo, 
									ptodescricao as descricao
								FROM 
									obras.pretipoobra
								WHERE
									tooid = 1";
				combo_popup( 'ptoid_pac', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
	    	?>	
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="4" style="text-align: center">
			<input type="button" value="Gerar Relatório" name="btnGerarHtml" id="btnGerarHtml" style="cursor: pointer;" onclick="gerarRel('html');">
			<input type="button" value="Exportar Excel" name="btnGerar" id="btnGerar" style="cursor: pointer;" onclick="gerarRel('xls');">
		</td>
	</tr>
</table>
</form>

<?

if( $_POST['requisicao'] == 'html' ){
	relatorio();
}

?>


<script type="text/javascript">

function gerarRel(tipo){

	selectAllOptions(document.getElementById('esfera'));
	selectAllOptions(document.getElementById('uf'));
	selectAllOptions(document.getElementById('muncod'));
	selectAllOptions(document.getElementById('esdid_par'));
	selectAllOptions(document.getElementById('esdid_pac'));
	selectAllOptions(document.getElementById('ptoid_par'));
	selectAllOptions(document.getElementById('ptoid_pac'));

	$('#requisicao').val(tipo);
	$('#formulario').submit();
}

function filtraMunicipio(){

		
		var uf = document.getElementById('uf');
		var listauf = "";

		for(i=0;i<uf.length;i++){
			listauf = listauf + "," + uf[i].value;
		}

		if(listauf) listauf = listauf.substr(1);
		
		if(!listauf) listauf = 'XX';

		var req = new Ajax.Request('par.php?modulo=relatorio/relatorioProgramas&acao=A', {
				        method:     'post',
				        parameters: '&ajaxUf='+listauf,
				        onComplete: function (res)
				        {
							td_municipio.innerHTML = res.responseText;
				        }
					});

		//filta o campo escola
		//var td_escolas   = document.getElementById('td_escolas');

		
}

</script>
<?

function relatorio() {
	global $db;
	
	extract($_POST);
	
	$where 	= array();
	$having = Array('1=1');
	
	//preid
	if($preid){
		array_push($where, " pre.preid = {$preid} ");
	}
	
	//obrid
	if($obrid){
		array_push($where, " pre.obrid = {$obrid} ");
	}
	
	//esfera
	if( $esfera[0] ){
		array_push($where, " pre.preesfera " . (!$esfera_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ");
	}
	
	//uf
	if( $uf[0] ){
		array_push($where, " mun.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uf ) . "') ");
	}
	
	//muncod
	if( $muncod[0] ){
		array_push($where, " pre.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') ");
	}
	
	//dtobra
	if( $dtobra_inicio ){
		$dtobra2 = formata_data_sql($dtobra_inicio);
		$having[] = " to_char(CASE WHEN popdataprazoaprovado IS NOT NULL THEN popdataprazoaprovado ELSE MIN(pag.pagdatapagamentosiafi) + 720 END::date, 'YYYYMMDD')::numeric >= to_char('{$dtobra2}'::date,'YYYYMMDD')::numeric ";
	}
	
	//dtobra
	if( $dtobra_fim ){
		$dtobra2 = formata_data_sql($dtobra_fim);
		$having[] = " to_char(CASE WHEN popdataprazoaprovado IS NOT NULL THEN popdataprazoaprovado ELSE MIN(pag.pagdatapagamentosiafi) + 720 END::date, 'YYYYMMDD')::numeric <= to_char('{$dtobra2}'::date,'YYYYMMDD')::numeric ";
	}
	// Flag nova para diferenciar campos nas situaçoes "obra indeferida " "Obra indeferida - decurso prazo de diligência " "Obra Arquivada " e "Obra cancelada " Conforme solicitado na demanda 2272
	$isIndeferidaCancelada = false;
	if( $programa == 'PAR' ){		
		$programaInner = "		obras.preobra 			pre 
							INNER JOIN par.pagamentoobrapar pob ON pob.preid = pre.preid 
							INNER JOIN par.pagamento 		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A'";
		$where[] = " pre.tooid <> 1 ";
		
		// Adiciona exceção para criar query para as situações diferenciadas conforme demanda 2272
		if( $esdid_par[0] != '' )
		{
			// Situações exceção demanda 2272
			$arrIndeferidasCanceladas = Array( 332,  335,  338,  628 );
			foreach($esdid_par as $k => $v)
			{
				// Caso alguma delas for selecionada demanda 2272
				if(in_array($v ,$arrIndeferidasCanceladas ))
				{
					// Cria array para elas e seta que existem obras deste tipo para usar o union all demanda 2272
					$arrClauseEsdidPar[] = $v;
					$isIndeferidaCancelada = true;
				}
			}
			// Caso existam ele adiciona clausula para a segunda query que será adicionada a padrão da tela demanda 2272
			if($isIndeferidaCancelada)
			{
				if( $arrClauseEsdidPar[0] != '' ) $whereEsdidC[] = " doc.esdid IN (".implode(',',$arrClauseEsdidPar).") ";
				$programaInnerC = "		obras.preobra 			pre
							LEFT JOIN par.pagamentoobrapar pob ON pob.preid = pre.preid
							LEFT JOIN par.pagamento 		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A'";
			}

		}
		
		if( $esdid_par[0] != '' ) $whereEsdid[] = " doc.esdid IN (".implode(',',$esdid_par).") ";
		if( $ptoid_par[0] != '' ) $where[] = " pre.ptoid IN (".implode(',',$ptoid_par).") ";
	}else{
		$programaInner = "		obras.preobra 			pre 
							INNER JOIN par.pagamentoobra	pob ON pob.preid = pre.preid 
							INNER JOIN par.pagamento 		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A'";
		$where[] = " pre.tooid = 1 ";
		
		// Adiciona exceção para criar query para as situações diferenciadas conforme demanda 2272 
		if( $esdid_pac[0] != '' )
		{
			// Situações exceção demanda 2272
			$arrIndeferidasCanceladas = Array( 368, 213, 221, 229 );
			foreach($esdid_pac as $k => $v)
			{
				// Caso alguma delas for selecionada demanda 2272
				if(in_array($v ,$arrIndeferidasCanceladas ))
				{
					// Cria array para elas e seta que existem obras deste tipo para usar o union all demanda 2272
					$arrClauseEsdidPac[] = $v;
					$isIndeferidaCancelada = true;
				}
			}
			// Caso existam ele adiciona clausula para a segunda query que será adicionada a padrão da tela demanda 2272
			if($isIndeferidaCancelada)
			{
				if( $arrClauseEsdidPac[0] != '' ) $whereEsdidC[] = " doc.esdid IN (".implode(',',$arrClauseEsdidPac).") ";
				$programaInnerC = "		obras.preobra 			pre
							LEFT JOIN par.pagamentoobra	pob ON pob.preid = pre.preid
							LEFT JOIN par.pagamento 		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A'";
			}
			
		}
		
		if( $esdid_pac[0] != '' ) $whereEsdid[] = " doc.esdid IN (".implode(',',$esdid_pac).") ";
		if( $ptoid_pac[0] != '' ) $where[] = " pre.ptoid IN (".implode(',',$ptoid_pac).") ";
	}
	
	$sql = "(SELECT 
				pre.preid as preid,
				pre.obrid as obrid,
				pre.predescricao as nome,
				pto.ptodescricao as tipologia,
				mun.mundescricao as municipio,
				mun.estuf as uf,
				pre.preesfera as esfera,
				CASE WHEN popdataprazoaprovado IS NOT NULL THEN popdataprazoaprovado ELSE MIN(pag.pagdatapagamentosiafi) + 720 END as prazo,
				es.esddsc as situacao_par, 
				esd.esddsc as situacao_obras2, 
				obrpercentultvistoria as percentual,
				vv.vigencia, 
				obrdtultvistoria as ultimaatualizacao
			FROM 
				$programaInner
			INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento es  ON es.esdid  = doc.esdid
			LEFT  JOIN obras.pretipoobra 		pto ON pre.ptoid = pto.ptoid
			INNER JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
			LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = pre.preid AND popstatus = 'A' AND popvalidacao = 't'
			INNER JOIN obras2.obras 			o   ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
			INNER JOIN workflow.documento 		d   ON d.docid = o.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
			left join par.v_vigenciaobras vv on vv.preid = pre.preid and vv.tipo = 'PAC'
			WHERE 1=1 " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
			" . ( $whereEsdid[0] ? ' AND' . implode(' AND ', $whereEsdid) : '' ) . "
			GROUP BY
				pre.preid, pre.obrid, pre.predescricao, mun.mundescricao, mun.estuf, popdataprazoaprovado, pre.preesfera, esd.esddsc, es.esddsc, obrpercentultvistoria, obrdtultvistoria,pto.ptodescricao, vv.vigencia
			HAVING " . implode(' AND ', $having). "
			ORDER BY
				prazo, mun.estuf, mun.mundescricao)
	";
     //dbg($sql,1);
	// Conforme descrito na demanda 2272 agora adicionando a segunda query caso exista pesquisa por situações descritas acima
	if( $isIndeferidaCancelada == true )
	{
		$sql .= "
				union all
				(SELECT
					pre.preid as preid,
					pre.obrid as obrid,
					pre.predescricao as nome,
					pto.ptodescricao as tipologia,
					mun.mundescricao as municipio,
					mun.estuf as uf,
					pre.preesfera as esfera,
					CASE WHEN popdataprazoaprovado IS NOT NULL THEN popdataprazoaprovado ELSE MIN(pag.pagdatapagamentosiafi) + 720 END as prazo,
					es.esddsc as situacao_par,
					esd.esddsc as situacao_obras2,
					obrpercentultvistoria as percentual,
					vv.vigencia,
					obrdtultvistoria as ultimaatualizacao
				FROM
					$programaInnerC
				INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
				INNER JOIN workflow.estadodocumento es  ON es.esdid  = doc.esdid
				LEFT  JOIN obras.pretipoobra 		pto ON pre.ptoid = pto.ptoid
				INNER JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
				LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = pre.preid AND popstatus = 'A' AND popvalidacao = 't'
				LEFT JOIN obras2.obras 			o   ON o.preid = pre.preid  AND o.obridpai IS NULL
				LEFT JOIN workflow.documento 		d   ON d.docid = o.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
				left join par.v_vigenciaobras vv on vv.preid = pre.preid and vv.tipo = 'PAC'
				WHERE 1=1 " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
				" . ( $whereEsdidC[0] ? ' AND' . implode(' AND ', $whereEsdidC) : '' ) . "
					GROUP BY
							pre.preid, pre.obrid, pre.predescricao, mun.mundescricao, mun.estuf, popdataprazoaprovado, pre.preesfera, esd.esddsc, es.esddsc, obrpercentultvistoria, obrdtultvistoria,pto.ptodescricao, vv.vigencia
					HAVING " . implode(' AND ', $having). "
									ORDER BY
											prazo, mun.estuf, mun.mundescricao)
			";
	}
	
	
	$registros = $db->carregar($sql);
	$arDados = array();
	if($registros[0]) {
		
		$arCabecalho = array('PREID', 'OBRID', 'NOME DA OBRA', 'TIPOLOGIA DA OBRA', 'MUNICÍPIO', 'UF', 'ESFERA', 'PRAZO', 'SITUAÇÃO - PAR', 'SITUAÇÃO - OBRAS 2.0', 'PERCENTUAL', 'DATA SOLICITAÇÃO DE PRORROGAÇÃO', 'DATA DA ÚLTIMA ATUALIZAÇÃO');
	
		foreach($registros as $reg) {
			
			$arDados[] = array('preid'=>$reg['preid'],
							   'obrid'=>$reg['obrid'],
							   'nome'=>$reg['nome'],
							   'tipologiaobra' => $reg['tipologia'],
							   'municipio'=>$reg['municipio'],
							   'uf'=>$reg['uf'],
							   'esfera'=>$reg['esfera'],
							   'prazo'=>formata_data($reg['prazo']),
							   'situacao_par'=>$reg['situacao_par'],
							   'situacao_obras'=>$reg['situacao_obras2'],
							   'percentual'=>$reg['percentual'],
							   'vigencia'=>formata_data($reg['vigencia']),
							   'ultimaatualizacao'=>formata_data($reg['ultimaatualizacao'])
			
			);
		}
	}
	
	if( $_POST['requisicao'] == 'xls'){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_Relat".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatPAR".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
		exit;
	}else{
		$db->monta_lista_array($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	}
	
}
?>