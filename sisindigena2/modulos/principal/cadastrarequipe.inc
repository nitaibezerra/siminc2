<? verificarTermoCompromisso(array("iusd"=>$_SESSION['sisindigena2'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_gerenciador)); ?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

function exibirHistorioUsuario(iuscpf) {
	ajaxatualizar('requisicao=carregarHistoricoUsuario&usucpf='+iuscpf,'modalHistoricoUsuario');
	
	jQuery("#modalHistoricoUsuario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function enviarMarcado(suscod) {
	var tchk = jQuery("[name^='chk[']:enabled:checked").length;
	
	if(tchk==0) {
		alert('Selecione os Usuários para enviar');
		return false;
	}
	
	var sit = new Array();
	sit['A'] = 'Ativar';
	sit['B'] = 'Bloquear';
	conf = confirm('Deseja realmente '+sit[suscod]+' todos os usuários marcados?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "suscod");
		input.setAttribute("value", suscod);
		document.getElementById("formulario").appendChild(input);

	
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "requisicao");
		input.setAttribute("value", "ativarEquipe");
		document.getElementById("formulario").appendChild(input);
		
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "uncid");
		input.setAttribute("value", "<?=$_SESSION['sispacto'][$sis]['uncid'] ?>");
		document.getElementById("formulario").appendChild(input);
		
		document.getElementById('formulario').submit();
	}
}

function reiniciarSenha(cpf) {
	var conf = confirm('Deseja realmente ativar e reiniciar a senha deste usuário para "simecdti"?');
	
	if(conf) {
		window.location=window.location+'&requisicao=reiniciarSenha&usucpf='+cpf;
	
	}
}


function inserirUsuarioPerfil(pflcod) {

	jQuery('#pflcod__').val(pflcod);

	jQuery("#modalFormulario2").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function carregaUsuario_(esp) {
	var usucpf=document.getElementById('iuscpf'+esp).value;

	usucpf = usucpf.replace('-','');
	usucpf = usucpf.replace('.','');
	usucpf = usucpf.replace('.','');
				
	var comp = new dCPF();
	comp.buscarDados(usucpf);
	var arrDados = new Object();
	
	if(!comp.dados.no_pessoa_rf){
		alert('CPF Inválido');
		return false;
	}
	
	document.getElementById('iusnome'+esp).value=comp.dados.no_pessoa_rf;
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=pegarDadosUsuarioPorCPF&cpf='+usucpf,
   		async: false,
   		success: function(dados){
   			var da = dados.split("||");
			document.getElementById('iusemailprincipal'+esp).value = da[0];
   		}
	});
	
	divCarregado();
}


function carregarDetalhesPerfil(pflcod) {
	ajaxatualizar('requisicao=carregarDetalhesPerfil&uncid=<?=$_SESSION['sispacto'][$sis]['uncid'] ?>&pflcod_='+pflcod,'detalhesperfil');
}


function efetuarInsercaoUsuarioPerfil() {

	jQuery('#iuscpf__').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf__').val()));
	
	if(jQuery('#iuscpf__').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#iuscpf__').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#iusnome__').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#iusemailprincipal__').val()=='') {
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#iusemailprincipal__').val())) {
    	alert('Email inválido');
    	return false;
    }
    
    if(document.getElementById('picid__')) {
		if(jQuery('#picid__').val()=='') {
			alert('Esfera em branco');
			return false;
		}
    }
    
    if(document.getElementById('turid__')) {
		if(jQuery('#turid__').val()=='') {
			alert('Turma em branco');
			return false;
		}
    }

    
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "uncid");
	input.setAttribute("value", "<?=$_SESSION['sispacto'][$sis]['uncid'] ?>");
	document.getElementById("formulario_insercao").appendChild(input);

	document.getElementById('formulario_insercao').submit();

}


function excluirUsuarioPerfil(pflcod,iusd) {
	var conf = confirm('Deseja realmente excluir o este cargo? \n\n- Essa ação será definitiva e não poderá ser incluído um novo membro.\n- Caso este ja tenha recebido algum pagamento, o sistema não permitirá a exclusão\n- Ícone ao lado é uma ferramenta de substituição, tenha certeza de que ela não é a ferramenta que esta necessitando.');
	if(conf) {
		window.location='<?=$_SERVER['REQUEST_URI'] ?>&requisicao=excluirUsuarioPerfil&pflcod='+pflcod+'&iusd='+iusd;
	}
}

function marcarTodos(obj) {
	jQuery("[name^='chk[']").attr('checked',obj.checked);
}

function atualizarEmail(iusd,email) {
	var fname=prompt("Digite novo e-mail:",email);
	if(fname) {
		if(fname=='') {
			alert('Email Principal em branco');
			return false;
		}
	    if(!validaEmail(fname)) {
	    	alert('Email Principal inválido');
	    	return false;
	    }
		ajaxatualizar('requisicao=atualizarEmail&iusd='+iusd+'&iusemailprincipal='+fname,'');
		
		document.getElementById('formulario').submit();
	}
}

</script>

<div id="modalHistoricoUsuario" style="display:none;">

</div>

<div id="modalFormulario2" style="display:none;">
<form method="post" name="formulario_insercao" id="formulario_insercao">
<input type="hidden" name="requisicao" value="efetuarInsercaoUsuarioPerfil">
<input type="hidden" name="pflcod__" id="pflcod__">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<p>Esta é uma ferramenta de inserção de usuários direta, sem aprovações ou fluxos. Todas alterações são registradas no projeto, logando as informações de quem será substituido, do novo membro e do responsável pela mudança.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?=campo_texto('iuscpf__', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf__"', '', '', 'if(this.value!=\'\'){carregaUsuario_(\'__\');}'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Nome</td>
	<td><?=campo_texto('iusnome__', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome__"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Email</td>
	<td><?=campo_texto('iusemailprincipal__', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal__"'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" value="Salvar" onclick="efetuarInsercaoUsuarioPerfil();">
	</td>
</tr>
</table>
</form>
</div>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Gerenciar Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto/sispacto.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':(($sis=='coordenadorlocal')?'coordenadorlocalexecucao':$sis))."&acao=A&aba=gerenciarusuario"); ?></td>
</tr>
<tr>
	<td colspan="2" valign="top">
	<p><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Pesquisadores</b></p>
	<input type="button" name="inserir" value="Inserir Pesquisador" onclick="inserirUsuarioPerfil();">
	
	<p><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Conteudistas</b></p>
	<input type="button" name="inserir" value="Inserir Conteudista" onclick="inserirUsuarioPerfil();">

	<p><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Formadores</b></p>
	<input type="button" name="inserir" value="Inserir Formador" onclick="inserirUsuarioPerfil('<?=PFL_FORMADORIES ?>');">


	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<? if($goto_pro) : ?>
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	<? endif; ?>
	</td>
</tr>
</table>