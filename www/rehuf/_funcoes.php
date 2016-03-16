<?
/**
 * Função utilizada para ordenar o posicionamento dos agrupamentos dentro dos grupositens
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[agpatual] ID do agrupamento clicado
 * @param integer $dados[agpir]    ID do agrupamento no qual a posição será trocada com o agrupamento clicado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function ordenaragrupamento($dados) {
	global $db;
	$sql = "SELECT agpordem FROM rehuf.agrupamento WHERE agpid = '". $dados['agpatual'] ."'";
	$ordematual = $db->pegaUm($sql);
	$sql = "SELECT agpordem FROM rehuf.agrupamento WHERE agpid = '". $dados['agpir'] ."'";
	$ordemir = $db->pegaUm($sql);
	$sql = "UPDATE rehuf.agrupamento SET agpordem = '". $ordemir ."', agpins=NOW() WHERE agpid = '". $dados['agpatual'] ."'";
	$db->executar($sql);
	$sql = "UPDATE rehuf.agrupamento SET agpordem = '". $ordematual ."', agpins=NOW() WHERE agpid = '". $dados['agpir'] ."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Função utilizada para ordenar o posicionamento dos grupositens dentro das tabelas
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[grupoitematual] ID do grupoitem clicado
 * @param integer $dados[grupoitemir]    ID do grupoitem no qual a posição será trocada com o grupoitem clicado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function ordenargrupoitem($dados) {
	global $db;
	$sql = "SELECT gitordem FROM rehuf.grupoitem WHERE gitid = '". $dados['grupoitematual'] ."'";
	$ordematual = $db->pegaUm($sql);
	$sql = "SELECT gitordem FROM rehuf.grupoitem WHERE gitid = '". $dados['grupoitemir'] ."'";
	$ordemir = $db->pegaUm($sql);
	$sql = "UPDATE rehuf.grupoitem SET gitordem = '". $ordemir ."', gitins=NOW() WHERE gitid = '". $dados['grupoitematual'] ."'";
	$db->executar($sql);
	$sql = "UPDATE rehuf.grupoitem SET gitordem = '". $ordematual ."', gitins=NOW() WHERE gitid = '". $dados['grupoitemir'] ."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Função utilizada para ordenar o posicionamento das colunas dentro dos grupositens
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[colunaatual] ID da coluna clicada
 * @param integer $dados[colunair]    ID da coluna no qual a posição será trocada com a coluna clicada 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function ordenarcoluna($dados) {
	global $db;
	$sql = "SELECT colordem FROM rehuf.coluna WHERE colid = '". $dados['colunaatual'] ."'";
	$ordematual = $db->pegaUm($sql);
	$sql = "SELECT colordem FROM rehuf.coluna WHERE colid = '". $dados['colunair'] ."'";
	$ordemir = $db->pegaUm($sql);
	$sql = "UPDATE rehuf.coluna SET colordem = '". $ordemir ."', colins=NOW() WHERE colid = '". $dados['colunaatual'] ."'";
	$db->executar($sql);
	$sql = "UPDATE rehuf.coluna SET colordem = '". $ordematual ."', colins=NOW() WHERE colid = '". $dados['colunair'] ."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Função utilizada para ordenar o posicionamento das linhas dentro dos grupositens
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[linhaatual] ID da linha clicada
 * @param integer $dados[linhair]    ID da linha na qual a posição será trocada com a linha clicada 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function ordenarlinha($dados) {
	global $db;
	$sql = "SELECT linordem FROM rehuf.linha WHERE linid = '". $dados['linhaatual'] ."'";
	$ordematual = $db->pegaUm($sql);
	$sql = "SELECT linordem FROM rehuf.linha WHERE linid = '". $dados['linhair'] ."'";
	$ordemir = $db->pegaUm($sql);
	$sql = "UPDATE rehuf.linha SET linordem = '". $ordemir ."', linins=NOW() WHERE linid = '". $dados['linhaatual'] ."'";
	$db->executar($sql);
	$sql = "UPDATE rehuf.linha SET linordem = '". $ordematual ."', linins=NOW() WHERE linid = '". $dados['linhair'] ."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Função utilizada carregar a lista de elementos (linhas, colunas, agrupamentos) dentro de select
 * 
 * @author Alexandre Dourado
 * @return htmlobject Combobox contendo a lista dos elementos selecionados
 * @param string $dados[tipodado]  Tipo de dado que deverá retornar
 * @param integer $dados[tabtid]   ID da tabela referente 
 * @param integer $dados[gitid]    ID do grupoitem referente
 * @param integer $dados[agpid]    ID do agrupamento
 * @param boolean $dados[islinha]  Se é linha(TRUE) ou coluna(FALSE) 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function buscardadostabela($dados) {
	global $db;
	switch($dados['tipodado']) {
		case 'grupoitem':
			$sql = "SELECT gitid as codigo, gitdsc as descricao FROM rehuf.grupoitem WHERE tabtid = '". $dados['tabtid'] ."' ORDER BY gitordem";
			$acaoordem = array('subir' => 'subirgrupoitem', 'descer' => 'descergrupoitem');
			$idselect = 'selectgrupoitem';
			$complemento = "onclick=\"window.location='?modulo=principal/gerenciarestrutura&acao=A&visetapa=etapa2&tabtid=".$_REQUEST['tabtid']."&gitid='+this.value;\"";
			break;
		case 'subcoluna':
			$sql = "SELECT colid as codigo, coldsc as descricao FROM rehuf.coluna WHERE gitid = '". $dados['gitid'] ."' AND agpid = '". $dados['agpid'] ."' ORDER BY colordem";
			$acaoordem = array('subir' => 'subirsubcoluna', 'descer' => 'descersubcoluna');
			$idselect = 'selectsubcoluna';
			$complemento = "onDblClick=\"window.open('?modulo=principal/inserirdadostabela&acao=A&op=coluna&agpid='+ document.getElementById('selectagrupamentocoluna').options[document.getElementById('selectagrupamentocoluna').selectedIndex].value +'&colid='+this.options[this.selectedIndex].value,null,'scrollbars=no,height=450,width=500,status=no,toolbar=no,menubar=no,location=no');\"";
			break;
		case 'sublinha':
			$sql = "SELECT linid as codigo, lindsc as descricao FROM rehuf.linha WHERE gitid = '". $dados['gitid'] ."' AND agpid = '". $dados['agpid'] ."' ORDER BY linordem";
			$acaoordem = array('subir' => 'subirsublinha', 'descer' => 'descersublinha');
			$idselect = 'selectsublinha';
			$complemento = "onDblClick=\"window.open('?modulo=principal/inserirdadostabela&acao=A&op=linha&agpid='+ document.getElementById('selectagrupamentolinha').options[document.getElementById('selectagrupamentolinha').selectedIndex].value +'&linid='+this.options[this.selectedIndex].value,null,'scrollbars=no,height=450,width=500,status=no,toolbar=no,menubar=no,location=no');\"";
			break;
		case 'coluna':
			$sql = "SELECT colid as codigo, coldsc as descricao FROM rehuf.coluna WHERE gitid = '". $dados['gitid'] ."' ORDER BY colordem";
			$acaoordem = array('subir' => 'subircoluna', 'descer' => 'descercoluna');
			$idselect = 'selectcoluna';
			$complemento = "onDblClick=\"window.open('?modulo=principal/inserirdadostabela&acao=A&op=coluna".(($dados['is_porano'])?"&is_porano=true":"")."&colid='+this.options[this.selectedIndex].value,null,'scrollbars=no,height=450,width=500,status=no,toolbar=no,menubar=no,location=no');\"";
			break;
		case 'linha':
			$sql = "SELECT linid as codigo, lindsc as descricao FROM rehuf.linha WHERE gitid = '". $dados['gitid'] ."' ORDER BY linordem";
			$acaoordem = array('subir' => 'subirlinha', 'descer' => 'descerlinha');
			$idselect = 'selectlinha';
			$complemento = "onDblClick=\"window.open('?modulo=principal/inserirdadostabela&acao=A&op=linha&linid='+this.options[this.selectedIndex].value,null,'scrollbars=no,height=450,width=500,status=no,toolbar=no,menubar=no,location=no');\"";
			break;
		case 'agrupamento':
			$sql = "SELECT agpid as codigo, (agpdsc ||' ('||(SELECT COUNT(".(($dados['islinha'])?'linid':'colid').") FROM rehuf.".(($dados['islinha'])?'linha':'coluna')." aux WHERE aux.agpid = agp.agpid)||')') as descricao FROM rehuf.agrupamento agp WHERE gitid = '". $dados['gitid'] ."' AND agplinha = ". (($dados['islinha'])?'TRUE':'FALSE') ."  ORDER BY agpordem";
			$acaoordem = array('subir' => 'subiragrupamento'.(($dados['islinha'])?'linha':'coluna'), 'descer' => 'desceragrupamento'.(($dados['islinha'])?'linha':'coluna'));
			$idselect = 'selectagrupamento'.(($dados['islinha'])?'linha':'coluna');
			$complemento = "onDblClick=\"window.open('?modulo=principal/inserirdadostabela&acao=A&op=agrupamento&agpid='+this.options[this.selectedIndex].value,null,'scrollbars=no,height=450,width=500,status=no,toolbar=no,menubar=no,location=no');\" onclick=\"document.getElementById('idsubnivel".(($dados['islinha'])?'linha':'coluna')."').style.display='';ajaxatualizar('requisicao=buscardadostabela&tipodado=sub".(($dados['islinha'])?'linha':'coluna')."&agpid='+this.options[this.selectedIndex].value,'sub". (($dados['islinha'])?'linha':'coluna') ."');\"";
			break;
			
		
	}
	$dadostipo = (array) $db->carregar($sql);
	$options = array();
	foreach($dadostipo as $dtipo) {
		$options[] = "<option value='". $dtipo['codigo'] ."'>". $dtipo['descricao'] ."</option>";
	}
	echo "<select id='". $idselect ."' class='CampoEstilo' style='width:300px;' size='4' ". $complemento .">
		  ". implode("", $options) ."
		  </select>
		  <img id=\"setasubir".$dados['tipodado']."\" src=\"../imagens/seta_cima.gif\" onclick=\"ordenar(document.getElementById('". $idselect ."').selectedIndex,'". $acaoordem['subir'] ."');\"> <img id=\"setadescer".$dados['tipodado']."\" src=\"../imagens/seta_baixo.gif\" onclick=\"ordenar(document.getElementById('". $idselect ."').selectedIndex,'". $acaoordem['descer'] ."');\">";
	exit;
}
/**
 * Função utilizada para carregar a lista de grupos de opções em uma combobox, utilizada na criação de linhas dinâmicas.
 * 
 * @author Alexandre Dourado
 * @return htmlobject Combobox contendo a lista dos elementos selecionados
 * @param boolean $dados[iscoluna] Se é coluna(TRUE) ou linha(FALSE)
 * @param integer $dados[gpoid] ID do grupo de opções 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function buscargrupoopcoes($dados) {
	global $db;
	if($dados['iscoluna']) {
		$gpolinha = "FALSE";
	} else {
		$gpolinha = "TRUE";
	}
	
	$grupoopcoes= $db->carregar("SELECT gpoid as codigo, gpodsc as descricao FROM rehuf.grupoopcoes WHERE gpolinha = ". $gpolinha ." ORDER BY gpodsc");
	if($grupoopcoes) {
		echo "<select name='gpoid' class='CampoEstilo'>";
		foreach($grupoopcoes as $gopcao) {
			echo "<option value='". $gopcao['codigo'] ."' ". (($gopcao['codigo']==$dados['gpoid'])?'selected':'') .">". $gopcao['descricao'] ."</option>";	
		}
		echo "</select>";
	} else {
		echo "Não existem grupos de opções cadastrados.";
	}
	exit;
}
/**
 * Função utilizada para inserir um agrupamento, utilizada no administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[gitid]          ID do grupoitem
 * @param boolean $dados[agplinha]       Se é linha(TRUE) ou coluna(FALSE)
 * @param string $dados[agpdsc]          Nome do agrupamento  
 * @param boolean $dados[agppossuitotal] Se o agrupamento possui TOTAL
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inseriragrupamento($dados) {
	global $db;
	$sql = "SELECT agpordem AS ordem FROM rehuf.agrupamento WHERE gitid = '". $dados['gitid'] ."' AND agplinha=". (($dados['agplinha']=='sim')?'TRUE':'FALSE')." ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.agrupamento(
            agpdsc, agpordem, agppossuitotal, gitid, agplinha)
    		VALUES ('". $dados['agpdsc'] ."', '". (($ordem)?($ordem+1):'1') ."', ". (($dados['agppossuitotal']=='sim')?'TRUE':'FALSE') .", '". $dados['gitid'] ."', ". (($dados['agplinha']=='sim')?'TRUE':'FALSE') .");";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.document.getElementById('idsubnivel<? echo (($dados['agplinha']=='sim')?'linha':'coluna'); ?>').style.display = 'none';
	window.opener.ajaxatualizar('requisicao=buscardadostabela<? echo (($dados['agplinha']=='sim')?'&islinha=true':''); ?>&gitid=<? echo $dados['gitid']; ?>&tipodado=agrupamento','agrupamento<? echo (($dados['agplinha']=='sim')?'linha':'coluna'); ?>');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para atualizar um agrupamento, utilizada no administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[gitid]          ID do grupoitemdeve conter o índice "", "agplinha", "agpdsc", "agppossuitotal"
 * @param boolean $dados[agplinha]       Se é linha(TRUE) ou coluna(FALSE)
 * @param string $dados[agpdsc]          Nome do agrupamento 
 * @param boolean $dados[agppossuitotal] Se o agrupamento possui TOTAL 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function atualizaragrupamento($dados) {
	global $db;
	$sql = "UPDATE rehuf.agrupamento
            SET agpins=NOW(), agpdsc = '". $dados['agpdsc'] ."', agppossuitotal = ". (($dados['agppossuitotal']=='sim')?'TRUE':'FALSE') .", gitid = '". $dados['gitid'] ."' 
    		WHERE agpid = '". $dados['agpid'] ."'";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.document.getElementById('idsubnivel<? echo (($dados['agplinha']=='sim')?'linha':'coluna'); ?>').style.display = 'none';
	window.opener.ajaxatualizar('requisicao=buscardadostabela<? echo (($dados['agplinha']=='sim')?'&islinha=true':''); ?>&gitid=<? echo $dados['gitid']; ?>&tipodado=agrupamento','agrupamento<? echo (($dados['agplinha']=='sim')?'linha':'coluna'); ?>');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para atualizar uma coluna, utilizada no administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[tpiid]         ID do tipo de item (contendo a mascara)
 * @param string $dados[coldsc]         Nome da coluna 
 * @param string $dados[colobs]         Label contendo observações sobre a coluna
 * @param boolean $dados[colpermiteobs] Se nos dados podem ser feito observações 
 * @param integer $dados[gpoid]         ID do grupo de opções
 * @param integer $dados[colid]         ID da coluna
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function atualizarcoluna($dados) {
	global $db;
	$sql = "UPDATE rehuf.coluna 
            SET tpiid='". $dados['tpiid'] ."', 
            	coldsc='". $dados['coldsc'] ."', 
            	colobs='". $dados['colobs'] ."',
            	colins=NOW(),
            	colpermiteobs= ".(($dados['colpermiteobs']=='sim')?"TRUE":"FALSE").",
            	gpoid=". (($dados['gpoid'])?"'".$dados['gpoid']."'":"NULL") ."
    		WHERE colid='". $dados['colid'] ."'";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=coluna&gitid=<? echo $dados['gitid']; ?>','coluna');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para atualizar uma linha, utilizada no administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[lindsc]        Nome da linha
 * @param string $dados[linobs]         Label contendo observações sobre a linha 
 * @param boolean $dados[linpermiteobs] Se nos dados podem ser feito observações 
 * @param integer $dados[linid]         ID da linha
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 18/12/2008
 */
function atualizarlinha($dados) {
	global $db;
	$sql = "UPDATE rehuf.linha 
            SET lindsc='". $dados['lindsc'] ."', 
            	linobs='". $dados['linobs'] ."',
            	linins=NOW(),
            	linpermiteobs=".(($dados['linpermiteobs']=='sim')?"TRUE":"FALSE")." 
    		WHERE linid='". $dados['linid'] ."'";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=linha&gitid=<? echo $dados['gitid']; ?>','linha');
	window.close();
	</script>
	<?
	exit;
}

/**
 * Função utilizada para inserir uma coluna, utilizada na administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[gitid]         ID do grupoitem
 * @param integer $dados[tpiid]         ID do tipo de item (contendo a mascara)
 * @param string $dados[coldsc]         Nome da coluna 
 * @param string $dados[colobs]         Label contendo observações sobre a coluna
 * @param boolean $dados[colpermiteobs] Se nos dados podem ser feito observações 
 * @param integer $dados[gpoid]         ID do grupo de opções
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inserircoluna($dados) {
	global $db;
	$sql = "SELECT colordem AS ordem FROM rehuf.coluna WHERE gitid = '". $dados['gitid'] ."' ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.coluna(
            gitid, agpid, tpiid, coldsc, colobs, colordem, gpoid, colpermiteobs)
    		VALUES ('". $dados['gitid'] ."', NULL, '". $dados['tpiid'] ."', '". $dados['coldsc'] ."', '". $dados['colobs'] ."', '". (($ordem)?($ordem+1):'1') ."',". (($dados['gpoid'])?"'".$dados['gpoid']."'":"NULL") .", ".(($dados['colpermiteobs']=='sim')?"TRUE":"FALSE").");";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=coluna&gitid=<? echo $dados['gitid']; ?>','coluna');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para inserir uma linha, utilizada na administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[gitid]         ID do grupoitem
 * @param string $dados[lindsc]         Nome da linha 
 * @param string $dados[linobs]         Label contendo observações sobre a linha
 * @param boolean $dados[linpermiteobs] Se nos dados podem ser feito observações 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 18/12/2008
 */
