<?php


header('content-type: text/html; charset=ISO-8859-1');

$inuId = $_GET['inuid'];

$today = date('Y-m-d');
if($inuId)
{
	$sqlRestricoes = 
		"SELECT 
			res.resdescricao,
			tpr.tprdescricao as tipo_restricao,
			CASE WHEN res.restemporestricao THEN
				to_char(resdatainicio, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_inicio,
			
			CASE WHEN res.restemporestricao THEN
				to_char(resdatafim, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_fim
		FROM
			par.restricaofnde res
		INNER JOIN
			par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
		INNER JOIN
			par.instrumentounidaderestricaofnde iur ON res.resid = iur.resid
		WHERE
			res.resstatus = 'A'
		AND
			iur.inuid = {$inuId}
		AND
			CASE WHEN res.restemporestricao THEN
				CASE
					WHEN '{$today}' between resdatainicio and resdatafim THEN
						true
					ELSE
						false
					END
			ELSE
				true
			END
		";
	
	$arrRestricoes = $db->carregar($sqlRestricoes);
	
	$sqlDesc = $db->pegaLinha("SELECT 
			m.mundescricao || ' - ' || m.estuf as descricaomun
		FROM 
			par.instrumentounidade iu
		INNER JOIN 
			territorios.municipio m on iu.muncod = m.muncod
		WHERE
			inuid = {$inuId}");
	$descricaoMun = $sqlDesc['descricaomun'];
	if( (count($arrRestricoes) && is_array($arrRestricoes)) ) 
	{
	?>
	
		<table cellspacing="0" cellpadding="3" border="0"  align="center" style="border-top: none; width:500px; " class="tabela">
			<thead>
				<tr bgcolor="#DCDCDC" >
					<td  colspan="4"> <label style="color:#000000;" class="TituloTela"> Restrições do município: <?=$descricaoMun?></label> </td>
				</tr>
			</thead>
			<tbody >
			
				<?php
				if( count($arrRestricoes) && is_array($arrRestricoes))
				{
					foreach($arrRestricoes as $kRes => $vRes )
					{
						echo "
							<tr >
								<td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
							</tr>
							";	
					}
				}
				?>
			</tbody>
			</table>
	<br>
<?php }
	}
	die();
?>