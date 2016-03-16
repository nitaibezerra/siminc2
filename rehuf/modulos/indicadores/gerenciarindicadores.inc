<?
include APPRAIZ ."includes/cabecalho.inc";
include "_funcoesindicadores.php";
echo '<br>';

define("BTN_VOLTAR", true);

// montando o menu
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=indicadores/gerenciarindicadores&acao=A");

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf($estid);
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

if($_REQUEST['indid']) {
	// carregar o indicador
	$arquivo = $db->pegaLinha("SELECT indarquivo, inddsc, indformula, indanoini, indanofim FROM rehuf.indicador WHERE indid='".$_REQUEST['indid']."'");
	// pegar da tabela indicadores
	for($i = $arquivo['indanoini'];$i<=$arquivo['indanofim'];$i++) {
		$_anos[] = $i;
	}
	// pegar o subitens do indicador
	$sql = "SELECT * FROM rehuf.indicadorsubitem WHERE indid='".$_REQUEST['indid']."' ORDER BY sinordem";
	$subitensindicadores = $db->carregar($sql);
	// carregando os valores
	$sql = "SELECT * FROM rehuf.indicadorvalor";
	$indicadorvalores = $db->carregar($sql);
	if($indicadorvalores) {
		foreach($indicadorvalores as $valores) {
			$indvalores["{valor".$valores['invid']."}"] = $valores['invselect'];
		}
	}
	
	echo "<link rel=\"stylesheet\" href=\"/includes/LyteBox/lytebox.css\" type=\"text/css\" media=\"screen\" />
		  <script type=\"text/javascript\" src=\"/includes/LyteBox/lytebox.js\"></script>";
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	if($_anos) {
		echo "<tr>
			  <td class=\"SubTituloCentro\" width=\"400\">".$arquivo['inddsc'];
		if($arquivo['indformula']) {
			echo "<br /><font size=1>Fórmula: ".$arquivo['indformula']."</font>";
		}
		echo "</td>";
		foreach($_anos as $ano) {
			echo "<td class=\"SubTituloCentro\">".$ano."</td>";
		}
		echo "</tr>"; 
	}
	
	if($subitensindicadores) {
		foreach($subitensindicadores as $subitensindicador) {
			echo "<tr>";
			if($subitensindicador['sincores']) {
				echo "<td class=\"SubTituloDireita\"><a href=\"rehuf.php?modulo=indicadores/grafico_sub_indicador&acao=A&sinid=".$subitensindicador['sinid']."\" rel=\"lyteframe\" rev=\"width: 900px; height: 450px; scrolling: auto;\">".$subitensindicador['sindsc']."</a></td>";	
			} else {
				echo "<td class=\"SubTituloDireita\">".$subitensindicador['sindsc']."</td>";
			}
			
			foreach($_anos as $ano) {
				$indices = array_keys($indvalores);
				$sql_formula = str_replace($indices, $indvalores, $subitensindicador['sinformula']);
				$sql_formula = str_replace(array("{esuid}","{ano}"), array($_SESSION['rehuf_var']['esuid'], $ano), $sql_formula);
				if($sql_formula) {
					$resultado = $db->pegaUm("SELECT (".$sql_formula.")");
				}
				echo "<td align='right'>".number_format($resultado, 2, ',', '')."</td>";
			}
			echo "</tr>";
		}
	}
	echo "</table>";
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr bgcolor=\"#C0C0C0\"><td colspan=\"2\"><div style=\"float: left;\"><input type='button' onclick=\"window.location='?modulo=indicadores/gerenciarindicadores&acao=A';\" value='Voltar'></div></td></tr>
		  </table>";
	
} else {
	$sql = "SELECT * FROM rehuf.dimensao";
	$dimensoes = $db->carregar($sql);
	if($dimensoes[0]) {
		foreach($dimensoes as $dimensao) {
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"0\" cellPadding=\"0\" align=\"center\">";
			echo "<tr><td class='SubTituloCentro'>".$dimensao['dimdsc']."</td></tr>";
			echo "</table>";
			$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=indicadores/gerenciarindicadores&acao=A&indid='|| indid ||'\'\">' AS acao, '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=indicadores/gerenciarindicadores&acao=A&indid='|| indid ||'\'\">'||inddsc||'</a>' FROM rehuf.indicador WHERE dimid='".$dimensao['dimid']."'";
			$cabecalho = array("","Indicador");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);
		}
	}
}
?>