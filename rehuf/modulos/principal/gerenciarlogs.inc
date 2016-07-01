<?php

function detalharDados($dados) {
	global $db;
	$ltaid = $dados['ltaid'];
	$log = $db->pegaUm("SELECT ltalog FROM rehuf.logtabelasgrupos WHERE ltaid='".$dados['ltaid']."'");
	$dados = json_decode($log, true);
	
	$dadosgrupo = $db->pegaLinha("SELECT * FROM rehuf.grupoitem WHERE gitid='".$dados['gitid']."'");
	
	if($dadosgrupo['tpgidlinha'] == TPG_LDINAM_OPCOES || 
	   $dadosgrupo['tpgidlinha'] == TPG_LDINAM_TEXT) {

		$linhas = $db->carregar("SELECT l.linid, l.lindsc, o.opcdsc FROM rehuf.linha l 
								 LEFT JOIN rehuf.opcoes o ON o.opcid = l.opcid 
								 WHERE l.gitid='".$dados['gitid']."' AND 
								 	   l.esuid IN(SELECT esuid FROM rehuf.logtabelasgrupos WHERE ltaid='".$ltaid."') AND l.linano='".$dados['ctiexercicio']."'".(($dados['perid'])?" AND l.perid='".$dados['perid']."'":"")." ORDER BY linordem");
	   	
	} else {
		$linhas = $db->carregar("SELECT l.linid, l.lindsc, o.opcdsc FROM rehuf.linha l 
								 LEFT JOIN rehuf.opcoes o ON o.opcid = l.opcid 
								 WHERE gitid='".$dados['gitid']."'");
	}
	
	$colunas = $db->carregar("SELECT c.colid, c.coldsc, a.agpdsc FROM rehuf.coluna c 
							  LEFT JOIN rehuf.agrupamento a ON a.agpid = c.agpid 
							  WHERE c.gitid='".$dados['gitid']."' ORDER BY agpordem, colordem");

	echo "<div style=width:100%;height:100%;overflow:auto;>";
	echo "<table class=listagem width=100%>";
	if($dados['ctiexercicio']) {
		echo "<tr><td class=SubTituloDireita>Ano</td><td>".$dados['ctiexercicio']."</td></tr>";
	}
	if($dados['perid']) {
		echo "<tr><td class=SubTituloDireita>Período</td><td>".$db->pegaUm("SELECT perdsc FROM rehuf.periodogrupoitem WHERE perid='".$dados['perid']."'")."</td></tr>";
	}
	if($dados['esuid']) {
		echo "<tr><td class=SubTituloDireita>Hospital</td><td>".$db->pegaUm("SELECT entnome FROM rehuf.estruturaunidade e 
																			 LEFT JOIN entidade.entidade en ON en.entid = e.entid 
																			 WHERE esuid='".$dados['esuid']."'")."</td></tr>";
	}
	echo "</table>";
	
	echo "<table class=listagem width=100%>";
	echo "<tr>";
	echo "<td>&nbsp;</td>";
	if($colunas[0]) {
		foreach($colunas as $col) {
			echo "<td class=SubTituloCentro><font size=1>".$col['coldsc']." ".(($col['agpdsc'])?"(".$col['agpdsc'].")":"")."</font></td>";
		}
	}
	echo "</tr>";
	
	if($linhas[0]) {
		foreach($linhas as $lin) {
			echo "<tr>";
			echo "<td class=SubTituloDireita><font size=1>".(($lin['lindsc'])?$lin['lindsc']:$lin['opcdsc'])."</font></td>";
			if($colunas[0]) {
				foreach($colunas as $col) {
					//print_r($dados['conteudoitem'][$lin['linid']][$col['colid']]);
					if(is_array($dados['conteudoitem'][$lin['linid']][$col['colid']]['ctivalor'])) {
						echo "<td align=right>";
						echo "<table width=100%>";
						foreach($dados['conteudoitem'][$lin['linid']][$col['colid']]['ctivalor'] as $perid => $valor) {
							echo "<tr>";
							echo "<td width=50% align=right><font size=1>".$db->pegaUm("SELECT perdsc FROM rehuf.periodogrupoitem WHERE perid='".$perid."'")." :</font></td>";
							echo "<td width=50%>".$valor."</td>";
							echo "</tr>";
						}
						echo "</table>";
						echo "</td>";
					} else {
						echo "<td align=right>".(($dados['conteudoitem'][$lin['linid']][$col['colid']]['ctivalor'])?$dados['conteudoitem'][$lin['linid']][$col['colid']]['ctivalor']:"-")."</td>";
					}
				}
			}
			echo "</tr>";
		}
	} else {
		echo "<tr>";
		echo "<td>Não existem linhas</td>";
		echo "</tr>";
	}
	
	echo "</table>";
	
	echo "<p align=center><input type=button value=Fechar onclick=closeMessage();></p>";
	
	echo "</div>";
	
}

function carregargrupoitemlog($dados) {
	global $db;
	
	$sql = "SELECT '<img src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=abrirGrupo(\''||gitid||'\',this);> '||gitdsc as desc, CASE WHEN gitvisivel = true THEN 'Ativo' ELSE 'Inativo' END as status, to_char(gitins, 'dd/mm/YYYY HH24:MI:ss') as dat 
			FROM rehuf.grupoitem 
			WHERE tabtid='".$dados['tabtid']."'
			ORDER BY gitordem";
	
	$cabecalho = array("<b>Grupo</b>", "<b>Status</b>", "<b>Última atualização (Grupos)</b>");
	
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
}

function carregardadoslog($dados) {
	global $db;
	
	echo "<table class=listagem width=100%>";
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Selecione o hospital:</td>";
	echo "<td width=70%>";
	
	$db->monta_combo('hospid_'.$dados['gitid'], "SELECT ent.entid as codigo, ent.entnome || coalesce(' - '||ent2.entsig,'') as descricao 
									FROM entidade.entidade ent
									LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
									LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
									INNER JOIN entidade.entidade ent2 on fue.entid = ent2.entid 
									WHERE fen.funid = " . HOSPITALUNIV . " ORDER BY ent.entnome", 'S', "Selecione...", '', '', '', '300', 'N', 'hospid_'.$dados['gitid']);
	
	echo " <input type=button value=Visualizar onclick=\"abrirDados('".$dados['gitid']."',document.getElementById('hospid_".$dados['gitid']."').value);\"></td></tr>";
	echo "<tr><td colspan=2><div id=tr_".$dados['gitid']." style=\"width:100%;height:250px;overflow:auto;\"></div></td></tr>";
	echo "</table>";
}

function carregarlog($dados) {
	global $db;
	
	$sql = "SELECT CASE WHEN ltalog IS NULL THEN '<center><img src=../imagens/consultar_01.gif></center>' ELSE '<center><img src=../imagens/consultar.gif style=cursor:pointer; onclick=detalharDados('||ltaid||');></center>' END as acao, usunome, to_char(ltadata,'dd/mm/YYYY HH24:MI:ss') as data 
			FROM rehuf.logtabelasgrupos l
			LEFT JOIN seguranca.usuario u ON l.usucpf = u.usucpf 
			WHERE gitid='".$dados['gitid']."' AND esuid IN( SELECT esuid FROM rehuf.estruturaunidade WHERE entid='".$dados['entid']."' ) 
			ORDER BY ltadata DESC";
	
	$cabecalho = array("&nbsp;","<b>Usuário</b>","<b>Data de alteração</b>");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}


switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('carregargrupoitemlog'=>true,
									 'carregardadoslog'=>true,
									 'carregarlog'=>true,
									 'detalharDados'=>true
									 );
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script>
function abrirTabela(tabtid, obj) {

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
	
		obj.title = 'menos';
		obj.src = '../imagens/menos.gif';
		var linhan = tabela.insertRow(linha.rowIndex);
		var cell0 = linhan.insertCell(0);
		cell0.colSpan = 2;
		cell0.id = 'id_'+tabtid+'_'+linhan.rowIndex;
		cell0.innerHTML = "Carregando...";
		ajaxatualizar('requisicao=carregargrupoitemlog&tabtid='+tabtid,'id_'+tabtid+'_'+linhan.rowIndex);
		
	} else {
	
		obj.title = 'mais';
		obj.src = '../imagens/mais.gif';
		tabela.deleteRow(linha.rowIndex);
	
	}
}

function abrirGrupo(gitid, obj) {
	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
	
		obj.title = 'menos';
		obj.src = '../imagens/menos.gif';
		var linhan = tabela.insertRow(linha.rowIndex);
		var cell0 = linhan.insertCell(0);
		cell0.colSpan = 3;
		cell0.id = 'id_'+gitid+'_'+linhan.rowIndex;
		cell0.innerHTML = "Carregando...";
		ajaxatualizar('requisicao=carregardadoslog&gitid='+gitid,'id_'+gitid+'_'+linhan.rowIndex);
		
	} else {
	
		obj.title = 'mais';
		obj.src = '../imagens/mais.gif';
		tabela.deleteRow(linha.rowIndex);
	
	}
}

function abrirDados(gitid, entid) {
	document.getElementById('tr_'+gitid).innerHTML = "Carregando...";
	ajaxatualizar('requisicao=carregarlog&gitid='+gitid+'&entid='+entid,'tr_'+gitid);
}

function detalharDados(ltaid) {
	displayMessage('rehuf.php?modulo=principal/gerenciarlogs&acao=A&requisicao=detalharDados&ltaid='+ltaid);
}

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow
	
function displayMessage(url) {
	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(1000,650);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

</script>

<?

$titulo_modulo = "REHUF - Reestruturação dos Hospitais Universitários Federais";
monta_titulo( $titulo_modulo, 'Gerenciamento de Logs' );


$sql = "SELECT '<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=abrirTabela(\''||tabtid||'\',this);> '||tabtdsc as desc, to_char(tabtins, 'dd/mm/YYYY HH24:MI:ss') as dat FROM rehuf.tabela ORDER BY dimid, tabordem";
$cabecalho = array("<b>Tabelas</b>", "<b>Última atualização (Tabelas)</b>");
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);

?>