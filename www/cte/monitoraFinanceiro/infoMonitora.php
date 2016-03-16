<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();


$hmcid = $_REQUEST['hmcid'];
$ano = $_REQUEST['ano'];
$total = "0%";
$analisado = "0%";
$status = "Não iniciado.";
$resto = "0%";
$executadoNoMomento = "0%";

if($ano == NULL){
	$ano = 0;
}
if($hmcid == NULL){
	$hmcid = 0;
}else{
	$sql = "SELECT hmcstatus
				FROM cte.historicomonitoramentoconvenio
				WHERE hmcid = ".$hmcid;
	
	$dados = $db->carregar($sql);
	
	if(is_array($dados)){
		foreach($dados as $dado){
			if($dado['hmcstatus'] == "I"){
				$status = "Em Andamento";
			}else{
				$status = "Finalizado.";
			}
		}
	}

	$sql = "SELECT 	SUM(total) AS total, 
						SUM(analisado) AS analisado
				FROM(	SELECT  p.prsvalorconvenio as total,
							0 AS analisado,
							p.prsnumconvsape
					FROM cte.projetosape p
					INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = p.prsid
					WHERE  hc.hmcid = ".$hmcid."
					UNION ALL
					SELECT  0 AS total,
							sum(hmsvalortotalempenhado) AS analisado,
							p.prsnumconvsape
					FROM cte.historicomonitoramentoconvenio hc
					INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
					INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
					INNER JOIN cte.projetosape p ON  hc.prsid = p.prsid
					WHERE hmsvalortotalempenhado IS NOT NULL 
					AND hc.hmcid <= ".$hmcid."
					AND p.inuid = ".$_SESSION['inuid']."
					GROUP BY p.prsnumconvsape
				) AS valores GROUP BY prsnumconvsape";
	
	
	//dbg($sql,1);
	

	
	$valores = $db->pegaLinha($sql);
	
	if(is_array($valores)){
		$total 		= number_format($valores['total'],2,",",".");
		$analisado 	= number_format($valores['analisado'],2,",",".");
		$resto = $valores['total'] - $valores['analisado'];
		$resto = number_format($resto,2,",",".");
	}
	
	$sql = "SELECT  sum(hmsvalortotalpago) as atemomento
					FROM cte.historicomonitoramentoconvenio hc
					INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
					INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
					WHERE hmsvalortotalpago IS NOT NULL AND hc.hmcid = ".$hmcid;
	
	$executadoNoMomento = $db->pegaUm($sql);
	$executadoNoMomento = number_format($executadoNoMomento,2,",",".");
	
}
header( 'Content-type: text/html; charset=iso-8859-1' );
?>
<table>
	<tr>
		<td>Status: </td>
		<td><?=$status; ?></td>
	</tr>
	<tr>
		<td>Total conveniado: </td>
		<td><?=$total; ?></td>
	</tr>
	<tr>
		<td>Total executado neste monitoramento: </td>
		<td><?=$executadoNoMomento; ?></td>
	</tr>
	<tr>
		<td>Total executado acumulado: </td>
		<td><?=$analisado; ?></td>
	</tr>
	<tr>
		<td>Total à ser executado: </td>
		<td><?=$resto; ?></td>
	</tr>
</table>