<?php 
include 'config.inc';
include APPRAIZ . 'includes/funcoes.inc';
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'www/par/autoload.php';
include APPRAIZ . 'www/par/_constantes.php';
$db =  new cls_banco();

if( !$_GET['modelo'] ){
	echo "<script>alert('Falta o Modelo!');history.back(-1);</script>";
	exit;
}

$sql = "SELECT UPPER(poddescricao) AS poddescricao, podmodelo FROM obras.pretipodocumento WHERE podid = {$_GET['modelo']}";
$txModelo = $db->pegaLinha($sql);

$preid = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

if( !$preid ){
	echo "<script>alert('Falta o preid!');history.back(-1);</script>";
	exit;
}

$oSubacaoControle = new SubacaoControle();
$classObra = $oSubacaoControle->verificaClassificacaoObra($preid, SIS_OBRAS);
$catObra = $oSubacaoControle->verificaCategoriaObra($preid);

switch( $classObra ){
	case 'P':
			$txModelo['podmodelo'] = str_replace("{categoria}", 'Escola', $txModelo['podmodelo']);
		break;
	case 'E':
			$txModelo['podmodelo'] = str_replace("{categoria}", 'Escola', $txModelo['podmodelo']);
		break;
	case 'C':
			$txModelo['podmodelo'] = str_replace("{categoria}", 'Cobertura', $txModelo['podmodelo']);
		break;
	case 'Q':
			$txModelo['podmodelo'] = str_replace("{categoria}", 'Quadra', $txModelo['podmodelo']);
		break;
}

if($catObra == 'E'){
	$txModelo['podmodelo'] = str_replace("{tipo}", 'Estaca', $txModelo['podmodelo']);
}else{
	$txModelo['podmodelo'] = str_replace("{tipo}", 'Sapata', $txModelo['podmodelo']);
}

// Cria objetos e arrays
$obPreObraControle 	= new PreObraControle();
if($_SESSION['par']['muncod']!='' && $_SESSION['par']['esfera'] == 'M'){
	$arDadosPrefeito 	= $obPreObraControle->recuperarPrefeitoMunicipio($_SESSION['par']['muncod']);
	$arDadosPrefeitura 	= $obPreObraControle->recuperarPrefeitoMunicipio($_SESSION['par']['muncod'], 1);
}else{
	$arDadosPrefeito 	= $obPreObraControle->recuperarSecretarioMunicipio($_SESSION['par']['estuf'], 25);
	$arDadosPrefeitura 	= $obPreObraControle->recuperarSecretarioMunicipio($_SESSION['par']['estuf'], 6);
}

// Recupera dados do prefeito para mesclar com documento
$arDadosPrefeito['entnome'] 		= $arDadosPrefeito['entnome'] 			? $arDadosPrefeito['entnome'] : $_POST['entnomeprefeito'];
$arDadosPrefeito['entnumcpfcnpj'] 	= $arDadosPrefeito['entnumcpfcnpj'] 	? $arDadosPrefeito['entnumcpfcnpj'] : $_POST['entnumcpfcnpjprefeito'];
$arDadosPrefeito['estuf'] 			= $arDadosPrefeito['estuf'] 			? $arDadosPrefeito['estuf'] : $_POST['estufprefeito'];
$arDadosPrefeito['estdescricao'] 	= $arDadosPrefeito['estdescricao']  	? $arDadosPrefeito['estdescricao'] : $_POST['estdescricaoprefeito'];
$arDadosPrefeito['mundescricao'] 	= $arDadosPrefeito['mundescricao']  	? $arDadosPrefeito['mundescricao'] : $_POST['mundescricaoprefeito'];
$arDadosPrefeito['entnumrg'] 		= $arDadosPrefeito['entnumrg'] 			? $arDadosPrefeito['entnumrg'] : $_POST['entnumrgprefeito'];
$arDadosPrefeito['endereco'] 		= $arDadosPrefeito['endlog'] 			? $arDadosPrefeito['endlog'].", ".$arDadosPrefeito['endbai'] : $_POST['endbaiprefeito'];