function inserirlinha($dados) {
	global $db;
	$sql = "SELECT linordem AS ordem FROM rehuf.linha WHERE gitid = '". $dados['gitid'] ."' ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.linha(
     esuid, gitid, agpid, opcid, lindsc, linobs, linordem, linpermiteobs)
     VALUES (NULL, '". $dados['gitid'] ."', NULL, NULL, '". $dados['lindsc'] ."', '". $dados['linobs'] ."', '". (($ordem)?($ordem+1):'1') ."', ".(($dados['linpermiteobs']=='sim')?"TRUE":"FALSE").");";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=linha&gitid=<? echo $dados['gitid']; ?>','linha');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para atualizar uma subcoluna (coluna dentro de uma agrupamento), utilizada no administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[tpiid]         ID do tipo de linha
 * @param string $dados[coldsc]         Nome da subcoluna
 * @param string $dados[colobs]         Label contendo observações sobre a subcoluna 
 * @param boolean $dados[colpermiteobs] Se nos dados podem ser feito observações
 * @param integer $dados[gpoid]         ID do grupo de opções
 * @param integer $dados[colid]         ID da subcoluna
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 18/12/2008
 */
function atualizarsubcoluna($dados) {
	global $db;
	$sql = "UPDATE rehuf.coluna SET tpiid='". $dados['tpiid'] ."', 
								    coldsc='". $dados['coldsc'] ."', 
								    colobs='". $dados['colobs'] ."',
								    colins=NOW(),
									colpermiteobs=".(($dados['colpermiteobs']=='sim')?"TRUE":"FALSE").",
								    gpoid=". (($dados['gpoid'])?"'".$dados['gpoid']."'":"NULL") ."
			WHERE colid='".$dados['colid']."'";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=subcoluna&agpid=<? echo $dados['agpid'] ?>&gitid=<? echo $dados['gitid']; ?>','subcoluna');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para atualizar uma sublinha (linha dentro de uma agrupamento), utilizada no administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Chamada javascript para atualizar a página pai
 * @param string $dados[lindsc]         Nome da sublinha
 * @param string $dados[linobs]         Label contendo observações sobre a sublinha 
 * @param boolean $dados[linpermiteobs] Se nos dados podem ser feito observações
 * @param integer $dados[linid]         ID da sublinha
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 18/12/2008
 */
function atualizarsublinha($dados) {
	global $db;
	$sql = "UPDATE rehuf.linha SET lindsc='". $dados['lindsc'] ."', 
								   linobs='". $dados['linobs'] ."',
								   linins=NOW(),
								   linpermiteobs=".(($dados['linpermiteobs']=='sim')?"TRUE":"FALSE")."
			WHERE linid='".$dados['linid']."'";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=sublinha&agpid=<? echo $dados['agpid'] ?>&gitid=<? echo $dados['gitid']; ?>','sublinha');
	window.close();
	</script>
	<?
	exit;
}

/**
 * Função utilizada para inserir uma subcoluna, utilizada na administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[gitid]         ID do grupoitem
 * @param integer $dados[agpid]         ID do agrupador 
 * @param integer $dados[tpiid]         ID do tipo de item (contendo a mascara)
 * @param string $dados[coldsc]         Nome da coluna 
 * @param string $dados[colobs]         Label contendo observações sobre a coluna
 * @param boolean $dados[colpermiteobs] Se nos dados podem ser feito observações 
 * @param integer $dados[gpoid]         ID do grupo de opções
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inserirsubcoluna($dados) {
	global $db;
	$sql = "SELECT colordem AS ordem FROM rehuf.coluna WHERE gitid = '". $dados['gitid'] ."' AND agpid='". $dados['agpid'] ."' ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.coluna(
            gitid, agpid, tpiid, coldsc, colobs, colordem, gpoid, colpermiteobs)
		    VALUES ('". $dados['gitid'] ."', '". $dados['agpid'] ."', '". $dados['tpiid'] ."', '". $dados['coldsc'] ."', '". $dados['colobs'] ."', '". (($ordem)?($ordem+1):'1') ."', ". (($dados['gpoid'])?"'".$dados['gpoid']."'":"NULL") .", ".(($dados['colpermiteobs']=='sim')?"TRUE":"FALSE").");";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=subcoluna&agpid=<? echo $dados['agpid'] ?>&gitid=<? echo $dados['gitid']; ?>','subcoluna');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para inserir uma sublinha, utilizada na administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[gitid]         ID do grupoitem
 * @param integer $dados[agpid]         ID do agrupador 
 * @param string $dados[lindsc]         Nome da linha 
 * @param string $dados[linobs]         Label contendo observações sobre a linha
 * @param boolean $dados[linpermiteobs] Se nos dados podem ser feito observações 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inserirsublinha($dados) {
	global $db;
	
	$sql = "SELECT linordem AS ordem FROM rehuf.linha WHERE gitid = '". $dados['gitid'] ."' AND agpid = '". $dados['agpid'] ."' ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.linha(
     esuid, gitid, agpid, opcid, lindsc, linobs, linordem, linpermiteobs)
     VALUES (NULL, '". $dados['gitid'] ."', '". $dados['agpid'] ."', NULL, '". $dados['lindsc'] ."', '". $dados['linobs'] ."', '".(($ordem)?($ordem+1):'1')."', ".(($dados['linpermiteobs']=='sim')?"TRUE":"FALSE").");";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=sublinha&agpid=<? echo $dados['agpid'] ?>&gitid=<? echo $dados['gitid']; ?>','sublinha');
	window.close();
	</script>
	<?
	exit;
}
/**
 * Função utilizada para atualizar os dados da tabela, utilizada na administração de tabelas.
 * Gerencia o controle de acesso aos anos por perfil
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param string $dados[tabtdsc]        			   Nome da tabela
 * @param string $dados[tabobs]         			   Observações da tabela 
 * @param string $dados[tabanoini]      			   Ano de inicio 
 * @param string $dados[tabanofim]      			   Ano de fim
 * @param integer $dados[dimid]         			   Dimensão da tabela 
 * @param boolean $dados[tabvisivel]    			   Deixar a tabela visível para os hospitais 
 * @param integer $dados[tabtid]        			   ID da tabela
 * @param integer $dados['pflcod']      			   Lista de perfis que terão filtro por ano 
 * @param integer $dados['filtroanoperfilinicio']      Ano de inicio do filtro 
 * @param integer $dados['filtroanoperfilfim']         Ano de fim do filtro 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function atualizartabela($dados) {
	global $db;
	$sql = "UPDATE rehuf.tabela SET tabtdsc = '". $dados['tabtdsc'] ."',
									tabobs = '". $dados['tabobs'] ."',
									tabanoini = '". $dados['tabanoini'] ."', 
									tabanofim = '". $dados['tabanofim'] ."',
									tabordem = '1',
									tabtins=NOW(),
									tabtusucpf='".$_SESSION['usucpf']."',
									dimid = '".$dados['dimid']."',
									tabvisivel=".(($dados['tabvisivel']=="sim")?"TRUE":"FALSE")."  
			WHERE tabtid = '".$dados['tabtid']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM rehuf.filtrodadosanoperfil WHERE tabtid='".$dados['tabtid']."'";
	$db->executar($sql);
	
	if($dados['pflcod'][0] && $dados['filtroanoperfilinicio'] && $dados['filtroanoperfilfim']) {
		foreach($dados['pflcod'] as $pflcod) {
			$sql = "INSERT INTO rehuf.filtrodadosanoperfil(pflcod, tabtid, anoini, anofim)
    		 		VALUES ('".$pflcod."', '".$dados['tabtid']."', '".$dados['filtroanoperfilinicio']."', '".$dados['filtroanoperfilfim']."');";
			$db->executar($sql);
		}
	}
	
	$db->commit($sql);
	echo "<script>
			alert('Atualização efetuada com sucesso!');
			window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=". $dados['visetapa'] ."&tabtid=". $dados['tabtid'] ."';
		  </script>";
	exit;
}
/**
 * Função utilizada para inserir os dados da tabela, utilizada na administração de tabelas.
 * Gerencia o controle de acesso aos anos por perfil
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param string $dados[tabtdsc]        Nome da tabela
 * @param string $dados[tabobs]         Observações da tabela 
 * @param string $dados[tabanoini]      Ano de inicio 
 * @param string $dados[tabanofim]      Ano de fim
 * @param integer $dados[dimid]         Dimensão da tabela 
 * @param boolean $dados[tabvisivel]    Deixar a tabela visível para os hospitais 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inserirtabela($dados) {
	global $db;
	$sql = "INSERT INTO rehuf.tabela(
            tabtdsc, tabobs, tabanoini, tabanofim, tabordem, dimid, tabvisivel, tabtusucpf)
    		VALUES ('". $dados['tabtdsc'] ."',
    				'". $dados['tabobs'] ."',
    				'". $dados['tabanoini'] ."',
    				'". $dados['tabanofim'] ."',
    				'1',
    				'".$dados['dimid']."',
    				".(($dados['tabvisivel']=="sim")?"TRUE":"FALSE").",
    				'".$_SESSION['usucpf']."') RETURNING tabtid;";
	$tabtid = $db->pegaUm($sql);
	
	if($dados['pflcod'][0] && $dados['filtroanoperfilinicio'] && $dados['filtroanoperfilfim']) {
		foreach($dados['pflcod'] as $pflcod) {
			$sql = "INSERT INTO rehuf.filtrodadosanoperfil(pflcod, tabtid, anoini, anofim)
    		 		VALUES ('".$pflcod."', '".$tabtid."', '".$dados['filtroanoperfilinicio']."', '".$dados['filtroanoperfilfim']."');";
			$db->executar($sql);
		}
	}
	$db->commit();
	echo "<script>
			alert('Gravação efetuada com sucesso!');
			window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=". $dados['visetapa'] ."&tabtid=". $tabtid ."';
		  </script>";
	exit;
}

/**
 * Função utilizada para remover uma tabela de dados (contedo todas as subpartes como grupo, linha, ), 
 * utilizada na administração de tabelas
 * 
 * @author Alexandre Dourado
 * @return javascriptcode chamada javascript para atualizar a página pai
 * @param integer $dados[tabtid]         ID do grupoitem
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function removertabela($dados) {
	global $db;
	
	$grupoitens = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid='".$dados['tabtid']."'");
	
	if($grupoitens[0]) {
		foreach($grupoitens as $grp) {	
			// deletando os agrupamentos caso exista
			$agrupamentos = $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid='". $grp['gitid'] ."'");
			if($agrupamentos[0]) {
				foreach($agrupamentos as $agrupamento) {
					// 	deletando sublinhas...
					$linhas = $db->carregar("SELECT * FROM rehuf.linha WHERE agpid='". $agrupamento['agpid'] ."'");
					if($linhas[0]) {
						foreach($linhas as $linha) {
							$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '".$linha['linid']."'");
							$db->executar("DELETE FROM rehuf.linha WHERE linid = '".$linha['linid']."'");
						}
					}
					// deletando subcolunas...
					$colunas = $db->carregar("SELECT * FROM rehuf.coluna WHERE agpid='". $agrupamento['agpid'] ."'");
					if($colunas[0]) {
						foreach($colunas as $coluna) {
							$db->executar("DELETE FROM rehuf.conteudoitem WHERE colid = '".$coluna['colid']."'");
							$db->executar("DELETE FROM rehuf.coluna WHERE colid = '".$coluna['colid']."'");
						}
					}
					// deletando agrupamento
					$db->executar("DELETE FROM rehuf.agrupamento WHERE agpid='". $agrupamento['agpid'] ."'");
				}
			}
			// deletando linhas...
			$linhas = $db->carregar("SELECT * FROM rehuf.linha WHERE gitid='". $grp['gitid'] ."'");
			if($linhas[0]) {
				foreach($linhas as $linha) {
					$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '".$linha['linid']."'");
					$db->executar("DELETE FROM rehuf.linha WHERE linid = '".$linha['linid']."'");
				}
			}
			// deletando colunas...
			$colunas = $db->carregar("SELECT * FROM rehuf.coluna WHERE gitid='". $grp['gitid'] ."'");
			if($colunas[0]) {
				foreach($colunas as $coluna) {
					$db->executar("DELETE FROM rehuf.conteudoitem WHERE colid = '".$coluna['colid']."'");
					$db->executar("DELETE FROM rehuf.coluna WHERE colid = '".$coluna['colid']."'");
				}
			}
			// deletando grupoitem
			$db->executar("DELETE FROM rehuf.periodogrupoitem WHERE gitid = '".$grp['gitid']."'");
			$db->executar("DELETE FROM rehuf.grupoitem WHERE gitid = '".$grp['gitid']."'");
		}
	}
	$sql = "DELETE FROM rehuf.tabela WHERE tabtid='".$dados['tabtid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Gravação efetuada com sucesso!');
			window.location = '?modulo=principal/listarestrutura&acao=A';
		  </script>";
	exit;
}
function definirobservacao($dadoscol,$linid,$ano = null,$percodigo = null) {
	global $obsgit;
	$valorobs = (($percodigo)?$obsgit[$linid][$dadoscol['colid']][$ano][$percodigo]:$obsgit[$linid][$dadoscol['colid']][$ano]);
	$observacao = "<input type='hidden' id='idobs_".$linid."_".$dadoscol['colid'].(($percodigo)?"_".$percodigo:"")."' name=\"conteudoobservacao[".$linid."][".$dadoscol['colid']."]".(($percodigo)?"[".$percodigo."]":"")."\" value='".$valorobs."'> ";
	$observacao .= "<img id='imgidobs_".$linid."_".$dadoscol['colid'].(($percodigo)?"_".$percodigo:"")."' src=\"".(($valorobs)?"../imagens/edit_on.gif":"../imagens/edit_off.gif")."\" border=\"0\" onclick=\"abreobservacao('idobs_".$linid."_".$dadoscol['colid'].(($percodigo)?"_".$percodigo:"")."');\" title=\"".$valorobs."\">";
	return $observacao;
	
}
/**
 * Função utilizada para definir o tipo de campo (input valor, input total, checkbox, select, etc)
 * 
 * @author Alexandre Dourado
 * @return string $campo Variavel contendo o objeto HTML, podendo ser inputs, selects, etc
 * @param integer $dadoscol[tpitipocampo]    Define os tipo de campos existentes (textpossuitotalagrupador, textpossuitotalcoluna, text, select, checkbox)
 * @param integer $dadoscol[agpid]           ID do agrupador
 * @param string $dadoscol[tpimascara]       Mascara utilizada 
 * @param string $dadoscol[colid]            ID da coluna
 * @param boolean $dadoscol[gpoid]           ID do grupo de opções 
 * @param integer $dadoscol[tpicampo]        Tipo do campo
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function definircampo($dadoscol,$linid,$ano = null,$permiteobservacao = false) {
	global $rspgit, $permissoes, $dadosperiodopa, $perid, $db;
	
	switch($dadoscol['tpitipocampo']) {
		case 'textpossuitotalagrupador':
		$campo = "<input id='totalizadoragp".$linid."_".$dadoscol['agpid']."' type=\"text\" value='". (($valortotal)?$valortotal:'') ."' name=\"conteudoitem[". $linid ."][texttotalagrupador]\" size=\"17\" class=\"disabled\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);\"":"") ." disabled>";
		break;
		case 'textpossuitotalcoluna':
		if($rspgit[$linid]) {
			$totalcoluna=0;
			foreach($rspgit[$linid] as $valor) {
				$ano = key($valor);
				if($perid) {
					$totalcoluna += $valor[$ano][$perid];
				} else {
					$totalcoluna += $valor[$ano];
				}
			}
			$rspgit[$linid]["totalcol"][$ano] = $totalcoluna; 
		}
		
		$campo = "<input id='totalizadorcoluna_".$linid."' type=\"text\" value='". mascaraglobal($totalcoluna,$dadoscol['tpimascara']) ."' name=\"conteudoitem[". $linid ."][texttotalcoluna]\" size=\"17\" class=\"disabled\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);totalizadorgeral(this);\"":"") ." disabled>";
		break;
		case 'textpossuitotallinha':
		$totallinha=0;
		if($rspgit) {
			if($dadosperiodopa[$ano]) {
				
				$dadoinformadoacumulado = $db->pegaUm("SELECT pgdid FROM rehuf.periodogrupoitemacumulado WHERE perano='".$ano."' AND gitid='".$dadoscol['gitid']."'");
				
				if($dadoinformadoacumulado) {
					$per = end($dadosperiodopa[$ano]);
					foreach($rspgit as $valor) {
						$totallinha += $valor[$dadoscol['colid']][$ano][$per['codigo']];
					}
					
				} else {
					foreach($dadosperiodopa[$ano] as $per) {
						foreach($rspgit as $valor) {
							$totallinha += $valor[$dadoscol['colid']][$ano][$per['codigo']];
						}
					}
				}
			} else {
				foreach($rspgit as $valor) {
					if($perid) {
						$totallinha += $valor[$dadoscol['colid']][$ano][$perid];
					} else {
						$totallinha += $valor[$dadoscol['colid']][$ano];
					}
				}
			}
		}
		$casadec = explode(",",$dadoscol['tpimascara']);
		$campo = "<input id='totalizadorlinha_".$dadoscol['colid']."' type=\"text\" value='". mascaraglobal($totallinha,$dadoscol['tpimascara']) ."' name=\"conteudoitem[". $linid ."][texttotallinha]\" size=\"17\" class=\"disabled\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);\"":"") ." disabled>";
		break;
		case 'text':
		// verificando se ha coluna totalizadora geral (última coluna)
		global $coluna, $colunapa, $linhas, $linhaDinOp;
		if($coluna) {
			$fimlinha = end($coluna);
		} elseif($colunapa) {
			$fimlinha = end($colunapa);
		}
		if($linhas) {
			$fimcoluna = end($linhas);
		} elseif($linhaDinOp) {
			$fimcoluna = end($linhaDinOp);
		}
		
		// Caso seja um totalizador no tipo subniveis
		if($fimlinha[0]) {
			$fimlinha = current($fimlinha);
			$fimparciallinha = end($coluna[$dadoscol['agpid']]);
			switch($fimparciallinha['tpitipocampo']) {
				case 'textpossuitotalagrupador':
					foreach($coluna[$dadoscol['agpid']] as $c) {
						if($c['colid']) {
							$cs[] = $c['colid'];
						}
					}
					$comp .= "calculalinhasub('".$linid."', new Array('". implode("','",$cs) ."'),".$dadoscol['agpid'].");";
					break;
			}
		}
		// Caso seja um totalizador no tipo subniveis
		if($fimcoluna[0]) {
			$fimcoluna = current($fimcoluna);
			/* A implementar caso seja necessario, somatorio por agrupadores de linha
			$fimparciallinha = end($coluna[$dadoscol['agpid']]);
			switch($fimparciallinha['tpitipocampo']) {
				case 'textpossuitotalagrupador':
					foreach($coluna[$dadoscol['agpid']] as $c) {
						if($c['colid']) {
							$cs[] = $c['colid'];
						}
					}
					$comp .= "calculalinhasub('".$linid."', new Array('". implode("','",$cs) ."'),".$dadoscol['agpid'].");";
					break;
			}
			*/
		}
		switch($fimlinha['tpitipocampo']) {
			case 'textpossuitotalcoluna':
				$comp .= "calculalinha(this);";
				break;
		}
		
		switch($fimcoluna['lintipocampo']) {
			case 'textpossuitotallinha':
				$comp .= "calculacoluna(this);";
				break;
		}
		
		// fim - verificação da coluna totalizadora
		
		switch($dadoscol['tpiid']) {
			case TPIID_NUMERO:
				if(is_array($rspgit[$linid][$dadoscol['colid']][$ano])) {
					foreach($rspgit[$linid][$dadoscol['colid']][$ano] as $perid_ => $valor) {
						$rspgit[$linid][$dadoscol['colid']][$ano][$perid_] = (integer) $valor;
					}
				} else {
						if(isset($rspgit[$linid][$dadoscol['colid']][$ano])) {
							$rspgit[$linid][$dadoscol['colid']][$ano] = (integer) $rspgit[$linid][$dadoscol['colid']][$ano];
						}
				}
				break;
		}
		// Criando estrutura por periodos quando for tabela por ano
		if($dadosperiodopa[$ano]) {
			$campo .= "<table cellpadding='2' cellspacing='0'>";
			foreach($dadosperiodopa[$ano] as $c => $per) {
				unset($campoobs);
				if($permiteobservacao) {
					$campoobs = definirobservacao($dadoscol,$linid,$dadoscol['coldsc'],$per['codigo']);
				}
			
				$campo .= "<tr>";
				$campo .= "<td onmouseover=\"return escape('".$per['descricao']."');\" nowrap>".($c+1)." :</td>";
				$campo .= "<td><input type=\"text\" id='id_".$linid."_".$dadoscol['colid']."_".$per['codigo']."' value='".mascaraglobal(trim($rspgit[$linid][$dadoscol['colid']][$ano][$per['codigo']]),$dadoscol['tpimascara'])."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."][". $per['codigo'] ."]\" size=\"17\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);".$comp."\"":"") ." ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') ." onchange=\"document.getElementById('alteracaodados').value=1;\" onfocus=\"this.select();\"></td>";
				$campo .= (($campoobs)?"<td>".$campoobs."</td>":"");
				$campo .= "</tr>";
			}
			$campo .= "</table>";
		} else {
			global $perid;
			if(is_array($rspgit[$linid][$dadoscol['colid']][$ano])) {
				if($permiteobservacao) {
					$campoobs = definirobservacao($dadoscol,$linid,$ano,$perid);
				}
				$campo = "<input type=\"text\" id='id_".$linid."_".$dadoscol['colid']."' value='".mascaraglobal($rspgit[$linid][$dadoscol['colid']][$ano][$perid],$dadoscol['tpimascara'])."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" size=\"17\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);".$comp."\"":"") ." ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') ." onchange=\"document.getElementById('alteracaodados').value=1;\" onfocus=\"this.select();\">".$campoobs;
			} else {
				if($permiteobservacao) {
					$campoobs = definirobservacao($dadoscol,$linid,$ano);
				}
				$campo = "<input type=\"text\" id='id_".$linid."_".$dadoscol['colid']."' value='".mascaraglobal($rspgit[$linid][$dadoscol['colid']][$ano],$dadoscol['tpimascara'])."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" size=\"17\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);".$comp."\"":"") ." ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') ." onchange=\"document.getElementById('alteracaodados').value=1;\" onfocus=\"this.select();\">".$campoobs;
			}
		}
		break;
		case 'select':
		global $dadoscombo;
		$campo = "<select class='CampoEstilo' style='width:100px;' id='id_".$linid."_".$dadoscol['colid']."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') .">";
		$campo .= "<option value=''>Selecione</option>";
		foreach($dadoscombo[$dadoscol['gpoid']] as $item) {
			$campo .= "<option value='".$item['codigo']."' ".(($rspgit[$linid][$dadoscol['colid']][$ano]==$item['codigo'])?"selected":"").">".$item['descricao']."</option>";
		}
		$campo .= "</select>";				
		break;
		case 'checkbox':
		global $perid;
		if($perid) {
			$campo = "<input type='hidden' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" value='FALSE'>
					  <input type='checkbox' class='CampoEstilo' id='id_".$linid."_".$dadoscol['colid']."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') ." value='TRUE' ".(($rspgit[$linid][$dadoscol['colid']][$ano][$perid]=="t")?"checked":"").">";
		} else {
			$campo = "<input type='hidden' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" value='FALSE'>
					  <input type='checkbox' class='CampoEstilo' id='id_".$linid."_".$dadoscol['colid']."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') ." value='TRUE' ".(($rspgit[$linid][$dadoscol['colid']][$ano]=="t")?"checked":"").">";
		}
		break;
	}
	return $campo;
}

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function definircamporelatorio($dadoscol,$linid,$ano = null) {
	global $rspgit, $permissoes, $dadosperiodopa, $perid, $linhaDinOp;
	switch($dadoscol['tpitipocampo']) {
		case 'textpossuitotalagrupador':
		$campo = "<input id='totalizadoragp".$linid."_".$dadoscol['agpid']."' type=\"text\" value='". (($valortotal)?$valortotal:'') ."' name=\"conteudoitem[". $linid ."][texttotalagrupador]\" size=\"17\" class=\"disabled\" ". (($dadoscol['tpimascara'])?"onKeyUp=\"this.value=mascaraglobal('" . $dadoscol['tpimascara'] . "',this.value);\"":"") ." disabled>";
		break;
		case 'textpossuitotalcoluna':
		if($rspgit[$linid]) {
			$totalcoluna=0;
			foreach($rspgit[$linid] as $valor) {
				$ano = key($valor);
				if($perid) {
					$totalcoluna += $valor[$ano][$perid];
				} else {
					if(!is_array($valor[$ano]))
						$totalcoluna += $valor[$ano];
				}
			}
			$rspgit[$linid]["totalcol"][$ano] = $totalcoluna; 
		}
		$campo = mascaraglobal($totalcoluna,$dadoscol['tpimascara']);
		break;
		case 'textpossuitotallinha':
			$totallinha=0;
			
			if($rspgit) {
				
				if($dadosperiodopa[$ano]) {
					foreach($dadosperiodopa[$ano] as $per) {
						if($linhaDinOp[0]) {
							
							foreach($linhaDinOp as $valor) {
								$totallinha += (($rspgit[$valor['linid']][$dadoscol['colid']][$ano][$per['codigo']])?$rspgit[$valor['linid']][$dadoscol['colid']][$ano][$per['codigo']]:0);
							}
							
						} else {
							
							foreach($rspgit as $valor) {
								$totallinha += (($valor[$dadoscol['colid']][$ano][$per['codigo']])?$valor[$dadoscol['colid']][$ano][$per['codigo']]:0);
							}
						
						}
					}
				} else {
					foreach($rspgit as $valor) {
						if($perid) {
							$totallinha += $valor[$dadoscol['colid']][$ano][$perid];
						} else {
							$totallinha += $valor[$dadoscol['colid']][$ano];
						}
					}
				}
			}
			$campo = mascaraglobal($totallinha,$dadoscol['tpimascara']);
		break;
		case 'text':
		// verificando se ha coluna totalizadora geral (última coluna)
		global $coluna, $colunapa, $linhas, $linhaDinOp;
		if($coluna) {
			$fimlinha = end($coluna);
		} elseif($colunapa) {
			$fimlinha = end($colunapa);
		}
		if($linhas) {
			$fimcoluna = end($linhas);
		} elseif($linhaDinOp) {
			$fimcoluna = end($linhaDinOp);
		}
		
		// Caso seja um totalizador no tipo subniveis
		if($fimlinha[0]) {
			$fimlinha = current($fimlinha);
			$fimparciallinha = end($coluna[$dadoscol['agpid']]);
			switch($fimparciallinha['tpitipocampo']) {
				case 'textpossuitotalagrupador':
					foreach($coluna[$dadoscol['agpid']] as $c) {
						if($c['colid']) {
							$cs[] = $c['colid'];
						}
					}
					$comp .= "calculalinhasub('".$linid."', new Array('". implode("','",$cs) ."'),".$dadoscol['agpid'].");";
					break;
			}
		}
		
			// Caso seja um totalizador no tipo subniveis
		if($fimcoluna[0]) {
			$fimcoluna = current($fimcoluna);
			/* A implementar caso seja necessario, somatorio por agrupadores de linha
			$fimparciallinha = end($coluna[$dadoscol['agpid']]);
			switch($fimparciallinha['tpitipocampo']) {
				case 'textpossuitotalagrupador':
					foreach($coluna[$dadoscol['agpid']] as $c) {
						if($c['colid']) {
							$cs[] = $c['colid'];
						}
					}
					$comp .= "calculalinhasub('".$linid."', new Array('". implode("','",$cs) ."'),".$dadoscol['agpid'].");";
					break;
			}
			*/
		}
		
		switch($fimlinha['tpitipocampo']) {
			case 'textpossuitotalcoluna':
				$comp .= "calculalinharelatorio(this);";
				break;
		}
		
		switch($fimcoluna['lintipocampo']) {
			case 'textpossuitotallinha':
				$comp .= "calculacolunarelatorio(this);";
				break;
		}
		// Criando estrutura por periodos quando for tabela por ano
		if(count($dadosperiodopa[$ano])>1) {
			$campo .= "<table cellpadding='0' cellspacing='0' width='100%'>";
			foreach($dadosperiodopa[$ano] as $c => $per) {
				$campo .= "<tr>";
				$campo .= "<td style='font-size:10px;' nowrap>".$per['descricao']." :</td>";
				$campo .= "<td align='right'>".mascaraglobal($rspgit[$linid][$dadoscol['colid']][$ano][$per['codigo']],$dadoscol['tpimascara'])."</td>";
				$campo .= "</tr>";
			}
			$campo .= "</table>";
		} elseif(count($dadosperiodopa[$ano])==1) {
			$per = current($dadosperiodopa[$ano]);	
			$campo .= mascaraglobal($rspgit[$linid][$dadoscol['colid']][$ano][$per['codigo']],$dadoscol['tpimascara']);
		} else {
			if(!is_array($rspgit[$linid][$dadoscol['colid']][$ano]))
				$campo = mascaraglobal($rspgit[$linid][$dadoscol['colid']][$ano],$dadoscol['tpimascara']);
		}
		break;
		case 'select':
		global $dadoscombo;
		$campo = "<select class='CampoEstilo' style='width:100px;' id='id_".$linid."_".$dadoscol['colid']."' name=\"conteudoitem[" . $linid . "][" . $dadoscol['colid'] . "][". $dadoscol['tpicampo'] ."]\" ". ((!$permissoes['gravar'])?'class="disabled" disabled':'class="normal"') .">";
		$campo .= "<option value=''>Selecione</option>";
		foreach($dadoscombo[$dadoscol['gpoid']] as $item) {
			$campo .= "<option value='".$item['codigo']."' ".(($rspgit[$linid][$dadoscol['colid']][$ano]==$item['codigo'])?"selected":"").">".$item['descricao']."</option>";
		}
		$campo .= "</select>";				
		break;
		case 'checkbox':
		// Criando estrutura por periodos quando for tabela por ano
		if(count($dadosperiodopa[$ano]) > 1) {
			$campo .= "<table cellpadding='0' cellspacing='0' width='100%'>";
			foreach($dadosperiodopa[$ano] as $c => $per) {
				$campo .= "<tr>";
				$campo .= "<td style='font-size:10px;' nowrap>".$per['descricao']." :</td>";
				$campo .= "<td align='right'>".(($rspgit[$linid][$dadoscol['colid']][$ano][$per['codigo']]=="t")?"Sim":"Não")."</td>";
				$campo .= "</tr>";
			}
			$campo .= "</table>";
		} elseif(count($dadosperiodopa[$ano])==1) {
			$per = current($dadosperiodopa[$ano]);	
			$campo = (($rspgit[$linid][$dadoscol['colid']][$ano][$per['codigo']]=="t")?"Sim":"Não");
		} else {
			$campo = (($rspgit[$linid][$dadoscol['colid']][$ano]=="t")?"Sim":"Não");
		}
		break;
	}
	return $campo;
}
/**
 * Função utilizada inserir o grupo de uma tabela
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Redireciona para a página de gerenciamento de estrutura
 * @param integer $dados[tabtid]             ID da tabela
 * @param integer $dadoscol[gpoid]           ID do grupo de opções (se o grupo tiver linhas contendo opções, mostrar o tipo de grupo)
 * @param integer $dadoscol[tpgidlinha]       ID do tipo de linha 
 * @param integer $dadoscol[tpgidcoluna]      ID do tipo de coluna
 * @param string $dadoscol[gitdsc]          Título do grupo 
 * @param string $dadoscol[gitobs]        Observação sobre o grupo
 * @param bollean $dadoscol[gitpossuitotallinha]        Verifica se o grupo possui linha totalizadora
 * @param bollean $dadoscol[gitpossuitotalcoluna]        Verifica se o grupo possui coluna totalizadora  
 * @param array $dados['perdsc']        Dados dos períodos 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inserirgrupoitem($dados) {
	global $db;
	$sql = "SELECT MAX(gitordem) AS ordem FROM rehuf.grupoitem WHERE tabtid = '". $dados['tabtid'] ."'";
	$ordem = $db->pegaUm($sql);
	
	$sql = "INSERT INTO rehuf.grupoitem(
            tabtid, gpoid, tpgidlinha, tpgidcoluna, gitdsc, gitobs, 
            gitordem, gitpossuitotallinha, gitpossuitotalcoluna, gitvisivel)
    		VALUES ('". $dados['tabtid'] ."', ". (($dados['gpoid'])?"'".$dados['gpoid']."'":'NULL') .", '". $dados['tpgidlinha'] ."', '". $dados['tpgidcoluna'] ."', '". $dados['gitdsc'] ."', '". $dados['gitobs'] ."', 
            '".(($ordem)?($ordem+1):'1')."', ". (($dados['gitpossuitotallinha']=='sim')?'true':'false') .",". (($dados['gitpossuitotalcoluna']=='sim')?'true':'false') .", ". (($dados['gitvisivel']=='sim')?'true':'false') .") RETURNING gitid;";
	$gitid = $db->pegaUm($sql);
	
	if($dados['perdsc']) {
		foreach($dados['perdsc'] as $ac => $arrano) {
			foreach($arrano as $ano => $arrperdsc) {
				switch($ac) {
					case 'ins':
						foreach($arrperdsc as $perdsc) {
							$sql = "INSERT INTO rehuf.periodogrupoitem(gitid, perano, perdsc) VALUES ('".$gitid."', '".$ano."', '".$perdsc."');";
							$db->executar($sql);
						}
						break;
					case 'atu':
						foreach($arrperdsc as $perid => $perdsc) {
							$sql = "UPDATE rehuf.periodogrupoitem SET perins=NOW(), perdsc='".$perdsc."' WHERE perid='".$perid."'";
							$db->executar($sql);
						}
						break;
				}
			}
		}
	}
	
	$db->commit();
	echo "<script>
			alert('Gravação efetuada com sucesso!');
			window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=". $dados['visetapa'] ."&tabtid=". $dados['tabtid'] ."&gitid=". $gitid ."';
		  </script>";
	exit;
}

function atualizargrupoitem($dados) {
	global $db;
	
	$sql = "UPDATE rehuf.grupoitem SET gpoid = ". (($dados['gpoid'])?"'".$dados['gpoid']."'":'NULL') .", 
									   tpgidlinha = '". $dados['tpgidlinha'] ."', 
									   tpgidcoluna = '". $dados['tpgidcoluna'] ."', 
									   gitdsc = '". $dados['gitdsc'] ."', 
									   gitobs = '". $dados['gitobs'] ."',
									   gitins = NOW(),
									   gitpossuitotallinha  = ". (($dados['gitpossuitotallinha']=='sim')?'true':'false') .",
									   gitpossuitotalcoluna = ". (($dados['gitpossuitotalcoluna']=='sim')?'true':'false') .",
									   gitvisivel           = ". (($dados['gitvisivel']=='sim')?'true':'false') ." 
			WHERE gitid = '". $dados['gitid'] ."'";
	
	$db->executar($sql);
	if($dados['perdsc']) {
		foreach($dados['perdsc'] as $ac => $arrano) {
			foreach($arrano as $ano => $arrperdsc) {
				switch($ac) {
					case 'ins':
						foreach($arrperdsc as $perdsc) {
							$sql = "INSERT INTO rehuf.periodogrupoitem(gitid, perano, perdsc) VALUES ('".$dados['gitid']."', '".$ano."', '".$perdsc."');";
							$db->executar($sql);
						}
						break;
					case 'atu':
						foreach($arrperdsc as $perid => $perdsc) {
							$sql = "UPDATE rehuf.periodogrupoitem SET perins=NOW(), perdsc='".$perdsc."' WHERE perid='".$perid."'";
							$db->executar($sql);
						}
						break;
				}
			}
		}
	}
	
	$sql = "DELETE FROM rehuf.periodogrupoitemacumulado WHERE gitid='".$dados['gitid']."'";
	$db->executar($sql);
	
	if($dados['pgd']) {
		foreach($dados['pgd'] as $key => $vl) {
			if($vl) {
				$sql = "INSERT INTO rehuf.periodogrupoitemacumulado(gitid, perano)
	    				VALUES ('".$dados['gitid']."', '".$key."')";
				$db->executar($sql);
			}
		}
	}
	
	$db->commit();	
	
	echo "<script>
			alert('Atualização efetuada com sucesso!');
			window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=". $dados['visetapa'] ."&tabtid=". $dados['tabtid'] ."&gitid=". $dados['gitid'] ."';
		  </script>";
	exit;
}

function pegarEstadoDocumento($docid) {
	global $db;
	$docid = (integer) $docid;
	$sql = "SELECT esdid FROM workflow.documento WHERE docid = '".$docid."'";
	return (integer) $db->pegaUm( $sql );
}


function pegarDocid($entid) {
	global $db;
	$entid = (integer) $entid;
	$sql = "SELECT docid FROM rehuf.estruturaunidade WHERE entid = '".$entid."'";
	return (integer) $db->pegaUm( $sql );
}

function criarDocumento($entid) {
	global $db;
	if( ! pegarDocid($entid)){
		$sqlTpdid = "SELECT t.tpdid 
					 FROM seguranca.sistema s					
					 INNER JOIN workflow.tipodocumento t ON s.sisid = t.sisid					
					 WHERE s.sisabrev = 'REHUF'";
		$tpdid = $db->pegaUm( $sqlTpdid );
		$sqlDescricao = "SELECT	entnome FROM entidade.entidade WHERE entid = '".$entid."'";
		$descricao = $db->pegaUm( $sqlDescricao );
		$docdsc = "Cadastramento REHUF - " . $descricao;
		// cria documento
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		$sql = "INSERT INTO rehuf.estruturaunidade (entid, docid, usucpf) 
				VALUES ('".$entid."', '".$docid."', '".$_SESSION['usucpf']."')";	
		$db->executar( $sql );		
		$db->commit();
		return $docid;		
	}
		
}

function inserirlinhadintx($dados) {
	global $db;
	if($dados['ano']) {
		$linano = array(", linano",",'".$dados['ano']."'");
	}
	if($dados['perid']) {
		$perid = array(", perid",",'".$dados['perid']."'");
	}
	$sql = "SELECT linordem AS ordem FROM rehuf.linha WHERE gitid = '". $dados['gitid'] ."' ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	if($_SESSION['rehuf_var']['esuid']) {
		$sql = "INSERT INTO rehuf.linha(
    			esuid, gitid, agpid, opcid, lindsc, linobs, linordem ".$linano[0]." ".$perid[0].")
     			VALUES ('". $_SESSION['rehuf_var']['esuid'] ."', '". $dados['gitid'] ."', NULL, NULL, '". $dados['lindsc'] ."', NULL, '". (($ordem)?($ordem+1):'1') ."' ".$linano[1]." ".$perid[1].") RETURNING linid;";
		$db->executar($sql);
		$db->commit();
	}
	exit;
}
/**
 * Função utilizada para remover um linha do tipo dinâmica com texto
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[linid] ID da linha
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function removerlinhadintx($dados) {
	global $db;
	if($dados['linid']) {
		$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '". $dados['linid'] ."' AND esuid = '".$_SESSION['rehuf_var']['esuid']."'");
		$db->executar("DELETE FROM rehuf.linha WHERE linid = '". $dados['linid'] ."' AND esuid = '".$_SESSION['rehuf_var']['esuid']."'".(($dados['ano'])?" AND linano='".$dados['ano']."'":"").(($dados['perid'])?" AND perid='".$dados['perid']."'":""));
		$db->commit();
	}
	exit;
}

/**
 * Função utilizada para inserir uma linha do tipo dinâmica com opções
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[ano] Ano que será inserido a linha 
 * @param integer $dados[gitid] ID do grupoitem
 * @param integer $dados[opcid] ID da opção selecionada 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function inserirlinhadinop($dados) {
	global $db;
	if($dados['ano']) {
		$linano = array(", linano",",'".$dados['ano']."'");
	}
	if($dados['perid']) {
		$perid = array(", perid",",'".$dados['perid']."'");
	}
	
	$sql = "SELECT linordem AS ordem FROM rehuf.linha WHERE esuid='". $_SESSION['rehuf_var']['esuid'] ."' AND gitid = '". $dados['gitid'] ."' ORDER BY ordem DESC LIMIT 1";
	$ordem = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.linha(
     esuid, gitid, agpid, opcid, lindsc, linobs, linordem ".$linano[0]." ".$perid[0].")
     VALUES ('". $_SESSION['rehuf_var']['esuid'] ."', '". $dados['gitid'] ."', NULL, '". $dados['opcid'] ."', '', NULL, '". (($ordem)?($ordem+1):'1') ."' ".$linano[1]." ".$perid[1].") RETURNING linid;";
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Função utilizada para remover um linha do tipo dinâmica com opções
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[gitid] ID do grupoitem
 * @param integer $dados[opcid] ID da opção selecionada 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/12/2008
 */
