<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
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
/*	if ($situacao){
		$where[] = "ed.esdid = '$situacao'";
	}*/
/* 	if ($_POST['f_situacao'][0]){
		
		$where[] = "ed.esdid = {$_POST['f_situacao'][0]}";
	} 	*/	

	if ($f_regiao[0] && $regiao_campo_flag){
		$where[] = " re.regcod ".(!$f_regiao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_regiao)."') ";
	}
	
	if ($f_estuf[0] && $estuf_campo_flag){
		$where[] = " m.estuf ".(!$f_estuf_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_estuf)."') ";
	}
	
	if ($f_municipio[0] && $municipio_campo_flag){
		$where[] = " m.muncod ".(!$f_municipio_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_municipio)."') ";
	}
	if ($f_situacao[0] && $situacao_campo_flag){
		$where[] = " ed.esdid ".(!$f_situacao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_situacao)."') ";
		
		 
	}
	/*
	 * Correção por Alexandre Dourado 17/11/2009
	 * - Testando se o agrupador existe e é um array, caso não exista defini-lo como array
	 */
	if(!$agrupador[0]) {
		$agrupador = array();
	}
 
	if ($grandescidades || ($f_ideb[0] && $ideb_campo_flag) || in_array('classe',$agrupador) ){
		$from[] = "	INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = m.muncod
					INNER JOIN territorios.tipomunicipio tm ON mtm.tpmid = tm.tpmid ";
		
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
				e.entcodent AS codigo,
				'<a href=\"javascript:void(0);\" onclick=\"janela(\'?modulo=relatorio/formRel&acao=A&entid=' || e.entid  || '&entcodent=' || e.entcodent || '\', 750, 300,\'entidade\'); \">' || e.entnome || '</a>' AS nome,
				m.estuf AS estado,
				m.mundescricao AS municipio,
				CASE
					WHEN ed.esdid IS NULL THEN 'Não iniciado'
					ELSE ed.esddsc
				END AS situacao,
				".(is_array($select) ? implode(',', $select).',' : '')."
		--		CASE ei.epiclasse
		--		  WHEN 'A' THEN 'Prioridades IDEB 2005'
		--		  WHEN 'B' THEN 'Prioridades IDEB 2007'
		--		  WHEN 'C' THEN 'Abaixo da Média IDEB 2007' 	
		--		END AS classe,
				ei.vlrpaf AS valor,
				1 AS quant
			FROM
				entidade.entidade e
				INNER JOIN pdeescola.entpdeideb ei ON ei.epientcodent = e.entcodent
				INNER JOIN entidade.endereco e1 ON e1.entid = e.entid
				INNER JOIN territorios.municipio m ON m.muncod = e1.muncod
				INNER JOIN territorios.estado est ON est.estuf = m.estuf
				INNER JOIN territorios.regiao re ON re.regcod = est.regcod				
				LEFT JOIN pdeescola.pdeescola p ON p.entid = e.entid	
				
				left join workflow.documento d on d.docid = p.docid
				left join workflow.estadodocumento ed on ed.esdid = d.esdid			
				".(is_array($from) ? implode(',', $from) : '')."
			".(is_array($where) ? " WHERE ".implode(' AND ', $where) : '')."
			ORDER BY
				estado, municipio, nome, {$order} valor
			";
	//dbg($sql,1);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
										/*	"codigo",
									   		"nome", 
 									   		"estado",
							   				"municipio", */
											"quant",
											"classe",
											"valor" 		   		
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
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
		    case 'classe':
				array_push($agp['agrupador'], array(
													"campo" => "classe",
											 		"label" => "Classe IDEB")										
									   				);					
		    	continue;			
		        break;	
		    case 'nome':
				array_push($agp['agrupador'], array(
												"campo" => "nome",
												"label" => "Escola")										
										   		);	
				continue;
				break;	  
			case 'situacao':
				array_push($agp['agrupador'], array(
												"campo" => "situacao",
												"label" => "Situação")										
										   		);	
				continue;
				break;	  
			  	
		}
	endforeach;
	
	return $agp;
}

function monta_coluna(){
	$coluna    = array(
					/*
					array(
						  "campo" 	 => "codigo",
				   		  "label" 	 => "Código"
					),
					array(
						  "campo" => "nome",
				   		  "label" => "Nome"
					),	
					array(
						  "campo" => "municipio",
				   		  "label" => "Município"	
					),
					array(
						  "campo" => "estado",
				   		  "label" => "Estado"	
					),*/
					array(
						  "campo" 	 => "quant",
				   		  "label" 	 => "Quantidade de Escolas",
				   		  "blockAgp" => "nome",
				   		  "type"	 => "numeric"
					),	
					array(
						  "campo" => "valor",
				   		  "label" => "Valor Paf"	
					)					
				  );
				  	
	return $coluna;			  	
}
?>
</body>