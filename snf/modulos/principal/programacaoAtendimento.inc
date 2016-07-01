<?php

function inserirPreOferta($dados) {
	global $db;
	if($dados['val']) {
		foreach($dados['val'] as $curid => $arr1) {
			foreach($arr1 as $ano => $arr2) {
				foreach($arr2 as $muncod => $arr3) {
					$db->executar("DELETE FROM snf.atendimento WHERE curid='".$curid."' AND ateano='".$ano."' AND muncod='".$muncod."'");
					foreach($arr3 as $t => $quantidade) {
						if($quantidade) {
							$vls = explode("_",$t);
							
							$sql = "INSERT INTO snf.atendimento(".$vls[0].", curid, ateqtde, muncod, ateano)
	    							VALUES (".$vls[1].", '".$dados['curid']."', '".$quantidade."', '".$muncod."', '".$ano."');";
							
							$db->executar($sql);
						}
						
					}
				}
			}
		}
	}
	
	$db->commit();
	
	echo "<script>
			alert('Vagas gravadas com sucesso');
			window.close();
		  </script>";
}


function gerenciarPreOferta($dados) {
	global $db;
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<script>
			function gravapreoferta() {
				var total=0;
				jQuery(\"[name^='val[']\").each(function(k, v){
					if(v.value != '') {
   						total += parseInt(v.value);
   					}
 				});
 				
 				document.getElementById('formulario').submit();
			}
		  </script>";
	
	monta_titulo('Gerenciar Pre-Oferta','');
	
	echo "<form name=formulario id=formulario method=post>";
	echo "<input type=hidden name=requisicao value=inserirPreOferta>";
	
	$sql = "select insnome, ndescricao, mundescricao, quantidade, '<center><input name=\"val[".$dados['curid']."][".$dados['ano']."][".$dados['muncod']."]['||tipo||'_'||id||']\" type=\"text\" class=\"normal\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###\',this.value);\" maxlength=\"4\" size=\"5\" style=\"text-align:;\" value=\"'||coalesce((select ateqtde from snf.atendimento where curid=foo.curid and ateano=foo.ofeano and muncod=foo.muncod and case when tipo='polid' then polid=foo.id when tipo='cmpid' then cmpid=foo.id when tipo='pobid' then pobid=foo.id end)::character varying(10),'') ||'\"></center>' as v from (
			select tipo, id, curid, estuf, muncod, mundescricao, ndescricao, sum(ofeqtde) as quantidade, insnome, dirnome, ofeano
			from (
			select 'pobid' as tipo, pu.pobid as id, o.curid, o.ofeid, en.estuf, m.muncod, m.mundescricao, pu.pobdescricao as ndescricao, o.ofeqtde, ies.insnome, dm.dirnome, o.ofeano
			from snf.oferta o
			inner join snf.polouab pu
			using(pobid)
			inner join snf.instituicaouab iu
			on iu.pobid = pu.pobid
			inner join snf.instituicaoensino ies
			on ies.insid = iu.insid
			and ies.insid = o.insid
			inner join snf.dirigentemaximo dm
			on ies.insid = dm.insid
			and dm.dirstatus = 'A'
			inner join snf.enderecopolo en
			on en.endid = pu.endid
			inner join territorios.municipio m
			on m.muncod = en.muncod
			where o.curid = '".$dados['curid']."' and en.estuf = '".$dados['estuf']."' and o.ofeano='".$dados['ano']."'
			UNION
			select 'polid' as tipo, pu.polid as id, o.curid, o.ofeid, en.estuf, m.muncod, m.mundescricao, pu.poldescricao as ndescricao, o.ofeqtde, ies.insnome, dm.dirnome, o.ofeano
			from snf.oferta o
			inner join snf.polo pu
			using(polid)
			inner join snf.instituicaoensino ies
			on ies.insid = pu.insid
			inner join snf.dirigentemaximo dm
			on ies.insid = dm.insid
			and dm.dirstatus = 'A'
			inner join snf.enderecopolo en
			on en.endid = pu.endid
			inner join territorios.municipio m
			on m.muncod = en.muncod
			where o.curid = '".$dados['curid']."' and en.estuf = '".$dados['estuf']."' and o.ofeano='".$dados['ano']."'
			UNION   
			select 'cmpid' as tipo, c.cmpid as id, o.curid, o.ofeid, enc.estuf, m.muncod, m.mundescricao, ent.entnome as ndescricao, o.ofeqtde, ies.insnome, dm.dirnome, o.ofeano
			from snf.oferta o
			inner join academico.campus c
			on o.cmpid = c.cmpid 
			inner join entidade.entidade ent
			on c.entid = ent.entid
			inner join entidade.endereco enc
			on enc.entid = c.entid
			inner join entidade.funcaoentidade fen
			on fen.entid = ent.entid
			and fen.funid = 18
			inner join entidade.funentassoc fea
			on fea.fueid = fen.fueid
			inner join entidade.entidade I
			on I.entid = fea.entid
			inner join entidade.funcaoentidade fies
			on fies.entid = I.entid
			and fies.funid in (11,12)
			inner join snf.instituicaoensino ies
			on ies.entid = fies.entid
			inner join snf.dirigentemaximo dm
			on ies.insid = dm.insid
			inner join territorios.municipio m
			on m.muncod = enc.muncod
			where o.curid = '".$dados['curid']."' and enc.estuf = '".$dados['estuf']."' and o.ofeano='".$dados['ano']."'
		) AS OFERTAS

		group by tipo, id, curid, estuf, muncod, mundescricao, ndescricao, insnome, dirnome, ofeano) foo";
	
		//echo $sql;
		//die;
	
	$cabecalho = array("","Campus/Polo/Unidade","Município","Vagas(Pré-oferta)","Vagas utilizadas");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','95%',$par2);
	
	$mundescricao = $db->pegaUm("select mundescricao from territorios.municipio where muncod='".$dados['muncod']."'");
	echo "<table class=tabela align=center cellSpacing=1 cellPadding=3 ><tr><td class=subtituloDireita><b>Vagas Solicitadas(".$dados['ano'].") - ".$mundescricao." :</b></td><td>".$dados['vagas_total']."<input type=hidden name=vagas_total id=vagas_total value=\"".$dados['vagas_total']."\"></td></tr></table>";
	
	echo "<p align=center><input type=button name=ok value=Ok onclick=\"gravapreoferta();\"></p>";
	echo "</form>";
}


