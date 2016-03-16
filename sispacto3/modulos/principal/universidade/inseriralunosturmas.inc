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
<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>

<script>

function inserirAlunoTurma(iusd, obj) {
	divCarregando();
	if(obj.checked) {
		
		ajaxatualizar('requisicao=inserirAlunoTurma&plpabreviacao=<?=$turma['plpabreviacaosub'] ?>&uncid=<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>&turid=<?=$turma['turid'] ?>&iusd='+iusd,'');

		var linha = obj.parentNode.parentNode;
		linha.cells[6].innerHTML = "<?=$turma['turdesc'] ?>";
		
		
	} else {
		var conf = confirm("Deseja realmente excluir o aluno da Turma?");
		if(conf) {

			ajaxatualizar('requisicao=excluirAlunoTurma&plpabreviacao=<?=$turma['plpabreviacaosub'] ?>&iusd='+iusd,'');
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
	<td class="SubTituloCentro" colspan="2">Inserir Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita">Perfil</td>
	<td><?
	$sql = "SELECT pflcod as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflcod IN(".PFL_FORMADORIESP.",".PFL_FORMADORIES.",".PFL_COORDENADORLOCAL.",".PFL_ORIENTADORESTUDO.") ORDER BY pfldsc";
	$db->monta_combo('pflcod', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'pflcod', '', $_REQUEST['pflcod']); 
	?></td>
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
	<td class="SubTituloCentro" colspan="2"><input type="button" name="filtrar" value="Filtrar" onclick="filtrarAlunos();"> <input type="button" name="vertodos" value="Ver todos" onclick="window.location='sispacto3.php?modulo=principal/universidade/inseriralunosturmas&acao=A&turid=<?=$_REQUEST['turid'] ?>';"></td>
</tr>
<tr>
	<td colspan="2"><?
	
	if($_REQUEST['estuf_endereco']) {
		$f[] = "m.estuf='".$_REQUEST['estuf_endereco']."'";
	}
	
	if($_REQUEST['muncod_endereco']) {
		$f[] = "m.muncod='".$_REQUEST['muncod_endereco']."'";
	}
	
	if($_REQUEST['pflcod']) {
		$f[] = "t.pflcod='".$_REQUEST['pflcod']."'";
	}
	
	$sql = "SELECT '<input type=\"checkbox\" name=\"iusd[]\" value=\"'||i.iusd||'\" onclick=\"inserirAlunoTurma('||i.iusd||', this);\" '|| CASE WHEN tu.turid IS NOT NULL THEN 'checked' ELSE '' END ||'>' as acao, 
				   replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   m.mundescricao, 
				   CASE WHEN p.muncod IS NOT NULL THEN 'Municipal'
						WHEN p.estuf IS NOT NULL THEN 'Estadual' 
						ELSE 'Equipe IES' END || ' ( '||per.pfldsc||' )' as esfera,  
				   COALESCE(tu.turdesc,'<span style=background-color:red;>&nbsp;&nbsp;</span>') as turdesc  
				FROM sispacto3.identificacaousuario i
				LEFT JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid
				LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
				INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod
				LEFT JOIN sispacto3.".$turma['plpabreviacaosub']."turma ot ON ot.iusd = i.iusd
				LEFT JOIN sispacto3.turmas tu ON tu.turid = ot.turid
				WHERE t.pflcod IN(".$turma['pflcodsub'].") AND i.uncid='".$turma['uncid']."' ".(($f)?"AND ".implode(" AND ",$f):"")." ORDER BY i.iusnome";


	
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Município","Esfera","Turma");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%',$par2,true,false,false,true);
	
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="fechar" value="Ok" onclick="window.close();"></td>
</tr>
</table>
</form>
