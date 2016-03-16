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
	
	
	if ($tipo){
		$where[] = "e.tpcid IN($tipo)";
	}
	// Regiao
	if ($f_regiao[0] && $regiao_campo_flag){
		$where[] = " re.regcod ".(!$f_regiao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_regiao)."') ";
	}
	// Estado
	if ($f_estuf[0] && $estuf_campo_flag){
		$where[] = " m.estuf ".(!$f_estuf_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_estuf)."') ";
	}
	// Município
	if ($f_municipio[0] && $municipio_campo_flag){
		$where[] = " m.muncod ".(!$f_municipio_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_municipio)."') ";
	}
	// Dependência administrativa
	if(isset($_POST["tpcid"]) && $_POST["tpcid"] != "") {
		$where[] = " e.tpcid IN (".$_POST["tpcid"].") ";
		//array_push($where, "esd.esdid in (".implode(",",$_REQUEST["esdid"]).")");
	}
	// Questoes	
	if (isset($_POST["qapid"]) && $_POST["qapid"] != ""){
		$where[] =  " qap.qapid IN ('".implode("','",$_POST["qapid"])."')"; 
	}	
	// Codigo INEP
	if ($f_inep[0] && $inep_campo_flag){
		$where[] = " e.entcodent ".(!$f_inep_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_inep)."') ";
	}
	
	if(!$agrupador[0]) {
		$agrupador = array();
	}
 
	$count = count($_POST['qapid']);

	
	$sql = "SELECT 
					inep,
					nome,
					regiao,
					estado,
					municipio,
					--qapid,
					--qapcodigo,
					tipo,
					SUM(Q_1_1) AS Q_1_1,
					SUM(Q_1_2) AS Q_1_2,
					SUM(Q_1_3) AS Q_1_3,
					SUM(Q_1_4) AS Q_1_4,
					SUM(Q_1_5) AS Q_1_5,
					SUM(Q_2_1) AS Q_2_1,
					SUM(Q_2_2) AS Q_2_2,
					SUM(Q_2_3) AS Q_2_3,
					SUM(Q_2_4) AS Q_2_4,
					SUM(Q_2_5) AS Q_2_5,
					SUM(Q_3_1) AS Q_3_1,
					SUM(Q_3_2) AS Q_3_2,
					SUM(Q_3_3) AS Q_3_3,
					SUM(Q_3_4) AS Q_3_4,
					SUM(Q_3_5) AS Q_3_5,
					SUM(Q_3_6) AS Q_3_6,
					SUM(Q_4_1) AS Q_4_1,
					SUM(Q_4_2) AS Q_4_2,
					SUM(Q_4_3) AS Q_4_3,
					SUM(Q_4_4) AS Q_4_4,
					SUM(Q_4_5) AS Q_4_5,
					SUM(Q_4_6) AS Q_4_6,
					SUM(Q_5_1) AS Q_5_1,
					SUM(Q_5_2) AS Q_5_2,
					SUM(Q_5_3) AS Q_5_3,
					SUM(Q_5_4) AS Q_5_4,
					SUM(Q_5_5) AS Q_5_5,
					SUM(Q_5_6) AS Q_5_6,
					SUM(Q_5_7) AS Q_5_7,
					SUM(Q_6_1) AS Q_6_1,
					SUM(Q_6_2) AS Q_6_2,
					SUM(Q_6_3) AS Q_6_3,
					SUM(Q_6_4) AS Q_6_4,
					SUM(Q_6_5) AS Q_6_5,
					SUM(Q_6_6) AS Q_6_6,
					SUM(Q_7_1) AS Q_7_1,
					SUM(Q_7_2) AS Q_7_2,
					SUM(Q_7_3) AS Q_7_3,
					SUM(Q_7_4) AS Q_7_4,
					SUM(Q_7_5) AS Q_7_5,
					SUM(Q_7_6) AS Q_7_6,
					SUM(Q_7_7) AS Q_7_7,
					SUM(Q_7_8) AS Q_7_8,
					SUM(Q_7_9) AS Q_7_9,
					SUM(Q_7_10) AS Q_7_10,
					SUM(Q_7_11) AS Q_7_11,
					SUM(Q_8_1) AS Q_8_1,
					SUM(Q_8_2) AS Q_8_2,
					SUM(Q_8_3) AS Q_8_3,
					SUM(Q_8_4) AS Q_8_4,
					SUM(Q_8_5) AS Q_8_5,
					SUM(Q_8_6) AS Q_8_6,
					SUM(Q_8_7) AS Q_8_7,
					SUM(Q_8_8) AS Q_8_8,
					SUM(Q_8_9) AS Q_8_9,					
					SUM (total) as total,
					maximo,
					(((((total/maximo::numeric)*100)) :: numeric(12,0))|| ' % ' ) as  perc
			FROM (
				SELECT
					DISTINCT
					e.entcodent as inep,
					'' || p.pdenome || '' AS nome,
					re.regdescricao as regiao,
					m.estuf AS estado,
					m.mundescricao AS municipio,
					qap.qapid,
					qap.qapcodigo,
					CASE e.tpcid WHEN 1 THEN 'Escolas Estaduais' ELSE 'Escolas Municipais' END AS tipo,
					CASE WHEN qapcodigo = '1.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_1,
					CASE WHEN qapcodigo = '1.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_2,
					CASE WHEN qapcodigo = '1.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_3,
					CASE WHEN qapcodigo = '1.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_4,
					CASE WHEN qapcodigo = '1.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_1_5,
					CASE WHEN qapcodigo = '2.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_1,
					CASE WHEN qapcodigo = '2.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_2,
					CASE WHEN qapcodigo = '2.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_3,
					CASE WHEN qapcodigo = '2.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_4,
					CASE WHEN qapcodigo = '2.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_2_5,
					CASE WHEN qapcodigo = '3.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_1,
					CASE WHEN qapcodigo = '3.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_2,
					CASE WHEN qapcodigo = '3.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_3,
					CASE WHEN qapcodigo = '3.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_4,
					CASE WHEN qapcodigo = '3.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_3_5,
					CASE WHEN qapcodigo = '3.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_3_6,
					CASE WHEN qapcodigo = '4.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_1,
					CASE WHEN qapcodigo = '4.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_2,
					CASE WHEN qapcodigo = '4.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_3,
					CASE WHEN qapcodigo = '4.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_4,
					CASE WHEN qapcodigo = '4.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_4_5,
					CASE WHEN qapcodigo = '4.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_4_6,
					CASE WHEN qapcodigo = '5.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_1,
					CASE WHEN qapcodigo = '5.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_2,
					CASE WHEN qapcodigo = '5.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_3,
					CASE WHEN qapcodigo = '5.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_4,
					CASE WHEN qapcodigo = '5.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_5_5,
					CASE WHEN qapcodigo = '5.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_5_6,
					CASE WHEN qapcodigo = '5.7'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_5_7,
					CASE WHEN qapcodigo = '6.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_1,
					CASE WHEN qapcodigo = '6.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_2,
					CASE WHEN qapcodigo = '6.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_3,
					CASE WHEN qapcodigo = '6.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_4,
					CASE WHEN qapcodigo = '6.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_6_5,
					CASE WHEN qapcodigo = '6.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_6_6,
					CASE WHEN qapcodigo = '7.1'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_1,
					CASE WHEN qapcodigo = '7.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_2,
					CASE WHEN qapcodigo = '7.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_3,
					CASE WHEN qapcodigo = '7.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_4,
					CASE WHEN qapcodigo = '7.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_5,
					CASE WHEN qapcodigo = '7.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_6,
					CASE WHEN qapcodigo = '7.7'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_7,
					CASE WHEN qapcodigo = '7.8'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_8,
					CASE WHEN qapcodigo = '7.9'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_9,
					CASE WHEN qapcodigo = '7.10'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_10,
					CASE WHEN qapcodigo = '7.11'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_11,
					CASE WHEN qapcodigo = '8.1'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_1,
					CASE WHEN qapcodigo = '8.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_2,
					CASE WHEN qapcodigo = '8.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_3,
					CASE WHEN qapcodigo = '8.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_4,
					CASE WHEN qapcodigo = '8.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_5,
					CASE WHEN qapcodigo = '8.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_6,
					CASE WHEN qapcodigo = '8.7'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_7,
					CASE WHEN qapcodigo = '8.8'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_8,
					CASE WHEN qapcodigo = '8.9'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_9,
					ea.eavvalor as total,
					".$count." * 5 AS maximo
										
				FROM
					entidade.entidade e
				INNER JOIN 
					pdeescola.pdeescola p ON p.entid = e.entid	
				INNER JOIN 
					pdeescola.entpdeideb ei ON ei.epientcodent = p.pdecodinep
				INNER JOIN 
					entidade.endereco e1 ON e1.entid = e.entid
				INNER JOIN 
					territorios.municipio m ON m.muncod = e1.muncod
				INNER JOIN 
					territorios.estado est ON est.estuf = m.estuf
				INNER JOIN 
					territorios.regiao re ON re.regcod = est.regcod				
				INNER JOIN 
					pdeescola.avaliacaoplano ap ON ap.pdeid = p.pdeid
				INNER JOIN 
					pdeescola.questaoavaliacaoplano qap ON qap.qapid = ap.qapid AND ap.aplid IS NOT NULL AND qap.qapidpai IS NOT NULL
				INNER JOIN 
					pdeescola.escalaavaliacao ea ON ea.eavid = ap.eavid 
				".$where = (!empty($where) ? "WHERE ".implode(" AND ", $where) : "")."
				ORDER BY 
					inep, qapcodigo
				) AS f
			GROUP BY
				inep,
				nome,
				regiao,
				estado,
				municipio,
				tipo,
				f.total,
				f.maximo
				";