function removerlinhadinop($dados) {
	global $db;
	$linid = $db->pegaUm("SELECT linid FROM rehuf.linha WHERE gitid='".$dados['gitid']."' AND esuid='".$_SESSION['rehuf_var']['esuid']."' AND opcid='".$dados['opcid']."' AND linano='".$dados['ano']."'".(($dados['perid'])?" AND perid='".$dados['perid']."'":""));
	/*
	 * Adaptação na MUDANÇA do mecanismo
	 */
	if(!$linid)
		$linid = $db->pegaUm("SELECT linid FROM rehuf.linha WHERE gitid='".$dados['gitid']."' AND esuid='".$_SESSION['rehuf_var']['esuid']."' AND opcid='".$dados['opcid']."' AND linano IS NULL");
	if($linid) {
		$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '". $linid ."'");
		$db->executar("DELETE FROM rehuf.linha WHERE linid = '". $linid ."'");
		$db->commit();
	}
	exit;
}
/**
 * Função utilizada para inserir os dados das tabelas, incluindo as modificações nas linhas dinâmicas 
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Mensagem de confirmação e redirecionamento da página
 * @param array $dados[conteudoitem]   Contem todos os dados a serem inseridos (linid, colid, valor)
 * @param integer $dados[ctiexercicio] Ano em que os dados se referem
 * @param integer $dados[gitid]        ID do grupoitem 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 18/12/2008
 */
