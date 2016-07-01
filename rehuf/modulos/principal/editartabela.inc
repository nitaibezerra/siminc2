<?
/*
 * TELA DE EDIÇÃO DE TABELAS (Monta vários tipo de tabelas definidos pelo administrador)
 * Parâmetro obrigatório : $_REQUEST['tabtid'], parâmetro opcional : $_REQUEST['gitid'],$_REQUEST['ano']
 *  
 */

/* TRATAMENTO DE SEGURANÇA */
// Validando se as variaveis obrigatorias estão com valores setados
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

/* Tratamento do documento */
$docid = pegarDocid($_SESSION['rehuf_var']['entid']);
if($docid) {
	$estid = pegarEstadoDocumento($docid);
} else {
	// Caso não exista documento, retornar para o inicio
	echo "<script>
			alert('Problemas com o cadastramento do documento. Entre em contato com o administrador.');
			window.location = '?modulo=inicio&acao=C';
		  </script>";
}
/* FIM - Tratamento do documento */


$permissoes = verificaPerfilRehuf($estid, $_REQUEST['tabtid']);
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
// Validando os acessos das funções autorizadas
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirconteudoitem'=>true,
									 'inserirlinhadintx'=>true,
									 'removerlinhadintx'=>true,
									 'removerlinhadinop'=>true);
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);	
	}
}
/* FIM - TRATAMENTO DE SEGURANÇA */





include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script language="JavaScript">
window.onbeforeunload = verificaAlteracao;

