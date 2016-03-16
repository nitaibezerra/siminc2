<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","250M");
set_time_limit(0);
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php

//Altera o titulo do relatorio
if(!$_REQUEST['titulo']) $_REQUEST['titulo'] = "Relatório Componente 2 <br><br> TIPO: ";
if( $_REQUEST['tiporel'] == 'S' ) {
	$_REQUEST['titulo'] .= "Avaliações Realizadas na SSE"; 	
}elseif( $_REQUEST['tiporel'] == 'N' ) {
	$_REQUEST['titulo'] .= "Avaliações Realizadas Fora da SSE";
}else{
	$_REQUEST['titulo'] .= "Avaliações Realizadas";
}


include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(false);
$r->setEspandir(true);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);
//$r->setBrasao(true);

if($_POST['tipo_relatorio'] == 'xls'){
	ob_clean();
	$nomeDoArquivoXls="relatorio_comp2_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}elseif($_POST['tipo_relatorio'] == 'html'){
	echo $r->getRelatorio();
}
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[1] = " AND m.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[2] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $equipe[0] && ( $equipe_campo_flag || $equipe_campo_flag == '1' )){
		$where[3] = " AND a.cnecodigocnes " . (( $equipe_campo_excludente == null || $equipe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $equipe ) . "') ";		
	}
	if( $idave[0] && ( $idave_campo_flag || $idave_campo_flag == '1' )){
		$where[3] = " AND a.cnecodigocnes " . (( $idave_campo_excludente == null || $idave_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $idave ) . "') ";		
	}
	/*
	if( $escola[0] && ( $escola_campo_flag || $escola_campo_flag == '1' )){
		$where[4] = " AND ent.entid " . (( $escola_campo_excludente == null || $escola_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $escola ) . "') ";		
	}
	*/
	
	if($tiporel){
		$where[4] = " AND av.avlavalsseoumonit='$tiporel' ";
	}
        if( $_REQUEST['ano']){
                $anoInicio = $_REQUEST['ano']."-01-01";
                $anoFim = $_REQUEST['ano']."-12-31";
		$where[5] = " AND avldtavaliacao between '{$anoInicio}' and  '{$anoFim}'";		
	}
			
	$sql = "SELECT 
				ne.niveldescricao as nivel, 
				av.avlqtpactuada as qtdpactuada,
				av.avlqtrealizada as qtdrealizada,
				ac.acaodescricao as acao,
				ent.entnome as escola,
				a.cnecodigocnes ||' - '|| a.cnenomefantasia ||' - '|| a.cneseqequipe as equipe,
				m.estuf as estado, 
				av.idave as idave, 
				m.mundescricao as municipio
			FROM pse.avaliaescola av
			INNER JOIN pse.niveldeensino ne ON ne.idnivel=av.idnivel
			INNER JOIN pse.acao ac ON ac.acaoid = av.acaoid and ac.comid = 2 and ac.acaoano=2013 and ac.acaoid in (25,26,28,29,30,31,35)
			INNER JOIN pse.escolabasepse esc ON esc.idebp = av.idebp
			INNER JOIN entidade.entidade ent ON ent.entcodent = esc.idecodinep
			INNER JOIN territorios.municipio m ON m.muncod = esc.muncod
			LEFT JOIN pse.scnesavaliaescola sc ON sc.idave = av.idave
			LEFT JOIN pse.scnes a ON sc.cneid = a.cneid
			WHERE	
				ent.entstatus='A'
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				".$where[4]."
                                ".$where[5]."
			ORDER BY 
				m.estuf, m.mundescricao, a.cnecodigocnes ||' - '|| a.cnenomefantasia ||' - '|| a.cneseqequipe, idave,
				ent.entnome, ac.acaodescricao";
// 	ver($sql,d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"nivel",
											"qtdpactuada",
											"qtdrealizada"		
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
		    case 'equipe':
				array_push($agp['agrupador'], array(
													"campo" => "equipe",
											  		"label" => "Equipe")										
									   				);					
		    	continue;
		    case 'idave':
				array_push($agp['agrupador'], array(
													"campo" => "idave",
											  		"label" => "Identificador")										
									   				);					
		    	continue;
		    break;
		    case 'nivel':
				array_push($agp['agrupador'], array(
													"campo" => "nivel",
											  		"label" => "Nível de Ensino")										
									   				);					
		    	continue;
		    break;
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											  		"label" => "Escola")										
									   				);					
		    	continue;
		    break;
		     case 'acao':
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											  		"label" => "Ação")										
									   				);					
		    	continue;
		    break;
		}
		$count++;
	endforeach;
	
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "nivel",
					   		  "label" => "Nível",
							  "type"  => "string"
						),
						array(
							  "campo" => "qtdpactuada",
					   		  "label" => "Quantidade Pactuada",
							  "type"  => "string"
						),
						array(
							  "campo" => "qtdrealizada",
					   		  "label" => "Quantidade Realizada",
							  "type"  => "numeric"
						)
					);
					

	return $coluna;			  	
	
}
?>
