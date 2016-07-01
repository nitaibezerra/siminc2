<?php
	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	
	if ($_POST['agrupador']){
		header('Content-Type: text/html; charset=iso-8859-1'); 
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
	$r->setMonstrarTolizadorNivel(true);
	$r->setTotalizador(true);
	$r->setTolizadorLinha(true);

	if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_analitico_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();	
	}
?>

<?php 
function monta_sql(){
	global $filtroSession, $db;
	extract($_POST);
	
	if($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
		$where[0] = " AND par.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if($datainicio != '' && $datafim != ''){
		$where[1] = "AND evt.evtdataini BETWEEN '".formata_data_sql($datainicio)." 00:00:00' AND '".formata_data_sql($datafim)." 23:59:59'";		
	} 

	if($parlamentar != ''){
		$where[2] = " AND pes.pesid = {$parlamentar}";		
	}	

	if($partido != ''){
		$where[3] = " AND par.parpartido = '$partido'";		
	}	
	
	if($parlamentar != ''){
		$sql_evento = "SELECT evt.evtid 
					  FROM atendimento.evento evt
					  LEFT JOIN atendimento.pessoaevento pev ON pev.evtid = evt.evtid
					  WHERE pev.pesid = {$parlamentar}";
		$rsEvento = $db->pegaLinha($sql_evento);
		if(!empty($rsEvento)){
			$condicao = "AND ev.evtid = {$rsEvento['evtid']}";			
		} 
		$col_sql = "
			array_to_string(array(
							SELECT pe.pesnome
							FROM atendimento.pessoa pe 
							INNER JOIN atendimento.pessoaevento pv ON pe.pesid = pv.pesid
							INNER join atendimento.evento ev on ev.evtid = pv.evtid
							WHERE pv.tppid = 2 AND pv.pevstatus = 'A' $condicao),',') as participante_externo,
			array_to_string(array(
							SELECT pe.pesnome
							FROM atendimento.pessoa pe 
							INNER JOIN atendimento.pessoaevento pv ON pe.pesid = pv.pesid
							INNER join atendimento.evento ev on ev.evtid = pv.evtid
							WHERE pv.tppid = 1 AND pv.pevstatus = 'A' $condicao),',') as participante_mec";
	} else {
		$col_sql="'' as participante_externo, '' as participante_mec";
	}
	
	$sql = "SELECT evt.evtid, pes.pesnome as parlamentar, to_char(evt.evtdataini,'DD/MM/YYYY') as evtdataini, 
			evt.evtpauta, evt.evtencaminhamentos, par.parpartido, par.estuf,
			$col_sql
			FROM atendimento.evento evt
			LEFT JOIN atendimento.pessoaevento pev ON pev.evtid = evt.evtid
			LEFT JOIN atendimento.pessoa pes ON pev.pesid = pes.pesid
			LEFT JOIN atendimento.parlamentar par ON par.pesid = pes.pesid
			WHERE pev.pevstatus = 'A'
			--UTILIZAR UMA DAS TRÊS OPÇÕES ABAIXO Filtrar por Ano de Referência.
			--Estado
			".$where[0]."
			--Data Inicio.
			".$where[1]."
			--Parlamentar.
			".$where[2]."		
			--Partido.
			".$where[3]."
			ORDER BY 
					pes.pesnome
			";
	return $sql;
}

function monta_coluna(){
	
	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();
	
	$coluna = array();
	
	foreach ($colunas as $val){
		switch ($val) {
			case 'evtdataini':
				array_push( $coluna, 
								array(	"campo" 	=> "evtdataini",
							   		   	"label"		=> "Data",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;			
			case 'parlamentar':
				array_push( $coluna, 
								array(	"campo" 	=> "parlamentar",
							   		   	"label"		=> "Parlamentar",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'parpartido':
				array_push( $coluna, 
								array(	"campo" 	=> "parpartido",
							   		   	"label"		=> "Partido",
							   		   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'estuf':
				array_push( $coluna,
								array(	"campo"		=> "estuf",
										"label" 	=> "Unidade Federativa",
									   	"type"	  	=> "string"
								)
				);
				continue;
			break;			
			case 'participante_externo':
				array_push( $coluna, 
								array(	"campo" 	=> "participante_externo",
							   		   	"label"		=> "Participante Externo",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'participante_mec':
				array_push( $coluna, 
								array(	"campo" 	=> "participante_mec",
							   		   	"label"		=> "Participantes MEC",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'evtpauta':
				array_push( $coluna, 
								array(	"campo" 	=> "evtpauta",
							   		   	"label"		=> "Pauta",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'evtencaminhamentos':
				array_push( $coluna, 
								array(	"campo" 	=> "evtencaminhamentos",
							   		   	"label"		=> "Encaminhamento",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
		}
	}	
	//ver($coluna, d);	
	return $coluna;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("evtdataini","parlamentar","parpartido","estuf","participante_externo","participante_mec","evtpauta","evtencaminhamentos")	  
			);
			
	$count = 1;
	$i = 0;

	foreach ($agrupador as $val){
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}	
		
		switch ($val) {
			case 'estuf':
				array_push($agp['agrupador'], array("campo" => "estuf", "label" => "$var Estado") );				
		   		continue;
		    break;
		    case 'evtdataini':
				array_push($agp['agrupador'], array("campo" => "evtdataini", "label" => "$var Data"));					
		    	continue;			
		    break;
		    case 'parlamentar':
				array_push($agp['agrupador'], array("campo" => "parlamentar","label" => "$var Parlamentar"));					
		   		continue;			
		    break;
		    case 'parpartido':
				array_push($agp['agrupador'], array("campo" => "parpartido","label" => "$var Partido"));					
		   		continue;			
		    break;		    
		}
		$count++;
	}
	return $agp;
}
?>

</body>
</html>