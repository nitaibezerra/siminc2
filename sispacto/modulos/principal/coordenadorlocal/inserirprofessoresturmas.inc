<?
include "_funcoes_coordenadorlocal.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']) {
 	$al = array("alert"=>"Erro na navegação. Acesse novamente.","javascript"=>"window.close();window.opener.location='sispacto.php?modulo=inicio&acao=C';");
 	alertlocation($al);
}

$turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid']));
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>

<script>
jQuery(document).ready(function() {
	window.opener.jQuery("[id^='img_']").each(function() {
	
		var id_img = replaceAll(replaceAll(jQuery(this).attr('id'),".",""),"-","");
		
		if(document.getElementById('a_'+id_img)) {
		
			jQuery('#a_'+id_img).attr('src','../imagens/gif_inclui_d.gif');
			jQuery('#a_'+id_img).attr('onclick','').unbind('click');
			jQuery('#a_'+id_img).css('cursor','');
			
			var emailp = window.opener.document.getElementById('td_'+jQuery(this).attr('id')).innerHTML;
			emailp = emailp.split("<");
			
			jQuery('#email_'+id_img).val(emailp[0]);
			jQuery('#email_'+id_img).attr('class','disabled');
			jQuery('#email_'+id_img).attr('readonly', true);
			
			var serie = window.opener.document.getElementById('td2_'+jQuery(this).attr('id')).title;

			var chk = serie.split(';');
			
			jQuery("[id^='serie_"+id_img+"']").attr('disabled',true);
			
			if(chk.length) {
				for(i=0;i<chk.length;i++) {
					jQuery('#serie_'+id_img+'_'+chk[i]).attr('checked',true);
				}
			}
			
		}
	});
});

function inserirProfessoresAlfabetizadoresCpfLivre() {

	jQuery('#iuscpf').val( mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()) );
	var cpf   = replaceAll(replaceAll(jQuery('#iuscpf').val(),".",""),"-","");
	var nome  = jQuery('#iusnome').val();
	var email = jQuery('#iusemailprincipal').val();

	var serie = '';
	jQuery("[name^='sercod[']").each(function() {
		if(jQuery(this).attr('checked')==true) {
			serie += jQuery(this).val()+';';
		}
	});
	
	if(cpf=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(cpf)) {
		alert('CPF inválido');
		return false;
	}
	
	if(nome=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(email=='') {
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(email)) {
    	alert('Email inválido');
    	return false;
    }
    
    if(serie=='') {
    	alert('Série não selecionada');
    	return false;
    }
    
	var existe_censo_2012 = false;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarCenso2012&cpf='+cpf,
   		async: false,
   		success: function(texto){
   			if(texto!="") {
   				alert('CPF cadastrado consta no Censo Escola 2012 no Município '+texto+' e não pode ser inserido por esta aba, selecione a aba Lista do Censo Escolar e digite o CPF.');
   				existe_censo_2012 = true;	
   			}
   		}
	});
	if(existe_censo_2012) {
		return false;
	}

    
    var prev = window.opener.document.getElementById('td_numprofessoresprevisto').innerHTML;
    var cada = window.opener.document.getElementById('td_numprofessorescadastrados').innerHTML;
    if(parseInt(prev)<=parseInt(cada)) {
    	alert('Não há mais vagas de Professores Alfabetizadores. Caso queira substituir, exclua um dos Professores Alfabetizadores selecionados na tela principal e selecione novamente');
    	return false;
    }
    
    var conf = confirm('Termo de Veracidade das Informações\n\nVocê assegura que este(a) professor(a) preenche os requisitos?');
    
    if(!conf) {
    	return false;
    }
	
	var resultado = inserirProfessorAlfabetizadorLabel(cpf,nome,email,serie,'cpflivre');
	
	divCarregando();
	
	window.opener.calcularNumProfessoresCadastrados();
	
	divCarregado();

}

