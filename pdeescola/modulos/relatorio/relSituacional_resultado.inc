<? 
ini_set( "memory_limit", "-1" );
       set_time_limit(0);
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php

if ($_GET['entid']){
	echo tabEscola($_GET['entid']);
	exit;
}

include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql     = monta_sql();
$dados   = $db->carregar($sql);
$agrup   = monta_agp();
$col     = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotNivel(false);
$r->setTolizadorLinha(true);
$r->setTotalizador(false);
$r->setTotNivel(false);
$r->setTotalizador = $dados;
$r->setBrasao(true);



//$r->setTextoTotalizador();
$r->setOverFlowTableHeight(false);

echo $r->getRelatorio();

$ineps = array();

//tratando $dados para exibir somente inep ->um por vez
foreach( $dados as $dado ){
	if( !in_array( $dado['inep'], $ineps )){//se $dado['inep'] não existe em $ineps, add através do array_push
		array_push( $ineps, $dado['inep'] );
		$j++;
	}
	//ver($dado['inep']);
}




// FILTROS DO RELATÓRIO:
function monta_sql(){
	extract($_POST);
	
	
	// Regiao
	if ($f_regiao[0] && $regiao_campo_flag){
		$where[] = " tr.regcod ".(!$f_regiao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_regiao)."') ";
	}
	// Estado
	if ($f_estuf[0] && $estuf_campo_flag){
		$where[] = " tm.estuf ".(!$f_estuf_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_estuf)."') ";
	}
	// Município
	if ($f_municipio[0] && $municipio_campo_flag){
		$where[] = " tm.muncod ".(!$f_municipio_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_municipio)."') ";
	}

	// Codigo INEP
	if ($f_inep[0] && $inep_campo_flag){
		$where[] = " e.entcodent ".(!$f_inep_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_inep)."') ";
	}
	
	if(!$agrupador[0]) {
		$agrupador = array();
	}
 
	$count = count($_POST['qapid']);
	$order = implode(",", $_POST['agrupador']);
	
					$sql = "SELECT DISTINCT 
							e.entid, 
							tm.estuf as ESTADO,
							regdescricao as REGIAO,
							tm.mundescricao as MUNICIPIO,
							e.entcodent as INEP, 
							e.entnome as NOME_ESCOLA, 
							epd.epiclasse as IDEB, 
							CASE
							    WHEN SUM(pde.pdevlrplanocusteio + pde.pdevlrplanocapital)  IS NOT NULL THEN SUM(pde.pdevlrplanocusteio + pde.pdevlrplanocapital)
							    ELSE 0.00
							END as PLANO,
							CASE
							    WHEN SUM(pde.pdevlrcomplementarcusteio + pde.pdevlrcomplementarcapital)  IS NOT NULL THEN SUM(pde.pdevlrcomplementarcusteio + pde.pdevlrcomplementarcapital)
							    ELSE 0.00
							END as PARCELA,
							CASE
							    WHEN est.esddsc IS NOT NULL THEN est.esddsc
							    ELSE 'Aguardando iniciação'::character varying
							END AS status, 
							CASE
							    WHEN hd.htddata IS NULL THEN ' -- '::text
							    ELSE to_char(hd.htddata, 'dd/mm/yyyy'::text)
							END AS data_ultimo_tramite
						FROM 
							entidade.entidade e
						JOIN 
							entidade.endereco endi ON endi.entid = e.entid
						JOIN 	
							territorios.municipio tm ON tm.muncod = endi.muncod::bpchar
						JOIN 
							territorios.estado te ON te.estuf = tm.estuf
						JOIN 
							territorios.regiao tr ON tr.regcod = te.regcod	
						JOIN 
							entidade.entidadedetalhe ed ON ed.entid = e.entid AND ed.entpdeescola = true	
						JOIN 
							pdeescola.entpdeideb epd ON epd.epientcodent = ed.entcodent::bpchar
						LEFT JOIN 
							pdeescola.pdeescola pde ON pde.entid = e.entid AND pde.pdeano = 2008::numeric
						LEFT JOIN 
							workflow.documento d ON d.docid = pde.docid
						LEFT JOIN 
							( SELECT historicodocumento.docid, max(historicodocumento.htddata) AS htddata FROM workflow.historicodocumento GROUP BY historicodocumento.docid) hd ON hd.docid = d.docid
						LEFT JOIN 
							workflow.estadodocumento est ON est.esdid = d.esdid
						WHERE 
							(e.tpcid = ANY (ARRAY[1, 3]))
							".$where = (!empty($where) ? "AND ".implode(" AND ", $where) : "")."
						GROUP BY
							e.entid, 
							tm.estuf, 
							tr.regdescricao,
							tm.mundescricao, 
							e.entcodent, 
							e.entnome, 
							epd.epiclasse, 
							pde.pdevlrplanocusteio, 
							pde.pdevlrplanocapital, 
							pde.pdevlrcomplementarcapital, 
							pde.pdevlrcomplementarcusteio,
							d.esdid, 
							est.esddsc, 
							hd.htddata
						ORDER BY
							$order";

	return $sql;
}

// Função para montar o array com os agrupadores
function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = array(
			"agrupador" => array(),
			"agrupadoColuna" => array(
										"nome_escola",
										"ideb",
										"plano",
										"parcela",
										"status",
										"data_ultimo_tramite"  		
									  )	  
			);
	
	foreach ($agrupador as $val): 
		switch ($val) {
			case 'regiao':
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Regiao")										
									   				);				
		    	continue;
		        break;
		    case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
									   				);				
		    	continue;
		        break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);					
		    	continue;
		        break;	
		    case 'inep':
				array_push($agp['agrupador'], array(
													"campo" => "inep",
											  		"label" => "Codigo INEP")										
									   				);					
		    	continue;
		        break;		
		}
	endforeach;
	
	return $agp;
}

function monta_coluna(){
	global $db;
	
	$coluna    = array(
					array(
						  "campo" => "nome_escola",
				   		  "label" => "Nome Escola",
						  "blockAgp" => array("estado", "municipio"),
					),	
					array(
						  "campo" => "ideb",
				   		  "label" => "IDEB"	
					),
					array(
						  "campo" => "plano",
				   		  "label" => "Plano"	
					),
					array(
						  "campo" 	 => "parcela",
				   		  "label" 	 => "Parcela"
					),	
					array(
						  "campo" 	 => "status",
				   		  "label" 	 => "Status"
					),	
					array(
						  "campo" => "data_ultimo_tramite",
				   		  "label" => "Data Tramitação"		  	
					)				
				  );
					  	
	return $coluna;			  	
}
?>
	
 </body>
 </html>