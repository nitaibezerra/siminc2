<?
include "_funcoes_coordenadorlocal.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid']) {
	$al = array("alert"=>"Problemas no acesso a página. Por favor, utilize a navegação do sistema para acessar as páginas.","location"=>"sispacto3.php?modulo=inicio&acao=C");
	alertlocation($al);
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>
var tipoOrientador = new Array(); 

<? foreach($_TIPO_ORIENTADORES as $key => $value) : ?>
tipoOrientador['<?=$key ?>'] = "<?=$value ?>";
<? endforeach; ?>

function inserirOrientadorEstudoLabel(cpf,nome,email,tipo,muncod,mundescricao) {

	var existe_cpf = window.opener.document.getElementById('img_'+cpf);
	
	if(existe_cpf) {
		alert('CPF ja esta cadastrado');
		return false;
	}
	
	var existe_em_outros_municipios = false;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarPerfilOutrosMunicipios&cpf='+cpf+'&picid=<?=$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'] ?>',
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


	var tabela_or = window.opener.document.getElementById('tabelaorientadoresestudo');
	
	var total_a_serem_cadastrados = parseInt(window.opener.document.getElementById('total_a_serem_cadastrados').value);
	
	if(total_a_serem_cadastrados <= (tabela_or.rows.length-2)) {
		alert('Não há mais vagas de Orientadores de Estudo. Caso queira substituir, exclua um dos Orientadores selecionados na tela principal e selecione novamente');
		return false;
	}

	var linha = tabela_or.insertRow(tabela_or.rows.length-1);
	var col_acoes = linha.insertCell(0);
	col_acoes.innerHTML = "<center><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\" id=\"img_"+cpf+"_"+muncod+"\" onclick=\"removerOrientadorEstudo('', this);\"></center>";
	var col_cpf = linha.insertCell(1);
	col_cpf.innerHTML = cpf+"<input type=\"hidden\" name=\"cpf[]\" value=\""+cpf+"\">";
	var col_nome = linha.insertCell(2);
	col_nome.innerHTML = nome+"<input type=\"hidden\" name=\"nome["+cpf+"]\" value=\""+nome+"\">";
	var col_email = linha.insertCell(3);
	col_email.id  = "td_img_"+cpf+'_'+muncod;
	col_email.innerHTML = email+"<input type=\"hidden\" name=\"email["+cpf+"]\" value=\""+email+"\">";
	var col_mundescricao = linha.insertCell(4);
	col_mundescricao.innerHTML = mundescricao+"<input type=\"hidden\" name=\"muncodatuacao["+cpf+"]\" value=\""+muncod+"\">";
	var col_tipo = linha.insertCell(5);
	col_tipo.innerHTML = tipoOrientador[tipo]+"<input type=\"hidden\" name=\"tipo["+cpf+"]\" value=\""+tipo+"\">";
	var col_anexo = linha.insertCell(6);
	if(tipo=="profissionaismagisterio") {
		col_anexo.innerHTML = "<input type=\"file\" name=\"anexoportaria["+cpf+"]\">";	
	} else {
		col_anexo.innerHTML = "&nbsp;";
	}

	
	window.opener.document.getElementById('alteracaodados').value="1";
	
	return true;

}

function atualizaBotaoOrientadorEstudo(tipo) {
	var deparaMostrarBotaoOrientadorEstudo = new Array();
	deparaMostrarBotaoOrientadorEstudo['professorsispacto2014'] 	  = 'tutoresproletramento';
	deparaMostrarBotaoOrientadorEstudo['tutoresproletramento'] 		  = 'tutoresredesemproletramento';
	deparaMostrarBotaoOrientadorEstudo['tutoresredesemproletramento'] = 'profissionaismagisterio';

	if(deparaMostrarBotaoOrientadorEstudo[tipo]) {
		ajaxatualizar('requisicao=atualizarBotoesOrientadoresEstudo&picid=<?=$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'] ?>&tipo='+deparaMostrarBotaoOrientadorEstudo[tipo],'');
	}

}

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

