<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

<? if($consulta) : ?>
jQuery(document).ready(function() {
	jQuery("[name^='chk[']").css('display','none');
	jQuery("[src='../imagens/icon_campus_1.png']").css('display','none');
	jQuery("[src='../imagens/refresh.gif']").css('display','none');
	jQuery("[src='../imagens/excluir.gif']").css('display','none');

	jQuery("#marcartodos").css('display','none');
	
	jQuery("#ativarmarcados").css('display','none');
	jQuery("#bloquearmarcados").css('display','none');

});
<? endif; ?>


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

function trocarUsuarioPerfil(pflcod, iusd) {

	jQuery('#iusdantigo').val(iusd);
	jQuery('#pflcod_').val(pflcod);

	jQuery("#modalFormulario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function inserirUsuarioPerfil() {

	jQuery("#modalFormularioCriacao").dialog({
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

function efetuarTrocaUsuarioPerfil() {

	jQuery('#iuscpf_').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf_').val()));
	
	if(jQuery('#iuscpf_').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#iuscpf_').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#iusnome_').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#iusemailprincipal_').val()=='') {
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#iusemailprincipal_').val())) {
    	alert('Email inválido');
    	return false;
    }

	document.getElementById('formulario_troca').submit();

}

function carregarDetalhesPerfil(pflcod) {
	ajaxatualizar('requisicao=carregarDetalhesPerfil&pflcod_='+pflcod,'detalhesperfil');
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

function atualizarMunicipioAtuacao(iusd,muncod) {
	if(muncod=='') {
		alert('Selecione um Município de atuação');
		return false;	
	}
	
	ajaxatualizar('requisicao=atualizarMunicipioAtuacao&iusd='+iusd+'&muncod='+muncod,'');
		
	document.getElementById('formulario').submit();

}


function exibirMunicipiosAtuacao(iuscpf) {
	ajaxatualizar('requisicao=exibirMunicipiosAtuacao&iuscpf='+iuscpf,'modalHistoricoUsuario');
	
	jQuery("#modalHistoricoUsuario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}



</script>

<div id="modalHistoricoUsuario" style="display:none;"></div>


<!-- FORMULARIO DE CRIAÇÃO DE BOLSISTAS -->
<div id="modalFormularioCriacao" style="display:none;">

<form method="post" name="formulario_insercao" id="formulario_insercao">

<input type="hidden" name="requisicao" value="efetuarInsercaoUsuarioPerfil">

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
	<td class="SubTituloDireita" width="25%">Perfil</td>
	<td>
	<?
	
	$sql = "SELECT p.pflcod as codigo, 
				   p.pfldsc as descricao 
			FROM seguranca.perfil p 
			WHERE pflcod IN('".PFL_COORDENADOR_INSTITUCIONAL."')
			ORDER BY p.pfldsc";
	
	$db->monta_combo('pflcod__', $sql, 'S', 'Selecione', 'carregarDetalhesPerfil', '', '', '', 'N', 'pflcod__','', $_REQUEST['pflcod__']);
	
	?>
	</td>
</tr>
<tr>
	<td colspan="2" id="detalhesperfil"></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" value="Salvar" onclick="efetuarInsercaoUsuarioPerfil();">
	</td>
</tr>
</table>
</form>

</div>

<!-- FORMULARIO DE TROCA DE BOLSISTAS -->
<div id="modalFormularioTroca" style="display:none;">

<form method="post" name="formulario_troca" id="formulario_troca">
<input type="hidden" name="requisicao" value="efetuarTrocaUsuarioPerfil">
<input type="hidden" name="iusdantigo" id="iusdantigo" value="">
<input type="hidden" name="pflcod_" id="pflcod_" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<p>Esta é uma ferramenta de troca de usuários direta, sem aprovações ou fluxos. Todas alterações são registradas no projeto, logando as informações de quem será substituido, do novo membro e do responsável pela mudança.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?=campo_texto('iuscpf_', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf_"', '', '', 'if(this.value!=\'\'){carregaUsuario_(\'_\');}'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Nome</td>
	<td><?=campo_texto('iusnome_', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome_"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Email</td>
	<td><?=campo_texto('iusemailprincipal_', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal_"'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" value="Salvar" onclick="efetuarTrocaUsuarioPerfil();">
	</td>
</tr>
</table>
</form>

</div>





<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto/sispacto.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':(($sis=='coordenadorlocal')?'coordenadorlocalexecucao':$sis))."&acao=A&aba=gerenciarusuario"); ?></td>
</tr>
<tr>
	<td colspan="2" valign="top">
	
	<form method=post name="formbuscar" id="formbuscar">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td class="SubTituloDireita">CPF</td>
    	<td><?=campo_texto('iuscpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iuscpf']); ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Nome</td>
    	<td><?=campo_texto('iusnome', "N", "S", "Nome", 67, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Perfil</td>
    	<td>
    	<? 
    	$sql = "SELECT pfldsc as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflcod IN('".PFL_COORDENADOR_INSTITUCIONAL."')";
    	$db->monta_combo('pfldsc', $sql, 'S', 'Selecione', 'selecionarPerfilGerenciar', '', '', '200', 'S', 'fpbid','', $_REQUEST['pfldsc']); 
    	?> 
    	</td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Situação</td>
    	<td>
    	<? 
		$arrSituacao = array(0 => array("codigo"=>"A","descricao"=>"Ativo"),
	    					 1 => array("codigo"=>"B","descricao"=>"Bloqueado"),
	    					 2 => array("codigo"=>"P","descricao"=>"Pendente"),
	    					 3 => array("codigo"=>"N","descricao"=>"Não Cadastrado")
	    					 );
	    $db->monta_combo('status', $arrSituacao, 'S', 'Selecione', '', '', '', '200', 'S', 'status','', $_REQUEST['status']); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">UF</td>
		<td>
		<?
		$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
		$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município</td>
		<td id="td_municipio3">
		<? 
		if($muncod_endereco) : 
			$sql = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod='".$muncod_endereco."'";
			$mundescricao = $db->pegaUm($sql);
			echo $mundescricao;
			echo '<input type="hidden" id="muncod_endereco" name="muncod_endereco" value="'.$muncod_endereco.'">';
		else :
			echo "Selecione UF";
		endif;
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();">
		<input type="button" name="vertodos" value="Ver todos" onclick="window.location=window.location;">
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">
		<input type="checkbox" id="marcartodos" onclick="marcarTodos(this);"> Marcar todos
		</td>
		<td class="SubTituloEsquerda">
		<input type="button" name="incluir" value="Incluir Bolsista" onclick="inserirUsuarioPerfil();">
		</td>
	</tr>
	</table>
	
	</form>
	
	<?
	$perfis = pegaPerfilGeral();
	
	if($_REQUEST['iuscpf']) {
		$where[] = "foo.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
	}
	
	if($_REQUEST['pfldsc']) {
		$where[] = "foo.pfldsc='".$_REQUEST['pfldsc']."'";
	}
	
	if($_REQUEST['estuf_endereco']) {
		$rede .= $_REQUEST['estuf_endereco']." / ";
	}
	
	if($_REQUEST['muncod_endereco']) {
		$rede .= $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod_endereco']."'");
	}
	
	if($rede) {
		$where[] = "foo.rede ilike '%".$rede."%'";
	}
	
	if($_REQUEST['iusnome']) {
		$where[] = "foo.iusnome ilike '%".$_REQUEST['iusnome']."%'";
	}
	
	if($_REQUEST['status']) {
		if($_REQUEST['status']=="N") {
			$where[] = "(foo.status IS NULL OR foo.perfil IS NULL)";
		} else {
			$where[] = "(foo.status='".$_REQUEST['status']."' OR usu.suscod='".$_REQUEST['status']."')";	
		}
	}
	
	if($db->testa_superuser()) {
		
		$iconsubstituir = "<img src=\"../imagens/refresh.gif\" style=\"cursor:pointer;\" onclick=\"trocarUsuarioPerfil(\''||foo.pflcod||'\',\''||foo.iusid||'\');\" onmouseover=\"return escape(\'Efetuar troca de usuário\');\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirUsuarioPerfil(\''||foo.pflcod||'\',\''||foo.iusid||'\');\" onmouseover=\"return escape(\'Excluir usuário\');\"> ";
		
	}
	
	$sql = "SELECT '<input type=\"checkbox\" name=\"chk['||foo.pflcod||'][]\" value=\"'||foo.iuscpf||'\"> {$iconsubstituir}'|| CASE WHEN foo.status IS NOT NULL AND foo.perfil IS NOT NULL THEN '<img src=\"../imagens/icon_campus_1.png\" border=\"0\" style=\"cursor:pointer;\" onclick=\"reiniciarSenha(\''||foo.iuscpf||'\');\" onmouseover=\"return escape(\'Ativar usuário e reiniciar senha\');\">' ELSE '' END as acao, 
				    CASE WHEN foo.status='A' AND foo.perfil IS NOT NULL AND usu.suscod='A' THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"exibirHistorioUsuario(\''||foo.iuscpf||'\');\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"exibirHistorioUsuario(\''||foo.iuscpf||'\');\">' END ||'
				   '|| CASE WHEN foo.status IS NULL OR foo.perfil IS NULL THEN 'Não Cadastrado'
				   		WHEN (foo.status='A' AND usu.suscod='A') THEN 'Ativo'
				   		WHEN (foo.status='B' OR usu.suscod='B')	 THEN 'Bloqueado' 
				   		WHEN (foo.status='P' OR usu.suscod='P')  THEN 'Pendente' END as situacao,
				   replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
				   foo.iusnome, 
				   foo.iusemailprincipal || ' <img src=../imagens/arrow_v.png style=cursor:pointer; align=absmiddle onclick=\"atualizarEmail('||foo.iusid||',\''||foo.iusemailprincipal||'\');\">' as iusemailprincipal, 
				   foo.pfldsc,
				   foo.instituicao
			FROM (
			(
			
			{$sql_equipe}
			
			)
			 
			) foo 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = foo.iuscpf 
			".(($where)?" WHERE ".implode(" AND ",$where):"")." ORDER BY foo.iusnome";

	$cabecalho = array("&nbsp;","Situação","CPF","Nome","E-mail","Perfil","Instituição");
	$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));
	?>
	</td>
</tr>
<tr>
	<td colspan="2" align="center">
	<input type="button" id="ativarmarcados" value="Ativar Marcados" onclick="enviarMarcado('A');">
	<input type="button" id="bloquearmarcados" value="Bloquear Marcados" onclick="enviarMarcado('B');">
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