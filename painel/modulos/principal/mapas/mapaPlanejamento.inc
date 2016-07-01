<?php
	function montaBalaoTipoMunicipio()
	{
		global $db;
		
		ini_set("memory_limit", "2048M");
		set_time_limit(0);
		//error_reporting(-1);
		
		extract($_REQUEST);
		
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		
		
		$sql = "select
					tma.tmaid
				from
					mapa.mapatema mpt
				inner join
					mapa.mapa map ON map.mapid = mpt.mapid
				inner join
					mapa.tema tma ON tma.tmaid = mpt.tmaid
				where
					map.mapid = {$_SESSION['painel_vars']['mapid']}
				and
					tpdid != 4
				and
					tma.tmaid not in (2,3) 
				group by 
					tma.tmaid, tma.tmadsc
				order by
					tma.tmadsc";
		$hdn_tmaid = $db->carregarColuna($sql);
		
		$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
		$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
		$arrWhere[] = "tpm.gtmid = $gtmid";
		$arrWhere[] = "tpm.tpmid = $tpmid";
		$campo = "tpm.tpmid";
		$descricao = "tpmdsc";
		$chave = "tpm.tpmid";
		$arrGroupBy[] = "tpm.tpmid";
		$arrGroupBy[] = "tpmdsc";
			
		if($hdn_tmaid){
			foreach($hdn_tmaid as $tmaid){
				if($tmaid != 1){
					$arrInnerJoin[] = "mapa.temadado  tem_{$tmaid} ON tem_{$tmaid}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$tmaid}.tmaid = $tmaid");
					$sql = "select 
								tmadsc,
								tpddsc,
								tpdcampotema 
							from 
								mapa.tipodado tpd
							inner join
								mapa.tema tma ON tma.tpdid = tpd.tpdid
							where
								tma.tmaid = $tmaid";
					$tdm = $db->pegaLinha($sql);
					if($tdm['tpddsc'] == "Boleano"){
						$arrCampos[] = "'<div style=\"width:100%;color:blue;text-align:right\" >' || (CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) || '</div>' as capmpo{$tmaid}";
						$arrGroupBy[] = "tem_{$tmaid}.{$tdm['tpdcampotema']}";
					}else{
						$arrCampos[] = "'<div style=\"text-align:right;width:100%;color:blue\" >' || sum(tem_{$tmaid}.{$tdm['tpdcampotema']})".($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")." || '</div>' as capmpo{$tmaid}";	
					}
					$arrColunas[] = $tdm['tmadsc'];
				}else{
					$arrCampos[] = "'<div style=\"text-align:right;width:100%;color:blue\" >' || sum(mun.munpopulacao) || '</div>' as campo{$tmaid}";
					$arrColunas[] = "População";
				}
			}
		}
					
		$sql = "select distinct
					$campo as acao,
					$chave as muncod,
					$descricao as descri
					".($arrCampos ? " , ".implode(" , ",$arrCampos) : "")."
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " left join ".implode(" left join ",$arrInnerJoin) : "")."
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
				order by
					$descricao";
		
		//dbg($sql,1);
		$arrCabecalho = array("Ação","Descrição");
	
		if($arrColunas){
			foreach($arrColunas as $coluna){
				array_push($arrCabecalho,$coluna);
			}
		}
		
		$dados = $db->pegaLinha($sql);
		
		//echo "<pre>";
		//print_r($dados);
		
		if($dados){
			$n=0;
			foreach($dados as $chave => $dado){
				$n++;
				if($n > 3){
					$arrValores[] = $dado;
				}
			}
		}
		
		$n=0;
		foreach($arrColunas as $lista){
			$arrLista[$n] = array("descricao" => $lista, "valor" => $arrValores[$n]);
			$n++;
		}
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>{$dados['descri']}</td></tr>";
		echo "</table>";
		
		$sqlMun = "select 
					distinct muncod
				from
					territoriosgeo.muntipomunicipio
				where
					tpmid = $tpmid";
		
		$arrMuncod = $db->carregarColuna($sqlMun);
		
		$dadosplanejamento_sim = $db->carregar("SELECT * FROM mapa.planejamento2 WHERE trim(\"IBGE\") in('".implode("','",$arrMuncod)."') and trim(\"AnaliseCGPAC\")='Sim' ORDER BY \"PRIORIDADE DO PROPONENTE\"");
		if($dadosplanejamento_sim[0]) {
			echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
				echo "<tr><td class=SubTituloCentro>EMPREENDIMENTOS INDICADOS</td></tr>";
			foreach($dadosplanejamento_sim as $dp) {

				echo "<tr><td class=SubTituloEsquerda>".$dp['EMPREENDIMENTO']."</td></tr>";
				echo "<tr><td><table width=100%>";
				if($dp['MUNICIPIOSBENEFICIADOS']!="-") :
					echo "<tr>
								<td class=SubTituloDireita>MUNICÍPIOS BENEFICIADOS:</td>
								<td>".$dp['MUNICIPIOSBENEFICIADOS']."</td>
						  </tr>";
				endif;
				if($dp['MODALIDADE']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>MODALIDADE:</td>
							<td>".$dp['MODALIDADE']."</td>
						</tr>";
				endif;
				if($dp['DETALHAMENTO']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>DETALHAMENTO:</td>
							<td>".$dp['DETALHAMENTO']."</td>
						</tr>";
				endif;
				if($dp['QTDUHMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>QTD UH MCMV:</td>
							<td>".$dp['QTDUHMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor MCMV (R$ milhões):</td>
							<td>".$dp['ValorMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorUrbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Urbanização (R$ milhões):</td>
							<td>".$dp['ValorUrbanizacao']."</td>
						</tr>";
				endif;
				if($dp['ValorEstudosProjetos']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Estudos e Projetos Risco (R$ milhões):</td>
							<td>".$dp['ValorEstudosProjetos']."</td>
						</tr>";
				endif;
				if($dp['ValorObraRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Obra Risco (R$ milhões):</td>
							<td>".$dp['ValorObraRisco']."</td>
						</tr>";
				endif;
				if($dp['ValorTotalRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Total Risco (Proj+Obra) (R$ milhões):</td>
							<td>".$dp['ValorTotalRisco']."</td>
						</tr>";
				endif;
				if($dp['Projeto']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Projeto:</td>
							<td>".$dp['Projeto']."</td>
						</tr>";
				endif;
				if($dp['AnaliseCGPAC']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Análise CGPAC:</td>
							<td>".$dp['AnaliseCGPAC']."</td>
						</tr>";
				endif;
				if($dp['Justificativa']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Justificativa:</td>
							<td>".$dp['Justificativa']."</td>
						</tr>";
				endif;
				
				if($dp['drenagemcontencaocheias']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade macrodrenagem e/ou contenção de cheias:</td>
							<td>".(($dp['drenagemcontencaocheias']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['contecaoencostas']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade contenção de encostas:</td>
							<td>".(($dp['contecaoencostas']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['reassentamentourbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade reassentamento e/ou urbanização:</td>
							<td>".(($dp['reassentamentourbanizacao']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
						
				echo "</table></td></tr>";
			}
			echo "</table>";
		}
		
		
		$dadosplanejamento_nao = $db->carregar("SELECT * FROM mapa.planejamento2 WHERE trim(\"IBGE\") in('".implode("','",$arrMuncod)."') and trim(\"AnaliseCGPAC\")='Não' ORDER BY \"PRIORIDADE DO PROPONENTE\"");
		if($dadosplanejamento_nao[0]) {
			echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
				echo "<tr><td class=SubTituloCentro>EMPREENDIMENTOS NÃO INDICADOS</td></tr>";
			foreach($dadosplanejamento_nao as $dp) {

				echo "<tr><td class=SubTituloEsquerda>".$dp['EMPREENDIMENTO']."</td></tr>";
				echo "<tr><td><table width=100%>";
				if($dp['MUNICIPIOSBENEFICIADOS']!="-") :
					echo "<tr>
								<td class=SubTituloDireita>MUNICÍPIOS BENEFICIADOS:</td>
								<td>".$dp['MUNICIPIOSBENEFICIADOS']."</td>
						  </tr>";
				endif;
				if($dp['MODALIDADE']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>MODALIDADE:</td>
							<td>".$dp['MODALIDADE']."</td>
						</tr>";
				endif;
				if($dp['DETALHAMENTO']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>DETALHAMENTO:</td>
							<td>".$dp['DETALHAMENTO']."</td>
						</tr>";
				endif;
				if($dp['QTDUHMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>QTD UH MCMV:</td>
							<td>".$dp['QTDUHMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor MCMV (R$ milhões):</td>
							<td>".$dp['ValorMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorUrbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Urbanização (R$ milhões):</td>
							<td>".$dp['ValorUrbanizacao']."</td>
						</tr>";
				endif;
				if($dp['ValorEstudosProjetos']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Estudos e Projetos Risco (R$ milhões):</td>
							<td>".$dp['ValorEstudosProjetos']."</td>
						</tr>";
				endif;
				if($dp['ValorObraRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Obra Risco (R$ milhões):</td>
							<td>".$dp['ValorObraRisco']."</td>
						</tr>";
				endif;
				if($dp['ValorTotalRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Total Risco (Proj+Obra) (R$ milhões):</td>
							<td>".$dp['ValorTotalRisco']."</td>
						</tr>";
				endif;
				if($dp['Projeto']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Projeto:</td>
							<td>".$dp['Projeto']."</td>
						</tr>";
				endif;
				if($dp['AnaliseCGPAC']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Análise CGPAC:</td>
							<td>".$dp['AnaliseCGPAC']."</td>
						</tr>";
				endif;
				if($dp['Justificativa']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Justificativa:</td>
							<td>".$dp['Justificativa']."</td>
						</tr>";
				endif;
				
				if($dp['drenagemcontencaocheias']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade macrodrenagem e/ou contenção de cheias:</td>
							<td>".(($dp['drenagemcontencaocheias']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['contecaoencostas']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade contenção de encostas:</td>
							<td>".(($dp['contecaoencostas']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['reassentamentourbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade reassentamento e/ou urbanização:</td>
							<td>".(($dp['reassentamentourbanizacao']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
						
				echo "</table></td></tr>";
			}
			echo "</table>";
		}
		
		
		
		
		
		$sql2 = "SELECT  CASE WHEN mpcdsc IS NOT NULL THEN dtidsc ||' ('|| mpcdsc ||')' else dtidsc END as dtidsc,
						'<div style=\"text-align:right;width:100%;color:blue\" >' ||CASE WHEN tpdid = 1 THEN replace(to_char(ROUND(sum(vlivalor),2), '999g999g999g999d99'),',','.')
							 WHEN tpdid = 2 THEN replace(to_char(ROUND(sum(vliqtd)), '999g999g999g999'),',','.') 
						END || '</div>' as valor
				FROM mapa.valorindicador vi 
				INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
				LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
				INNER JOIN mapa.indicador ind ON ind.indid = di.indid
				WHERE ind.indstatus = 'A' and di.tpdid!= 4 and di.tpdid!= 3 and vi.muncod in ('".implode("','",$arrMuncod)."') AND ind.mapid='".$_SESSION['painel_vars']['mapid']."' 
				group by mpcdsc,di.dtidsc,vi.vliboleano,ind.inddsc,di.dtiordem,tpdid
				ORDER BY ind.inddsc, di.dtiordem";
		//dbg($sql2);
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'")."</td></tr>";
		echo "</table>";
		$cabecalho = array("Detalhe","&nbsp;");
		$db->monta_lista_simples($sql2,$cabecalho,50,5,'N','100%',$par2);
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>Temas</td></tr>";
		echo "</table>";
		$cabecalho = array("Tema","&nbsp;");
		$db->monta_lista_simples($arrLista,$cabecalho,50,5,'N','100%',$par2);
	}




	function montaBalao() {
		global $db;
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		
		echo "<script>
				function controleImg(obj,map) {
					if(obj.title=='mais') {
						obj.title='menos';
						obj.src='../imagens/menos.gif';
						document.getElementById('tr_'+map).style.display='';
					} else {
						obj.title='mais';
						obj.src='../imagens/mais.gif';
						document.getElementById('tr_'+map).style.display='none';
					}
				}
			  </script>";
		
		$municipio = $db->pegaUm("SELECT mundescricao||' / '||estuf FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>".$municipio."</td></tr>";
		echo "</table>";

			$dadosplanejamento_sim = $db->carregar("SELECT * FROM mapa.planejamento2 WHERE trim(\"IBGE\")='".$_REQUEST['muncod']."' and trim(\"AnaliseCGPAC\")='Sim' ORDER BY \"PRIORIDADE DO PROPONENTE\"");
		if($dadosplanejamento_sim[0]) {
			echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
				echo "<tr><td class=SubTituloCentro>EMPREENDIMENTOS INDICADOS</td></tr>";
			foreach($dadosplanejamento_sim as $dp) {

				echo "<tr><td class=SubTituloEsquerda>".$dp['EMPREENDIMENTO']."</td></tr>";
				echo "<tr><td><table width=100%>";
				if($dp['MUNICIPIOSBENEFICIADOS']!="-") :
					echo "<tr>
								<td class=SubTituloDireita>MUNICÍPIOS BENEFICIADOS:</td>
								<td>".$dp['MUNICIPIOSBENEFICIADOS']."</td>
						  </tr>";
				endif;
				if($dp['MODALIDADE']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>MODALIDADE:</td>
							<td>".$dp['MODALIDADE']."</td>
						</tr>";
				endif;
				if($dp['DETALHAMENTO']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>DETALHAMENTO:</td>
							<td>".$dp['DETALHAMENTO']."</td>
						</tr>";
				endif;
				if($dp['QTDUHMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>QTD UH MCMV:</td>
							<td>".$dp['QTDUHMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor MCMV (R$ milhões):</td>
							<td>".$dp['ValorMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorUrbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Urbanização (R$ milhões):</td>
							<td>".$dp['ValorUrbanizacao']."</td>
						</tr>";
				endif;
				if($dp['ValorEstudosProjetos']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Estudos e Projetos Risco (R$ milhões):</td>
							<td>".$dp['ValorEstudosProjetos']."</td>
						</tr>";
				endif;
				if($dp['ValorObraRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Obra Risco (R$ milhões):</td>
							<td>".$dp['ValorObraRisco']."</td>
						</tr>";
				endif;
				if($dp['ValorTotalRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Total Risco (Proj+Obra) (R$ milhões):</td>
							<td>".$dp['ValorTotalRisco']."</td>
						</tr>";
				endif;
				if($dp['Projeto']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Projeto:</td>
							<td>".$dp['Projeto']."</td>
						</tr>";
				endif;
				if($dp['AnaliseCGPAC']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Análise CGPAC:</td>
							<td>".$dp['AnaliseCGPAC']."</td>
						</tr>";
				endif;
				if($dp['Justificativa']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Justificativa:</td>
							<td>".$dp['Justificativa']."</td>
						</tr>";
				endif;
				
				if($dp['drenagemcontencaocheias']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade macrodrenagem e/ou contenção de cheias:</td>
							<td>".(($dp['drenagemcontencaocheias']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['contecaoencostas']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade contenção de encostas:</td>
							<td>".(($dp['contecaoencostas']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['reassentamentourbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade reassentamento e/ou urbanização:</td>
							<td>".(($dp['reassentamentourbanizacao']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
						
				echo "</table></td></tr>";
			}
			echo "</table>";
		}
		
		
		$dadosplanejamento_nao = $db->carregar("SELECT * FROM mapa.planejamento2 WHERE trim(\"IBGE\")='".$_REQUEST['muncod']."' and trim(\"AnaliseCGPAC\")='Não' ORDER BY \"PRIORIDADE DO PROPONENTE\"");
		if($dadosplanejamento_nao[0]) {
			echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
				echo "<tr><td class=SubTituloCentro>EMPREENDIMENTOS NÃO INDICADOS</td></tr>";
			foreach($dadosplanejamento_nao as $dp) {

				echo "<tr><td class=SubTituloEsquerda>".$dp['EMPREENDIMENTO']."</td></tr>";
				echo "<tr><td><table width=100%>";
				if($dp['MUNICIPIOSBENEFICIADOS']!="-") :
					echo "<tr>
								<td class=SubTituloDireita>MUNICÍPIOS BENEFICIADOS:</td>
								<td>".$dp['MUNICIPIOSBENEFICIADOS']."</td>
						  </tr>";
				endif;
				if($dp['MODALIDADE']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>MODALIDADE:</td>
							<td>".$dp['MODALIDADE']."</td>
						</tr>";
				endif;
				if($dp['DETALHAMENTO']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>DETALHAMENTO:</td>
							<td>".$dp['DETALHAMENTO']."</td>
						</tr>";
				endif;
				if($dp['QTDUHMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>QTD UH MCMV:</td>
							<td>".$dp['QTDUHMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorMCMV']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor MCMV (R$ milhões):</td>
							<td>".$dp['ValorMCMV']."</td>
						</tr>";
				endif;
				if($dp['ValorUrbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Urbanização (R$ milhões):</td>
							<td>".$dp['ValorUrbanizacao']."</td>
						</tr>";
				endif;
				if($dp['ValorEstudosProjetos']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Estudos e Projetos Risco (R$ milhões):</td>
							<td>".$dp['ValorEstudosProjetos']."</td>
						</tr>";
				endif;
				if($dp['ValorObraRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Obra Risco (R$ milhões):</td>
							<td>".$dp['ValorObraRisco']."</td>
						</tr>";
				endif;
				if($dp['ValorTotalRisco']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Valor Total Risco (Proj+Obra) (R$ milhões):</td>
							<td>".$dp['ValorTotalRisco']."</td>
						</tr>";
				endif;
				if($dp['Projeto']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Projeto:</td>
							<td>".$dp['Projeto']."</td>
						</tr>";
				endif;
				if($dp['AnaliseCGPAC']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Análise CGPAC:</td>
							<td>".$dp['AnaliseCGPAC']."</td>
						</tr>";
				endif;
				if($dp['Justificativa']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Justificativa:</td>
							<td>".$dp['Justificativa']."</td>
						</tr>";
				endif;
				
				if($dp['drenagemcontencaocheias']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade macrodrenagem e/ou contenção de cheias:</td>
							<td>".(($dp['drenagemcontencaocheias']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['contecaoencostas']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade contenção de encostas:</td>
							<td>".(($dp['contecaoencostas']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
				if($dp['reassentamentourbanizacao']!="-") :
					echo "<tr>
							<td class=SubTituloDireita>Proposta que se enquadra na modalidade reassentamento e/ou urbanização:</td>
							<td>".(($dp['reassentamentourbanizacao']=="1")?"Sim":"Não")."</td>
						</tr>";
				endif;
				
						
				echo "</table></td></tr>";
			}
			echo "</table>";
		}
		
		$sql = "SELECT  CASE WHEN mpcdsc IS NOT NULL THEN dtidsc ||' ('|| mpcdsc ||')' else dtidsc END as dtidsc,
						'<div style=\"text-align:right;width:100%;color:blue\" >' ||CASE WHEN vlivalor IS NOT NULL THEN replace(to_char(ROUND(vlivalor,2), '999g999g999g999d99'),',','.')
							 WHEN vliqtd IS NOT NULL THEN replace(to_char(ROUND(vliqtd), '999g999g999g999'),',','.') 
							 WHEN vliboleano IS NOT NULL THEN CASE WHEN vliboleano=TRUE THEN 'Sim' ELSE 'Não' END 
						END || '</div>' as valor
				FROM mapa.valorindicador vi 
				INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
				LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
				INNER JOIN mapa.indicador ind ON ind.indid = di.indid
				WHERE ind.indstatus = 'A' and vi.muncod='".$_REQUEST['muncod']."' AND ind.mapid='".$_SESSION['painel_vars']['mapid']."' 
				ORDER BY dtidsc, ind.inddsc, di.dtiordem";
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'")."</td></tr>";
		echo "</table>";
		$cabecalho = array("Detalhe","&nbsp;");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		$sql = "SELECT tmadsc,
						'<div style=\"text-align:right;width:100%;color:blue\" >' || CASE WHEN tmdvalor IS NOT NULL THEN to_char(ROUND(tmdvalor,2), '999g999g999g999d99')
						WHEN tmdboleano IS NOT NULL THEN CASE WHEN tmdboleano=TRUE THEN 'Sim' ELSE 'Não' END 
						WHEN tmdtexto IS NOT NULL THEN tmdtexto 
						END || '</div>'as valor  
				FROM mapa.temadado td 
				INNER JOIN mapa.tema tm ON tm.tmaid = td.tmaid
				INNER JOIN mapa.mapatema mtm ON tm.tmaid = mtm.tmaid 
				WHERE muncod='".$_REQUEST['muncod']."' and mtm.mapid = ".$_REQUEST['mapid']." order by tmadsc";
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>Dados Gerais</td></tr>";
		echo "</table>";
		$cabecalho = array("Tema","&nbsp;");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		
		
		// outros mapas 
		
		$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
		$pflcod = PAINEL_PERFIL_MAPAS;
		
		//pegando os grupos mapas
		$sql = "SELECT gm.gpmdsc, gm.gpmid FROM mapa.mapagrupomapa mg 
		 		INNER JOIN mapa.grupomapa gm ON gm.gpmid = mg.gpmid   
		 		WHERE mg.mapid='".$_SESSION['painel_vars']['mapid']."' order by gm.gpmdsc";
		
		$gruposdomapa = $db->carregar($sql);
		
		if($gruposdomapa[0]) {
			foreach($gruposdomapa as $gdm) {
				echo "<p><b>Outros Mapas - ".$gdm['gpmdsc']."</b></p>";
				if($db->testa_superuser()) {
					$sql_m = "SELECT m.mapid FROM mapa.mapa m 
							  INNER JOIN mapa.mapagrupomapa mgm ON mgm.mapid = m.mapid  
							  WHERE m.mapstatus = 'A' AND m.mapid!='".$_SESSION['painel_vars']['mapid']."' AND mgm.gpmid='{$gdm['gpmid']}'";
				} else {
					$sql_m = "SELECT m.mapid FROM painel.usuarioresponsabilidade ur 
							  INNER JOIN mapa.mapa m ON ur.mapid = m.mapid 
							  INNER JOIN mapa.mapagrupomapa mgm ON mgm.mapid = m.mapid 
							  WHERE	ur.rpustatus = 'A' AND mgm.gpmid='{$gdm['gpmid']}' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod AND m.mapstatus = 'A' AND m.mapid!='".$_SESSION['painel_vars']['mapid']."'";
				}
				
				$sql = "SELECT  ind.mapid,
				 				CASE WHEN mpcdsc IS NOT NULL THEN dtidsc ||' ('|| mpcdsc ||')' else dtidsc END as dtidsc,
								CASE WHEN vlivalor IS NOT NULL THEN ROUND(vlivalor,2)::text
									 WHEN vliqtd IS NOT NULL THEN ROUND(vliqtd)::text
									 WHEN vliboleano IS NOT NULL THEN CASE WHEN vliboleano=TRUE THEN '<center>Sim</center>' ELSE '<center>Não</center>' END 
								END as valor
						FROM mapa.valorindicador vi 
						INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
						LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
						INNER JOIN mapa.indicador ind ON ind.indid = di.indid 
						INNER JOIN mapa.mapa map ON map.mapid = ind.mapid
						WHERE vi.muncod='".$_REQUEST['muncod']."' AND ind.mapid IN({$sql_m}) 
						ORDER BY map.mapdsc,dtidsc,ind.inddsc,di.dtiordem";
				
				$mapas = $db->carregar($sql);
				
				if($mapas[0]) {
					foreach($mapas as $ma) {
						$arrMapas[$ma['mapid']][] = array("dtidsc"=>$ma['dtidsc'],"valor"=>$ma['valor']); 
					}
				}
				if($arrMapas) {
					foreach($arrMapas as $mapid => $dadosMapa) {
						echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
						echo "<tr><td style=width:10px;><img src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=controleImg(this,'".$mapid."');></td><td class=SubTituloEsquerda>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$mapid."'")."</td></tr>";
						echo "<tr style=display:none; id=tr_".$mapid."><td colspan=2>";
						$cabecalho = array("Detalhe","&nbsp;");
						$db->monta_lista_simples($dadosMapa,$cabecalho,50,5,'N','100%',$par2);
						echo "</td></tr>";
						echo "</table>";
			
					}
				}
				
				
			}
		}
		
	
	}

define('MAP_FIXED',	  '27');
define('LINK_PADRAO', 'painel.php?modulo=principal/mapas/mapaPlanejamento&acao=A');

include_once APPRAIZ.'/painel/modulos/principal/mapas/mapaPadrao.inc';

?>