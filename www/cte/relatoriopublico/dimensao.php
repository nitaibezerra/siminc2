<?php
$db->cria_aba( $abacod_tela, $url, '' );
$sql = sprintf("select 
					ins.itrid
					,d.dimid
					,d.dimcod || '. ' ||d.dimdsc as dimdsc
					,c.ctrpontuacao 
					,count ( c.ctrpontuacao ) as qtpontos
					
				from cte.instrumento ins
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
				group by ins.itrid, d.dimcod, d.dimdsc, c.ctrpontuacao , d.dimid 				
				order by d.dimcod , dimdsc
				" 
				,$inuid
				
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
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>

<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SINTESE DA DIMENSÃO</b></td>
	</tr>
	<tr>
		<td>
			<?php if( isset($relatorio)): ?>
				<table style=" margin-top:5px;  margin-bottom:5px;" border="0" width="98%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem">
					<thead>
					<tr style="border-bottom:4px solid black;">
						<td bgcolor="#acbc73" align="center" rowspan="2">Dimensão</td>
						<td bgcolor="#acbc73" align="center" colspan="5">Pontuação</td>
					</tr>
					<tr>
						<td bgcolor="#ccd7a4" align="center"><b>4</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>3</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>2</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>1</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>n/a</b></td>
					</tr>
					</thead>
					<?php foreach($relatorio as $keyr => $valr ): ?>
						<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
							<td><?php echo $valr["dimdsc"]; ?></td>
							<td align="center"><?php echo (int)$valr["4"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["3"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["2"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["1"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["0"]; ?>&nbsp;</td>
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
							<td bgcolor="#ccd7a4" align="right"><b>Total:</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t4"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t3"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t2"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t1"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t0"]; ?>&nbsp;</b></td>
						</tr>
						<tfoot>
						<tr >
								<td colspan="6" align="right" bgcolor="#ccd7a4">*n/a :  Não se Aplica.</td>
						</tr>
						<tfoot>
				</table>
			<?php else: ?>
				<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
			<?php endif; ?>
		</td>
	</tr>
</table>	