//ver($sql,d);
	return $sql;
}

// Função para montar o array com os agrupadores
function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
							   				"q_1_1",
											"q_1_2",
											"q_1_3",
											"q_1_4",
											"q_1_5",
											"q_2_1",
											"q_2_2",
											"q_2_3",
											"q_2_4",
											"q_2_5",
											"q_3_1",
											"q_3_2",
											"q_3_3",
											"q_3_4",
											"q_3_5",
											"q_3_6",
											"q_4_1",
											"q_4_2",
											"q_4_3",
											"q_4_4",
											"q_4_5",
											"q_4_6",
											"q_5_1",
											"q_5_2",
											"q_5_3",
											"q_5_4",
											"q_5_5",
											"q_5_6",
											"q_5_7",
											"q_6_1",
											"q_6_2",
											"q_6_3",
											"q_6_4",
											"q_6_5",
											"q_6_6",
											"q_7_1",
											"q_7_2",
											"q_7_3",
											"q_7_4",
											"q_7_5",
											"q_7_6",
											"q_7_7",
											"q_7_8",
											"q_7_9",
											"q_7_10",
											"q_7_11",
											"q_8_1",
											"q_8_2",
											"q_8_3",
											"q_8_4",
											"q_8_5",
											"q_8_6",
											"q_8_7",
											"q_8_8",
											"q_8_9",
											"total",
											"maximo",
											"perc"
	   		
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
	
	$arDados = $_POST['qapid'];
	
	$coluna = array();
	foreach ($arDados as $dados){

		$sql = "SELECT  
				'q_' || replace(qapcodigo, '.', '_') as campo,
				'Questão ' || qapcodigo as label
			FROM 
				pdeescola.questaoavaliacaoplano 
			WHERE 
				qapid = " .  $dados;

		$dado = $db->pegaLinha($sql);
		array_push($coluna, array(
					  				"campo" 	 => $dado['campo'],
			   		  				"label" 	 => $dado['label'],
					  				"type"	 	=> "numeric"					
								  ));
	}
		array_push($coluna,array(
									"campo" 	 => "total",
						   		  	"label" 	 => "Totais",
								  	"type"	 	=> "numeric"
								  ));									
		array_push($coluna,array(
								  	"campo" 	 => "maximo",
						   		  	"label" 	 => "Total Máximo",
								  	"type"	 	=> "numeric"
								  )	);
		array_push($coluna,array(
								  	"campo" 	 => "perc",
						   		  	"label" 	 => "%",
								  	"type"	 	=> "string"
								  )	);
					  	
	return $coluna;			  	
}
	

	$totalMaximo = $j * 5;
	$totalMaximo = number_format($totalMaximo,0,',','.')