function atendimentoDemanda($dados) {
	global $db;
	
	$sql = "select	pcfid as codigo,
					pcfano as descricao
			from pdeinterativo.periodocursoformacao
			where pcfstatus = 'A' and pcfid in (".$dados['pcfid'].") 
			order by pcfano";
	
	$periodos = $db->carregar($sql);
	
	$html_tit[] = "<td class=\"SubtituloCentro\">Município</td>";
	
	if($periodos[0]) {
		foreach($periodos as $per) {
			$html_tit[] = "<td class=\"SubtituloCentro\" width=8%>Vagas Solicitadas (".$per['descricao'].")</td>";
			$html_tit[] = "<td class=\"SubtituloCentro\" width=30%>Ofertantes (".$per['descricao'].")</td>";
		}
	}
	
	if($periodos[0]) {
		foreach($periodos as $per) {
			
			$sql = "select p.pcfano, p.pdiesfera, m.muncod, coalesce(sum(p.privagasprevistas),0) as vagas  
					from snf.prioridadecursoescola p
					inner join territorios.municipio m using(muncod)
					where p.estuf = '".$dados['estuf']."'
					and p.pristatus = 'A'
					and p.pcfid = '".$per['codigo']."'
					and p.ateid = '".$dados['ateid']."'
					and p.curid = '".$dados['curid']."'
					-- and p.ncuid = '".$dados['ncuid']."'
					and p.modid = '".$dados['modid']."'
					group by p.pcfano, p.pdiesfera, m.muncod
					having coalesce(sum(p.privagasprevistas),0)  <> 0
					order by 1, 4";
		
			$linhas = $db->carregar($sql);
			
			if($linhas[0]) {
				foreach($linhas as $linha) {
					if($linha) $dados_ac[$linha['muncod']][$linha['pcfano']] = $linha['vagas'];		
				}
			}
			
		}
	}
	
	if($dados_ac) {
		foreach($dados_ac as $municipio =>$l) {

			$mundescricao = $db->pegaUm("select mundescricao from territorios.municipio where muncod='".$municipio."'");
			$html_col[] = "<td>".$mundescricao."</td>";
			if($periodos[0]) {
				foreach($periodos as $per) {
					
					$html_col[] = "<td width={td_width} align=center><font size=3><b>".(($l[$per['descricao']])?$l[$per['descricao']]:"0")."</b></font></td>";
					
					$sql = "select case when a.polid is not null then poldescricao 
										when c.cmpid is not null then ent2.entnome||' / '||ent.entnome
										when b.pobid is not null then b.pobdescricao
								   end as descricao, ateqtde from snf.atendimento a 
							left join snf.polo p on p.polid = a.polid 
							left join snf.polouab b on b.pobid = a.pobid
							left join academico.campus c on a.cmpid = c.cmpid 
							left join entidade.entidade ent on c.entid = ent.entid 
							left join entidade.funcaoentidade fen on fen.entid = ent.entid 
							left join entidade.funentassoc fea on fea.fueid = fen.fueid 
							left join entidade.entidade ent2 on ent2.entid = fea.entid 
							left join entidade.funcaoentidade fen2 on fen2.entid = ent2.entid and fen2.funid=".FUN_UNIVERSIDADE."
							where curid='".$dados['curid']."' and ateano='".$per['descricao']."' and muncod='".$municipio."'";
					
					
					$ofer = $db->carregar($sql);
					
					$ofertante_html = "<p align=center><input type=button name=ger value=Gerenciar  onclick=\"gerenciarPreOferta('&requisicao=gerenciarPreOferta&estuf=".$dados['estuf']."&curid=".$dados['curid']."&ano=".$per['descricao']."&muncod=".$municipio."&vagas_total=".(($l[$per['descricao']])?$l[$per['descricao']]:"0")."');\"></p>";
					
					if($ofer[0]) {
						$ofertante_html .= "<table width=100%>";
						$ofertante_html .= "<tr><td class=\"SubtituloCentro\">/UNID.</td><td class=\"SubtituloCentro\">VAGAS</td></tr>";
						foreach($ofer as $of) {
							$ofertante_html .= "<tr><td>".$of['descricao']."</td><td>".$of['ateqtde']."</td></tr>"; 
						}
						$ofertante_html .= "</table>";
					} else {
						$ofertante_html .= "<p align=center>Não existem ofertantes</p>";
					}
					
					$html_col[] = "<td>".$ofertante_html."</td>";
				}
			}
			$html_lin[] = "<tr>".implode("",$html_col)."</tr>";
			unset($html_col);
				
		}
	}
	echo "<table class=\"listagem\" width=\"100%\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr>".implode("",$html_tit)."</tr>";
	echo (($html_lin)?implode("",$html_lin):"<tr><td colspan=".count($html_tit)." align=\"center\" style=\"color:#cc0000;\">Não foram encontrados Registros.</td></td>");
	echo "</table>";
	
	
}

function filtraCursos($dados) {
	global $db;
	if($dados['ateid']) {
		/*
		$sql = "select 
					curid as codigo,
					curdesc as descricao
				from 
					catalogocurso.curso
				where 
					curstatus = 'A'
				and
					ateid = '".$dados['ateid']."'
				order by 
					curdesc";
		*/
		$ateid = $dados['ateid'];
		$sql = "select c.curid ||''|| mc.modid as codigo, c.curdesc || '(' || n.ncudesc || ' - '  || m.moddesc || ')' as descricao
							from catalogocurso.curso c
							inner join catalogocurso.modalidadecurso_curso mc
								on mc.curid = c.curid
							inner join catalogocurso.modalidadecurso m
								on mc.modid = m.modid
							natural join catalogocurso.nivelcurso n
							where c.curstatus = 'A'
							and c.ateid = $ateid
							order by 2";
		$db->monta_combo("curid",$sql,"S","Selecione...","","","","","S","curid","",$curid.$modid);
	} else {
		echo "Selecione uma Área Temática";
	}
}

