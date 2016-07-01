<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
//monta_titulo( $titulo_modulo, '&nbsp;' );
cte_montaTitulo( $titulo_modulo, 'Neste item, agregam-se as planilhas dos quadros de <b>Sistematização dos Créditos de Pontuação</b> e <b>Sistematização Geral por Dimensão</b> da Parte III do Instrumento de Campo.' );

$itrid = cte_pegarItrid( $_SESSION['inuid'] );

$sql = sprintf("select distinct
					d.dimcod
					,d.dimdsc
					,ad.ardcod
					,ad.arddsc
					,i.indcod
					,c.ctrpontuacao
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
				$_SESSION['inuid']
			);
$resultado = $db->carregar($sql);

?>
			<?php if($resultado):?>
				<table border="0" width="95%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem">
					<?php foreach( $resultado as $key => $val ): ?>
					<?php if($key == 0 or $val["dimcod"] != $resultado[$key - 1]["dimcod"]):?>
					<tr> 
						<th colspan="6" class="class1"><?php echo $val["dimcod"] . '. ' . $val["dimdsc"];?></th>
					</tr>
					<?php endif;?>
					<?php if($key == 0 or $val["ardcod"] != $resultado[$key - 1]["ardcod"]):?>
					<tr> 
						<td></td>
						<th colspan="5" class="class2"><?php echo $val["ardcod"] . '. ' . $val["arddsc"];?></th>
					</tr>
					<tr> 
						<td></td>
						<td></td>
						<th width="25">indicador</th>
						<th width="25">pontua&ccedil;&atilde;o</th>
						<th>justificativa</th>
						<th>demanda potencial</th>
					</tr>
					<?php $cor = '#dfdfdf'; ?>
					<?php endif;?>
					<tr bgcolor="<?php echo $cor; ?>"> 
						<td></td>
						<td></td>
						<td align="center"><?php echo $val["indcod"];?>&nbsp;</td>
						<td align="center"><?php echo $val["ctrpontuacao"];?>&nbsp;</td>
						<td><?php echo $val["ptojustificativa"];?>&nbsp;</td>
						<td>
								<?php print(trim($val["ptodemandaestadual"]) == "")?"":"<p><b>Estadual:</b> ".trim($val["ptodemandaestadual"]) . "</p>" ;?>
								<?php print(trim($val["ptodemandamunicipal"]) == "")?"":"<p><b>Municipal:</b> ".trim($val["ptodemandamunicipal"]) . "</p>";?>
						</td>
					</tr>
					<?php if($cor == '#dfdfdf') $cor = '#ffffff'; else $cor = '#dfdfdf'; ?>
					<?php endforeach; ?>
				</table>
			<?php else: ?>
				<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
			<?php endif; ?>