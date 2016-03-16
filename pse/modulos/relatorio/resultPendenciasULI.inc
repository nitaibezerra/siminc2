<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
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
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$bola = $dados;

if ( $bola ){
	foreach( $dados as $k => $dado ){
		if( $dado['pendencias'] == '' ){
			$bola[$k]['qtdc'] = 1;
		} else {
			$bola[$k]['qtdp'] = 1;
		}
	}
}
//ver ($dados, $k, $bola);


$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $bola); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);

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
	

	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND estado " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND f.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if($_POST['portaria'] == "2"){
		$where[2] = " AND pendencias = ''";		
	}
	if($_POST['portaria'] == "1"){
		$where[3] = " AND pendencias != ''";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[4] = " AND esf.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[5] = " AND f.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if( $port[0] && ( $port_campo_flag ||$port_campo_flag == '1' )){
		$where[] = " AND port.porano " . (( $port_campo_excludente == null || $port_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $port ) . "') ";		
	}
//	if ($_POST['analitico'] == "1"){
	$sql = "SELECT DISTINCT pendencias, porano, f.muncod as municipio, f.muncod, mundescricao, estado, entnome as escola, 0 as qtdc, 0 as qtdp, case when esf.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera FROM (
							SELECT  esc.*,
								esc.estuf as estado,
						
								( CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 1 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Gestão - Atores que participaram da execução da Agenda de Educação e Saúde 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 2 AND resp.espid = esc.espid ) = 0 
								THEN
									'Gestão - Parcerias para implementação do programa 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 3 AND resp.espid = esc.espid ) = 0 
								THEN
									'Gestão - Informações para planejamento 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid IN ('4','5') AND resp.espid = esc.espid ) = 0 
								THEN
									'Gestão - Frequência de comunicação 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 6 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Gestão - Agenda Educação e Saúde

'
								ELSE
								(CASE WHEN ( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('6') AND resp.espid = esc.espid ) = 0 AND ( SELECT resp.rulflagresposta FROM pse.rulresposta resp WHERE resp.pulid = 6 AND resp.espid = esc.espid ) = 's'
								THEN 
									'Gestão - Agenda Educação e Saúde

'
								ELSE
								''
								END)
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 8 AND resp.espid = esc.espid ) = 0 
								THEN
									'Componente I - Material enviado pelo MEC

'
								ELSE
								(CASE WHEN ( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('8') AND resp.espid = esc.espid ) = 0 AND ( SELECT resp.rulflagresposta FROM pse.rulresposta resp WHERE resp.pulid = 8 AND resp.espid = esc.espid ) = 's'
								THEN 
									'Componente I - Material enviado pelo MEC

'
								ELSE
								''
								END)
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 10 AND resp.espid = esc.espid ) = 0 
								THEN
									'Componente I - Plano de atendimento

'
								ELSE
								(CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 11 AND resp.espid = esc.espid ) = 0 AND ( SELECT resp.rulflagresposta FROM pse.rulresposta resp WHERE resp.pulid = 10 AND resp.espid = esc.espid ) = 's'
								THEN 
									'Componente I - Plano de atendimento

'
								ELSE
								''
								END)
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 17 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente I - Avaliação Clínica e Psicosocial

'
								ELSE
								(CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid in ('12','18') AND resp.espid = esc.espid ) = 0 AND ( SELECT resp.rulflagresposta FROM pse.rulresposta resp WHERE resp.pulid = 17 AND resp.espid = esc.espid ) = 's'
								THEN 
									'Componente I - Avaliação Clínica e Psicosocial

