<?php

ini_set('memory_limit','1024M');

?>
<script type="text/javascript" src="/includes/prototype.js"></script>	

<?php

/*Função para montar lista com Agrupador e Links*/
function listaSituacao($id = "tabela_1", $sql, $titulo = null, $cabecalho = null, $sqlAgrupador = array(), $exibeSoma = "S", $link = array(), $arrOff = array() ){
	
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
	 	$tabela .= "<tr bgcolor=\"#e9e9e9\" onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
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
	 		
	 		if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']] || $d[$sqlAgrupador['agrupador']] == "0" ){
	 			
	 			if( $d[$sqlAgrupador['agrupador']] == "Não iniciado" && $id == "tabela_1" ){
	 				#Executa esta sql ("AGRUPADORA") quando o campo "que é o agrupador "esddsc" é null" --> Não iniciado	
	 				$sqlAg = "
						Select  us.unscodigo,
								und.unicod,
								Case When ed.esddsc is null 
									Then 'Não Iniciado'
									Else ed.esddsc
								End as esddsc,
								trim(und.unidsc) as descricao
						From public.unidade und
						Join elabrev.unidadeordenadora o on o.unicod = und.unicod
						Left Join elabrev.wf_esplanada_sustentavel wes on wes.unicod = o.unicod
						Left Join workflow.documento d on d.docid = wes.docid
						Left Join workflow.estadodocumento ed on ed.esdid = d.esdid
						Left Join elabrev.unidadesustentavel us on cast(us.unscodigo as text) = und.unicod
						Where (esddsc is null or esddsc = '') and uniordstatus = 'A'
						Group by us.unscodigo, und.unicod, und.unidsc, ed.esddsc
						Order by 4
	 				";
				}else{
	 				$sqlAg = str_replace("|agrupador|"," = '".$d[$sqlAgrupador['agrupador']]."'", $sqlAg);
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
	 						
	 						#BLOCO INSERIDO PARA ADEQUAÇÃO AO SISTEMA. "DEPOIS DE IDENTIFICADO A INTIDADE, O LINK É MONTADO DE ACORDO COM O CASO"
	 						if($dA['unscodigo'] != ''){
	 							$sqlAgrupador['link'] = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel&acao=A';
	 						}else{
	 							$sqlAgrupador['link'] = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel_alunos&acao=A';
	 						}
	 						#FIM BLOCO.
	 						
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
							 		$listaAgrupada .= "<td align=\"left\"><span style=\"color:rgb(0, 102, 204);text-align:left\" >$seta_filho $linkAg_a $campo $linkAg_b</span></td>";
			 					}else{
							 		$listaAgrupada .= "<td align=\"left\" style=\"width:auto\">$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
							 	}
	 						}else{
	 							if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"left\" style=\"width:8%\"><span style=\"color:rgb(0, 102, 204);text-align:left;width:auto\" >$seta_filho $campo</span></td>";
			 					}else{
							 		$listaAgrupada .= "<td align=\"left\" style=\"width:8%\">$seta_filho {$dA[$kk[$ii]]}</td>";
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
						
			#Monta os links;
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

	//Monta o cabeçalho e título da tela
	include  APPRAIZ."includes/cabecalho.inc";
	
	echo"<br>";

	$titulo = "<a href=\"elabrev.php?modulo=principal/esplanadaSustentavel/listaUnidades&acao=C\">Projeto Esplanada Sustentável</a>";
		
	monta_titulo("Painel", $titulo);
?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td valign="top">
			<?php
				$perfil = array();
				$perfil = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
				
			    $sql = "
				    Select Case When ed.esddsc is null
				    			Then 'Não iniciado'
				    			Else ed.esddsc
				    		End as esddsc,				            
							count(*) as total_inst
					From elabrev.unidadeordenadora und
					Full Join elabrev.wf_esplanada_sustentavel wes on wes.unicod = und.unicod
					Left Join workflow.documento d on d.docid = wes.docid 
					Left Join workflow.estadodocumento ed on ed.esdid = d.esdid
					Where uniordstatus = 'A'
					Group by esddsc
					Order by 1			    
			    ";

				$sql_agp = "
					Select  und.unicod,
							us.unscodigo,
							Case When ed.esddsc is null 
								Then 'Não Iniciado'
								Else ed.esddsc
							End as esddsc,
							und.unidsc as descricao
					From public.unidade und
					Join elabrev.unidadeordenadora o on o.unicod = und.unicod
					Left Join elabrev.wf_esplanada_sustentavel wes on wes.unicod = o.unicod
					Left Join workflow.documento d on d.docid = wes.docid
					Left Join workflow.estadodocumento ed on ed.esdid = d.esdid
					Left Join elabrev.unidadesustentavel us on cast(us.unscodigo as text) = und.unicod
					where ed.esddsc |agrupador|
					Group by und.unicod, us.unscodigo, und.unidsc, ed.esddsc
					Order by 4
				";
				//$dados = $db->carregar($sql_agp); ver(1, $dados, $dados['unscodigo'], d);

				if(	in_array(PERFIL_SUPER_USUARIO, $perfil) || in_array(PERFIL_CONSULTA_ESPLANADA_SUSTENTAVEL, $perfil)){
						$sqlAgrupador = array(
								"sql" 		=> $sql_agp, 
								"agrupador" => "esddsc",
								"link" 		=> "elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel&acao=A",
								"campo" 	=> array("unicod", "descricao"),#Usado para definir os campos na URL do link.
								"get" 		=> array("unicod", "descricao"),#Usado para pegar os dados usados nos campos na URL do link.
								"arrOff" 	=> array("esddsc", "unscodigo"),#Ocuta o campo no grid.
								"exibeLink" => array("esddsc", "descricao"),#Campo que é exibido e criado o link.
						);
				}else{
					$sqlAgrupador = array(
							"sql" 		=> $sql_agp,
							"agrupador" => "esddsc",
							"link" => "elabrev.php?modulo=principal/esplanadaSustentavel/listaUnidades&acao=C",
							"campo" 	=> array(""),#Usado para definir os campos na URL do link.
							"get" 		=> array(""),#Usado para pegar os dados usados nos campos na URL do link.
							"arrOff" 	=> array("esddsc", "unscodigo"),#Ocuta o campo no grid.
							"exibeLink" => array("esddsc", "descricao"),#Campo que é exibido e criado o link.
					);	
				}
				$link = "";
				listaSituacao('tabela_1', $sql, '', $cabecalho, $sqlAgrupador, 'S', $link);
				
				?>
			</td>	
		</tr>
	</table>