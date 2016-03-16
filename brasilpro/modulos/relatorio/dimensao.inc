<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, 'A partir da sistematização anterior, a totalização da pontuação por dimensão ficou assim distribuida:' );

$itrid = cte_pegarItrid( $_SESSION['inuid'] );

$sql = sprintf("select distinct
					ins.itrid
					,d.dimid
					,d.dimcod || '. ' ||d.dimdsc as dimdsc
					,c.ctrpontuacao 
					,count ( c.ctrpontuacao ) as qtpontos
					
				from cte.instrumento ins
					inner join cte.instrumentounidade iu on iu.itrid = ins.itrid and iu.inuid = %d
					inner join cte.dimensao d on d.itrid = ins.itrid
					inner join cte.areadimensao ad on d.dimid = ad.dimid
					inner join cte.indicador i on i.ardid = ad.ardid
					inner join cte.criterio c on c.indid = i.indid
					left join cte.pontuacao pt on pt.crtid = c.crtid and pt.indid = i.indid and pt.inuid = %d
				where
					pt.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
				group by ins.itrid, d.dimcod, d.dimdsc, c.ctrpontuacao , d.dimid" 
				,$_SESSION['inuid']
				,$_SESSION['inuid']
				);

if( $resultado = $db->carregar($sql) )
{
	//percorrendo o resultado e criando um array por dimensao 
	foreach($resultado as $key => $val )
	{
		$cor = $icone ? '#959595' : '#133368';
		$relatorio[$val["dimid"]]["dimdsc"] = $val["dimdsc"];
		switch($val["ctrpontuacao"])
		{
			case "0":
				$relatorio[$val["dimid"]]["0"] = $val["qtpontos"];
				break;
			case "1":
				$relatorio[$val["dimid"]]["1"] = $val["qtpontos"];
				break;
			case "2":
				$relatorio[$val["dimid"]]["2"] = $val["qtpontos"];
				break;
			case "3":
				$relatorio[$val["dimid"]]["3"] = $val["qtpontos"];
				break;
			case "4":
				$relatorio[$val["dimid"]]["4"] = $val["qtpontos"];
				break;
		}
	}
}

$total["t0"] = 0;
$total["t1"] = 0;
$total["t2"] = 0;
$total["t3"] = 0;
$total["t4"] = 0;
$cor = '#e7e7e7';
?>
			<?php if( isset($relatorio)): ?>
				<table border="0" width="95%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem">
					<thead>
					<tr style="border-bottom:4px solid black;">
						<th rowspan="2">Dimensão</th>
						<th colspan="5">Pontuação</th>
					</tr>
					<tr>
						<th>4</th>
						<th>3</th>
						<th>2</th>
						<th>1</th>
						<th>n/a</th>
					</tr>
					</thead>
					<?php foreach($relatorio as $keyr => $valr ): ?>
						<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
							<td><?php echo $valr["dimdsc"]; ?></td>
							<td align="right"><?php echo (int)$valr["4"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["3"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["2"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["1"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["0"]; ?>&nbsp;</td>
						</tr>
						<?php 
							$total["t0"] += (int)$valr["0"];
							$total["t1"] += (int)$valr["1"];
							$total["t2"] += (int)$valr["2"];
							$total["t3"] += (int)$valr["3"];
							$total["t4"] += (int)$valr["4"];
							if($cor == '#e7e7e7') $cor = '#ffffff'; else $cor = '#e7e7e7';
						?>
					<?php endforeach; ?>
						<tr>
							<td align="right"><b>Total:</b></td>
							<td align="right"><b><?php echo $total["t4"]; ?>&nbsp;</b></td>
							<td align="right"><b><?php echo $total["t3"]; ?>&nbsp;</b></td>
							<td align="right"><b><?php echo $total["t2"]; ?>&nbsp;</b></td>
							<td align="right"><b><?php echo $total["t1"]; ?>&nbsp;</b></td>
							<td align="right"><b><?php echo $total["t0"]; ?>&nbsp;</b></td>
						</tr>
						<tfoot>
						<tr >
								<td colspan="6" align="right">*n/a :  Não se Aplica.</td>
						</tr>
						<tfoot>
				</table>
			<?php else: ?>
				<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
			<?php endif; ?>