function demandaTotal($dados) {
	global $db;
	
	if($dados['curid']){
		$dados['modid'] = substr($dados['curid'], -1);
		$dados['curid'] = substr($dados['curid'], 0, -1);
	}

	$sql = "select	pcfid as codigo,
					pcfano as descricao
			from pdeinterativo.periodocursoformacao
			where pcfstatus = 'A' and pcfid in (".$dados['pcfid'].") 
			order by pcfano";
	
	$periodos = $db->carregar($sql);
	
	$html_tit[] = "<td width=33%>&nbsp;</td>";
	$html_lin[] = "<td class=\"SubtituloDireita\">TOTAL:</td>";
	
	if($periodos[0]) {
		foreach($periodos as $per) {
			
			$sql = "select sum(p.privagasprevistas) as prev
					from snf.prioridadecursoescola p
					inner join snf.prioridadedocumento pd using(prdid) 
					inner join workflow.documento d using(docid) 
					where p.estuf = '".$dados['estuf']."'
					and p.pristatus = 'A'
					and p.pcfid = '".$per['codigo']."' 
					and p.ateid = '".$dados['ateid']."'
					and p.curid = '".$dados['curid']."' 
					-- and p.ncuid = '".$dados['ncuid']."' 
					and p.modid = '".$dados['modid']."'
					and d.esdid = 464 
					order by 1";

			$vagasprevistas = $db->pegaUm($sql);
			
			$html_tit[] = "<td class=\"SubtituloCentro\">".$per['descricao']."</td>";
			$html_lin[] = "<td>".(($vagasprevistas)?$vagasprevistas:"0")."</td>";
		}
	}
	
	echo "<table class=\"listagem\" width=\"100%\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr>".implode("",$html_tit)."</tr>";
	echo "<tr>".implode("",$html_lin)."</tr>";
	echo "</table>";
}

function demandaEsfera($dados) {
	global $db;
	
	if($dados['curid']){
		$dados['modid'] = substr($dados['curid'], -1);
		$dados['curid'] = substr($dados['curid'], 0, -1);
	}
	
	$sql = "select	pcfid as codigo,
					pcfano as descricao
			from pdeinterativo.periodocursoformacao
			where pcfstatus = 'A' and pcfid in (".$dados['pcfid'].") 
			order by pcfano";
	
	$periodos = $db->carregar($sql);
	
	$esferas = array("Estadual","Municipal","Federal");
	
	$html_tit[] = "<td width=33% class=\"SubtituloCentro\">REDE</td>";
	
	if($periodos[0]) {
		foreach($periodos as $per) {
			$html_tit[] = "<td class=\"SubtituloCentro\">".$per['descricao']."</td>";
		}
	}
	
	foreach($esferas as $esf) {
		
		$html_col[] = "<td class=\"SubtituloDireita\">".$esf.":</td>";
		
		if($periodos[0]) {
			foreach($periodos as $per) {
				
				
				$sql = "Select sum(p.privagasprevistas)
						from snf.prioridadecursoescola p
						inner join snf.prioridadedocumento pd using(prdid)
						inner join workflow.documento d using(docid)
						where p.estuf = '".$dados['estuf']."'
						and p.pristatus = 'A'
						and p.pcfid = '".$per['codigo']."'
						and p.ateid = '".$dados['ateid']."'
						and p.curid = '".$dados['curid']."'
						--and p.ncuid = '".$dados['ncuid']."'
						and p.modid = '".$dados['modid']."' 
						and p.pdiesfera = '".$esf."' 
						and d.esdid = 464
						group by p.pcfid, p.pcfano
						order by 1";
			
// 				$sql = "select coalesce(sum(p.privagasprevistas),0) as vagas  
// 						from snf.prioridadecursoescola p
// 						where p.estuf 	  = '".$dados['estuf']."'
// 						and p.pristatus   = 'A'
// 						and p.pcfid 	  = '".$per['codigo']."'
// 						and p.ateid 	  = '".$dados['ateid']."' 
// 						and p.curid 	  = '".$dados['curid']."' 
// 						-- and p.ncuid 	  = '".$dados['ncuid']."' 
// 						and p.modid 	  = '".$dados['modid']."' 
// 						and p.pdiesfera = '".$esf."' 
// 						order by 1";
				
				$vagasprevistas = $db->pegaUm($sql);

				$html_col[] = "<td>".(($vagasprevistas)?$vagasprevistas:"0")."</td>";
			}
		}
		
		$html_lin[] = "<tr>".implode("",$html_col)."</tr>";
		unset($html_col);
		
	}
	
	echo "<table class=\"listagem\" width=\"100%\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr>".implode("",$html_tit)."</tr>";
	echo implode("",$html_lin);
	echo "</table>";
	
}

