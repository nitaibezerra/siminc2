<?

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */


include "_funcoes_escola.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>

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
			
		}
	});
});

function inserirProfessoresAlfabetizadoresCpfLivre() {

	jQuery('#iuscpf').val( mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()) );
	var cpf   = replaceAll(replaceAll(jQuery('#iuscpf').val(),".",""),"-","");
	var nome  = jQuery('#iusnome').val();
	var email = jQuery('#iusemailprincipal').val();
	var tipo = jQuery('#tipo').val();

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

    /*
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
	*/
	
	if(tipo == '<?=PFL_COORDENADORPEDAGOGICO?>') {
		tipo = '<input type="hidden" name="pflcod['+cpf+']" value="<?=PFL_COORDENADORPEDAGOGICO ?>"> <input type="hidden" name="tipo['+cpf+']" value="censo"> Coordenador Pedagógico ( Bolsista )';
	} else if(tipo == '<?=PFL_PROFESSORALFABETIZADOR?>') {
		tipo = '<input type="hidden" name="pflcod['+cpf+']" value="<?=PFL_PROFESSORALFABETIZADOR ?>"> <input type="hidden" name="tipo['+cpf+']" value="cpflivre"> Professor ( Não bolsista )';
	} else {

	    var prev = window.opener.document.getElementById('td_numprofessoresprevisto').innerHTML;
	    var cada = window.opener.document.getElementById('td_numprofessorescadastrados').innerHTML;
	    if(parseInt(prev)<=parseInt(cada)) {
	    	alert('Não há mais vagas de Professores. Caso queira substituir, exclua um dos Professores selecionados na tela principal e selecione novamente');
	    	return false;
	    }
	
		
	} 
    
    var conf = confirm('Termo de Veracidade das Informações\n\nVocê assegura que este profissional atua como Docente ou Coordenador Pedagógico, em 2014, em turmas do Ensino Médio, nesta escola?');
    
    if(!conf) {
    	return false;
    }

    
	
	var resultado = inserirProfessorAlfabetizadorLabel(cpf,nome,email,tipo);
	
	divCarregando();
	
	window.opener.calcularNumProfessoresCadastrados();

	jQuery('#iuscpf').val('');
	jQuery('#iusnome').val('');
	jQuery('#iusemailprincipal').val('');
	jQuery('#tipo').val('');
	
	
	divCarregado();

}

function inserirProfessoresAlfabetizadores(obj) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	
	var cpf   = tabela.rows[linha.rowIndex-1].cells[1].innerHTML.substr(0,11);
	var nome  = tabela.rows[linha.rowIndex-1].cells[2].innerHTML;
	var email = tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].value;

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
    
    var prev = window.opener.document.getElementById('td_numprofessoresprevisto').innerHTML;
    var cada = window.opener.document.getElementById('td_numprofessorescadastrados').innerHTML;
    
    if(parseInt(prev)<=parseInt(cada)) {
    	alert('Não há mais vagas de Professores. Caso queira substituir, exclua um dos Professores selecionados na tela principal e selecione novamente');
    	return false;
    }
    
    var conf = confirm('Termo de Veracidade das Informações\n\nVocê assegura que este profissional atua como Docente ou Coordenador Pedagógico, em 2014, em turmas do Ensino Médio, nesta escola?');
    
    if(!conf) {
    	return false;
    }
	
	var resultado = inserirProfessorAlfabetizadorLabel(cpf,nome,email,'<input type="hidden" name="pflcod['+cpf+']" value="<?=PFL_PROFESSORALFABETIZADOR ?>"> <input type="hidden" name="tipo['+cpf+']" value="censo"> Professor ( Bolsista )');
	
	divCarregando();
	
	if(resultado) {
		tabela.rows[linha.rowIndex-1].cells[0].innerHTML = "<img src=../imagens/gif_inclui_d.gif>";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].className = "disabled";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].readOnly=true;
	}
	
	window.opener.calcularNumProfessoresCadastrados();
	
	divCarregado();

}