//	$percMaximo = $j / $totalMaximo;
?>
<br></br>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
		<tr>
			<td class="subtituloesquerda" align="left">&nbsp;&nbsp;&nbsp;&nbsp;</td>
			<?php foreach( $_POST['qapid'] as $qapid ){
				//select que monta colunas
				$sql = "SELECT  
							'q_' || replace(qapcodigo, '.', '_') as campo,
							'Questão ' || qapcodigo as label
						FROM 
							pdeescola.questaoavaliacaoplano 
						WHERE 
							qapid = " .  $qapid;
						$qap = $db->pegaLinha( $sql );
			?>
				<td class="subtituloesquerda" ><?=$qap['label'] ?></td>
			<? } ?>	

		</tr>
		<tr>
			<td class="subtituloesquerda" align="left">&nbsp;&nbsp;&nbsp;&nbsp;<b>Total Máximo: <?php echo $totalMaximo?></b> </td>
				<?php foreach( $_POST['qapid'] as $qapid ){?>
					<td class="subtituloesquerda" ></td>
				<?php }?>

		</tr>
		<tr>
			<td class="subtituloesquerda" align="left">&nbsp;&nbsp;&nbsp;&nbsp;<b> % : </b></td>
				<?php foreach( $_POST['qapid'] as $qapid ){?>
					<td class="subtituloesquerda" ></td>
				<?php }?>

		</tr>
	</table>
