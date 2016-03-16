<?php
pegaSessoes(1,1,0);
$tabela 	= $_REQUEST['tab'];
$clsid 		= $_REQUEST['clsid'];
$orgid 		= $_SESSION['academico']['orgid']; 
$tpetipo 	= $_SESSION["academico"]["tpetipo"];
$edpid 		= $_SESSION['academico']['edpid'];	
$entid 		= $_SESSION['academico']['entid'];	
$edpidhomo 	= $_SESSION['academico']['edpidhomo'];		
$prtid 		= $_SESSION['academico']['prtid'];
$prgid 		= $_SESSION['academico']['prgid'];
$entidcampus= $_SESSION['academico']['entidcampus'];
$ano 		= $_SESSION['academico']['ano'];

function montaPopupCargos(){
	global $clsid, $tabela, $prtid, $edpid , $entid, $edpidhomo, $tpetipo, $ano, $entidcampus, $prgid, $orgid;
	global $db;	

	if( $tpetipo == ACA_TPEDITAL_HOMOLOGACAO ){		
			
		$edpidhomo = $edpidhomo ? $edpidhomo : $edpid;
		
		$sql = "SELECT
					COALESCE (lp.lepvlrpublicacao, 0)as publicado,
					ca.crgdsc as cargo,
					lp.crgid as codigo,
					COALESCE (prj.prjnumero, 0) as projetado,
					COALESCE ((SELECT
								sum(lp2.lepvlrhomologado) as homologado						 
								FROM
									academico.lancamentoeditalportaria AS lp2
								WHERE
									lp2.edpid = $edpid AND
									lp.crgid = lp2.crgid
								), 0)as homologado
				FROM
					academico.lancamentoeditalportaria AS lp
				LEFT JOIN 
					academico.editalportaria AS ep ON (ep.edpid = lp.edpid AND lp.edpid = $edpidhomo)
				LEFT JOIN 
					academico.portarias AS p ON (p.prtid = ep.prtid and p.prtid = $prtid)
				LEFT JOIN 
					academico.cargos AS ca ON (ca.crgid  = lp.crgid)
				INNER JOIN 
					academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
				LEFT JOIN 
					(select distinct crgid, prjano,  sum(prjnumero) as prjnumero from academico.projetados where entidcampus = $entidcampus and prgid = $prgid group by crgid, prjano) as prj ON prj.crgid = lp.crgid
				WHERE
					lp.lepstatus = 'A' AND
					prj.prjano = '$ano'
				ORDER BY 
					ca.crgdsc ";	
		$dados = $db->carregar($sql);			
	}else{
		if($orgid  == ACA_ORGAO_TECNICO){
			$sql = "SELECT	
					 '0' as publicado,       
			        ca.crgdsc as cargo,
			        ca.crgid as codigo,
			        '0' as projetado
					FROM
					academico.cargos AS ca 		
					INNER JOIN 
						academico.classes AS cls ON (cls.clsid  = ca.clsid) and cls.clsid = $clsid";
		}else{
			
			$perfis_listartodos = array(PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO);
			
			if(academico_possui_perfil($perfis_listartodos)){				
				
				$sql = "SELECT	
						 '0' as publicado,       
				        ca.crgdsc as cargo,
				        ca.crgid as codigo,
				      	COALESCE (prjnumero, 0) as projetado
						FROM
						academico.cargos AS ca 								
						LEFT JOIN 
						 academico.projetados AS prj ON (prj.crgid  = ca.crgid	AND
						 prj.entidcampus = ".$entidcampus." AND 
						 prj.entidentidade = ".$entid." AND
						 prj.prjano = '".$ano."' AND
						 prj.prgid = ".$prgid." AND 
						 prj.prjstatus = 'A' 
						 )
					 	WHERE
					 		 ca.clsid = $clsid
						";	
			}else{
				
				$sql = "SELECT	
					COALESCE (lp.lepvlrpublicacao, 0)as publicado,       
			        ca.crgdsc as cargo,
			        ca.crgid as codigo,
			        COALESCE (sum(prj.prjnumero), 0) as projetado
				FROM
				academico.cargos AS ca 		
				LEFT JOIN 
					academico.lancamentoeditalportaria AS lp ON (ca.crgid  = lp.crgid)  and lp.edpid = $edpid and lp.lepstatus = 'A'
				LEFT JOIN 
					academico.editalportaria AS ep ON (ep.edpid  = lp.edpid) 
				LEFT JOIN 
					academico.portarias AS p ON (p.prtid  = ep.prtid) and p.prtid = $prtid
				INNER JOIN 
					academico.classes AS cls ON (cls.clsid  = ca.clsid) and cls.clsid = $clsid
				LEFT JOIN 
					 academico.projetados AS prj ON (prj.crgid  = ca.crgid	AND
					 prj.entidcampus = ".$entidcampus." AND 
					 prj.entidentidade = ".$entid." AND
					 prj.prjano = '".$ano."' AND
					 prj.prgid = ".$prgid." AND 
					 prj.prjstatus = 'A' 
					 )
				-- A pedido do Mario foi comentado no dia 22/12/2011. Victor Benzi	" . ($orgid != ACA_ORGAO_TECNICO ? 'WHERE prj.prjnumero <> 0' : '') . "
				GROUP BY lepvlrpublicacao,  ca.crgdsc, ca.crgid
				ORDER BY 
					ca.crgdsc";	
			}
		}
		$dados = $db->carregar($sql);		
	}
	$count = 1;
	if($dados){
		foreach($dados as $chave => $val){
			$crgid = $val['codigo'];
			$crgdsc = $val['cargo'];	
			$projetado = $val['projetado'];
			$publicado = $val['publicado'];
			$homologado = $val['homologado'];				
			$cor = "#f4f4f4";
			$count++;
			$nome = "cargo_".$crgid;
			if ($count % 2){
				$cor = "#e0e0e0";
			}
			echo "	
				<script type=\"text/javascript\"> id_cargos.push('$crgid'); </script>	
				<input type=\"hidden\" name=\"projetado_".$crgid."\" id=\"projetado_".$crgid."\" value='\"".$projetado."\"'>
				<input type=\"hidden\" name=\"publicado_".$crgid."\" id=\"publicado_".$crgid."\" value='\"".$publicado."\"'>
				<input type=\"hidden\" name=\"homologado_".$crgid."\" id=\"homologado_".$crgid."\" value='\"".$homologado."\"'>
				<tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
					<td $title>
					<input id=\"".$crgid."\" name=\"".$crgdsc."\" type=\"checkbox\" value=\"" . $crgid . "\" $key onclick=\"marcaItem('".$crgdsc."', '".$crgid."', '".$projetado."', '".$publicado."', '".$homologado."');\">" . $crgdsc . "
					
					</td>					
				</tr>
			";
		}		
	}
}

