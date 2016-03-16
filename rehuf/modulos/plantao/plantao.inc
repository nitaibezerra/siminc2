<?
/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('gridPlantao'=>true,
									 'inserirLinhaPlantao'=>true,
									 'selecionarFuncionarioPlantao'=>true,
									 'inserirEscalaPlantao'=>true,
									 'selecionarLinhaPlantaoSIAPE'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=C';
		  </script>";
	exit;
}

echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */
/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
/* fim montando cabeçalho */

?>
<form method='post' name='form'>
<input type='hidden' name='requisicao' value='inserirEscalaPlantao'>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro" colspan="4">Filtros</td>
	<td width=10% align="right" onmouseover="return escape('<fieldset style=width:220px;text-align:left;><legend>Legenda</legend>PN - Plantão normal<br />PD - Plantão presencial dias úteis<br />PF - Plantão presencial final de semana e feriados<br />SD - Plantão sobreaviso dias úteis<br />SF - Plantão sobreaviso final de semana e feriados</fieldset>');"><b>Legenda</b></td>
</tr>
<tr>
	<td class="SubTituloDireita">Período:</td>
	<td>
	<?
	if($_REQUEST['periodo']) $periodo=$_REQUEST['periodo'];
	
	if($db->testa_superuser()) {
		
		$sql = "SELECT ppldata as codigo, to_char(ppldata, 'mm/YYYY') as descricao 
				FROM rehuf.periodoplantao p WHERE p.pplstatus='A' ORDER BY ppldata"; 
		
	} else {
		$sql = "SELECT ppldata as codigo, to_char(ppldata, 'mm/YYYY') as descricao 
				FROM rehuf.periodoplantao p 
				LEFT JOIN rehuf.periodoplantaodata pd ON p.pplid = pd.pplid
				WHERE pd.pplpreenchimentoinicial <= cast(NOW() as date) AND 
					  pd.pplpreenchimentofinal >= cast(NOW() as date) AND 
					  p.pplstatus='A' AND 
					  pd.entid='".$_SESSION['rehuf_var']['entid']."'";
	}
	$db->monta_combo('periodo', $sql, 'S', 'Selecione', 'atualizarGridPlantao', '', '', '', 'S', 'periodo');
	?>
	</td>
	<td class="SubTituloDireita">Setor:</td>
	<td>
	<?
	$sql = "SELECT sep.setid as codigo, sep.setnome as descricao FROM rehuf.setorplantao sep 
			INNER JOIN rehuf.setorhospitalplantao shp ON shp.setid=sep.setid 
			WHERE setstatus='A' AND shp.entid='".$_SESSION['rehuf_var']['entid']."'";
	$db->monta_combo('setid', $sql, 'S', 'Selecione', 'atualizarGridPlantao', '', '', '', 'S', 'setid');
	?>
	</td>
	<td width=10%>
	<input type="button" name="btn" value="Cadastrar Funcionário" onclick="window.location='?modulo=plantao/cadastrofuncionarios&acao=A';">
	<input type="button" name="btn" value="Importar Plantão" onclick="abrirImportacao();">
	</td>
</tr>
</table>
<div id="gridPlantao" style="overflow:auto;position:relative;"></div>
</form>

<script>

function abrirImportacao() {
	window.open('rehuf.php?modulo=plantao/importarplantao&acao=A','Observações','scrollbars=no,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
}

function redimencionarBodyData() {
	var myWidth;
	var myHeight;
	if( typeof( window.innerWidth ) == 'number' ) {
	   	//Non-IE
	   	myWidth = window.innerWidth;
	   	myHeight = window.innerHeight;
		document.getElementById('gridPlantao').style.left = '50%';
		document.getElementById('gridPlantao').style.marginLeft = Arredonda((-1*myWidth*0.95)/2,0);
		document.getElementById('gridPlantao').style.width = Arredonda(myWidth*0.95,0);
		document.getElementById('gridPlantao').style.height = (myHeight-335);
	} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
	   	//IE 6+ in 'standards compliant mode'
	   	myWidth = document.documentElement.clientWidth;
	   	myHeight = document.documentElement.clientHeight;
		document.getElementById('gridPlantao').style.marginLeft = Arredonda((-1*myWidth*0.95)/2,0);
		document.getElementById('gridPlantao').style.width = Arredonda(myWidth*0.95,0);
		document.getElementById('gridPlantao').style.height = (myHeight-775);
	} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
	   	//IE 4 compatible
	   	myWidth = document.body.clientWidth;
	   	myHeight = document.body.clientHeight;
		document.getElementById('gridPlantao').style.marginLeft = Arredonda((myWidth*0.025),0);
		document.getElementById('gridPlantao').style.width = Arredonda(myWidth*0.949,0);
		document.getElementById('gridPlantao').style.height = (myHeight-330);
	
	}
}
<?
if($_REQUEST['setid']) {
	echo "document.getElementById('setid').value='".$_REQUEST['setid']."';";
	echo "document.getElementById('setid').onchange();";	
}
?>
</script>