function inserirProfessoresAlfabetizadores(obj) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	
	var cpf   = tabela.rows[linha.rowIndex-1].cells[1].innerHTML.substr(0,11);
	var nome  = tabela.rows[linha.rowIndex-1].cells[2].innerHTML;
	var email = tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].value;

	var serie = '';

	for(var i=0;i<tabela.rows[linha.rowIndex-1].cells[4].childNodes.length;i++) {
		if(tabela.rows[linha.rowIndex-1].cells[4].childNodes[i].checked) {
			serie += tabela.rows[linha.rowIndex-1].cells[4].childNodes[i].value+';';
		}
	}
	
	if(cpf=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(cpf)) {
		alert('CPF inválido');
		return false;
	}
	
	if(nome=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(email=='') {
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(email)) {
    	alert('Email inválido');
    	return false;
    }
    
    if(serie=='') {
    	alert('Série não selecionada');
    	return false;
    }
    
    var prev = window.opener.document.getElementById('td_numprofessoresprevisto').innerHTML;
    var cada = window.opener.document.getElementById('td_numprofessorescadastrados').innerHTML;
    if(parseInt(prev)<=parseInt(cada)) {
    	alert('Não há mais vagas de Professores Alfabetizadores. Caso queira substituir, exclua um dos Professores Alfabetizadores selecionados na tela principal e selecione novamente');
    	return false;
    }
    
    var conf = confirm('Termo de Veracidade das Informações\n\nVocê assegura que este(a) professor(a) preenche os requisitos?');
    
    if(!conf) {
    	return false;
    }
	
	var resultado = inserirProfessorAlfabetizadorLabel(cpf,nome,email,serie,'censo');
	
	divCarregando();
	
	if(resultado) {
		tabela.rows[linha.rowIndex-1].cells[0].innerHTML = "<img src=../imagens/gif_inclui_d.gif>";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].className = "disabled";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].readOnly=true;
	}
	
	window.opener.calcularNumProfessoresCadastrados();
	
	divCarregado();

}


function inserirProfessorAlfabetizadorLabel(cpf,nome,email,serie,tipo) {

	var series = new Array();
	<? if($_SERIE_TURMA) : ?>
	<? foreach($_SERIE_TURMA as $sercod => $serdsc) : ?>
	series['<?=$sercod ?>']  = "<?=$serdsc ?>";
	<? endforeach; ?>
	<? endif; ?>

	var existe_cpf = window.opener.document.getElementById('img_'+cpf);
	
	if(existe_cpf) {
		alert('CPF ja esta cadastrado');
		return false;
	}
	
	var existe_em_outros_municipios = false;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarPerfilOutrosMunicipios&cpf='+cpf+'&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>',
   		async: false,
   		success: function(texto){
   			if(texto!="") {
   				alert(texto);
   				existe_em_outros_municipios = true;	
   			}
   		}
	});
	if(existe_em_outros_municipios) {
		return false;
	}


	var tabela_or = window.opener.document.getElementById('tabelaprofessoresalfabetizadores');
	
	var linha = tabela_or.insertRow(tabela_or.rows.length);
	var col_acoes = linha.insertCell(0);
	col_acoes.innerHTML = "<center><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\" id=\"img_"+cpf+"\" onclick=\"removerOrientadorEstudo('', this);\"></center>";
	var col_cpf = linha.insertCell(1);
	col_cpf.innerHTML = mascaraglobal('###.###.###-##',cpf)+"<input type=\"hidden\" name=\"cpf[]\" value=\""+cpf+"\">";
	var col_nome = linha.insertCell(2);
	col_nome.innerHTML = nome+"<input type=\"hidden\" name=\"nome["+cpf+"]\" value=\""+nome+"\">";
	var col_email = linha.insertCell(3);
	col_email.id  = "td_img_"+cpf;
	col_email.innerHTML = email+"<input type=\"hidden\" name=\"email["+cpf+"]\" value=\""+email+"\">";
	var col_serie = linha.insertCell(4);
	col_serie.id  = "td2_img_"+cpf;
	col_serie.title = serie;
	
	var labelserie = '';
	for(i=0;i<serie.length;i=i+3) {
		labelserie+=series[serie.substr(i,2)]+'&nbsp;';
	}
	
	col_serie.innerHTML = labelserie+"<input type=\"hidden\" name=\"serie["+cpf+"]\" id=\"serie_img_"+cpf+"\" value=\""+serie+"\"><input type=\"hidden\" name=\"tipo["+cpf+"]\" value=\""+tipo+"\">";
	var col_anexo = linha.insertCell(5);
	if(tipo=="cpflivre") {
		col_anexo.innerHTML = "<input type=\"file\" name=\"anexoportaria["+cpf+"]\">";	
	} else {
		col_anexo.innerHTML = "&nbsp;";
	}
	
	var col_tipo = linha.insertCell(6);
	if(tipo=="cpflivre") {
		col_tipo.innerHTML = "Não bolsista";	
	} else {
		col_tipo.innerHTML = "Bolsista";
	}
	
	window.opener.document.getElementById('alteracaodados').value="1";
	
	return true;

}
</script>

