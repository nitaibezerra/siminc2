<?php

ini_set('memory_limit','1024M');

if( ($_POST['act'] == 'listarUfByEstado') && ($_POST['esdid'] != '' && ($_POST['esdid'] != 0)) ){
	
	$stWhere = '';
	if($_SESSION['estuf']){
		$stWhere = " AND t.estuf = '{$_SESSION['estuf']}' ";
	}

	$sql = "SELECT 
				DISTINCT
				estdescricao as descricao,
				est.esdid as esdid,
				t.estuf as estuf,
				count(*) as count
			FROM 
				entidade.entidade e
			INNER JOIN 
				entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
			LEFT JOIN 
				em.emiensinomedioinovador pde ON emi.entid = e.entid AND emi.pdeano = 2008
			LEFT JOIN 
				territorios.estado t ON upper(emi.pdeuf) = upper(t.estuf)
			LEFT JOIN 
				workflow.documento d ON d.docid = emi.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				d.esdid  = {$_POST['esdid']}
			AND
				estdescricao IS NOT NULL
			AND 
				emi.pdepafretorno IS NULL
			{$stWhere}
			GROUP BY
				est.esdid,
				t.estuf,
				descricao
			ORDER BY
				descricao";
	 
	$rsEstado = $db->carregar( $sql ); 
	
 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
	 if(!$rsEstado){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }else{
	 	
	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
	 		 ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"em.php?modulo=lista&acao=E&esdid=".$rsEstado[$i]['esdid']."&estuf=".$rsEstado[$i]['estuf']."\">".$rsEstado[$i]['descricao']."</a></td>";
	 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
	 		$tabela .= "</tr>"; 
	 	}
	 	$tabela .= "</table>";
	 }
	echo $tabela;
	die;
	
}else if(( $_POST['act'] == 'listarUfByEstado') && ( $_POST['esdid'] == 0)){

	$stWhere = '';
	if($_SESSION['estuf']){
		$stWhere = " AND est.estuf = '{$_SESSION['estuf']}' ";
	}
	
	$sql = "SELECT 
				est.estdescricao as descricao,
				coalesce(ed.esdid,0) as esdid,
				est.estuf as estuf,
				count(*) as count
			FROM 
				entidade.entidade ent
			LEFT JOIN
				em.emiensinomedioinovador emi ON ent.entid = emi.entid
			INNER JOIN 
				entidade.entidadedetalhe det on det.entid = ent.entid
			INNER JOIN 
				entidade.endereco entEnd on det.entid = entEnd.entid
			INNER JOIN
				territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
			LEFT JOIN 
				workflow.documento d on d.docid = emi.docid
			LEFT JOIN 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			WHERE
				d.esdid is null
			AND
				det.entpdeescola = 't'
			{$stWhere}
			GROUP BY
				ed.esdid,
				est.estuf,
				descricao
			ORDER BY
				descricao";

	$rsEstado = $db->carregar( $sql ); 
	
 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
	 if(!$rsEstado){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }else{
	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
	 		 ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"em.php?modulo=lista&acao=E&esdid=".$rsEstado[$i]['esdid']."&estuf=".$rsEstado[$i]['estuf']."\">".$rsEstado[$i]['descricao']."</a></td>";
	 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
	 		$tabela .= "</tr>"; 
	 	}
	 	$tabela .= "</table>";
	 }
	echo $tabela;
	die;	
}

	
function listarUfByEstado(){
	
	global $db;
	
	$stWhere = '';
	if($_SESSION['estuf']){
		$stWhere = " AND t.estuf = '{$_SESSION['estuf']}' ";
	}
	
	$sql = "(SELECT * FROM (
				 (SELECT 
				 	CASE  
				 	WHEN est.esddsc IS NOT NULL THEN est.esddsc
				       ELSE 'Não Iniciado'
				    END AS esddsc,
				    CASE   WHEN est.esdid IS NOT NULL THEN est.esdid
				       ELSE 0
				    END AS esdid,
				    count(*) as count,
				    est.esdordem
				    FROM 
				    	entidade.entidade e
				    INNER JOIN entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
				    LEFT JOIN em.emiensinomedioinovador pde ON emi.entid = e.entid AND emi.pdeano = 2008
				    LEFT JOIN territorios.estado t ON upper(emi.pdeuf) = upper(t.estuf)
				    LEFT JOIN workflow.documento d ON d.docid = emi.docid
				    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
				   
				    inner JOIN em.preenchimento pr ON pr.entid = emi.entid

				    WHERE
					pr.entcodent is not null
					AND est.esdid IS NULL
					OR est.esdid != '90'
					AND est.esdid != '87'
					{$stWhere}
  					
				    GROUP BY est.esddsc, est.esdid, est.esdordem)
				   UNION
				  (SELECT 
				  	CASE 
				  	WHEN est.esddsc IS NOT NULL THEN est.esddsc
				       ELSE 'Não Iniciado'
				    END AS esddsc,
				    CASE WHEN est.esdid IS NOT NULL THEN est.esdid
				       ELSE 0
				    END AS esdid,
				    count(*) as count,
				    est.esdordem
				    FROM 
				  		entidade.entidade e
				    INNER JOIN entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
				    LEFT JOIN em.emiensinomedioinovador pde ON emi.entid = e.entid AND emi.pdeano = 2008
				    LEFT JOIN territorios.estado t ON upper(emi.pdeuf) = upper(t.estuf)
				    LEFT JOIN workflow.documento d ON d.docid = emi.docid
				    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid

				    inner JOIN em.preenchimento pr ON pr.entid = emi.entid AND est.esdid != '76'
				    
				  WHERE pr.esdid = '87' or est.esdid = '90' 
				  {$stWhere}
				     GROUP BY est.esddsc, est.esdid, est.esdordem)
		         ) as pp
			ORDER BY pp.esdordem)";

	$rsEstado = $db->carregar( $sql ); 
	$tabela .= '<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">';   
 	
	 if(!$rsEstado){
	 	$tabela = "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }else{
	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
	 		$img = "<img onclick=\"exibeUfAjax('".$rsEstado[$i]['esdid']."')\" style=\"cursor:pointer; display:'';\" id=\"imgMais_".$rsEstado[$i]['esdid']."\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" /> 
	 				<img onclick=\"escondeUfAjax('".$rsEstado[$i]['esdid']."')\" style=\"cursor:pointer; display:none \" id=\"imgMenos_".$rsEstado[$i]['esdid']."\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";				
	 		($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 		$tabela .= "	<td width =\"80%\"> ".$img."<a href=\"em.php?modulo=lista&acao=E&esdid=".$rsEstado[$i]['esdid']."\">".$rsEstado[$i]['esddsc']."</a>";
	 		$tabela .= "<div id='msg_".$rsEstado[$i]['esdid']."' style='display:none;'></div>"; 
	 		$tabela.="</td>";
	 		$tabela .= "	<td width =\"20%\" style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
	 		$tabela .= "</tr>";
	 		$tabela .= "<tr id=\"tr_".$rsEstado[$i]['esdid']."\" style=\"display:none;\" >
	 						<td colspan=\"2\">
	 						<div id=\"subitem_".$rsEstado[$i]['esdid']."\" ></div>
	 						</td>
	 					</tr>";
	 	}
	 	$tabela .= "</table>";
	 }
	 return $tabela;
}
?>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<!-- script src="./js/meajax.js" type="text/javascript"></script -->
<script>

