<?
function pegaQtdPorAnoETipo($dados){
	
	global $db;
	
	
	if( $dados['anoRel'] == 2013 ){
		$sql = "SELECT 
					count(mc.mceid), coalesce(ed.esddsc,'Não iniciada') 
				FROM minc.mcemaiscultura mc
				LEFT JOIN workflow.documento d using(docid)
				LEFT JOIN workflow.estadodocumento ed using(esdid)
				WHERE 
					mc.mcestatus = 'A'
					AND mc.mceanoreferencia = {$dados['anoRel']}
				GROUP BY
				 	ed.esddsc";
	}

	
	$qtd = $db->pegaUm($sql);
	return $qtd;
}

function montaPainel( $dados ){
	
	global $db;
	
	$ano = $dados['ano'];
	$anoAnterior = $dados['anoAnterior'];
	$anoMaximo = $db->pegaUm("select max(prsano) from pdeescola.programacaoexercicio");
	
 	$sql = "SELECT 
				CASE WHEN ed.esddsc is null then 'Não Iniciado' ELSE ed.esddsc END as esddsc, 
    			CASE WHEN ed.esdid IS NOT NULL THEN ed.esdid ELSE 9999999 END AS esdid, 
				count(mc.mceid)
			FROM minc.mcemaiscultura mc
			INNER JOIN entidade.entidade en using(entid)
			INNER JOIN entidade.endereco ec using(entid)
			LEFT JOIN workflow.documento d using(docid)
			LEFT JOIN workflow.estadodocumento ed using(esdid)
			WHERE 
				mc.mcestatus = 'A'
				  AND mc.mceanoreferencia = {$dados['anoRel']}
			GROUP BY
				 ed.esddsc,
				 ed.esdid";
				
	$sql2 = "SELECT  
				est.estdescricao as descricao,
				CASE WHEN d.esdid IS NOT NULL THEN d.esdid ELSE 9999999 END AS esdid, 
				est.estuf as estuf,
				count(*) as count
			FROM 
				minc.mcemaiscultura mc
			
			INNER JOIN entidade.endereco ent ON ent.entid = mc.entid
			INNER JOIN territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
			LEFT JOIN workflow.documento d on d.docid = mc.docid
			LEFT JOIN workflow.estadodocumento ed on ed.esdid=d.esdid
			WHERE	
				mc.mceanoreferencia = 2013 
				AND mc.mcemodalidadeensino = 'F'
				AND mc.mcestatus = 'A'
				AND d.esdid |agrupador|
			GROUP BY
				d.esdid,
				est.estuf,
				descricao
			ORDER BY
				descricao";
	
	$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "minc.php?modulo=principal/lista&acao=A&modalidade=F&mceanoreferencia=".$ano."", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
	$link = array("link" => "minc.php?modulo=principal/lista&acao=A&modalidade=F&mceanoreferencia=".$ano."", "campo" => "esddsc", "get" => "esdid");
	$perfis = pegaPerfilGeral();
	
	if( in_array(PERFIL_MINC_ADMINISTRADOR,$perfis)  || $db->testa_superuser() ){
		listaSituacaoPorUF($dados['nomeTabela'],$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
   	}
}

if( $_REQUEST['reqJson'] ){
	$dados = json_decode(str_replace('*','"',$_REQUEST['dados']),true);
	$_REQUEST['reqJson']($dados);
	die();
}

//ini_set('memory_limit','1024M');
//if( ( $_POST['act'] == 'listarUfByEstado') && ( $_POST['esdid'] != '' && ($_POST['esdid'] != 0)) ){
//	//echo listarEstadoAjax( $_POST['esdid'] );
//	$sql = "SELECT 
//				DISTINCT
//				estdescricao as descricao,
//				est.esdid as esdid,
//				t.estuf as estuf,
//				count(*) as count
//			FROM 
//				entidade.entidade e
//			INNER JOIN 
//				entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
//			LEFT JOIN 
//				pdeescola.pdeescola pde ON pde.entid = e.entid AND pde.pdeano = 2008
//			LEFT JOIN 
//				territorios.estado t ON upper(pde.pdeuf) = upper(t.estuf)
//			LEFT JOIN 
//				workflow.documento d ON d.docid = pde.docid
//			LEFT JOIN 
//				workflow.estadodocumento est ON est.esdid = d.esdid
//			WHERE
//				d.esdid  = {$_POST['esdid']}
//			AND
//				estdescricao IS NOT NULL
//			AND 
//				pde.pdepafretorno IS NULL
//			GROUP BY
//				est.esdid,
//				t.estuf,
//				descricao
//			ORDER BY
//				descricao";
//	 
//	$rsEstado = $db->carregar( $sql, null, 3600 ); 
//	
// 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
//	 if(!$rsEstado){
//	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
//	 	echo $tabela; 
//	 	return false;
//	 }else{
//	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
//	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
//	 		 ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
//	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
//	 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"minc.php?modulo=lista&acao=A&esdid=".$rsEstado[$i]['esdid']."&estuf=".$rsEstado[$i]['estuf']."\">".$rsEstado[$i]['descricao']."</a></td>";
//	 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
//	 		$tabela .= "</tr>"; 
//	 	}
//	 	$tabela .= "</table>";
//	 }
//	 echo $tabela;
//	die();
//}else if(( $_POST['act'] == 'listarUfByEstado') && ( $_POST['esdid'] == 0)){
//	//echo listarEstadoAjax( $_POST['esdid'] );
//	$sql = "SELECT 
//				est.estdescricao as descricao,
//				coalesce(ed.esdid,0) as esdid,
//				est.estuf as estuf,
//				count(*) as count
//			FROM 
//				entidade.entidade ent
//			LEFT JOIN
//				minc.mcemaiscultura mc  ON ent.entid = mc.entid
//			INNER JOIN 
//				entidade.entidadedetalhe det on det.entid = ent.entid
//			INNER JOIN 
//				entidade.endereco entEnd on det.entid = entEnd.entid
//			INNER JOIN
//				territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
//			LEFT JOIN 
//				workflow.documento d on d.docid = mc.docid
//			LEFT JOIN 
//				workflow.estadodocumento ed on ed.esdid = d.esdid
//			WHERE
//				d.esdid is null
//			AND
//				det.entpdeescola = 't'
//			GROUP BY
//				ed.esdid,
//				est.estuf,
//				descricao
//			ORDER BY
//				descricao";
//
//	$rsEstado = $db->carregar( $sql, null, 3600 ); 
//	
// 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
//	 if(!$rsEstado){
//	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
//	 	echo $tabela; 
//	 	return false;
//	 }else{
//	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
//	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
//	 		 ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
//	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
//	 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"minc.php?modulo=lista&acao=A&esdid=".$rsEstado[$i]['esdid']."&estuf=".$rsEstado[$i]['estuf']."\">".$rsEstado[$i]['descricao']."</a></td>";
//	 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
//	 		$tabela .= "</tr>"; 
//	 	}
//	 	$tabela .= "</table>";
//	 }
//	 echo $tabela;
//	die();	
//}

	
////function listarUfByEstado(){
////	global $db;
////	$sql = "(SELECT * FROM (
////				 (SELECT 
////				 	CASE  
////				 	WHEN est.esddsc IS NOT NULL THEN est.esddsc
////				       ELSE 'Não Iniciado'
////				    END AS esddsc,
////				    CASE   WHEN est.esdid IS NOT NULL THEN est.esdid
////				       ELSE 0
////				    END AS esdid,
////				    count(*) as count,
////				    est.esdordem
////				    FROM 
////				    	entidade.entidade e
////				    INNER JOIN entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
////				    LEFT JOIN minc.mcemaiscultura mc  ON ent.entid = mc.entid
////				    LEFT JOIN territorios.estado t ON upper(pde.pdeuf) = upper(t.estuf)
////				    LEFT JOIN workflow.documento d ON d.docid = mc.docid
////				    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
////				   
////				    inner JOIN pdeescola.preenchimento pr ON pr.entid = mc.entid
////
////				    WHERE
////					pr.entcodent is not null
////					AND est.esdid IS NULL
////					OR est.esdid != '90'
////					AND est.esdid != '87'
////
////  					
////				    GROUP BY est.esddsc, est.esdid, est.esdordem)
////				   UNION
////				  (SELECT 
////				  	CASE 
////				  	WHEN est.esddsc IS NOT NULL THEN est.esddsc
////				       ELSE 'Não Iniciado'
////				    END AS esddsc,
////				    CASE WHEN est.esdid IS NOT NULL THEN est.esdid
////				       ELSE 0
////				    END AS esdid,
////				    count(*) as count,
////				    est.esdordem
////				    FROM 
////				  		entidade.entidade e
////				    INNER JOIN entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
////				    LEFT JOIN minc.mcemaiscultura mc  ON ent.entid = mc.entid
////				    LEFT JOIN territorios.estado t ON upper(pde.pdeuf) = upper(t.estuf)
////				    LEFT JOIN workflow.documento d ON d.docid = mc.docid
////				    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
////
////				    inner JOIN pdeescola.preenchimento pr ON pr.entid = mc.entid AND est.esdid != '76'
////				    
////				  WHERE pr.esdid = '87' or est.esdid = '90' 
////				     GROUP BY est.esddsc, est.esdid, est.esdordem)
////		         ) as pp
////			ORDER BY pp.esdordem)";
////
////	$rsEstado = $db->carregar( $sql, null, 3600 ); 
////	$tabela .= '<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">';   
//// 	
//	 if(!$rsEstado){
//	 	$tabela = "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
//	 	echo $tabela; 
//	 	return false;
//	 }else{
//	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
//	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
//	 		$img = "<img onclick=\"exibeUfAjax('".$rsEstado[$i]['esdid']."')\" style=\"cursor:pointer; display:'';\" id=\"imgMais_".$rsEstado[$i]['esdid']."\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" /> 
//	 				<img onclick=\"escondeUfAjax('".$rsEstado[$i]['esdid']."')\" style=\"cursor:pointer; display:none \" id=\"imgMenos_".$rsEstado[$i]['esdid']."\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";				
//	 		($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
//	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
//	 		$tabela .= "	<td width =\"80%\"> ".$img."<a href=\"minc.php?modulo=lista&acao=A&esdid=".$rsEstado[$i]['esdid']."\">".$rsEstado[$i]['esddsc']."</a>";
//	 		$tabela .= "<div id='msg_".$rsEstado[$i]['esdid']."' style='display:none;'></div>"; 
//	 		$tabela.="</td>";
//	 		$tabela .= "	<td width =\"20%\" style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
//	 		$tabela .= "</tr>";
//	 		$tabela .= "<tr id=\"tr_".$rsEstado[$i]['esdid']."\" style=\"display:none;\" >
//	 						<td colspan=\"2\">
//	 						<div id=\"subitem_".$rsEstado[$i]['esdid']."\" ></div>
//	 						</td>
//	 					</tr>";
//	 	}
//	 	$tabela .= "</table>";
//	 }
//	 return $tabela;
//}
?>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script src="./js/meajax.js" type="text/javascript"></script>
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
		var req = new Ajax.Request('minc.php?modulo=painelmaicultura&acao=A',
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
	 //dbg($sql);
	 $dados = $db->carregar($sql, null, 3600);
 
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
	 				//$sqlAg = str_replace("|agrupador|"," is null ");
	 				
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								d.esdid as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								minc.mcemaiscultura mc
							
							INNER JOIN entidade.endereco ent ON ent.entid = mc.entid
							INNER JOIN territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
							LEFT JOIN workflow.documento d on d.docid = mc.docid
							LEFT JOIN workflow.estadodocumento ed on ed.esdid=d.esdid
							WHERE	
								mc.mceanoreferencia = 2013 
								AND mc.mcemodalidadeensino = 'F'
								AND mc.mcestatus = 'A'
								AND d.esdid is null
							GROUP BY
								d.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao
	 				";

	 			}
	 			else{
	 				
	 				if($d[$sqlAgrupador['agrupador']] == "0"||$d[$sqlAgrupador['agrupador']] == "9999999"){
	 					$sqlAg = str_replace("|agrupador|"," is null ",$sqlAg);
	 				}
	 				else{
	 					$sqlAg = str_replace("|agrupador|"," = ".$d[$sqlAgrupador['agrupador']],$sqlAg);
	 				}
	 			}
	 			//dbg($sqlAg);
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
			 					}
							 	else{
							 		$listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
							 	}
	 						}
	 						else{
	 							if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\" ><span style=\"color:rgb(0, 102, 204);text-align:right;width:100%\" >$seta_filho $campo</span></td>";
			 					}
							 	else{
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

if ( $_REQUEST["requisicao"] == 'cadastra' ){	
	$entid = pdeescola_pega_escola_atribuida($_SESSION["usucpf"]);

	if ( $entid ){
			
		if ( !pdeescola_possui_perfil(PDEESC_PERFIL_CAD_MAIS_EDUCACAO) ){
			
			// insere o usuário no perfil
			$sql = "insert into seguranca.perfilusuario (usucpf, pflcod) values
					('{$_SESSION["usucpf"]}',  " . PDEESC_PERFIL_CAD_MAIS_EDUCACAO. ")";	
			$db->executar($sql);
			$db->commit();
				
		}
		
		if ( !pdeescola_possui_escola_atribuida_cadastrador($_SESSION["usucpf"]) ){ 
			// cria a responsabilidade
			$sql = "insert into
						pdeescola.usuarioresponsabilidade 
						   (rpustatus,
						   	rpudata_inc,					   		
						   	entid,
						   	usucpf,
						   	pflcod 	
						   	)
						   	values
						   	('A',
						   	 now(),					   		
							 {$entid},
							 '{$_SESSION["usucpf"]}',
						   	 " . PDEESC_PERFIL_CAD_MAIS_EDUCACAO . ")";					   						 
		
			$db->executar($sql);
			$db->commit();
		}
		if( pdeescola_possui_perfil($arPerfilMaisEducacaoVeLista)) {
		
			?>
			<script>
				window.location.href = '/minc/minc.php?modulo=lista&acao=A';
			</script>							  	
			<?php
		} else {
			?>
			<script>
				redirecionaME('meajax.php', 'tipo=redirecioname&entid='+<?php echo $entid;?>);
			</script>							  	
			<?php
		}
	}else{ 
		$ano = $_SESSION["exercicio"];
		$anoAnterior = $ano -1;

			if ((($_GET['modalidade']) == 'M' && $_GET['memanoreferencia'] == $ano)) {?>
			    <script>
					window.location.href="/minc/minc.php?modulo=lista&acao=A&requisicao=cadastra&modalidade=M&mceanoreferencia=<?php echo $ano ?>";
				</script><?php 
			}elseif(( ($_GET['modalidade']) == 'F' && $_GET['memanoreferencia'] == $ano)){ ?>
				<script>
					window.location.href="/minc/minc.php?modulo=lista&acao=A&requisicao=cadastra&modalidade=F&mceanoreferencia=<?php echo $ano ?>";
				</script><?php 
			}elseif((($_GET['modalidade']) == 'M' && $_GET['memanoreferencia'] == $anoAnterior)){ ?>
				<script>
					window.location.href="/minc/minc.php?modulo=lista&acao=A&requisicao=cadastra&modalidade=M&mceanoreferencia=<?php echo $anoAnterior ?>";
				</script><?php
			}elseif((($_GET['modalidade']) == 'F'&& $_GET['memanoreferencia'] == $anoAnterior) ){ ?>
				<script>
					window.location.href="/minc/minc.php?modulo=lista&acao=A&requisicao=cadastra&modalidade=F&mceanoreferencia=<?php echo $anoAnterior ?>";
				</script><?php 				 		
			}else{?>
				<script>				
					window.location.href="/minc/minc.php?modulo=lista&acao=A&requisicao=cadastra";
				</script><?php
			}					
		}
}

//Monta o cabeçalho e título da tela
include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Painel", "Mais Cultura");

$ano = $_SESSION["exercicio"];
$anoAnterior = $ano -1;

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

jQuery.noConflict();

jQuery(document).ready(function() {
	jQuery('.carrega').click(function(){
		var idDiv = jQuery(this).attr('id');
		var dados = jQuery('#param_'+idDiv).val();
		if( jQuery('#linha_'+idDiv).html() == '' ){
			jQuery('#linha_'+idDiv).html('Carregando...');
			jQuery.ajax({
		   		type: "POST",
		   		url: 'minc.php?modulo=painelmaiscultura&acao=A',
		   		data: "reqJson=montaPainel&dados="+dados,
		   		async: false,
		   		success: function(retorno){
					jQuery('#linha_'+idDiv).html(retorno);
					jQuery('#linha_'+idDiv).slideDown("slow");
		   		}
		 	});
		}else{
			if( jQuery('#linha_'+idDiv).css('display') == 'none' ){
				jQuery('#linha_'+idDiv).slideDown("fast");
			}else{
				jQuery('#linha_'+idDiv).slideUp();
			}
		}
	});
});
</script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/minc/minc.php?modulo=painelmaiscultura&acao=A&requisicao=cadastra&modalidade=F&mceanoreferencia=<?php echo $_SESSION["exercicio"]?>'">Mais Cultura (Ano <?php echo $_SESSION["exercicio"];?>)</a>
		</th>
	</tr>
	<tr>
		<th>
			<div id="div_1" class="carrega" style="cursor:pointer">
			</div>
			<?
				//ano atual
				$ano = $_SESSION["exercicio"];
				$anoAnterior = $ano - 1;
				$dados['anoRel'] = $ano;
				$dados['ano'] = $ano;
				$dados['anoAnterior'] = $anoAnterior;
				$dados['nomeTabela'] = 'tabela_1';
				montaPainel( $dados );
			?>
		</th>
	</tr>
	<tr><td valign="top" id="linha_div_3" style="display:none"></td></tr>
	<tr>
		<th align="center">
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" >
				<tr bgcolor="DCDCDC" onmouseover="this.bgColor='#ffffcc'" onmouseout="this.bgColor='DCDCDC'">
					<td>
						<b>TOTAL GERAL:</b>
					</td>
					<td align="right" width="17%">
						<b>
							<?
							$sql = "SELECT
										count(mc.mceid)
									FROM minc.mcemaiscultura mc
									INNER JOIN entidade.endereco ent ON ent.entid = mc.entid
									WHERE mc.mcestatus = 'A'
										AND mc.mceanoreferencia = ".$_SESSION['exercicio']." ";
							$totalGeral = $db->pegaUm($sql);
							echo number_format($totalGeral,0,",",".");
							?>
						</b>
					</td>
				</tr>
			</table>
		</th>
	</tr>
	<tr><td valign="top" id="linha_div_6" style="display:none"></td></tr>
</table>