function demandaMunicipio($dados) {
	global $db;
	
	if($dados['curid']){
		$dados['modid'] = substr($dados['curid'], -1);
		$dados['curid'] = substr($dados['curid'], 0, -1);
	}
	
	$sql = "select	pcfid as codigo,
					pcfano as descricao
			from pdeinterativo.periodocursoformacao
			where pcfstatus = 'A' and pcfid in (".$dados['pcfid'].") 
			order by pcfano";
	
	$periodos = $db->carregar($sql);
	
	$html_tit[] = "<td width=10% class=\"SubtituloCentro\">REDE</td>";
	$html_tit[] = "<td width=25% class=\"SubtituloCentro\">Município</td>";
	
	if($periodos[0]) {
		$contador = 0;
		foreach($periodos as $per) {
			$html_tit[] = "<td class=\"SubtituloCentro\">Vagas demandadas (".$per['descricao'].")</td>";
			if( $per['descricao'] != '2012' ){
				$html_tit[] = "<td class=\"SubtituloCentro\">Vagas Atendidas (".$per['descricao'].")</td>";
			}
			$contador++;
		}
	}
	
	if($periodos[0]) {
		foreach($periodos as $per) {
			
			$sql = "select p.pcfano, p.pdiesfera, m.muncod||'_'||m.mundescricao as mundescricao, coalesce(sum(p.privagasprevistas),0) as vagas  
					from snf.prioridadecursoescola p
					inner join territorios.municipio m using(muncod)
					where p.estuf = '".$dados['estuf']."'
					and p.pristatus = 'A'
					and p.pcfid = '".$per['codigo']."'
					and p.ateid = '".$dados['ateid']."'
					and p.curid = '".$dados['curid']."'
					-- and p.ncuid = '".$dados['ncuid']."'
					and p.modid = '".$dados['modid']."'
					group by p.pcfano, p.pdiesfera, m.mundescricao, m.muncod
					having coalesce(sum(p.privagasprevistas),0)  <> 0
					order by 1, 4";
						
			$linhas = $db->carregar($sql);
			// echo '<pre>';print_r($linhas);echo '</pre>';exit;
			
			if($linhas[0]) {
				foreach($linhas as $linha) {
					$dados_ac[$linha['pdiesfera']][$linha['mundescricao']][$linha['pcfano']] = $linha['vagas'];
				}
			}
			// echo '<pre>';print_r($dados_ac);echo '</pre>';exit;
		}
	}

	// echo '<pre>';print_r($periodos);echo '</pre>';exit;
	if( $dados_ac ) {
		foreach($dados_ac as $esfera => $l) {
			$municipio_pri = key($l);
			$munparts = explode("_",$municipio_pri);
			$muncod 	  = $munparts[0];
			$mundescricao = $munparts[1];
			$html_col[] = "<td rowspan=".count($l)." style='vertical-align:top;'>".$esfera."</td>";
			$html_col[] = "<td>".$mundescricao."</td>";
			if($periodos[0]) {
				foreach($periodos as $per) {

					if( $l[$municipio_pri][$per['descricao']] ){
						$if1 = 'style="color:blue"';
						$if2 = $l[$municipio_pri][$per['descricao']];
					}else{
						$if1 = '0';
						$if2 = '0';
					}

					$html_col[] = "<td ". $if1 .">". $if2 ."</td>";
					$qtd = $db->pegaUm("select sum(ateqtde) from snf.atendimento where curid='".$dados['curid']."' and ateano='".$per['descricao']."'   and modid = '" .$dados['modid']. "' and muncod='".$muncod."'");
					if( $qtd ){
						$stylaTd = 'style="color:blue"';
						$valorTd = $qtd;
					}else{
						$styleTd = "0";
						$valorTd = '0';
					}
					if( $per['descricao'] != 2012 ) // retirando vagas atendidas de 2012
						$html_col[] = "<td ". $stylaTd .">". $valorTd ."</td>";

				}
			}

			$html_lin[] = "<tr>".implode("",$html_col)."</tr>";
			// echo '<pre>';print_r($html_lin);echo '</pre>';exit;
			unset($html_col,$l[$municipio_pri]);
			if($l) {
				// echo '<pre>';print_r(array_keys($l));echo '</pre>';exit;
				foreach(array_keys($l) as $municipio) {
					$munparts = explode("_",$municipio);
					$muncod 	  = $munparts[0];
					$mundescricao = $munparts[1];
					
					$html_col[] = "<td>".$mundescricao."</td>";
					if($periodos[0]) {
						foreach($periodos as $per) {

							if( $l[$municipio][$per['descricao']] ){
								$if1 = "style='color:blue'";
								$if2 = $l[$municipio][$per['descricao']];
							}else{
								$if1 = "0";
								$if2 = "0";
							}
							$html_col[] = "<td ". $if1 .">". $if2 ."</td>";

							$sql = "select sum(ateqtde) from snf.atendimento where curid='".$dados['curid']."' and ateano='".$per['descricao']."' and modid = '" .$dados['modid']. "' and muncod='".$muncod."' ";
							$qtd = $db->pegaUm( $sql );
							if( $qtd ){
								$styleTd = 'style="color:blue"';
								$valorTd = $qtd;
							}else{
								$styleTd = '';
								$valorTd = "0";
							}
							if( $per['descricao'] != 2012 ) // retirando vagas atendidas de 2012
								$html_col[] = "<td ". $styleTd .">". $valorTd ."</td>";
						
						}
					}
					// echo '<pre>';print_r($html_col);echo '</pre>';exit;
					$html_lin[] = "<tr>".implode("",$html_col)."</tr>";
					unset($html_col);
					
				}
			}
		}
	}
	// echo '<pre>';print_r($html_lin);echo '</pre>';exit;

	echo "<table class=\"listagem\" width=\"100%\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr>".implode("",$html_tit)."</tr>";
	echo (($html_lin)?implode("",$html_lin):"<tr><td colspan=".count($html_tit)." align=\"center\" style=\"color:#cc0000;\">Não foram encontrados Registros.</td></td>");
	echo "</table>";

}