<?
if(!$_REQUEST['aba']) $_REQUEST['aba'] = "buscacenso";

$abaativa = "sispacto.php?modulo=principal/coordenadorlocal/inserirprofessoresturmas&acao=A&aba=".$_REQUEST['aba']."&turid=".$_REQUEST['turid'];

$menu[] = array("id" => 1, "descricao" => "Lista Censo Escolar", "link" => "sispacto.php?modulo=principal/coordenadorlocal/inserirprofessoresturmas&acao=A&aba=buscacenso&turid=".$_REQUEST['turid']);
$menu[] = array("id" => 2, "descricao" => "Professores fora do Censo", "link" => "sispacto.php?modulo=principal/coordenadorlocal/inserirprofessoresturmas&acao=A&aba=cpflivre&turid=".$_REQUEST['turid']);

echo montarAbasArray($menu, $abaativa);

if($_REQUEST['aba'] == 'buscacenso') :
?>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao('/sispacto/sispacto.php?modulo=principal/coordenadorlocal/inserirprofessoresturmas&acao=A'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><?=campo_texto('cpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="cpf"', '', $_REQUEST['cpf']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><?=campo_texto('nome', "N", "S", "Nome", 30, 60, "", "", '', '', 0, 'id="nome"', '', $_REQUEST['nome']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	if(!$_REQUEST['uf']) $_REQUEST['uf'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf'];
	$sql = "SELECT estuf as codigo, estuf || ' - ' || estdescricao as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<?
	if(!$_REQUEST['muncod_endereco']) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'];
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE ".(($_REQUEST['uf'])?"estuf='".$_REQUEST['uf']."'":"1=2");
	$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Voltar" onclick="divCarregando();window.location='sispacto.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></td>
</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2">
	<?
	// tiver filtro por CPF, não executar outros filtros
	if($_REQUEST['cpf']) {
		$f[] = "cpf='".str_replace(array(".","-"),array(""),$_REQUEST['cpf'])."'";
	} else {
		if($_REQUEST['uf']) {
			$f[] = "uf='".$_REQUEST['uf']."'";
		}
		if($_REQUEST['muncod_endereco']) {
			$f[] = "t.muncod='".$_REQUEST['muncod_endereco']."'";
		}
		if($_REQUEST['nome']) {
			$f[] = "removeacento(docente)=removeacento('".$_REQUEST['nome']."')";
		}
	}
	
	//criando select
	if($_SERIE_TURMA) {
		foreach($_SERIE_TURMA as $sercod => $serdsc) {
			$opt .= "<input '||CASE WHEN tt.tpeid IS NULL THEN '' ELSE 'disabled' END||' type=\"checkbox\" '|| CASE WHEN strpos(i.iusserieprofessor, '{$sercod};')!=0 THEN 'checked' ELSE '' END||' id=\"serie_img_'||cpf||'_{$sercod}\" value=\"{$sercod}\"> ".$serdsc;
		}
	}
	
	$sql = "SELECT '<img id=\"a_img_'||cpf||'\" '||CASE WHEN tt.tpeid IS NULL THEN 'src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirProfessoresAlfabetizadores(this);\"' ELSE 'src=\"../imagens/gif_inclui_d.gif\"' END||' >' as acao, cpf, docente, '<input id=\"email_img_'||cpf||'\" type=text name=email['||cpf||'] size=30 value=\"'||email||'\" '||CASE WHEN tt.tpeid IS NULL THEN 'class=normal' ELSE 'class=disabled disabled' END||'>' as email, '{$opt}' as serie, CASE WHEN tt.tpeid IS NOT NULL THEN mu2.estuf || ' / ' || mu2.mundescricao || ' ( ' || tu.turdesc || ' )' ELSE '' END as munalocado
			FROM sispacto.professoresalfabetizadores t 
			LEFT JOIN territorios.municipio m ON m.muncod = t.muncod 
			LEFT JOIN sispacto.identificacaousuario i ON i.iuscpf = t.cpf AND i.iusstatus='A'
			LEFT JOIN sispacto.tipoperfil tt ON tt.iusd = i.iusd AND tt.pflcod=".PFL_PROFESSORALFABETIZADOR." 
			LEFT JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			LEFT JOIN territorios.municipio mu2 ON mu2.muncod = p.muncod 
			LEFT JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			LEFT JOIN sispacto.turmas tu ON tu.turid = ot.turid
			".(($f)?" WHERE ".implode(" AND ",$f):"")."
			GROUP BY cpf, docente, email, m.estuf, m.muncod, m.mundescricao, tt.tpeid, i.iusserieprofessor, mu2.estuf, mu2.mundescricao, tu.turdesc
			ORDER BY docente";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Turma(s) que o docente irá lecionar em 2013","Inserido em");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
	?>	
	</td>
</tr>
</table>
<? else : ?>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>
function carregaUsuario() {
	divCarregando();
	var usucpf=document.getElementById('iuscpf').value;
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
	
	document.getElementById('iusnome').value=comp.dados.no_pessoa_rf;
	divCarregado();
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td>Na aba "Professores fora do Censo" é possível buscar pelo CPF, professores que não foram cadastrados no Censo Escolar de 2012. É permitido inserir nas turmas apenas  professores em efetivo exercício no ano de 2013 em turmas de alfabetização (1º, 2º e 3º ano ou turmas multisseriadas e multietapa). Os professores que forem inseridos e não estão no Censo Escolar não receberão bolsa de estudo</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">CPF</td>
	<td><?=campo_texto('iuscpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##"), 'if(this.value!=\'\'){carregaUsuario();}'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome</td>
	<td><?=campo_texto('iusnome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Email</td>
	<td><?=campo_texto('iusemailprincipal', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Ano/Etapa</td>
	<td><?
	if($_SERIE_TURMA) {
		foreach($_SERIE_TURMA as $sercod => $serdsc) {
			$opt .= "<input type=\"checkbox\" name=\"sercod[]\" value=\"{$sercod}\"> ".$serdsc;
		}
	}
	echo $opt;
	
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$_REQUEST['uf'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf'];
	echo "<input type=\"hidden\" name=\"uf\" id=\"uf\" value=\"".$_REQUEST['uf']."\">".$_REQUEST['uf'];
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<?
	if($_SESSION['sispacto']['esfera']=='municipal') { 
		$_REQUEST['muncod_endereco'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'];
		$mundescricao = $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod_endereco']."'");
		echo "<input type=\"hidden\" name=\"mundescricao\" id=\"mundescricao\" value=\"".$mundescricao."\"><input type=\"hidden\" name=\"muncod_endereco\" id=\"muncod_endereco\" value=\"".$_REQUEST['muncod_endereco']."\">".$mundescricao;
	} else {
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf']."' ORDER BY mundescricao";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', 'carregarMundescricao', '', '', '', 'S', 'muncod_endereco', '');
		echo "<input type=\"hidden\" name=\"mundescricao\" id=\"mundescricao\" value=\"\">";
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="inserirProfessoresAlfabetizadoresCpfLivre();"> <input type="button" value="Voltar" onclick="window.close();"></td>
</tr>
</table>
<? endif; ?>