?>
<html>
	<head>
		<title>Inserir Cargos</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="JavaScript" src="./academico/geral/js/academico.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript"><!--
		var id_cargos = new Array();
		
		function selecionaTodos() {
			
			var i, cargo, id, nome, projetado, publicado, homologado;
	
			id_publicado = 'publicado_'+"<?=$tabela?>_";
			id_projetado = 'publicado_'+"<?=$tabela?>_";
			id_homologado = 'homologado_'+"<?=$tabela?>_";
			id_autorizado = 'autorizado_'+"<?=$tabela?>_";
			id_efetivado =	'efetivado_'+"<?=$tabela?>_";
			id_nefetivado =	'nefetivado_'+"<?=$tabela?>_";
			
			for(i=0; i<id_cargos.length; i++) {				
				cargo = document.getElementById(id_cargos[i]);
				var proj = 'projetado_'+id_cargos[i];
				var pub = 'publicado_'+id_cargos[i];			
				var hom = 'homologado_'+id_cargos[i];
				
				if(document.getElementById(proj)){
					projetado = document.getElementById(proj).value;
				}else{
					projetado = '';
				}				
				
				if(document.getElementById(pub)){
					publicado = document.getElementById(pub).value;
				}else{
					publicado = '';
				}
				if(document.getElementById(hom)){
					homologado = document.getElementById(pub).value;
				}else{
					homologado = '';
				}
											
				if((document.getElementById("selecionar_todos").checked == true)&&(cargo.checked == false)) {
					cargo.checked = true;
					nome = cargo.name;
					id = cargo.id;														
					marcaItem(nome, id, projetado, publicado, homologado);
					
				} else if((document.getElementById("selecionar_todos").checked == false)&&(cargo.checked == true)) {
					cargo.checked = false;
					nome = cargo.name;
					id = cargo.id;	
					marcaItem(nome, id, projetado, publicado, homologado);
				}			
			}
			
			<?	
			if($_SESSION["academico"]["orgid"] == ACA_ORGAO_SUPERIOR){
			?>
				//recalculaTotal( 'projetado_<?=$tabela?>_','total_proj_<?=$tabela?>');
			<?	
			}
			?>	
			recalculaTotal( 'publicado_<?=$tabela?>_','total_pub_<?=$tabela?>');
			
			<?
			if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_HOMOLOGACAO){						
			?>	
				recalculaTotal( 'homologado_<?=$tabela?>_','total_hom_<?=$tabela?>');
			<?	
			}			    
			?>	
			
		}
		
		function recalculaTotal(id_coluna, id_total) {				
			var i, soma = 0;
				
			form = window.opener.document.getElementById("formulario");			
					
			for(i=0; i<form.length; i++) {		
				if((form.elements[i].id.lastIndexOf(id_coluna) > -1) && (form.elements[i].value != '')) {	
					soma = Number(soma + parseInt(form.elements[i].value));
				}	
			}			
			window.opener.document.getElementById(id_total).value = soma;
			return soma;	
		}
		
		function marcaItem (cargo, id, projetado, publicado, homologado) {				
			var tabela = window.opener.document.getElementById("<?=$tabela?>");	
			if(document.getElementById(id).checked == true) {				
				var tamanho = tabela.rows.length;	
				var linha = tabela.insertRow(1);
				
				if(tamanho == 1) {
					linha.style.backgroundColor = "#f4f4f4";
				}else {
					if(tabela.rows[2].style.backgroundColor == "rgb(224, 224, 224)") {
						linha.style.backgroundColor = "#f4f4f4";					
					} else {
						linha.style.backgroundColor = "#e0e0e0";					
					}
				}
				
				id_projetado  = 'projetado_'+"<?=$tabela?>_" + id;	
				id_publicado  = 'publicado_'+"<?=$tabela?>_" + id;
				id_homologado = 'homologado_'+"<?=$tabela?>_" + id;
				id_autorizado = 'autorizado_'+"<?=$tabela?>_" + id;
				id_efetivado  =	'efetivado_'+"<?=$tabela?>_" + id;
				id_nefetivado =	'nefetivado_'+"<?=$tabela?>_" + id;
	
				linha.id = "<?=$tabela?>_" + id;	
				//caso não exista nenhum cargo  na lista do opener, inserir a linha de totais também. 	
				if(tamanho  == 1){
					
					var totais = tabela.insertRow(tamanho + 1);
					totais.style.backgroundColor = "#FFFFFF";
					var colTotalAcao = totais.insertCell(0);					
					var colTotalCargo = totais.insertCell(1);
					<?	
						if($_SESSION["academico"]["orgid"] == ACA_ORGAO_SUPERIOR){
					?>
						//var colTotalProjetado= totais.insertCell(2);
						//colTotalProjetado.style.textAlign = "center";
						//colTotalProjetado.innerHTML = "<input disabled=\"false\" id='total_proj_<?=$tabela?>' class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";
					<?	
						}
					?>	
					var colTotalPublicado = totais.insertCell(2);	
							
					//var colTotalAutorizado = totais.insertCell(5);
					//var colTotalEfetivado = totais.insertCell(6);
					//var colTotalNEfetivado = totais.insertCell(7);					
					
					//colTotalProjetado.style.textAlign = "center";	
					colTotalPublicado.style.textAlign = "center";
					
					colTotalCargo.style.textAlign = "right";
					colTotalCargo.innerHTML = '<b>Totais<b/>';					
					colTotalPublicado.innerHTML = "<input disabled=\"false\" id='total_pub_<?=$tabela?>' class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";
														
					<?	
						if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_HOMOLOGACAO){
					?>
						var colTotalHomologado= totais.insertCell(3);
						colTotalHomologado.style.textAlign = "center";
						colTotalHomologado.innerHTML = "<input disabled=\"false\" id='total_hom_<?=$tabela?>' class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value="+homologado+">";						
					<?	
						}
					?>
					//colTotalAutorizado.innerHTML = "<input disabled=\"false\" id='total_aut_<?=$tabela?>' class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";
					//colTotalEfetivado.innerHTML = "<input disabled=\"false\" id='total_efe_<?=$tabela?>' class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";
					//colTotalNEfetivado.innerHTML = "<input disabled=\"false\" id='total_nefe_<?=$tabela?>' class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";				
				}
				
				var colAcao = linha.insertCell(0);
				var colCargo = linha.insertCell(1);					
				
				colAcao.style.textAlign = "center";	
				colCargo.style.textAlign = "left";
				colAcao.style.width = "4%";
				colCargo.style.width = "46%";
								
				<?php
				$perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);
				
				$habil = verificaPerfil($perfis);
				if( $habil){
    				$editavel = '';
			    }else{
			    	$editavel = "disabled=\"false\"";
			    }	
			    
				if($_SESSION["academico"]["orgid"] == ACA_ORGAO_SUPERIOR){
				?>
					//var colProjetado= linha.insertCell(2);
					//colProjetado.style.textAlign = "center";
					//colProjetado.innerHTML = "<input id=\""+id_projetado+"\" disabled=\"false\" class=\"CampoEstilo\" type=\"text\" size=\"15\" maxlength=\"15\" value="+projetado+">";					
				<?	
				}
				?>
				
				var colPublicado = linha.insertCell(2);	
				colPublicado.style.textAlign = "center";
		    	<?
				if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_HOMOLOGACAO){
					$editavel_homo = "readonly=readonly style='color: C0C0C0'";		
				?>					
					var colHomologado= linha.insertCell(3);
					colHomologado.style.textAlign = "center";
					colHomologado.innerHTML = "<input id=\""+id_homologado+"\" onkeypress=\"return somenteNumeros(event);\"  onchange=\"valida_lancamento('<?=$tabela?>', "+id+");\"  onkeyup=\"calculaTotal(this, 'total_hom_<?=$tabela?>');\" class=\"CampoEstilo\" name=\"homologado[]\" type=\"text\" size=\"15\" maxlength=\"15\" value="+homologado+">";					
				<?	
				}			    
				?>				
														
				colAcao.innerHTML = "<img style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '<?=$tabela?>');\">" + "<input type=\"hidden\" value=\"" + id + "\" name=\"crgid[]\">";
				colCargo.innerHTML = cargo;								
				colPublicado.innerHTML = "<input type=\"hidden\" id=\"publicado_old_<?=$tabela?>_"+id+"\" value=\""+publicado+"\"><input <?=$editavel_homo?> id=\""+id_publicado+"\" onkeypress=\"return somenteNumeros(event);\"  onblur=\"valida_lancamento('<?=$tabela?>', "+id+");\"   onkeyup=\"calculaTotal(this, 'total_pub_<?=$tabela?>');\"class=\"CampoEstilo\" name=\"publicado[]\" type=\"text\" size=\"15\" maxlength=\"15\" value="+publicado+">";				
				
				//var colAutorizado = linha.insertCell(5);
				//var colEfetivado = linha.insertCell(6);
				//var colNEfetivado = linha.insertCell(7);
				//colAutorizado.innerHTML = "<input <?=$editavel?> id=\""+id_autorizado+"\" onkeypress=\"return somenteNumeros(event);\" onchange=\"valida_lancamento('<?=$tabela?>', "+id+");\"  onkeyup=\"calculaTotal(this, 'total_aut_<?=$tabela?>');\"class=\"CampoEstilo\" name=\"autorizado[]\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";
				//colEfetivado.innerHTML = "<input id=\""+id_efetivado+"\" onkeypress=\"return somenteNumeros(event);\" onchange=\"valida_lancamento('<?=$tabela?>', "+id+");calcula_nefetivados('<?=$tabela?>', "+id+");\"onkeyup=\"calculaTotal(this, 'total_efe_<?=$tabela?>');\"class=\"CampoEstilo\" name=\"efetivado[]\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">";
				//colNEfetivado.innerHTML = "<input disabled=\"false\" id=\""+id_nefetivado+"\" onkeypress=\"return somenteNumeros(event);\" onchange=\"valida_lancamento('<?=$tabela?>', "+id+");\"  class=\"CampoEstilo\" name=\"nefetivado[]\" type=\"text\" size=\"15\" maxlength=\"15\" value=\"0\">	<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif\" border=0 onclick=\"cadastrarObs('nefetivado_obs_<?=$tabela?>_"+id+"');\" title=\"Observação\"><input type=\"hidden\" id=\"nefetivado_obs_<?=$tabela?>_"+id+"\" name=\"obs_nefetivado[]\">";
								
			}
			else {	
				var linha = window.opener.document.getElementById("<?=$tabela?>_" + id).rowIndex;				
				tabela.deleteRow(linha);
			}
			
			//----------------- calculado os totais
			
			<?	
			if($_SESSION["academico"]["orgid"] == ACA_ORGAO_SUPERIOR){
			?>
				//recalculaTotal( 'projetado_<?=$tabela?>_','total_proj_<?=$tabela?>');
			<?	
			}
			?>	
			recalculaTotal( 'publicado_<?=$tabela?>_','total_pub_<?=$tabela?>');
			
			<?
			if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_HOMOLOGACAO){						
			?>	
				recalculaTotal( 'homologado_<?=$tabela?>_','total_hom_<?=$tabela?>');
			<?	
			}			    
			?>	
			
		}
		
	--></script>
	<body>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="60%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Inserir Cargos 
					</label>
				</td>
			</tr>
		</table>
		
		<br/>
				
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">		
			<tr bgcolor="#cdcdcd">
				<td  valign="top">
					<strong>Selecione o(s) Cargos(s)</strong>
				</td>				
			</tr>
			<tr>
				<td colspan="2">
					<input type="checkbox" value="todos" name="selecionar_todos" id="selecionar_todos" onclick="selecionaTodos();"> <strong>Selecionar Todos</strong>
				</td>
			</tr>
			<?php
				montaPopupCargos();
			?>
			<tr bgcolor="#C0C0C0">
				<td colspan="2"> 
					<input type="button" name="ok" value="Ok" onclick="self.close();">
				</td>
			</tr>
		</table>
		
		<script type="text/javascript">
			var tabela = window.opener.document.getElementById("<?=$tabela?>");
			var i, id_linha, check;	
			//(tabela.rows.length -1) essa construção se deve à linha de totais (última linha) q não deve ser contada nesse laço
						
			for(i=1; i<(tabela.rows.length -1); i++) {
				
				id_linha = tabela.rows[i].id;				
				id_linha = id_linha.substr((id_linha.lastIndexOf("_") + 1));
				check = document.getElementById(id_linha);	
				
				if(check){
					check.checked = true;
				}
			}
		</script>
		
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>  
	</body>
</html>