function ofertaGeral($dados) {
	global $db;
	
	if($dados['curid']){
		$dados['modid'] = substr($dados['curid'], -1);
		$dados['curid'] = substr($dados['curid'], 0, -1);
	}
	
	$sql = "select	pcfano as descricao
			from pdeinterativo.periodocursoformacao
			where pcfstatus = 'A' and pcfid in (".$dados['pcfid'].") 
			order by pcfano";
	
	$anos = $db->carregarColuna($sql);
	
	
	$sql = "SELECT 
				id, 
				curid, 
				estuf, 
				muncod, 
				mundescricao, 
				pobdescricao, 
				sum(ofeqtde) as quantidade, 
				insnome, 
				dirnome, 
				ofeano,
				memnome
			FROM (
			SELECT 

                                           'estuf='||en.estuf||'&ofeano='||o.ofeano||'&curid='||o.curid||'&modid='||o.modid||'&pobid='||o.pobid as id, 
                                           o.curid, 
                                           o.modid, 
                                           o.ofeid, 
                                           en.estuf, 
                                           m.muncod, 
                                           m.mundescricao, 
                                           pu.pobdescricao, 
                                           o.ofeqtde, 
                                           ies.insnome, 
                                           dm.dirnome, 
                                           o.ofeano,
                                           mc.memnome
                                  FROM 
                                           snf.oferta o
                                  INNER JOIN snf.polouab pu USING (pobid)
                                  INNER JOIN snf.instituicaouab iu ON iu.pobid = pu.pobid
                                  INNER JOIN snf.instituicaoensino ies ON ies.insid = iu.insid
                                                                       AND ies.insid = o.insid
                                  INNER JOIN snf.dirigentemaximo dm ON ies.insid = dm.insid 
                                  									AND dm.dirstatus = 'A' 
                                  INNER JOIN snf.enderecopolo en ON en.endid = pu.endid
                                  INNER JOIN territorios.municipio m on m.muncod = en.muncod
							      INNER JOIN snf.comite co ON co.insid = ies.insid
							      INNER JOIN snf.membrocomite mc ON co.comid = mc.comid AND papid = 9  
                                  
				WHERE o.curid = '".$dados['curid']."' 
				  AND o.modid = '".$dados['modid']."' 
				  AND en.estuf = '".$dados['estuf']."' 
				  AND o.ofeano in('".implode("','",$anos)."')	
                                  
				
				UNION
				
				SELECT 
					'estuf='||en.estuf||'&ofeano='||o.ofeano||'&curid='||o.curid||'&modid='||o.modid||'&polid='||o.polid as id, 
					o.curid, 
					o.modid, 
					o.ofeid, 
					en.estuf, 
					m.muncod, 
					m.mundescricao, 
					pu.poldescricao, 
					o.ofeqtde, 
					ies.insnome, 
					dm.dirnome, 
					o.ofeano,
					mc.memnome
				FROM 
					snf.oferta o
				INNER JOIN snf.polo 				 pu USING(polid)
				INNER JOIN snf.instituicaoensino 	ies ON ies.insid = pu.insid
				INNER JOIN snf.dirigentemaximo 		 dm ON ies.insid = dm.insid
														AND dm.dirstatus = 'A'
				INNER JOIN snf.enderecopolo 		 en ON en.endid = pu.endid
				INNER JOIN territorios.municipio 	  m ON m.muncod = en.muncod
     		    INNER JOIN snf.comite co ON co.insid = ies.insid
     			INNER JOIN snf.membrocomite mc ON co.comid = mc.comid AND papid = 9  
				
				WHERE o.curid = '".$dados['curid']."' 
				  AND o.modid = '".$dados['modid']."' 
				  AND en.estuf = '".$dados['estuf']."' 
				  AND o.ofeano in('".implode("','",$anos)."')	

				UNION   
				
				SELECT 
					'estuf='||en.estuf||'&ofeano='||o.ofeano||'&curid='||o.curid||'&modid='||o.modid||'&cmpid='||o.cmpid as id,
					o.curid, 
					o.modid, 
					o.ofeid, 
					en.estuf, 
					m.muncod, 
					m.mundescricao, 
					ent.entnome as pobdescricao, 
					o.ofeqtde, 
					ies.insnome, 
					dm.dirnome, 
					o.ofeano,
					mc.memnome
				FROM 
					snf.oferta o
				INNER JOIN academico.campus           c ON o.cmpid = c.cmpid 
                INNER JOIN entidade.entidade        ent ON c.entid = ent.entid
                INNER JOIN entidade.funcaoentidade  fen ON fen.entid = ent.entid AND fen.funid = 18
                INNER JOIN entidade.funentassoc     fea ON fea.fueid = fen.fueid
                INNER JOIN entidade.entidade          I ON I.entid = fea.entid
                INNER JOIN entidade.funcaoentidade fies ON fies.entid = I.entid AND fies.funid in (11,12)
                INNER JOIN snf.instituicaoensino    ies ON ies.entid = fies.entid
                INNER JOIN snf.dirigentemaximo       dm ON ies.insid = dm.insid
                INNER JOIN entidade.endereco         en ON c.entid = en.entid 
                inner join territorios.municipio m on m.muncod = en.muncod
			    INNER JOIN snf.comite co ON co.insid = ies.insid
			    INNER JOIN snf.membrocomite mc ON co.comid = mc.comid AND papid = 9  
                
				WHERE o.curid = '".$dados['curid']."' 
				  AND o.modid = '".$dados['modid']."' 
				  AND en.estuf = '".$dados['estuf']."' 
				  AND o.ofeano in('".implode("','",$anos)."')	
                
                
			) AS OFERTAS
			GROUP BY 
				id, curid, estuf, muncod, mundescricao, pobdescricao, insnome, dirnome, ofeano, memnome";
	
	$preofertas = $db->carregar($sql);
	
	if($preofertas[0]) {
		foreach($preofertas as $po) {
			$agrup[$po['insnome']][$po['memnome']][$po['pobdescricao']][$po['mundescricao']][$po['ofeano']] = array("quantidade" => $po['quantidade'],"id" => $po['id']);
		}
	}
	
	if($agrup) {
		foreach($agrup as $ies => $arr1)
				foreach($arr1 as $responsavel => $arr2)
					foreach($arr2 as $campuspolounidade => $arr3)
						foreach($arr3 as $municipio => $arr4) {
							$rr['ies'] 		   		 = $ies;
							$rr['responsavel'] 		 = $responsavel;
							$rr['campuspolounidade'] = $campuspolounidade;
							$rr['municipio'] 		 = $municipio;
							if($anos) {
								foreach($anos as $ano) {
									$rr[$ano] = (($arr4[$ano]['quantidade'])?"<a style=cursor:pointer; onclick=gerenciarOferta('".$arr4[$ano]['id']."')>".$arr4[$ano]['quantidade']."</a>&nbsp;&nbsp;<a style=cursor:pointer; onclick=gerenciarOferta('".$arr4[$ano]['id']."')><img src='../imagens/editar_nome.gif'/></a>":"0");
								}
							}
							$registros[] = $rr;
							unset($rr);
						}
	}
	// echo '<pre>';print_r($registros);echo '</pre>';exit;
	
	$cabecalho = array("IES","Responsável","Campus/Polo/Unidade","Município");
	
	if($anos) {
		foreach($anos as $ano) {
			$cabecalho[] = $ano;
		}
	}
	
	$db->monta_lista_simples((($registros)?$registros:array()),$cabecalho,500,5,'N','100%',$par2);
	
}

