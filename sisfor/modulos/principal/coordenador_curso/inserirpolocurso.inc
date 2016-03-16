<?

function carregarFiltrosMunicipios($dados) {
	global $db;
	if($dados['estuf']) {
		$sql = "SELECT m.muncod as codigo, m.estuf||' - '||REPLACE(m.mundescricao,'\'',' ') as descricao
				FROM territorios.municipio m
				WHERE m.estuf='".$dados['estuf']."' AND m.muncod NOT IN(SELECT muncod FROM sisfor.abrangenciacurso WHERE esfera='".$dados['esfera']."' AND sifid='".$dados['sifid']."') ORDER BY mundescricao";
	} else {
		$sql = "SELECT muncod as codigo, muncod as descricao FROM sisfor.abrangenciacurso WHERE 1=2";
	}

	$_SESSION['indice_sessao_combo_popup']['muncod_abrangencia']['sql'] = $sql;
}

function cadastrarPolocurso($dados) {
	global $db;
	
	if($dados['polid']) {
		
		$polid = $dados['polid'];
		$sql = "UPDATE sisfor.poloscurso SET polnome='".$dados['polnome']."', polnumvagas='".$dados['polnumvagas']."' WHERE polid='".$polid."'";
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO sisfor.poloscurso(
	            sifid, polnome, polnumvagas, polmuncodsede, polstatus)
	    		VALUES ('".$dados['sifid']."', '".$dados['polnome']."', '".$dados['polnumvagas']."', null, 'A') RETURNING polid;";
		
		$polid = $db->pegaUm($sql);
	
	}
	
	$sql = "DELETE FROM sisfor.abrangenciacurso WHERE polid='".$polid."'";
	$db->executar($sql);
	
	if($dados['muncod_abrangencia'][0]) {
		foreach($dados['muncod_abrangencia'] as $muncod) {
			if($muncod) {
				
				$sql = "INSERT INTO sisfor.abrangenciacurso(
			            muncod, sifid, abrstatus, esfera, polid)
			    		VALUES ('".$muncod."', '".$dados['sifid']."', 'A', 'M', '".$polid."');";
				
				$db->executar($sql);
				
			}
		}
	}
	
	$db->commit();
		
	$al = array("alert"=>"Polo inserido com sucesso","javascript"=>"window.opener.definirAbrangencia();window.close();");
	alertlocation($al);

}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_REQUEST['polid']) {
	
	$sql = "SELECT * FROM sisfor.poloscurso WHERE polid='".$_REQUEST['polid']."'";
	$poloscurso = $db->pegaLinha($sql);
	
	$sql = "SELECT m.muncod as codigo, m.estuf||' - '||m.mundescricao as descricao 
			FROM sisfor.abrangenciacurso a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			WHERE polid='".$poloscurso['polid']."'";
	$muncod_abrangencia = $db->carregar($sql);
	
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>

<script>

function carregarFiltrosMunicipios(estuf) {

	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=carregarFiltrosMunicipios&sifid=<?=$_REQUEST['sifid'] ?>&esfera=M&estuf='+estuf,
   		async: false,
   		success: function(html){}
	});
	
}

function cadastrarPoloCurso() {
	
	if(jQuery('#polnome').val()=='') {
		alert('Nome em branco');
		return false;
	}

	if(jQuery('#polnumvagas').val()=='') {
		alert('Número de vagas em branco');
		return false;
	}
	
	selectAllOptions( document.getElementById( 'muncod_abrangencia' ) );
	
	document.getElementById('formulario').submit();
}

function habilitarUF(rede) {
	if(rede=='') {
		document.getElementById('estuf_abrangencia').value = '';
		carregarFiltrosMunicipios(document.getElementById('estuf_abrangencia').value);
		document.getElementById('estuf_abrangencia').disabled = true;
	} else {
		document.getElementById('estuf_abrangencia').disabled = false;
	}
}

</script>

<form method="post" id="formulario" enctype="multipart/form-data">
<? if($_REQUEST['polid']) : ?>
<input type="hidden" name="polid" id="polid" value="<?=$_REQUEST['polid'] ?>">
<? endif; ?>
<input type="hidden" name="requisicao" value="cadastrarPolocurso">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir municípios</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td>Orientação</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome do polo</td>
	<td>
	<?=campo_texto('polnome', "S", "S", "Nome do polo", 60, 60, "", "", '', '', 0, 'id="polnome"', "", $poloscurso['polnome']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Número de vagas do polo</td>
	<td>
	<?=campo_texto('polnumvagas', "S", "S", "Número de vagas do polo", 8, 7, "######", "", '', '', 0, 'id="polnumvagas"', "", $poloscurso['polnumvagas']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_abrangencia', $sql, 'S', 'Selecione', 'carregarFiltrosMunicipios', '', '', '', 'S', 'estuf_abrangencia', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Município</td>
	<td>
	<?
	$sql = "SELECT muncod as codigo, muncod as descricao FROM sisfor.abrangenciacurso WHERE 1=2";
	combo_popup( "muncod_abrangencia", $sql, "Municípios", "192x400", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="cadastrarpolo" value="Cadastrar Polo" onclick="cadastrarPoloCurso();"></td>
</tr>
</table>
</form>
