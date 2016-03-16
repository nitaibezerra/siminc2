<?php

$totalPeri1			= 0;
$totalPeri2			= 0;
$totalPeri3			= 0;
$totalPeri4			= 0;

$totalPeriodo1 		= 0;
$totalPeriodo2 		= 0;
$totalPeriodo3 		= 0;
$totalPeriodo4 		= 0;

$indicePeriodo1 	= 0;
$indicePeriodo2 	= 0;
$indicePeriodo3 	= 0;
$indicePeriodo4 	= 0;

$contaSubMonitora 	= 0;
$mediaIndice 		= 0;
$porcentagemTotal 	= 0;

$sql1 = "	SELECT DISTINCT saim.sbaid, mnt.mntid, saim.sbadsc
			FROM cte.indicador i 				
				INNER JOIN cte.pontuacao p on p.indid = i.indid
				INNER JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid
				INNER JOIN cte.subacaoparecertecnico spt on saim.sbaid = spt.sbaid
				INNER JOIN cte.proposicaosubacao ps ON ps.ppsid = saim.ppsid
					INNER JOIN cte.monitoramentosubacao AS mnt ON mnt.sbaid = saim.sbaid
					LEFT JOIN cte.qtdfisicoano qfa ON saim.sbaid = qfa.sbaid					
			WHERE mnt.mntstatus = 'A' 	
				AND saim.frmid IN ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
				AND spt.ssuid IN (3) --tem subação com valor 6 
				AND ppsindcobuni = false	
				AND p.inuid IN (".$_REQUEST['inuid'].") 	
				AND p.ptostatus = 'A'	
				AND aim.acilocalizador = 'M'
				AND spt.sptano <= 2009";

$registros = $db->carregar($sql1);
$totalRegistros = count($registros);

if($totalRegistros > 0)
{	
	foreach((array) $registros as $registro){
	
		$mntid = (int) $registro["mntid"];		
		$sql2 = "select e.perid, perdsc, es.estid, es.estdsc, es.estcor 
				from cte.execucaosubacao e
					inner join cte.estadosubacao es on e.estid = es.estid
					inner join cte.periodoreferencia p on e.perid = p.perid
				where e.mntid = '".$mntid."'
				and exeatual = true
				order by perid, perdtiniciocad";
		
	
		$resultado = $db->carregar( $sql2 );
		$totalResultado = count($resultado);		
		
		if($totalResultado > 0)
		{
			foreach((array) $resultado as $estadoSub)
			{						
				if($estadoSub['perid'] == 1) {
					$totalPeri1 += count($estadoSub['perid']);							
				} elseif ($estadoSub['perid'] == 2) {
					$totalPeri2 += count($estadoSub['perid']);
				} elseif ($estadoSub['perid'] == 3) {
					$totalPeri3 += count($estadoSub['perid']);
				} elseif ($estadoSub['perid'] == 4) {
					$totalPeri4 += count($estadoSub['perid']);
				} 					
			}			
		}				
	}
}
$totalPeriodo1 = $totalPeri1 ? $totalPeri1 : 0;
$totalPeriodo2 = $totalPeri2 ? $totalPeri2 : 0;
$totalPeriodo3 = $totalPeri3 ? $totalPeri3 : 0;
$totalPeriodo4 = $totalPeri4 ? $totalPeri4 : 0;
if( $totalRegistros > 0 ){
	$indicePeriodo1 = round(($totalPeriodo1/$totalRegistros)*100, 2);
	$indicePeriodo2 = round(($totalPeriodo2/$totalRegistros)*100, 2);
	$indicePeriodo3 = round(($totalPeriodo3/$totalRegistros)*100, 2);
	$indicePeriodo4 = round(($totalPeriodo4/$totalRegistros)*100, 2);
	
	$mediaIndice = ($indicePeriodo1 + $indicePeriodo2 + $indicePeriodo3 + $indicePeriodo4)/4;	
}

$cor 	= "#cococo";
$cor1 	= "black";
$porcentagemTotal = 100;

$barra = '	<div class="barra1">
				<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
					<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.round($mediaIndice, 2).'<span style="color:'.$cor1.'" >%</span></div>
				</div>
			<img class="imgBarra" style="width: '.$mediaIndice.'%; height: 10px" src="../imagens/cor1.gif"/>
			</div>
			';
?>