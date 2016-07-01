<?
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."includes/funcoes_espelhoperfil.php";

include "../_constantes.php";
include "../_funcoes.php";
include "../_funcoes_coordenadorlocal.php";


$db = new cls_banco();

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

function inserirCoordenadorLocalResp($dados) {
	global $db;
	
	$sql = "SELECT * FROM seguranca.usuario u 
			LEFT JOIN sispacto3.identificacaousuario iu ON iu.iuscpf = u.usucpf 
			WHERE usucpf='".$dados['usucpf']."'";
	$us = $db->pegaLinha($sql);
	
	if($us['iusd']) {
		$arr = array("iusd"=>$us['iusd'],"pflcod"=>PFL_ORIENTADORESTUDO,"naoredirecionar"=>true);
		removerTipoPerfil($arr);
		$arr = array("iusd"=>$us['iusd'],"pflcod"=>PFL_COORDENADORLOCAL,"naoredirecionar"=>true);
		removerTipoPerfil($arr);
		
	}
	
	if($_REQUEST['iusdantigo']) {
		$arr1 = array("iusd"=>$dados['iusdantigo'],"pflcod"=>PFL_COORDENADORLOCAL,"naoredirecionar"=>true);
		removerTipoPerfil($arr1);
	}
	
	$arr2 = array("picid"=>$dados['picid'],"iuscpf"=>$us['usucpf'],"iusnome"=>$us['usunome'],"iusemailprincipal"=>$us['usuemail'],"naoredirecionar"=>true);
	inserirCoordenadorLocalGerenciamento($arr2);

	$al = array("alert"=>"Coordenador Local inserido com sucesso","location"=>"cadastro_responsabilidade_coordenadorlocal.php?pflcod=".$dados['pflcod']."&usucpf=".$dados['usucpf']);
	alertlocation($al);

}


$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Coordenador Local</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css'
	href='../../includes/listagem.css'>

</head>