'
								ELSE
								''
								END)
								END ||
								CASE WHEN ( SELECT DISTINCT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid IN ('13','14') AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente I - Avaliação nutricional e Saúde Bucal 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 15 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente I - Estratégias planejadas para avaliaçãoclínica e psicosocial 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 16 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente I - Atores envolvidos na avaliação 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 19 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente II - Ações de segurança alimentar e Promoção da Alimentação Saudável 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 20 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente II - Estratégias realizadas par729a execução das ações de prevenção e promoção de saúde 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 21 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente II - Atores envolvidos nas ações de prevenção e promoção da saúde na escola 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 22 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente II - Articulação com ESF / UBS

'
								ELSE
								(CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 23 AND resp.espid = esc.espid ) = 0 AND (SELECT resp.rulflagresposta FROM pse.rulresposta resp WHERE resp.pulid = 22 AND resp.espid = esc.espid ) = 's'
								THEN 
									'Componente II - Articulação com ESF / UBS

'
								ELSE
								''
								END)
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 24 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Componente III - Ações / Atividades realizadas 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 25 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Reconhecimento de Território - Ações realizadas para intercâmbio 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 26 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Reconhecimento de Território - Participação da comunidade escolar e jovens 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 27 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Reconhecimento de Território - Registros das experiências locais 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 28 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Reconhecimento de Território - Ações do PSE inseridas no projeto político-Pedagogico 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 29 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Reconhecimento de Território - Encaminhamento dos estudantes aos especialistas 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 30 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Reconhecimento de Território - Esclarecimento aos pais 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('31') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 01) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('32') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 02)

 '
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('33') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 03) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('34') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 04) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('35') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 05) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('36') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 06) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('37') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 07) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('38') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 08)

 '
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 39 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Ambiente Escolar (Questao 09) 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 40 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Ambiente Escolar (Questao 10) 

'
								ELSE
								''
								END ||
								CASE WHEN ( SELECT count(resp.rulid) FROM pse.rulresposta resp WHERE resp.pulid = 41 AND resp.espid = esc.espid ) = 0 
								THEN 
									'Ambiente Escolar (Questao 11) 

'
								ELSE
								''
								END ||
								CASE WHEN (( SELECT COUNT (it.isuid) FROM pse.rulresposta resp INNER JOIN pse.isuitemselecionado it ON it.rulid = resp.rulid WHERE resp.pulid IN ('42') AND resp.espid = esc.espid) = 0) 
								THEN 
									'Ambiente Escolar (Questao 12) 

'
								ELSE
								''
								END
								) as pendencias 
						FROM
							pse.escolapse esc
				) as f
				INNER JOIN pse.estadomunicipiopse esf ON esf.muncod = f.muncod
				INNER JOIN pse.portariapse port ON port.pormunicipio = esf.muncod
				INNER JOIN entidade.entidade ent ON ent.entid = f.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = f.muncod
		WHERE
			1 = 1
			".$where[0]."
			".$where[1]."
			".$where[2]."
			".$where[3]."
			".$where[4]."
			".$where[5]."
			".$where[6]."
		ORDER BY
			estado, mundescricao";
	
//	ver ($sql, d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "qtdc", "qtdp" )	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "- Unidades Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "- Unidades Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "- Unidades Estaduais e Municipais<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			if ($_POST['portaria'] == "1"){
				$var = "PSE - Programa Saúde na Escola - Relação de Unidades Locais Integradas com pendência no PSE$vari<br>";
			} else if ($_POST['portaria'] == "2"){
				$var = "PSE - Programa Saúde na Escola - Relação de Unidades Locais Integradas com cadastro completo no PSE$vari<br>";
			}
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "$var Município")										
									   				);					
		    	continue;
		    break;		    	
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											 		"label" => "$var Escola")										
									   				);					
		   		continue;			
		    break;
		    case 'port':
				array_push($agp['agrupador'], array(
													"campo" => "porano",
											  		"label" => "$var Portaria")										
									   				);					
		    	continue;
		    break;
		}
		$count++;
	endforeach;
	if($_POST['portaria'] == "1" & $_POST['analitico'] == '1'){
				array_push($agp['agrupador'], array(
													"campo" => "pendencias",
											 		"label" => "$var Pendencias")										
									   				);					
	}
	return $agp;
}

function monta_coluna(){
if ($_POST['analitico'] == "2"){
$coluna    = array(
						array(
							  "campo" => "qtdc",
					   		  "label" => "Escolas com cadastro Completo",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "qtdp",
					   		  "label" => "Escolas com Pendencias",
							  "type"  => "numeric"
						)
					);
}
	return $coluna;		  	
	
}
?>
