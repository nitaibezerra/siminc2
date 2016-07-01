<?

pegaSessoes(1, 1, 0);
/***** constantes *******/
$tabela = "tabela_docente";
$clsid = 1;
$crgid = 323;

$perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);
	
//$habil = verificaPerfil($perfis);

if( $habil ){
    		$editavel = '';
    }else{
    $editavel = "disabled=\"false\"";
}


$autoriazacaoconcursos = new autoriazacaoconcursos();
$entidentidade	= $autoriazacaoconcursos->buscaentidade($entidcampus);
$edpideditalhomologacao = $_SESSION['academico']['edpideditalhomologacao'];
$prtid			= $_SESSION['academico']['prtid'];
$entidcampus 	= $_SESSION['academico']['entidcampus'];
$edpidhomo 		= $_SESSION['academico']['edpidhomo'];
$edpid 			= $_SESSION['academico']['edpid'];
$tpetipo		= $_SESSION["academico"]["tpetipo"];
$prgid 			= $_SESSION['academico']['prgid'];
$ano 			= $_SESSION['academico']['ano'];
$select 		= array();
$from   		= array();

$titulo = $tpetipo == ACA_TPEDITAL_NOMEACAO ? "Valor da nomeação maior do que de homologação!" : "Valor da homologação maior do que de publicação!";
$img = "<img align='middle' src='/imagens/atencao.png'/ title='{$titulo}' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";

if($tpetipo == ACA_TPEDITAL_HOMOLOGACAO){
	$display = "";
	$editavel_homo = "readonly=\"readonly\" style=\"color: 696969\"";		
}else{
	$editavel_homo = "";
	$display = "style='display:none;'";		
}
$display_efe = "style=\"display:none\"  disabled=\"disabled\"";
$editavel_nomeado = "";
//obetendo o total projetado
$sql_proj = "SELECT COALESCE (lnpvalor, 0) as lnpvalor
	FROM academico.lancamentosportaria
	WHERE
    prtid = ".$prtid." AND
    entidcampus = ".$entidcampus." AND
    entidentidade = ".$entidentidade." AND
    clsid = ".$clsid." AND
    lnpstatus = 'A'";
            
$projetado = $db->pegaUm($sql_proj); 

//total utilizado autorizado para concurso	   
		    
$sql_aut_proj = "
					SELECT 
					COALESCE (sum(lp.lepvlrpublicacao), 0) as lepvlrpublicacao
					
					FROM academico.lancamentoeditalportaria lp
					INNER JOIN 
						academico.cargos AS ca ON (ca.crgid  = lp.crgid)
					INNER JOIN 
						academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
					INNER JOIN 
						academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = ".ACA_TPEDITAL_PUBLICACAO." AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus AND eppub.edpidhomo IS null) 
					INNER JOIN 
						academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
					WHERE					
					lp.lepstatus = 'A' AND
					eppub.edpstatus = 'A' 
					-- AND lp.lepano = '$ano'";

$utilizado_projetado = $db->pegaUm($sql_aut_proj); 
$disponivel_projetado = $projetado - $utilizado_projetado;

