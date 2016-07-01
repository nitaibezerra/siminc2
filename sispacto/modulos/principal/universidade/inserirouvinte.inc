<?

include "_funcoes_universidade.php";

if(!$_SESSION['sispacto']['universidade']['uncid']) {
	$al = array("alert"=>"Problemas na navegação, por favor acesse novamente.","javascript"=>"window.opener.location='sispacto.php?modulo=inicio&acao=C';window.close();");
	alertlocation($al);
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>

<script>
function carregaUsuario() {
	divCarregando();
	var usucpf=document.getElementById('fiocpf').value;
	usucpf = usucpf.replace('-','');
	usucpf = usucpf.replace('.','');
	usucpf = usucpf.replace('.','');
				
	var comp = new dCPF();
	comp.buscarDados(usucpf);
	var arrDados = new Object();
	
	if(!comp.dados.no_pessoa_rf){
		alert('CPF Inválido');
		divCarregado();
		return false;
	}
	
	document.getElementById('fionome').value=comp.dados.no_pessoa_rf;
	divCarregado();
}

function inserirOuvinte() {
	
	jQuery('#fiocpf').val(mascaraglobal('###.###.###-##',jQuery('#fiocpf').val()));
	
	if(jQuery('#fiocpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#fiocpf').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#fionome').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#fioemail').val()=='') {
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#fioemail').val())) {
    	alert('Email inválido');
    	return false;
    }
    
	if(jQuery('#fioesfera').val()=='') {
		alert('Esfera em branco');
		return false;
	}
	
	if(jQuery('#estuf_endereco').val()=='') {
		alert('UF em branco');
		return false;
	}
	
	if(jQuery('#muncod_endereco').val()=='') {
		alert('Município em branco');
		return false;
	}
	
    divCarregando();
    
	document.getElementById('formulario').submit();

}

function carregarMunicipiosPorUF_ouvinte(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_endereco&name=muncod_endereco&estuf='+estuf,'td_municipio3');
	} else {
		document.getElementById('td_municipio3').innerHTML = "Selecione uma UF";
	}
}

function carregarUsuarioTroca() {

	if(jQuery('#fioesfera').val()=='') {
		alert('Esfera em branco');
	}
	
	ajaxatualizar('requisicao=carregarUsuariosHabiliatadosTroca&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>&esfera='+jQuery('#fioesfera').val()+'&muncod='+jQuery('#muncod_endereco').val()+'&estuf='+jQuery('#estuf_endereco').val(),'ouvintesub');

}

</script>
<?
if($_REQUEST['fioid']) {
	$sql = "SELECT fioid, fiocpf, fionome, fioemail, fioesfera, estuf as estuf_endereco, muncod as muncod_endereco, iusd, uncid FROM sispacto.formacaoinicialouvintes WHERE fioid='".$_REQUEST['fioid']."'";
	$formacaoinicialouvintes = $db->pegaLinha($sql);
	extract($formacaoinicialouvintes);
	$requisicao="atualizarOuvinte";
} else {
	$requisicao="inserirOuvinte";
}
?>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
<input type="hidden" name="fioid" value="<?=$fioid ?>">
<input type="hidden" name="uncid" value="<?=$_SESSION['sispacto']['universidade']['uncid'] ?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Orientador Substituto</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">CPF</td>
	<td><?=campo_texto('fiocpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="fiocpf"', '', mascaraglobal($fiocpf,"###.###.###-##"), 'if(this.value!=\'\'){carregaUsuario();}'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome</td>
	<td><?=campo_texto('fionome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="fionome"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Email</td>
	<td><?=campo_texto('fioemail', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="fioemail"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Esfera:</td>
	<td><?
	$arr = array(0=>array("codigo"=>"M","descricao"=>"Municipal"),1=>array("codigo"=>"E","descricao"=>"Estadual"));
	$db->monta_combo('fioesfera', $arr, 'S', 'Selecione', '', '', '', '', 'S', 'fioesfera', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF_ouvinte', '', '', '', 'S', 'estuf_endereco', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Município</td>
	<td id="td_municipio3">
	<? 
	if($muncod_endereco) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf_endereco."' ORDER BY mundescricao";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '');
	else :
		echo "Selecione UF";
	endif;
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" onclick="inserirOuvinte();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
</table>
</form>