function inserirProfissionalMagisterio() {

	if(!document.getElementById('muncod_endereco')) {
		alert('UF em branco');
		return false;
	}

	jQuery('#iuscpf').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()));
	
	if(jQuery('#iuscpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#iuscpf').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#iusnome').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#iusemailprincipal').val()=='') {
		alert('Email Principal em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#iusemailprincipal').val())) {
    	alert('Email Principal inválido');
    	return false;
    }
    
    if(jQuery('#muncod_endereco').val()=='') {
    	alert('Município em branco');
    	return false;
    }
    
    var conf = confirm('Termo de Veracidade das Informações\n\nVocê assegura que o(a) profissional cadastrado preenche os requisitos abaixo?\n\n-É servidor(a) efetivo(a) rede de ensino\n-Possui formação em Licenciatura ou Pedagogia\n-Possui experiência na formação de professores alfabetizadores\n-Possui disponibilidade para atuar como Orientador(a) de estudo');
    
    if(!conf) {
    	return false;
    }

	
	var resultado = inserirOrientadorEstudoLabel(jQuery('#iuscpf').val(),jQuery('#iusnome').val(),jQuery('#iusemailprincipal').val(),"profissionaismagisterio",jQuery('#muncod_endereco').val(),jQuery('#uf').val()+' / '+jQuery("#mundescricao").val());
	
	divCarregando();	
	
	if(resultado) {
		window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';
	} else {
		divCarregado();
	}

}

function inserirProfessoresSispacto2014(obj, mun) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	
	var cpf    = tabela.rows[linha.rowIndex-1].cells[1].innerHTML.substr(0,11);
	var nome   = tabela.rows[linha.rowIndex-1].cells[2].innerHTML;
	var email  = tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].value;
	var muncod = jQuery('#muncod_pl_'+cpf+'_'+mun).val();
	var mundescricao = jQuery('#uf_pl_'+cpf+'_'+mun).val()+' / '+jQuery('#mundescricao_pl_'+cpf+'_'+mun).val();
	
	cpf = mascaraglobal('###.###.###-##',cpf);
	
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
    
    if(muncod=='') {
    	alert('Município em branco');
    	return false;
    }
    
    var conf = confirm('Você assegura que este(a) profissional foi Professor alfabetizador no SISPACTO 2014 e foi recomendado para certificação?');
    
    if(!conf) {
    	return false;
    }
	
	var resultado = inserirOrientadorEstudoLabel(cpf,nome,email,"professorsispacto2014",muncod,mundescricao);
	
	divCarregando();
	
	if(resultado) {
		atualizaBotaoOrientadorEstudo('professorsispacto2014');
		atualizaBotaoOrientadorEstudo('tutoresproletramento');
		atualizaBotaoOrientadorEstudo('tutoresredesemproletramento');
		tabela.rows[linha.rowIndex-1].cells[0].innerHTML = "<img src=../imagens/gif_inclui_d.gif>";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].className = "disabled";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].readOnly=true;
	}
	
	divCarregado();

}

function inserirTutoresProletramento(obj, mun) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	
	var cpf    = tabela.rows[linha.rowIndex-1].cells[1].innerHTML.substr(0,11);
	var nome   = tabela.rows[linha.rowIndex-1].cells[2].innerHTML;
	var email  = tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].value;
	var muncod = jQuery('#muncod_pl_'+cpf+'_'+mun).val();
	var mundescricao = jQuery('#uf_pl_'+cpf+'_'+mun).val()+' / '+jQuery('#mundescricao_pl_'+cpf+'_'+mun).val();
	
	cpf = mascaraglobal('###.###.###-##',cpf);
	
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
    
    if(muncod=='') {
    	alert('Município em branco');
    	return false;
    }
    
    var conf = confirm('Você assegura que este(a) profissional foi tutor do Pró-Letramento e está vinculado à sua rede de ensino?');
    
    if(!conf) {
    	return false;
    }
	
	var resultado = inserirOrientadorEstudoLabel(cpf,nome,email,"tutoresproletramento",muncod,mundescricao);
	
	divCarregando();
	
	if(resultado) {
		atualizaBotaoOrientadorEstudo('professorsispacto2014');
		atualizaBotaoOrientadorEstudo('tutoresproletramento');
		atualizaBotaoOrientadorEstudo('tutoresredesemproletramento');
		tabela.rows[linha.rowIndex-1].cells[0].innerHTML = "<img src=../imagens/gif_inclui_d.gif>";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].className = "disabled";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].readOnly=true;
	}
	
	divCarregado();

}