//obetendo total nomeado/efetuvado
if($tpetipo == ACA_TPEDITAL_NOMEACAO){
	
	$edpid_homo = pegaHomologado($edpid);    	
    $edpid_pub  = pegaPublicado($edpid_homo);	
	
	$display = "";
	$display_efe = "";	
	    //total concurso
    	$sql_concurso = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
							FROM academico.lancamentosportaria lp
							INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
							WHERE 
							lp.prtid = ".$prtid." AND
							lp.entidcampus = ".$entidcampus." AND 
							lp.entidentidade = ".$entidentidade." AND
							lp.clsid =".$clsid." AND
							p.prtano = '".$ano."' AND
							p.tprid = ".ACA_TPORTARIA_CONCURSO." AND
							lp.lnpstatus = 'A'";
		$concurso = $db->pegaUm($sql_concurso);  
				
		//total provimento
		$sql_provimento = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
							FROM academico.lancamentosportaria lp
							INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
							WHERE 
							p.prtidautprov = $prtid AND
							lp.entidcampus = ".$entidcampus." AND 
							lp.entidentidade = ".$entidentidade." AND
							lp.clsid =".$clsid." AND
							p.prtano = '".$ano."' AND
							p.tprid = ".ACA_TPORTARIA_PROVIMENTO." AND
							lp.lnpstatus = 'A'";
		$provimento = $db->pegaUm($sql_provimento);     	
		
		//total utilizado para efetivação		
	   $sql_nomeado = "
				    	SELECT
							COALESCE (sum(lp.lepvlrprovefetivados), 0) as lepvlrprovefetivados						
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lp ON lp.edpid = ep.edpid 
												 AND lp.lepstatus = 'A'
												-- AND lp.lepano = '$ano'
						INNER JOIN  
							academico.cargos c on c.crgid = lp.crgid						 
						WHERE
							c.clsid = ".$clsid."
							AND edpideditalhomologacao = $edpid_homo
							AND edpstatus = 'A' ";	
	    
		$nomeado = $db->pegaUm($sql_nomeado); 
		//disponivel para efetivação
		$disponivel_nomeacao = $provimento - $nomeado;
		
    	$total_nomeado = "<tr>
    					   <input type=\"hidden\" id=\"concurso_autorizado_".$tabela."\"  value=\"".$concurso."\">
    					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
						  <td>".$concurso."</td>
						  </tr>
						  <input type=\"hidden\" id=\"provimento_autorizado_".$tabela."\"  value=\"".$provimento."\">					     
    					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Autorizado:</b></td>
						  <td>".$provimento."</td>
						  </tr>
						  <tr>						  
    					  <td  width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Efetivado:</b></td>
						  <td id=\"td_provimento_efetivado_".$tabela."\">".$nomeado."</td>
						  </tr>
						  <tr>						 
    					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Efetivação:</b></td>
						 <td id=\"td_disponivel_efetivacao_".$tabela."\">".$disponivel_nomeacao."</td>
						 </tr>";   
	
	$editavel_nomeado = "readonly=\"readonly\" style=\"color: 696969\"";	
	   	
}

    	
if($tpetipo == ACA_TPEDITAL_NOMEACAO){
		    	echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
					<thead>
						<input type=\"hidden\" id=\"td_disponivel_projetado_".$tabela."\"  value=\"0\">
						<input type=\"hidden\" id=\"projetado_".$tabela."\"  value=\"".$projetado."\">
						<input type=\"hidden\" id=\"disponivel_projetado_".$tabela."\"  value=\"".$disponivel_projetado."\">
						".$total_nomeado."			
					</table>
				";
		    }else{
		    	echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
					<thead>
						<tr>
						<input type=\"hidden\" id=\"projetado_".$tabela."\"  value=\"".$projetado."\">
						<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
						<td>".$projetado ."</td>																
						</tr>	
						<tr>
						<input type=\"hidden\" id=\"disponivel_projetado_".$tabela."\"  value=\"".$disponivel_projetado."\">
						<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Publicação:</b></td>
						<td id=\"td_disponivel_projetado_".$tabela."\">".$disponivel_projetado ."</td>		
						</tr>		
					</table>
				";		    	
		    }

?>
<table id="<?=$tabela?>" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td  width="4%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Ação</strong></td>		            		
					<td  width="46%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Cargo</strong></td>
					<td style='display:none;' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Concursos Autorizados</strong></td>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Concursos Publicados</strong></td>
					<td <?=$display ?> align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Concursos Homologados</strong></td>
					<td style='display:none;'  width="14%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Provimentos Autorizados</strong></td>
					<td <?=$display_efe ?>  width="14%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Provimentos Efetivados</strong></td>
					<td style='display:none;'  width="14%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" ><strong>Provimentos Não Efetivados</strong></td>
				</tr>
			</thead>
		    <tbody>
		    <?php 
		    
if ($tpetipo == ACA_TPEDITAL_NOMEACAO){
    	$edpid_homo = pegaHomologado($edpid);    	
    	$edpid_pub  = pegaPublicado($edpid_homo);
    	
    	$sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					sum(efetivado) AS efetivado,
					cargo,
					crgid
				FROM (
				
						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							0 AS efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
												  AND lep.lepstatus = 'A'
						INNER JOIN 
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN 
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_pub
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano' 
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid
				
					UNION ALL 
				
						SELECT
							0 AS publicado,
							sum( lepvlrhomologado ) as homologado,
							0 as efetivado,
							ca.crgdsc as cargo,
							lep.crgid			
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
												  AND lep.lepstatus = 'A'
						INNER JOIN 
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN 
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_homo
							AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano' 
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid
				
					UNION ALL
				
						SELECT
							0 AS publicado,
							0 as homologado,
							sum(lepvlrprovefetivados) as efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
												  AND lep.lepstatus = 'A'
						INNER JOIN 
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN 
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_NOMEACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano' 
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid
				
				
				) as f
				GROUP BY
					cargo,
					crgid";
    	
		$display = "";
		$editavel_homo = "readonly=\"readonly\" style=\"color: #696969\""; 
		
		$display_efe = "";
		$editavel_efe = "readonly=\"readonly\" style=\"color: #696969\"";
		$display_publicacao = "style=\"display:none\"";		
		
    }elseif ($tpetipo == ACA_TPEDITAL_HOMOLOGACAO){
    	$edpid_pub  = pegaPublicado($edpid);   
    	$sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					sum(efetivado) AS efetivado,
					cargo,
					crgid
				FROM (
				
						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							0 AS efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
												  AND lep.lepstatus = 'A'
						INNER JOIN 
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN 
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_pub
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano' 
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid
				
					UNION ALL 
				
						SELECT
							0 AS publicado,
							sum( lepvlrhomologado ) as homologado,
							0 as efetivado,
							ca.crgdsc as cargo,
							lep.crgid			
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
												  AND lep.lepstatus = 'A'
						INNER JOIN 
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN 
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano' 
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid				
				
				) as f
				GROUP BY
					cargo,
					crgid";
    	
		$display = "style=\"display:block\"";
		$editavel_homo = "readonly=\"readonly\" style=\"color: #696969\"";
		$display_publicacao = "style=\"display:none\"";    			
    }else{
    	$sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					sum(efetivado) AS efetivado,
					cargo,
					crgid
				FROM (
				
						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							0 AS efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
												  AND lep.lepstatus = 'A'
						INNER JOIN 
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN 
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano' 
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid
							
				) as f
				GROUP BY
					cargo,
					crgid";     	
    	
		$display_publicacao = "";
    	
    }
    
