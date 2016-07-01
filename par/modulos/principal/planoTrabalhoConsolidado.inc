<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php

global $db;
$dopId = 19129;

$prpId = $_REQUEST['prpid'];
$sqlDadosDoc = "SELECT 

	dop.dopid as dopid,  -- id do documento
	prpnumeroprocesso as nprocesso , -- número do processo
	dop.dopnumerodocumento || '/' || to_char(dop.dopdatainclusao, 'YYYY') as ndocumento, -- número do documento
	mo.mdonome as dscdocumento -- descricao do documento
FROM 
	par.processopar prp 
LEFT JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
LEFT JOIN par.modelosdocumentos mo on mo.mdoid = dop.mdoid

where prp.prpid = {$prpId}
";

$dadosDoc = $db->pegaLinha($sqlDadosDoc);


function monta_lista_numerada($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null,$param=Array()) {
		
		global $db;
		$ordena = isset($param['ordena']) ? $param['ordena'] : true;
		$param['totalLinhas'] = is_bool($param['totalLinhas']) ? $param['totalLinhas'] : true;
		
		if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);	
		//Controla o Order by
		if (!is_array($sql) && $_REQUEST['ordemlista']<>'' && $ordena)
		{
			if ($_REQUEST['ordemlistadir'] <> 'DESC') {
				$ordemlistadir  = 'ASC';
				$ordemlistadir2 = 'DESC';
			} else {
				$ordemlistadir  = 'DESC'; 
				$ordemlistadir2 = 'ASC';
			}
			
			$subsql 	= substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));
			
			if ( !empty($param['managerOrder'][$_REQUEST['ordemlista']]) ){
				if ( is_array( $param['managerOrder'][$_REQUEST['ordemlista']] ) ){
					$campoOrder = $param['managerOrder'][$_REQUEST['ordemlista']]['campo']; 
					$aliasOrder = $param['managerOrder'][$_REQUEST['ordemlista']]['alias']; 
				} else {
					$campoOrder = $param['managerOrder'][$_REQUEST['ordemlista']]; 
					$aliasOrder = $param['managerOrder'][$_REQUEST['ordemlista']]; 
				}
				$ordemLista = $aliasOrder . '_oculto';
				
				$strReplace = 'SELECT';
				if ( strpos( trim(strtoupper($subsql)), 'DISTINCT' ) ){
					$posIni 	    = strpos( trim(strtoupper($subsql)), 'SELECT' ) + 6;
					$posFim 	    = strpos( trim(strtoupper($subsql)), 'DISTINCT' ) - 6;
					$espacoDistinct = trim(substr($subsql, $posIni, $posFim) );
					
					if ( empty($espacoDistinct) ){
						$strReplace = 'DISTINCT';	
					}
				}
				$subsql = preg_replace('/' . $strReplace . '/i', 
									   $strReplace . " " . $campoOrder . ' AS ' . $aliasOrder . '_oculto, ',
									   $subsql,
									   1);
			}else{
				$ordemLista = $_REQUEST['ordemlista'];
			}
			
			$sql 		= (!$subsql ? $sql : $subsql).' order by '.$ordemLista.' '.$ordemlistadir;
		}
		
	    if (is_array($sql)){
	        $RS = $sql;
	        $totalRegistro = count($RS);
		}else{
			$sql = trim($sql);
			$char = substr($sql, -1); 
			if ($char == ";") $sql = substr($sql, 0, -1);
			
			$sqlCount = "select 
							count(1)
						 from (" . $sql . ") rs";
			
			$totalRegistro = $db->pegaUm($sqlCount,0,$tempocache);
			
			$sql = $sql . " LIMIT {$perpage} offset ".($numero-1);	
				
			$RS = $db->carregar($sql,null,$tempocache);
	    }
		$nlinhas = count($RS);
		
		if (! $RS) $nl = 0; else $nl=$nlinhas;
	//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
		$reg_fim = $nlinhas;
		if ($nl>0)
		{
			$total_reg = $totalRegistro;
			
			//monta o formulario da lista mantendo os parametros atuais da pgina
			print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';

			foreach($_POST as $k=>$v){
				if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero'){
					if( is_array($v)){
						foreach($v as $k2 => $v2){
							if( is_array($v2)){
								foreach($v2 as $k3 => $v3){
									print '<input type="Hidden" name="'.$k.'['.$k2.']['.$k3.']" value="'.$v3.'"/>';
								}
							}else{
								print '<input type="Hidden" name="'.$k.'['.$k2.']" value="'.$v2.'"/>';
							}
						}
					}else{
						print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';
					}
				}
			}
						
			print '</form>';
			
			if($nomeformulario != "")
				print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'" action="" enctype="multipart/form-data" method="post">';
				
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			//Monta Cabealho
			if ( $cabecalho === null ) {
	
			}else if(is_array($cabecalho))
			{
				foreach($cabecalho as $cab){
					if(is_array($cab)){
						$cab_rowspan = " rowspan='2' ";
					}
				}
				
				print '<thead><tr> <td valign="top" align="95%" title="Número da linha" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; width:3%; border-left: 1px solid #ffffff;" class="title">Nº</td>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if(!is_array($cabecalho[$i])){
						if ($_REQUEST['ordemlista'] == ($i+1)) {
							$ordemlistadirnova = $ordemlistadir2;
							$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
						} else {
							$ordemlistadirnova = 'ASC';
							$imgordem = '';
						}
						$arrColunasComuns[] = 1;
						$onclick = '';
						print '<td '.$cab_rowspan.' align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" '.$onclick.' title="Ordenar por '.strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
					}else{
						print '<td colspan="'.count($cabecalho[$i]['colunas']).'"  align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" ><center><strong>'.$cabecalho[$i]['label'].'</strong></center></label>';
						foreach($cabecalho[$i]['colunas'] as $col_b){
							$arrColunasDebaixo[] = $col_b;
						}
					}
				}
				if($arrColunasDebaixo){
					print "<tr>";
					$i = count($arrColunasComuns);
					foreach($arrColunasDebaixo as $colunaDeBaixo){
						if ($_REQUEST['ordemlista'] == $i+1) {
							$ordemlistadirnova = $ordemlistadir2;
							$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
						} else {
							$ordemlistadirnova = 'ASC';
							$imgordem = '';
						}
						
						print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.strip_tags($colunaDeBaixo).'">'.$imgordem.'<strong>'.$colunaDeBaixo.'</strong></label>';
						$i++;
					}
				}
				print '</tr> </thead>';
			}
			else
			{
				print '<thead><tr>'; $i=0;
				foreach($RS[0] as $k=>$v)
				{
					if ($_REQUEST['ordemlista'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						$i=$i+1;}
						print '</tr> </thead>';
			}
			//Monta Listagem
			$totais = array();
			$tipovl = array();
	//		for ($i=($numero-1);$i<$reg_fim;$i++)
	
			for ($i=0; $i < $reg_fim; $i++)
			{
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
				$cont = $i + 1;
				print "<td>$cont</td>";
				
				// contador -> numero de celulas
				$numCel = 0;
				foreach($RS[$i] as $k=>$v) {
					if ( strpos($k, '_oculto') ){
						continue;
					}
					
					// Setando o alinhamento da célula usando o array $celAlign.
					// Se não for passado o parâmetro, usa o padrão do componente.
					if(is_array($celAlign)) {
						$alignNumeric = $alignNotNumeric = $celAlign[$numCel];
					} else {
						$alignNumeric 		= 'right';
						$alignNotNumeric	= 'left';
					}
					// Setando o tamanho da célula usando o array $celWidth.
					// Se não for passado o parâmetro, usa o padrão do componente.
					$width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';
					
					if (is_numeric($v))
					{
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						//Mostra o resultado
						if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
						if ($v<0) print '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.strip_tags($cabecalho[$c]).'">('.$v.')'; else print '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.strip_tags($cabecalho[$c]).'">'.$v;
						print ('<br>'.$totais[$c]);
					}
					else {
						 print ' <td align="'.$alignNotNumeric.'" '.$width.' title="'.strip_tags($cabecalho[$c]).'">'.$v;
					}
					print '</td>';
					$c = $c + 1;
					$numCel++;
				}
				print '</tr>';
			}
	
			if ($soma=='S'){
				//totaliza (imprime totais dos campos numericos)
				print '<thead><tr>';
				for ($i=0;$i<$c;$i++)
				{
					print '<td align="right" title="'.strip_tags($cabecalho[$i]).'">';
	
					if ($i==0) print 'Totais:   ';
					if (is_numeric($totais['0'.$i])) {
						
					if($valormonetario == 'S'){
								print number_format($totais['0'.$i], 2, ',', '.'); 
							}else{
								print $totais['0'.$i]; 
							}
					}
						else print $totais['0'.$i];
					print '</td>';
				}
				print '</tr>';
				//fim totais
			}
	
			print '</table>';
	
			if($nomeformulario != "")
				print '</form>';
			if( $param['totalLinhas'] ){
				print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
                print '</td></tr></table>';
			}
	
			include APPRAIZ."includes/paginacao.inc";
			
			print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
		}
		else
		{
			print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
	}

$dopId = $dadosDoc['dopid'];

if( ! $dopId )
{
	
	print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
	print '<tr><td align="center" style="color:#cc0000;">O processo não possui Termo de Compromisso PAR</td></tr>';
	print '</table>';
	die();
}

$numeroProcesso		= $dadosDoc['nprocesso'];
$numeroDocumento	= $dadosDoc['ndocumento']; 
$descricaoDocumento	= $dadosDoc['dscdocumento']; 

?>
<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">
	<thead  >
	<tr bgcolor="#DCDCDC" >
		<td width="100%" colspan="4" align="center">
			<label class="TituloTela" style="color:#000000;">
				Plano de Trabalho consolidado
			</label>
		</td>
	</tr>
	</thead>
	<tbody>
	<tr>	
		<td bgcolor="#e9e9e9"  width="20%">
			<b>Nº Processo:</b> 
		</td>
		
		<td  >
			<?= $numeroProcesso ?>
		</td>
	</tr>
	
	<tr>	
		<td bgcolor="#e9e9e9"  width="20%">
			<b>Nº do termo</b>
		</td>
			
		<td >
			<?= $numeroDocumento?>
		</td>
	</tr>
	<tr>		
		<td bgcolor="#e9e9e9"  width="20%">
			<b>Descrição do termo</b>
		</td>
		
		<td >
			<?= $descricaoDocumento ?>
		</td>
	</tr>
	</tbody>
</table>
<br>
<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">
	
	<tr bgcolor="#DCDCDC" >
		<td width="100%" colspan="4" align="center">
			<label class="TituloTela" style="color:#000000;">
				Detalhamento do PTA
			</label>
		</td></tr>
		<tr>
			<td colspan="2" class="SubTitulocentro">Ações</td>
		</tr>
		</table>
<?php
$sql = "SELECT DISTINCT acidsc
		FROM par.acao 
		WHERE aciid in 
		 (
		SELECT DISTINCT
			s.aciid
						
			FROM 
				par.processoparcomposicao ppc
			INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			WHERE
				ppc.ppcstatus = 'A' and
				ppc.prpid = {$prpId} 
		) "; 

	$cabecalho = array("Descrição da ação");
	monta_lista_numerada($sql,$cabecalho,50000000,5,'N','95%','S');
?>

<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">

		<tr>
			<td class="SubTitulocentro">Subações</td>
		</tr>
		</table>


<?php 		
$sql = "SELECT DISTINCT
				par.retornacodigosubacao(s.sbaid) as localizacao,
				s.sbadsc,
				TO_CHAR( (SELECT par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano)), '999G999G999D99') as valor,
				sd.sbdano
				
			FROM 
				par.processoparcomposicao ppc
			INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			LEFT JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A' 
			LEFT JOIN  par.empenhopregaoitemperdido epi ON epi.sbdid = sd.sbdid AND epi.prpid = ppc.prpid
			WHERE
				ppc.ppcstatus = 'A' and
				ppc.prpid = ".$prpId; 

	$cabecalho = array("Localização", "Descrição da Subação", "Valor da Subação", "Ano da Subação" );
	monta_lista_numerada($sql,$cabecalho,500,5,'N','95%','S');
