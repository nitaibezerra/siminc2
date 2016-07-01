<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if(isset($_SESSION["pdeid"])) {
	$pdeid = $_SESSION["pdeid"];
}
else {
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
	
	return count($db->carregarColuna("SELECT
							   distinct ace3.aceid
							  FROM
							   pdeescola.prioridadeace p
							   INNER JOIN pdeescola.analisecriterioeficacia ace ON ace.aceid = p.aceid
							   INNER JOIN pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
							   INNER JOIN pdeescola.analisecriterioeficacia ace3 ON ace3.aceid = ace2.aceidpai
							  WHERE
							   pdeid = $pdeid"));	
}

function salva(){
	global $db;
	
	if($_POST["submetido"]) {
		$pdeid = $_POST["pdeid"];
		
		$excluir = $db->carregar("SELECT 
									p.praid 
								 FROM 
								 	pdeescola.prioridadeace p
							   	 WHERE 
							   	 	p.pdeid = ".$pdeid);
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
					$db->executar("INSERT INTO pdeescola.prioridadeace( 
										pdeid,
										aceid,
										praseq
								   )VALUES(
								   		".$pdeid.",
								   		".$idReq.",
								   		".$j.")");
					
					for($k=1; $k<=3; $k++) {
						$idCaract = (int)$_POST["caracteristica"][$_POST["criterio"][$i]][$idReq][$k];
						
						if($idCaract > 0) {
							$db->executar("INSERT INTO 
											pdeescola.prioridadeace(
												pdeid,
												aceid,
												praseq
											)VALUES(
												".$pdeid.",
												".$idCaract.",
												".$k.")");
							$asdf .= "INSERT INTO pdeescola.prioridadeace(pdeid,aceid,praseq) 
								   VALUES(".$pdeid.",".$idCaract.",".$k.")<BR>";
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
											 	ppritem = 'cadastro_prioridade'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'fr2', 'cadastro_prioridade','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();
	}
}

$uf = trim( $_SESSION['estado'] );

if( $uf == 'PR'){
	$descricao 		= "acedescricaoPR";
	$descricaoJoin  = "ace.acedescricaoPR";
	$descricaoJoin2 = "ace2.acedescricaoPR";
	$descricaoJoin3 = "ace3.acedescricaoPR";
}
else
{
	$descricao 		= "acedescricao";
	$descricaoJoin  = "ace.acedescricao";
	$descricaoJoin2 = "ace2.acedescricao";
	$descricaoJoin3 = "ace3.acedescricao";
}
// Recupera todos os critérios cadastrados.
$criterios = $db->carregar("SELECT
								DISTINCT
								ace2.aceid as id_criterio,
								$descricaoJoin2 as desc_criterio
							FROM 
								pdeescola.analisecriterioeficacia ace 
							INNER JOIN
								pdeescola.criticidadeace ca ON ca.aceid = ace.aceid AND 
															   ca.pdeid = ".$pdeid."
							INNER JOIN
								pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai AND 
																		  ace2.aceidpai is null AND
																		  (ace2.aceid = 208 OR ace2.aceid = 1)");

// Forma antiga de recuperar os critérios.
/*$criterios = $db->carregar("SELECT
								ace.aceid as id_criterio,
								ace.acedescricao as desc_criterio
							FROM 
								pdeescola.analisecriterioeficacia ace 
							INNER JOIN
								pdeescola.criticidadeace ca ON ca.aceid = ace.aceid
							WHERE 
								ace.aceidpai is null AND 
								ace.aceseq is null AND 
								ace.aceano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
								ca.pdeid = ".$pdeid."
							ORDER BY
								ace.acecodigo ASC");*/
if(!$criterios) {
	echo 
	'<script>;
		alert("O cadastro de Criticidade ainda não foi realizado.");
		history.back(-1);
	 </script>';
	exit;
}

?>

<br />
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<?=subCabecalho('Ficha-resumo 2 - Prioridade'); ?>
<form method="post" id="form_prioridade" name="form_prioridade">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" id="tabela_prioridade" width="100%" bgcolor="#dfdfdf" cellSpacing="0" cellPadding="0" align="center">
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
												pdeescola.prioridadeace ca
											INNER JOIN
												pdeescola.analisecriterioeficacia ace ON ace.aceid = ca.aceid
											INNER JOIN
												pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
																					   AND ace2.aceid = ".$criterios[$i]["id_criterio"]."
											WHERE 
												ca.pdeid = ".$pdeid." AND ca.praseq = ".($j+1)."
											ORDER BY
												ca.praseq ASC");
//				dbg("SELECT
//												ace.aceid as id_requisito,
//												ace.acedescricao as desc_requisito
//											FROM 
//												pdeescola.prioridadeace ca
//											INNER JOIN
//												pdeescola.analisecriterioeficacia ace ON ace.aceid = ca.aceid
//											INNER JOIN
//												pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
//																					   AND ace2.aceid = ".$criterios[$i]["id_criterio"]."
//											WHERE 
//												ca.pdeid = ".$pdeid." AND ca.praseq = ".($j+1)."
//											ORDER BY
//												ca.praseq ASC");
				
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
														pdeescola.prioridadeace ca
													INNER JOIN
														pdeescola.analisecriterioeficacia ace ON ace.aceid = ca.aceid
													INNER JOIN
														pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
																							   AND ace2.aceid = ".$requisitos[0]["id_requisito"]."
													WHERE 
														ca.pdeid = ".$pdeid." AND ca.praseq = ".($k+1)."
													ORDER BY
														ca.praseq ASC");
					
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
			
		//	if($i != (count($criterios) - 1))
				echo "<tr height=\"5px\" bgcolor=\"#C0C0C0\"><td colspan=\"5\"></td></tr>";
		}
	?>
</tbody>






<tbody>
	<?php
		unset($criterio);
		$quantPrioridade  = quantPrioridade($pdeid);

		if ($quantPrioridade == 3){
			$combocriterio = $db->pegaUm("SELECT
											ace3.aceid as id_criterio,
											$descricaoJoin3 as desc_criterio
										 FROM 
											pdeescola.analisecriterioeficacia ace 
											INNER JOIN pdeescola.prioridadeace ca ON ca.aceid = ace.aceid AND 
														      						 ca.pdeid = $pdeid
											INNER JOIN pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai					
											INNER JOIN pdeescola.analisecriterioeficacia ace3 ON ace3.aceid = ace2.aceidpai	
										 WHERE
										 	ace3.aceid != 208 AND
										    ace3.aceid != 1");
			$criterio = $combocriterio;
		}
		
		$sql = "SELECT
					ace2.aceid as codigo,
					ace2.acedescricao as descricao
				FROM 
					pdeescola.analisecriterioeficacia ace 
				INNER JOIN
					pdeescola.criticidadeace ca ON ca.aceid = ace.aceid AND 
												   ca.pdeid = ".$pdeid."
				INNER JOIN
					pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai AND 
															  ace2.aceidpai is null AND
															  ace2.aceid != 208 AND 
															  ace2.aceid != 1";
	
//		$contRequisito = 2;
?>		
			<tr align="center" id="teste">
					<td bgcolor="#f0f0f0">
						<?$db->monta_combo( 'combocriterio', $sql, 'S', '-- Informe --', 'preparaCriterioCombo(this);', '' )?>
						<input type="hidden" name="criterio[]" value="<?=$criterio ?>">
					</td>
					<td bgcolor="#f0f0f0" colspan="4">
						<table id="tabela_requisitos_<?=$criterio?>" width="100%" bgcolor="#dfdfdf" cellSpacing="1" cellPadding="0" align="center">
				  	    	<tbody>
<?php
		for($j=0; $j<=2; $j++) {
				$contRequisito++;
				if ($quantPrioridade == 3){
					$requisitos = $db->carregar("SELECT
													ace.aceid as id_requisito,
													$descricaoJoin as desc_requisito
												FROM 
													pdeescola.prioridadeace ca
												INNER JOIN
													pdeescola.analisecriterioeficacia ace ON ace.aceid = ca.aceid
												INNER JOIN
													pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai AND 
																		  ace2.aceid = ".$criterio."
												WHERE 
													ca.pdeid = ".$pdeid." AND ca.praseq = ".($j+1)."
												ORDER BY
													ca.praseq ASC");
				}
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
							<a style=\"cursor:pointer;\" onclick=\"janelaRequisitos('',".$requisitos[0]["id_requisito"].",this.parentNode.parentNode.rowIndex,".$contRequisito.",1);\">
							<img src=\"../imagens/arrow_simples.gif\">".$requisitos[0]["desc_requisito"]."</a>
						</td>
						<td bgcolor=\"#f0f0f0\" width=\"6%\">
							".$img."
							<input type=\"hidden\" id=\"requisito[".$criterio."][".($j+1)."]\" name=\"requisito[".$criterio."][".($j+1)."]\" value=\"".$requisitos[0]["id_requisito"]."\">
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
														pdeescola.prioridadeace p
													INNER JOIN
														pdeescola.analisecriterioeficacia ace ON ace.aceid = p.aceid
													INNER JOIN
														pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai AND 
																								  ace2.aceid = ".$requisitos[0]["id_requisito"]."
													WHERE 
														p.pdeid = ".$pdeid." AND p.praseq = ".($k+1)."
													ORDER BY
														p.praseq ASC");
					
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
								<a style=\"cursor:pointer;\" onclick=\"janelaCaracteristicas('',".$requisitos[0]["id_requisito"].",".$caracteristicas[0]["id_caracteristica"].",this.parentNode.parentNode.rowIndex,".$contRequisito.",1);\">
									<img src=\"../imagens/arrow_simples.gif\">
									".$caracteristicas[0]["desc_caracteristica"]."
								</a>
							</td>
							<td bgcolor=\"#f0f0f0\" width=\"6%\" align=\"center\">
								".$img."
								<input type=\"hidden\" id=\"caracteristica[".$criterio."][".$requisitos[0]["id_requisito"]."][".($k+1)."]\" name=\"caracteristica[".$criterio."][".$requisitos[0]["id_requisito"]."][".($k+1)."]\" value=\"".$caracteristicas[0]["id_caracteristica"]."\">
								<input type=\"hidden\" value=\"".($k+1)."\">
							</td>
						  </tr>";
				}
				echo "</tbody></table></td></tr>";
			}
			echo "</tbody></table></td></tr>";
			
	?>
</tbody>






<tfoot>
	<? $cForm->montarbuttons("submeteDadosPrioridade();");?>				
</tfoot>
</table>
</td>
</tr>
</tbody>
</table>
</form>
<script language="javascript" type="text/javascript">
var comboAnt = '<?=$criterio?>';
function preparaCriterioCombo(combo){
	d = document;

	var criterio = d.getElementsByName('criterio[]');	
	var critLen  = d.getElementsByName('criterio[]').length - 1;
	
	criterio[critLen].value = combo.value;

	//alert(criterio[critLen].parentNode.parentNode.getElementsByTagName('table').length);
	var table = criterio[critLen].parentNode.parentNode.getElementsByTagName('table');

	table[0].id = 'tabela_requisitos_'+combo.value;

	var input = table[0].getElementsByTagName('input');

	for (i=0; i < input.length; i++){
		input[i].id   = input[i].id.replace('['+comboAnt+']','['+combo.value+']');	
		input[i].name = input[i].name.replace('['+comboAnt+']','['+combo.value+']');			
	}

	comboAnt = combo.value;
	zera(combo);
}

function zera(combo){
	var d 	  = document;
	var tr = d.getElementById('tabela_requisitos_'+combo.value).getElementsByTagName('tr');
	var row = 6;
	
	for (i=0; i < tr.length; i++){
		var td = tr[i].getElementsByTagName('td');
		for (a=0; a < td.length; a++){
			if (a == 0){
				if (i==0 || i==4 || i==8){
					row++;
					td[a].innerHTML = '<a onclick="janelaRequisitos('+combo.value+',0,this.parentNode.parentNode.rowIndex,'+row+',1);" style="cursor: pointer;">'+
									  '		<img src="../imagens/arrow_simples.gif"/>Clique para escolher um requisito</a>'+
									  '</a>';	
				}else{
					td[a].innerHTML = '<a onclick="janelaCaracteristicas('+combo.value+',0,0,this.parentNode.parentNode.rowIndex,'+row+',1);" style="cursor: pointer;">'+
												'<img src="../imagens/arrow_simples.gif"/>'+
												'Clique para escolher uma característica'+
											'</a>';					
				}	
			}	
		}
		
	}	
}

function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A";
}

function submeteDadosPrioridade() {
	var contRequisito;
	var contCaracteristica;
	var descCriterio;
	var controle = 0;
	
	var reqId = 1;
	while(reqId < 209) {
		contRequisito = 0;
		contCaracteristica = 0;
		
		if(reqId == 1)
			descCriterio = "'Ensino e Aprendizagem'";
		else
			descCriterio = "'Resultados'";
		
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
		
		reqId = Number(reqId + 207); 
	}
	
	if(controle == 0)
	{	document.getElementById('form_prioridade').submit();
		return true;
	}	
	else
	{
		return false;
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

//function janelaRequisitos(criterio,requisito,rowindex,idTabela) {
function janelaRequisitos(criterio,requisito,rowindex,idTabela,dif) {
	var d = document;
	
	criterio = criterio == '' || dif ? d.getElementsByName('combocriterio')[0].value : criterio;

	if (criterio == ''){
		alert('A combo Critérios de Eficácia Escolar\nDeve ser preenchido!');
		return;
	}		
	
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

function janelaCaracteristicas(criterio,requisito,caracteristica,rowindex,idTabela,dif) {
	var d = document;
	criterio = criterio == '' || dif ? d.getElementsByName('combocriterio')[0].value : criterio;
	
	var posicao = (rowindex + 1);

	var input = d.getElementById('tabela_caracteristicas_'+idTabela).parentNode.parentNode.getElementsByTagName('input');

	if (requisito == 0){
		for (i=0; i<input.length; i++){
			if (input[i].name.indexOf('requisito') > -1){
				requisito = input[i].value;
				break;
			}				
		}	
	}
	
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
</script>