//desabilita menu de opções após clicar no botão direito
function desabilitaMenu(e) {
	if (window.Event) {
		if (e.which == 2 || e.which == 3)
			return false;
	} else {
		event.cancelBubble = true
		event.returnValue = false;
		return false;
	}
}
//desabilita botão direito
function desabilitaBotaoDireito(e) {
	if (window.Event){
		if (e.which == 2 || e.which == 3)
			return false;
	} else if (event.button == 2 || event.button == 3) {
		event.cancelBubble = true
		event.returnValue = false;
		return false;
	}
}
//desabilita botão direito do mouse
if (window.captureEvents){
	if ( window.Event )
		document.captureEvents(Event.MOUSEUP);
	if ( document.layers )
		document.captureEvents(Event.MOUSEDOWN);

	document.oncontextmenu = desabilitaMenu;
	document.onmousedown = desabilitaBotaoDireito;
	document.onmouseup = desabilitaBotaoDireito; 
}
</script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center; 
}
.stylelinhas {
	background-color: #dcdcdc;
	text-align: right; 
}
.styleconteudo {
	text-align: left;
	vertical-align: top;
}
.TabelaEditRehuf
{
    FONT-SIZE: xx-small;
    BORDER-RIGHT: #cccccc 1px solid;
    BORDER-TOP: #cccccc 1px solid;
    BORDER-LEFT: #cccccc 1px solid;
    BORDER-BOTTOM: #cccccc 1px solid;
	TEXT-DECORATION: none;
	WIDTH: 100%;
	TEXT-COLOR: #000000;
}
</style>
<?
// montando o menu
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/listartabela&acao=A&entid=".$_SESSION['rehuf_var']['entid']);
?>
<form name='formulario' method='post' id='formulario'>
<input type='hidden' id='alteracaodados' value='0'>
<input type='hidden' name='requisicao' value='inserirconteudoitem'>
<?
// Pegando dados da TABELA (pai de toda a estrutura)
if($_REQUEST['tabtid']) {
	$tabela = $db->carregar("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid = '". $_REQUEST['tabtid'] ."'");
}
// Verifica se existe a TABELA
if($tabela) {
	// Caso exista, pegar o primeiro registro
	$tabela = current($tabela);
	// Se existir o filtro de 'grupo', buscar somente tal grupo
	if($_REQUEST['gitid']) {
		$filgrupo = "AND gitid = '". $_REQUEST['gitid'] ."'";
	}
	// Pegar um registro (ou registro filtrado, ou primeiro registro da ordem)
	$grupoitem = $db->pegaLinha("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' AND gitvisivel=true ".$filgrupo." ORDER BY gitordem LIMIT 1");
	if($grupoitem) {
		// Analisando o tipo de coluna (definido em constantes.php)
		switch($grupoitem['tpgidcoluna']) {
			case TPG_CFIXAS_PA: // Colunas fixas referentes por ano
				/*
				 * Verificando se o usuário possui filtro de ano para visualização
				 * Se tiver, utilizar período predefinido, senão utilizar período global da tabela
				 */
				if($permissoes['filtrodadosano']) {
					$filtroanoperfilpa = " AND col.coldsc >= '".$permissoes['filtrodadosano']['anoini']."' AND col.coldsc <= '".$permissoes['filtrodadosano']['anofim']."'";
				}
				$colunapa = (array) $db->carregar("SELECT * FROM rehuf.coluna col
												   LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
												   WHERE gitid = '". $grupoitem['gitid'] ."' ".$filtroanoperfilpa." ORDER BY coldsc");
				break;
			case TPG_CFIXAS_SN: // Colunas fixas sem agrupadores
				// Analisando o tipo de linha (definido em constantes.php)
				$anoreferente = (($_REQUEST['ano'])?$_REQUEST['ano']:$tabela['tabanofim']);
				
				$coluna = (array) $db->carregar("SELECT * FROM rehuf.coluna col
												 LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
												 WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY colordem");
				break;
			case TPG_CFIXAS_CN: // Colunas fixas agrupadores agrupadores
				// Analisando o tipo de linha (definido em constantes.php)
				$anoreferente = (($_REQUEST['ano'])?$_REQUEST['ano']:$tabela['tabanofim']);
				
				// Carregando os agrupadores de coluna
				$agrupadorescoluna = (array) $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid = '". $grupoitem['gitid'] ."' AND agplinha = false ORDER BY agpordem");
				// Verifica se o agrupador esta vazio (a função carregar retorna o indice 0 vazio: array(0 =>))
				if($agrupadorescoluna[0]) {
					foreach($agrupadorescoluna as $agpcoluna) {
						// Carregando as colunas de cada agrupador
						$coluna[$agpcoluna['agpid']] = (array) $db->carregar("SELECT * FROM rehuf.coluna col
																			  LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
																			  WHERE gitid = '". $grupoitem['gitid'] ."' AND agpid = '". $agpcoluna['agpid'] ."' ORDER BY colordem");
						// Caso não tenha coluna em um agrupador, criando coluna indicando 'Sem coluna' 
						if(!$coluna[$agpcoluna['agpid']][0]) {
							$coluna[$agpcoluna['agpid']][0] = array("coldsc" => "Sem coluna");
						}
					}
				} else { // Caso tenha apenas o indice 0, limpar o agrupador
					unset($agrupadorescoluna);
				}
				break;
		}
		switch($grupoitem['tpgidlinha']) {
			case TPG_LDINAM_TEXT:
				if(!$_REQUEST['perid']) {
					$sql = "SELECT perid FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$anoreferente."' ORDER BY perid LIMIT 1";
					$perid = $db->pegaUm($sql);
					if($perid) $_REQUEST['perid'] = $perid;
				}
				$linhaDinTx = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' AND esuid = '". $_SESSION['rehuf_var']['esuid'] ."' AND (linano IN('".$anoreferente."') ".(($anoreferente<=MUDANCA_ANO1)?"OR linano IS NULL":"").") ".(($_REQUEST['perid'])?"AND perid='".$_REQUEST['perid']."'":"")." ORDER BY linordem");
				break;
			case TPG_LDINAM_OPCOES:
				if(!$_REQUEST['perid']) {
					$sql = "SELECT perid FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$anoreferente."' ORDER BY perid LIMIT 1";
					$perid = $db->pegaUm($sql);
					if($perid) $_REQUEST['perid'] = $perid;
				}
				$linhaDinOp = (array) $db->carregar("SELECT * FROM rehuf.linha lin
													 LEFT JOIN rehuf.opcoes opc ON opc.opcid = lin.opcid  
													 WHERE gitid = '". $grupoitem['gitid'] ."' AND esuid = '". $_SESSION['rehuf_var']['esuid'] ."' ".(($anoreferente)?"AND (lin.linano IN('".$anoreferente."') ".(($anoreferente<=MUDANCA_ANO1)?"OR linano IS NULL":"").")":"")." ".(($_REQUEST['perid'])?"AND perid='".$_REQUEST['perid']."'":"")." ORDER BY linordem");
				break;
			case TPG_LFIXAS_SN:
				$linhas = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY linordem");
				break;
			case TPG_LFIXAS_CN:
				$agrupadoreslinha = (array) $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid = '". $grupoitem['gitid'] ."' AND agplinha = true ORDER BY agpordem");
				if($agrupadoreslinha[0]) {
					foreach($agrupadoreslinha as $agplinha) {
						$linhas[$agplinha['agpid']] = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' AND agpid = '". $agplinha['agpid'] ."' ORDER BY linordem");
						if(!$linhas[$agplinha['agpid']][0]) {
							$linhas[$agplinha['agpid']][0] = array("lindsc" => "Sem coluna");;
						}
					}
				} else {
					unset($agrupadoreslinha);
				}
				break;
		}
	}
	monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
	monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
	echo "<br />";
	// Criando navegação no cabeçalho (navegando entre os grupos da tabela)
	$grupositem = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' AND gitvisivel=true ORDER BY gitordem");
	// Fazendo controle dos grupos de itens, encontrando qual posição o grupo escolhido se encontra no Array contendo todos
	if($grupositem[0]) {
		$i = 0;
		foreach($grupositem as $gri) {
			if($gri['gitid']==$grupoitem['gitid']) {
				$crtgrupoitem = $i;
			}
			$i++;
		}
	} else {
		echo "<script>alert('Não existem subgrupos dentro da tabela');window.location='?modulo=inicio&acao=C';</script>";
		exit;
	}
	if(count($grupositem) > 1) {
		$crtgrupo = "(".($crtgrupoitem+1)."/".count($grupositem).")";
		if($crtgrupoitem != '0') {
			$crtback = "<input type=\"button\" value=\"<<\" onclick=\"window.location='?modulo=principal/editartabela&acao=A&gitid=".$grupositem[($crtgrupoitem-1)]['gitid']."&tabtid=".$_REQUEST['tabtid']."'\">";
		}
		if(($crtgrupoitem+1)!=count($grupositem)) {
			$crtfoward = "<input type='button' value=\">>\" onclick=\"window.location='?modulo=principal/editartabela&acao=A&gitid=".$grupositem[($crtgrupoitem+1)]['gitid']."&tabtid=".$_REQUEST['tabtid']."'\">";
		}
		// imprimindo cabeçalho (contendo nome do grupo com paginação e nome do hospital)
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
				<tr><td align='center'><span class=\"TituloTela\" style=\"color:#000000;\">".$crtback." ".$tabela['tabtdsc']." ".$crtgrupo ." ".$crtfoward."</span> ".((count($permissoes['verhospitais'])>1)?"<input type='button' value='Comparar' onclick=\"window.open('?modulo=principal/comparargrupos&acao=A&gitid=".$grupoitem['gitid']."','Comparar','scrollbars=yes,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');\">":"")."</td></tr>
			  </table>";
	} else {
		// imprimindo cabeçalho (contendo nome do grupo com paginação e nome do hospital)
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			  <tr><td align='center'><span class=\"TituloTela\" style=\"color:#000000;\">".$tabela['tabtdsc']."</span> ".((count($permissoes['verhospitais'])>1)?"<input type='button' value='Comparar' onclick=\"window.open('?modulo=principal/comparargrupos&acao=A&gitid=".$grupoitem['gitid']."&ano='+document.getElementById('ctiexercicio').value,'Comparar','scrollbars=yes,height=500,width=700,status=no,toolbar=no,menubar=no,location=no');\">":"")."</td></tr>
			  </table>";
		
	}
	echo "<br />";
	?>
	<div style="overflow:auto;position:relative;" id="controlerolagem">
	<?
	
	/* Se o tipo de coluna for igual a 'referentes ao ano', não imprime abas.
	 * Tratamento especial para o tipo 'colunas fixas referente por ano', onde
	 * o controle do exercicio é feito descrição da coluna (coldsc) 
	 */
	if($grupoitem['tpgidcoluna'] != TPG_CFIXAS_PA) {
		/*
		 * Verificando se o usuário possui filtro de ano para visualização
		 * Se tiver, utilizar período predefinido, senão utilizar período global da tabela
		 */
	
		if($permissoes['filtrodadosano']) {
			$anoini = $permissoes['filtrodadosano']['anoini'];
			$anofim = $permissoes['filtrodadosano']['anofim'];
		} else {
			$anoini = $tabela['tabanoini'];
			$anofim = $tabela['tabanofim'];
		}
		
		// Criando Abas de navegação por ano
		for($i = $anoini; $i <= $anofim; $i++) {
			$ano[] = array("id" => $i,
						   "descricao" => $i,
						   "link" => "?modulo=principal/editartabela&acao=A&gitid=". $grupoitem['gitid'] ."&tabtid=". $_REQUEST['tabtid'] ."&ano=".$i);	
		}
		$_REQUEST['ano'] = (($_REQUEST['ano'])?$_REQUEST['ano']:$anofim);
		// Caso o grupo item tenha abas por ano, criar filtro para pegar as respostas apenas por ano
		$fano = "cti.ctiexercicio = '". $_REQUEST['ano'] ."' AND";
		echo montarAbasArray($ano, "?modulo=principal/editartabela&acao=A&gitid=". $grupoitem['gitid'] ."&tabtid=". $_REQUEST['tabtid'] ."&ano=".$_REQUEST['ano']);
		/*
		 * Código que trata a divisão do ano em períodos 
		 */
		$sql = "SELECT perid as codigo, perdsc as descricao FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$_REQUEST['ano']."' ORDER BY perid";
		$periodogrupoitem = $db->carregar($sql);
		if($periodogrupoitem[0]) {
			foreach($periodogrupoitem as $c => $periodo) {
				// se não tiver o periodo selecionado, pegar o primeiro da lista
				if(!$c && !$_REQUEST['perid']) {
					$_REQUEST['perid'] = $periodo['codigo'];
				}
				$dadoscombo[] = array("codigo"=>$periodo['codigo'],"descricao"=>($c+1)." : ".$periodo['descricao']);
			}
			
			echo "<script>
					function mudarperiodo(perid) {
						window.location='?modulo=principal/editartabela&acao=A&gitid=".$grupoitem['gitid']."&tabtid=".$_REQUEST['tabtid']."&ano=".$_REQUEST['ano']."&perid='+perid;
					}
				  </script>";
			
			echo "<table class=\"TabelaEditRehuf\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			echo "<tr>
				  <td class='SubTituloDireita'><b>Períodos de cadastramento no ano :</b></td>
				  <td>";
			$perid = $_REQUEST['perid'];
			$db->monta_combo('perid', $dadoscombo, 'S', '', 'mudarperiodo', '', '', '200', 'S', 'perid');
			echo "</td></tr>";
			echo "</table>";
		}
		/*
		 * FIM Código que trata a divisão do ano em períodos 
		 */
	} else {
		/*
		 * Código que trata a divisão do ano em períodos nas tabelas por ano
		 */
		$sql = "SELECT perid as codigo, perdsc as descricao, perano FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' ORDER BY perid";
		$periodogrupoitem = $db->carregar($sql);
		if($periodogrupoitem[0]) {
			foreach($periodogrupoitem as $c => $periodo) {
				$dadosperiodopa[$periodo['perano']][] = array("codigo"=>$periodo['codigo'],"descricao"=>($c+1)." : ".$periodo['descricao']);
			}
		}
		$sql = "SELECT pgdid, colid, perano FROM rehuf.periodogrupoitemacumulado pga 
				INNER JOIN rehuf.coluna col ON col.coldsc = pga.perano 
				WHERE pga.gitid='".$grupoitem['gitid']."' AND col.gitid='".$grupoitem['gitid']."'";
		
		$itemacumulado = $db->carregar($sql);
		if($itemacumulado[0]) {
			foreach($itemacumulado as $iacum) {
				$dadosacum[] = array('coluna' => $iacum['colid'],
									 'ano'    => $iacum['perano']);
			}
		}
		/*
		 * FIM Código que trata a divisão do ano em períodos nas tabelas por ano
		 */
	}
	
	// Carregando as respostas dadas no itens
	$sql = "SELECT cti.* FROM rehuf.grupoitem git 
			LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid 
			LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid 
			LEFT JOIN rehuf.conteudoitem cti ON cti.linid = lin.linid AND cti.colid = col.colid  
			WHERE ". $fano ." git.gitid = '".$grupoitem['gitid']."' ".(($_REQUEST['perid'])?"AND cti.perid='".$_REQUEST['perid']."'":"")." AND cti.esuid = '". $_SESSION['rehuf_var']['esuid'] ."' ".(($_REQUEST['perid'] && ($grupoitem['tpgidlinha']==TPG_LDINAM_TEXT || $grupoitem['tpgidlinha']==TPG_LDINAM_OPCOES))?"AND cti.perid='".$_REQUEST['perid']."' AND lin.perid='".$_REQUEST['perid']."'":"");
	$rsps = (array) $db->carregar($sql);
	foreach($rsps as $rsp) {
		if($rsp['perid']) {
			$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']][$rsp['perid']] = $rsp['ctiobs'];
			$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']][$rsp['perid']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
		} else {
			$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
			$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
		}
	}
?>
	<script>
	// Função utilizada na inserção de linhas com o campo text
	// No momento do clique no ícone de inserir
	function inserirlinhatx(linha, cols) {
		ajaxatualizar('requisicao=inserirlinhadintx&lindsc='+document.getElementById('lindsc_'+linha).value+'&gitid=<? echo $grupoitem['gitid']; echo (($_REQUEST['ano'])?"&ano=".$_REQUEST['ano']:""); echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>','');
		window.location.href='rehuf.php?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $grupoitem['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>';
		history.go(0);
	}
	function removerlinhatx(linid) {
		var conf = confirm("Esta ação irá apagar todos os dados referentes a esta linha. Deseja continuar ?");
		if(conf) {
			ajaxatualizar('requisicao=removerlinhadintx&linid='+linid,'');
			window.location.href='rehuf.php?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $grupoitem['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>';
			history.go(0);
		}
	}
	
	function removerlinha(opcid) {
		var conf = confirm("Esta ação irá apagar todos os dados referentes a esta linha. Deseja continuar ?");
		if(conf) {
			ajaxatualizar('requisicao=removerlinhadinop&opcid='+opcid+'&gitid=<? echo $grupoitem['gitid']; ?>','');
			window.location='?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $grupoitem['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>'
			alert('Linha removida com sucesso');
		}
	}
	
	</script>
	<input type='hidden' name='gitid' value='<? echo $grupoitem['gitid']; ?>'>
	<input type='hidden' name='esuid' value='<? echo $_SESSION['rehuf_var']['esuid']; ?>'>
	<input type='hidden' name='ctiexercicio' id='ctiexercicio' value='<? echo $_REQUEST['ano']; ?>'>
	<?
	if($dadosacum) {
		foreach($dadosacum as $acm) {
			$sql = "SELECT * FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$acm['ano']."'";
			$periodogrupositem = $db->carregar($sql);
			if($periodogrupositem[0]) {
				$grup = end($periodogrupositem);
				echo "<input type='hidden' id='colunaacum_".$acm['coluna']."' name='colunaacum_".$acm['coluna']."' value='".$grup['perid']."'>";
			}
		}
	}
	?>
	<table class="TabelaEditRehuf" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?	if($grupoitem) { ?>
	<tr>
		<td width='50%' <? if($grupoitem['gitobs']) {?>onmouseover="return escape('<? echo nl2br($grupoitem['gitobs']); ?>')"<? } ?> class='stylecolunas' <? echo (($agrupadorescoluna)?'rowspan="2"':''); echo (($agrupadoreslinha)?'colspan="2"':''); ?> ><strong><font style="font-size:13px;"><? echo $grupoitem['gitdsc']; ?></font></strong></td>
		<?
		// Caso o grupo tenha agrupadores de de coluna, imprime os agrupadores
		if($agrupadorescoluna) {
			/*
			 * INÍCIO
			 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS  
			 */
			
			// Verifica se o grupo item possui uma coluna totalizadora
			if($grupoitem['gitpossuitotalcoluna']=='t') {
				// a mascara do totalizador será igual a mascara da primeira coluna, do primeiro agrupador
				$mask = current($coluna[$agrupadorescoluna[0]['agpid']]);
				$mask = $mask['tpimascara'];
				
				$agrupadorescoluna[] = array("agpid" => "total");
				$coluna["total"][] = array("colid" => "colunatotal", "coldsc" => "<strong>TOTAL</strong>", "tpitipocampo" => "textpossuitotalcoluna", "tpimascara" => $mask, "colobs" => "Total geral");
			}
			foreach($agrupadorescoluna as $agpcoluna) {
				// Verifica se o agrupador possui totalizador, obs: totalizar somente od itens do agrupador, e não todos
				if($agpcoluna['agppossuitotal']=='t') {
					$mask = current($coluna[$agpcoluna['agpid']]);
					$mask = $mask['tpimascara'];
					
					$coluna[$agpcoluna['agpid']][] = array("coldsc" => "<strong>TOTAL</strong>", "tpitipocampo" => "textpossuitotalagrupador", "agpid" => $agpcoluna['agpid'], "tpimascara" => $mask, "colobs" => "Total do agrupador",);
				}
				// imprimindo o primeiro nivel das colunas (agrupadores), em seguida imprimir o segundo nivel contendo as colunas (COD. LINE: +- 261 )
				echo "<td class='stylecolunas' colspan=". (count($coluna[$agpcoluna['agpid']])) .">".$agpcoluna['agpdsc']."</td>";
			}
			// Caso o tipo de linha seja dinâmica, inserir uma coluna de ações
			if($linhaDinOp || $linhaDinTx) {
				echo "<td rowspan='2' onmouseover=\"return escape('Ações');\" class='stylecolunas'>Ações</td>";	
			}
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS  
			 */
			
		} elseif($coluna) { // Caso não tenha agrupadores, imprimir as colunas diretamente
			/*
			 * INÍCIO
			 * FORMATO DE COLUNA : COLUNAS FIXAS SEM SUBNIVEIS  
			 */
			// Verifica se o grupo item possui uma coluna totalizadora (coluna no final com o total de todas as colunas)
			if($grupoitem['gitpossuitotalcoluna']=='t') {
				// a mascara do totalizador será igual a mascara da primeira coluna
				$mask = current($coluna);
				$mask = $mask['tpimascara'];
				
				$coluna[] = array("colid" => "colunatotal",
								  "coldsc" => "<strong>TOTAL</strong>", 
								  "tpitipocampo" => "textpossuitotalcoluna",
								  "colobs" => "Total geral",
								  "tpimascara" => $mask);
			}
			// Imprimindo as colunas (sem agrupadores)
			foreach($coluna as $col) {
				echo "<td class='stylecolunas' ".(($col['colobs'])?"onmouseover=\"return escape('". str_replace("\n","<br />",$col['colobs']) ."');\"":"").">". $col['coldsc'] ."</td>";
			}
			// Caso o tipo de linha seja dinâmica, inserir uma coluna de ações (para inserir os ícones de inserir e remover)
			if($linhaDinOp || $linhaDinTx) {
				echo "<td class='stylecolunas' onmouseover=\"return escape('Ações');\">Ações</td>";	
			}
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS SEM SUBNIVEIS  
			 */
		} elseif($colunapa) {
			/*
			 * INÍCIO
			 * FORMATO DE COLUNA : COLUNAS FIXAS REFERENTES AOS ANOS DA TABELA  
			 */
			
			// Verifica se o grupo item possui uma coluna totalizadora
			if($grupoitem['gitpossuitotalcoluna']=='t') {
				// a mascara do totalizador será igual a mascara da primeira coluna
				$mask = current($colunapa);
				$mask = $mask['tpimascara'];
				
				$colunapa[] = array("colid" => "colunatotal",
									"coldsc" => "<strong>TOTAL</strong>", 
									"tpitipocampo" => "textpossuitotalcoluna",
									"colobs" => "Total geral",
									"tpimascara" => $mask);
			}
			// imprimindo as colunas
			foreach($colunapa as $col) {
				echo "<td class='stylecolunas'>". $col['coldsc'] ."</td>";
			}
			// Caso o tipo de linha seja dinâmica, inserir uma coluna de ações
			if($linhaDinOp || $linhaDinTx) {
				echo "<td class='stylecolunas' onmouseover=\"return escape('Ações');\">Ações</td>";	
			}
			
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS REFERENTES AOS ANOS DA TABELA  
			 */
		}
		?>
	</tr>
	<?
	/*
	 * INÍCIO
	 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS (2ºETAPA)
	 * Caso tenha agrupadores de colunas, este laço irá imprimir as colunas do agrupadores
	 * é uma nova linha referente aos dados de coluna (ainda representa o cabeçalho)  
	 */
	if($agrupadorescoluna) {
		echo "<tr>";
		foreach($agrupadorescoluna as $agpcoluna) {
			if($coluna[$agpcoluna['agpid']]) {
				foreach($coluna[$agpcoluna['agpid']] as $col) {
					echo "<td class='stylecolunas' ".(($col['colobs'])?"onmouseover=\"return escape('".$col['colobs']."');\"":"").">".$col['coldsc']."</td>";
				}
			}
		}
		echo "</tr>";
	}
	/*
	 * FIM
	 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS (2ºETAPA)
	 */
	

	/*
	 * INÍCIO
	 * FORMATO DE LINHA : LINHAS FIXAS COM SUBNIVEIS
	 * Caso tenha agrupadores por linha
	 */

	if($agrupadoreslinha) {
		// verificando se existe linha totalizador geral
		if($grupoitem['gitpossuitotallinha'] == 't') {
			$agrupadoreslinha[] = array("agpid" => "total");
			$linhas['total'][] = array("lindsc" => "<strong>TOTAL</strong>", "lintipocampo" => "textpossuitotallinha");
		}
		// varrendo as linhas fixas cadastradas
		foreach($agrupadoreslinha as $agplinha) {
			// se o agrupador de linha tiver sublinhas (o normal é ter linhas dentro do agrupador)
			if(count($linhas[$agplinha['agpid']]) > 0) {
				// imprimindo a primeira linha com agrupador(rowspan)
				echo "<tr>";
				echo "<td class='stylecolunas' rowspan='". count($linhas[$agplinha['agpid']]) ."'>".$agplinha['agpdsc']."</td>";
				echo "<td class='stylelinhas' ".(($linhas[$agplinha['agpid']][0]['linobs'])?"onmouseover=\"return escape('".$linhas[$agplinha['agpid']][0]['linobs']."');\"":"").">".$linhas[$agplinha['agpid']][0]['lindsc']." : </td>";
				// definindo campos da linha (varrendo os dados de coluna)
				if($agrupadorescoluna) { // verifica se o formato de coluna é 'fixas com subniveis'
					foreach($agrupadorescoluna as $agpcoluna) {
						if($coluna[$agpcoluna['agpid']]) {
							foreach($coluna[$agpcoluna['agpid']] as $col) {
								// 	verificando se pode existir obs
								unset($campoobs);
								if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
									$campoobs = definirobservacao($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']);
								}
								echo "<td class='styleconteudo' nowrap>".definircampo($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
							}
						}
					}
				} elseif($coluna) { // verifica se o formato de coluna é 'fixas sem subniveis'
					foreach($coluna as $col) {
						// 	verificando se pode existir obs
						unset($permiteobservacao);
						if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						if($linhas[$agplinha['agpid']][0]['lintipocampo']) {
							$col['tpitipocampo'] = $linhas[$agplinha['agpid']][0]['lintipocampo'];
						}
						
						echo "<td class='styleconteudo' nowrap>". definircampo($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano'], $permiteobservacao)."</td>";
					}
				} elseif($colunapa) {// verifica se o formato de coluna é 'fixas refentes ao ano'
					foreach($colunapa as $col) {
						// 	verificando se pode existir obs
						unset($permiteobservacao);
						if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						if($linhas[$agplinha['agpid']][0]['lintipocampo']) {
							$col['tpitipocampo'] = $linhas[$agplinha['agpid']][0]['lintipocampo'];
						}
						echo "<td class='styleconteudo' nowrap>". definircampo($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc'],$permiteobservacao) ."<input type='hidden' name='anoexercicioitem[". $linhas[$agplinha['agpid']][0]['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'></td>";
					}
				}
				
				unset($linhas[$agplinha['agpid']][0]);
				
				echo "</tr>";
				// fim da primeira linha com o agrupador
				// imprime as demais linhas contidas no agrupador
				foreach($linhas[$agplinha['agpid']] as $sublinha) {
					echo "<tr>";
					echo "<td class='stylelinhas' ".(($sublinha['linobs'])?"onmouseover=\"return escape('".$sublinha['linobs']."');\"":"").">".$sublinha['lindsc']." : </td>";
					
					if($agrupadorescoluna) {
						foreach($agrupadorescoluna as $agpcoluna) {
							if($coluna[$agpcoluna['agpid']]) {
								foreach($coluna[$agpcoluna['agpid']] as $col) {
									// verificando se pode existir obs
									unset($permiteobservacao);
									if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
										$permiteobservacao = true;
									}
									echo "<td class='styleconteudo' nowrap>".definircampo($col,$sublinha['linid'],$_REQUEST['ano'],$permiteobservacao)."</td>";
								}
							}
						}
					} elseif($coluna) {
						foreach($coluna as $col) {
							// verificando se pode existir obs
							unset($permiteobservacao);
							if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$permiteobservacao = true;
							}
							
							echo "<td class='styleconteudo' nowrap>". definircampo($col,$sublinha['linid'],$_REQUEST['ano'],$permiteobservacao) ."</td>";
						}	
					} elseif($colunapa) {
						foreach($colunapa as $col) {
							// verificando se pode existir obs
							unset($permiteobservacao);
							if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$permiteobservacao = true;
							}
							
							echo "<td class='styleconteudo' nowrap>". definircampo($col,$sublinha['linid'],$col['coldsc'],$permiteobservacao) ."<input type='hidden' name='anoexercicioitem[". $sublinha['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'></td>";
						}
					}
					echo "</tr>";
				}
				// fim das linhas do agrupador
			}
		}
	/*
	 * FIM
	 * FORMATO DE LINHA : LINHAS FIXAS COM SUBNIVEIS
	 */
		
	} elseif($linhas[0]) { // Caso não tenha agrupadores de linha, imprimir as linhas diretamente
		// verificando se existe linha totalizador geral
		if($grupoitem['gitpossuitotallinha'] == 't') {
			$linhas[] = array("lindsc" => "<strong>TOTAL</strong>", "lintipocampo" => "textpossuitotallinha");
		}
		
		foreach($linhas as $lin) {
			echo "<tr>";
			echo "<td class='stylelinhas' ".(($lin['linobs'])?"onmouseover=\"return escape('".$lin['linobs']."');\"":"").">".$lin['lindsc']."</td>";
			if($agrupadorescoluna) {
				foreach($agrupadorescoluna as $agpcoluna) {
					foreach($coluna[$agpcoluna['agpid']] as $col) {
						// 	verificando se pode existir obs
						unset($permiteobservacao);
						if(($lin['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						// verificando se é uma linha totalizadora
						if($lin['lintipocampo']) {
							$col['tpitipocampo'] = $lin['lintipocampo'];
						}
						echo "<td class='styleconteudo' nowrap>". definircampo($col,$lin['linid'],$_REQUEST['ano'],$permiteobservacao) ."</td>";
					}
				}
			}elseif($coluna) {
				foreach($coluna as $col) {
					if(!$dadoscombo[$col['gpoid']][0] && $col['gpoid']) {
						$dadoscombo[$col['gpoid']] = $db->carregar("SELECT opcid AS codigo, opcdsc AS descricao FROM rehuf.opcoes WHERE gpoid='".$col['gpoid']."' ORDER BY descricao");
					}
					// verificando se pode existir obs
					unset($permiteobservacao);
					if(($lin['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
						$permiteobservacao = true;
					}
					// verificando se é uma linha totalizadora
					if($lin['lintipocampo']) {
						$col['tpitipocampo'] = $lin['lintipocampo'];
					}
					
					echo "<td class='styleconteudo' nowrap>". definircampo($col,$lin['linid'],$_REQUEST['ano'],$permiteobservacao)."</td>";
				}
			} elseif($colunapa) {
				foreach($colunapa as $col) {
					// verificando se pode existir obs
					unset($permiteobservacao);
					if(($lin['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
						$permiteobservacao = true;
					}
					
					// verificando se é uma linha totalizadora
					if($lin['lintipocampo']) {
						$col['tpitipocampo'] = $lin['lintipocampo'];
					}
					echo "<td class='styleconteudo' nowrap>". definircampo($col,$lin['linid'],$col['coldsc'],$permiteobservacao) ."<input type='hidden' name='anoexercicioitem[". $lin['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'></td>";
				}
			}
			echo "</tr>";
		}
	} elseif($linhaDinOp) { // Carregar as linhas que foram criadas dinamicamente
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÕES(COMBOBOX)  
	 	*/
		// verficar se existe linhas dinâmicas com opções cadastradas para tal entidade
		if($linhaDinOp[0]) {
			// verificando se existe linha totalizador geral
			if($grupoitem['gitpossuitotallinha'] == 't') {
				$linhaDinOp[] = array("opcdsc" => "<strong>TOTAL</strong>", "lintipocampo" => "textpossuitotallinha");
			}
			//imprimindo linhas ja cadastradas
			foreach($linhaDinOp as $lindiop) {
				echo "<tr>";
				echo "<td nowrap>";
				echo $lindiop['opcdsc'];
				echo "</td>";
				// construindo o campo editavel, de acordo com o formato de colunas,
				// o campo é construido por linha
				if($agrupadorescoluna) {
					// varrendo as colunas dentro dos agrupadores
					foreach($agrupadorescoluna as $agpcoluna) {
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							// verificando se pode existir obs
							unset($permiteobservacao);
							if(($lindiop['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$permiteobservacao = true;
							}
							// verificando se é uma linha totalizadora
							if($lindiop['lintipocampo']) {
								$col['tpitipocampo'] = $lindiop['lintipocampo'];
							}
							
							echo "<td>".definircampo($col,$lindiop['linid'],$_REQUEST['ano'],$permiteobservacao)."</td>";
						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {
						// verificando se pode existir obs
						unset($permiteobservacao);
						if(($lindiop['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						// verificando se é uma linha totalizadora
						if($lindiop['lintipocampo']) {
							$col['tpitipocampo'] = $lindiop['lintipocampo'];
						}
						echo "<td>".definircampo($col,$lindiop['linid'],$_REQUEST['ano'],$permiteobservacao)."</td>";
					}
				} elseif($colunapa) {
					foreach($colunapa as $col) {
						
						// verificando se pode existir obs
						unset($permiteobservacao);
						if(($lindiop['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						// verificando se é uma linha totalizadora
						if($lindiop['lintipocampo']) {
							$col['tpitipocampo'] = $lindiop['lintipocampo'];
						}
						echo "<td class='styleconteudo'>". definircampo($col,$lindiop['linid'],$col['coldsc'],$permiteobservacao) ."<input type='hidden' name='anoexercicioitem[". $lindiop['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'></td>";
					}
				}
				echo "<td>". (($permissoes['gravar'] && $lindiop['lintipocampo'] != "textpossuitotallinha")?"<img src=\"../imagens/excluir.gif\" onclick=\"removerlinha(". $lindiop['opcid'] .");\" style=\"cursor:pointer\">":"&nbsp") ."</td>";
				echo "</tr>";
			}
		}
		if($permissoes['gravar']) {
			// essa linha sempre será a última, estatica para inserção
			echo "<tr>";
			// definindo o número de colunas para saber o tamanho do colspan 
			if($agrupadorescoluna) {
				$clspan = 1;
				foreach($agrupadorescoluna as $agpcoluna) {
					$clspan += count($coluna[$agpcoluna['agpid']]);
				}
			}elseif($coluna) {
				$clspan = (count($coluna)+1);
			}
			// colocando o ícone de inserir na última coluna
			echo "<td colspan='".($clspan+1)."'><a href=\"javascript:void(0);\" onclick=\"window.open('?modulo=principal/inserirlinhaop&acao=A&ano=".$_REQUEST['ano']."&tabtid=". $tabela['tabtid'] ."&esuid=". $_SESSION['rehuf_var']['esuid'] ."&gitid=".$grupoitem['gitid']."".(($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:"")."',null,'scrollbars=yes,height=600,width=600,status=no,toolbar=no,menubar=no,location=no');\"><img src=\"../imagens/gif_inclui.gif\" border=\"0\" style=\"cursor:pointer\"> Incluir linha</a></td>";
			unset($coljs);
			echo "</tr>";
		}
	   /*
	 	* FIM
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÕES(COMBOBOX)  
	 	*/
		
	} elseif($linhaDinTx) {
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÃO DE ESCREVER  
	 	*/
		
		if($linhaDinTx[0]) {
			foreach($linhaDinTx as $linditx) {
				echo "<tr>";
				echo "<td>";
				$nometx = 'lindsc_'.$linditx['linid'];
				$$nometx = $linditx['lindsc'];
				echo campo_texto($nometx, "N", (($permissoes['gravar'])?"S":"N"), "Descrição da linha", 60, 240, "", "", '', '', 0, 'id="gitdsc_'.$linditx['linid'].'"' );
				echo "</td>";
				if($agrupadorescoluna) {
					$clspan = 1;
					foreach($agrupadorescoluna as $agpcoluna) {
						$clspan += count($coluna[$agpcoluna['agpid']]);
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							// verificando se pode existir obs
							unset($permiteobservacao);
							if(($linditx['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$permiteobservacao = true;
							}
							
							echo "<td>". definircampo($col,$linditx['linid'],$_REQUEST['ano'],$permiteobservacao)."</td>";
						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {
						$clspan = (count($coluna)+1);
						
						// verificando se pode existir obs
						unset($permiteobservacao);
						if(($linditx['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						echo "<td>". definircampo($col,$linditx['linid'],$_REQUEST['ano'],$permiteobservacao)."</td>";
					}
				} elseif($colunapa) {
					foreach($colunapa as $col) {
						
						// verificando se pode existir obs
						unset($permiteobservacao);
						if(($linditx['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$permiteobservacao = true;
						}
						echo "<td class='styleconteudo'>". definircampo($col,$linditx['linid'],$col['coldsc'],$permiteobservacao) ."<input type='hidden' name='anoexercicioitem[". $linditx['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'></td>";
					}
				}
				echo "<td>".(($permissoes['gravar'])?"<img src=\"../imagens/excluir.gif\" onclick=\"removerlinhatx('". $linditx['linid'] ."');\" style=\"cursor:pointer\">":"&nbsp")."</td>";
				echo "</tr>";
			}
		} 
		if($permissoes['gravar']) {
			echo "<tr style='display:none'>";
			echo "<td>"; 
			echo campo_texto('lindsc_insercao', "N", "S", "Descrição da linha", 50, 240, "", "", '', '', 0, 'id="lindsc_insercao"' );
			echo "</td>";
			if($agrupadorescoluna) {
				foreach($agrupadorescoluna as $agpcoluna) {
					foreach($coluna[$agpcoluna['agpid']] as $col) {
						echo "<td>". definircampo($col,'insercao') ."</td>";
					}
				}
			}elseif($coluna) {
				foreach($coluna as $col) {
					echo "<td>". definircampo($col,'insercao') ."</td>";
				}
				
			} elseif($colunapa) {
				foreach($colunapa as $col) {
					echo "<td>". definircampo($col,'insercao') ."</td>";
				}
				
			}
			// referente a ações
			echo "<td>&nbsp;</td>";
			echo "</tr>";
			echo "<tr>";
			echo "<td><a href=\"javascript:void(0);\" onclick=\"inserirlinhatx('insercao', '');\"><img src=\"../imagens/gif_inclui.gif\" border=\"0\" style=\"cursor:pointer\"> Incluir linha</a></td>";
			unset($coljs);
			echo "</tr>";
		}
		
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÃO DE ESCREVER  
	 	*/
		
	}
}
?>
</table>
</div>

<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<div style="float: left;">
		<? if($permissoes['gravar']) { ?>
		<input type='submit' value='Gravar' onclick="document.getElementById('alteracaodados').value=0;">
		<? } ?>
		<input type='button' onclick="this.disabled=true;window.location='?modulo=principal/listartabela&acao=A&entid=<? echo $_SESSION['rehuf_var']['entid']; ?>'" value='Voltar'>
		</div>
	</td>
</tr>
</table>


<script>
var myWidth;
var myHeight;
if( typeof( window.innerWidth ) == 'number' ) {
   	//Non-IE
   	myWidth = window.innerWidth;
   	myHeight = window.innerHeight;
	document.getElementById('controlerolagem').style.left = '50%';
	document.getElementById('controlerolagem').style.marginLeft = Arredonda((-1*myWidth*0.93)/2,0);
	document.getElementById('controlerolagem').style.width = Arredonda(myWidth*0.93,0);
	document.getElementById('controlerolagem').style.height = (myHeight-375);
} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
   	//IE 6+ in 'standards compliant mode'
   	myWidth = document.documentElement.clientWidth;
   	myHeight = document.documentElement.clientHeight;
	document.getElementById('controlerolagem').style.marginLeft = Arredonda((-1*myWidth*0.95)/2,0);
	document.getElementById('controlerolagem').style.width = Arredonda(myWidth*0.95,0);
	document.getElementById('controlerolagem').style.height = (myHeight-775);
} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
   	//IE 4 compatible
   	myWidth = document.body.clientWidth;
   	myHeight = document.body.clientWidth;
	document.getElementById('controlerolagem').style.left = '50%';
	document.getElementById('controlerolagem').style.marginLeft = Arredonda((-1*myWidth*0.93)/2,0);
	document.getElementById('controlerolagem').style.width = Arredonda(myWidth*0.93,0);
	document.getElementById('controlerolagem').style.height = (myHeight-1070);

}

</script>
<? 
} else {
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
		  <tr>
			<td colspan=\"2\">
			<div style=\"float: left;\"><strong>Estrutura não encontrada.</strong></div>
			</td>
		  </tr>
		  </table>"; 
}
?>
</form>