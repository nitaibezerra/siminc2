<?php
ini_set("memory_limit","4000M");
set_time_limit(0);

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

$sql   = monta_sql();
if($_REQUEST['pesquisa']=='2') {
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=relatoriogerencial_par_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=relatoriogerencial_par_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$cabecalho = $_SESSION['par']['cabecalho'];
	/*
	extract( $_POST );
	$cabecalho = array("dimid","dimcod","dimensao","areid","arecod","area","indid","indcod","indicador","ptoid","aciid","acao","sbaid","subacao","sbaordem","frmid","frmdsc","crtpontuacao","sbaextraordinaria","pi","ptres","local","uf","icodescricao","quantidadesubacao","valorsubacao","quantidadesubacaoaprov","valorsubacaoaprov","qtditenssubacao","muncod");
	if( $documentos[0] || $dopusucpfvalidacaofnde || in_array('situacaoempenho', $agrupador2) || in_array('valorempenho', $agrupador2) || in_array('empnumeroprocesso', $agrupador2) || in_array('numerotermo', $agrupador2) ){
		array_push( $cabecalho, 'situacaoempenho', 'valorempenho', 'total', 'pagamentototal', 'empnumeroprocesso' );
		if( in_array('numerotermo', $agrupador2) || $documentos[0] || $dopusucpfvalidacaofnde[0] ){
			array_push( $cabecalho, "numerotermo" );
		}
	}
	array_push( $cabecalho, "status" );
*/
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2,'N',true);
	exit;
}



if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}


// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = '?modulo=relatorio/formRelatorioGerencial&acao=A';
	</script>
	<?
	die;
}


?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script>
			function abreFechaTodos(tipo){
				if( tipo == 'abre' ){
					jQuery('[title="Clique para expandir"]').each(function() {
						this.click();
					});
				} else {
					jQuery('[title="Clique para minimizar"]').each(function() {
						this.click();
					});
				}
			}
		</script>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';


$dados = $db->carregar($sql, null, 3600);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
//$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

//$r->setBrasao(true);
echo '
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
		<tr>
			<td class="subtituloesquerda">
			<center>
			<a href="javascript: abreFechaTodos(\'abre\');">Clique para Abrir Todos </a>/ <a href="javascript: abreFechaTodos(\'fecha\');">Clique para Fechar Todos</a>
			</center>
			</td>
		</tr>
	</table>
	';
echo $r->getRelatorio();