function inserirTutoresSemProletramento(obj, mun) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	
	var cpf   = tabela.rows[linha.rowIndex-1].cells[1].innerHTML.substr(0,11);
	var nome  = tabela.rows[linha.rowIndex-1].cells[2].innerHTML;
	var email = tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].value;
	var muncod = jQuery('#muncod_pl_'+cpf+'_'+mun).val();
	var mundescricao = jQuery('#uf_pl_'+cpf+'_'+mun).val()+' / '+jQuery('#mundescricao_pl_'+cpf+'_'+mun).val();
	
	cpf = mascaraglobal('###.###.###-##',cpf);
	
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
    
    if(muncod=='') {
    	alert('Município em branco');
    	return false;
    }
    
    var conf = confirm('Termo de Veracidade das Informações\n\nVocê assegura que este(a) professor(a) preenche os requisitos abaixo?\n\n-É servidor(a) efetivo(a) rede de ensino\n-Possui formação em Licenciatura ou Pedagogia\n-Possui experiência na formação de professores alfabetizados\n-Não recebe bolsa de estudo de outro programa federal de formação\n-Possui disponibilidade para atuar como Orientador(a) de estudo');
    
    if(!conf) {
    	return false;
    }
	
	var resultado = inserirOrientadorEstudoLabel(cpf,nome,email,"tutoresredesemproletramento",muncod,mundescricao);
	
	divCarregando();
	
	if(resultado) {
		tabela.rows[linha.rowIndex-1].cells[0].innerHTML = "<img src=../imagens/gif_inclui_d.gif>";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].className = "disabled";
		tabela.rows[linha.rowIndex-1].cells[3].childNodes[0].readOnly=true;
	}
	
	divCarregado();

}

