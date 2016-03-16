<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","1000M");
set_time_limit(0);


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
		var tipo = '<?=$_REQUEST['tipo']?>';
		location.href = '?modulo=relatorio/formRelPreen&acao=A&tipo=' + tipo;
	</script>
	<?
	die;
}

?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script>

			function abreRelatorio(){
			
				var indNivel = 0;
				
				d = d1.retornaElemento();

				abrirTodos(d[indNivel],'');
			} 
		
			function abrirTodos(elemento, idPai){
				if( elemento['elemento'].length == 0 ) return;
				var id = elemento['id'];

				window.setTimeout("d1.controle('" + id + "', '" + (idPai ? idPai : id) + "', '" + id + "_img')", 2);
				idPai = (idPai ? idPai + ":" + id : id);

				for( var i = 0; i < elemento['elemento'].length; i++ ){
					abrirTodos(elemento['elemento'][i], idPai);
				}
				
			}
		</script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

	echo '
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
		<tr>
			<td class="subtituloesquerda">
			<center>
			Relatório de Preenchimento<br>
			<a href="javascript: abreRelatorio();">Abrir / Fechar Todos</a>
			</center>
			</td>
		</tr>
	</table>
	';
//$r->setBrasao(true);
echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	extract($_POST);

	$select = array();
	$from	= array();
	
	$where = NULL;
	$select = NULL;
	$anoSelect = false;
	
	
	if( $tipo == 'est' ){
		$grouby .= 'es.estuf, ';
		$select1 .= " es.estuf as estado, ";
		$select2 .= " es.estuf as estado, ";
//		$selectgeral .= " foo.estado, ";
		$inner  .= " INNER JOIN territorios.estado 		es ON es.estuf = iu.estuf AND d.itrid = 1 ";		
		if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
			$where[0] = " AND es.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";
		}
	} else {
		$grouby .= 'iu.mun_estuf, m.mundescricao, ';
		$select1 .= " iu.mun_estuf as estado, m.mundescricao as municipio, ";
		$select2 .= " iu.mun_estuf as estado, m.mundescricao as municipio, ";
	//	$selectgeral .= " foo.estado, foo.municipio, ";
		$inner  .= " INNER JOIN territorios.municipio 	m  ON m.muncod = iu.muncod AND d.itrid = 2  ";		
		if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
			$where[0] = " AND iu.mun_estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";
		}
		if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
			$where[1] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";
		}
	}
	
	if( $grupo[0] && ( $grupo_campo_flag || $grupo_campo_flag == '1' )){
		$inner .= "inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid IN ('".implode( "','", $grupo )."')";
	}
	
	if( is_array($anos) ){
		$whereanos = implode(',', $anos);
	} else {
		$whereanos = "2011, 2012, 2013, 2014";
	}
	
	if( is_array( $agrupador ) ){
		if( in_array( 'item', $agrupador )) {
			if( $frmid == 2 ){ //Financeira
				$select1 .= " trim(si.icodescricao) as item, si.icodetalhe, si.icovalor as valor, si.icoquantidade as qtd, COALESCE(si.icoquantidade*si.icovalor,0) as valortotal, ";
				$select2 .= " trim(si.icodescricao) as item, si.icodetalhe, si.icovalor as valor, sum(COALESCE(ssi.seiqtd,0)) as qtd, COALESCE(sum(COALESCE(ssi.seiqtd,0))*si.icovalor,0) as valortotal, ";
				$grouby1 .= 'si.icodescricao, si.icodetalhe, si.icovalor, si.icoquantidade, ';
				$grouby2 .= 'si.icodescricao, si.icodetalhe, si.icovalor, ';
				$selectgeral2 .= " , trim(foo.item), foo.icodetalhe, foo.valortotal, foo.valor, foo.qtd ";
				$inner1  .= " INNER JOIN par.subacaoitenscomposicao 	si ON si.sbaid = s.sbaid -- AND si.icoano in (".$whereanos.")
							  INNER JOIN par.propostaitemcomposicao 	pi ON si.picid = pi.picid  ";		
				$inner2  .= " INNER JOIN par.subacaoitenscomposicao 	si ON si.sbaid = s.sbaid -- AND si.icoano in (".$whereanos.")
							  INNER JOIN par.propostaitemcomposicao 	pi ON si.picid = pi.picid  
							  INNER  JOIN par.subacaoescolas	se ON se.sbaid = s.sbaid AND se.sesano = sbd.sbdano
				  			  INNER JOIN par.subescolas_subitenscomposicao ssi ON se.sesid = ssi.sesid AND si.icoid = ssi.icoid	";	
			}
			if( in_array( 'ano', $agrupador )) {
				$select1 .= " si.icoano as ano, ";
				$select2 .= " si.icoano as ano, ";
	//			$selectgeral .= " ano, ";
				$grouby1 .= 'si.icoano, ';
				$grouby2 .= 'si.icoano, ';
				$anoSelect = true;
			}
		}
	}
	if( $itemcomposicao[0] != '' && ( $itemcomposicao_campo_flag ||$itemcomposicao_campo_flag == '1' )){
		$where[2] = " AND pi.picid " . (( $itemcomposicao_campo_excludente == null || $itemcomposicao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $itemcomposicao ) . "') ";
		if( !in_array( 'item', $agrupador )) {
			if( $frmid == 2 ){ //Financeira
				$select1 .= " trim(si.icodescricao) as item, si.icodetalhe, si.icovalor as valor, si.icoquantidade as qtd, COALESCE(si.icoquantidade*si.icovalor,0) as valortotal, ";
				$select2 .= " trim(si.icodescricao) as item, si.icodetalhe, si.icovalor as valor, sum(COALESCE(ssi.seiqtd,0)) as qtd, COALESCE(sum(COALESCE(ssi.seiqtd,0))*si.icovalor,0) as valortotal, ";
				$grouby1 .= 'si.icodescricao, si.icodetalhe, si.icovalor, si.icoquantidade, ';
				$grouby2 .= 'si.icodescricao, si.icodetalhe, si.icovalor, ';
				$selectgeral2 .= " , trim(foo.item), foo.icodetalhe, foo.valortotal, foo.valor, foo.qtd ";
				$inner1  .= " INNER JOIN par.subacaoitenscomposicao 	si ON si.sbaid = s.sbaid -- AND si.icoano in (".$whereanos.")
							  INNER JOIN par.propostaitemcomposicao 	pi ON si.picid = pi.picid  ";		
				$inner2  .= " INNER JOIN par.subacaoitenscomposicao 	si ON si.sbaid = s.sbaid -- AND si.icoano in (".$whereanos.")
							  INNER JOIN par.propostaitemcomposicao 	pi ON si.picid = pi.picid  
							  INNER  JOIN par.subacaoescolas	se ON se.sbaid = s.sbaid AND se.sesano = sbd.sbdano
				  			  INNER JOIN par.subescolas_subitenscomposicao ssi ON se.sesid = ssi.sesid AND si.icoid = ssi.icoid	";
			}
			if( in_array( 'ano', $agrupador )) {
				$select1 .= " si.icoano as ano, ";
				$select2 .= " si.icoano as ano, ";
	//			$selectgeral .= " ano, ";
				$grouby1 .= 'si.icoano, ';
				$grouby2 .= 'si.icoano, ';
				$anoSelect = true;
			}
		}
	}

	if( in_array( 'ano', $agrupador ) && $anoSelect == false ) {
		$select1 .= " sbd.sbdano as ano, ";
		$select2 .= " sbd.sbdano as ano, ";
//		$selectgeral .= " ano, ";
		$grouby1 .= 'sbd.sbdano, ';
		$grouby2 .= 'sbd.sbdano, ';
	}	
	
	if( $subacao[0] != '' && ( $subacao_campo_flag || $subacao_campo_flag == '1' )){
		$where[3] = " AND s.ppsid " . (( $subacao_campo_excludente == null || $subacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $subacao ) . "') ";
	}
	
	$selectgeral = implode(',', $agrupador);
	
	if( $frmid == 1 ){ //técnica
		if( $execpropria == 1 ){
			if( $tipo == 'est' ){
				$frm = "12";
				$innExecPro = "INNER JOIN par.propostatiposubacao 		pts  ON pts.frmid = s.frmid and pts.ptsid in(45,46)";
			} else {
				$frm = "4";
				$innExecPro = "INNER JOIN par.propostatiposubacao 		pts  ON pts.frmid = s.frmid and pts.ptsid in(42,43)";
			}
		} else {
			$frm = "2,4,12";
		}
		$select1 .= " COALESCE(sum(sbd.sbdquantidade),0) as quantidadesubacao, ";
		$select2 .= " COALESCE(sum(sse.sesquantidade),0) as quantidadesubacao, ";
		$grouby1 .= '';
		$grouby2 .= 'sse.sesquantidade,';
		$selectgeral2 .= " , foo.quantidadesubacao ";
		$inner1  .= "";		
		$inner2  .= " LEFT JOIN par.subacaoescolas sse ON sse.sbaid = s.sbaid AND sse.sesano = sbd.sbdano ";	
	} else { //Financeira
		if( $execpropria == 1 ){
			if( $tipo == 'est' ){
				$frm = "12";
				$innExecPro = "INNER JOIN par.propostatiposubacao 		pts  ON pts.frmid = s.frmid and pts.ptsid in(45,46)";
			} else {
				$frm = "4";
				$innExecPro = "INNER JOIN par.propostatiposubacao 		pts  ON pts.frmid = s.frmid and pts.ptsid in(42,43)";
			}
		} else {
			$frm = "6";
		}
	}
	
	
	$sql = "SELECT
			pais, ".$selectgeral." ".$selectgeral2."
			FROM (
			
			SELECT DISTINCT 
				".$select."
				".$select1."
				d.dimid,
				d.dimcod,
				d.dimdsc,
				a.areid,
				a.arecod,
				a.aredsc,
				i.indid,
				i.indcod,
				i.inddsc,
				ac.aciid,
				ac.acidsc,
				s.sbaid,
				'Brasil' as pais,
		--		COALESCE(sum(sbd.sbdquantidade),0) as quantidadesubacao,
				d.dimcod || '.' || a.arecod || '.' || i.indcod || '.' || s.sbaordem || ' - ' || s.sbadsc as subacao, 
			
				iu.inuid
			FROM par.dimensao 						d
			INNER JOIN par.area 					a  ON a.dimid  = d.dimid
			INNER JOIN par.indicador 				i  ON i.areid  = a.areid
			INNER JOIN par.criterio 				c  ON c.indid  = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.pontuacao 				p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.instrumentounidade 		iu ON iu.inuid = p.inuid
			INNER JOIN par.acao 					ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 					s  ON s.aciid  = ac.aciid AND s.sbastatus = 'A'
			".$innExecPro."
			LEFT JOIN par.subacaodetalhe			sbd ON s.sbaid = sbd.sbaid
			".$inner."
			".$inner1."
			WHERE
				s.frmid in ( ".$frm." )
				and  s.sbacronograma = 1
				AND sbd.sbdano IN ( ".$whereanos." )
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
			GROUP BY
				".$grouby."
				".$grouby1."
				d.dimid,
				d.dimcod,
				d.dimdsc,
				a.areid,
				a.arecod,
				a.aredsc,
				i.indid,
				i.indcod,
				i.inddsc,
				ac.aciid,
				ac.acidsc,
				s.sbaid,
				s.sbaordem,
				s.sbadsc,
				iu.inuid
			
			 union all 
			
			SELECT DISTINCT 
				".$select."
				".$select2."
				d.dimid,
				d.dimcod,
				d.dimdsc,
				a.areid,
				a.arecod,
				a.aredsc,
				i.indid,
				i.indcod,
				i.inddsc,
				ac.aciid,
				ac.acidsc,
				s.sbaid,
				'Brasil' as pais,
		--		COALESCE(sum(sbd.sbdquantidade),0) as quantidadesubacao,
				d.dimcod || '.' || a.arecod || '.' || i.indcod || '.' || s.sbaordem || ' - ' || s.sbadsc as subacao, 
			
				iu.inuid
			FROM par.dimensao 						d
			INNER JOIN par.area 					a  ON a.dimid  = d.dimid
			INNER JOIN par.indicador 				i  ON i.areid  = a.areid
			INNER JOIN par.criterio 				c  ON c.indid  = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.pontuacao 				p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
			INNER JOIN par.instrumentounidade 		iu ON iu.inuid = p.inuid
			INNER JOIN par.acao 					ac ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao 					s  ON s.aciid  = ac.aciid AND s.sbastatus = 'A'
			".$innExecPro."
			LEFT JOIN par.subacaodetalhe			sbd ON s.sbaid = sbd.sbaid
			".$inner."
			".$inner2."
			WHERE
				s.frmid in ( ".$frm." )
				and  s.sbacronograma = 2
				AND sbd.sbdano IN ( ".$whereanos." )
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
			GROUP BY
				".$grouby."
				".$grouby2."
				d.dimid,
				d.dimcod,
				d.dimdsc,
				a.areid,
				a.arecod,
				a.aredsc,
				i.indid,
				i.indcod,
				i.inddsc,
				ac.aciid,
				ac.acidsc,
				s.sbaid,
				s.sbaordem,
				s.sbadsc,
				iu.inuid
			
			)  as foo 
			ORDER BY
				".$selectgeral."
			";
		
	//ver($sql, d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"quantidadesubacao",
											"qtd",
											"valor",
											"valortotal"
										  )	  
				);
	
	$count = 1;
		$i = 0;
		
		if( $i > 1 || $i == 0 ){
			$vari = "Relatório de Preenchimento - PAR<br>";	
		}
	if( is_array($agrupador) ){
		
	$novoAgrup = array(); 
	array_push($novoAgrup, 'pais');
	if( !is_array($agrupador) ){
		$agrupador = array();
	}
	foreach ($agrupador as $val){
		array_push($novoAgrup, $val);
	}
		
	foreach ($novoAgrup as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'pais':
				array_push($agp['agrupador'], array(
													"campo" => "pais",
											  		"label" => "$var País")										
									   				);				
		   		continue;
		    break;
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "$var Município")										
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
		    case 'item':
				array_push($agp['agrupador'], array(
													"campo" => "item",
											 		"label" => "$var Item de Composição")										
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
		}
		$count++;
	endforeach;
	}
	return $agp;
}

function monta_coluna(){
	if( $_POST['frmid'] == 1 ){
		
	$coluna    = array(
						array(
							  "campo" => "quantidadesubacao",
					   		  "label" => "Quantidade",
							  "type"  => "numeric"
						)

					);

	} else {
		$coluna    = array(
						array(
							  "campo" => "valor",
					   		  "label" => "Valor Unitário(R$)",
							  "blockAgp" => array("estado", "municipio"),
							  "html"  => "<b>{valor}</b>"
						),
						array(
							  "campo" => "qtd",
					   		  "label" => "Quantidade de Itens",
						//	  "blockAgp" => array("estado", "municipio"),
							  "type"  => "numeric"
						),
						array(
							  "campo" => "valortotal",
					   		  "label" => "Valor Total(R$)",
							  "html"  => "<b>{valortotal}</b>"
						)

					);
	}
	return $coluna;			  	
	
}
?>