?>
<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">

		<tr>
			<td class="SubTitulocentro">Itens das Subações</td>
		</tr>
		</table>

<?php 
	
	$sqlGlobais = 	"SELECT 	
		DISTINCT s.sbaid
		FROM
			par.processoparcomposicao ppc
		INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
		INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
		INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
		WHERE
			ppc.ppcstatus = 'A' and
			ppc.prpid = $prpId
		AND s.sbacronograma = 1";           
	
	$resultGlobais = $db->carregar($sqlGlobais);
	$strGlobais = false;
	if( count($resultGlobais) && is_array($resultGlobais) )
	{
		foreach( $resultGlobais  as $k => $v)
		{
			$arrGlobais[] = $v['sbaid'];
		}
		$strGlobais = implode (', ', $arrGlobais);
	}
	
	
	$sqlEscolas = "SELECT 	DISTINCT s.sbaid
		FROM
			par.processoparcomposicao ppc
		INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
		INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
		INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
		WHERE
			ppc.ppcstatus = 'A' and
			ppc.prpid = $prpId
		AND s.sbacronograma <> 1";
	
	$resultEscolas = $db->carregar($sqlEscolas);
	$strEscolas = false;
	if( count($resultEscolas) && is_array($resultEscolas) )
	{
		foreach( $resultEscolas  as $k => $v)
		{
			$arrEscolas[] = $v['sbaid'];
		}
		$strEscolas = implode (', ', $arrEscolas);
	}
	
	$sqlItensGlobais = 
	"
		SELECT DISTINCT
				par.retornacodigosubacao(sic.sbaid) as localizacao,
				'Global' as cronograma,
				sic.icodescricao,
				sic.icoquantidadetecnico as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR( ( sic.icoquantidadetecnico * sic.icovalor), '999G999G999D99') as valortotal
			FROM
				par.subacaoitenscomposicao sic
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sic.sbaid AND sd.sbdano = sic.icoano
			LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A' AND emi.epiano = sd.sbdano
            --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid AND piap.piastatus = 'A'
			WHERE
				sic.icovalidatecnico = 'S' 
			AND
				sic.sbaid in 
				( {$strGlobais}
				)
	
	";
	$sqlItensEscolas = 
	"
		SELECT         
			par.retornacodigosubacao(sic.sbaid) as localizacao,
						'Escola' as cronograma,
                        sic.icodescricao,
                        SUM( sse.seiqtdtecnico ) as quantidade,
                        TO_CHAR( sic.icovalor, '999G999G999D99'),
                        TO_CHAR(SUM( sse.seiqtdtecnico * sic.icovalor ), '999G999G999D99') as valortotal
                FROM
                        par.subacaoitenscomposicao sic
                INNER JOIN par.subescolas_subitenscomposicao  sse ON sse.icoid = sic.icoid
                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sic.sbaid AND sd.sbdano = sic.icoano
				LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A' AND emi.epiano = sd.sbdano
                
                WHERE
                        sic.icovalidatecnico = 'S' AND
                        sic.sbaid in 
				( {$strEscolas} )
                GROUP BY
                        sic.icodescricao, sic.icovalor, sic.icoquantidadetecnico, emi.adesao, sic.sbaid, cronograma
	";
	
	$cabecalho = array('Localização', 'Cronograma', 'Descrição do Item', 'Quantidade', 'Valor', 'Valor Total');
	if($strEscolas && $strGlobais)
	{
		$sql = 
		$sqlItensGlobais
		.
		" UNION ALL"
		.
		$sqlItensEscolas;
		
		monta_lista_numerada($sql, $cabecalho,500,5,'N','95%','S');
	}
	elseif($strEscolas)
	{
		monta_lista_numerada($sqlItensEscolas, $cabecalho,500,5,'N','95%','S');
	}
	elseif($strGlobais)
	{
		monta_lista_numerada($sqlItensGlobais, $cabecalho,500,5,'N','95%','S');
	}

	$sqlListaEscolas = 
		"	
			SELECT 
				par.retornacodigosubacao(ses.sbaid) as localizacao,
				ent.entcodent as cod_inep,
				ent.entnome as nome_escola,
				CASE WHEN 
					( SELECT 	itr.itrid
						FROM
							par.processoparcomposicao ppc
					INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
					INNER JOIN par.instrumentounidade inu ON inu.inuid = pp.inuid
					INNER JOIN par.instrumento itr ON inu.itrid = itr.itrid
					INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
					INNER JOIN par.subacao s2 ON s2.sbaid = sd.sbaid
					WHERE
						ppc.ppcstatus = 'A' and
						s2.sbaid = ses.sbaid
					AND s2.sbacronograma <> 1 limit 1) = 1 
				THEN m.mundescricao else '' end as mundescricao,
				ses.sbaid,
				ses.sesano,
				array_to_string(ARRAY(
					SELECT 
							'<tr class=\"divItem\"> <td width=\"69%\" style=\"  \" > ' || ico2.icodescricao || '</td> <td width=\"32%\" > ' || ssc2.seiqtd || '</td></tr>' as linha
							
					FROM
						entidade.entidade t2
					INNER JOIN entidade.funcaoentidade f2 on f2.entid = t2.entid
					LEFT JOIN entidade.entidadedetalhe ed2 on t2.entid = ed2.entid
						and (
							     entdreg_infantil_creche = '1' or
							     entdreg_infantil_preescola = '1' or
							     entdreg_fund_8_anos = '1' or
							     entdreg_fund_9_anos = '1'
									)
					INNER JOIN entidade.endereco d2 on t2.entid = d2.entid
					LEFT JOIN par.escolas e2 on e2.entid = t2.entid
					INNER JOIN par.subacaoescolas ses2 on 
						ses2.escid = e2.escid 
						and 
						ses2.sesstatus = 'A'
						and
							ses2.sesano = ses.sesano
						and
							ses2.sbaid = ses.sbaid
					INNER JOIN par.subescolas_subitenscomposicao ssc2 ON ssc2.sesid = ses2.sesid 
					INNER JOIN par.subacaoitenscomposicao ico2 on ssc2.icoid = ico2.icoid
					
					WHERE
						(t2.entescolanova = false or t2.entescolanova is null)
					AND 
						t2.entstatus = 'A'
					and 
					f2.funid = 3
			
					AND 
						ses2.sesid = ses.sesid
						
				), ' ')
				as itens_escola 
			
			FROM 
				par.subacaoescolas  ses
			INNER JOIN par.escolas esc on ses.escid = esc.escid
			INNER JOIN entidade.entidade ent on esc.entid = ent.entid
			inner join entidade.funcaoentidade f on f.entid = ent.entid
			left join entidade.entidadedetalhe ed on ent.entid = ed.entid
			inner join entidade.endereco d on ent.entid = d.entid
			left join territorios.municipio m on m.muncod = d.muncod
			
			WHERE 
			ses.sbaid in (
			
				SELECT 	DISTINCT s.sbaid
					FROM
						par.processoparcomposicao ppc
				INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
				INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				WHERE
					ppc.ppcstatus = 'A' and
					ppc.prpid = {$prpId}
				AND s.sbacronograma <> 1	
			)
			
		";
	
	$resultListaEscolas = $db->carregar($sqlListaEscolas);
	
	if(is_array($resultListaEscolas) && count($resultListaEscolas))
	{
	
?>

	<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">
	
		<tr>
			<td class="SubTitulocentro">Lista de Escolas</td>
		</tr>
	</table>
	<style>
		.divItem {
				
				
		}
		
		.divItem:hover {background:#FAE892;}
	
	</style>
	<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
		<thead>
			<tr>
				<td width="9%" rowspan="2" valign="top" bgcolor="" align="95%" title="Ordenar por Localização" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
					<strong>Localização</strong>
				</td>
				<td width="10%" rowspan="2" valign="top" align="95%" title="Ordenar por Descrição da Subação" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
					<strong>Código Inep</strong>
				</td>
				<td width="20%" rowspan="2" valign="top" bgcolor="" align="95%" title="Ordenar por Valor da Subação" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
					<strong>Nome da Escola </strong>
				</td>
				<?php if( ! ($_SESSION['par']['muncod']))
				{?>
					<td width="20%" rowspan="2" valign="top" align="95%" title="Ordenar por Ano da Subação" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
						<strong>Município</strong>
					</td>
				<?php 
				}?>
				<td  valign="top" align="95%" title="Ordenar por Ano da Subação" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
					<strong >Itens</strong>
				</td>
			</tr> 
			<tr >
				<td  valign="top" align="95%" title="Ordenar por Ano da Subação" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">   
				<div style="width: 67%; height:100%; float: left; padding-left: 2%; border-right: 1px solid #c0c0c0; ">Item </div> <div  style=" padding-left: 2%; width: 28%; float: left;">Quantidade </div> </td>
			</tr>
		</thead>
		<tbody>
		
		<?php 
			$plinha = 0;
			foreach($resultListaEscolas as $k => $linha ) {
			 
			if($linha['itens_escola'])
			{
				$plinha++;
				?>
			
				<tr <?php if( ($plinha % 2)) { ?> 
						bgcolor="#F7F7F7" onmouseout="this.bgColor='#F7F7F7';" onmouseover="this.bgColor='#ffffcc';" 
					<?} else {?>
						bgcolor=""        onmouseout="this.bgColor='';"        onmouseover="this.bgColor='#ffffcc';"<?}?>
				>
					<td><?= $linha['localizacao']; ?> </td>
					<td><?= $linha['cod_inep']; ?> </td>
					<td><?= $linha['nome_escola']; ?> </td>
					<?php if( ! ($_SESSION['par']['muncod']))
					{?>
						<td><?= $linha['mundescricao']; ?> </td>
					<?php 
					}
					?>
					<td ><table cellpadding="0px" cellspacing="0px"  width="100%"><?= $linha['itens_escola']; ?></table> </td>
				</tr>
		<?php }
		}?>
		</tbody>
	
	</table>
<?php }?>

<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">

	<tr>
		<td class="SubTitulocentro">Detalhamento</td>
	</tr>
</table>
<?php 		

	$sql = "
			SELECT 
				par.retornacodigosubacao(s.sbaid) as localizacao,
				sd.sbddetalhamento
				
			FROM 
				par.processoparcomposicao ppc
			INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			WHERE
				ppc.ppcstatus = 'A' and
				ppc.prpid = {$prpId}
			AND 
				sd.sbddetalhamento is NOT NULL
	";
	$cabecalho = array("Localização", "Detalhamento");
	monta_lista_numerada($sql,$cabecalho,500,5,'N','95%','S');
?>

<table border="0" cellspacing="1" cellpadding="3" align="center" class="tabela" style="border: 1px; border-color: white; ">

	<tr>
		<td class="SubTitulocentro">Parecer</td>
	</tr>
</table>
<?php 		

	$sql = "
				SELECT DISTINCT
				par.retornacodigosubacao(s.sbaid) as localizacao,
				sd.sbdparecer
				
			FROM 
				par.processoparcomposicao ppc
			INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			WHERE
				ppc.ppcstatus = 'A' and
				ppc.prpid = {$prpId}
			AND 
				sd.sbdparecer is NOT NULL
	";
	$cabecalho = array("Localização", "Parecer");
	monta_lista_numerada($sql,$cabecalho,500,5,'N','95%','S');
?>


<?php 
// TERMO
$sql = "select * from (
			SELECT dpv.dpvcpf as cpfgestor, 
						to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
					FROM par.documentopar  dp
					INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
					INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
					INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
					LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
					left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
			union 
			SELECT dpv.dpvcpf as cpfgestor, 
						to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
					FROM par.documentopar  dp
					INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
					INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
					INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
					LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
					left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
			) as foo
		 WHERE dopid = ".$dopId; 