<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0"	MARGINHEIGHT="0" BGCOLOR="#ffffff">
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../js/sispacto.js"></script>
<?
$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_EQUIPEMEC,$perfis)) {
	$menu[] = array("id" => 1, "descricao" => "Municipial", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Municipal");
	$menu[] = array("id" => 2, "descricao" => "Estadual", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Estadual");
} else {
	$sql = "SELECT muncod, estuf FROM sispacto3.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$usuarioresponsabilidade = $db->carregar($sql);
	$menu = array();
	if($usuarioresponsabilidade[0]) {
		foreach($usuarioresponsabilidade as $ur) {
			if($ur['muncod']) {
				$in_muncod[] = $ur['muncod']; 
			} elseif($ur['estuf']) {
				$in_estuf[] = $ur['estuf'];
			}
		}
		if($in_muncod) $menu[] = array("id" => 1, "descricao" => "Municipial", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Municipal");
		if($in_estuf) $menu[] = array("id" => 2, "descricao" => "Estadual", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Estadual");
	}
}



if(!$_REQUEST['esfera']) $_REQUEST['esfera'] = "Municipal";


monta_titulo( "Lista - Coordenador Local", "Lista ".$_REQUEST['esfera']." de Coordenadores Locais participantes");

?>
<script>
function selecionarPacto(obj,iusdantigo) {
	var conf = confirm('Você esta adicionado um Coordenador Local. Confirmando esta ação você estará:\n\n - Caso exista Coordenador Local cadastrado, este será removido\n - Caso este CPF(<?=$_REQUEST['usucpf'] ?>) esteja cadastrado como Orientador de Estudo, este será removido do perfil\n\n Deseja realmente continuar?');
	
	if(conf) {
		window.location='cadastro_responsabilidade_coordenadorlocal.php?requisicao=inserirCoordenadorLocalResp&iusdantigo='+iusdantigo+'&pflcod=<?=$_REQUEST['pflcod'] ?>&usucpf=<?=$_REQUEST['usucpf'] ?>&picid='+obj.value;
	} else {
		obj.checked=false;
	}
}
</script>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Esfera</td>
	<td><input type="radio" name="esfera" value="Municipal" <?=(($_REQUEST['esfera']=="Municipal")?"checked":"") ?> onclick="if(this.checked){divCarregando();window.location='cadastro_responsabilidade_coordenadorlocal.php?pflcod=<?=$_REQUEST['pflcod'] ?>&usucpf=<?=$_REQUEST['usucpf'] ?>&esfera=Municipal';}"> Municipal <input type="radio" name="esfera" value="Estadual"  <?=(($_REQUEST['esfera']=="Estadual")?"checked":"") ?>  onclick="if(this.checked){divCarregando();window.location='cadastro_responsabilidade_coordenadorlocal.php?pflcod=<?=$_REQUEST['pflcod'] ?>&usucpf=<?=$_REQUEST['usucpf'] ?>&esfera=Estadual';}"> Estadual</td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', (($_REQUEST['esfera'] == "Municipal")?'carregarMunicipiosPorUF3':''), '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<? if($_REQUEST['esfera'] == "Municipal") : ?>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<? 
	if($_REQUEST['uf']) :
		if(!isset($_REQUEST['muncod_endereco'])) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'];
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['uf']."' ORDER BY mundescricao"; 
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	else: 
		echo "Selecione uma UF";
	endif; ?>
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='cadastro_responsabilidade_coordenadorlocal.php?pflcod=<?=$_REQUEST['pflcod'] ?>&usucpf=<?=$_REQUEST['usucpf'] ?>';"></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">
	<font size=1>
	<?=$db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".$_REQUEST['usucpf']."'") ?> <br/> 
	<? 
	$descricao = $db->pegaUm("SELECT CASE WHEN m.muncod IS NOT NULL THEN m.estuf || ' / ' || m.mundescricao 
										  WHEN e.estuf  IS NOT NULL THEN e.estuf || ' / ' || e.estdescricao END as descricao FROM sispacto.identificacaousuario i 
				 				 INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd 
				 				 INNER JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid  
				 				 LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
				 				 LEFT JOIN territorios.estado e ON e.estuf = p.estuf
				 			     WHERE i.iuscpf='".$_REQUEST['usucpf']."' AND t.pflcod=".PFL_COORDENADORLOCAL.""); 
	echo (($descricao)?"<i>Coordenador Local vinculado: ".$descricao."</i>":"<i>Coordenador Local não vinculado</i>");
	?>
	</font>
	</td>
</tr>
</table>
</form>
<?

if($_REQUEST['uf']) {
	$f[] = "foo.estuf='".$_REQUEST['uf']."'";
}
if($_REQUEST['muncod_endereco']) {
	$f[] = "foo.muncod='".$_REQUEST['muncod_endereco']."'";
}
if($_REQUEST['esdid']) {
	$f[] = "foo.esdid='".$_REQUEST['esdid']."'";
}

if($_REQUEST['esfera'] == "Estadual") {
	$inn = "INNER JOIN territorios.estado m ON m.estuf = p.estuf".(($in_estuf)?" AND p.estuf IN('".implode("','",$in_estuf)."')":"");
	$col = "'<input type=\"radio\" name=\"picid\" onclick=\"selecionarPacto(this,\''||COALESCE((SELECT i.iusd::text FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'')||'\');\" value=\"'||p.picid||'\">' as acao, m.estuf as estado, m.estdescricao as descricao,";
} else {
	$inn = "INNER JOIN territorios.municipio m ON m.muncod = p.muncod".(($in_muncod)?" AND p.muncod IN('".implode("','",$in_muncod)."')":"");;
	$col = "'<input type=\"radio\" name=\"picid\" onclick=\"selecionarPacto(this,\''||COALESCE((SELECT i.iusd::text FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'')||'\');\" value=\"'||p.picid||'\">' as acao, m.estuf as estado, m.mundescricao as descricao,";
}


$sql = "SELECT foo.acao, foo.estado, foo.descricao, foo.coordenadorlocal FROM (
		SELECT {$col} COALESCE((SELECT iusnome FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'Coordenador Local não cadastrado') as coordenadorlocal,
		COALESCE(e.esddsc,'Não iniciou Elaboração') as situacao,
		p.picstatus,
		m.estuf,
		p.muncod,
		e.esdid
		FROM sispacto3.pactoidadecerta p 
		{$inn} 
		LEFT JOIN workflow.documento d ON d.docid = p.docid  
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
		WHERE foo.picstatus='A'".(($f)?" AND ".implode(" AND ",$f):"")." ORDER BY foo.estuf, foo.descricao";

$cabecalho = array("&nbsp;","UF","Descrição","Coordenador Local");
$db->monta_lista($sql,$cabecalho,20,10,'N','center',$par2);
?>
</body>