function gerenciarOferta($dados) {
	global $db;
	
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<script>
			function gravaoferta() {
 				document.getElementById('formulario').submit();
			}
		  </script>";
	
	$anos = explode(",",$dados['ofeano']);
		
	if($dados['cmpid']) {
		
		$sql = "select I.entnome ||' / ' || ent.entnome as nome 
				from academico.campus c 
				inner join entidade.entidade ent on c.entid = ent.entid
				inner join entidade.endereco enc on enc.entid = c.entid
				inner join entidade.funcaoentidade fen on fen.entid = ent.entid	and fen.funid = 18
				inner join entidade.funentassoc fea on fea.fueid = fen.fueid
				inner join entidade.entidade I on I.entid = fea.entid
				inner join entidade.funcaoentidade fies	on fies.entid = I.entid	and fies.funid in (11,12) 
				where c.cmpid='".$dados['cmpid']."'";
		
		$campo = "Universidade / Campus";
		$nome  = $db->pegaUm($sql);
		
		$sql_resumo = "select ofeano, ofertadas, (ofertadas-utilizadas) as disponiveis from (
					   select o.ofeano, o.ofeqtde as ofertadas, COALESCE((select sum(ateqtde) from snf.atendimento where ateano=o.ofeano and cmpid=o.cmpid and curid=o.curid and modid=o.modid),0) as utilizadas
					   from snf.oferta o where o.curid='".$dados['curid']."' and o.modid='".$dados['modid']."' and o.cmpid='".$dados['cmpid']."' and o.ofeano in(".implode(",",$anos).")) foo";
		
	}
	
	if($dados['pobid']) {
		
		$sql = "select m.mapdescricao ||' / ' || c.pobdescricao as nome 
				from snf.polouab c 
				left join snf.mantenedorpolo m on m.mapid = c.mapid
				where c.pobid='".$dados['pobid']."'";
		
		$campo = "Polo UAB";
		$nome  = $db->pegaUm($sql);
		
		$sql_resumo = "select ofeano, ofertadas, (ofertadas-utilizadas) as disponiveis from (
					   select o.ofeano, o.ofeqtde as ofertadas, COALESCE((select sum(ateqtde) from snf.atendimento where ateano=o.ofeano and pobid=o.pobid and curid=o.curid and modid=o.modid),0) as utilizadas
					   from snf.oferta o where o.curid='".$dados['curid']."' and o.modid='".$dados['modid']."' and o.pobid='".$dados['pobid']."' and o.ofeano in(".implode(",",$anos).")) foo";
		
		
	}
	
	if($dados['polid']) {
		
		$sql = "select c.poldescricao as nome 
				from snf.polo c 
				where c.polid='".$dados['polid']."'";
		
		$campo = "Polo";
		$nome  = $db->pegaUm($sql);
		
		$sql_resumo = "select ofeano, ofertadas, (ofertadas-utilizadas) as disponiveis from (
					   select o.ofeano, o.ofeqtde as ofertadas, COALESCE((select sum(ateqtde) from snf.atendimento where ateano=o.ofeano and polid=o.polid and curid=o.curid and modid=o.modid),0) as utilizadas
					   from snf.oferta o where o.curid='".$dados['curid']."' and o.modid='".$dados['modid']."' and o.polid='".$dados['polid']."' and o.ofeano in(".implode(",",$anos).")) foo";
		// exit($sql_resumo);
		
	}
	
	
	$sql = "select curdesc as descricao from catalogocurso.curso where curstatus = 'A' and curid = '".$dados['curid']."' and modid = '".$dados['modid']."'";
	// exit($sql_resumo);
	$curdescricao = $db->pegaUm($sql);
	
	
	echo "<table class=\"listagem\" width=\"100%\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class=\"SubtituloDireita\">{$campo}:</td><td>{$nome}</td></tr>";
	echo "<tr><td class=\"SubtituloDireita\">Curso:</td><td>{$curdescricao}</td></tr>";
	
	echo "<tr>";
	echo "<td colspan=2>";
	
	if($sql_resumo){
		$db->monta_lista_simples($sql_resumo,array("Ano","Ofertadas","Disponíveis"),500,5,'N','100%',$par2);
	}
	
	echo "</td>";
	echo "</tr>";	
	
	
	echo "</table>";
	
	
	$sql = "select p.pdiesfera, m.muncod, m.mundescricao, coalesce(sum(p.privagasprevistas),0) as vagas  
						from snf.prioridadecursoescola p
						inner join territorios.municipio m using(muncod)
						where p.estuf = '".$dados['estuf']."'
						and p.pristatus = 'A'
						and p.curid = '".$dados['curid']."'
						and p.modid = '".$dados['modid']."'
						group by p.pdiesfera, m.muncod, m.mundescricao
						having coalesce(sum(p.privagasprevistas),0)  <> 0
						order by 1,2";
	
	$municipios = $db->carregar($sql);
	// echo '<pre>';print_r($municipios);echo '</pre>';exit;
	
	$cabecalho = array("Esfera","Município","Vagas demandadas");
	
	
	
	foreach($anos as $ano) {
		 $cabecalho[] = $ano;
	}
	
	if($municipios[0]) {
		foreach($municipios as $municipio) {
			$re['pdiesfera'] 	= $municipio['pdiesfera'];
			$re['mundescricao'] = $municipio['mundescricao'];
			$re['vagas'] = "<font style='color:#000'>".$municipio['vagas']."</font>";
			
			if($dados['cmpid']) $f = " AND cmpid='".$dados['cmpid']."'";
			if($dados['pobid']) $f = " AND pobid='".$dados['pobid']."'";
			if($dados['polid']) $f = " AND polid='".$dados['polid']."'";
			
			foreach($anos as $ano) {
				$qtd = $db->pegaUm("SELECT ateqtde FROM snf.atendimento WHERE curid='".$dados['curid']."' AND modid='".$dados['modid']."' AND ateano='".$ano."' AND muncod='".$municipio['muncod']."'".$f);
				$re[$ano] = "<center><input type=\"text\" style=\"text-align:;\" name=\"oferta[".$municipio['muncod']."][".$ano."]\" size=\"7\" maxlength=\"6\" value=\"".$qtd."\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onkeyup=\"this.value=mascaraglobal(\'######\',this.value);\" title=\"\" class=\" normal\"></center>";
			}
			
			$registros[] = $re;
			unset($re);
		}
	}
	// echo '<pre>';print_r($registros);echo '</pre>';exit;
	
	echo "<form name=formulario id=formulario method=post>";
	echo "<input type=hidden name=requisicao value=inserirOferta>";
	if($dados['cmpid']) echo "<input type=hidden name=cmpid value=".$dados['cmpid'].">";
	if($dados['pobid']) echo "<input type=hidden name=pobid value=".$dados['pobid'].">";
	if($dados['polid']) echo "<input type=hidden name=polid value=".$dados['polid'].">";
	
	
	$db->monta_lista_simples((($registros)?$registros:array()),$cabecalho,500,5,'N','100%',$par2);
	
	echo "<p align=center><input type=button name=ok value=Ok onclick=\"gravaoferta();\"></p>";
	
	echo "</form>";
	
}