$html = $db->pegaLinha($sql);

$sql = "SELECT 
			dpv.dpvcpf as cpfgestor, 
			to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
			us.usunome, 
			us.usucpf
		FROM par.documentopar  dp
		INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
		INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
		WHERE 
			dpv.dpvstatus = 'A' and
			dp.dopid = ".$dopId;

$dadosValidacao = $db->carregar($sql);

$textoValidacao = "";

if(is_array($dadosValidacao)){
	foreach( $dadosValidacao as $dv ){
		$cpfvalidacao[] = $dv['cpfgestor'];
		$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
	}
}

$cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array();

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

//if( !empty($html['cpfgestor']) ){
if( $html['mdoqtdvalidacao'] == $html['qtdvalidado'] || in_array($_SESSION['usucpf'], $cpfvalidacao) ){
	$display = 'style="display: none;"';
	$displayValidado = '';
} else {
	if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
        in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
		in_array(PAR_PERFIL_PREFEITO, $perfil) || 
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) || 
		in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil)  
	){
		$display = '';
	} else {
		$display = 'style="display: none;"';
	}
	$displayValidado = 'style="display: none;"';
	if( $html['qtdvalidado'] >= 1 ){
		$displayValidado = '';
	}
}


if( $html['tpdcod'] != '102' && $html['tpdcod'] != '21' ) $display = 'style="display: none;"';

