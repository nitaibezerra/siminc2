<?
if($_POST['ajaxCursos']){
	//header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['exec_function']($_REQUEST);
	exit;
}

/* Atribuindo variaveis para SESSÃO */
$_SESSION['sig_var']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];

if($_REQUEST['retid']) {
	$_SESSION['sig_var']['retid'] = (integer) $_REQUEST['retid'];
}else{
	$sql = "SELECT
				retid
			FROM
				academico.reitoriasitem
			WHERE
				entid =" . $_SESSION['sig_var']['entid'];
	$_SESSION['sig_var']['retid'] = $db->pegaUm($sql);
	$_REQUEST['retid'] = $_SESSION['sig_var']['retid'];
}
//ver($_SESSION['sig_var'],d);
if($_REQUEST['orgid']) {
	$_SESSION['academico']['orgid'] = (integer) $_REQUEST['orgid'];
}

$perfilNotBloq = array(PERFIL_IFESCADBOLSAS, 
					   PERFIL_IFESCADCURSOS, 
					   PERFIL_IFESCADASTRO, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR,
					   PERFIL_IFESCADTCU);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança


/* Executando função (_funcoes.php) */
if($_REQUEST['exec_function']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['exec_function']($_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";

// Monta Abas
if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57294' : '57270'; // Dados Desenv
}else{
	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57273' : '57263';
}
$db->cria_aba($abacod_tela,$url,$parametros);
if($_SESSION['sig_var']['entid']) {
	$sql = "SELECT 
				en.estuf AS uf,
				m.mundescricao AS municipio,
				teu.orgid,
				ea.entid AS unidade,
				e.entid as entid_ret
			FROM entidade.entidade e 
			INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
			INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			INNER JOIN entidade.entidade ea ON ea.entid = fea.entid 
			INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ea.entid
			INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid	
			LEFT JOIN entidade.endereco en ON( en.entid = e.entid AND en.tpeid = 1)
			LEFT JOIN territorios.municipio m on m.muncod = en.muncod 	
			WHERE e.entid = ". $_SESSION['sig_var']['entid'] ;
} else {
	// caso não seja passado o parametreo cmpid, enviar para o início do programa
	die("<script>
		  alert('Erro na passagem de parâmetros. O programa será redirecionado a página principal. Caso o erro persista, entre em contato com o administrador.');
		  window.location = '?modulo=inicio&acao=C';
		  </script>");
}
/* Carregando os dados do campus */
$dadoscampus = $db->carregar($sql);
if( $dadoscampus[0] ){
	$dadoscampus = $dadoscampus[0];
}
if($dadoscampus) {
//	$dadoscampus = current($dadoscampus);
	$entid_ret = $dadoscampus['entid_ret'];
	$estuf = $dadoscampus['uf'];
	$mundescricao = $dadoscampus['municipio'];
	$_SESSION['sig_var']['entid'] = $dadoscampus['entid_ret'];
} else {
	die("<script>
			alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
			window.location='?modulo=inicio&acao=C';
		 </script>");
}

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>

<form action="?modulo=principal/dadosindicadorestcu_reitoria&acao=C" method="post" name="formulario" id="formulario">
<?
monta_titulo( "Dados para os Indicadores TCU", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid_ret);

?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro">
				<? echo (($tituloitens[$dadoscampus['orgid']][1])?$tituloitens[$dadoscampus['orgid']][1]:$tituloitens['default']); ?>
			</td>
		</tr>
		<tr>
			<td>
			<?
			// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
			if($anosanalisados[$dadoscampus['orgid']]) {
				$anos = $anosanalisados[$dadoscampus['orgid']];
			} else {
				$anos = $anosanalisados['default'];
			}
			unset($cabecalho,$paramselects);
			$cabecalho[] = "Itens";
			foreach($anos as $ano) {
				$paramselects[] = "'<input ".(!$permissoes['gravar']?'readonly':'')." class=\"normal\" id=\"' || itm.itmid || '".$ano."\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\" name=\"itenstcu[' || itm.itmid || '][". $ano ."][0]\" '|| 
									CASE WHEN tpi.tpimascara is null 
										THEN 'onkeyup=\"\"' 
										ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"' 
									END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" size=\"14\" type=\"text\" value=\"' || 
									CASE WHEN (SELECT 
											(coalesce(rei.rettexto,'') || coalesce(cast(rei.retvalor as varchar),'')) 
										   FROM academico.reitoriasitem rei 
										   WHERE 
											itm.itmid = rei.itmid 
											AND rei.retano = '". $ano ."' 
											AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
											AND rei.rettabnum=1) is null 
										THEN '' 
										ELSE (SELECT 
											(coalesce(rei.rettexto,'') || coalesce(cast(rei.retvalor as varchar),'')) 
										      FROM academico.reitoriasitem rei 
										      WHERE 
											itm.itmid = rei.itmid 
											AND rei.retano = '". $ano ."' 
											AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
											AND rei.rettabnum=1) 
									END || '\"> '||  
									CASE WHEN itm.itmpermiteobs IS TRUE 
											THEN '<img  '|| CASE WHEN ((SELECT 
															rei.retobs 
														    FROM academico.reitoriasitem rei 
														    WHERE 
															itm.itmid = rei.itmid 
															AND rei.retano = '". $ano ."' 
															AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
															AND rei.rettabnum=1) = '') 
														OR ((SELECT 
															rei.retobs 
														     FROM academico.reitoriasitem rei 
														     WHERE 
															itm.itmid = rei.itmid 
															AND rei.retano = '". $ano ."' 
															AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
															AND rei.rettabnum=0) IS NULL) 
																THEN 'src=\"../imagens/edit_off.gif\" border=\"0\"' 
																ELSE 'title=\"'|| (SELECT 
																			rei.retobs 
																		   FROM academico.reitoriasitem rei 
																		   WHERE 
																			itm.itmid = rei.itmid 
																			AND rei.retano = '". $ano ."' 
																			AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
																			AND rei.rettabnum=0) ||'\" src=\"../imagens/edit_on.gif\"' 
															END ||' id=\"img' || itm.itmid || '_". $ano ."\" onclick=\"abreobservacao(\''|| itm.itmid ||'_".$ano."\');\"><input type=\"hidden\" value=\"' || 
															CASE WHEN (SELECT 
																	rei.retobs 
																   FROM academico.reitoriasitem rei 
																   WHERE 
																	itm.itmid = rei.itmid 
																	AND rei.retano = '". $ano ."' 
																	AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
																	AND rei.rettabnum=0) IS NULL 
																THEN '' 
																ELSE (SELECT 
																	rei.retobs 
																      FROM academico.reitoriasitem rei 
																      WHERE 
																	itm.itmid = rei.itmid 
																	AND rei.retano = '". $ano ."' 
																	AND rei.entid = '". $_SESSION['sig_var']['entid'] ."' 
																	AND rei.rettabnum=0) END || '\" id=\"'|| itm.itmid ||'_".$ano."\" name=\"obs['|| itm.itmid ||'][".$ano."]\">' 
											ELSE '' 
										END  ||'' AS ano_".$ano;
				$cabecalho[] = $ano;		
			}
			$paramselects = implode(",",$paramselects);
			// criando o SELECT
			$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>',
					". $paramselects ."
					FROM academico.item itm 
					LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
					LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
					WHERE 
						tei.orgid = '". $dadoscampus['orgid'] ."' 
						AND itm.itmglobal = false 
						AND itm.itmtcu = true
					ORDER BY tei.teiordem";

			$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
			?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2" align="center">	
				<input type="hidden" value="<? echo $_SESSION['sig_var']['entid']; ?>" name="entid">
				<input type="hidden" name="exec_function" value="atualizardadoscampustcu_reitoria">
				<? if($permissoes['gravar']) { ?><input type='button' onclick="document.getElementById('formulario').submit();this.disabled=true" value='Gravar'><? } ?>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>
<script>
var form = document.getElementById('formulario');
for (var j=0; j < form.length; j++){
	if(form[j].name.substr(0,8)=="itenstcu"){
		form[j].onkeyup();
	}
}
function aplicarmascara() {
	var form = document.getElementById('formulario');
	for (var j=0; j < form.length; j++){
		if(form[j].value != "" && form[j].type == "text" && form[j].name.substr(0,14) == "gravacaocampo_") {
			if( form[j].name.substr(0,6) == "cursos" ){
				alert(form[j].name);
			}
			form[j].onkeyup();
		}
	}
}
aplicarmascara();
</script>