function inserirOferta($dados) {
	global $db;
	
	if($dados['oferta']) {
		foreach($dados['oferta'] as $muncod => $arr1)
			foreach($arr1 as $ano => $qtd) {
				$db->executar("DELETE FROM snf.atendimento WHERE curid='".$dados['curid']."' AND modid='".$dados['modid']."' AND ateano='".$ano."' AND muncod='".$muncod."' ".(($dados['polid'])?"AND polid='".$dados['polid']."'":"")." ".(($dados['pobid'])?".pobid='".$dados['pobid']."'":"")." ".(($dados['cmpid'])?" AND cmpid='".$dados['cmpid']."'":"")."");
				$anos[$ano] = $ano;
				if($qtd) {
					
					$sql = "INSERT INTO snf.atendimento(polid, pobid, cmpid, curid, modid, ateqtde, muncod, ateano)
	  						VALUES (".(($dados['polid'])?$dados['polid']:"NULL").", ".(($dados['pobid'])?$dados['pobid']:"NULL").", ".(($dados['cmpid'])?$dados['cmpid']:"NULL").", '".$dados['curid']."', '".$dados['modid']."', '".$qtd."', '".$muncod."', '".$ano."');";
								
					$db->executar($sql);
					
				}
				
			}
			
	}
	
	$rollback = false;
	
	if($dados['cmpid']) {
		$sql_resumo = "select (ofertadas-utilizadas) as disponiveis from (
					   select o.ofeano, o.ofeqtde as ofertadas, COALESCE((select sum(ateqtde) from snf.atendimento where ateano=o.ofeano and cmpid=o.cmpid and curid=o.curid and modid=o.modid),0) as utilizadas
					   from snf.oferta o where o.curid='".$dados['curid']."' and o.modid='".$dados['modid']."' and o.cmpid='".$dados['cmpid']."' and o.ofeano in(".implode(",",$anos).")) foo";
		
		$disp = $db->carregarColuna($sql_resumo);
		
		if($disp) {
			foreach($disp as $di) {
				if($di < 0) $rollback = true;
			}
		}
	}
	
	if($dados['pobid']) {
		$sql_resumo = "select (ofertadas-utilizadas) as disponiveis from (
					   select o.ofeano, o.ofeqtde as ofertadas, COALESCE((select sum(ateqtde) from snf.atendimento where ateano=o.ofeano and pobid=o.pobid and curid=o.curid and modid=o.modid),0) as utilizadas
					   from snf.oferta o where o.curid='".$dados['curid']."' and o.modid='".$dados['modid']."' and o.pobid='".$dados['pobid']."' and o.ofeano in(".implode(",",$anos).")) foo";

		$disp = $db->carregarColuna($sql_resumo);
		
		if($disp) {
			foreach($disp as $di) {
				if($di < 0) $rollback = true;
			}
		}
		
	}
	
	if($dados['polid']) {
		$sql_resumo = "select (ofertadas-utilizadas) as disponiveis from (
					   select o.ofeano, o.ofeqtde as ofertadas, COALESCE((select sum(ateqtde) from snf.atendimento where ateano=o.ofeano and polid=o.polid and curid=o.curid and modid=o.modid),0) as utilizadas
					   from snf.oferta o where o.curid='".$dados['curid']."' and o.modid='".$dados['modid']."' and o.polid='".$dados['polid']."' and o.ofeano in(".implode(",",$anos).")) foo";
		
		$disp = $db->carregarColuna($sql_resumo);
		
		if($disp) {
			foreach($disp as $di) {
				if($di < 0) $rollback = true;
			}
		}
		
	}
	
	if($rollback) {
		$db->rollback();
		echo "<script>
				alert('Vagas insuficientes');
				window.location='snf.php?modulo=principal/programacaoAtendimento&acao=A&requisicao=gerenciarOferta&estuf=".$dados['estuf']."&ofeano=".implode(",",$anos)."&curid=".$dados['curid']."&modid=".$dados['modid']."".(($dados['polid'])?"&polid=".$dados['polid']:"")."".(($dados['cmpid'])?"&cmpid=".$dados['cmpid']:"")."".(($dados['pobid'])?"&pobid=".$dados['pobid']:"")."';
	      	  </script>";
		
	} else {
		$db->commit();
		echo "<script>
				alert('Gravado com sucesso');
				window.opener.carregar_demandaMunicipio();
				window.close();
	      	  </script>";
		
	}

	
	
}



if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
	
include  APPRAIZ."includes/cabecalho.inc";

