<?
/*
 * TELA utilizada para inserir linhas dinâmicas com opções pré-definidas
 * 
 * Variaveis obrigatórias passadas por $_REQUEST
 * tabtid - id da tabela
 * ano - ano referente
 * gitid - id do grupo item
 * opccod - código da opção (somente quando selecionada a busca por código)
 * letraselecionada - letra do alfabeto (somente quando escolhida a letra do alfabeto)
 */

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.opener.location='?modulo=inicio&acao=A';
			window.close();
		  </script>";
	exit;
}

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirlinhadinop'=>true,
									 'removerlinhadinop'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


/* Executar as requisições solicitadas ao BANCO DE DADOS */
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}
?>
<html>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script>
function crtopcoes(chk) {
	if(chk.checked) {
		ajaxatualizar('requisicao=inserirlinhadinop&opcid='+chk.value+'&gitid=<? echo $_REQUEST['gitid']; ?>&ano=<? echo $_REQUEST['ano']; ?><? echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>','');
		window.opener.location='?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $_REQUEST['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; ?><? echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>'
	} else {
		var conf = confirm("Esta ação irá apagar todos os dados referentes a esta linha. Deseja continuar ?");
		if(conf) {
			ajaxatualizar('requisicao=removerlinhadinop&opcid='+chk.value+'&gitid=<? echo $_REQUEST['gitid']; ?>&ano=<? echo $_REQUEST['ano']; ?>','teste');
			window.opener.location='?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $_REQUEST['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; ?><? echo (($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:""); ?>'
		} else {
			chk.checked = true;
		}
	}
}

// Seta-se no atributo "notclose" o obj instanciado "this", a fim de que não se feche a popup
this.opener.notclose = this;

</script>
<?
/* Array contendo todas as letras do alfabeto */
$alfa = $db->carregar("SELECT DISTINCT(SUBSTR(UPPER(opcdsc),1,1)) as alfa FROM rehuf.opcoes ORDER BY alfa");
if($alfa[0]) {
	foreach($alfa as $alf) {
		$alfabeto[] = $alf['alfa'];
	}
}
/* Caso não tenha nenhuma letra escolhida, por default pegar a primeira do array */
if(!$_REQUEST['letraselecionada']) {
	$_REQUEST['letraselecionada'] = 'A';
}
/* Imprimindo o layout de escolha das opções (navegação por alfabeto) */
echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
echo "<tr>";
echo "<td colspan='".count($alfabeto)."' class='SubTituloCentro'>Clique na letra do alfabeto referente ao nome da opção</td>";
echo "</tr>";
/* Listando as letras do alfabeto */
echo "<tr>";
foreach($alfabeto as $letra) {
	echo "<td style=\"cursor:pointer;\" onclick=\"window.location='?modulo=principal/inserirlinhaop&acao=A&tabtid=".$_REQUEST['tabtid']."&ano=".$_REQUEST['ano']."&gitid=".$_REQUEST['gitid']."&letraselecionada=".$letra."".(($_REQUEST['perid'])?"&perid=".$_REQUEST['perid']:"")."';\">".(($letra==$_REQUEST['letraselecionada'])?"<b>".$letra."</b>":$letra)."</td>";
}
echo "</tr>";
// Verificando se o grupo item possui busca por código de busca
$gpopossuicod = $db->pegaUm("SELECT gpo.gpopossuicod FROM rehuf.grupoitem git 
							 LEFT JOIN rehuf.grupoopcoes gpo ON git.gpoid = gpo.gpoid 
							 WHERE git.gitid = '".$_REQUEST['gitid']."'");
// Caso grupo de opções possa ser buscado pelo código, imprimir um campo de busca para o código (ficara busca por alfabeto + código)...
if($gpopossuicod == "t") {	
	$opccod = $_REQUEST['opccod'];
	echo "<tr>";
	echo "<td colspan='".count($alfabeto)."'>
			 <form name='formulario' method='post' action='?modulo=principal/inserirlinhaop&acao=A'>
			 <input type='hidden' name='tipobusca' value='buscacodigo'>
			 <input type='hidden' name='ano' value='".$_REQUEST['ano']."'>
			 <input type='hidden' name='gitid' value='".$_REQUEST['gitid']."'>
			 <table cellSpacing=\"0\" cellPadding=\"0\" border=\"0\">
			 	<tr>
		 		<td>Busca por código :</td>
				<td>". campo_texto('opccod', "N", "S", "Código", 30, 50, "", "", '', '', 0, '' ) ." <input type='button' value='Buscar' onclick='document.formulario.submit();'></td>
			 	</tr>
			 </table>
			 </form>
		  </td>";
	echo "</tr>";
	// Por código, imprimir a coluan código na lista
	$cabecalho = array("","Descrição","Código");
	$campoespecifico = ", opc.opccod";
} else {
	$cabecalho = array("","Descrição");
}
echo "</table>";
// Forma de buscar os dados filtrados 
if($_REQUEST['tipobusca'] == 'buscacodigo') {
	// Caso a busca tenha sido pelo pelo código
	$sql = "SELECT '<input type=\"checkbox\" onclick=\"crtopcoes(this);\" value=\"'|| opc.opcid ||'\" '|| CASE WHEN (SELECT count(*) FROM rehuf.linha lin WHERE lin.gitid='".$_REQUEST['gitid']."' AND lin.esuid='".$_SESSION['rehuf_var']['esuid']."' AND lin.opcid = opc.opcid AND (lin.linano='".$_REQUEST['ano']."' OR lin.linano IS NULL) ".(($_REQUEST['perid'])?"AND perid='".$_REQUEST['perid']."'":"").") =0 THEN '' ELSE 'checked' END ||'>' as acao, opc.opcdsc ".$campoespecifico." FROM rehuf.grupoitem git
			LEFT JOIN rehuf.opcoes opc ON opc.gpoid = git.gpoid  
			WHERE git.gitid = '".$_REQUEST['gitid']."' AND 
				  opc.opccod = '".$_REQUEST['opccod']."' 
			ORDER BY opc.opcdsc";
} else {
	// Caso tenha sido pelo alfabeto
	$sql = "SELECT '<input type=\"checkbox\" onclick=\"crtopcoes(this);\" value=\"'|| opc.opcid ||'\" '|| CASE WHEN (SELECT count(*) FROM rehuf.linha lin WHERE lin.gitid='".$_REQUEST['gitid']."' AND lin.esuid='".$_SESSION['rehuf_var']['esuid']."' AND lin.opcid = opc.opcid AND (lin.linano='".$_REQUEST['ano']."' OR lin.linano IS NULL) ".(($_REQUEST['perid'])?"AND perid='".$_REQUEST['perid']."'":"").") =0 THEN '' ELSE 'checked' END ||'>' as acao, opc.opcdsc ".$campoespecifico." FROM rehuf.grupoitem git
			LEFT JOIN rehuf.opcoes opc ON opc.gpoid = git.gpoid  
			WHERE git.gitid = '".$_REQUEST['gitid']."' AND 
				  (opc.opcdsc LIKE '". $_REQUEST['letraselecionada'] ."%' OR
			   	   opc.opcdsc LIKE '". strtolower($_REQUEST['letraselecionada']) ."%') 
			ORDER BY opc.opcdsc";	
}
// Montando a lista
$db->monta_lista_simples($sql,$cabecalho,200,5,'N','95%',$par2);
?>
<div id="teste"></div>
</html>