<br>
	<table border="1" align="center" cellpadding="0" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; ">
		<?php   
		$legenda = $db->carregar("SELECT eavid,eavdescricao FROM pdeescola.escalaavaliacao ORDER BY eavid");?>
			<tr>
				<td align="middle" width="100"><b>Valores</b></td>
				<td align="middle" width="210" ><b>Respostas</b></td>
			</tr>
		<?php for($i=0; $i<count($legenda); $i++){?>	
			<tr>
				<td align="middle" width="100">
				<?=$legenda[$i]['eavid'];?>
				</td>
				<td width="210" >
				<?=substr($legenda[$i]['eavdescricao'],4);?>
				</td>
			</tr>
		<?php } ?>
	</table>
	
	
<script>
//funcao para recuperar total coluna (exibido pelo componente Relatorio) e fazer calculos porcentagem.
	$(document).ready(function (){
		var totMaximo  = new Number(<?php echo $totalMaximo;?>);
		var arTotalRel = new Array();
		
		$('table:first tr:last td').not(':first').not(':last').each(function (){//varrendo td da segunda tabela, última tr. Ignorando primeira e última tr.
			arTotalRel.push(new Number($(this).html()));//jogando dentro do (array)arTotalRel os valores de cada td da última tr.
		});

		var calc  = new Number();
		for(var i=0; i < arTotalRel.length; i++){
			indTD = i + 1;//indice da td, para não começar por 0

			txtCalc = '(' + arTotalRel[i] + ' / '  + totMaximo + ') * 100'; 
			$('table:eq(2) tr:eq(1) td:eq(' + indTD + ')').html(txtCalc);

			calc = ((arTotalRel[i] / totMaximo) * 100).toFixed(2);

			if (calc < 60){
				color = '#FF0000';
			}else{
				color = '#0066CC';
			}

			$('table:eq(2) tr:last td:eq(' + indTD + ')').html(calc + '%')
														 .css('color', color);
			
		}
	});
</script>	
 </body>
 </html>