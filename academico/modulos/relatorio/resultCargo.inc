<?
if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

//xx($_POST);

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
//dbg($sql,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setBrasao(true);
echo $r->getRelatorio();

function monta_sql(){
	extract($_POST);

	if ($f_tipoensino){
		$where[] = "o.orgid = '{$f_tipoensino}'"; 		
	}
		
//	if ($f_tipoensino){
//		$f_tipoensino = (array) explode(',', $f_tipoensino); 		
//	}
//	
//	if ($f_tipoensino[0] && $f_tipoensino_campo_flag){
//		$where[] = " o.orgid ".($f_tipoensino_campo_excludente == 'false' ? ' IN ' : ' NOT IN ')."('".implode("','",$f_tipoensino)."') ";
//	}
	
/*	
	if ($tipo){
		$where[] = "e.tpcid IN($tipo)";
	}
	
	if ($valini){
		 $where[] = "ei.vlrpaf >= ".str_replace(array(".",","),array("","."),$valini);
	}
	
	if ($valfim){
		$where[] = "ei.vlrpaf <= ".str_replace(array(".",","),array("","."),$valfim);
	}
	
	if ($epiclasse){
		$where[] = "ei.epiclasse = '$epiclasse'";
	}		

	if ($f_regiao[0] && $regiao_campo_flag){
		$where[] = " re.regcod ".(!$f_regiao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_regiao)."') ";
	}
	
	if ($f_estuf[0] && $estuf_campo_flag){
		$where[] = " m.estuf ".(!$f_estuf_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_estuf)."') ";
	}
	
	if ($f_municipio[0] && $municipio_campo_flag){
		$where[] = " m.muncod ".(!$f_municipio_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_municipio)."') ";
	}

	if ($f_acao[0] && $acao_campo_flag){
		$where[] = " dpa.dpaid ".(!$f_acao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_acao)."') ";
	}
	
	if ($f_programa[0] && $programa_campo_flag){
		$where[] = " tp.tprid ".(!$f_programa_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_programa)."') ";
	}

	if ($f_categoria[0] && $categoria_campo_flag){
		$where[] = " tc.tcaid ".(!$f_categoria_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_categoria)."') ";
	}	

	if ($f_fonte[0] && $fonte_campo_flag){
		$where[] = " fr.forid ".(!$f_fonte_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_fonte)."') ";
	}
	if ($esdid)	
		$where[] = " estdoc.esdid = '".$esdid."'";		
	
	if ($grandescidades || ($f_ideb[0] && $ideb_campo_flag) || in_array('classe',$agrupador) ){
		$from[] = "	LEFT JOIN territorios.muntipomunicipio mtm ON mtm.muncod = m.muncod
					LEFT JOIN territorios.tipomunicipio tm ON mtm.tpmid = tm.tpmid ";
		
		$where[] = "(tm.gtmid = 1 OR tm.gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) ) AND
					tm.tpmstatus = 'A'"; 
		
		if ($grandescidades){
			$where1[] = " tm.tpmid IN ({$grandescidades})";		
		}
		
		if ($f_ideb[0] && $ideb_campo_flag){
			$where1[] = " tm.tpmid ".(!$f_ideb_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_ideb)."') ";			
		}
		
		if (is_array($where1)){
			$where[] = '('.implode(' OR ', $where1).')';
		}
		
		if ($agrupador){
			$select[] = " tm.tpmdsc AS classe ";
		}
		
		$order = 'classe, ';
	}

	$sql = "SELECT
				DISTINCT
				pai.paidescricao AS pais,
				e.entcodent AS codigo,
				e.entnome AS nome,
				m.estuf AS estado,
				m.mundescricao AS municipio,
				".(is_array($select) ? implode(',', $select).',' : '')."
				dpadescricaoacao AS acao,
				dpavalorcapital::float AS capital,
				dpavalorcusteio::float AS custeio,
				tprdescricao AS programa,
				tcadescricao AS categoria,
				fordescricao AS fonte,
				1 AS quant
			FROM
				entidade.entidade e 
				INNER JOIN pdeescola.entpdeideb ei ON ei.epientcodent = e.entcodent
				INNER JOIN entidade.endereco e1 ON e1.entid = e.entid 
				INNER JOIN territorios.municipio m ON m.muncod = e1.muncod
				INNER JOIN territorios.estado est ON est.estuf = m.estuf
				INNER JOIN territorios.regiao re ON re.regcod = est.regcod
				INNER JOIN territorios.pais pai ON pai.paiid = re.paiid
				LEFT JOIN pdeescola.pdeescola p	ON p.entid = e.entid			
				LEFT JOIN pdeescola.planosuporteestrategico ps ON ps.pdeid = p.pdeid
				LEFT JOIN pdeescola.planoacao pa ON pa.pseid = ps.pseid
				LEFT JOIN pdeescola.detalheplanoacao dpa ON dpa.plaid = pa.plaid
				LEFT JOIN pdeescola.tipoprograma tp ON tp.tprid = dpa.tprid
				LEFT JOIN pdeescola.tipocategoria tc ON tc.tcaid = dpa.tcaid
				LEFT JOIN pdeescola.fonterecurso fr ON fr.forid = dpa.forid
				LEFT JOIN workflow.documento d ON d.docid = p.docid
				LEFT JOIN workflow.estadodocumento estdoc ON estdoc.esdid = d.esdid 
				".(is_array($from) ? implode(',', $from) : '')."
			".(is_array($where) ? " WHERE ".implode(' AND ', $where)." and dpadescricaoacao != ''" : '')."
			ORDER BY
				estado, municipio, nome--, {$order} pais
			";
	//xd($sql);
	 * 
	 */
	$sql = "SELECT
				--distinct
				o.orgdesc as tipoensino,
				u.entnome as unidade,
				cp.entnome as campus,
				c.clsdsc as classe,
				p.prtid || ' - ' || p.prtnumero as portaria,
				p.prtano as ano,
				pr.prgdsc as programa,
				ap.acpvalor as projetado,
				lp.lnpvalor as autorizado
			FROM
				academico.portarias p
				INNER JOIN academico.orgao o ON o.orgid = p.orgid AND 
												o.orgstatus = 'A'
				INNER JOIN academico.programa pr ON pr.prgid = p.prgid
				INNER JOIN academico.lancamentosportaria lp ON lp.prtid = p.prtid AND 
									       					   lnpstatus = 'A'
				INNER JOIN academico.acumuladoprojetado ap ON ap.prtid = p.prtid AND
									      ap.acpstatus = 'A'
				INNER JOIN academico.classes c ON c.clsid = lp.clsid AND
												  c.clsid = ap.clsid AND
												  c.clsstatus = 'A' 	
				INNER JOIN entidade.entidade cp ON cp.entid = lp.entidcampus AND
								   				   cp.entid = ap.entidcampus
				INNER JOIN entidade.entidade u ON u.entid = lp.entidentidade AND
								  				  u.entid = ap.entidentidade					   
			WHERE
				prtstatus = 'A' AND
				lp.clsid = ap.clsid " . 
				(is_array($where) ? " AND ".implode(' AND ', $where) : '') . 
			" ORDER BY c.clsid ASC;";
	
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
//	dbg($_POST,1);
//	if (!$agrupador){
//		$agrupador = array(
//							'tipoensino',
///*							'ano',
//							'portaria',
//							'unidade',
//							'campus',
//							'programa',
//							'classe'*/
//						 );
//	}else{
//		$agrupador = explode(",", $agrupador);
//	}	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
									   		"autorizado", 
 									   		"projetado"
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'tipoensino':
				array_push($agp['agrupador'], array(
													"campo" => "tipoensino",
											  		"label" => "Tipo Ensino")										
									   				);				
		    	continue;
		        break;
		    case 'portaria':
				array_push($agp['agrupador'], array(
													"campo" => "portaria",
											  		"label" => "Portaria")										
									   				);					
		    	continue;
		        break;		    	
		    case 'campus':
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											 		"label" => "Campus")										
									   				);					
		    	continue;			
		        break;	
		    case 'classe':
				array_push($agp['agrupador'], array(
												"campo" => "classe",
												"label" => "Classe")										
										   		);	
				continue;
				break;	    	
		    case 'programa':
				array_push($agp['agrupador'], array(
												"campo" => "programa",
												"label" => "Programa")										
										   		);	
				continue;
				break;	    	
		    case 'unidade':
				array_push($agp['agrupador'], array(
												"campo" => "unidade",
												"label" => "Unidade")										
										   		);	
				continue;
				break;					
		    case 'ano':
				array_push($agp['agrupador'], array(
												"campo" => "ano",
												"label" => "ano")										
										   		);	
				continue;
				break;					
		}
	endforeach;
	return $agp;
}

function monta_coluna(){
	$coluna    = array(
					array(
						  "campo" => "projetado",
				   		  "label" => "Projetado",
						  "type"  => "numeric"
					),
					array(
						  "campo" => "autorizado",
				   		  "label" => "Autorizado",
						  "type"  => "numeric"
					)					
				  );
				  	
	return $coluna;			  	
}
?>
</body>