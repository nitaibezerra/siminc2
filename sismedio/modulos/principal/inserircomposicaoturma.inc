<?
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid']));
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>

<script>

function inserirAlunoTurma(iusd, obj) {
	divCarregando();
	if(obj.checked) {
	
		ajaxatualizar('requisicao=inserirAlunoTurma&uncid=<?=$_SESSION['sismedio']['uncid'] ?>&turid=<?=$turma['turid'] ?>&iusd='+iusd,'');
		var linha = obj.parentNode.parentNode;
		linha.cells[5].innerHTML = '<?=$turma['turdesc'] ?>';
		
	} else {
		
		var conf = confirm("Deseja realmente excluir o aluno da Turma?");
		if(conf) {
			ajaxatualizar('requisicao=excluirAlunoTurma&iusd='+iusd,'');
			var linha = obj.parentNode.parentNode;
			linha.cells[5].innerHTML = "";

		} else {
			obj.checked=true;
		}
		
	}
	window.opener.carregarAlunosTurma();
	divCarregado();
}

function filtrarAlunos() {
	document.getElementById('formulario').submit();
}

</script>

<? 
if($_REQUEST['aba']=='fr') {
	$perfil_equipe = "t.pflcod=".PFL_ORIENTADORESTUDO."";
	$label_tit = "Inserir Orientadores de Estudo";
}

if($_REQUEST['aba']=='oe') {
	$perfil_equipe = "t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.")";
	$label_tit = "Inserir Professores e Coordenadores Pedagógicos";
}


?>

<form method="post" id="formulario" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2"><?=$label_tit ?></td>
</tr>
<tr>
	<td colspan="2"><?
	
	$sql = "SELECT '<input type=\"checkbox\" name=\"iusd[]\" value=\"'||i.iusd||'\" onclick=\"inserirAlunoTurma('||i.iusd||', this);\" '|| CASE WHEN tu.turid IS NOT NULL THEN 'checked' ELSE '' END ||'>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, l.lemnomeescola, COALESCE('<span style=font-size:xx-small;>'||tu.turdesc||'</span>','<span style=background-color:red;>&nbsp;&nbsp;</span>') as turdesc  
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sismedio.listaescolasensinomedio l ON l.lemcodigoinep::bigint = i.iuscodigoinep 
			LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			LEFT JOIN sismedio.turmas tu ON tu.turid = ot.turid 
			WHERE {$perfil_equipe} AND i.uncid IN(SELECT uncid FROM sismedio.turmas WHERE turid='".$turma['turid']."') ".(($f)?"AND ".implode(" AND ",$f):"")." ORDER BY l.lemnomeescola, i.iusnome";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Escola","Turma");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%',$par2,true,false,false,true);
	
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="fechar" value="Ok" onclick="window.close();"></td>
</tr>
</table>
</form>
