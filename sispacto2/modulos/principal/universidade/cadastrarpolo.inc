<?
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

function carregarPolo($dados) {
	global $db;
	$arrPolo['dados'] 	    = $db->pegaLinha("SELECT p.polid, p.poldesc, m.estuf as estuf_sede, m.muncod as muncod_sede, p.polunico FROM sispacto.polo p
	 										  LEFT JOIN territorios.municipio m ON m.muncod = p.muncod  
											  WHERE polid='".$dados['polid']."'");
	
	$arrPolo['supervisor']  = $db->pegaLinha("SELECT spocpf, sponome, spodddtelefone, sponumtelefone, spoemail FROM sispacto.supervisorpolo WHERE polid='".$dados['polid']."'");
	$arrPolo['abrangencia']['arrAb'] = $db->carregar("SELECT m.muncod, m.estuf||' / '||m.mundescricao as municipios FROM sispacto.abrangenciapolomunicipio a
											 	   INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
											 	   WHERE polid='".$dados['polid']."'");
	return $arrPolo;
}

if($_REQUEST['polid']) {
	$infopolo = carregarPolo(array("polid"=>$_REQUEST['polid']));
	if($infopolo['dados'])extract($infopolo['dados']);
	if($infopolo['supervisor'])extract($infopolo['supervisor']);
	if($infopolo['abrangencia'])extract($infopolo['abrangencia']);
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

function removerAreaAbrangencia(obj) {
	var linha = obj.parentNode.parentNode;
	alert(linha);
}


function carregarMunicipiosPorUFSede(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_sede&name=muncod_sede&estuf='+estuf,'td_municipio_sede');
	} else {
		document.getElementById('td_municipio_sede').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUFAbrangencia(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_abrangencia&name=muncod_abrangencia&estuf='+estuf,'td_municipio_abrangencia');
	} else {
		document.getElementById('td_municipio_abrangencia').innerHTML = "Selecione uma UF";
	}
}

function adicionarMunicipioAbrangencia() {

	if(jQuery('#estuf_abrangencia').val()=='') {
		alert('Selecione uma UF');
		return false;
	}
	
	if(jQuery('#muncod_abrangencia').val()=='') {
		alert('Selecione um Município');
		return false;
	}
	
	if(document.getElementById('td_ab_'+jQuery('#muncod_abrangencia').val())) {
		alert('Área de Abrangência ja cadastrada para esta Universidade');
		return false;
	}
	
	var existe_municipio_uni = false;
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarAreaAbrangencia&muncod='+jQuery('#muncod_abrangencia').val(),
   		async: false,
   		success: function(res){
   			if(res!='') {
   				existe_municipio_uni = true;
   			}
   		}});
   		
   	if(existe_municipio_uni) {
   		alert('Área de Abrangência ja cadastrada em outra Universidade');
   		return false;
   	}

	var tabela = document.getElementById('abrangencia');
	
	var linha = tabela.insertRow(tabela.rows.length);
	var col1 = linha.insertCell(0);
	col1.id = "td_ab_"+jQuery('#muncod_abrangencia').val();
	col1.innerHTML = "<center><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"removerAreaAbrangencia(this);\"><input type=hidden name=abrangenciapolomunicipio[] value=\""+jQuery('#muncod_abrangencia').val()+"\"></center>";
	var col2 = linha.insertCell(1);
	col2.innerHTML = jQuery('#estuf_abrangencia').val()+" / "+jQuery("#muncod_abrangencia option:selected").text();
}

function inserirPolo() {
	if(jQuery('#poldesc').val()=='') {
		alert('Nome do polo em branco');
		return false;
	}

	if(jQuery('#estuf_sede').val()=='') {
		alert('UF da sede em branco');
		return false;
	}
    
	if(jQuery('muncod_sede').val()=='') {
		alert('Município da sede em branco');
		return false;
	}
	
	if(document.getElementById('abrangencia').rows.length==1) {
		alert('É necessário 1 município de abrangência');
		return false;
	}

	document.getElementById('formulario').submit();
}

</script>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirPolo">
<input type="hidden" name="ecuid" value="<?=$_SESSION['sispacto']['universidade']['ecuid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Cadastrar Polo</td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome do polo</td>
	<td><?=campo_texto('poldesc', "S", (($polunico=="t")?"N":"S"), "Nome", 67, 150, "", "", '', '', 0, 'id="poldesc"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sede do polo</td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_sede', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFSede', '', '', '', 'S', 'estuf_sede', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio_sede">
	<? 
	if($estuf_sede) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf_sede."' ORDER BY mundescricao";
		$db->monta_combo('muncod_sede', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_sede', '');
	else :
		echo "Selecione UF";
	endif;
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Área de Abrangência</td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_abrangencia', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFAbrangencia', '', '', '', 'S', 'estuf_abrangencia', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio_abrangencia">
	<? 
	if($estuf_abrangencia) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf_abrangencia."' ORDER BY mundescricao";
		$db->monta_combo('muncod_abrangencia', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_abrangencia', '');
	else :
		echo "Selecione UF";
	endif;
	?>
	</td>
</tr>
<tr>
	<td colspan="2" align="center">
	<input type="button" value="Adicionar Área de Abrangência" onclick="adicionarMunicipioAbrangencia();">
	</td>
</tr>
<tr>
	<td colspan="2">
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="abrangencia" width="100%">
	<tr>
	<td class="SubTituloCentro" width="10%">&nbsp;</td>
	<td class="SubTituloCentro">Municípios</td>
	</tr>
	<? if($arrAb[0]) : ?>
	<? foreach($arrAb as $ab) : ?>
	<tr>
	<td width="10%"><center><img src="../imagens/excluir.gif" style="cursor:pointer;" onclick="removerAreaAbrangencia(this);"><input type="hidden" name="abrangenciapolomunicipio[]" value="<?=$ab['muncod'] ?>"></center></td>
	<td><?=$ab['municipios'] ?></td>
	</tr>
	<? endforeach; ?>
	<? endif; ?>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" onclick="inserirPolo();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
</table>
</form>