echo "<br/>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

	$(document).ready(function(){
		$('.manual').click(function(){
			var  url="http://<?=$_SERVER[HTTP_HOST] ?>/snf/Documentos/lista_de_cursos.pdf";
			return windowOpen( url,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		});
	});

	function filtraCursos(ateid){
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/programacaoAtendimento&acao=A',
			   data: "requisicao=filtraCursos&ateid=" + ateid,
			   success: function(msg){
			   		jQuery("#td_curid").html(msg);
			   }
		});
	}
	
	function pesquisarProgramacaoAtendimento() {
		if(jQuery('#estuf').val()=='') {
			alert('Selecione um Estado');
			return false;
		}
		if(jQuery('#muncod').val()=='') {
			alert('Selecione um Município');
			return false;
		}
		if(jQuery('#ateid').val()=='') {
			alert('Selecione uma Área Temática');
			return false;
		}
		if(jQuery('#curid').val()=='') {
			alert('Selecione uma Cursos');
			return false;
		}
		if(jQuery('#pcfid').val()=='') {
			alert('Selecione um Período');
			return false;
		}
		/*
		if(jQuery('#modid').val()=='') {
			alert('Selecione uma Modalidade');
			return false;
		}
		*/
//		if(jQuery('#ncuid').val()=='') {
//			alert('Selecione um Núcleo');
//			return false;
//		}
		
		carregar_demandaTotal();
		carregar_demandaEsfera();
		carregar_demandaMunicipio();
		carregar_ofertaGeral();
		
		document.getElementById('tr_result').style.display='';
	}
	
	function carregar_demandaTotal() {
		jQuery("#box_1").html("Carregando...");
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/programacaoAtendimento&acao=A',
			   data: "requisicao=demandaTotal&estuf=" + jQuery('#estuf').val() +"&ateid=" + jQuery('#ateid').val() +"&curid=" + jQuery('#curid').val() +"&pcfid=" + jQuery('#pcfid').val() +"&modid=" + jQuery('#modid').val() +"&ncuid=" + jQuery('#ncuid').val(),
			   success: function(msg){
			   		jQuery("#box_1").html(msg);
			   }
		});
	}
	
	function carregar_demandaEsfera() {
		jQuery("#box_3").html("Carregando...");
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/programacaoAtendimento&acao=A',
			   data: "requisicao=demandaEsfera&estuf=" + jQuery('#estuf').val() +"&ateid=" + jQuery('#ateid').val() +"&curid=" + jQuery('#curid').val() +"&pcfid=" + jQuery('#pcfid').val() +"&modid=" + jQuery('#modid').val() +"&ncuid=" + jQuery('#ncuid').val(),
			   success: function(msg){
			   		jQuery("#box_3").html(msg);
			   }
		});
	}
	
	function carregar_demandaMunicipio() {
		jQuery("#box_4").html("Carregando...");
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/programacaoAtendimento&acao=A',
			   data: "requisicao=demandaMunicipio&estuf=" + jQuery('#estuf').val() +"&ateid=" + jQuery('#ateid').val() +"&curid=" + jQuery('#curid').val() +"&pcfid=" + jQuery('#pcfid').val() +"&modid=" + jQuery('#modid').val() +"&ncuid=" + jQuery('#ncuid').val(),
			   success: function(msg){
			   		jQuery("#box_4").html(msg);
			   }
		});
	}
	
	function carregar_ofertaGeral() {
		jQuery("#box_2").html("Carregando...");
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/programacaoAtendimento&acao=A',
			   data: "requisicao=ofertaGeral&estuf=" + jQuery('#estuf').val() +"&ateid=" + jQuery('#ateid').val() +"&curid=" + jQuery('#curid').val() +"&pcfid=" + jQuery('#pcfid').val() +"&modid=" + jQuery('#modid').val() +"&ncuid=" + jQuery('#ncuid').val(),
			   success: function(msg){
			   		jQuery("#box_2").html(msg);
			   }
		});
	}
	
	
	function gerenciarOferta(filtros) {
		window.open('snf.php?modulo=principal/programacaoAtendimento&acao=A&requisicao=gerenciarOferta&'+filtros,'Observações','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
	}



</script>
	
<?
/* montando o menu */
echo montarAbasArray(montaMenuAtendimentoForum(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
			<h1 class="notprint">
				Informações do
				Estado
				:
				<?=$db->pegaUm("select estdescricao||' - '||estuf from territorios.estado where estuf='".$_SESSION['snf']['estuf']."'"); ?>
			</h1>
		</th>
	</thead>

<tr>
	<td width="25%" class="SubtituloDireita" >Estado:</td>
	<td>
	<?php 
	
	$perfis = pegaPerfilGeral();
	
	if((in_array(PERFIL_FORUM_APROVACAO,$perfis) || in_array(PERFIL_FORUM_DISTRIBUICAO,$perfis))) {
		
		$sql = "SELECT DISTINCT estuf FROM snf.usuarioresponsabilidade WHERE pflcod IN('".PERFIL_FORUM_APROVACAO."','".PERFIL_FORUM_DISTRIBUICAO."') AND usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
		$estuf = $db->pegaUm($sql);
		
		$sql = "select	estuf as codigo,
						estdescricao as descricao
				from territorios.estado 
				where estuf='".$estuf."'
				order by estdescricao";
	
		$db->monta_combo("estuf",$sql,"N","Selecione...","","","","","S","estuf","",$estuf);
		
	} else {

		$sql = "select	estuf as codigo,
						estdescricao as descricao
				from territorios.estado
				order by estdescricao";
	
		$db->monta_combo("estuf",$sql,"S","Selecione...","","","","","S","estuf","");
		
	}

	?>
	</td>
</tr>
<tr>
	<td class="SubtituloDireita" >Área Temática:</td>
	<td>
	<?php 
	
	$sql = "select 
				ateid as codigo,
				atedesc as descricao
			from 
				catalogocurso.areatematica
			where 
				atestatus = 'A'
			order by 
				atedesc";

	$db->monta_combo("ateid",$sql,"S","Selecione...","filtraCursos","","","","S","ateid","",$ateid);
		
	?>
	</td>
</tr>
<tr>
	<td class="SubtituloDireita" >Cursos:</td>
	<td id="td_curid" >Selecione uma Área Temática</td>
</tr>
<tr>
	<td class="SubtituloDireita" >Período:</td>
	<td>
	<?
	
	$arrPeriodo = array(0 => array("codigo" => "1,2", "descricao" => "2012/2013"),
						1 => array("codigo" => "3"  , "descricao" => "2014"),
						2 => array("codigo" => "4"  , "descricao" => "2015"),
						3 => array("codigo" => "10" , "descricao" => "2016"));
						
	$db->monta_combo("pcfid",$arrPeriodo,"S","","","","","","S","pcfid","",$pcfid);
	
	?>
	<div style="float:right;cursor:pointer" title="Clique aqui para baixar." class="manual"><a><u>Lista de Cursos</u></a></div>
	</td>
</tr>
<!-- 
<tr>
	<td class="SubtituloDireita" >Modalidade:</td>
	<td>
	<?php
	/* 
	$sql = "select 
				modid as codigo,
				moddesc as descricao
			from 
				catalogocurso.modalidadecurso
			where 
				modstatus = 'A'
			order by 
				moddesc";
	 
	$db->monta_combo("modid",$sql,"S","Selecione...","","","","","S","modid","",$modid);
	*/
	?>
	</td>
</tr>
 -->
<!--  
<tr>
	<td class="SubtituloDireita" >Nível:</td>
	<td>
	<?php
	 
//	$sql = "select 
//				ncuid as codigo,
//				ncudesc as descricao
//			from 
//				catalogocurso.nivelcurso
//			where 
//				ncustatus = 'A'
//			order by 
//				ncudesc"; 
//	
//	$db->monta_combo("ncuid",$sql,"S","Selecione...","","","","","S","ncuid","",$ncuid);
	
	?>
		</td>
	</tr>
-->
	<tr>
		<td class="SubtituloDireita" colspan="2">
		<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarProgramacaoAtendimento()" />
		</td>
	</tr>
	<tr id="tr_result" style="display:none;">
		<td colspan="2">
		<p align="center">DEMANDA APURADA e PRÉ-OFERTA INFORMADA</p>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubtituloCentro" width="50%">DEMANDA (vagas)</td>
				<td class="SubtituloCentro" width="50%">PRÉ-OFERTA (vagas)</td>
			</tr>
			<tr>
				<td id="box_1"></td>
				<td rowspan="3" id="box_2" valign="top"></td>
			</tr>
			<tr>
				<td id="box_3"></td>
			</tr>
			<tr>
				<td id="box_4"></td>
			</tr>
		</table>
		</td>
	</tr>
</table>


