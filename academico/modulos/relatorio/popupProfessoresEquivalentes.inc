<?php 

function montaSqlProfEquivalente($filtros = array())
{
	
	if(!empty($filtros)) extract($_REQUEST);
	
	if($ano) $arWhere[] = "ppe.ppeano = '{$ano}'";
	if($portaria) $arWhere[] = "ppe.ppeid = {$portaria}";
	if($mes){
		$arWhere[] = "mpemes = {$mes}";
	}
	
	$sql = "SELECT
			UPPER(entsig) as codigo,				
			UPPER(entnome) as orgao, 
			upper(mun.mundescricao) as municipio, 
			upper(mun.estuf) as uf,
			--mpeano,
			--mpemes,
			coalesce(mpevlr20h,0) as mpevlr20h,
			coalesce(mpevlr40h,0) as mpevlr40h,
			coalesce(mpevlrdedexclusiva,0) as mpevlrdedexclusiva,
			(mpevlr20h+mpevlr40h+mpevlrdedexclusiva) as total,
			coalesce(mpevlrsubstituto,0) as mpevlrsubstituto,
			coalesce(mpevlrvisitante,0) as mpevlrvisitante,
			coalesce(mpevlrvagos,0) as mpevlrvagos,
			coalesce(mpevlrbcequiv,0) as mpevlrbcequiv,
			coalesce(ptvvalor,0) as ptvvalor,
			(coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ) as portmpmec
		FROM
			entidade.entidade e 
		INNER JOIN
			entidade.funcaoentidade ef ON ef.entid = e.entid
		LEFT JOIN
			academico.portariavalor pv ON pv.entid = e.entid  
		LEFT JOIN
			entidade.endereco ed ON ed.entid = e.entid
		LEFT JOIN
			territorios.municipio mun ON mun.muncod = ed.muncod
		LEFT JOIN
			academico.movprofequivalente mpe ON mpe.entid = e.entid AND mpestatus = 'A'				
		LEFT JOIN 
			academico.portariaprofequival ppe ON pv.ppeid = ppe.ppeid
		WHERE
			e.entstatus = 'A' AND ef.funid  in ('12')
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."		
		ORDER BY
			 e.entsig, e.entnome";

	return $sql;
}


if($_REQUEST['ano']) extract($_REQUEST);
?>
 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery.printElement.js"></script>
<style>
</style>
<script><!--

	$(function(){
		
		window.print();
		
	});
--></script>

<div id="div_relatorio_professores_equivalentes">
	<?php 
	$linha1 = "Banco de Professor Equivalente";
	$linha2 = "Relatório";
	
	monta_titulo($linha1, $linha2);
	?>
	<form name="formulario" id="formulario" method="post" action="">
		<input type="hidden" name="boPopup" id="boPopup" value="f" /> 
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita" width="160">Ano</td>
			<td>
				<?php echo $ano; ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Mês</td>
			<td id="td_mes">
				<?php echo $mes; ?>
			</td>			
		</tr>
		<tr>
			<td class="subtituloDireita">Portaria</td>
			<td id="td_portaria">
				<?php 
				
				$sql = "select 
						distinct ppenumero
						from 
						academico.portariaprofequival as p where ppeid = ".$portaria;
				
				if( $portaria ){
					$numPortaria = $db->pegaUm($sql);
					echo $numPortaria; 
				}
				?>
			</td>
		</tr>
		
	</table>
	</form>
	<?php
	if($_REQUEST['ano']){
		
		$sql = montaSqlProfEquivalente($_REQUEST);
		
		$cabecalho = array('Código', 'Órgão', 'Município', 'UF', '20h', '40h', 'DE', 'Total', 'Subst.', 'Visit.', 'Vagos', 'BancoPEqv', 'Port. MP/MEC 440/2011', 'Saldo');
		$db->monta_lista($sql, $cabecalho, 1000000, 10, 'N', '', '', '', '');
	}	
	?>
</div>
