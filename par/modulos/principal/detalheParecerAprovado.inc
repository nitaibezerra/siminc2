<?php
	if($_REQUEST['pdcid'] != ''){
		$pdcid = $_REQUEST['pdcid'];
		$where = " Where pdcid = $pdcid ";
		$sql = "
			Select	pdcparecer
			From par.parecerdacomissao 
			$where
		";
		$result = $db->pegaUm($sql);
	}
	
	monta_titulo('Detalhamento da Aprovação de Programa', 'Parecer e Listagem das subações Aprovadas');
?>	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">Parecer:</td>
			<td>			
				<textarea rows="10" cols="100" readonly="readonly""><?php echo $result;?></textarea>
			</td>
		</tr>
	</table>
	
<?php 
	echo "<br/>";

	if($_REQUEST['pdcid'] != '' && ($_REQUEST['estuf'] != '' || $_REQUEST['muncod'] != '')){

		if($_REQUEST['pdcid']) {
			$pdcid = $_REQUEST['pdcid'];
			$where = " Where spc.pdcid = $pdcid ";
		}		
		
		if($_REQUEST['estuf']) {
			$estuf = $_REQUEST['estuf'];
			$where .= " and e.estuf = '$estuf' ";
		}
		
		if($_REQUEST['muncod'] && $_REQUEST['muncod'] != 'null') {
			$muncod = $_REQUEST['muncod'];
			$where .= "and m.muncod = '$muncod'";
		}
		
		echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
		echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>";		
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
		echo "<td class=SubTituloCentro>Listagem:  Planos aprovados</td>";
		echo "<tr>";
		echo "</tr>";
		echo "<tr>";
		echo "<td>";
		
		$sql = "
			Select 	distinct s.sbaid,
					s.sbadsc,
					to_char(
					   case when sbacronograma = 1
					      then(
						Select	case when sic.icovalidatecnico is null
							   then
							      sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric
							   else
							      case when sic.icovalidatecnico = 'S'
							         then
								    sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric
							      end
							end as vlrsubacao
						From par.subacaoitenscomposicao sic
						Where sic.sbaid = s.sbaid and sic.icoano = sd.sbdano
						Group by sic.sbaid, sic.icovalidatecnico
					      )else(
						 Select case when (s.frmid = 2) or ( s.frmid = 4 AND s.ptsid = 42 ) or ( s.frmid = 12 AND s.ptsid = 46 )
							   then
							      case when se.sesvalidatecnico is null
								 then
								    sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric
								 else
								    sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric
							      end
							else
							   case when se.sesvalidatecnico is null
							      then
								 sum(coalesce(ssi.seiqtd,0) * coalesce(sic.icovalor,0))::numeric
							      else
								 sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric
							   end
							end as vlrsubacao
						From par.subacaoitenscomposicao sic
						Join par.subacaoescolas se on se.sbaid = sic.sbaid and se.sesano = sic.icoano
						Left Join  par.subescolas_subitenscomposicao ssi on ssi.sesid = se.sesid
						Where sic.sbaid = s.sbaid and sic.icoano = sd.sbdano
						Group by sic.sbaid, se.sesvalidatecnico
					      )END,'999G999G999G999D99'
					) AS valorsubacao,
					sd.sbdano
				FROM par.dimensao d
				Join par.area a ON a.dimid = d.dimid
				Join par.indicador i ON i.areid = a.areid
				Join par.criterio c ON c.indid = i.indid
				Join par.pontuacao p ON p.crtid = c.crtid
				Join par.instrumentounidade iu ON iu.inuid = p.inuid		
				Join par.acao ac ON ac.ptoid = p.ptoid
				Join par.subacao s ON s.aciid = ac.aciid and s.sbastatus = 'A'
				Join par.subacaodetalhe sd ON sd.sbaid = s.sbaid
				Join workflow.documento doc ON doc.docid = s.docid AND doc.esdid = 462
				
				Left Join par.programa  pr ON s.prgid = pr.prgid
				Left Join territorios.municipio m ON m.muncod = iu.muncod and iu.mun_estuf = m.estuf
				Left Join territorios.estado e ON e.estuf = iu.estuf
				Left Join par.subacaoparecerdacomissao spc ON spc.sbdid = sd.sbdid			
				$where
		";
		//die($sql);
		$cabecalho = array("Cód. Subação", "Descrição da Subação", "Valor", "ano");
		$db->monta_lista_simples($sql, $cabecalho, 100, 10, 'N', '', '');
		
		echo "</td>";
		echo "</tr>";
		echo "</table>";
		
		exit;
	}

?>