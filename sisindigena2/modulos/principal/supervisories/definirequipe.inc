<?
$estado = wf_pegarEstadoAtual( $_SESSION['sisindigena2']['supervisories']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

?>
<? verificarTermoCompromisso(array("iusd"=>$_SESSION['sisindigena2']['supervisories']['iusd'],"sis"=>'supervisories',"pflcod"=>PFL_SUPERVISORIES)); ?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

function inserirUsuarioPerfil(pflcod) {

	jQuery('#tr_sup').css('display','none');
	jQuery('#td_label_sup').html('');
	jQuery('#td_name_sup').html('');
	
	
	if(pflcod=='<?=PFL_ORIENTADORESTUDO ?>') {
		jQuery('#tr_sup').css('display','');
		jQuery('#td_label_sup').html('Supervisor:');
		jQuery('#td_name_sup').html('<input type="hidden" name="iusdsup" id="iusdsup" value="<?=$_SESSION['sisindigena2']['supervisories']['iusd'] ?>"> <?=$_SESSION['sisindigena2']['supervisories']['iusnome'] ?>');
	}
	
	if(pflcod=='<?=PFL_PROFESSORALFABETIZADOR ?>') {
		jQuery('#tr_sup').css('display','');
		jQuery('#td_label_sup').html('Orientador:');
		ajaxatualizar('requisicao=carregarUsuarioPerfil&iusd=<?=$_SESSION['sisindigena2']['supervisories']['iusd'] ?>&picid=<?=$_SESSION['sisindigena2']['supervisories']['picid'] ?>&pflcod=<?=PFL_ORIENTADORESTUDO ?>','td_name_sup');
	}

	if(pflcod=='<?=PFL_COORDENADORLOCAL ?>') {
		jQuery('#tr_sup').css('display','');
		jQuery('#td_label_sup').html('Rede:');
		ajaxatualizar('requisicao=carregarRedeTerritorio','td_name_sup');
	}


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

function excluirMembroEquipe(iusd, pflcod) {
	var conf = confirm('Deseja realmente excluir?');
	
	if(conf) {
		window.location='sisindigena2.php?modulo=principal/supervisories/supervisories&acao=A&aba=definirequipe&requisicao=removerTipoPerfil&iusd='+iusd+'&pflcod='+pflcod;
	}

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
    
    if(document.getElementById('retid')) {
		if(jQuery('#retid').val()=='') {
			alert('Rede em branco');
			return false;
		}
    }
    
    if(document.getElementById('iusdsup')) {
		if(jQuery('#iusdsup').val()=='') {
			alert('Turma em branco');
			return false;
		}
    }

    
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "picid__");
	input.setAttribute("value", "<?=$_SESSION['sisindigena2']['supervisories']['picid'] ?>");
	document.getElementById("formulario_insercao").appendChild(input);

	document.getElementById('formulario_insercao').submit();

}


function excluirUsuarioPerfil(pflcod,iusd) {
	var conf = confirm('Deseja realmente excluir o este cargo? \n\n- Essa ação será definitiva e não poderá ser incluído um novo membro.\n- Caso este ja tenha recebido algum pagamento, o sistema não permitirá a exclusão\n- Ícone ao lado é uma ferramenta de substituição, tenha certeza de que ela não é a ferramenta que esta necessitando.');
	if(conf) {
		window.location='<?=$_SERVER['REQUEST_URI'] ?>&requisicao=excluirUsuarioPerfil&pflcod='+pflcod+'&iusd='+iusd;
	}
}

function importarEquipeSUSisindigena2014() {
	var conf = confirm('Deseja realmente importar a "Equipe" do SISIndígena 2014?');
	if(conf) {
		window.location='sisindigena2.php?modulo=principal/supervisories/supervisories&acao=A&requisicao=importarEquipeSUSisindigena2014';
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
<tr id="tr_sup" style="display:none;">
	<td class="SubTituloDireita" width="25%" id="td_label_sup"></td>
	<td id="td_name_sup"></td>
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
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>

<tr>
	<td class="SubTituloDireita" colspan="2"><input type="button" name="importar" value="Importar SISIndígena 2014" onclick="importarEquipeSUSisindigena2014();"></td>
</tr>

<tr>
	<td colspan="2" valign="top">

	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Orientadores de Estudo</b></td>
			<td class="SubTituloDireita">
			<? if(!$consulta) : ?>
			<input type="button" name="inserir" value="Inserir Orientador" onclick="inserirUsuarioPerfil('<?=PFL_ORIENTADORESTUDO ?>');">
			<? endif; ?>
			</td>
		</tr>
	</table>
	<?
	$sql = "SELECT '<center>".((!$consulta)?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirMembroEquipe('||i.iusd||','||t.pflcod||');\">":"")."</center>' as acao, CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal, i2.iusnome as supnome
			FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena2.orientadorturma o ON o.iusd = i.iusd 
			INNER JOIN sisindigena2.turmas tu ON tu.turid = o.turid 
			INNER JOIN sisindigena2.identificacaousuario i2 ON i2.iusd = tu.iusd
			WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.picid='".$_SESSION['sisindigena2']['supervisories']['picid']."' AND i2.iusd='".$_SESSION['sisindigena2']['supervisories']['iusd']."'";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Supervisor");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N');
	?>


	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Professores Alfabetizadores</b></td>
			<td class="SubTituloDireita">
			<? if(!$consulta) : ?>
			<input type="button" name="inserir" value="Inserir Professor" onclick="inserirUsuarioPerfil('<?=PFL_PROFESSORALFABETIZADOR ?>');">
			<? endif; ?>
			</td>
		</tr>
	</table>
	<?
	$sql = "SELECT '<center>".((!$consulta)?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirMembroEquipe('||i.iusd||','||t.pflcod||');\">":"")."</center>' as acao, CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal, i2.iusnome as orinome 
			FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena2.orientadorturma o ON o.iusd = i.iusd
			INNER JOIN sisindigena2.turmas tu ON tu.turid = o.turid 
			INNER JOIN sisindigena2.identificacaousuario i2 ON i2.iusd = tu.iusd 
			INNER JOIN sisindigena2.orientadorturma o2 ON o2.iusd = i2.iusd
			INNER JOIN sisindigena2.turmas tu2 ON tu2.turid = o2.turid 
			INNER JOIN sisindigena2.identificacaousuario i3 ON i3.iusd = tu2.iusd
			WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND i.picid='".$_SESSION['sisindigena2']['supervisories']['picid']."' AND i3.iusd='".$_SESSION['sisindigena2']['supervisories']['iusd']."' ORDER BY i2.iusnome, i.iusnome";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","Orientador");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N');
	?>
 	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/supervisories/supervisories&acao=A&aba=dados';">
	<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/supervisories/supervisories&acao=A&aba=visualizacao_projeto';">
	</td>
</tr>
</table>