// Se não hover dados do prefeito inclui lacuna para preenchimento
$arDadosPrefeito['entnome'] 		= $arDadosPrefeito['entnome'] 			? $arDadosPrefeito['entnome'] : "______________________________________";
$arDadosPrefeito['entnumcpfcnpj'] 	= $arDadosPrefeito['entnumcpfcnpj'] 	? formatar_cpf($arDadosPrefeito['entnumcpfcnpj']) : "______________________________________";
$arDadosPrefeito['estuf'] 			= $arDadosPrefeito['estuf'] 			? $arDadosPrefeito['estuf'] : "______";
$arDadosPrefeito['estdescricao'] 	= $arDadosPrefeito['estdescricao']  	? $arDadosPrefeito['estdescricao'] : "______";
if($_SESSION['par']['muncod']!='' && $_SESSION['par']['esfera'] == 'M'){
	$arDadosPrefeito['mundescricao'] 	= $arDadosPrefeito['mundescricao']  	? $arDadosPrefeito['mundescricao'] : "______________________________________";
}else{
	$arDadosPrefeito['mundescricao'] 	= $arDadosPrefeitura['estdescricao']  	? $arDadosPrefeitura['estdescricao'] : "______________________________________";
}
$arDadosPrefeito['endereco'] 		= $arDadosPrefeito['endlog'] 			? $arDadosPrefeito['endlog'].", ".$arDadosPrefeito['endbai'] : "____________________________";
$arDadosPrefeito['entnumrg'] 		= $arDadosPrefeito['entnumrg'] 			? $arDadosPrefeito['entnumrg'] : "____________________________";
$arDadosPrefeito['natural'] 		= $_POST['naturalidade'] 				? $_POST['naturalidade'] : "_______________________________";
$arDadosPrefeito['estadocivil']		= $_POST['estadocivil'] 				? "estado civil ".$_POST['estadocivil'] : "estado civil _______________________________";

// Recupera dados da prefeitura para mesclar com documento
$arDadosPrefeitura['endereco'] 		= $arDadosPrefeitura['endlog'] 			? $arDadosPrefeitura['endlog'] : $_POST['endlogprefeitura'];
$arDadosPrefeitura['bairro'] 		= $arDadosPrefeitura['endbai'] 			? $arDadosPrefeitura['endbai'] : $_POST['endbaiprefeitura'];
$arDadosPrefeitura['entnumcpfcnpj'] = $arDadosPrefeitura['entnumcpfcnpj'] 	? formatar_cnpj($arDadosPrefeitura['entnumcpfcnpj']) : $_POST['entnumcpfcnpjprefeitura'];
$arDadosPrefeitura['sede'] 			= $arDadosPrefeitura['entsede'] 		? $arDadosPrefeitura['entsede'] : $_POST['entsedeprefeitura'];

// Se não hover dados da prefeitura inclui lacuna para preenchimento
$arDadosPrefeitura['endereco'] 		= $arDadosPrefeitura['endlog'] 			? $arDadosPrefeitura['endlog'] : "______________________";
$arDadosPrefeitura['bairro'] 		= $arDadosPrefeitura['endbai'] 			? $arDadosPrefeitura['endbai'] : "______________";
$arDadosPrefeitura['entnumcpfcnpj'] = $arDadosPrefeitura['entnumcpfcnpj'] 	? $arDadosPrefeitura['entnumcpfcnpj'] : "___________________";
$arDadosPrefeitura['sede'] 			= $arDadosPrefeitura['sede'] 			? $arDadosPrefeitura['sede'] : "______________________";

