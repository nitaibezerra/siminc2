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
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>

<script>

function inserirAlunoTurma(iusd, obj) {
	divCarregando();
	if(obj.checked) {
		var nummax = false;
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: 'requisicao=numeroAlunosTurma&turid=<?=$turma['turid'] ?>',
	   		async: false,
	   		success: function(num){
				if(num>33) {
					nummax = true;
				}
	   		}
		});
		
		if(nummax) {
			alert('Esta turma possui mais de 34 Orientadores vinculados.');
			obj.checked=false;
			divCarregado();
			return false;
		}
	
		ajaxatualizar('requisicao=inserirAlunoTurma&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>&turid=<?=$turma['turid'] ?>&iusd='+iusd,'');
		var linha = obj.parentNode.parentNode;
		linha.cells[6].innerHTML = "<?=$turma['turdesc'] ?>";
	} else {
		var conf = confirm("Deseja realmente excluir o aluno da Turma?");
		if(conf) {
			ajaxatualizar('requisicao=excluirAlunoTurma&iusd='+iusd,'');
			var linha = obj.parentNode.parentNode;
			linha.cells[6].innerHTML = "";

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

<form method="post" id="formulario" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Orientadores de Estudo</td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '', $_REQUEST['estuf_endereco']); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<? 
	if($_REQUEST['estuf_endereco']) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['estuf_endereco']."' ORDER BY mundescricao";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	else :
		echo "Selecione UF";
	endif;
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="filtrar" value="Filtrar" onclick="filtrarAlunos();"> <input type="button" name="vertodos" value="Ver todos" onclick="window.location='sispacto.php?modulo=principal/universidade/inseriralunosturmas&acao=A&turid=<?=$_REQUEST['turid'] ?>';"></td>
</tr>
<tr>
	<td colspan="2"><?
	
	if($_REQUEST['estuf_endereco']) {
		$f[] = "m.estuf='".$_REQUEST['estuf_endereco']."'";
	}
	
	if($_REQUEST['muncod_endereco']) {
		$f[] = "m.muncod='".$_REQUEST['muncod_endereco']."'";
	}
	
	$sql = "(SELECT '<input type=\"checkbox\" name=\"iusd[]\" value=\"'||i.iusd||'\" onclick=\"inserirAlunoTurma('||i.iusd||', this);\" '|| CASE WHEN tu.turid IS NOT NULL THEN 'checked' ELSE '' END ||'>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, m.mundescricao, 'Municipal' as esfera,  COALESCE(tu.turdesc,'<span style=background-color:red;>&nbsp;&nbsp;</span>') as turdesc  FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			LEFT JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			LEFT JOIN sispacto.turmas tu ON tu.turid = ot.turid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid IN(SELECT uncid FROM sispacto.turmas WHERE turid='".$turma['turid']."') AND a.esfera='M' ".(($f)?"AND ".implode(" AND ",$f):"")." ORDER BY i.iusnome) 
			UNION ALL (
			SELECT '<input type=\"checkbox\" name=\"iusd[]\" value=\"'||i.iusd||'\" onclick=\"inserirAlunoTurma('||i.iusd||', this);\" '|| CASE WHEN tu.turid IS NOT NULL THEN 'checked' ELSE '' END ||'>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, m.mundescricao, 'Estadual' as esfera, COALESCE(tu.turdesc,'<span style=background-color:red;>&nbsp;&nbsp;</span>') as turdesc FROM sispacto.identificacaousuario i 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			INNER JOIN sispacto.pactoidadecerta p ON p.estuf = m.estuf AND p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto.abrangencia a ON m.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			LEFT JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			LEFT JOIN sispacto.turmas tu ON tu.turid = ot.turid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid IN(SELECT uncid FROM sispacto.turmas WHERE turid='".$turma['turid']."') AND a.esfera='E' ".(($f)?"AND ".implode(" AND ",$f):"")." ORDER BY i.iusnome)";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Município","Esfera","Turma");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%',$par2,true,false,false,true);
	
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="fechar" value="Ok" onclick="window.close();"></td>
</tr>
</table>
</form>
