<?php
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$percCampo = $_POST['percCampo'] ? $_POST['percCampo'] : '50';
$percPobreza = $_POST['percPobreza'] ? $_POST['percPobreza'] : '12'; 
$percAnalf = $_POST['percAnalf'] ? $_POST['percAnalf'] : '25'; 
$percEducacao = $_POST['percEducacao'] ?$_POST['percEducacao'] : '30'; 

$titulo_modulo = "Relatório de Convênios";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_curso.js"></script>
<script type="text/javascript">
$(document).ready(function() {

	$('.perc').keyup(function(){
		if(parseFloat($(this).val())<0){
			$(this).val(0)
		}
		if(parseFloat($(this).val())>100){
			$(this).val(100)
		}
	});

	$('.perc').blur(function(){
		if(parseFloat($(this).val())<0){
			$(this).val(0)
		}
		if(parseFloat($(this).val())>100){
			$(this).val(100)
		}
	});

	$('.pesquisar').click(function(){
		$('#formulario').submit();
	});
	
});
</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="pesquisa" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="15%">Agrupadores</td>
			<td>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
					<tr>
						<td width="5%">
							<center>
								<input type="checkbox" name="campo" checked value="S"/>
							</center>
						</td>
						<td width="8%">
							<center>
								<?=campo_texto('percCampo', 'N', 'S', '', '3', '6','[#],##','','','','','class="perc"','',$percCampo); ?>&nbsp;%
							</center>
						</td>
						<td>
							<b>População do campo.</b>
						</td>
					</tr>
					<tr>
						<td>
							<center>
								<input type="checkbox" name="pobreza" checked value="S"/>
							</center>
						</td>
						<td>
							<center>
								<?=campo_texto('percPobreza', 'N', 'S', '', '3', '6','[#],##','','','','','class="perc"','',$percPobreza); ?>&nbsp;%
							</center>
						</td>
						<td>
							<b>População em situação de pobreza extrema.</b>
						</td>
					</tr>
					<tr>
						<td>
							<center>
								<input type="checkbox" name="analf" checked value="S"/>
							</center>
						</td>
						<td>
							<center>
								<?=campo_texto('percAnalf', 'N', 'S', '', '3', '6','[#],##','','','','','class="perc"','',$percAnalf); ?>&nbsp;%
							</center>
						</td>
						<td>
							<b>População com 15 anos ou mais não alfabetizada.</b>
						</td>
					</tr>
					<tr>
						<td>
							<center>
								<input type="checkbox" name="educacao" checked value="S"/>
							</center>
						</td>
						<td>
							<center>
								<?=campo_texto('percEducacao', 'N', 'S', '', '3', '6','[#],##','','','','','class="perc"','',$percEducacao); ?>&nbsp;%
							</center>
						</td>
						<td>
							<b>Docentes que atuam no campo sem educação superior.</b>
						</td>
					</tr>
					<tr>
						<td>
							<center>
								<input type="checkbox" name="cidadania" checked value="S"/>
							</center>
						</td>
						<td>
						</td>
						<td>
							<b>Territórios da Cidadania.</b>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center;">
				<input type="button" value="Pesquisar" class="pesquisar" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<?php 
	if($_POST['campo']){
		$populacao['Campo'] = "/* X% da população do campo - 1619 */
							SELECT 
								muncod, 
								sum(total), 
								sum(valorrural), 
								CASE WHEN sum(total) > 0 THEN sum(valorrural)/sum(total)*100 ELSE 0 
								END as percentual 
							FROM
							(
								SELECT muncod, sum(tmdvalor) as total, 0 as valorrural 
								FROM mapa.temadado WHERE tmaid IN (18,19) GROUP BY muncod
								UNION ALL
								SELECT muncod, 0 as total, tmdvalor as valorrural 
								FROM mapa.temadado WHERE tmaid = 18
							) as f
							GROUP BY muncod
							HAVING CASE WHEN sum(total) > 0 THEN sum(valorrural)/sum(total)*100 ELSE 0 END > ".str_replace(',','.',$_POST['percCampo']);
	}
	
	if($_POST['pobreza']){
		$populacao['Pobreza'] = "/* X% da população em situação de pobreza extrema - 1553 */
							select muncod, sum(total), sum(valorrural), Case when sum(total) > 0 then sum(valorrural)/sum(total)*100 else 0 end as percentual from
							
							(
							select muncod, sum(tmdvalor) as total, 0 as valorrural from mapa.temadado where tmaid in (18,19) group by muncod
							union all
							select muncod, 0 as total, tmdvalor as valorrural from mapa.temadado where tmaid = 16
							) as f
							group by muncod
							having Case when sum(total) > 0 then sum(valorrural)/sum(total)*100 else 0 end > ".str_replace(',','.',$_POST['percPobreza']);
	}
	
	if($_POST['analf']){
		$populacao['Analf15Anos'] = "/* X% da população com 15 anos ou mais não alfabetizada - 1303 */
								select muncod, sum(total), sum(valorrural), sum(valorrural) as percentual from
								(
								select muncod, sum(tmdvalor) as total, 0 as valorrural from mapa.temadado where tmaid in (18,19) group by muncod
								union all
								select muncod, 0 as total, sum(tmdvalor) as valorrural from mapa.temadado where tmaid in (10) group by muncod
								) as f
								group by muncod
								having sum(valorrural) > ".str_replace(',','.',$_POST['percAnalf']);
	}
	
	if($_POST['educacao']){
		$populacao['SemEducSup'] = "/* 30% dos docentes que atuam no campo sem educação superior - 2563 */
									select muncod, sum(total), sum(valorrural), sum(valorrural) as percentual from
									(
									select muncod, sum(tmdvalor) as total, 0 as valorrural from mapa.temadado where tmaid in (18,19) group by muncod
									union all
									select muncod, 0 as total, sum(tmdvalor) as valorrural from mapa.temadado where tmaid in (22) group by muncod
									) as f
									group by muncod
									having sum(valorrural) > ".str_replace(',','.',$_POST['percEducacao']);
	}
	
	if($_POST['cidadania']){
		$populacao['TerCid'] = "/* Territórios da Cidadania - 3704 */
								select mtm.muncod, 0 as total, 0 as valorrural, 0 as valorrural from territorios.grupotipomunicipio gtm
								inner join territorios.tipomunicipio tm ON tm.gtmid = gtm.gtmid
								inner join territorios.muntipomunicipio mtm ON mtm.tpmid = tm.tpmid
								where gtm.gtmid = 5";
	}

	if(is_array($populacao)){
		$sql = "SELECT DISTINCT 
					m.muncod, 
					m.estuf|| ' - ' ||m.mundescricao 
				FROM territorios.municipio m
				INNER JOIN
				(
				".implode(' union all ', $populacao)."
				) as f ON f.muncod = m.muncod
				ORDER BY m.estuf|| ' - ' ||m.mundescricao";
		
		$cabecalho = array("Código do Município", "Município");
		$db->monta_lista($sql, $cabecalho, 50, 20, '', '', '', '', Array('10%','90%'));
	}
	
?>