function carregando(id){	
	var msg = document.getElementById('msg_'+id);
	msg.style.display="block";
	msg.innerHTML="<img src=\"../imagens/wait.gif\"'>";
}
function exibeUfAjax(id){
	if( id == ''){ 
			return false;
		}
		var tr  = document.getElementById('tr_'+id); 
		var td  = document.getElementById('subitem_'+id); 
		var imgMais = document.getElementById('imgMais_'+id);
		var imgMenos= document.getElementById('imgMenos_'+id);
	 	var msg = document.getElementById('msg_'+id);
		var req = new Ajax.Request('em.php?modulo=painel&acao=A',
		                                               {
		                                                   method: 'post',
		                                                   parameters: 'esdid='+id+'&act=listarUfByEstado',
		                                                   onLoading: carregando(id),
		                                                   onComplete: function(res)
		                                                   {
		                                                   		td.innerHTML = res.responseText;        
		                                                   		tr.style.display='';
		                                                   		td.style.display=''; 
		                                                   		imgMais.style.display = 'none';
																imgMenos.style.display = ''; 
																msg.style.display ='none';
		                                                   }
		                                               }); 
}
function escondeUfAjax(id){
	var tr  = document.getElementById('tr_'+id); 
	var td  = document.getElementById('subitem_'+id); 
	var imgMais = document.getElementById('imgMais_'+id);
	var imgMenos= document.getElementById('imgMenos_'+id);
	tr.style.display='none';
	td.style.display='none'; 
	imgMais.style.display = '';
	imgMenos.style.display = 'none';
}
</script>
<?php