function inserirProfessorAlfabetizadorLabel(cpf,nome,email,tipo) {

	var existe_cpf = window.opener.document.getElementById('img_'+cpf);
	
	if(existe_cpf) {
		alert('CPF ja esta cadastrado');
		return false;
	}
	/*
	var existe_em_outros_municipios = false;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarPerfilOutrosMunicipios&cpf='+cpf+'&picid=<?=$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['picid'] ?>',
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
	*/

	var tabela_or = window.opener.document.getElementById('tabelaprofessoresalfabetizadores');
	
	var linha = tabela_or.insertRow(tabela_or.rows.length);
	var col_acoes = linha.insertCell(0);
	col_acoes.innerHTML = "<center><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\" id=\"img_"+cpf+"\" onclick=\"removerProfessorAlfabetizador('', this);\"></center>";
	var col_cpf = linha.insertCell(1);
	col_cpf.innerHTML = mascaraglobal('###.###.###-##',cpf)+"<input type=\"hidden\" name=\"cpf[]\" value=\""+cpf+"\">";
	var col_nome = linha.insertCell(2);
	col_nome.innerHTML = nome+"<input type=\"hidden\" name=\"nome["+cpf+"]\" value=\""+nome+"\">";
	var col_email = linha.insertCell(3);
	col_email.id  = "td_img_"+cpf;
	col_email.innerHTML = email+"<input type=\"hidden\" name=\"email["+cpf+"]\" value=\""+email+"\">";
	var col_tipo = linha.insertCell(4);
	col_tipo.id  = "td2_img_"+cpf;
	col_tipo.innerHTML = tipo;
	
	window.opener.document.getElementById('alteracaodados').value="1";
	
	return true;

}
</script>
<div id="modalOrientacaoAdm" style="display:none;">
	<form method="post" id="formulario_orientacao" name="formulario_orientacao">
	<input type="hidden" name="abaid" id="abaid">
	<input type="hidden" name="requisicao" value="salvarOrientacaoAdm">
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orientação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"></td>
		<td><? echo campo_textarea( 'oabdesc', 'S', 'S', '', '70', '4', '5000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="salvarorientacao" value="Salvar Orientação" onclick="salvarOrientacaoAdm();"></td>
	</tr>
	</table>
	</form>
	</div>
<?
if(!$_REQUEST['aba']) $_REQUEST['aba'] = "buscacenso";
if(!$_REQUEST['codigoinep']) $_REQUEST['codigoinep'] = $_SESSION['sismedio']['gestorescola']['lemcodigoinep'];

$abaativa = "sismedio.php?modulo=principal/escola/inserirprofessores&acao=A&aba=".$_REQUEST['aba'];

$menu[] = array("id" => 1, "descricao" => "Profissionais Censo Escolar", "link" => "sismedio.php?modulo=principal/escola/inserirprofessores&acao=A&aba=buscacenso");
$menu[] = array("id" => 2, "descricao" => "Profissionais Fora do Censo", "link" => "sismedio.php?modulo=principal/escola/inserirprofessores&acao=A&aba=cpflivre");

echo montarAbasArray($menu, $abaativa);

if($_REQUEST['aba'] == 'buscacenso') :
?>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao('/sismedio/sismedio.php?modulo=principal/escola/inserirprofessores&acao=A&aba=buscacenso'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><?=campo_texto('cpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="cpf"', '', $_REQUEST['cpf']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><?=campo_texto('nome', "N", "S", "Nome", 30, 60, "", "", '', '', 0, 'id="nome"', '', $_REQUEST['nome']); ?></td>
</tr>
<!-- 
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	if(!$_REQUEST['uf']) $_REQUEST['uf'] = $_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['estuf'];
	$sql = "SELECT estuf as codigo, estuf || ' - ' || estdescricao as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<?
	if(!$_REQUEST['muncod_endereco']) $_REQUEST['muncod_endereco'] = $_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['muncod'];
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE ".(($_REQUEST['uf'])?"estuf='".$_REQUEST['uf']."'":"1=2");
	$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	?>
	</td>
</tr>
 -->
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Voltar" onclick="divCarregando();window.location='sismedio.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></td>
</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2">
	<?
	
	$filtroutilizado = false;
	
	// tiver filtro por CPF, não executar outros filtros
	if($_REQUEST['cpf']) {

		$f[] = "t.cpf='".str_replace(array(".","-"),array(""),$_REQUEST['cpf'])."'";
		$filtroutilizado = true;
		
	} else {

		if($_REQUEST['uf']) {
			$f[] = "t.uf='".$_REQUEST['uf']."'";
			$filtroutilizado = true;
		}
		
		if($_REQUEST['muncod_endereco']) {
			$f[] = "t.muncod='".$_REQUEST['muncod_endereco']."'";
			$filtroutilizado = true;
		}
		
		if($_REQUEST['nome']) {
			$f[] = "removeacento(docente) ilike '%'||removeacento('".$_REQUEST['nome']."')||'%'";
			$filtroutilizado = true;
		}
	}
	
	if(!$filtroutilizado) {
		if($_SESSION['sismedio']['gestorescola']['lemcodigoinep']) {
			$f[] = "p.codigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."'";
		}
	}
	
	//criando select
	if($_SERIE_TURMA) {
		foreach($_SERIE_TURMA as $sercod => $serdsc) {
			$opt .= "<input '||CASE WHEN tt.tpeid IS NULL THEN '' ELSE 'disabled' END||' type=\"checkbox\" '|| CASE WHEN strpos(i.iusserieprofessor, '{$sercod};')!=0 THEN 'checked' ELSE '' END||' id=\"serie_img_'||cpf||'_{$sercod}\" value=\"{$sercod}\"> ".$serdsc;
		}
	}
	
	if(!$f) {
		die("<script>
				alert('Nenhum filtro foi utilizado! A janela irá fechar, tente novamente.');
				window.close();
			 </script>");
	}
	
	$sql = "SELECT DISTINCT '<img id=\"a_img_'||t.cpf||'\" '||CASE WHEN tt.tpeid IS NULL THEN 'src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirProfessoresAlfabetizadores(this);\"' ELSE 'src=\"../imagens/gif_inclui_d.gif\"' END||' >' as acao, 
				   t.cpf, 
				   t.docente,
				   '<input id=\"email_img_'||t.cpf||'\" type=text name=email['||t.cpf||'] size=30 value=\"'||t.email||'\" '||CASE WHEN tt.tpeid IS NULL THEN 'class=normal' ELSE 'class=disabled disabled' END||'>' as email,
					'<font style=font-size:xx-small;>'||lemnomeescola||'</font>' as escola 
			FROM sismedio.professoresalfabetizadores t 
			LEFT JOIN sismedio.professoresalfabetizadoresescola p ON p.cpf = t.cpf 
			LEFT JOIN sismedio.identificacaousuario i ON i.iuscpf = t.cpf 
			LEFT JOIN sismedio.tipoperfil tt ON tt.iusd = i.iusd 
			LEFT JOIN sismedio.listaescolasensinomedio lem ON lem.lemcodigoinep::bigint = i.iuscodigoinep
			".(($f)?" WHERE ".implode(" AND ",$f):"")."
			ORDER BY t.docente";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Cadastrado em");
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
	<td><? echo carregarOrientacao('/sismedio/sismedio.php?modulo=principal/escola/inserirprofessores&acao=A&aba=cpflivre'); ?></td>
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
	<td class="SubTituloDireita" width="20%">Tipo</td>
	<td>
	<? 
	$arrPP = array(0 => array('codigo' => PFL_PROFESSORALFABETIZADOR, 'descricao' => 'Professor ( Não bolsista )'),
				   1 => array('codigo' => PFL_COORDENADORPEDAGOGICO, 'descricao' => 'Coordenador Pedagógico ( Bolsista )'),
				   );
	$db->monta_combo('tipo', $arrPP, 'S', 'Selecione', '', '', '', '', 'N', 'tipo', ''); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="inserirProfessoresAlfabetizadoresCpfLivre();"> <input type="button" value="Voltar" onclick="window.close();"></td>
</tr>
</table>
<? endif; ?>
