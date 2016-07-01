<?

ini_set("memory_limit", "2048M");


$sql = "SELECT * FROM seguranca.usuario u 
		LEFT JOIN territorios.municipio m ON m.muncod = u.muncod 
		WHERE usucpf='".$_SESSION['sismedio']['gestorescola']['usucpf']."'";

$gestorescola = $db->pegaLinha($sql);

extract($gestorescola);

if(!$consulta) $consulta = verificaPermissao();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>

jQuery(document).ready(function() {
  jQuery(window).keydown(function(event){
    if(event.keyCode == 13) {
      event.preventDefault();
      return false;
    }
  });
});

function salvarDados(goto) {
		  
	jQuery('#usucpf').val(mascaraglobal('###.###.###-##',jQuery('#usucpf').val()));
	
	if(jQuery('#usucpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#usucpf').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#usunome').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#usudatanascimento').val()=='') {
		alert('Data de Nascimento em branco');
		return false;
	}
	
	if(!validaData(document.getElementById('usudatanascimento'))) {
		alert('Data de Nascimento inválida');
		return false;
	}
	
	var iussexo_r = jQuery("[name^='ususexo']:enabled:checked").length;
	
	if(iussexo_r==0) {
		alert('Sexo em branco');
		return false;
	}

	if(jQuery('#estuf').val()=='') {
		alert('Estado em branco');
		return false;
	}
	
	if(document.getElementById('muncod')) {
		if(jQuery('#muncod').val()=='') {
			alert('Município em branco');
			return false;
		}
	} else {
		alert('Município em branco');
		return false;
	}
	
	if(jQuery('#usufoneddd').val()=='') {
		alert('DDD Trabalho em branco');
		return false;
	}
	
	if(jQuery('#usufoneddd').val().length!=2) {
		alert('DDD Trabalho deve possuir 2 dígitos');
		return false;
	}
	
	if(jQuery('#usufonenum').val()=='') {
		alert('Telefone Trabalho em branco');
		return false;
	}
	
	if(jQuery('#usufonenum').val().length!=9) {
		alert('Telefone Trabalho deve possuir 8 dígitos');
		return false;
	}
	
	if(jQuery('#usuemail').val()=='') {
		alert('Email Principal em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#usuemail').val())) {
    	alert('Email Principal inválido');
    	return false;
    }

    divCarregando();
    
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}


<? if($consulta) : ?>
jQuery(function() {
jQuery("[type='text']").attr('class','disabled');
jQuery("[type='text']").attr('disabled','disabled');
jQuery("[type='radio']").attr('disabled','disabled');
jQuery("[name='muncod'],[name='estuf']").attr('disabled','disabled');
jQuery("[name='salvar'],[name='salvarcontinuar'],[name='tdoid'],[name='itdufdoc'],[name='eciid'],[name='nacid'],[name='muncod_nascimento'],[name='estuf_nascimento'],[name='iusagenciaend'],[name='tvpid'],[name='funid'],[name='foeid'],[name='iusnaodesejosubstituirbolsa']").css('display','none');
});
<? endif; ?>

</script>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="mensagemalert" value="<?=$mensagemalert ?>">
<input type="hidden" name="requisicao" id="requisicao" value="atualizarDadosGestorEscola">
<input type="hidden" name="goto" id="goto" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Dados</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($abaativa); ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados Pessoais</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">CPF</td>
	<td><?=campo_texto('usucpf', "S", "N", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="usucpf"', '', mascaraglobal($usucpf,"###.###.###-##")); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome</td>
	<td><?=campo_texto('usunome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="usunome"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Data de Nascimento</td>
	<td><?=campo_data2('usudatanascimento','S', (($consulta)?'N':'S'), 'Data nascimento', 'S', '', '', '', '', '', 'usudatanascimento'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Estado</td>
	<td>
	<? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFGestor', '', '', '', 'S', 'estuf', ''); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Município:</td>
	<td id="td_municipio">
	<? 
	if($estuf) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf."' ORDER BY mundescricao";
		$db->monta_combo('muncod', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod', '');
	else :
		echo "Selecione uma UF";
	endif;
	?> 
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">Sexo</td>
	<td><input type="radio" name="ususexo" value="M" <?=(($ususexo=="M")?"checked":"") ?>> Masculino <input type="radio" name="ususexo" value="F" <?=(($ususexo=="F")?"checked":"") ?>> Feminino</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" colspan="2">Contatos</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Telefone</td>
	<td><?=campo_texto('usufoneddd', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="usufoneddd"', '' ); ?> <?=campo_texto('usufonenum', "S", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="usufonenum"', '', trim($usufonenum) ); ?></td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">E-mail</td>
	<td><?=campo_texto('usuemail', "S", "S", "Email", 60, 60, "", "", '', '', 0, 'id="usuemail"'); ?></td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=principal';">
	<input type="button" name="salvar" value="Salvar" onclick="salvarDados('sismedio.php?modulo=principal/escola/escola&acao=A&aba=dadosgestor');">
	<input type="button" name="salvarcontinuar" value="Salvar e Continuar" onclick="salvarDados('sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirprofessores');">
	<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirprofessores';">
	</td>
</tr>
</table>
</form>