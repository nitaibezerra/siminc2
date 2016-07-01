<?php
$db->cria_aba( $abacod_tela, $url, '' );
//monta_titulo( $titulo_modulo, '&nbsp;' );
$sql = sprintf("select distinct
					d.dimcod
					,d.dimdsc
					,ad.ardcod
					,ad.arddsc
					,i.indcod
					,c.ctrpontuacao
					,c.crtdsc
					,p.ptojustificativa
					,p.ptodemandamunicipal
					,p.ptodemandaestadual
				from 
					cte.instrumento ins
					inner join cte.dimensao d on d.itrid = ins.itrid
					inner join cte.areadimensao ad on d.dimid = ad.dimid
					inner join cte.indicador i on i.ardid = ad.ardid
					inner join cte.criterio c on c.indid = i.indid
					inner join cte.pontuacao p on p.crtid = c.crtid
				where 
					p.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
					and p.inuid = %d
				" , 
				$inuid
			);
$resultado = $db->carregar($sql);

?>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>

<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SÍNTESE DO INDICADOR</b></td>
	</tr>
	<tr>
		<td>
<?php if($resultado):?>
	<table border="0" width="98%" cellspacing="0" cellpadding="4" align="center" bgcolor="#999999" class="listagem">
		<?php foreach( $resultado as $key => $val ): ?>
		<?php if($key == 0 or $val["dimcod"] != $resultado[$key - 1]["dimcod"]):?>
		<tr> 
			<th colspan="6" class="class1" style=" background-color:#7e8e47;" ><?php echo $val["dimcod"] . '. ' . $val["dimdsc"];?></th>
		</tr>
		<?php endif;?>
		<?php if($key == 0 or $val["ardcod"] != $resultado[$key - 1]["ardcod"]):?>
		<tr> 
			<td bgcolor="#acbc73"></td>
			<th colspan="5" class="class2" style=" background-color:#acbc73;"><?php echo $val["ardcod"] . '. ' . $val["arddsc"];?></th>
		</tr>
		<tr> 
			<td bgcolor="#ccd7a4"></td>
			<td bgcolor="#ccd7a4"></td>
			<td width="25" bgcolor="#ccd7a4">Indicador</td>
			<td width="25" bgcolor="#ccd7a4">Pontua&ccedil;&atilde;o</td>
			<td bgcolor="#ccd7a4" align="center">Critério</td>
		</tr>
		<?php $cor = '#dfdfdf'; ?>
		<?php endif;?>
		<tr bgcolor="<?php echo $cor; ?>"> 
			<td></td>
			<td></td>
			<td align="center"><?php echo $val["indcod"];?>&nbsp;</td>
			<td align="center"><?php echo $val["ctrpontuacao"];?>&nbsp;</td>
			<td><?php echo $val["crtdsc"];?>&nbsp;</td>
		</tr>
		<?php if($cor == '#dfdfdf') $cor = '#ffffff'; else $cor = '#dfdfdf'; ?>
		<?php endforeach; ?>
	</table>
<?php else: ?>
	<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
<?php endif; ?>
		</td>
	</tr>
</table>