if($_POST['duncpf']){
	
	// Mescla dados do engenheiro
	$txModelo['podmodelo'] = str_replace("{nome}", $_POST['dunnome'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{cpf}", $_POST['duncpf'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{crea}", $_POST['crea'], $txModelo['podmodelo']);
	
	// Nome para assinatura
	$stNome = $_POST['dunnome'];
}else{
	
	// Mescla dados do prefeito
	$txModelo['podmodelo'] = str_replace("{nome}", $arDadosPrefeito['entnome'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{cpf}", $arDadosPrefeito['entnumcpfcnpj'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{crea}", $arDadosPrefeito['entcrea'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{uf}", $arDadosPrefeito['estdescricao'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{municipio}", $arDadosPrefeito['mundescricao'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{endereco}", $arDadosPrefeito['endereco'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{rg}", $arDadosPrefeito['entnumrg']." - ".$arDadosPrefeito['entorgaoexpedidor'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{natural}", $_POST['naturalidade'], $txModelo['podmodelo']);
	$txModelo['podmodelo'] = str_replace("{estadocivil}", $_POST['estadocivil'], $txModelo['podmodelo']);
	
	// Nome para assinatura
	$stNome = $arDadosPrefeito['entnome'];
}

// Mescla dados da prefeitura
$txModelo['podmodelo'] = str_replace("{cnpj}", $arDadosPrefeitura['entnumcpfcnpj'], $txModelo['podmodelo']);
$txModelo['podmodelo'] = str_replace("{sede}", $arDadosPrefeitura['sede'], $txModelo['podmodelo']);
?>
<script type="text/javascript">
<!--
	function imprimir(){
		print();
	}

	function PrintElementID(id, pg) {
	    var oPrint, oJan;
	    oPrint  = window.document.getElementById(id).innerHTML;
	    oJan    = window.open(pg);
	    oJan.document.write(oPrint);
	    oJan.history.go();
	    oJan.window.print();

	    fechar();
	    
	}

	function fechar(){
		window.close();
	}

//-->
</script>
<div style="padding:10px;border:1px solid black;" align="center">
	<span style="color:red;">
		<b>Este documento deverá ser impresso assinado digitalizado e anexado.</b>
	</span>
</div>
<?php if($_SESSION['par']['muncod'] && $_SESSION['par']['esfera'] == 'M'){?>
<div style="padding:10px;" align="center" id="conteudoParaImpressao">
	<center>
		<p>
		<img width="120" src="../../imagens/brasao.JPG" /><br />
		PREFEITURA DE <?php echo strtoupper($arDadosPrefeito['mundescricao']) ?><br />
		<?php echo $arDadosPrefeitura['endereco'].", ".$arDadosPrefeitura['bairro']." - ".$arDadosPrefeito['mundescricao']."-".$arDadosPrefeito['estuf'] ?><br />
		</p>
		<b><?php echo strtoupper($txModelo['poddescricao']) ?></b>
	</center>
	<p style="text-align:justify">
		<?php echo $txModelo['podmodelo'] ?>
	</p>
<?php }else{ ?>
<div style="padding:10px;" align="center" id="conteudoParaImpressao">
	<center>
		<p>
		<img width="120" src="../../imagens/brasao.JPG" /><br />
		SECRETARIA ESTADUAL DE EDUCAÇÃO - <?php echo strtoupper($arDadosPrefeito['mundescricao']) ?><br />
		<?php echo $arDadosPrefeitura['endereco'].", ".$arDadosPrefeitura['bairro']." - ".$arDadosPrefeito['mundescricao']."-".$arDadosPrefeito['estuf'] ?><br />
		</p>
		<b><?php echo strtoupper($txModelo['poddescricao']) ?></b>
	</center>
	<p style="text-align:justify">
		<?php echo $txModelo['podmodelo'] ?>
	</p>	
<?php }?>
	<center>
	<br/><br/><br/>	
	____________________________________________<br/>
	<?php echo $stNome ?>
	</center>
</div>
<center>
	<a href="javascript:void(0)" onclick="PrintElementID('conteudoParaImpressao')">Imprimir</a> |
	<a href="javascript:fechar()">Fechar</a>  
</center>