function inserirconteudoitem($dados) {
	global $db;

	// Verifica se existe conteúdo a ser inserido
	if($dados['conteudoitem']) {
		foreach($dados['conteudoitem'] as $linid => $dados1) {
			// Verifica se a linha é númerica
			if(is_numeric($linid)) {
				$existelinha = $db->pegaUm("SELECT linid FROM rehuf.linha WHERE linid='".$linid."'");
				// Verifica se a linha existe
				if($existelinha) {
					// Deleta todos os registros da linha
					$db->executar("DELETE FROM rehuf.conteudoitem WHERE esuid='".(($dados['esuid'])?$dados['esuid']:$_SESSION['rehuf_var']['esuid'])."' AND linid='". $linid ."' ".(($dados['perid'])?"AND (perid='".$dados['perid']."' OR perid IS NULL) ":"")." ".(($dados['ctiexercicio'])?"AND ctiexercicio='".$dados['ctiexercicio']."'":"AND ctiexercicio IN('".implode("','",$dados['anoexercicioitem'][$linid])."')"));
					// Atualiza a opção da linha
					if($dados['opcid_'.$linid]) {
						$db->executar("UPDATE rehuf.linha SET linins=NOW(), opcid='". $dados['opcid_'.$linid] ."' WHERE linid='". $linid ."'");
					}
					// Atualiza a descrição da linha
					if($dados['lindsc_'.$linid]) {
						$db->executar("UPDATE rehuf.linha SET linins=NOW(), lindsc='". $dados['lindsc_'.$linid] ."' WHERE linid='". $linid ."'");
					}
					foreach($dados1 as $colid =>$dados2) {
						if(is_numeric($colid)) {
							$existecoluna = $db->pegaUm("SELECT colid FROM rehuf.coluna WHERE colid='".$colid."'");
							// Verifica se a coluna existe
							if($existecoluna) {
								if(is_array($dados2[current(array_keys($dados2))])) {
									$campo = current(array_keys($dados2));
									foreach($dados2[current(array_keys($dados2))] as $perid => $valor) {
										if(($valor !== "") || $_REQUEST['conteudoobservacao'][$linid][$colid]) {
											$sql = "INSERT INTO rehuf.conteudoitem(
		    	       				 			   ".$campo.", ctiobs, ctiexercicio, esuid, ctistatus, linid, colid, perid)
		   						 			    	VALUES (". $campo($valor) .", '".substr($_REQUEST['conteudoobservacao'][$linid][$colid][$perid],0,500)."', '". (($dados['ctiexercicio'])?$dados['ctiexercicio']:$dados['anoexercicioitem'][$linid][$colid]) ."', '". (($dados['esuid'])?$dados['esuid']:$_SESSION['rehuf_var']['esuid']) ."', 'A', '". $linid ."', '". $colid ."','".$perid."');";
											$db->executar($sql);
										}
									}
								} elseif(($dados2[current(array_keys($dados2))] !== "") || $_REQUEST['conteudoobservacao'][$linid][$colid]) {
									$campo = current(array_keys($dados2));
									$sql = "INSERT INTO rehuf.conteudoitem(
    	       				 			   ".$campo.", ctiobs, ctiexercicio, esuid, ctistatus, linid, colid, perid)
   						 			    	VALUES (". $campo($dados2[current(array_keys($dados2))]) .", '".(($dados['perid'])?substr($_REQUEST['conteudoobservacao'][$linid][$colid][$dados['perid']],0,500):substr($_REQUEST['conteudoobservacao'][$linid][$colid],0,500))."', '". (($dados['ctiexercicio'])?$dados['ctiexercicio']:$dados['anoexercicioitem'][$linid][$colid]) ."', '". (($dados['esuid'])?$dados['esuid']:$_SESSION['rehuf_var']['esuid']) ."', 'A', '". $linid ."', '". $colid ."', ".(($dados['perid'])?"'".$dados['perid']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
					}
				}
			}
		}
	}
	if($_SESSION['rehuf_var']['esuid']) {
		/*
		 * Efetuando o log por tabela e grupo
		 */
		$db->executar("INSERT INTO rehuf.logtabelasgrupos(usucpf, tabtid, esuid, ltadata, gitid, ltalog)
    				   VALUES ('".$_SESSION['usucpf']."', '".$dados['tabtid']."', '".(($dados['esuid'])?$dados['esuid']:$_SESSION['rehuf_var']['esuid'])."', NOW(), '".$dados['gitid']."', '".addslashes(simec_json_encode($_REQUEST))."');");
		
		$db->commit();
		echo "<script>
				alert('Gravação efetuada com sucesso!');
				window.location = '?modulo=principal/editartabela&acao=A&gitid=".$dados['gitid']."&ano=". $dados['ctiexercicio'] ."&tabtid=". $dados['tabtid']."".(($dados['perid'])?"&perid=".$dados['perid']:"")."';
		  	  </script>";
	} else {
		$db->rollback();
		echo "<script>
				alert('Problemas encontrados na gravação!');
				window.location = '?modulo=inicio&acao=C';
		  	  </script>";
	}
	exit;
}
/**
 * Função que valida se o valor é númerico ou float 
 * 
 * @author Alexandre Dourado
 * @return string No formato utilizado em um INSERT(entre aspas quando houver, ou NULL quando estiver vazia)
 * @param integer $valor 
 * @version v1.0 18/12/2008
 */
function ctivalor($valor) {
	$valor = str_replace(array(".",","),array("","."),$valor);
	if(is_float($valor) || is_numeric($valor)) {
		return "'".$valor."'"; 
	} else {
		return "NULL";
	}
}
/**
 * Função que valida se o valor é do tipo opcid (não testa nenhum tipo) 
 * 
 * @author Alexandre Dourado
 * @return integer 
 * @param integer $valor 
 * @version v1.0 18/12/2008
 */
function opcid($valor) {
	return trim($valor);	
}
/**
 * Função que valida se o valor é do tipo bolean (não testa nenhum tipo) 
 * 
 * @author Alexandre Dourado
 * @return bolean 
 * @param bolean $valor 
 * @version v1.0 18/12/2008
 */
function ctibooleano($valor) {
	return trim($valor);
}
/**
 * Função que controla as permissões dentro do sistema a partir do perfil. Todas varivaveis contendo 
 * informações deverão ser atribuidas dentro desta função afim de centralizar as regras de permissões 
 * 
 * @author Alexandre Dourado
 * @return array $permissoes Contendo todas as informações sobre permissão do perfil
 * @param integer $estid Estado do documento
 * @param integer $tabtid ID da tabela 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 18/12/2008
 */
function verificaPerfilRehuf($estid = false,$tabtid = false) {
	global $db;
	$sql = "SELECT p.pflcod FROM seguranca.perfil p 
			LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". SISID ."' 
			ORDER BY pflnivel DESC";
	$perfilids = $db->carregar($sql);
	
	if($perfilids[0]) {
		
		foreach($perfilids as $pfl) {
			
			$perfilid = $pfl['pflcod'];
	
			if($tabtid) {
				$sql = "SELECT * FROM rehuf.filtrodadosanoperfil WHERE tabtid='".$tabtid."' AND pflcod='".$perfilid."'";
				$filtrodadosanoperfil = $db->pegaLinha($sql);
				if($filtrodadosanoperfil) {
					$permissoes['filtrodadosano'] = array('anoini'=>$filtrodadosanoperfil['anoini'],'anofim'=>$filtrodadosanoperfil['anofim']);
				}
			}
			
			if($db->testa_superuser() ||
			   $perfilid == PRF_ADMREHUF ||
			   $perfilid == PRF_EQTECMEC) {
			   	// seleciona todos os hospitais
				$sql = "SELECT ent.entid, ent.entnome FROM entidade.entidade ent 
						LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
						WHERE fen.funid IN ('".HOSPITALUNIV."','".HOSPITALFEDE."') ORDER BY ent.entnome";
				$entids = (array) $db->carregar($sql);
				if($entids[0]) {
					foreach($entids as $ent) {
						$permissoes['verhospitais'][] = $ent['entid'];
					}
				}
				// permissão para atualizar preenchimento do pregão
				$permissoes['gravarpregao'] = true;
				// permissao para remover e gravar
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['alterarsituacaocad'] = true;
			} else {
				// Analisando os hospitais que ele pode acessar
				switch($perfilid) {
					case PRF_CONSULTAMEC:
						// seleciona todos os hospitais
						$sql = "SELECT ent.entid FROM entidade.entidade ent 
								LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
								WHERE fen.funid IN('".HOSPITALUNIV."','".HOSPITALFEDE."')";
						$entids = (array) $db->carregar($sql);
						if($entids[0]) {
							foreach($entids as $ent) {
								$permissoes['verhospitais'][] = $ent['entid'];
							}
						}
						break;
					case PRF_GESTORHF:
					case PRF_CONSULTAHU:
					case PRF_EQAPOIOHU:
					case PRF_GESTORHU:
						// seleciona apenas os hospitais cadatrados para o usuario
						$sql = "SELECT entid FROM rehuf.usuarioresponsabilidade WHERE pflcod = '". $perfilid ."' AND usucpf = '". $_SESSION['usucpf'] ."' AND rpustatus = 'A'";
						$entids = (array) $db->carregar($sql);
						if($entids[0]) {
							foreach($entids as $ent) {
								$permissoes['verhospitais'][] = $ent['entid'];
								$acesso['hosp_'.$perfilid][] = $ent['entid'];
							}
						}
						break;
				}
				// Analisando permissão de acesso de acordo com o estado do documento
				switch($perfilid) {
					case PRF_EQTECMEC:
						$permissoes['alterarsituacaocad'] = false;
						// permissão para atualizar preenchimento do pregão
						$permissoes['gravarpregao'] = false;
						// analisando estado do documento 
						switch($estid) {
							default:
								// Solicitação feita por Marco Antonio Avelino 8/9/2009
								// * Equipe Tecnica do MEC deve alterar tudo com excessão das tabelas 
								$permissoes['remover'] = false;
								$permissoes['gravar'] = false;
						}
						// fim da analise do documento
						break;
					case PRF_GESTORHF:
						$permissoes['hospfederal'] = true;
					case PRF_GESTORHU:
						$permissoes['alterarsituacaocad'] = false;
						// permissão para atualizar preenchimento do pregão
						if($_SESSION['rehuf_var']['preid']) {
							$permissoes['gravarpregao'] = $db->pegaUm("SELECT preid FROM rehuf.pregao p WHERE p.predatafinalpreenchimento >= to_date(to_char(now(),'YYYY-MM-DD'),'YYYY-MM-DD') AND p.predatainicialpreenchimento <= to_date(to_char(now(),'YYYY-MM-DD'),'YYYY-MM-DD') AND preid='".$_SESSION['rehuf_var']['preid']."'");
						} else {
							$permissoes['gravarpregao'] = false;
						}
						// analisando estado do documento 
						switch($estid) {
							case DOC_APROVACAOHU:
							case DOC_CADHU:
								$permissoes['remover'] = true;
								$permissoes['gravar'] = true;
								break;
							default:
								$permissoes['remover'] = false;
								$permissoes['gravar'] = false;
						}
						// fim da analise do documento
						break;
					case PRF_EQAPOIOHU:
						$permissoes['alterarsituacaocad'] = false;
						// permissão para atualizar preenchimento do pregão
						if($_SESSION['rehuf_var']['preid']) {
							$permissoes['gravarpregao'] = $db->pegaUm("SELECT preid FROM rehuf.pregao p WHERE p.predatafinalpreenchimento >= to_date(to_char(now(),'YYYY-MM-DD'),'YYYY-MM-DD') AND p.predatainicialpreenchimento <= to_date(to_char(now(),'YYYY-MM-DD'),'YYYY-MM-DD') AND preid='".$_SESSION['rehuf_var']['preid']."'");
						} else {
							$permissoes['gravarpregao'] = false;
						}
						// analisando estado do documento
						switch($estid) {
							case DOC_CADHU:
								$permissoes['remover'] = false;
								$permissoes['gravar'] = false;
								
								if($acesso['hosp_'.PRF_EQAPOIOHU]) {
									foreach($acesso['hosp_'.PRF_EQAPOIOHU] as $pf) {
										if($_SESSION['rehuf_var']['entid'] == $pf) {
											$permissoes['remover'] = true;
											$permissoes['gravar'] = true;
										}
									}
								}
								
								break;
							default:
								$permissoes['remover'] = false;
								$permissoes['gravar'] = false;
						}
						// fim da analise do documento
						break;
					case PRF_CONSULTAMEC:
					case PRF_CONSULTAHU:
						$permissoes['alterarsituacaocad'] = false;
						// permissão para atualizar preenchimento do pregão
						$permissoes['gravarpregao'] = false;
						$permissoes['remover'] = false;
						$permissoes['gravar'] = false;
						break;
					default:
						// permissão para atualizar preenchimento do pregão
						$permissoes['alterarsituacaocad'] = false;
						$permissoes['gravarpregao'] = false;
						$permissoes['remover'] = false;
						$permissoes['gravar'] = false;
				}
			}
		}
	
	}
	return $permissoes;
}

function pegaArrayPerfil(){

	global $db;

	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '".SISID."'
				AND pu.usucpf = '".$_SESSION['usucpf']."'";

	$pflcod = $db->carregar( $sql );

	foreach($pflcod as $dados){
		$arPflcod[] = $dados['pflcod'];
	}

	return $arPflcod;
}

function validaAcessoHospital($permissoes, $entid) {
	$permissoes = array_flip($permissoes);
	if(!isset($permissoes[$entid])) {
		die("<script>
				alert('Você não possui autorização para acessar o HOSPITAL.');
				window.location = '?modulo=inicio&acao=C';
			 </script>");
	}
}


function removergrupoitem($dados) {
	global $db;
	$agrupamentos = $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid='". $dados['gitid'] ."'");
	if($agrupamentos[0]) {
		foreach($agrupamentos as $agrupamento) {
			// Linhas...
			$linhas = $db->carregar("SELECT * FROM rehuf.linha WHERE agpid='". $agrupamento['agpid'] ."'");
			if($linhas[0]) {
				foreach($linhas as $linha) {
					$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '".$linha['linid']."'");
					$db->executar("DELETE FROM rehuf.linha WHERE linid = '".$linha['linid']."'");
				}
			}
			// Colunas...
			$colunas = $db->carregar("SELECT * FROM rehuf.coluna WHERE agpid='". $agrupamento['agpid'] ."'");
			if($colunas[0]) {
				foreach($colunas as $coluna) {
					$db->executar("DELETE FROM rehuf.conteudoitem WHERE colid = '".$coluna['colid']."'");
					$db->executar("DELETE FROM rehuf.coluna WHERE colid = '".$coluna['colid']."'");
				}
			}
			$db->executar("DELETE FROM rehuf.agrupamento WHERE agpid='". $agrupamento['agpid'] ."'");
		}
	}
	// Linhas...
	$linhas = $db->carregar("SELECT * FROM rehuf.linha WHERE gitid='". $dados['gitid'] ."'");
	if($linhas[0]) {
		foreach($linhas as $linha) {
			$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '".$linha['linid']."'");
			$db->executar("DELETE FROM rehuf.linha WHERE linid = '".$linha['linid']."'");
		}
	}
	// Colunas...
	$colunas = $db->carregar("SELECT * FROM rehuf.coluna WHERE gitid='". $dados['gitid'] ."'");
	
	if($colunas[0]) {
		foreach($colunas as $coluna) {
			$db->executar("DELETE FROM rehuf.conteudoitem WHERE colid = '".$coluna['colid']."'");
			$db->executar("DELETE FROM rehuf.coluna WHERE colid = '".$coluna['colid']."'");
		}
	}
	$db->executar("DELETE FROM rehuf.periodogrupoitem WHERE gitid = '".$dados['gitid']."'");
	$db->executar("DELETE FROM rehuf.grupoitem WHERE gitid = '".$dados['gitid']."'");
	$db->commit();
	echo "<script>
			alert('Remoção foi efetuada com sucesso.');
			window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=etapa2&tabtid=".$dados['tabtid']."';
		  </script>";
	exit;
}

function removeragrupamento($dados) {
	global $db;
	$linhas = $db->carregar("SELECT * FROM rehuf.linha WHERE agpid='".$dados['agpid']."'");
	if($linhas[0]) {
		foreach($linhas as $linha) {
			$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '".$linha['linid']."'");
			$db->executar("DELETE FROM rehuf.linha WHERE linid = '".$linha['linid']."'");
		}
	}
	$colunas = $db->carregar("SELECT * FROM rehuf.coluna WHERE agpid='".$dados['agpid']."'");
	if($colunas[0]) {
		foreach($colunas as $coluna) {
			$db->executar("DELETE FROM rehuf.conteudoitem WHERE colid = '".$coluna['colid']."'");
			$db->executar("DELETE FROM rehuf.coluna WHERE colid = '".$coluna['colid']."'");
		}
	}
	$db->executar("DELETE FROM rehuf.agrupamento WHERE agpid = '".$dados['agpid']."'");
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	if(window.opener.document.getElementById('idsubnivellinha') != null) {
		window.opener.document.getElementById('sublinha').innerHTML = '';
		window.opener.document.getElementById('idsubnivellinha').style.display = 'none';
	}
	if(window.opener.document.getElementById('idsubnivelcoluna') != null) {
		window.opener.document.getElementById('subcoluna').innerHTML = '';
		window.opener.document.getElementById('idsubnivelcoluna').style.display = 'none';
	}
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento','agrupamentocoluna');
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&islinha=true','agrupamentolinha');
	window.close();
	</script>
	<?
	
	
}

function removerlinha($dados) {
	global $db;
	$linha = $db->pegaLinha("SELECT * FROM rehuf.linha WHERE linid='".$dados['linid']."'");
	if($linha) {
		$db->executar("DELETE FROM rehuf.conteudoitem WHERE linid = '".$linha['linid']."'");
		$db->executar("DELETE FROM rehuf.linha WHERE linid = '".$linha['linid']."'");
		$db->commit();
	}
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('requisicao=buscardadostabela&tipodado=<? echo (($dados['agpid'])?'&tipodado=sublinha&agpid='.$dados['agpid']:'&tipodado=linha'); ?>','<? echo (($dados['agpid'])?'sublinha':'linha'); ?>');
	window.close();
	</script>
	<?
}

function removercoluna($dados) {
	global $db;
	$coluna = $db->pegaLinha("SELECT * FROM rehuf.coluna WHERE colid='".$dados['colid']."'");
	if($coluna) {
		$db->executar("DELETE FROM rehuf.conteudoitem WHERE colid = '".$coluna['colid']."'");
		$db->executar("DELETE FROM rehuf.coluna WHERE colid = '".$coluna['colid']."'");
		$db->commit();
	}
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	window.opener.ajaxatualizar('consultaajax=buscardadostabela&tipodado=<? echo (($dados['agpid'])?'&tipodado=subcoluna&agpid='.$dados['agpid']:'&tipodado=coluna'); ?>','<? echo (($dados['agpid'])?'subcoluna':'coluna'); ?>');
	window.close();
	</script>
	<?
}

function atualizarestruturaunidade($dados) {
	global $db;
	$sql = "UPDATE rehuf.estruturaunidade
   			SET esuareacontruida=". (($dados['esuareacontruida'])?"'".str_replace(array(".",","),array("","."),$dados['esuareacontruida'])."'":'NULL') .", 
   				esucontratualizado=". (($dados['esucontratualizado']=="sim")?'true':'false') .", esucertiticado='". (($dados['esucertiticado']=="sim")?'true':'false') ."', esugestao=". (($dados['esugestao'])?"'".$dados['esugestao']."'":"NULL") .", esutipo=".(($dados['esutipo'])?"'".$dados['esutipo']."'":"NULL").",
   				esudataadereebserh=".(($dados['esudataadereebserh'])?"'".formata_data_sql($dados['esudataadereebserh'])."'":"NULL")."
 			WHERE esuid='". $_SESSION['rehuf_var']['esuid'] ."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Gravação feita com sucesso.');
			window.location = '?modulo=principal/dadosespecificos&acao=A';
		  </script>";
	exit;
}

function salvarRegistroEntidade($dados) {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->salvar();
    
    echo '<script type="text/javascript">
    		alert("Dados gravados com sucesso");
		    window.location = \'?modulo=principal/dadoshospital&acao=A\';
	      </script>';
    exit;
}

function salvarRegistroDirigente($dados) {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($dados);
	$entidade->adicionarFuncoesEntidade($dados['funcoes']);
	$entidade->salvar();
    echo '<script type="text/javascript">
	        window.opener.location = \'?modulo=principal/dadosdirigentes&acao=A\';
	        window.close();
	      </script>';
    exit;
}

function salvarFormacaoDirigentes($dados) {
	global $db;
	
	if($dados['edtid']) {
		$sql = "UPDATE rehuf.entidadedetalhe
				SET edtcurso='".$dados['edtcurso']."', 
   					edtlocalcurso='".$dados['edtlocalcurso']."', 
   					edtdtconclusaocurso='".formata_data_sql($dados['edtdtconclusaocurso'])."',
   					edtins=NOW(), 
       				edtnrhorascurso='".$dados['edtnrhorascurso']."'  
 				WHERE edtid='".$dados['edtid']."'";
		
		$db->executar($sql);
	} elseif($_REQUEST['rmedtid']) {
		
		$db->executar("DELETE FROM rehuf.entidadedetalhe WHERE edtid='".$_REQUEST['rmedtid']."'");
		
	} else {
		if($dados['entid']) {
			$sql = "INSERT INTO rehuf.entidadedetalhe(
           			entid, 
           			edtcurso, 
           			edtlocalcurso, 
           			edtdtconclusaocurso, 
           			edtnrhorascurso)
    				VALUES ('". $dados['entid'] ."', 
    						'". $dados['edtcurso'] ."', 
    						'". $dados['edtlocalcurso'] ."', 
    						'". formata_data_sql($dados['edtdtconclusaocurso']) ."', 
    						'". $dados['edtnrhorascurso'] ."');";
			$db->executar($sql);
		} else {
			echo "<script>alert('Os dados sobre o curso não estão completos.');window.close();</script>";
			exit;
		}
	}
	$db->commit();
    echo '<script type="text/javascript">
	        window.location = \'?modulo=principal/editardirigente&acao=A&funid='.$dados['funid'].'\';
	      </script>';
    exit;
}
/**
 * Função que monta o cabeçalho com dados do hospital
 * 
 * @author Alexandre Dourado
 * @return array $menu Contendo a lista de opções do menu
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function carregardadosmenurehuf() {
	global $permissoes;
	global $db;
	
	if($permissoes['hospfederal']) {
		// monta menu padrão contendo informações sobre as entidades
		$menu = array(0 => array("id" => 1, "descricao" => "Lista de hospitais",   "link" => "/rehuf/rehuf.php?modulo=inicio&acao=C"),
					  1 => array("id" => 2, "descricao" => "Pregões",    		   "link" => "/rehuf/rehuf.php?modulo=pregao/listaPregaoPreenchimento&acao=A"),
				  	  );
	} else {
		// monta menu padrão contendo informações sobre as entidades
		$perfis = pegaPerfilGeral();

		if( $db->testa_superuser() ){
			$menu = array(0 => array("id" => 1, "descricao" => "Lista de hospitais",   					"link" => "/rehuf/rehuf.php?modulo=inicio&acao=C"),
						  1 => array("id" => 2, "descricao" => "Tabelas", 			   					"link" => "/rehuf/rehuf.php?modulo=principal/listartabela&acao=A&entid=".$_SESSION['rehuf_var']['entid']),
						  2 => array("id" => 3, "descricao" => "Dados do hospital",    					"link" => "/rehuf/rehuf.php?modulo=principal/dadoshospital&acao=A"),
						  3 => array("id" => 4, "descricao" => "Dados dos dirigentes", 					"link" => "/rehuf/rehuf.php?modulo=principal/dadosdirigentes&acao=A"),
						  4 => array("id" => 5, "descricao" => "Dados específicos",    					"link" => "/rehuf/rehuf.php?modulo=principal/dadosespecificos&acao=A"),
						  5 => array("id" => 6, "descricao" => "Indicadores",    	   					"link" => "/rehuf/rehuf.php?modulo=indicadores/gerenciarindicadores&acao=A"),
						  6 => array("id" => 7, "descricao" => "Planilha Contábil",    					"link" => "/rehuf/rehuf.php?modulo=principal/planilhacontabil&acao=A"),
						  7 => array("id" => 8, "descricao" => "Execução",    							"link" => "/rehuf/rehuf.php?modulo=principal/execucao&acao=A"),
						  8 => array("id" => 9, "descricao" => "Pregões",    		   					"link" => "/rehuf/rehuf.php?modulo=pregao/listaPregaoPreenchimento&acao=A"),
						  9 => array("id" => 10, "descricao" => "Plantão",    		   					"link" => "/rehuf/rehuf.php?modulo=plantao/plantao&acao=A".(($_GET['setid'] && $_GET['mes'] && $_GET['ano'])?"&setid=".$_GET['setid']."&ano=".$_GET['ano']."&mes=".$_GET['mes']:"")),
						  10 => array("id" => 11, "descricao" => "Solicitação Decreto",  				"link" => "/rehuf/rehuf.php?modulo=principal/solicitacaodecreto&acao=A&entid=".$_SESSION['rehuf_var']['entid']),
						  11 => array("id" => 12, "descricao" => "Diagnóstico Situacional", 			"link" => "/rehuf/rehuf.php?modulo=principal/questionarioRehuf&acao=A"),
						  12 => array("id" => 13, "descricao" => "Diagnóstico Situacional - Avaliação", "link" => "/rehuf/rehuf.php?modulo=principal/questionarioRehuf&acao=A&avaliacao=true"),
						  13 => array("id" => 14, "descricao" => "Prestação de Contas", 				"link" => "/rehuf/rehuf.php?modulo=principal/prestacaocontas&acao=A"),
						  14 => array("id" => 15, "descricao" => "Pregões - Itens Homologados", 		"link" => "/rehuf/rehuf.php?modulo=pregao/listaPregaoHomologado&acao=A")
					  	  );
		} else {
			$menu = array(0 => array("id" => 1, "descricao" => "Lista de hospitais",   		"link" => "/rehuf/rehuf.php?modulo=inicio&acao=C"),
						  1 => array("id" => 2, "descricao" => "Tabelas", 			   		"link" => "/rehuf/rehuf.php?modulo=principal/listartabela&acao=A&entid=".$_SESSION['rehuf_var']['entid']),
						  2 => array("id" => 3, "descricao" => "Dados do hospital",    		"link" => "/rehuf/rehuf.php?modulo=principal/dadoshospital&acao=A"),
						  3 => array("id" => 4, "descricao" => "Dados dos dirigentes", 		"link" => "/rehuf/rehuf.php?modulo=principal/dadosdirigentes&acao=A"),
						  4 => array("id" => 5, "descricao" => "Dados específicos",    		"link" => "/rehuf/rehuf.php?modulo=principal/dadosespecificos&acao=A"),
						  5 => array("id" => 6, "descricao" => "Indicadores",    	   		"link" => "/rehuf/rehuf.php?modulo=indicadores/gerenciarindicadores&acao=A"),
						  6 => array("id" => 7, "descricao" => "Planilha Contábil",    		"link" => "/rehuf/rehuf.php?modulo=principal/planilhacontabil&acao=A"),
						  7 => array("id" => 8, "descricao" => "Execução",    							"link" => "/rehuf/rehuf.php?modulo=principal/execucao&acao=A"),
						  8 => array("id" => 9, "descricao" => "Pregões",    		   		"link" => "/rehuf/rehuf.php?modulo=pregao/listaPregaoPreenchimento&acao=A"),
						  9 => array("id" => 10, "descricao" => "Plantão",    		   		"link" => "/rehuf/rehuf.php?modulo=plantao/plantao&acao=A".(($_GET['setid'] && $_GET['mes'] && $_GET['ano'])?"&setid=".$_GET['setid']."&ano=".$_GET['ano']."&mes=".$_GET['mes']:"")),
						  10 => array("id" => 11, "descricao" => "Solicitação Decreto",  		"link" => "/rehuf/rehuf.php?modulo=principal/solicitacaodecreto&acao=A&entid=".$_SESSION['rehuf_var']['entid']),
						  11 => array("id" => 12, "descricao" => "Diagnóstico Situacional", "link" => "/rehuf/rehuf.php?modulo=principal/questionarioRehuf&acao=A"),
						  12 => array("id" => 13, "descricao" => "Prestação de Contas", 	"link" => "/rehuf/rehuf.php?modulo=principal/prestacaocontas&acao=A"),
						  13 => array("id" => 14, "descricao" => "Pregões - Itens Homologados", 		"link" => "/rehuf/rehuf.php?modulo=pregao/listaPregaoHomologado&acao=A")
					  	  );
		}
	}
	return $menu;
}
/**
 * Função que monta o cabeçalho com dados do hospital
 * 
 * @author Alexandre Dourado
 * @return htmlcode Código HTML
 * @param string $entid ID da entidade(hospital)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function monta_cabecalho_rehuf($entid, $preid=false) {
	global $db;
	
	if(!$entid) die("<script>alert('Hospital não foi selecionado.');window.location='rehuf.php?modulo=inicio&acao=C';</script>");
	
	$sql = "SELECT ent.entnome, ena.entsig, ende.estuf, mundescricao FROM entidade.entidade ent 
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
			LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
			LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
			LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
			WHERE ent.entid = '". $entid ."' ORDER BY ent.entnome";
	
	$dadosentidade = $db->pegaLinha($sql);
	
	echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	echo "<tr>";
	echo "<td class='SubTituloDireita'>Hospital :</td><td>".$dadosentidade['entnome']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita'>IFES :</td><td>".$dadosentidade['entsig']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita'>UF / Munícipio :</td><td>".$dadosentidade['estuf']." / ".$dadosentidade['mundescricao']."</td>";
	echo "</tr>";
	if($preid) {
		$pregao = $db->pegaLinha("SELECT preobjeto, to_char(predatainicialpreenchimento, 'DD/MM/YYYY') as predatainicialpreenchimento, to_char(predatafinalpreenchimento, 'DD/MM/YYYY') as predatafinalpreenchimento FROM rehuf.pregao WHERE preid='".$preid."'");
		if($pregao) {
			echo "<tr>";
			echo "<td class='SubTituloDireita'>Código - Objeto do pregão : </td><td>".$pregao['preobjeto']."</td>";
			echo "</tr>";
			echo "<tr>";
			echo "<td class='SubTituloDireita'>Período de Preenchimento : </td><td>".$pregao['predatainicialpreenchimento']." até ".$pregao['predatafinalpreenchimento']."</td>";
			echo "</tr>";
		}
	
	}
	echo "</table>";
}
/**
 * Função que busca a lista de grupos de uma determinada tabela
 * 
 * @author Alexandre Dourado
 * @return htmlcode Código HTML (via ajax)
 * @param string $dados[tabtid]         ID da tabela
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function buscargrupoitem($dados) {
	global $db;
	// se tiver problemas nas variaveis de sessão, imprimir o erro (ajax)
	if(validaVariaveisSistema()) {
		echo "<p align='center'>Problemas nas variáveis de sessão. <b><a href='?modulo=inicio&acao=C'>Clique aqui</a></b> e refaça os procedimentos.</p>";
		exit;	
	}
	$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editartabela&acao=A&gitid='|| git.gitid ||'&tabtid='|| git.tabtid ||'\'\"></center>' as opcoes, 
				   '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editartabela&acao=A&gitid='|| git.gitid ||'&tabtid='|| git.tabtid ||'\'\">'|| git.gitdsc ||'</a>' as grupos,
				   CASE WHEN (SELECT ltadata FROM rehuf.logtabelasgrupos lta WHERE lta.gitid = git.gitid AND lta.esuid='".$_SESSION['rehuf_var']['esuid']."' ORDER BY ltadata DESC LIMIT 1) IS NULL THEN 'Não existem registros' ELSE CAST((SELECT to_char(lta.ltadata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome FROM rehuf.logtabelasgrupos lta LEFT JOIN seguranca.usuario usu ON usu.usucpf = lta.usucpf WHERE lta.gitid = git.gitid AND lta.esuid='".$_SESSION['rehuf_var']['esuid']."' ORDER BY ltadata DESC LIMIT 1) as varchar) END AS logtg
				   FROM rehuf.grupoitem git WHERE tabtid ='".$dados['tabtid']."' AND git.gitvisivel=true ORDER BY git.gitordem";
	$cabecalho = array("", "Grupos", "Última Atualização");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	exit;
}

function carregarhospitaisporunidade($dados) {
	global $db;
	$permissoes = verificaPerfilRehuf();
	$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/listartabela&acao=A&entid='|| ent.entid ||'\'\"></center>' as acoes, '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/listartabela&acao=A&entid='|| ent.entid ||'\'\">'||ent.entnome||'</a>', ena.entsig, esddsc, ende.estuf, mundescricao FROM entidade.entidade ent 
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
			LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
			LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
			LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
			LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
			LEFT JOIN workflow.documento doc ON esu.docid = doc.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE fen.funid = '". HOSPITALUNIV ."' AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') AND ena.entid = '".$dados['unidadeid']."' ORDER BY ent.entnome";
	
	$cabecalho = array("Ações", "Hospitais", "IFES", "Situação", "UF", "Município");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	exit;
	
}
/**
 * Função que testa se as variaveis OBRIGATORIAS estão setadas
 * 
 * @author Alexandre Dourado
 * @return boolean Se as variáveis estão corretas(FALSE) ou erradas(TRUE)
 * @version v1.0 17/12/2008
 */
function validaVariaveisSistema() {
	$erro = false;
	if(!$_SESSION['rehuf_var']['esuid']) {
		$erro = true;
	}
	if(!$_SESSION['rehuf_var']['entid']) {
		$erro = true;
	}
	return $erro;
}
/**
 * Função utilizada para carregar os subitens de um indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Código HTML (via ajax)
 * @param integer $dados[indid] ID do indicador 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function carregarsubitensindicadores($dados) {
	global $db;
	$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.open(\'?modulo=indicadores/gerenciarsubitem&acao=A&indid='||indid||'&sinid='||sinid||'\',\'Gerenciar\',\'scrollbars=no,height=600,width=500,status=no,toolbar=no,menubar=no,location=no\');\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=indicadores/gerenciarestruturaindicadores&acao=A&requisicao=removersubitemindicador&indid='||indid||'&sinid='||sinid||'\',\'Deseja realmente excluir o subitem?\');\"></center>' as acoes, sindsc,sinformula, 
	'<center><img id=\"setasubirsubitemindicador\" src=\"../imagens/seta_cima.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(\''||sinid||'\',\'subirsubitemindicador\');\"><img id=\"setadescersubitemindicador\" style=\"cursor:pointer;\" src=\"../imagens/seta_baixo.gif\" onclick=\"ordenar(\''||sinid||'\',\'descersubitemindicador\');\"></center>' AS ordenacao FROM rehuf.indicadorsubitem WHERE indid='".$dados['indid']."' ORDER BY sinordem";
	$cabecalho  = array("Ações", "Subitem do indicador", "Fórmula", "");  
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	exit;
}
/**
 * Função utilizada para inserir os dados do indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Código de alerta e redicecionamento
 * @param string $dados[indformula]    Label da Formula (somente descritiva) 
 * @param integer $dados[indanoini]    Ano inicial de analise do indicador
 * @param integer $dados[indanofim]    Ano final de analise do indicador
 * @param integer $dados[dimid]    	   ID da dimensão 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function inserirdadosindicadores($dados) {
	global $db;
	$sql = "INSERT INTO rehuf.indicador(dimid, inddsc, indformula, indanoini, indanofim)
    		VALUES ('".$dados['dimid']."', '".$dados['inddsc']."', '".$dados['indformula']."', '".$dados['indanoini']."', '".$dados['indanofim']."') RETURNING indid;";
	$indid = $db->pegaUm($sql);
	$db->commit();
    echo '<script type="text/javascript">
    		alert("Inserção efetuado com sucesso.");
	        window.location = \'?modulo=indicadores/gerenciarestruturaindicadores&acao=A&pagina=editar&indid='.$indid.'\';
	      </script>';
    exit;
}
/**
 * Função utilizada para remover um indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Código de alerta e redicecionamento
 * @param integer $dados[indid]    	   ID do indicador 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function removerdadosindicadores($dados) {
	global $db;
	$sql = "DELETE FROM rehuf.indicadorsubitem WHERE indid='".$dados['indid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM rehuf.indicador WHERE indid='".$dados['indid']."'";
	$db->executar($sql);
	$db->commit();
    echo '<script type="text/javascript">
    		alert("Remoção efetuada com sucesso.");
	        window.location = \'?modulo=indicadores/gerenciarestruturaindicadores&acao=A\';
	      </script>';
    exit;
}
/**
 * Função utilizada para atualizar os dados do indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Código de alerta e redicecionamento
 * @param string $dados[indid]         Nome do subitem
 * @param string $dados[indiformula]   Label da Formula (somente descritiva) 
 * @param integer $dados[indanoini]    Ano inicial de analise do indicador
 * @param integer $dados[indanofim]    Ano final de analise do indicador
 * @param integer $dados[dimid]    	   ID da dimensão 
 * @param integer $dados[indid]    	   ID do indicador 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function atualizardadosindicadores($dados) {
	global $db;
	$sql = "UPDATE rehuf.indicador SET indins=NOW(), inddsc='".$dados['inddsc']."', indformula='".$dados['indformula']."', indanoini='".$dados['indanoini']."', indanofim='".$dados['indanofim']."', dimid='".$dados['dimid']."' WHERE indid='".$dados['indid']."'";
	$db->executar($sql);
	$db->commit();
    echo '<script type="text/javascript">
    		alert("Atualização efetuada com sucesso.");
	        window.location = \'?modulo=indicadores/gerenciarestruturaindicadores&acao=A&pagina=editar&indid='.$dados['indid'].'\';
	      </script>';
    exit;
}
/**
 * Função utilizada para inserir o subitem do indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Chamada javascript para atualizar a página pai
 * @param string $dados[sindsc]         Nome do subitem
 * @param string $dados[sinformula]     Formula executada 
 * @param integer $dados[indid] 		ID do indicador
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function inserirsubitemindicador($dados) {
	global $db;
	if($dados['faixa']) {
		foreach($dados['faixa'] as $faixa) {
			$dados['sincores'] .= $faixa['min'].",".$faixa['max'].",".$faixa['cor'].";";
		}
	}
	$sql = "SELECT sinordem AS ordem FROM rehuf.indicadorsubitem WHERE indid='".$dados['indid']."' ORDER BY ordem DESC LIMIT 1";
	$ordematual = $db->pegaUm($sql);
	$sql = "INSERT INTO rehuf.indicadorsubitem(sindsc, sinformula, indid, sinordem, sincores)
    		VALUES ('".$dados['sindsc']."', '".$dados['sinformula']."', '".$dados['indid']."', '".(($ordematual)?($ordematual+1):"1")."', ".(($dados['sincores'])?"'".$dados['sincores']."'":"NULL").");";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	alert("Inserção efetuada com sucesso");
	window.opener.ajaxatualizar('requisicao=carregarsubitensindicadores&indid=<? echo $dados['indid']; ?>','listasubitens');
	window.close();
	</script>
	<?

}
/**
 * Função utilizada para atualizar o subitem do indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Chamada javascript para atualizar a página pai
 * @param string $dados[sindsc]         Nome do subitem
 * @param string $dados[sinformula]     Formula executada 
 * @param integer $dados[sinid] 		ID do subitem
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function atualizarsubitemindicador($dados) {
	global $db;
	if($dados['faixa']) {
		foreach($dados['faixa'] as $faixa) {
			$dados['sincores'] .= $faixa['min'].",".$faixa['max'].",".$faixa['cor'].";";
		}
	}
	$sql = "UPDATE rehuf.indicadorsubitem SET sinins=NOW(), sindsc='".$dados['sindsc']."', sinformula='".$dados['sinformula']."', sincores=".(($dados['sincores'])?"'".$dados['sincores']."'":"NULL")." WHERE sinid='".$dados['sinid']."'";
	$db->executar($sql);
	$db->commit();
	?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script>
	alert("Atualização efetuada com sucesso");
	window.opener.ajaxatualizar('requisicao=carregarsubitensindicadores&indid=<? echo $dados['indid']; ?>','listasubitens');
	window.close();
	</script>
	<?
	
}
/**
 * Função utilizada para remover o subitem do indicador, utilizada no administração de indicadores
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Código de alerta e redicecionamento
 * @param integer $dados[sinid] ID do subitem
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 02/04/2009
 */
function removersubitemindicador($dados) {
	global $db;
	$sql = "DELETE FROM rehuf.indicadorsubitem WHERE sinid='".$dados['sinid']."'";
	$db->executar($sql);
	$db->commit();
    echo '<script type="text/javascript">
    		alert("Remoção efetuada com sucesso.");
	        window.location = \'?modulo=indicadores/gerenciarestruturaindicadores&acao=A&pagina=editar&indid='.$dados['indid'].'\';
	      </script>';
    exit;
}

function ordenarsubitemindicador($dados) {
	global $db;
	if($dados['subitemindicadoratual']) {
		$subitemindicadoratual = $db->pegaLinha("SELECT sinid, sinordem FROM rehuf.indicadorsubitem WHERE sinid='".$dados['subitemindicadoratual']."'");
		$subitemindicadorir = $db->pegaLinha("SELECT sinid, sinordem FROM rehuf.indicadorsubitem WHERE indid='".$dados['indid']."' AND sinordem < '".$subitemindicadoratual['sinordem']."' ORDER BY sinordem DESC LIMIT 1");
		if($subitemindicadorir) {
			$db->executar("UPDATE rehuf.indicadorsubitem SET sinins=NOW(), sinordem='".$subitemindicadorir['sinordem']."' WHERE sinid='".$subitemindicadoratual['sinid']."'");
			$db->executar("UPDATE rehuf.indicadorsubitem SET sinins=NOW(), sinordem='".$subitemindicadoratual['sinordem']."' WHERE sinid='".$subitemindicadorir['sinid']."'");
			$db->commit();
		}
	} elseif($dados['desitemindicadoratual']) {
		$desitemindicadoratual = $db->pegaLinha("SELECT sinid, sinordem FROM rehuf.indicadorsubitem WHERE sinid='".$dados['desitemindicadoratual']."'");
		$desitemindicadorir = $db->pegaLinha("SELECT sinid, sinordem FROM rehuf.indicadorsubitem WHERE indid='".$dados['indid']."' AND sinordem > '".$desitemindicadoratual['sinordem']."' ORDER BY sinordem ASC LIMIT 1");
		if($desitemindicadorir) {
			$db->executar("UPDATE rehuf.indicadorsubitem SET sinins=NOW(), sinordem='".$desitemindicadorir['sinordem']."' WHERE sinid='".$desitemindicadoratual['sinid']."'");
			$db->executar("UPDATE rehuf.indicadorsubitem SET sinins=NOW(), sinordem='".$desitemindicadoratual['sinordem']."' WHERE sinid='".$desitemindicadorir['sinid']."'");
			$db->commit();
		}
	}
}
/**
 * Função utilizada carregar a lista de valor utilizados nos indicadores
 * Caso seja enviado o ID da entidade e o ano, os valor serão simulados
 * retornando os valores para tal entidade e tal ano selecionados
 * 
 * @author Alexandre Dourado
 * @return htmlcode Código HTML (usado por ajax)
 * @param integer $dados[entid] ID da entidade
 * @param integer $dados[ano] ano referente a pesquisa 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 14/04/2009
 */
function carregarindicadorvalor($dados) {
	global $db;
	if($dados['entid'] && $dados['ano']) {
		$sql = "SELECT esuid FROM rehuf.estruturaunidade WHERE entid='".$dados['entid']."'";
		$esuid = $db->pegaUm($sql);
		$sql = "SELECT '{valor'||invid||'}' as valor, invdsc, invselect FROM rehuf.indicadorvalor ORDER BY invid";
		$dadossimulado = $db->carregar($sql);
		if($dadossimulado) {
			foreach($dadossimulado as $key => $ar) {
				$dadossimulado[$key]['total'] = number_format($db->pegaUm(str_replace(array("{esuid}","{ano}"), array($esuid, $dados['ano']), $ar['invselect'])), 2, ',', '.');
				unset($dadossimulado[$key]['invselect']);
			}
		} else {
			$dadossimulado = array();
		}
		$cabecalho  = array("Código", "Descrição","Valor");
		$db->monta_lista_simples($dadossimulado,$cabecalho,500,5,'N','100%',$par2);
	} else {
		$sql = "SELECT '{valor'||invid||'}' as valor, invdsc FROM rehuf.indicadorvalor ORDER BY invid";
		$cabecalho  = array("Código", "Descrição");  
		$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
	}
	exit;
}

function verificarremocaoperiodo($dados) {
	global $db;
	$sql = "SELECT COUNT(*) AS num FROM rehuf.conteudoitem WHERE perid='".$dados['perid']."'";
	$num = $db->pegaUm($sql);
	if($num > 0) {
		echo "naoremover";
	} else {
		echo "simremover";
	}
	exit;
	
}

function removerperiodogrupoitem($dados) {
	global $db;
	$sql = "DELETE FROM rehuf.periodogrupoitem WHERE perid='".$dados['perid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
}

function confirmarmudancasitucao($dados) {
	global $db;
	$sql = "SELECT ent.entnome, ena.entsig, doc.docid, esd.esdid, esd.esddsc, esd.esdordem, esd.tpdid, ende.estuf, mundescricao, to_char(esu.esudata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome as atualizacao FROM entidade.entidade ent  
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
			LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid  
			LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
			LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
			LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = esu.usucpf
			LEFT JOIN workflow.documento doc ON esu.docid = doc.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid  
			WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND ent.entid IN('".implode("','",$dados['entid'])."') ".$filtropesquisa." 
			ORDER BY ena.entsig";
	$situacao = $db->carregar($sql);
	if($situacao[0]) {
		foreach($situacao as $sit) {
			$dadosestado[$sit['esdid']] = array("ordem"=>$sit['esdordem'],"tpdid"=>$sit['tpdid']);
			$lista[$sit['esdid']][] = $sit;
		}
		echo "<form action='?modulo=inicio&acao=C' method='post'>
			  <input type='hidden' name='requisicao' value='salvarmudancasituacao'>";
		echo "<div style='width:675;height:370;overflow:auto;'>";
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
		foreach($lista as $esdid => $ar) {
			echo "<tr>
					<td class=\"SubTituloDireita\">Selecione a situação :</td>
					<td>";
			
			$sql = "SELECT esdiddestino as codigo, esddsc as descricao FROM workflow.acaoestadodoc aed
					LEFT JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino 
					WHERE esdidorigem='".$esdid."'";
			$db->monta_combo('esdid['.$esdid.']', $sql, 'S', 'Selecione', '', '', '', '200', 'S', '');
			echo "</td></tr>";
			echo "<tr><td colspan='2'><table cellSpacing=\"1\" cellPadding=\"3\">";
			foreach($ar as $hosp) {
				echo "<tr>
						<td><img src='../imagens/seta_filho.gif' border='0' align='absmiddle'> ".$hosp['entnome']."<input type='hidden' name='documento[".$esdid."][]' value='".$hosp['docid']."'></td>
						<td>".$hosp['entsig']."</td>
						<td>".$hosp['esddsc']."</td>
					  </tr>";
			}
			echo "</table></td></tr>";
		}
		echo "</table>";
		echo "</div>";
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
		echo "<tr bgcolor='#C0C0C0'><td colspan='2'><input type='submit' value='Salvar'> <input type='button' value='Cancelar' onclick='closeMessage();'></td></tr>";		
		echo "</table>";
		echo "</form>";
	}
	exit;
}

function salvarmudancasituacao($dados) {
	global $db;
	if($dados['esdid']) {
		foreach($dados['esdid'] as $estadoorigem => $estadodestino) {
			if($estadodestino) {
				$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$estadoorigem."' AND esdiddestino='".$estadodestino."'";
				$aedid = $db->pegaUm($sql);
				if($dados['documento'][$estadoorigem] && $aedid) {
					foreach($dados['documento'][$estadoorigem] as $docid) {
						$sql = "UPDATE workflow.documento SET esdid='".$estadodestino."' WHERE docid='".$docid."'";
						$db->executar($sql);
						$sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, htddata)
    							VALUES ('".$aedid."', '".$docid."', '".$_SESSION['usucpf']."', NOW());";
    					$db->executar($sql);
					}
				}
			}
		}
		$db->commit();
	}
	echo "<script>
			alert('As situações foram atualizadas com sucesso');
			window.location='?modulo=inicio&acao=C';
		  </script>";
	exit;
}

function carregarestruturavalores($dados) {
	global $db;
	/*
	 * Verifica se existe pelo menos uma tabela selecionada
	 */
	if($dados['tabtid']) {
		foreach($dados['tabtid'] as $tabtid) {
			// Carregando os dados da tabela
			$tabela = $db->pegaLinha("SELECT * FROM rehuf.tabela WHERE tabtid='".$tabtid."' ORDER BY tabtid");
			// Carregando os grupos dentro da tabela
			$grupos = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid='".$tabtid."' ORDER BY gitordem");
			// Se tiver grupos na tabela carregar o select (ja selecionar as opções escolhidas anteriormente)
			if($grupos[0]) {
				foreach($grupos as $gp) {
					$marcado = false;
					if($dados['gitid']) {
						foreach($dados['gitid'] as $gitid) {
							if($gp['gitid'] == $gitid) {
								$marcado = true;
							}
						}
					}
					$listagrupos .= "<option value='".$gp['gitid']."' ".(($marcado)?"selected":"").">Tabela: ".$tabela['tabtdsc']." | ".$gp['gitdsc']."</option>";
				}
			}
		}
		
		if($dados['gitid']) {
			$mostrarlistacolunas = false;
			$mostrarlistalinhas = false;
			$mostrarlistaperiodos = false;
			foreach($dados['gitid'] as $gitid) {
				$grupoitem = $db->pegaLinha("SELECT * FROM rehuf.grupoitem WHERE gitid='".$gitid."'");
				
				$periodos = $db->carregar("SELECT * FROM rehuf.periodogrupoitem g 
										   WHERE g.gitid='".$gitid."'");
				
				if($periodos[0]) {
					$mostrarlistaperiodos = true;
					foreach($periodos as $per) {
						if($dados['perid']) {
							$marcado = false;
							foreach($dados['perid'] as $perid) {
								if($per['perid'] == $perid) {
									$marcado = true;
								}
							}
						}
						$listaperiodos .= "<option value='".$per['perid']."' ".(($marcado)?"selected":"").">Período : ".$per['perano']." | ".$per['perdsc']."</option>";
					}
				}
				
				
				$linhas = $db->carregar("SELECT * FROM rehuf.linha lin 
										 LEFT JOIN rehuf.agrupamento agp ON agp.agpid = lin.agpid 
										 LEFT JOIN rehuf.opcoes opc ON opc.opcid = lin.opcid 
										 WHERE lin.gitid='".$gitid."' ORDER BY agpordem, linordem");
				if($linhas) {
					$mostrarlistalinhas = true;
					foreach($linhas as $lin) {
						$marcado = false;
						if($dados['linid']) {
							foreach($dados['linid'] as $linid) {
								if($lin['linid'] == $linid) {
									$marcado = true;
								}
							}
						}
						if(!$lin['opcid']) $listalinhas .= "<option value='".$lin['linid']."' ".(($marcado)?"selected":"").">Grupo : ".$grupoitem['gitdsc']." | ".(($lin['agpdsc'])?"Agrupador: ".$lin['agpdsc']." | ":"").$lin['lindsc']."</option>";
						if($opcid) {
							foreach($opcid as $opc) {
								if($lin['opcid'] == $opc) $marcado = true;
							}
						}
						if($lin['opcid'])$listalinhasdinbk[$lin['opcid']] = "<option value='".$lin['opcid']."' ".(($marcado)?"selected":"").">".(($lin['opcdsc'])?$lin['opcdsc']:"")."</option>";
						
					}
				}
				$colunas = $db->carregar("SELECT * FROM rehuf.coluna col 
										  LEFT JOIN rehuf.agrupamento agp ON agp.agpid = col.agpid 
										  WHERE col.gitid='".$gitid."' ORDER BY agpordem, colordem");
				if($colunas && $grupoitem['tpgidcoluna']!=TPG_CFIXAS_PA) {
					$mostrarlistacolunas = true;
					foreach($colunas as $col) {
						if($dados['colid']) {
							$marcado = false;
							foreach($dados['colid'] as $colid) {
								if($col['colid'] == $colid) {
									$marcado = true;
								}
							}
						}
					
						$listacolunas .= "<option value='".$col['colid']."' ".(($marcado)?"selected":"").">Grupo: ".$grupoitem['gitdsc']." | ".(($col['agpdsc'])?"Agrupador: ".$col['agpdsc']." | ":"").$col['coldsc']."</option>";
					}
					if($listalinhasdinbk) $listalinhasdin = implode("", $listalinhasdinbk);
				}
			}
		}
	}
	$listagrupos = "<select size='7' style='width:650;' name='grupos[]' onclick=\"carregarestrutura('');\" multiple>".$listagrupos."</select>";
	$listalinhasdin = "<select size='7' style='width:650;' name='opcid[]' onclick=\"atualizarselect();\" multiple ".((!$mostrarlistacolunas)?"disabled":"").">".$listalinhasdin."</select>";
	$listalinhas = "<select size='7' style='width:650;' name='linhas[]' onclick=\"atualizarselect();\" multiple>".$listalinhas."</select>";
	$listacolunas = "<select size='7' style='width:650;' name='colunas[]' onclick=\"atualizarselect();\" multiple ".((!$mostrarlistacolunas)?"disabled":"").">".$listacolunas."</select>";
	$listaperiodos = "<select size='7' style='width:650;' name='periodos[]' onclick=\"atualizarselect();\" multiple ".((!$mostrarlistaperiodos)?"disabled":"").">".$listaperiodos."</select>";
	
	echo $listagrupos."||";
	echo $listalinhas."||";
	echo $listacolunas."||";
	echo $listalinhasdin."||";
	echo $listaperiodos;
	exit;
}

function inserirvaloresindicadores($dados) {
	global $db;
	$sql = "INSERT INTO rehuf.indicadorvalor(invdsc, invselect, dimid) VALUES ('".$dados['invdsc']."', '".$dados['invselect']."', '".$dados['dimid']."');";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Valor gravado com sucesso.');
			window.location='?modulo=indicadores/gerenciarestruturavalores&acao=A';
		  </script>";
	exit;
}
function atualizarvaloresindicadores($dados) {
	global $db;
	$sql = "UPDATE rehuf.indicadorvalor SET invins=NOW(), invdsc='".$dados['invdsc']."', invselect='".$dados['invselect']."', dimid='".$dados['dimid']."' WHERE invid='".$dados['invid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Valor atualizado com sucesso.');
			window.location='?modulo=indicadores/gerenciarestruturavalores&acao=A&pagina=editar&invid=".$dados['invid']."';
		  </script>";
	exit;
}
function removervaloresindicadores($dados) {
	global $db;
	$db->executar("DELETE FROM rehuf.indicadorvalor WHERE invid='".$dados['invid']."'");
	$db->commit();
	echo "<script>
			alert('Valor removido com sucesso.');
			window.location='?modulo=indicadores/gerenciarestruturavalores&acao=A';
		  </script>";
	exit;
}

function filtrarValoresSelectColuna($invselect) {
	global $db;
	// buscando colid unico
	$buscacoluna = explode("colid='",$invselect);
	// se explodiu em dois pedaços é porque encontrou
	if(count($buscacoluna) > 1) {
		$colidunico = substr($buscacoluna[1],0,strpos($buscacoluna[1],"' AND"));
		if(is_numeric($colidunico)) $dados['coluna'][] = $colidunico;
		if($dados['coluna']) { 
			$gruposs= $db->carregar("SELECT git.* FROM rehuf.grupoitem git LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid WHERE colid IN('".implode("','",$dados['coluna'])."')");
			foreach($gruposs as $g) {
				$dados['grupo'][$g['gitid']] = $g;
			}
		}
	} else{
		// busca varios colid
		$buscacoluna = explode("colid IN('",$invselect);
		if(count($buscacoluna) > 1) {
			if($buscacoluna[1])$buscacoluna = substr($buscacoluna[1],0,strpos($buscacoluna[1],"')"));
			if($buscacoluna)$dados['coluna'] = explode("','", $buscacoluna);
			if($dados['coluna']) {
				$gruposs = $db->carregar("SELECT git.* FROM rehuf.grupoitem git LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid WHERE colid IN('".implode("','",$dados['coluna'])."')");
				if($gruposs) {
					foreach($gruposs as $g) {
						$dados['grupo'][$g['gitid']] = $g;
					}
				}
			}
		}
	}
	// retorna $dados com os grupos e as colunas
	return $dados;
}
function filtrarValoresSelectLinha($invselect) {
	global $db;
	$buscalinha = explode("cdi.linid='",$invselect);
	if(count($buscalinha) > 1) {
		$linidunico = substr($buscalinha[1],0,strpos($buscalinha[1],"' AND"));
		if(is_numeric($linidunico)) $dados['linha'][] = $linidunico;
		if($dados['linha']) { 
			$gruposs= $db->carregar("SELECT git.* FROM rehuf.grupoitem git LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid WHERE linid IN('".implode("','",$dados['linha'])."')");
			foreach($gruposs as $g) {
				$dados['grupo'][$g['gitid']] = $g;
			}
		}
	} else {
		$buscalinha = explode("cdi.linid IN('",$invselect);
		if(count($buscalinha) > 1) {
			if($buscalinha[1])$buscalinha = substr($buscalinha[1],0,strpos($buscalinha[1],"')"));
			if($buscalinha) $dados['linha'] = explode("','", $buscalinha);
			if($dados['linha']) {
				$gruposs = $db->carregar("SELECT git.* FROM rehuf.grupoitem git LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid WHERE linid IN('".implode("','",$dados['linha'])."')");
				if($gruposs) {
					foreach($gruposs as $g) {
						$dados['grupo'][$g['gitid']] = $g;
					}
				}
			}
		}
	}
	return $dados;
}
function filtrarValoresSelectOpcao($invselect) {
	$buscaopcid = explode("cdi.opcid='",$invselect);
	if(count($buscaopcid) > 1) {
		if($buscaopcid[1])$buscaopcid = substr($buscaopcid[1],0,strpos($buscaopcid[1],"' AND"));
		if(is_numeric($buscaopcid))$dados['opcao'][] = $buscaopcid;
	} else {
		$buscaopcid = explode("cdi.opcid IN('",$invselect);
		if(count($buscaopcid) > 1) {
			if($buscaopcid[1])$buscaopcid = substr($buscaopcid[1],0,strpos($buscaopcid[1],"')"));
			if($buscaopcid) $dados['opcao'] = explode("','", $buscaopcid);
		}
	}
	return $dados;
}

function filtrarValoresSelectPeriodo($invselect) {
	global $db;
	// buscando colid unico
	$buscaperiodo = explode("cdi.perid='",$invselect);
	// se explodiu em dois pedaços é porque encontrou
	if(count($buscaperiodo) > 1) {
		$peridunico = substr($buscaperiodo[1],0,strpos($buscaperiodo[1],"' AND"));
		if(is_numeric($peridunico)) $dados['periodo'][] = $peridunico;
		if($dados['periodo']) { 
			$gruposs= $db->carregar("SELECT git.* FROM rehuf.grupoitem git LEFT JOIN rehuf.periodogrupoitem per ON per.gitid = git.gitid WHERE perid IN('".implode("','",$dados['periodo'])."')");
			foreach($gruposs as $g) {
				$dados['grupo'][$g['gitid']] = $g;
			}
		}
	} else{
		// busca varios colid
		$buscaperiodo = explode("cdi.perid IN('",$invselect);
		if(count($buscaperiodo) > 1) {
			if($buscaperiodo[1])$buscaperiodo = substr($buscaperiodo[1],0,strpos($buscaperiodo[1],"')"));
			if($buscaperiodo)$dados['periodo'] = explode("','", $buscaperiodo);
			if($dados['periodo']) {
				$gruposs = $db->carregar("SELECT git.* FROM rehuf.grupoitem git LEFT JOIN rehuf.periodogrupoitem per ON per.gitid = git.gitid WHERE perid IN('".implode("','",$dados['periodo'])."')");
				if($gruposs) {
					foreach($gruposs as $g) {
						$dados['grupo'][$g['gitid']] = $g;
					}
				}
			}
		}
	}
	// retorna $dados com os grupos e as colunas
	return $dados;
}
/**
 * Função utilizada para carregar o ID do questionário vinculado a
 * entidade passada por parametro.
 * 
 * @author Victor Benzi
 * @return integer $qrpid - ID do questionario
 * @param integer $entid - ID da entidade
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 17/04/2012
 */
function pegaQrpidRehuf( $entid, $queid ){
	global $db;	
	
	$sql = "SELECT
            	eu.qrpid
            FROM
            	rehuf.estruturaunidade eu
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = eu.qrpid
            WHERE
            	eu.entid = {$entid}
            	AND qr.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
    	
        $sql = "SELECT
                    ent.entnome as descricao
                FROM
                    entidade.entidade ent
                WHERE
                    ent.entid = {$entid}";
        $titulo = $db->pegaUm( $sql );
        $arParam = array ( "queid" => $queid, "titulo" => "REHUF (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        if( $queid == QUESTIONARIO_REHUF ){
        	$sql = "UPDATE rehuf.estruturaunidade SET qrpid = {$qrpid} WHERE entid = ".$entid;
        } else {
        	$sql = "UPDATE rehuf.estruturaunidade SET qrpidavaliador = {$qrpid} WHERE entid = ".$entid;
        }
        $db->executar( $sql );
        $db->commit();
    }
    return $qrpid;
}

function salvarPlanoInterno()
{
	global $db;
	//dbg($_POST,1);
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
	include_once APPRAIZ . "rehuf/classes/PlanoInterno.class.inc";
	include_once APPRAIZ . "rehuf/classes/PlanoTrabalho.class.inc";
	include_once APPRAIZ . "rehuf/classes/Portaria.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInternoQuestionario.class.inc";
		
	$pli = new PlanoInterno();
	$pli->popularDadosObjeto( $_POST );
	$existe = $pli->validarPI($_POST['plinumero'],$_POST['pliid']);
	if(!$existe){
	
		if($_POST['plitid'] == 2){
			if($_POST['plinumerop']){
				$_POST['plinumero'] = mb_strtoupper($_POST['plinumerop']);
				$_POST['plinumerop'] = null;
			}
			if($_POST['plidtiniciop']){
				$_POST['plidtinicio'] = $_POST['plidtiniciop'];
				$_POST['plidtinicio'] = ($_POST['plidtinicio']);
				$_POST['plidtiniciop'] = null;
			}
			if($_POST['plidtterminop']){
				$_POST['plidttermino'] = $_POST['plidtterminop'];
				$_POST['plidttermino'] = ($_POST['plidttermino']);
				$_POST['plidtterminop'] = null;
			}
			
			$_POST['plidtinicio'] = formata_data_sql($_POST['plidtinicio']);
			$_POST['plidttermino'] = formata_data_sql($_POST['plidttermino']);
			
			$_POST['plinumero'] = mb_strtoupper($_POST['plinumero']);
			
			$pli = new PlanoInterno();
			$pli->popularDadosObjeto( $_POST );
			$pliid = $pli->salvar();
			$_POST['pliid'] = $pliid;
			$pli->commit();
			
			$plt = new PlanoTrabalho();
			$plt->popularDadosObjeto( $_POST );
			$pltid = $plt->salvar();
			$_POST['pltid'] = $pltid;
			$_POST['prtid'] = null;
			$plt->commit();
		}else{
			$_POST['plidtinicio'] = formata_data_sql($_POST['plidtinicio']);
			$_POST['plidttermino'] = formata_data_sql($_POST['plidttermino']);
			
			$pli = new PlanoInterno();
			$pli->popularDadosObjeto( $_POST );
			$pliid = $pli->salvar();
			$_POST['pliid'] = $pliid;
			$pli->commit();
			
			$_POST['prtdata'] = formata_data_sql($_POST['prtdata']);
			$_POST['prtdtpublicacao'] = formata_data_sql($_POST['prtdtpublicacao']);
			
			$prt = new Portaria();
			$prt->popularDadosObjeto( $_POST );
			$prtid = $prt->salvar();
			$_POST['prtid'] = $prtid;
			$_POST['pltid'] = null;
			$prt->commit();
		}	
	
		//Salvar Arquivo
		if($_FILES['arquivo']['name']){
			include_once APPRAIZ . "includes/classes/file.class.inc";
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file = new FilesSimec();
			$file->setUpload( $_FILES['arquivo']['name'] ,null,false);
			$_POST['arqid'] = $file->getIdArquivo();
		}
		
		$vpi = new VinculoPlanoInterno();
		$vpi->popularDadosObjeto( $_POST );
		$vpiid = $vpi->salvar();
		$vpi->salvarQuestionarioPorVinculoInterno( $vpiid, $_POST['plitid'] == 2 ? $_POST['queidp'] : $_POST['queid'] );
		$vpi->commit();
		
		$_SESSION['rehuf']['altert'] = "Operação realizada com sucesso!";
	}else{
		$_SESSION['rehuf']['altert'] = "O Plano Interno informado já está vinculado!";
	}
	return $vpi->buscarplanointerno($vpiid);
}
function excluirPlanoInterno(){
	global $db;
	extract($_POST);
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
	include_once APPRAIZ . "rehuf/classes/PlanoInterno.class.inc";
	include_once APPRAIZ . "rehuf/classes/PlanoTrabalho.class.inc";
	include_once APPRAIZ . "rehuf/classes/Portaria.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContas.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContasDetalhe.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContasResposta.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInternoQuestionario.class.inc";

	
	$vpi = new VinculoPlanoInterno();
	$vpi->carregarPorId($vpiid);
	extract($vpi->getDados());
	
	$prc = new PrestacaoContas();
	$arrDados = $prc->buscarPrestacaoContasVinculoPlanoInterno($vpiid);
	if($arrDados[0]){
		foreach($arrDados as $dado){
			$prcd = new PrestacaoContasDetalhe();
			$pcdid = $prcd->buscarPrestacaoContasDetalhePorPrestacaoContas($dado['prcid']);
			if($pcdid){
				$existe_erro = $prcd->excluir($pcdid);
				$pcdid = 0;
				$prcd->commit();
			}
			
			$prcr = new PrestacaoContasResposta();
			$pcrid = $prcr->buscarPrestacaoContasRespostaPorPrestacaoContas($dado['prcid']);
			if($pcrid){
				$existe_erro = $prcr->excluir($pcrid);
				$pcrid = 0;
				$prcd->commit();
			}
			
			$existe_erro = $prc->excluir($dado['prcid']);
			$prc->commit();
		}
	}
		
	$q = new VinculoPlanoInternoQuestionario($vpiid);
	$q->excluirVinculoQuestionario($vpiid);
	
	if($arqid){
		excluirArquivo($vpiid,$arqid);
	}

	$vpi->excluir($vpiid);
	
	$pli = new PlanoInterno();
	$existe_erro = $pli->excluir($pliid);
	$pli->commit();
	
	if($plitid == 2){
		$plt = new PlanoTrabalho();
		$existe_erro = $plt->excluir($pltid);
		$plt->commit();
	}else{
		$prt = new Portaria();
		$existe_erro = $prt->excluir($prtid);
		$prt->commit();
	}	
		
	$existe_erro = $vpi->excluir($vpiid);
		
	if($existe_erro == true){
		$_SESSION['rehuf']['altert'] = "Operação realizada com sucesso!";
	}else{
		$_SESSION['rehuf']['altert'] = "Não foi possível realizar a operação!";
	}
	header('Location: rehuf.php?modulo=daf/vincularplanotrabalhoplanointerno&acao=A');
}

function carregarPlanoInterno()
{
	global $db;
	$vpiid = $_POST['vpiid'];
	
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
		
	$v = new VinculoPlanoInterno();
	return $v->buscarplanointerno($vpiid);
}

function removerArquivo()
{
	global $db;
	$vpiid = $_POST['vpiid'];
	$arqid = $_POST['arqid'];
	
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
	
	$v = new VinculoPlanoInterno();
	$v->removerArquivo($vpiid,$arqid);
	
	//Remover Arquivo
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->excluiArquivoFisico($arqid);

}

function excluirArquivo($vpiid,$arqid)
{
	global $db;
	
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
	
	$v = new VinculoPlanoInterno();
	$v->removerArquivo($vpiid,$arqid);
	
	//Remover Arquivo
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->excluiArquivoFisico($arqid);

}


function recuperarQuestionarioPorVinculo($vpiid = null)
{
	global $db;
	if(!$vpiid){
		return false;
	}else{
		$sql = "SELECT
					que.queid AS codigo,
					que.quetitulo AS descricao
				FROM
					questionario.questionario que
				inner join
					rehuf.vinculointernoquestionario vpq ON vpq.queid = que.queid
				WHERE 
					vpq.vpiid = $vpiid
				ORDER BY
					que.quetitulo";
		return $db->carregar($sql);
	}
	
}

function exibeDataPorExtenso($data)
{
	$data = explode("-",$data);
    $dia = $data[2];
    $mes = $data[1];
    $ano = $data[0];
    switch ($mes){
	    case 1: $mes = "JANEIRO"; break;
	    case 2: $mes = "FEVEREIRO"; break;
	    case 3: $mes = "MARÇO"; break;
	    case 4: $mes = "ABRIL"; break;
	    case 5: $mes = "MAIO"; break;
	    case 6: $mes = "JUNHO"; break;
	    case 7: $mes = "JULHO"; break;
	    case 8: $mes = "AGOSTO"; break;
	    case 9: $mes = "SETEMBRO"; break;
	    case 10: $mes = "OUTUBRO"; break;
	    case 11: $mes = "NOVEMBRO"; break;
	    case 12: $mes = "DEZEMBRO"; break;
     }
 
        $mes=strtolower($mes);
        $data = ("$dia de $mes de $ano");

	return $data;
}

function salvarPrestacaoContas(){
	global $db;
	
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContasEstado.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContas.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContasDetalhe.class.inc";
	include_once APPRAIZ . "rehuf/classes/PrestacaoContasResposta.class.inc";
	include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
	include_once APPRAIZ . "rehuf/classes/QuestionarioResposta.class.inc";
	include_once APPRAIZ . "rehuf/classes/QuestionarioItemPergunta.class.inc";
	include_once APPRAIZ . "rehuf/classes/RespostasQuestionario.class.inc";
	
	
	$vpi = new VinculoPlanoInterno();
	$vpi->popularDadosObjeto( $_POST );
		extract($vpi->getDados());
	
	$prc = new PrestacaoContas();
	$prc->popularDadosObjeto( $_POST );
	$prcid = $prc->salvar();
	$_POST['prcid'] = $prcid;
	$prc->commit();
	
	$prcd = new PrestacaoContasDetalhe();
	$prcd->popularDadosObjeto( $_POST );
	$prcdid = $prcd->salvar();
	$_POST['prcdid'] = $prcdid;
	$prcd->commit();
	
	if($_POST['hdn_queid']){
		$n = 0;
		foreach($_POST['hdn_queid'] as $queid){
			
			$_POST['queid'] = $queid;
			$_POST['qrpid'] = $_POST['hdn_qrpid'][$queid][$n] ? $_POST['hdn_qrpid'][$queid][$n] : false;
			$qrp = new QuestionarioResposta();
			$qrp->popularDadosObjeto( $_POST );
			$qrpid = $qrp->salvar();
			$qrp->commit();
			$_POST['qrpid'] = $qrpid;
			
			$_POST['itptitulo'] = $_POST['qrptitulo'];
	
			if($_POST['hdn_perid'][$queid]){
				foreach($_POST['hdn_perid'][$queid] as $dado){
					$_POST['resid'] = $_POST['hdn_resid'][$queid][$n] ? $_POST['hdn_resid'][$queid][$n] : false;
					$_POST['perid'] = $_POST['hdn_perid'][$queid][$n];
					$_POST['resdsc'] = $_POST['campo_'.$queid."_".$n];
							
					$rq = new RespostasQuestionario();
					$rq->popularDadosObjeto( $_POST );
					$rq->salvar();
					$rq->commit();
					
					$prcr = new PrestacaoContasResposta();
					$prcr->popularDadosObjeto( $_POST );
					$prcr->salvar();
					$prcr->commit();

					$n++;
				}
			}
			
		}
	}
	
	$_SESSION['rehuf']['altert'] = "Operação realizada com sucesso!";
	if($pltid){
		header('Location: rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&pltid='.$pltid);
	}else{
		header('Location: rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&prtid='.$prtid);
	}
		
}

/**************
 * Função que cria o "workflow.documento", caso não exista, vinculando-o a "rehuf.prestacaocontas"
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $dmdid (integer)
 * @return docid (integer)
 *
 **************/
function criarDocumentoPrestacaoContas( $prcid = null ) {
	global $db;
	$docid = pegarDocidVinculoPlanoInterno($prcid);
	if(!$docid){

		$docdsc = "Workflow Prestação de Contas";
		$tpdid = TIPO_DOCUMENTO_PRESTACAO_CONTAS;
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
	}
	return $docid;
}

/**************
 * Função que retorna o ID do "workflow.documento", vinculado a Prestacao Contas.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $dmdid (integer)
 * @return docid (integer)
 *
 **************/
function pegarDocidVinculoPlanoInterno( $prcid  = null ) {
	global $db;
	if(!$prcid){
		return false;
	}
	$sql = "SELECT
			 docid
			FROM
			 rehuf.prestacaocontas
			WHERE
			 prcid = {$prcid}";
	return (integer) $db->pegaUm( $sql );
}

function pegarEstadoPrestacaoContas( $docid  = null ) {
	global $db;
	if(!$docid){
		return false;
	}
	$sql = "SELECT
			 esdid
			FROM
			 workflow.documento
			WHERE
			 docid = {$docid}";
	return (integer) $db->pegaUm( $sql );
}

function pegarEstadoDescricaoPrestacaoContas( $esdid  = null ) {
	global $db;
	if(!$esdid){
		return false;
	}
	$sql = "SELECT
			 esddsc
			FROM
			 workflow.estadodocumento
			WHERE
			 esdid = {$esdid}";
	return $db->pegaUm( $sql );
}


function verificaRespostaPergunta($prcid,$vpiid)
{
	global $db;
		$sql = "SELECT
							vpqid
					from
						rehuf.vinculointernoquestionario
				   where 
				   		vpiid = $vpiid";
			$arrVpqid = $db->carregarColuna($sql);
			
			$sql = "SELECT 
						count(qp.perid)
						
					FROM 
						rehuf.vinculointernoquestionario vpiq
					inner join 
						questionario.pergunta qp ON vpiq.queid=qp.queid
					WHERE vpqid in(".implode(",",$arrVpqid).")";
			$pergunta = $db->pegaUm($sql);
			
		$sql = "select
						count(distinct(res.perid))
					from 
						questionario.resposta res
					inner join
						questionario.questionarioresposta qrp ON qrp.qrpid = res.qrpid
					inner join
						rehuf.prestacaocontasresposta pcr ON pcr.qrpid  = qrp.qrpid
					where
						prcid = $prcid
					and res.resdsc <> ''";
		$resposta = $db->pegaUm($sql);
		
	if($resposta < $pergunta){
		return "Favor responder todas as perguntas.";
	}else{
		return true;
	}
}

function buscaracaoestadodoc($esdidorigem,$esdiddestino){
	global $db;
	
	$sql = "SELECT
					aedid
			FROM
				workflow.acaoestadodoc
			WHERE
				esdidorigem = $esdidorigem
			AND
				esdiddestino = $esdiddestino";
	return $db->pegaUm( $sql );
	
}
function verificarEstadoBloqueado($docid){
	global $db;
	
	$sql = "select 
					count(1) 
			from 
				workflow.historicodocumento wh
			inner join 
				workflow.acaoestadodoc wa on wa.aedid = wh.aedid
			inner join 
				workflow.estadodocumento we on we.esdid = wa.esdidorigem
			where 
				wh.docid = $docid
			and
				wh.aedid in (".WF_ACAO_DESBLOQUEADO.")";
	return $db->pegaUm( $sql );
}

function verificaPermissao(){
	global $db;
	$retorno = false;
	$arrayDadosPerfil = pegaPerfilGeral();
	if($arrayDadosPerfil){
		foreach($arrayDadosPerfil as $dado){
			if($dado == PRF_EQTECMEC || $dado == PRF_SUPERUSUARIO){
				$retorno = true;
				break;
			}
		}
		if(!$retorno){
			return "Dados enviados para DAF.";
		}else{
			return true;
		}
	}
}
?>