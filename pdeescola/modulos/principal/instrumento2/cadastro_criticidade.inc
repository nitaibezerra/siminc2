<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if(isset($_SESSION["pdeid"])) {
	$pdeid = $_SESSION["pdeid"];
}else {
	echo "<script language=\javascript\" type=\"text/javascript\">
			alert('Erro no envio dos parâmetros necessários.');
			window.history.go(-1);
		  </script>";
	exit;
}

$cDado = array("instrumento" => 2);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 2', '');
echo cabecalhoPDE();

function quantPrioridade($pdeid){
	global $db;
	
	return $db->pegaUm("SELECT
						 COUNT(praid)
						FROM
						 pdeescola.prioridadeace
						WHERE
						 pdeid = $pdeid");	
}

function salva(){
	global $db;
	
	if($_POST["submetido"]) {
		$pdeid = $_POST["pdeid"];
		
		
		/*** início - deleta os registros da tabela de criticidade. ***/
		$excluir = $db->carregar("SELECT 
									ca.craid 
								 FROM 
								 	pdeescola.criticidadeace ca
							   	 WHERE 
							   	 	ca.pdeid = ".$pdeid);
		$in = "";
		for($a=0; $a<count($excluir); $a++) {
			if($a==0)
				$in .= $excluir[$a]["craid"];
			else
				$in .= ",".$excluir[$a]["craid"];
		}
		if($in != "")
			$db->executar("DELETE FROM pdeescola.criticidadeace WHERE craid in (".$in.")");
		/*** fim - deleta os registros da tabela de criticidade. ***/

			
		/*** início - deleta os registros da tabela de prioridade. ***/
		$excluir = $db->carregar("SELECT 
									pa.praid 
								 FROM 
								 	pdeescola.prioridadeace pa
							   	 WHERE 
							   	 	pa.pdeid = ".$pdeid);
		$in = "";
		for($a=0; $a<count($excluir); $a++) {
			if($a==0)
				$in .= $excluir[$a]["praid"];
			else
				$in .= ",".$excluir[$a]["praid"];
		}
		if($in != "")
			$db->executar("DELETE FROM pdeescola.prioridadeace WHERE praid in (".$in.")");
	 
			
		for($i=0; $i<count($_POST["criterio"]); $i++) {
			for($j=1; $j<=3; $j++) {
				$idReq = (int)$_POST["requisito"][$_POST["criterio"][$i]][$j];
				
				if($idReq > 0) {
					$db->executar("INSERT INTO pdeescola.criticidadeace(pdeid,aceid,craseq) 
								   VALUES(".$pdeid.",".$idReq.",".$j.")");
					
					$db->executar("INSERT INTO pdeescola.prioridadeace(pdeid,aceid,praseq) 
								   VALUES(".$pdeid.",".$idReq.",".$j.")");	
									
					for($k=1; $k<=3; $k++) {
						$idCaract = (int)$_POST["caracteristica"][$_POST["criterio"][$i]][$idReq][$k];
						
						if($idCaract > 0) {
							$db->executar("INSERT INTO pdeescola.criticidadeace(pdeid,aceid,craseq) 
								   VALUES(".$pdeid.",".$idCaract.",".$k.")");
							
							$db->executar("INSERT INTO pdeescola.prioridadeace(pdeid,aceid,praseq) 
								   VALUES(".$pdeid.",".$idCaract.",".$k.")");
						}
					}
				}
			}
		} 
		$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'cadastro_criticidade'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'fr2', 'cadastro_criticidade','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();
	}
}
// Tratamento Exercicio base de comparacao - exibir dados do ano de exercicio ate ano vigente.
if($_SESSION["exercicio"] != $_SESSION['exercicio_atual']) {
				echo "<script language=\javascript\" type=\"text/javascript\">
						alert('Nenhum dado para este ano Exercício Base de Comparação.');
						window.history.go(-1);
 					</script>";
				exit;	
}
// Recupera todos os critérios cadastrados.

 

$uf = trim( $_SESSION['estado'] );

if( $uf == 'PR'){
	$descricao = "acedescricaoPR";
	$descricaoJoin = "ace.acedescricaoPR";
}
else
{
	$descricao = "acedescricao";
	$descricaoJoin = "ace.acedescricao";
}
$criterios = $db->carregar("SELECT
								aceid as id_criterio,
								$descricao as desc_criterio
							FROM 
								pdeescola.analisecriterioeficacia 
							WHERE 
								aceidpai is null AND 
								aceseq is null AND 
								aceano = ".ANO_EXERCICIO_PDE_ESCOLA."
							ORDER BY
								acecodigo ASC");
?>

<br />
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<?=subCabecalho('Ficha-resumo 2 - Criticidade'); ?>	
<form method="post" id="form_criticidade" name="form_criticidade">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" id="tabela_criticidade" width="100%" bgcolor="#dfdfdf" cellSpacing="0" cellPadding="0" align="center">
<thead>
	<tr>
		<th width="10%">Critérios de Eficácia Escolar</th>
		<th width="20%">Requisitos</th>
		<th width="5%">&nbsp;&nbsp;Ordem</th>
		<th width="60%">Características</th>
		<th width="5%">Ordem</th>
	</tr>
</thead>
<tbody>
	<?php
		$contRequisito = 0;
		for($i=0; $i<count($criterios); $i++) {
			echo "<tr align=\"center\">
					<td bgcolor=\"#f0f0f0\">
						<b>".$criterios[$i]["desc_criterio"]."</b>
						<input type=\"hidden\" name=\"criterio[]\" value=\"".$criterios[$i]["id_criterio"]."\">
					</td>
					<td bgcolor=\"#f0f0f0\" colspan=\"4\">
						<table id=\"tabela_requisitos_".$criterios[$i]["id_criterio"]."\" width=\"100%\" bgcolor=\"#dfdfdf\" cellSpacing=\"1\" cellPadding=\"0\" align=\"center\">
				  	    	<tbody>";
			
			for($j=0; $j<=2; $j++) {
				$contRequisito++;
				
				$requisitos = $db->carregar("SELECT
												ace.aceid as id_requisito,
												$descricaoJoin as desc_requisito
											FROM 
												pdeescola.criticidadeace ca
											INNER JOIN
												pdeescola.analisecriterioeficacia ace ON ace.aceid = ca.aceid
											INNER JOIN
												pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
																					   AND ace2.aceid = ".$criterios[$i]["id_criterio"]."
											WHERE 
												ca.pdeid = ".$pdeid." AND ca.craseq = ".($j+1)."
											ORDER BY
												ca.craseq ASC");
				
				if($requisitos[0]["id_requisito"] == NULL) {
					$requisitos[0]["id_requisito"]   = 0;
					$requisitos[0]["desc_requisito"] = "Clique para escolher um requisito";
				}
				
				if($j==0) {
					$img = "<img src=\"../imagens/seta_baixo.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('baixo','requisito',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">
							<img src=\"../imagens/seta_cimad.gif\" style=\"border:0; cursor:pointer;\">";
				} else if($j==1) {
					$img = "<img src=\"../imagens/seta_baixo.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('baixo','requisito',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">
							<img src=\"../imagens/seta_cima.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('cima','requisito',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">";
				} else {
					$img = "<img src=\"../imagens/seta_baixod.gif\" style=\"border:0; cursor:pointer;\">
							<img src=\"../imagens/seta_cima.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('cima','requisito',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">";
				}
				
				echo "<tr align=\"center\">
						<td bgcolor=\"#f0f0f0\" width=\"22%\">
							<a style=\"cursor:pointer;\" onclick=\"janelaRequisitos(".$criterios[$i]["id_criterio"].",".$requisitos[0]["id_requisito"].",this.parentNode.parentNode.rowIndex,".$contRequisito.");\">
							<img src=\"../imagens/arrow_simples.gif\">".$requisitos[0]["desc_requisito"]."</a>
						</td>
						<td bgcolor=\"#f0f0f0\" width=\"6%\">
							".$img."
							<input type=\"hidden\" id=\"requisito[".$criterios[$i]["id_criterio"]."][".($j+1)."]\" name=\"requisito[".$criterios[$i]["id_criterio"]."][".($j+1)."]\" value=\"".$requisitos[0]["id_requisito"]."\">
							<input type=\"hidden\" value=\"".($j+1)."\">
						</td>
						<td bgcolor=\"#f0f0f0\">
							<table id=\"tabela_caracteristicas_".$contRequisito."\" width=\"100%\" bgcolor=\"#dfdfdf\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
								<tbody>";
				
				for($k=0; $k<=2; $k++) {
					$caracteristicas = $db->carregar("SELECT
														ace.aceid as id_caracteristica,
														$descricaoJoin as desc_caracteristica
													FROM 
														pdeescola.criticidadeace ca
													INNER JOIN
														pdeescola.analisecriterioeficacia ace ON ace.aceid = ca.aceid
													INNER JOIN
														pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
																							   AND ace2.aceid = ".$requisitos[0]["id_requisito"]."
													WHERE 
														ca.pdeid = ".$pdeid." AND ca.craseq = ".($k+1)."
													ORDER BY
														ca.craseq ASC");
					
					if($caracteristicas[0]["id_caracteristica"] == NULL) {
						$caracteristicas[0]["id_caracteristica"]   = 0;
						$caracteristicas[0]["desc_caracteristica"] = "Clique para escolher uma característica";
					}
					
					if($k==0) {
						$img = "<img src=\"../imagens/seta_baixo.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('baixo','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">
								<img src=\"../imagens/seta_cimad.gif\" style=\"border:0; cursor:pointer;\">";
					} else if($k==1) {
						$img = "<img src=\"../imagens/seta_baixo.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('baixo','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">
								<img src=\"../imagens/seta_cima.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('cima','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">";
					} else {
						$img = "<img src=\"../imagens/seta_baixod.gif\" style=\"border:0; cursor:pointer;\">
								<img src=\"../imagens/seta_cima.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('cima','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">";
					}
					
					echo "<tr height=\"30px\">
							<td bgcolor=\"#f0f0f0\" width=\"76%\" align=\"left\">
								<a style=\"cursor:pointer;\" onclick=\"janelaCaracteristicas(".$criterios[$i]["id_criterio"].",".$requisitos[0]["id_requisito"].",".$caracteristicas[0]["id_caracteristica"].",this.parentNode.parentNode.rowIndex,".$contRequisito.");\">
									<img src=\"../imagens/arrow_simples.gif\">
									".$caracteristicas[0]["desc_caracteristica"]."
								</a>
							</td>
							<td bgcolor=\"#f0f0f0\" width=\"6%\" align=\"center\">
								".$img."
								<input type=\"hidden\" id=\"caracteristica[".$criterios[$i]["id_criterio"]."][".$requisitos[0]["id_requisito"]."][".($k+1)."]\" name=\"caracteristica[".$criterios[$i]["id_criterio"]."][".$requisitos[0]["id_requisito"]."][".($k+1)."]\" value=\"".$caracteristicas[0]["id_caracteristica"]."\">
								<input type=\"hidden\" value=\"".($k+1)."\">
							</td>
						  </tr>";
				}
				echo "</tbody></table></td></tr>";
			}
			echo "</tbody></table></td></tr>";
			
			if($i != (count($criterios) - 1))
				echo "<tr height=\"5px\" bgcolor=\"#C0C0C0\"><td colspan=\"5\"></td></tr>";
		}
	?>
</tbody>
<tfoot>
	<? $cForm->montarbuttons("submeteDadosCriticidade();");?>				
</tfoot>
</table>
</form>
<script language="javascript" type="text/javascript"><!--
function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A";
}

function submeteDadosCriticidade() {
	var contRequisito;
	var contCaracteristica;
	var descCriterio;
	var controle = 0;
	
	var reqIds = new Object();

	
	<?if( $uf != 'PR'){	?>
	reqIds[1]   = "'Ensino e Aprendizagem'";
	reqIds[66]  = "'Clima Escolar'";
	reqIds[124] = "'Pais e Comunidade'";
	reqIds[140] = "'Gestão de Pessoas'";
	reqIds[168] = "'Gestão de Processos'";
	reqIds[200] = "'Infra-estrutura'";
	reqIds[208] = "'Resultados'"; 
	<?}else{ ?>
	reqIds[1]   = "'Ensino e Aprendizagem'";
	reqIds[66]  = "'Estabelecimento com qualidade de ensino'";
	reqIds[124] = "'Pais e Comunidade'";
	reqIds[140] = "'Profissionais da escola '";
	reqIds[168] = "'Processos escolares'";
	reqIds[200] = "'Infra-estrutura'";
	reqIds[208] = "'Resultados'"; 
	<?} ?>

	


	
	for (var a in reqIds){
	//while(reqId < 209) {
		contRequisito = 0;
		contCaracteristica = 0;

		reqId 		 = a;
		descCriterio = reqIds[a];
//		if(reqId == 1)
//			descCriterio = "'Ensino e Aprendizagem'";
//		else
//			descCriterio = "'Resultados'";
		
		for(var i=1; i<=3; i++) {
			var requisito = document.getElementById('requisito['+reqId+']['+i+']').value;
			
			if(requisito == 0) {
				contRequisito++;
			} else {
				for(var j=1; j<=3; j++) {
					var caracteristica = document.getElementById('caracteristica['+reqId+']['+requisito+']['+j+']').value;
					
					if(caracteristica == 0)
						contCaracteristica++;
				}
			}
		}
		
		if((contRequisito == 3) || ((contRequisito == 2) && (contCaracteristica == 3)) ||		   
		   ((contRequisito == 1) && (contCaracteristica == 6)) || ((contRequisito == 0) && (contCaracteristica == 9))) {
				alert("Pelo menos um requisito e uma característica devem ser\n  selecionados para o critério "+descCriterio);
				controle = 1;
				break;
		}
		
	//	reqId = Number(reqId + 207); 
	}
	
	if(controle == 0){
		if (<?=quantPrioridade($pdeid)?> == 3){
			if (confirm('Você irá sobrescrever as prioridades.\nDeseja continuar?')){
				document.getElementById('form_criticidade').submit();	
				return true;			
			}else{
				return false;
			}		
		}			
		document.getElementById('form_criticidade').submit();
		return true;
	}	
}

function mudaPosicao(posicao,tipo,idTabela,index) {
	var tabela = document.getElementById(idTabela);	
	var index2 = (posicao == "baixo") ? (index+1) : (index-1);
	
	var celula1 = tabela.rows[index].cells[1];
	var tamanho1  = celula1.childNodes.length;		
	var valueCelula1 = celula1.childNodes[tamanho1 - 4].value;
	
	var celula2 = tabela.rows[index2].cells[1];
	var tamanho2  = celula2.childNodes.length;		
	var valueCelula2 = celula2.childNodes[tamanho2 - 4].value;
	
	celula1.childNodes[tamanho1 - 4].value = valueCelula2;
	celula2.childNodes[tamanho2 - 4].value = valueCelula1;
	
	if(tipo == 'requisito') {
		var celulaRequisito1a = tabela.rows[index].cells[0].innerHTML;
		var celulaRequisito1b = tabela.rows[index].cells[2].innerHTML;
		var celulaRequisito2a = tabela.rows[index2].cells[0].innerHTML;
		var celulaRequisito2b = tabela.rows[index2].cells[2].innerHTML;
		
		tabela.rows[index].cells[0].innerHTML = celulaRequisito2a;
		tabela.rows[index].cells[2].innerHTML = celulaRequisito2b;
		tabela.rows[index2].cells[0].innerHTML = celulaRequisito1a;
		tabela.rows[index2].cells[2].innerHTML = celulaRequisito1b;
	}
	if(tipo == 'caracteristica') {
		var celulaCaracteristica1 = tabela.rows[index].cells[0].innerHTML;
		var celulaCaracteristica2 = tabela.rows[index2].cells[0].innerHTML;
		
		tabela.rows[index].cells[0].innerHTML = celulaCaracteristica2;
		tabela.rows[index2].cells[0].innerHTML = celulaCaracteristica1;
	}
}

function janelaRequisitos(criterio,requisito,rowindex,idTabela) {
	var posicao = (rowindex + 1);
	
	if(posicao == 1) {
		var requisito2 = document.getElementById("requisito["+criterio+"][2]").value;
		var requisito3 = document.getElementById("requisito["+criterio+"][3]").value;
	}
	else if(posicao == 2) {
		var requisito2 = document.getElementById("requisito["+criterio+"][1]").value;
		var requisito3 = document.getElementById("requisito["+criterio+"][3]").value;
	}
	else {
		var requisito2 = document.getElementById("requisito["+criterio+"][1]").value;
		var requisito3 = document.getElementById("requisito["+criterio+"][2]").value;
	}
	
	requisito2 = (requisito2 == "") ? 0 : requisito2;
	requisito3 = (requisito3 == "") ? 0 : requisito3;
	
	windowOpen('?modulo=principal/instrumento2/lista_requisitos&acao=A&criterio='+criterio+'&requisito='+requisito+'&requisito2='+requisito2+'&requisito3='+requisito3+'&posicao='+posicao+'&rowindex='+rowindex+'&idtabela='+idTabela+'','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function janelaCaracteristicas(criterio,requisito,caracteristica,rowindex,idTabela) {
	var posicao = (rowindex + 1);
	
	if(posicao == 1) {
		var caracteristica2 = document.getElementById("caracteristica["+criterio+"]["+requisito+"][2]").value;
		var caracteristica3 = document.getElementById("caracteristica["+criterio+"]["+requisito+"][3]").value;
	}
	else if(posicao == 2) {
		var caracteristica2 = document.getElementById("caracteristica["+criterio+"]["+requisito+"][1]").value;
		var caracteristica3 = document.getElementById("caracteristica["+criterio+"]["+requisito+"][3]").value;
	}
	else {
		var caracteristica2 = document.getElementById("caracteristica["+criterio+"]["+requisito+"][1]").value;
		var caracteristica3 = document.getElementById("caracteristica["+criterio+"]["+requisito+"][2]").value;
	}
	
	caracteristica2 = (caracteristica2 == "") ? 0 : caracteristica2;
	caracteristica3 = (caracteristica3 == "") ? 0 : caracteristica3;
	
	windowOpen('?modulo=principal/instrumento2/lista_caracteristicas&acao=A&criterio='+criterio+'&requisito='+requisito+'&caracteristica='+caracteristica+'&caracteristica2='+caracteristica2+'&caracteristica3='+caracteristica3+'&posicao='+posicao+'&rowindex='+rowindex+'&idtabela='+idTabela+'','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}
--></script>