<?php
pde_verificaSessao();
/*----- Recuperando $docid para usar na função pegarEstadoAtual ------*/ 
$docid  = pegarDocid( $_SESSION['entid'] );
$estado = pegarEstadoAtual( $docid );
$estadoPerm = (($estado == 76) || ($estado == 37));
	
/*----------------- Verificando Perfil ---------------------*/ 
$perfis = arrayPerfil();
$boSuperUsuario = in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis );
$boPerfilPerm = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)));
	
/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid = ($_POST['pdeid']) ? $_POST['pdeid'] : $_SESSION['pdeid'];

if(!$pdeid) {
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

function salva($pdeid = null) {
	global $db;
	
	if($_POST["submetido"]) {
		if($_POST["itens_excluidos"] != "") {
			$excluidos = explode(";",$_POST["itens_excluidos"]);
			
			for($i=0; $i<count($excluidos); $i++) {
				$db->executar("DELETE FROM pdeescola.problemainstrumento1 WHERE priid = ".$excluidos[$i]);
			}
			
			# Se não tiver registro deletamos da tabela preenchimento 
		  	/*$sql = "SELECT * FROM pdeescola.problemainstrumento1 WHERE pdeid = $pdeid";
		  	
		  	$arProbInstrumentos = $db->carregar( $sql );
		  	if(!is_array($arProbInstrumentos)){
		  		$pdeanoreferencia = $db->pegaUm("SELECT pdeanoreferencia FROM pdeescola.pdepreenchimento WHERE pdeid = '$pdeid' AND ppritem = 'fr6'");
		  		if($pdeanoreferencia){
		  			$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = 'fr6' AND pdeanoreferencia = '".$pdeanoreferencia."'";
		  			$db->executar($sql);
		  		}
		  	}*/
			
		}
		
		$tprid = $db->pegaUm("SELECT tprid FROM pdeescola.tipoproblema WHERE tprvisivel = 'f'");
		
		for($i=0; $i<count($_POST["pridescricao"]); $i++) {
			if($_POST["priid"][$i] == "") {
				$db->executar("INSERT INTO 
									pdeescola.problemainstrumento1 (pdeid,pridescricao,tprid)
						       VALUES (".$pdeid.",'".$_POST["pridescricao"][$i]."',".$tprid.")");
			}
			else {
				$db->executar("UPDATE 
									pdeescola.problemainstrumento1 
							   SET 
							   		pridescricao = '".$_POST["pridescricao"][$i]."'
							   WHERE 
							   		priid = ".$_POST["priid"][$i]);
			}
		}
		 
		$existe_preenchimento = $db->pegaUm("SELECT 
											pprid
										 FROM 
										 	pdeescola.pdepreenchimento
										 WHERE
										 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'fr6'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'fr1', 'fr6','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
			 
		}
		$db->commit();  
	}
}

$dados = $db->carregar("SELECT
							pi.pridescricao,
							pi.priid
						FROM
							pdeescola.problemainstrumento1 pi
						INNER JOIN
							pdeescola.tipoproblema tp ON tp.tprid = pi.tprid
													  AND tp.tprvisivel = 'f'
						WHERE
							pi.pdeid = ".$pdeid."
						ORDER BY
							pi.priid DESC");

if(!$dados) {
	$vis = "style='visibility:hidden;'";
	echo "<script language=\"javascript\" type=\"text/javascript\">
			var qtdItens = 0;
		  </script>";
} else {
	$vis = "style='visibility:visible;'";
	echo "<script language=\"javascript\" type=\"text/javascript\">
			var qtdItens = ".count($dados).";
		  </script>";
}

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva( pdeid )', array("pdeid" => $pdeid));

include APPRAIZ.'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1 - Ficha Resumo 1';
monta_titulo($titulo_modulo, '');

?>
<?=cabecalhoPDE();?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?=subCabecalho('6) Problemas que devem ser atacados prioritariamente, com base nas informações anteriores e que sejam de governabilidade da escola'); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tbody>
	<tr>
		<td width="20%" class="SubTituloDireita">Problema:</td>
		<td width="80%">
			<textarea class="CampoEstilo" name="problema" id="problema" onblur="retiraNewLine();"onkeyup=" retiraNewLine();" cols="60" rows="4"></textarea>
		</td>
	</tr>
	<tr>
		<td width="100%" colspan="2" class="SubTituloDireita" style="text-align:center;">
			<input type="button" value="Adicionar" style="cursor: pointer" onclick="adicionarProblema();">
			<input type="button" value="Voltar" style="cursor: pointer" onclick="javascript: history.go(-1);">
		</td>
	</tr>
</tbody>
</table>
<br />
<form method="post" name="formFichaResumo" id="formFichaResumo">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<input type="hidden" id="itens_excluidos" name="itens_excluidos" value="">
<table class="tabela" id="tabela_problema" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" <?=$vis?>>
<thead>
	<tr>
		<th width="20%">Ação</th>
		<th width="50%">Problema</th>
	</tr>
</thead>
<tbody>
	<?php
		$contador = 0;
		$cont = 0;
		
		if($dados) {
			for($i=0; $i<count($dados); $i++) {
				$contador++;
				$cont++;
				$cor = ($cont % 2) ? "#f4f4f4" : "#e0e0e0";
				
			// 76 = estado atual -->diagnótico e somente podera excluir os perfis = 200, 225 ou 226
			if( $estadoPerm || $boSuperUsuario)
			{ 
				if( $boSuperUsuario || $boPerfilPerm)
				{
						$sql_estado = "<img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Alterar' onclick='windowOpen(\"?modulo=principal/instrumento1/popup_alterar_fr6&acao=A&descproblema=".$dados[$i]["pridescricao"]."&contador=".$contador."&priid=".$dados[$i]["priid"]."\",\"blank\",\"height=200,width=450,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes\");'>
					  					   <img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick='excluiProblema(this.parentNode.parentNode.rowIndex,".$dados[$i]["priid"].");'>";
				}
			}
				echo "<tr id=\"linha_".$contador."\" style=\"background-color:".$cor.";\" onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='".$cor."';\">
					  	<td align=\"center\">
					    ".$sql_estado."
					  		<input type=\"hidden\" name=\"pridescricao[]\" value=\"".$dados[$i]["pridescricao"]."\">
							<input type=\"hidden\" name=\"priid[]\" value=\"".$dados[$i]["priid"]."\">
						</td>
						<td align=\"center\">
							".$dados[$i]["pridescricao"]."
						</td>
					  </tr>";
			}
		}
		
		echo "<script language=\"javascript\" type=\"text/javascript\">
				var contador = ".$contador.";
			  </script>";
	?>
</tbody>
<tfoot>
	<? echo $cForm->montarbuttons("submeteFormFichaResumo()"); ?>
</tfoot>
</table>
</form>
<script language="javascript" type="text/javascript"><!--

var itensExcluir = "";

function submeteFormFichaResumo() {
	if((qtdItens < 1) || (qtdItens > 5)) {
		alert("Devem ser cadastrados no mínimo '1' ou no máximo '5' problemas.");
		return false;
	} else {
		document.getElementById('itens_excluidos').value = itensExcluir;
		document.getElementById('formFichaResumo').submit();
		return true;
	}
}

function retiraNewLine()
{ 
	var novaString = document.getElementById( "problema" ).value;
        
	while( novaString.indexOf("\n") != -1 )
	{               
		novaString = novaString.replace('\n', " ");
    }        
	while( novaString.indexOf("\r") != -1 )
	{               
		novaString = novaString.replace('\r', "");
    }        

    document.getElementById("problema").value = novaString;
    
} 

function adicionarProblema() {
	var descProblema = document.getElementById('problema');
	retiraNewLine();
	
	if(descProblema.value == "") {
		alert("O 'Problema' deve ser informado.");
		descProblema.focus();
	} 
	else {
		qtdItens++;
		contador++;
		
		var tabela = document.getElementById('tabela_problema');
		
		if(tabela.style.visibility == "hidden")
			tabela.style.visibility = "visible";
		
		var linha = tabela.rows[1];
		var cor = linha.style.backgroundColor;
		
		cor = ((cor == "#f4f4f4") || (cor == "rgb(244, 244, 244)")) ? "#e0e0e0" : "#f4f4f4";
		var novaLinha = tabela.insertRow(1);
		novaLinha.style.backgroundColor = cor;
		novaLinha.setAttribute("onMouseOver","this.style.backgroundColor='#ffffcc'");
		novaLinha.setAttribute("onMouseOut","this.style.backgroundColor='"+cor+"'");
		novaLinha.id = "linha_" + contador;
		
		var acao = novaLinha.insertCell(0);
		var problema = novaLinha.insertCell(1);
		
		acao.style.textAlign = "center";
		acao.innerHTML = 
			<?// 76 = estado atual -->diagnótico ou 37 = estado atual --> aguardando correção(cadastramento) e somente podera excluir os perfis = 200, 225 ou 226
			if( $boSuperUsuario || $estadoPerm)
			{ 
				if($boSuperUsuario || $boPerfilPerm)
				{ ?>
				 "<img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Alterar' onclick='windowOpen('?modulo=principal/instrumento1/popup_alterar_fr6&acao=A&descproblema="+descProblema.value+"&contador="+contador+"',\"blank\",\"height=200,width=450,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes\");'> " +
			  	 "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick='excluiProblema(this.parentNode.parentNode.rowIndex,0);'>" +
		    <?
		    	}
			} 
		    ?>
			  	 "<input type='hidden' name='pridescricao[]' value='"+descProblema.value+"'>" +
				 "<input type='hidden' name='priid[]' value=''>";
						 
		problema.style.textAlign = "center";
		problema.innerHTML = descProblema.value;
		
		descProblema.value = "";
	}
}

function excluiProblema(idRow, priid) {
	if(confirm("Deseja realmente excluir o registro?")) {
		var tabela = document.getElementById('tabela_problema');
		tabela.deleteRow(idRow);
		qtdItens--;
		
		if(qtdItens == 0) {
			tabela.style.visibility = "hidden";
		}
		
		if(priid != 0) {
			itensExcluir += (itensExcluir == "") ? (""+priid) : (";"+priid);
		}
	}
}

--></script>