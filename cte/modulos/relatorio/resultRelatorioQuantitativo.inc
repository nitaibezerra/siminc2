<?

include APPRAIZ. 'includes/classes/relatorio.class.inc';

// Funзгo para montar o array com os agrupadores
function monta_agp() {
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
 									   		"sbadsc",
											"entnome",
											"ecsqtd"
										  )	  
				);
	
					
	array_push($agp['agrupador'], array("campo" => "entcodent", "label" => "Cуdigo INEP"));
	
	return $agp;
}

// Funзгo para montar as colunas do relatуrio
function monta_coluna() {

	$coluna    = array(
						array(
							  "campo" => "sbadsc",
					   		  "label" => "Descriзгo Subaзгo"
						),
						array(
							  "campo" => "entnome",
					   		  "label" => "Escola"
						),
						array(
							  "campo" => "ecsqtd",
					   		  "label" => "Quantidade"
						
						)
					);
	
return $coluna;
			  	
}

$sql = '
SELECT DISTINCT 
	\'\' as sbadsc,		
	\'\' as entcodent, 
	\'\' as entnome, 
	\'\' as ecsqtd
UNION ALL
SELECT DISTINCT
	s.sbadsc,
	ent.entcodent,
    ent.entnome,    
    	case 
    		when substring( trim(to_char(coalesce(ecs.ecsqtd, 0), \'9999999990D99\')), position(\',\' in trim(to_char(coalesce(ecs.ecsqtd, 0), \'9999999990D99\'))) +1, 2 ) = \'00\'
    			then trim(to_char(coalesce(ecs.ecsqtd, 0), \'999999999\'))
    		else trim(to_char(coalesce(ecs.ecsqtd, 0), \'9999999990D99\'))
    	END as ecsqtd
FROM
    cte.qtdfisicoano qfa
LEFT JOIN
    cte.escolacomposicaosubacao ecs ON qfa.qfaid = ecs.qfaid and ecs.cosid  = ' . (integer) $_REQUEST['cosid'] . '
INNER JOIN
    entidade.entidade ent ON qfa.entid = ent.entid
INNER JOIN cte.subacaoindicador s ON s.sbaid = qfa.sbaid
WHERE
    qfa.sbaid  = ' . (integer) $_REQUEST['sbaid'] . '
    and
    qfa.qfaano = ' . (integer) $_REQUEST['qfaano']. '
ORDER BY
    entnome';

$entidades = $db->carregar($sql);

if($entidades) {
	$agrup = monta_agp();
	$col   = monta_coluna();
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $entidades); 
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(true);
	
	ob_clean(); 
	$nomeDoArquivoXls = 'relatorio';
	echo $r->getRelatorioXls();
	
	
	die;


}
?>