if( $_REQUEST['edita'] == 1 ){
	$display = 'style="display: none;"';
}

function monta_cabecalho_relatorio_par( $data = '', $largura ){
	
	global $db;
	
	$data = $data ? $data : date( 'd/m/Y' );
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">'			
				//.'		<td nowrap align="center" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>'				
//				.'			Acompanhamento da Execução Orçamentária<br/>'					
				.'			FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />'
				.'		</td>'
				//.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				//.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				//.'			Hora da Impressão: '.$data .' - ' . date( 'H:i:s' ) . '<br />'					
				//.'		</td>'	
				.'	</tr>'					
				.'</table><br><br>';					
								
		return $cabecalho;						
						
}

$doptexto = pegaTermoCompromissoArquivo( $dopid, '' );
?>
<style>

@media print {.notprint { display: none } .div_rolagem{display: none} }	
@media screen {.notscreen { display: none; }
.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
 
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function aceitarTermo(dopid){
		if(confirm('Eu li, e estou de acordo com o termo de compromisso.')){
			$('#aceitar').attr('disabled',true);
			$('#requisicao').val('aceita');
			
			$('#formulario').submit();
			
			<?php /*if( $_REQUEST['validacao'] == 'S' ){ ?>
				window.location.href = 'par.php?modulo=principal/teladeassinatura&acao=A&validacao=S&dopid='+dopid+'&aceita=S';
			<?php } else { ?>
				window.location.href = 'par.php?modulo=principal/teladeassinatura&acao=A&dopid='+dopid+'&aceita=S';
			<?php }*/ ?>
		}
	}
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="dopid" id="dopid" value="<?=$dopId ?>">
	<input type="hidden" name="validacao" id="validacao" value="<?=$_REQUEST['validacao']; ?>">
	
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
				<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$dopId ?>)" />
				<!-- <input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
				<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" /> -->
			</td>
		</tr>
	</table>
		<?
		$cabecalhoBrasao .= "<table width=\"95%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" >";
	$cabecalhoBrasao .= "<tr>" .
					"<td colspan=\"100\">" .($html['tpdcod'] == '101' ? monta_cabecalho_relatorio_par('29/11/2011', '100') : monta_cabecalho_relatorio_par('', '100') ).
					"</td>" .
				  "</tr>
				  </table>";
						echo $cabecalhoBrasao;
		?>
	<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td style="font-size: 12px; font-family:arial;">
				<div>
				<?php 
					echo html_entity_decode ($doptexto);
				?>
				</div>
			</td>
		</tr>
	</table>
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr <?=$displayValidado; ?> style="text-align: center;">
			<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
				<?=$textoValidacao; ?>
			</td>
		</tr>
	</table>
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
				<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$dopId ?>)" />
				<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
				<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
			</td>
		</tr>
	</table>
</form>