function carregarMundescricao(vlr) {
	jQuery('#mundescricao').val(jQuery("#muncod_endereco option:selected").text());
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
switch($_REQUEST['navegacao']) :
case 'professorsispacto2014':
	?>
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
	</script>
	<form method="post" name="formulario" id="formulario">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2"><?=$_TIPO_ORIENTADORES[$_REQUEST['navegacao']] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
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
		if(!isset($_REQUEST['uf'])) $_REQUEST['uf'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf'];
		$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
		$db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município</td>
		<td id="td_municipio3">
		<? 
		if($_REQUEST['uf']) :
			if(!isset($_REQUEST['muncod_endereco'])) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'];
			$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['uf']."' ORDER BY mundescricao"; 
			$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
		else: 
			echo "Selecione uma UF";
		endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Voltar" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></td>
	</tr>
	</table>
	</form>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2">
		<?
		if($_REQUEST['cpf']) {
			$f[] = "cpf='".str_replace(array(".","-"),array(""),$_REQUEST['cpf'])."'";
		}
		if($_REQUEST['nome']) {
			$f[] = "removeacento(nome) ilike '%'||removeacento('".$_REQUEST['nome']."')||'%'";
		}
		if($_REQUEST['uf']) {
			$f[] = "m.estuf='".$_REQUEST['uf']."'";
		}
		if($_REQUEST['muncod_endereco']) {
			$f[] = "m.muncod='".$_REQUEST['muncod_endereco']."'";
		}
		
		$sql = "SELECT '<img id=\"a_img_'||cpf||'_'||m.muncod||'\" src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirProfessoresSispacto2014(this,\''||m.muncod||'\');\" ><input type=\"hidden\" name=\"uf['||cpf||']\" id=\"uf_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.estuf||'\"><input type=\"hidden\" name=\"muncod['||cpf||']\" id=\"muncod_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.muncod||'\"><input type=\"hidden\" name=\"mundescricao['||cpf||']\" id=\"mundescricao_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.mundescricao||'\">' as acao, cpf, nome, '<input class=normal id=\"email_img_'||cpf||'_'||m.muncod||'\" type=text name=email['||cpf||'] size=30 value=\"'||coalesce(email,'')||'\">' as email, m.estuf||' / '||m.mundescricao as municipio 
				FROM (
				
			SELECT 
			c.cerfrequencia as nota, i.iuscpf as cpf, i.iusnome as nome, i.iusemailprincipal as email, i.muncodatuacao as muncod FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_PROFESSORALFABETIZADOR2014." 
			INNER JOIN sispacto2.certificacao c ON c.iusd = i.iusd AND c.pflcod = t.pflcod  
			LEFT JOIN sispacto3.identificacaousuario i2 ON i2.iuscpf = i.iuscpf 
			LEFT JOIN sispacto3.tipoperfil t2 ON t2.iusd = i2.iusd 
			WHERE t2.tpeid IS NULL
			ORDER BY i.iusnome
   					
				) foo
				LEFT JOIN territorios.municipio m ON m.muncod = foo.muncod   
				WHERE foo.nota >= 75 ".(($f)?" AND ".implode(" AND ",$f):"")." 
				GROUP BY cpf, nome, email, m.estuf, m.muncod, m.mundescricao
				ORDER BY nome";
		
		$cabecalho = array("&nbsp;","CPF","Nome","Email","Município");
		$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
		?>	
		</td>
	</tr>
	</table>
	<?
	break;	
case 'tutoresproletramento':
?>
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
</script>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2"><?=$_TIPO_ORIENTADORES[$_REQUEST['navegacao']] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
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
	if(!isset($_REQUEST['uf'])) $_REQUEST['uf'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf'];
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<? 
	if($_REQUEST['uf']) :
		if(!isset($_REQUEST['muncod_endereco'])) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'];
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['uf']."' ORDER BY mundescricao"; 
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	else: 
		echo "Selecione uma UF";
	endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Voltar" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></td>
</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2">
	<?
	if($_REQUEST['cpf']) {
		$f[] = "cpf='".str_replace(array(".","-"),array(""),$_REQUEST['cpf'])."'";
	}
	if($_REQUEST['nome']) {
		$f[] = "removeacento(nome)=removeacento('".$_REQUEST['nome']."')";
	}
	if($_REQUEST['uf']) {
		$f[] = "uf='".$_REQUEST['uf']."'";
	}
	if($_REQUEST['muncod_endereco']) {
		$f[] = "p.muncod='".$_REQUEST['muncod_endereco']."'";
	}
	
	$sql = "SELECT '<img id=\"a_img_'||cpf||'_'||m.muncod||'\" src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirTutoresProletramento(this,\''||m.muncod||'\');\" ><input type=\"hidden\" name=\"uf['||cpf||']\" id=\"uf_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.estuf||'\"><input type=\"hidden\" name=\"muncod['||cpf||']\" id=\"muncod_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.muncod||'\"><input type=\"hidden\" name=\"mundescricao['||cpf||']\" id=\"mundescricao_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.mundescricao||'\">' as acao, cpf, nome, '<input class=normal id=\"email_img_'||cpf||'_'||m.muncod||'\" type=text name=email['||cpf||'] size=30>' as email, m.estuf||' / '||m.mundescricao as municipio 
			FROM sispacto3.tutoresproletramento p
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod   
			WHERE tupstatus='A'".(($f)?" AND ".implode(" AND ",$f):"")." 
			GROUP BY cpf, nome, m.estuf, m.muncod, m.mundescricao
			ORDER BY nome";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Município");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
	?>	
	</td>
</tr>
</table>
<?
break;
case 'tutoresredesemproletramento':
?>
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
</script>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2"><?=$_TIPO_ORIENTADORES[$_REQUEST['navegacao']] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
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
	$_REQUEST['uf'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf'];
	echo "<input type=\"hidden\" name=\"uf\" id=\"uf\" value=\"".$_REQUEST['uf']."\">".$_REQUEST['uf'];
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<? 
	$_REQUEST['muncod_endereco'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'];
	$mundescricao = $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod_endereco']."'");
	echo "<input type=\"hidden\" name=\"mundescricao\" id=\"mundescricao\" value=\"".$mundescricao."\"><input type=\"hidden\" name=\"muncod_endereco\" id=\"muncod_endereco\" value=\"".$_REQUEST['muncod_endereco']."\">".$mundescricao; 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Voltar" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></td>
</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2">
	<?
	if($_REQUEST['cpf']) {
		$f[] = "cpf='".str_replace(array(".","-"),array(""),$_REQUEST['cpf'])."'";
	}
	if($_REQUEST['nome']) {
		$f[] = "removeacento(docente)=removeacento('".$_REQUEST['nome']."')";
	}
	if($_REQUEST['uf']) {
		$f[] = "uf='".$_REQUEST['uf']."'";
	}
	if($_REQUEST['muncod_endereco']) {
		$f[] = "t.muncod='".$_REQUEST['muncod_endereco']."'";
	}
	
	$sql = "SELECT '<img id=\"a_img_'||cpf||'_'||m.muncod||'\" src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirTutoresSemProletramento(this,\''||m.muncod||'\');\" ><input type=\"hidden\" name=\"uf['||cpf||']\" id=\"uf_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.estuf||'\"><input type=\"hidden\" name=\"muncod['||cpf||']\" id=\"muncod_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.muncod||'\"><input type=\"hidden\" name=\"mundescricao['||cpf||']\" id=\"mundescricao_pl_'||cpf||'_'||m.muncod||'\" value=\"'||m.mundescricao||'\">' as acao, cpf, docente, '<input class=normal id=\"email_img_'||cpf||'_'||m.muncod||'\" type=text name=email['||cpf||'] size=30 value=\"'||email||'\">' as email, m.estuf||' / '||m.mundescricao as municipio 
			FROM sispacto3.tutoressemproletramento t 
			LEFT JOIN territorios.municipio m ON m.muncod = t.muncod
			WHERE tspstatus='A'".(($f)?" AND ".implode(" AND ",$f):"")."
			GROUP BY cpf, docente, email, m.estuf, m.muncod, m.mundescricao
			ORDER BY docente";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Município");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
	?>	
	</td>
</tr>
</table>
<?
break;
case 'profissionaismagisterio':
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2"><?=$_TIPO_ORIENTADORES[$_REQUEST['navegacao']] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
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
	<td class="SubTituloDireita">UF</td>
	<td><?
	$_REQUEST['uf'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf'];
	echo "<input type=\"hidden\" name=\"uf\" id=\"uf\" value=\"".$_REQUEST['uf']."\">".$_REQUEST['uf'];
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<?
	if($_SESSION['sispacto3']['esfera']=='municipal') { 
		$_REQUEST['muncod_endereco'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'];
		$mundescricao = $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod_endereco']."'");
		echo "<input type=\"hidden\" name=\"mundescricao\" id=\"mundescricao\" value=\"".$mundescricao."\"><input type=\"hidden\" name=\"muncod_endereco\" id=\"muncod_endereco\" value=\"".$_REQUEST['muncod_endereco']."\">".$mundescricao;
	} else {
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf']."' ORDER BY mundescricao";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', 'carregarMundescricao', '', '', '', 'S', 'muncod_endereco', '');
		echo "<input type=\"hidden\" name=\"mundescricao\" id=\"mundescricao\" value=\"\">";
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="inserirProfissionalMagisterio();"> <input type="button" value="Voltar" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></td>
</tr>
</table>
<?
break;
default:
	$sql = "SELECT picmostrarbotaotutoresredesemproletramento, picmostrarbotaoprofissionaismagisterio, picmostrarbotaotutoresproletramento FROM sispacto3.pactoidadecerta WHERE picid='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid']."' AND picstatus='A'";
	$pactoidadecerta = $db->pegaLinha($sql);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro">Inserir Orientadores de Estudo</td>
</tr>
<tr>
	<td>
	<p><b>- Professor Alfabetizador do Pacto 2014 recomendado para certificação:</b></p>
	<p>Priorize o preenchimento das vagas disponíveis com Professores Alfabetizadores do Pacto 2014 recomendado para certificação. Ao clicar no botão "Professores Alfabetizadores do Pacto 2014", serão exibidos os nomes dos professores cujo endereço informado no Sistema Geral de Bolsas indicava este município.</p>
	<p align="center"><input type="button" name="professoresalfabetizadores" value="Professores Alfabetizadores do Pacto 2014" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A&navegacao=professorsispacto2014';"><br><input type="button" value="Não desejo cadastrar Professores Alfabetizadores do Pacto 2014 recomendados para certificação" onclick="atualizaBotaoOrientadorEstudo('professorsispacto2014');window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></p>
	<p>Em caso de dúvida, consulte o Manual de Orientações do sispacto 2014.</p>
	</td>
</tr>
<? if($pactoidadecerta['picmostrarbotaotutoresproletramento']=="t") : ?>
<tr>
	<td>
	<p><b>- Tutores do Pró-Letramento:</b></p>
	<p>Priorize o preenchimento das vagas disponíveis com profissionais da sua rede que foram tutores do Pró-Letramento. Ao clicar no botão "Tutores do Pró-Letramento", serão exibidos os nomes dos tutores cujo endereço informado no Sistema Geral de Bolsas indicava este município.</p>
	<p align="center"><input type="button" name="tutoresproletramento" value="Tutores do Pró-Letramento" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A&navegacao=tutoresproletramento';"><br><input type="button" value="Não desejo cadastrar Tutores do Pró-Letramento" onclick="atualizaBotaoOrientadorEstudo('tutoresproletramento');window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></p>
	<p>Em caso de dúvida, consulte o Manual de Orientações do sispacto 2014.</p>
	</td>
</tr>
<? endif; ?>
<? if($pactoidadecerta['picmostrarbotaotutoresredesemproletramento']=="t") : ?>
<tr>
	<td>
	<p><b>-Professores que não foram Tutores do Pró-Letramento:</b></p>
	<p>Caso a sua rede de ensino não possua profissionais que foram tutores do Pró-Letramento ou estes não possam atuar como Orientadores de Estudo do Pacto, clique no botão abaixo e preencha ou complete as vagas disponíveis com professores da sua rede (informados no Censo 2013) com experiência na formação de alfabetizadores e que atendam aos requisitos exigidos.</p>
	<p align="center"><input type="button" name="tutoresredesemproletramento" value="Professores da rede que não foram Tutores do Pró-Letramento" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A&navegacao=tutoresredesemproletramento';"><br><input type="button" value="Não desejo cadastrar Professores que não foram Tutores do Pró-Letramento" onclick="atualizaBotaoOrientadorEstudo('tutoresredesemproletramento');window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A';"></p>
	<p>Em caso de dúvida, consulte o Manual de Orientações do sispacto 2014.</p>
	</td>
</tr>
<? endif; ?>
<? if($pactoidadecerta['picmostrarbotaoprofissionaismagisterio']=="t") : ?>
<tr>
	<td>
	<p><b>- Profissionais do Magistério com experiência em formação:</b></p>
	<p>Se a sua rede de ensino não possui profissionais que foram tutores do Pró-Letramento nem professores com experiência na formação de alfabetizadores, preencha ou complete as vagas disponíveis com profissionais do magistério, vinculados à sua rede, que preencham os requisitos exigidos.</p>
	<p align="center"><input type="button" name="profissionaismagisterio" value="Profissionais do Magistério com experiência em formação de professores" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A&navegacao=profissionaismagisterio';"></p>	
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloCentro"><input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
</table>
<?
endswitch;
?>