function monta_sql(){
	global $filtroSession;
	extract($_POST);

	$select = array();
	$from	= array();
	
	$where = NULL;
	$select = NULL;
	$anoSelect = false;
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		if( $esferafiltro == "e" ){
			$est = " AND iu.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";
		} else {
			$est = " AND iu.mun_estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";
		}
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$mun = " AND iu.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $forma[0] && ($forma_campo_flag || $forma_campo_flag == '1' )){
		$where[1] = " AND f.frmid " . (( $forma_campo_excludente == null || $forma_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $forma ) . "') ";		
	}
	if( $statuspar[0] && ($statuspar_campo_flag || $statuspar_campo_flag == '1' )){
		$where[2] = " AND d.esdid " . (( $statuspar_campo_excludente == null || $statuspar_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $statuspar ) . "') ";		
	}
	if( $statussubacao[0] && ($statussubacao_campo_flag || $statussubacao_campo_flag == '1' )){
		$where[3] = " AND d2.esdid " . (( $statussubacao_campo_excludente == null || $statussubacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $statussubacao ) . "') ";		
	}
	if( $dimensao[0] && ($dimensao_campo_flag || $dimensao_campo_flag == '1' )){
		$where2 = " AND d.dimcod " . (( $dimensao_campo_excludente == null || $dimensao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dimensao ) . "') ";		
	}
	if( $tiposubacao[0] && ($tiposubacao_campo_flag || $tiposubacao_campo_flag == '1' )){
		$where[5] = " AND s.ptsid " . (( $tiposubacao_campo_excludente == null || $tiposubacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tiposubacao ) . "') ";		
	}
	if( $subacao[0] && ($subacao_campo_flag || $subacao_campo_flag == '1' )){
		$where[6] = " AND s.ppsid " . (( $subacao_campo_excludente == null || $subacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $subacao ) . "') ";		
	}
	if( $grupo[0] && ($grupo_campo_flag || $grupo_campo_flag == '1' )){
		if( $grupo_campo_excludente == null || $grupo_campo_excludente == '0' ){
			$inner .= "inner join territorios.muntipomunicipio mtm on mtm.muncod = mun.muncod and mtm.estuf = mun.estuf and mtm.tpmid IN ('".implode( "','", $grupo )."')";
		} else {
			$condicao = " AND mun.muncod NOT IN (SELECT mtm.muncod FROM territorios.muntipomunicipio mtm WHERE mtm.tpmid IN ( '".implode( "','", $grupo )."' ))";
		}
	}
	if( $planointerno[0] && ($planointerno_campo_flag || $planointerno_campo_flag == '1' )){
		$where[9] = " AND sd.sbdplanointerno " . (( $planointerno_campo_excludente == null || $planointerno_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $planointerno ) . "') ";
	}
	if( $ptres[0] && ($ptres_campo_flag || $ptres_campo_flag == '1' )){
		$where[10] = " AND sd.sbdptres " . (( $ptres_campo_excludente == null || $ptres_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ptres ) . "') ";
	}
	if( $programa[0] && ($programa_campo_flag || $programa_campo_flag == '1' )){
		$where[11] = " AND s.prgid " . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $programa ) . "') ";
	}
	if( $documentos[0] && ($documentos_campo_flag || $documentos_campo_flag == '1' )){
		$where[12] = " AND dop.mdoid " . (( $documentos_campo_excludente == null || $documentos_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $documentos ) . "') ";
	}
	if( $itemcomposicao[0] && ($itemcomposicao_campo_flag || $itemcomposicao_campo_flag == '1' )){
		$where[7] = " AND sic.picid " . (( $itemcomposicao_campo_excludente == null || $itemcomposicao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $itemcomposicao ) . "') ";		
	}
	if( $itens == 'S' ){ //COM ITENS
		$where[4] = " AND f.frmid IN ( SELECT frmid FROM par.propostatiposubacao WHERE ptsid in (43, 45) ) ";		
	} elseif( $itens == 'N' ){ //SEM ITENS
		$where[4] = " AND f.frmid IN ( SELECT frmid FROM par.propostatiposubacao WHERE ptsid in (42, 46) ) ";		
	} elseif( $itens == 'T' ){ //SEM ITENS
		$where[4] = " ";		
	}
	if( in_array('ano', $agrupador) ){
		$sel = ", ds.sbdano as ano";
		$sel2 = ", sd.sbdano";
		$grp = ", ds.sbdano";
	}
	
	if( $documentos[0] || $dopusucpfvalidacaofnde || in_array('situacaoempenho', $agrupador2) || in_array('valorempenho', $agrupador2) || in_array('empnumeroprocesso', $agrupador2) || in_array('valorpagamento', $agrupador2) || in_array('numerotermo', $agrupador2) ){
		if( in_array('numerotermo', $agrupador2) || $documentos[0] || $dopusucpfvalidacaofnde[0] ){
			$innerEmpenho = " INNER JOIN par.empenhosubacao ems ON ems.sbaid = s.sbaid
							INNER JOIN par.empenho emp ON emp.empid = ems.empid  and eobstatus = 'A' and empstatus = 'A' 
							INNER JOIN par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso AND prpstatus = 'A' 
							LEFT JOIN par.pagamentosubacao pob 
								INNER JOIN par.pagamento pag ON pag.pagid = pob.pagid AND pag.pagsituacaopagamento not ilike '%CANCELADO%' AND pag.pagstatus = 'A'
							ON pob.sbaid = s.sbaid and pobstatus = 'A'";
			if( $documentos[0] || $dopusucpfvalidacaofnde ){
				$c = '';
				if( $dopusucpfvalidacaofnde == 's' ){
					$c = " AND dopusucpfvalidacaogestor IS NOT NULL ";
				} elseif( $dopusucpfvalidacaofnde == 'n' ){
					$c = " AND dopusucpfvalidacaogestor IS NULL ";
				}
				$innerEmpenho .= " INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid ".$c;
			} else {
				$innerEmpenho .= " LEFT JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid";
			}
			$termo1 = " dop.dopid || '/' || date_part('year',dopdatainclusao) as numerotermo, "; 
			$termo2 = " dop.dopid, dopdatainclusao, "; 
			$termo3 = " ds.numerotermo, "; 
		} else {
			$innerEmpenho = " INNER JOIN par.empenhosubacao ems ON ems.sbaid = s.sbaid and eobstatus = 'A'
							INNER JOIN par.empenho emp ON emp.empid = ems.empid and empstatus = 'A'  
							LEFT JOIN par.pagamento pag ON pag.empid = emp.empid and pagstatus = 'A' 
							LEFT JOIN par.pagamentosubacao pob ON pob.sbaid = s.sbaid and pag.pagid = pob.pagid and pobstatus = 'A'  
							";
		}
		$dadosEmpenho1 = " emp.empsituacao as situacaoempenho, ems.eobvalorempenho as valorempenho, eobpercentualemp as eobpercentualemp, pob.pobpercentualpag, to_char(emp.empnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as empnumeroprocesso, {$termo1} ";
		$groupEmpenho = " emp.empsituacao, ems.eobvalorempenho, eobpercentualemp, pob.pobpercentualpag, emp.empnumeroprocesso, {$termo2} ";
		$dadosEmpenho2 = " ds.situacaoempenho, ds.valorempenho, ds.valorsubaprov * (ds.eobpercentualemp/ds.qtditenssubacao) / 100 as totaltotal, ds.valorsubaprov * (ds.pobpercentualpag/ds.qtditenssubacao) / 100 as pagamentototal, ds.empnumeroprocesso, {$termo3} ";
	}
	
	
	if( $est ){
		$mun = "";
		$where[8] = " AND CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END ";
		$itrid = " AND d.itrid = 1 ";
		$tpcid = " AND ent.tpcid IN (1) ";
	} else {
		$where[8] = " AND iu.itrid = 2 ";
		$itrid = " AND d.itrid = 2 ";
		$tpcid = " AND ent.tpcid IN (3) ";
	}
	
	if( $esferafiltro == "e" ){
		$where[8] = " AND CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END ";
		$itrid = " AND d.itrid = 1 ";
		$tpcid = " AND ent.tpcid IN (1) ";
	} elseif( $esferafiltro == "m" ){
		$where[8] = " AND iu.itrid = 2 ";
		$itrid = " AND d.itrid = 2 ";
		$tpcid = " AND ent.tpcid IN (3) ";
	} else {
		$where[8] = " ";
		$tpcid = $tpcid ? $tpcid : " AND ent.tpcid IN (1,3) ";
	}

	if( $bp[0] == 'sim' ){
		$itrid = " AND d.itrid = 3 ";
	}
	
	if( $anos[0] ){
		$anosSubacao = " AND sbdano IN (".implode( ", ", $anos ).")";
	}
	
	if($_REQUEST['pesquisa']=='2') { // EXCEL
		$agrup = monta_agp();
		$col   = monta_coluna();
		$novoA = array();
		$novoC = array();
		foreach( $agrup['agrupador'] as $a ){
			$da = $a['campo'];
			if( $da == 'dimensao' ){
				$da = "d.dimcod || '.' || d.dimdsc as dimensao";
			} elseif( $da == 'area' ){
				$da = "a.arecod || '.' || a.aredsc as area";
			} elseif( $da == 'indicador' ){
				$da = "i.indcod || '.' || i.inddsc as indicador";
			} elseif( $da == 'subacao' ){
				$da = "ds.sbaordem || '.' || ds.sbadsc as subacao";
                        } elseif( $da == 'ano' ){
				$da = "ds.sbdano as ano";
			}elseif( $da == 'status' ){
				$da = "ds.esddsc as status";
			} 
			array_push($novoA, $da);
		}
		foreach( $col as $c ){
			$de = $c['campo'];
			if( $de == 'totaltotal' ){
				$de = "ds.valorsubaprov * (ds.eobpercentualemp/ds.qtditenssubacao) / 100 as totaltotal";
			} elseif( $de == 'pagamentototal' ){
				$de = "ds.valorsubaprov * (ds.pobpercentualpag/ds.qtditenssubacao) / 100 as pagamentototal";
			} elseif( $de == 'valorsubacaoaprov' ){
				$de = "ds.valorsubaprov AS valorsubacaoaprov";
			} elseif( $de == 'quantidadesubacaoaprov' ){
				$de = "ds.quantidadesubaprov AS quantidadesubacaoaprov";
			} elseif( $de == 'valorsubacao' ){
				$de = "ds.valorsub AS valorsubacao";
			} elseif( $de == 'quantidadesubacao' ){
				$de = "ds.quantidadesub AS quantidadesubacao";
			} elseif( $de == 'ptres' ){
				$de = "ds.sbdptres as ptres";
			} elseif( $de == 'pi' ){
				$de = "ds.sbdplanointerno as pi";
			} elseif( $de == 'status' ){
				$de = "ds.esddsc as status";
			} elseif ($de == 'empnumeroprocesso') {
//                            $de = "to_char(empnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as empnumeroprocesso";
                            $de = "empnumeroprocesso";
                        }
			array_push($novoC, $de);
		}
		$selectExcel = implode(', ',$novoA);
		$noo = implode(', ',$novoC);
		$_SESSION['par']['cabecalho'] = array_merge( $novoA, $novoC );
		if( $noo ){
			$selectExcel = $selectExcel . ", " . $noo;
		}
		$selectFinal = $selectExcel; 
	} else { // NÃO É EXCEL
		$selectFinal = "d.dimid,
						d.dimcod,
		             	d.dimcod || '.' || d.dimdsc as dimensao,
		             	a.areid,
		             	a.arecod,
		             	a.arecod || '.' || a.aredsc as area,
		             	i.indid,
		             	i.indcod,
		             	i.indcod || '.' || i.inddsc as indicador,
		             	ds.ptoid,
		             	ds.aciid,
		             	ds.acao as acao,
		             	ds.sbaid,
		             	ds.sbaordem || '.' || ds.sbadsc as subacao,
		             	ds.sbaordem,
		             	ds.frmid,
		             	ds.frmdsc,
		             	ds.crtpontuacao, 
		             	ds.sbaextraordinaria,
		             	ds.sbdplanointerno as pi,
		             	ds.sbdptres as ptres,
		             	ds.local,
		             	ds.uf,
		             	ds.icodescricao,
		             	ds.quantidadesub AS quantidadesubacao,
		             	ds.valorsub AS valorsubacao,
		             	ds.quantidadesubaprov AS quantidadesubacaoaprov,
		             	ds.valorsubaprov AS valorsubacaoaprov,
		             	ds.qtditenssubacao as qtditenssubacao,
		             	ds.muncod,
		             	{$dadosEmpenho2}
		             	ds.esddsc as status
						{$sel}";
	}
	
	$sql = " SELECT
				{$selectFinal}
			FROM 
				par.dimensao d
			INNER JOIN par.area a ON a.dimid = d.dimid
			INNER JOIN par.indicador i ON i.areid = a.areid 
			INNER JOIN 
            	(
				SELECT p.ptoid,
					ac.aciid,
					ac.acidsc as acao,
					s.sbaid,
					s.sbadsc,
					s.sbaordem,
					s.frmid,
					sd.sbdplanointerno,
					sd.sbdptres,
					f.frmdsc,
					s.sbaextraordinaria,
					c.indid,
					c.crtpontuacao,
					CASE WHEN iu.itrid = 1 THEN est.estdescricao ELSE mun.mundescricao END AS local,
			        CASE WHEN iu.itrid = 1 THEN est.estuf ELSE mun.estuf END AS uf,
			        sic.icodescricao,
			        0 as qtditenssubacao,
			        sic.icoquantidade as quantidadesub,
			        CASE WHEN s.frmid = 11 THEN sum(pre.prevalorobra) ELSE sum(sic.icovalor * sic.icoquantidade) END AS valorsub,
			        0 as quantidadesubaprov,
			        0 as valorsubaprov,
			        mun.muncod,
					{$dadosEmpenho1}
				    esd2.esddsc
				    {$sel2}
				FROM par.criterio         c 
				INNER JOIN par.pontuacao       				p   ON p.crtid  = c.crtid  AND p.ptostatus  = 'A'
				INNER JOIN par.instrumentounidade 			iu  ON iu.inuid = p.inuid  {$mun} {$est}
				INNER JOIN par.acao                			ac  ON ac.ptoid = p.ptoid  AND ac.acistatus = 'A'
				INNER JOIN workflow.documento				d   ON d.docid  = iu.docid
				LEFT JOIN territorios.municipio 			mun ON mun.muncod = iu.muncod
             	{$inner}
				LEFT JOIN territorios.estado 				est ON est.estuf = iu.estuf
				LEFT JOIN par.subacao               		s   ON s.aciid  = ac.aciid AND s.sbastatus  = 'A'
				{$innerEmpenho}
				LEFT JOIN workflow.documento				d2  ON d2.docid  = s.docid
				LEFT JOIN workflow.estadodocumento			esd2 ON d2.esdid  = esd2.esdid
				LEFT JOIN par.subacaodetalhe 				sd  ON sd.sbaid = s.sbaid {$anosSubacao}
				LEFT JOIN par.formaexecucao 				f   ON f.frmid = s.frmid
				LEFT JOIN par.subacaoitenscomposicao 		sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
				INNER JOIN par.subacaoobra so ON so.sbaid = s.sbaid AND sd.sbdano = so.sobano
				INNER JOIN obras.preobra pre ON pre.preid = so.preid AND pre.prestatus = 'A' AND pre.preano = sd.sbdano
				WHERE 	c.crtstatus = 'A'
					AND f.frmid  IN  ('11') 
					 {$where[1]}
                     {$where[2]}
                     {$where[3]}
                     {$where[4]}
                     {$where[5]}
                     {$where[6]}
                     {$where[7]}
                     {$where[8]}
                     {$where[9]}
                     {$where[10]}
                     {$where[11]}
                     {$where[12]}
                     {$condicao}
				GROUP BY  p.ptoid,
				                        ac.aciid,
				                         ac.acidsc,
				                         iu.itrid,
				                         est.estdescricao,
				                         est.estuf,
				                         mun.mundescricao,
				                         mun.estuf,
				                         s.sbaid,
				                         s.sbadsc,
				                         s.sbaordem,
				                         sd.sbdplanointerno,
				                         sd.sbdptres,
				                         s.frmid,
				                         f.frmdsc,
				                         s.sbaextraordinaria,
				                         c.indid,
				                         c.crtpontuacao, 
				                         sic.icodescricao,
				                         s.sbacronograma,
				                         mun.muncod,
				                         sd.sbdano,
				                         esd2.esddsc,
				                         {$groupEmpenho}
				                         sic.icoquantidade
				                         {$sel2}
				UNION ALL
				
				SELECT  p.ptoid,
				        ac.aciid,
				        ac.acidsc  as acao,
				        s.sbaid,
				        s.sbadsc,
				        s.sbaordem,
				        s.frmid,
				        sd.sbdplanointerno,
				        sd.sbdptres,
				        f.frmdsc,
				        s.sbaextraordinaria,
				        c.indid,
				        c.crtpontuacao,
				        CASE WHEN iu.itrid = 1 THEN est.estdescricao ELSE mun.mundescricao END AS local,
				        CASE WHEN iu.itrid = 1 THEN est.estuf ELSE mun.estuf END AS uf,
				        sic.icodescricao,
				        1 as qtditenssubacao,
				        CASE WHEN s.sbacronograma = 1 
				        THEN 
				        	sum(sic.icoquantidade) 
				        ELSE 
				        	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
							THEN -- escolas sem itens
								sum(coalesce(se.sesquantidade,0))
							ELSE -- escolas com itens
								sum(coalesce(ssi.seiqtd,0))
							END
						END AS quantidadesub,
				        CASE WHEN s.sbacronograma = 1 THEN sum(sic.icovalor * sic.icoquantidade)   ELSE sum(sic.icovalor * ssi.seiqtd)   END AS valorsub,
				        --CASE WHEN sd.ssuid IN (2, 12) THEN
					        CASE WHEN s.sbacronograma = 1 
					        THEN 
					        	CASE WHEN sic.icovalidatecnico = 'S'
								THEN
					        		sum(sic.icoquantidadetecnico)
					        	END 
					        ELSE 
					        	CASE WHEN sic.icovalidatecnico = 'S'
								THEN
						        	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
									THEN -- escolas sem itens
										sum(coalesce(se.sesquantidadetecnico,0))
									ELSE -- escolas com itens
										sum(coalesce(ssi.seiqtdtecnico,0))
									END
								END
							--END 
						END AS quantidadesubaprov,
						--CASE WHEN sd.ssuid IN (2, 12) THEN
							CASE WHEN sic.icovalidatecnico = 'S'
							THEN
				        		CASE WHEN s.sbacronograma = 1 THEN sum(sic.icovalor * sic.icoquantidadetecnico)   ELSE sum(sic.icovalor * ssi.seiqtdtecnico) END
				        	--END 
				        END AS valorsubaprov,
				        mun.muncod,
				        {$dadosEmpenho1}
				        esd2.esddsc
				        {$sel2}
             		FROM par.criterio         c 
             		INNER JOIN par.pontuacao       				p   ON p.crtid  = c.crtid  AND p.ptostatus  = 'A'
             		INNER JOIN par.instrumentounidade 			iu  ON iu.inuid = p.inuid   {$mun} {$est}
             		INNER JOIN par.acao                			ac  ON ac.ptoid = p.ptoid  AND ac.acistatus = 'A'
             		INNER JOIN workflow.documento				d   ON d.docid  = iu.docid
             		LEFT JOIN territorios.municipio 			mun ON mun.muncod = iu.muncod
             		{$inner}
             		LEFT JOIN territorios.estado 				est ON est.estuf = iu.estuf
             		LEFT JOIN par.subacao               		s   ON s.aciid  = ac.aciid AND s.sbastatus  = 'A'
             		{$innerEmpenho}
             		LEFT JOIN workflow.documento				d2  ON d2.docid  = s.docid
             		LEFT JOIN workflow.estadodocumento			esd2 ON d2.esdid  = esd2.esdid
             		LEFT JOIN par.subacaodetalhe 				sd  ON sd.sbaid = s.sbaid {$anosSubacao}
             		LEFT JOIN par.formaexecucao 				f   ON f.frmid = s.frmid
             		LEFT JOIN par.subacaoitenscomposicao 		sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
             		LEFT JOIN par.subacaoescolas 				se  ON se.sbaid = s.sbaid AND se.sesano = sd.sbdano
             		LEFT JOIN par.escolas esc ON esc.escid = se.escid 
             		LEFT JOIN entidade.entidade ent ON ent.entid = esc.entid and (ent.entescolanova = false or ent.entescolanova is null) AND ent.entstatus = 'A' {$tpcid}
					LEFT JOIN entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 3
             		LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
             		WHERE c.crtstatus = 'A'
                          AND f.frmid not IN  ('11') 
                         {$where[1]}
                         {$where[2]}
                         {$where[3]}
                         {$where[4]}
                         {$where[5]}
                         {$where[6]}
                         {$where[7]}
                         {$where[8]}
                         {$where[9]}
                         {$where[10]}
                         {$where[11]}
                         {$where[12]}
                         {$condicao}
              		GROUP BY  p.ptoid,
                        ac.aciid,
                         ac.acidsc,
                         iu.itrid,
                         est.estdescricao,
                         est.estuf,
                         mun.mundescricao,
                         mun.estuf,
                         s.sbaid,
                         s.sbadsc,
                         s.sbaordem,
                         s.frmid,
                         f.frmdsc,
                         s.sbaextraordinaria,
                         c.indid,
                         c.crtpontuacao, 
                         sic.icodescricao,
                         s.sbacronograma,
                         sd.sbdplanointerno,
                         sd.sbdptres,
                         esd2.esddsc,
                         icovalidatecnico,
                         ssi.seiqtd,
                         ssi.seiqtdtecnico,
                         {$groupEmpenho}
                         se.sesquantidade,
                         sd.sbdano,
                         s.ptsid,
                         mun.muncod,
                         sd.ssuid
                         {$sel2}
           ) AS ds ON i.indid = ds.indid  
			WHERE
				dimstatus = 'A' AND
            	arestatus = 'A' AND
            	indstatus = 'A' AND
            	i.indid not in (213, 214, 220, 267, 268, 269, 270)
                {$where2} {$itrid}
		ORDER BY 
				".implode(", ", $_POST['agrupador']);
	/*
	
	$sql = "SELECT
				d.dimid,
				d.dimcod,
             	d.dimcod || '.' || d.dimdsc as dimensao,
             	a.areid,
             	a.arecod,
             	a.arecod || '.' || a.aredsc as area,
             	i.indid,
             	i.indcod,
             	i.indcod || '.' || i.inddsc as indicador,
             	ds.ptoid,
             	ds.aciid,
             	ds.acidsc as acao,
             	ds.sbaid,
             	ds.sbaordem || '.' || ds.sbadsc as subacao,
             	ds.sbaordem,
             	ds.frmid,
             	ds.frmdsc,
             	ds.crtpontuacao, 
             	ds.sbaextraordinaria,
             	ds.local,
             	ds.uf,
             	ds.icodescricao,
             	ds.valorsub AS valorsubacao
				{$sel}
			FROM 
				par.dimensao d
			INNER JOIN par.area a ON a.dimid = d.dimid
			INNER JOIN par.indicador i ON i.areid = a.areid 
			INNER JOIN 
            	(SELECT  p.ptoid,
                         ac.aciid,
                         ac.acidsc,
                         s.sbaid,
                         s.sbadsc,
                         s.sbaordem,
                         s.frmid,
                         f.frmdsc,
                         s.sbaextraordinaria,
                         c.indid,
                         c.crtpontuacao,
                         CASE WHEN iu.itrid = 1 THEN est.estdescricao ELSE mun.mundescricao END AS local,
                         CASE WHEN iu.itrid = 1 THEN est.estuf ELSE mun.estuf END AS uf,
                         sic.icodescricao,
                        -- CASE WHEN s.sbacronograma = 1 THEN sum(sic.icovalor * sic.icoquantidade)   ELSE sum(sic.icovalor * ssi.seiqtd)   END as valorsub 
                         CASE WHEN s.frmid = 11 THEN sum(pre.prevalorobra) ELSE 
							CASE WHEN s.sbacronograma = 1 THEN sum(sic.icovalor * sic.icoquantidade)   ELSE sum(sic.icovalor * ssi.seiqtd)   END
                         END AS valorsub
                         {$sel2}
             		FROM par.criterio         c 
             		INNER JOIN par.pontuacao       				p   ON p.crtid  = c.crtid  AND p.ptostatus  = 'A'
             		INNER JOIN par.instrumentounidade 			iu  ON iu.inuid = p.inuid {$mun} {$est}
             		INNER JOIN par.acao                			ac  ON ac.ptoid = p.ptoid  AND ac.acistatus = 'A'
             		INNER JOIN workflow.documento				d   ON d.docid  = iu.docid
             		LEFT JOIN territorios.municipio 			mun ON mun.muncod = iu.muncod
             		LEFT JOIN territorios.estado 				est ON est.estuf = iu.estuf
             		LEFT JOIN par.subacao               		s   ON s.aciid  = ac.aciid AND s.sbastatus  = 'A'
             		LEFT JOIN workflow.documento				d2  ON d2.docid  = s.docid
             		LEFT JOIN par.subacaodetalhe 				sd  ON sd.sbaid = s.sbaid
             		LEFT JOIN par.formaexecucao 				f   ON f.frmid = s.frmid
             		LEFT JOIN par.subacaoitenscomposicao 		sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
             		LEFT JOIN par.subacaoescolas 				se  ON se.sbaid = s.sbaid AND se.sesano = sd.sbdano
             		LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
             		LEFT JOIN par.subacaoobra so ON so.sbaid = s.sbaid AND sd.sbdano = so.sobano
             		LEFT JOIN obras.preobra pre ON pre.preid = so.preid AND pre.prestatus = 'A' AND pre.preano = sd.sbdano
             		WHERE 
             		--	p.inuid = 641 AND -- and s.sbaid = 1790952
              			c.crtstatus = 'A'
                         {$where[1]}
                         {$where[2]}
                         {$where[3]}
                         {$where[4]}
              			-- AND sd.sbdano in (2011,2012) caso seja por ano
              		GROUP BY  p.ptoid,
                        ac.aciid,
                         ac.acidsc,
                         iu.itrid,
                         est.estdescricao,
                         est.estuf,
                         mun.mundescricao,
                         mun.estuf,
                         s.sbaid,
                         s.sbadsc,
                         s.sbaordem,
                         s.frmid,
                         f.frmdsc,
                         s.sbaextraordinaria,
                         c.indid,
                         c.crtpontuacao, 
                         sic.icodescricao,
                         s.sbacronograma 
                         {$sel2}
              
             ) AS ds ON i.indid = ds.indid  
			WHERE
				d.itrid = {$itrid} AND
            	dimstatus = 'A' AND
            	arestatus = 'A' AND
            	indstatus = 'A' AND
            	i.indid not in (213, 214, 220, 267, 268, 269, 270) 
       
	            
			ORDER BY 
				d.dimcod,a.arecod,i.indcod, ds.sbaordem";
		*/
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"quantidadesubacao",
											"valorsubacao",
											"quantidadesubacaoaprov",
											"valorsubacaoaprov",
											"status",
											"pi",
											"ptres",
											"situacaoempenho",
											"valorempenho",
											"valorpagamento",
											"totaltotal",
											"pagamentototal",
											"empnumeroprocesso",
											"numerotermo",
											"muncod"
											//"qtditenssubacao"
										  )	  
				);
	
	$count = 1;
		$i = 0;
		
		if( $i > 1 || $i == 0 ){
			$vari = "Relatório Gerencial - PAR<br>";
		}
	if( is_array($agrupador) ){
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}

		switch ($val) {
			case 'frmdsc':
				array_push($agp['agrupador'], array(
													"campo" => "frmdsc",
											  		"label" => "$var Forma de Execução")										
									   				);				
		   		continue;
		    break;
		    case 'ano':
				array_push($agp['agrupador'], array(
													"campo" => "ano",
											  		"label" => "$var Ano")										
									   				);					
		    	continue;
		    break;		    	
		    case 'local':
				array_push($agp['agrupador'], array(
													"campo" => "local",
											 		"label" => "$var Descrição")										
									   				);					
		   		continue;			
		    break;
		    case 'uf':
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											 		"label" => "$var UF")										
									   				);					
		   		continue;			
		    break;
		    case 'ano':
				array_push($agp['agrupador'], array(
													"campo" => "ano",
											 		"label" => "$var Ano")										
									   				);					
		   		continue;			
		    break;
		    case 'dimensao':
				array_push($agp['agrupador'], array(
													"campo" => "dimensao",
											 		"label" => "$var Indicador")										
									   				);					
		   		continue;			
		    break;
		    case 'area':
				array_push($agp['agrupador'], array(
													"campo" => "area",
											 		"label" => "$var Área")										
									   				);					
		   		continue;			
		    break;
		    case 'indicador':
				array_push($agp['agrupador'], array(
													"campo" => "indicador",
											 		"label" => "$var Indicador")										
									   				);					
		   		continue;			
		    break;
		    case 'acao':
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											 		"label" => "$var Ação")										
									   				);					
		   		continue;			
		    break;
		    case 'subacao':
				array_push($agp['agrupador'], array(
													"campo" => "subacao",
											 		"label" => "$var Subação")										
									   				);					
		   		continue;			
		    break;
		    case 'icodescricao':
				array_push($agp['agrupador'], array(
													"campo" => "icodescricao",
											 		"label" => "$var Itens de Composição")										
									   				);					
		   		continue;			
		    break;
		    case 'status':
				array_push($agp['agrupador'], array(
													"campo" => "status",
											 		"label" => "$var Status da Subação")										
									   				);					
		   		continue;			
		    break;
		}
		$count++;
	endforeach;
	}
	return $agp;
}

function monta_coluna(){

	$coluna = array();
	
	$coluna2 = $_POST['agrupador2'];
	
	if( is_array($coluna2) ){
	foreach ($coluna2 as $val):
		switch ($val) {
			case 'quantidadesubacao':
				array_push($coluna, array(
													"campo" => "quantidadesubacao",
											   		"label" => "Quantidade Planejada",
											   		"type" => "numeric",
													"html"  => "<b>{quantidadesubacao}</b>")									
									   				);				
		   		continue;
		    break;
			case 'valorsubacao':
				array_push($coluna, array(
													"campo" => "valorsubacao",
											   		"label" => "Valor Planejado(R$)",
													"html"  => "<b>{valorsubacao}</b>")									
									   				);				
		   		continue;
		    break;
			case 'quantidadesubacaoaprov':
				array_push($coluna, array(
												"campo" => "quantidadesubacaoaprov",
										   		"label" => "Quantidade Aprovada",
												"type" => "numeric",
												"html"  => "<b>{quantidadesubacaoaprov}</b>")										
									   				);				
		   		continue;
		    break;
			case 'valorsubacaoaprov':
				array_push($coluna, array(
												"campo" => "valorsubacaoaprov",
										   		"label" => "Valor Aprovado(R$)",
												"html"  => "<b>{valorsubacaoaprov}</b>")										
									   				);				
		   		continue;
		    break;
			case 'status':
				array_push($coluna, array(
												"campo" => "status",
										   		"label" => "Status da Subação",
												"html"  => "<b>{status}</b>")										
									   				);				
		   		continue;
		    break;
			case 'pi':
				array_push($coluna, array(
												"campo" => "pi",
										   		"label" => "Plano Interno",
												"html"  => "<b>{pi}</b>")										
									   				);				
		   		continue;
		    break;
			case 'ptres':
				array_push($coluna, array(
												"campo" => "ptres",
										   		"label" => "PTRES",
												"type" => "string",
												"html"  => "<b>{ptres}</b>")										
									   				);				
		   		continue;
		    break;
			case 'situacaoempenho':
				array_push($coluna, array(
												"campo" => "situacaoempenho",
										   		"label" => "Situação do Empenho",
												"html"  => "<b>{situacaoempenho}</b>")										
									   				);				
		   		continue;
		    break;
			case 'empnumeroprocesso':
				array_push($coluna, array(
												"campo" => "empnumeroprocesso",
										   		"label" => "Número do Processo",
												"type" => "string",
												"html"  => "<b>{empnumeroprocesso}</b>")										
									   				);				
		   		continue;
		    break;
			case 'numerotermo':
				array_push($coluna, array(
												"campo" => "numerotermo",
										   		"label" => "Número do Termo de Compromisso",
												"type" => "string",
												"html"  => "<b>{numerotermo}</b>")										
									   				);				
		   		continue;
		    break;
			case 'muncod':
				array_push($coluna, array(
												"campo" => "muncod",
										   		"label" => "Código IBGE",
												"type" => "string",
												"mostraNivel" => "local",
												"html"  => "<b>{muncod}</b>")										
									   				);				
		   		continue;
		    break;
			case 'valorempenho':
				array_push($coluna, array(
												"campo" => "totaltotal",
										   		"label" => "Valor do Empenho",
											//	"mostraNivel" => "local",
											//	"type" => "string",
												"html"  => "<b>{totaltotal}</b>",
											//	"html"  => "<b>{valorempenhocalculado}</b>",
											//	"php"   => array("expressao" => "is_numeric({valorempenho})",
											//						"type"	 => "numeric",
											//						"var"	 => "valorempenhocalculado",
											//						"true"	 => "number_format(({valorempenho} / {qtditenssubacao}),2,',','.')",
											//						"false"	 => "")
										)										
									   				);				
		   		continue;
		    break;
			case 'valorpagamento':
				array_push($coluna, array(
												"campo" => "pagamentototal",
										   		"label" => "Valor do Pagamento",
												"html"  => "<b>{pagamentototal}</b>")										
									   	);				
		   		continue;
		    break;
		}
	endforeach;
	}
	return $coluna;			  	
	
}
 
?>