$dados = $db->pegaLinha($sql);	
$publicado = $dados["publicado"] ? $dados["publicado"] : 0;
$homologado = $dados["homologado"];	
$efetivado = $dados["efetivado"];

$img_alert_homo = ($homologado > $publicado) ? $img : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';
$img_alert_efe = ($efetivado > $homologado )? $img : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';
$img_src 	= ($dados["lepobs"] != '') ? "src=\"/imagens/restricao.png \"" :"src=\"/imagens/pop_p.gif \"" ;

//recuperando o lepid para o teste de exclusão do lançamento
$sql_lepid = "SELECT lepid 
	    	FROM academico.lancamentoeditalportaria
 			WHERE edpid = $edpid 
 			AND crgid = ".$crgid." 
 			--AND lepano = '$ano'";
$lepid = $db->pegaUm($sql_lepid);

				$btexcluir = $habilitado ? "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '".$tabela."', '".$lepid."', '".$tpetipo."');\">"
	    				  			     : "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir_01.gif\" border=\"0\" title=\"Excluir\">";
	    			

    			echo "<tr id=\"".$tabela."_".$crgid."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
						
    					<td align=\"center\">
							".$btexcluir."
							<input type=\"hidden\" value=\"".$crgid."\" name=\"crgid[]\">
						</td>
    					
						<td align=\"left\"align=\"center\">						
							Docentes
						</td>
						<td align=\"center\">		
							<input type=\"hidden\" id=\"publicado_old_".$tabela."_".$crgid."\"  value=\"".$publicado."\">			
							<input $editavel_homo id=\"publicado_".$tabela."_".$crgid."\" class=\"CampoEstilo\" 
							onchange=\"valida_lancamento_docentes('".$tabela."', '".$crgid."');\"							
							name=\"publicado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\" 
							onkeyup=\"calculaTotal(this, 'total_pub_".$tabela."');\"
							size=\"15\" maxlength=\"15\" value=\"".$publicado."\">
						</td>
						<td align=\"center\" $display>		
							<input  type=\"hidden\" id=\"homologado_old_".$tabela."_".$crgid."\"  value=\"".$homologado."\">			
							<input ".$display."  $editavel_nomeado id=\"homologado_".$tabela."_".$crgid."\" class=\"CampoEstilo\" 
							onchange=\"valida_lancamento_docentes('".$tabela."', '".$crgid."');\"
							name=\"homologado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\" 
							size=\"15\" maxlength=\"15\" value=\"".$homologado."\">
							".$img_alert_homo."
						</td>
						<td style='display:none;' align=\"center\">					
							<input ".$editavel." id=\"autorizado_".$tabela."_".$crgid."\" class=\"CampoEstilo\" 
							onchange=\"valida_lancamento_docentes('".$tabela."', '".$crgid."');\"
							name=\"autorizado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\" 
							size=\"15\" maxlength=\"15\" value=\"".$autorizado."\">
						</td>
						<td $display_efe align=\"center\">	
							<input type=\"hidden\" id=\"efetivado_old_".$tabela."_".$crgid."\"  value=\"".$efetivado."\">				
							<input $display_efe id=\"efetivado_".$tabela."_".$crgid."\" class=\"CampoEstilo\" 
							onchange=\"valida_lancamento_docentes('".$tabela."', '".$crgid."');\"
							name=\"efetivado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\" 
							size=\"15\" maxlength=\"15\" value=\"".$efetivado."\">
							".$img_alert_efe."
						</td>
						<td style='display:none;' align=\"center\">					
							<input type=\"hidden\" id=\"nefetivado_obs_".$tabela."_".$crgid."\" name=\"obs_nefetivado[]\" value=\"".$dados["lepobs"]."\">
							<input disabled=\"false\" id=\"nefetivado_".$tabela."_".$crgid."\" class=\"CampoEstilo\" 
							onchange=\"valida_lancamento_docentes('".$tabela."', '".$crgid."');\"
							name=\"nefetivado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\" 
							size=\"15\" maxlength=\"15\" value=\"".$nefeticado."\">
							<img style=\"cursor: pointer;\" ".$img_src." border=0 onclick=\"cadastrarObs('nefetivado_obs_".$tabela."_".$crgid."');\" title=\"Observação\">
						</td>
					</tr>";
		    ?>
    </tbody>
</table>