/*Função para montar lista com Agrupador e Links*/
function listaSituacaoPorUF($id = "tabela_1",$sql,$titulo = null,$cabecalho = null,$sqlAgrupador = array(),$exibeSoma = "S",$link = array(),$arrOff = array() ){
	
	 global $db;
	 $dados = $db->carregar($sql);
 
	 $tabela = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
	 
	 if(!$dados){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }

	 $num_colunas = count($dados[0]);
	 $num_colunas = $num_colunas - (count($arrOff));
	 
	 if($titulo){
	 	$tabela .= "<tr bgcolor=#CCCCCC ><td colspan=\"$num_colunas\" align=center ><b>$titulo</b></td></tr>";
	 }
	 
	 if($cabecalho){
	 	$tabela .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
		 $i = 0;
		 while($i < $num_colunas){
		 	$tabela .= "<td><b>".$cabecalho[$i]."</b></td>";
		 	$i++;
		 }
		 $tabela .= "</tr>";
	 }
	 $id_span = 1;
	 $i = 0;
	 foreach($dados as $d){
	 	
	 	$cor = ($i % 2) ? "#F7F7F7" : "#FCFCFC";
	 	
	 	$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 	
	 	$sqlAg = $sqlAgrupador['sql'];
	 	if($sqlAgrupador['sql']){
	 		
	 		if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']] || $d[$sqlAgrupador['agrupador']] == "0" || $d[$sqlAgrupador['agrupador']] == "999999"  || $d[$sqlAgrupador['agrupador']] == "888888"){
	 			
	 			if($d[$sqlAgrupador['agrupador']] == "0" && $id == "tabela_1"){
	 				
	 				$stWhere = '';
					if($_SESSION['estuf']){
						$stWhere = " AND est.estuf = '{$_SESSION['estuf']}' ";
					}
	 				
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"	
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1; 
									
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								entidade.entidade ent
							INNER JOIN
								em.emiensinomedioinovador emi ON ent.entid = emi.entid							
							INNER JOIN 
								entidade.endereco entEnd on ent.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = emi.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND emi.entcodent not in (select mem.entcodent from em.emiensinomedioinovador mem where mem.emianoreferencia = ".$anoAnterior." and mem.emistatus = 'A')
							AND emi.emianoreferencia = ".$ano."							
							AND emi.emistatus = 'A' 
							{$stWhere}
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";
	 			
	 			}elseif((($d[$sqlAgrupador['agrupador']] == "0") || ($d[$sqlAgrupador['agrupador']] == null)) && ($id == "tabela_2")){	 			
	 				
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
						 				
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								em.emiensinomedioinovador emi 
							INNER JOIN 
								em.emiensinomedioinovador me1 on me1.entid = emi.entid and me1.emianoreferencia = ".$anoAnterior." and me1.emistatus = 'A'							
							INNER JOIN 
								entidade.endereco entEnd on ent.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = emi.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND 
								emi.emianoreferencia = ".$ano."
							AND 
								emi.emistatus = 'A' 
							{$stWhere}
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";
 						
	 			}else{
	 				
	 				$sqlAg = str_replace("|agrupador|"," = ".$d[$sqlAgrupador['agrupador']],$sqlAg);
	 				
	 			}
	
	 			$dadosAgrupados = $db->carregar($sqlAg);

	 		}else{
	 			
	 			$dadosAgrupados = "";
	 			
	 		}
	 		
	 		$listaAgrupada = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';
	 		
	 		if(!$dadosAgrupados){
	 			$listaAgrupada .= "<tr><td><span style=\"color:#990000\" >Não existem registros.</span></td></tr>";
	 		}else{
	 			$xx = 0;
	 			foreach($dadosAgrupados as $dA){
	 				($xx % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
	 				$listaAgrupada .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

	 				foreach($dA as $k => $dd){
	 					$kk[] = $k;
	 				}
	 				$ii = 0;			
	 				while($ii < count($dA)){
	 					
	 					if($sqlAgrupador['link']){
	 						if($sqlAgrupador['campo']){
	 							if(is_array($sqlAgrupador['campo'])){
	 								
	 								unset($arrCampos);
	 								foreach($sqlAgrupador['get'] as $cmp){
	 									$arrCampos[] = "{$cmp}={$dA[$cmp]}";
	 									$campos = implode("&",$arrCampos);
	 								}
	 								
	 							}else{
	 								
	 								$campos = "{$sqlAgrupador['get']}={{$dA[$kk[$sqlAgrupador['get']]]}}"; 
	 							}
	 						}	 						
	 						$linkAg_a = "<a href=\"".$sqlAgrupador['link']."&".$campos."\" />"; 
	 						$linkAg_b = " </a>";
	 					}
	 					if($kk[$ii] == $kk[0]){
	 						$seta_filho = "<img src=\"../imagens/seta_filho.gif\" />";
	 					}else{
	 						$seta_filho = "";
	 					}

	 					if(!strstr($kk[$ii],"id") && !strstr($kk[$ii],"ordem") && !in_array($kk[$ii],$sqlAgrupador['arrOff'])){
	 					
	 						if(in_array($kk[$ii],$sqlAgrupador['exibeLink'])){
	 						
			 					if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\"><span style=\"color:rgb(0, 102, 204);text-align:right\" >$seta_filho $linkAg_a $campo $linkAg_b</span></td>";
			 					}else{
							 		$listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
							 	}
							 	
	 						}else{
	 							
	 							if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\" ><span style=\"color:rgb(0, 102, 204);text-align:right;width:100%\" >$seta_filho $campo</span></td>";
			 					}else{
							 		$listaAgrupada .= "<td>$seta_filho {$dA[$kk[$ii]]}</td>";
							 	}
	 						}
						 	
	 					}
						$ii++;
	 					
	 				}
	 				$listaAgrupada .= "</tr>";
	 			$xx++;
	 			}
	 		}
	 		$listaAgrupada .= "</table>";
	 	}
	 	
	 	$keys = array_keys($d);
	 	$j = 0;
		while($j < $num_colunas){
			if($sqlAgrupador && $keys[$j] == $keys[0] || $keys[$j] == $keys[999999] || $keys[$j] == $keys[888888] && $dadosAgrupados){
				$img = "<img onclick=\"exibeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer\" id=\"img_mais_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" />
						<img onclick=\"escondeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;display:none\" id=\"img_menos_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";
				$span = "<tr style=\"display:none\" bgcolor='#EEE9E9' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#EEE9E9'\" id=\"tr_view_{$id}_{$id_span}\"><td colspan=\"$num_colunas\">$listaAgrupada</td></td></tr>";	
				$id_span ++; 
			}
			else{
				$img = "&nbsp;&nbsp;&nbsp;&nbsp;";
			}
						
			//Monta os links;
			if($link && $dadosAgrupados){
				$link_a = "<a href=\"{$link['link']}&{$link['get']}=".$d[$link['get']]."\" >";
				$link_b = "</a>";
			}else{
				$link_a = "";
				$link_b = "";
			}
			
			if(!strstr($keys[$j],"id") && !strstr($keys[$j],"ordem") && !in_array($keys[$j],$arrOff)){
				
				if(is_numeric($d[$keys[$j]])){
					$tabela .= "<td align=\"right\">";
				}else{
					$tabela .= "<td>";
				}
				
				if($link['campo'] == $keys[$j]){
					$tabela .= $img.$link_a;
				}else{
					$tabela .= $img;
				}
			 	if(is_numeric($d[$keys[$j]])){
			 		$campo = str_replace(",",".",number_format($d[$keys[$j]]));
			 		$tabela .= "<span style=\"color:rgb(0, 102, 204)\" >".$campo.$link_b."</span></td>";
			 	}else{
				 	if($link['campo'] == $keys[$j]){
						$tabela .= $d[$keys[$j]].$link_b."</td>";
					}else{
						$tabela .= $d[$keys[$j]]."</td>";
					}
			 		
			 	}

			}
		 	
		 	if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
		 		$soma[$keys[$j]] += $d[$keys[$j]];
		 		$campo_soma[] = $keys[$j];
		 	}
		 	$j++;
		 	
		}
		
	 	$tabela .= "</tr>";
	 	$tabela .= $span;
	 	
	 	$i++;
	 }
	 
	 foreach($keys as $k => $k1){
	 	 if(strstr($k1,"id")){
	 	 	unset ($keys[$k]);
	 	 }
	 }
	 	 
	 //Exibe Soma
	 if($exibeSoma == "S"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 if($exibeSoma == "X"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 if($exibeSoma == "Y"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 	 
	 $tabela .= "</table>";
	 $tabela .="<script>
					 function exibeAgrupador(id){
					 	var img_mais = document.getElementById('img_mais_' +id);
					 	var img_menos = document.getElementById('img_menos_' +id);
					 	var tr_view = document.getElementById('tr_view_' +id);					 	
					 	img_mais.style.display = 'none';
					 	img_menos.style.display = '';
					 	tr_view.style.display = '';					 	
					 }					 
					 function escondeAgrupador(id){
					 	var img_mais = document.getElementById('img_mais_' +id);
					 	var img_menos = document.getElementById('img_menos_' +id);
					 	var tr_view = document.getElementById('tr_view_' +id);					 	
					 	img_mais.style.display = '';
					 	img_menos.style.display = 'none';
					 	tr_view.style.display = 'none';					 	
					 }	 
	 			</script>";
	 
	 echo $tabela;
}

if ($_REQUEST["requisicao"] == 'cadastra'){	
	
	$sql = "SELECT
				entid
			FROM em.usuarioresponsabilidade
			WHERE usucpf = '{$_SESSION["usucpf"]}' 
			AND rpustatus = 'A'";

	$entid = $db->pegaUm( $sql );
	
	if ( $entid ){
			
		if ( !checkPerfil(PERFIL_CADASTRADOR) ){
			
			// insere o usuário no perfil
			$sql = "insert into seguranca.perfilusuario (usucpf, pflcod) values
					('{$_SESSION["usucpf"]}',  " . PERFIL_CADASTRADOR. ")";	
			$db->executar($sql);
			$db->commit();
				
		}
		
		$sql = $sql." AND pflcod = ".PERFIL_CADASTRADOR." ";
			
		$entid = $db->pegaUm( $sql );
		
		if ( !$entid ){ 
			
			// cria a responsabilidade
			$sql = "insert into em.usuarioresponsabilidade 
						(rpustatus,rpudata_inc,entid,usucpf,pflcod)
					values
						('A',now(),{$entid},'{$_SESSION["usucpf"]}'," . PERFIL_CADASTRADOR . ")";					   						 
		
			$db->executar($sql);
			$db->commit();
		}
				
	}else{
		 
		$ano = $_SESSION["exercicio"];
		$anoAnterior = $ano -1;

			if ((($_GET['modalidade']) == 'M' && $_GET['emianoreferencia'] == $ano)) {?>
			    <script>
					window.location.href="/em/em.php?modulo=lista&acao=E&requisicao=cadastra&modalidade=M&emianoreferencia=<?php echo $ano ?>";
				</script><?php 
			}elseif(( ($_GET['modalidade']) == 'F' && $_GET['emianoreferencia'] == $ano)){ ?>
				<script>
					window.location.href="/em/em.php?modulo=lista&acao=E&requisicao=cadastra&modalidade=F&emianoreferencia=<?php echo $ano ?>";
				</script><?php 
			}elseif((($_GET['modalidade']) == 'M' && $_GET['emianoreferencia'] == $anoAnterior)){ ?>
				<script>
					window.location.href="/em/em.php?modulo=lista&acao=E&requisicao=cadastra&modalidade=M&emianoreferencia=<?php echo $anoAnterior ?>";
				</script><?php
			}elseif((($_GET['modalidade']) == 'F'&& $_GET['emianoreferencia'] == $anoAnterior) ){ ?>
				<script>
					window.location.href="/em/em.php?modulo=lista&acao=E&requisicao=cadastra&modalidade=F&emianoreferencia=<?php echo $anoAnterior ?>";
				</script><?php 				 		
			}else{?>
				<script>				
					window.location.href="/em/em.php?modulo=lista&acao=E&requisicao=cadastra";
				</script><?php
			}					
		}
}

//Monta o cabeçalho e título da tela
include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Painel", "Ensino Médio Inovador");

$ano = $_SESSION["exercicio"];
$anoAnterior = $ano -1;

?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
		<tr>
		 <th>
		 	<a style="cursor:pointer;" onclick="window.location.href='/em/em.php?modulo=painel&acao=A&requisicao=cadastra&modalidade=M&emianoreferencia=<?php echo $ano;?>'">Ensino Médio - Ano <?php echo $ano;?></a>
		 </th>
		</tr>
		<tr>
			<td valign="top">
				<?php
				
					$stWhere = '';
					if($_SESSION['estuf']){
						$stWhere = " AND est.estuf = '{$_SESSION['estuf']}' ";
					}
				
					$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
				
				    $sql = "SELECT CASE 
				    					when ed.esddsc is null then 'Não Iniciado' 
				    					else ed.esddsc 
				    				end as esddsc, 
				    				CASE
				                          WHEN ed.esdid is null THEN 0
				                          ELSE ed.esdid
				                         END AS esdid, 
				                    count(*) 
							FROM  
								em.emiensinomedioinovador m
							INNER JOIN
								entidade.endereco ent ON ent.entid = m.entid
							INNER JOIN
								territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid=m.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid=d.esdid 
							WHERE 
								m.entcodent not in (select mem.entcodent from em.emiensinomedioinovador mem where mem.emianoreferencia = ".$anoAnterior." and mem.emistatus = 'A')
							AND 
								m.emianoreferencia = ".$ano."
							AND 
							    m.emistatus = 'A'
							{$stWhere}
							GROUP BY 
								ed.esddsc, ed.esdordem, ed.esdid
							ORDER BY 
								ed.esdordem
							";

					$sql2 = "SELECT 
								est.estdescricao as descricao,
								d.esdid as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								em.emiensinomedioinovador emi
							INNER JOIN
								entidade.endereco ent ON ent.entid = emi.entid
							INNER JOIN
								territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid=emi.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid=d.esdid
							WHERE
								d.esdid |agrupador|
							AND 
								emi.entcodent not in (select mem.entcodent from em.emiensinomedioinovador mem where mem.emianoreferencia = ".$anoAnterior." and mem.emistatus = 'A')
							AND 
								emi.emianoreferencia = ".$ano."
							AND 
								emi.emistatus = 'A'
							{$stWhere}
							GROUP BY
								d.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";

					$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "em.php?modulo=lista&acao=E&modalidade=M", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
					
					$link = array("link" => "em.php?modulo=lista&acao=E&modalidade=M&emianoreferencia=".$ano."", "campo" => "esddsc", "get" => "esdid");
					if( checkPerfil(PERFIL_ADMINISTRADOR)  || $db->testa_superuser() ){
						listaSituacaoPorUF('tabela_1',$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
				   	}
				?>
			</td>	
		</tr>
		<tr>
		 <th>
		 	<a style="cursor:pointer;" onclick="window.location.href='/em/em.php?modulo=painel&acao=A&requisicao=cadastra&modalidade=M&emianoreferencia=<?php echo $anoAnterior;?>'">Ensino Médio - Ano <?php echo $anoAnterior;?></a>
		 </th>
		</tr>
		<tr>
			<td valign="top">
				<?php

				    $ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
				    $sql = "SELECT CASE 
				    					when ed.esddsc is null then 'Não Iniciado' 
				    					else ed.esddsc 
				    				end as esddsc, 
				    				CASE
				                          WHEN ed.esdid is null THEN 0
				                          ELSE ed.esdid
				                         END AS esdid, 
				                    count(*) 
							FROM  
								em.emiensinomedioinovador m
							INNER JOIN 
									em.emiensinomedioinovador me1 on me1.entid = m.entid and me1.emianoreferencia = ".$anoAnterior." and me1.emistatus = 'A'
							INNER JOIN
								entidade.endereco ent ON ent.entid = m.entid
							INNER JOIN
								territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = m.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid 
							WHERE 
								m.emianoreferencia = ".$ano."
							AND 
								m.emistatus = 'A'
							{$stWhere}
							GROUP BY 
								ed.esddsc, ed.esdordem, ed.esdid
							ORDER BY 
								ed.esdordem
							";

					$sql2 = "SELECT 
								est.estdescricao as descricao,
								d.esdid as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								em.emiensinomedioinovador emi
							INNER JOIN 
									em.emiensinomedioinovador me1 on me1.entid = emi.entid and me1.emianoreferencia = ".$anoAnterior." and me1.emistatus = 'A'								
							INNER JOIN
								entidade.endereco ent ON ent.entid = emi.entid
							INNER JOIN
								territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid=emi.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid=d.esdid
							WHERE
								d.esdid |agrupador|
							AND 
								emi.emianoreferencia = ".$ano."
							AND 
								emi.emistatus = 'A'
							{$stWhere}
							GROUP BY
								d.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";
					
					$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "em.php?modulo=lista&acao=E&modalidade=M", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
					
					$link = array("link" => "em.php?modulo=lista&acao=E&modalidade=M&emianoreferencia=".$anoAnterior."", "campo" => "esddsc", "get" => "esdid");

					if( checkPerfil(PERFIL_SEC_ESTADUAL, PERFIL_CONSULTA) || $db->testa_superuser()){
						listaSituacaoPorUF('tabela_2',$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
				   	}
				?>
			</td>	
		</tr>
	</table>