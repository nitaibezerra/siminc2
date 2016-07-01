<?
if(!verificarAceitacaoTermoCompromisso(array('iusid' => $_SESSION['escolaterra'][$sis]['iusid']))) {
 	$al = array("alert"=>"Por favor preencher os dados cadastrais, e aceitar o termo de compromisso","location"=>"/escolaterra/escolaterra.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=dados");
 	alertlocation($al);
}

$turma = carregarDadosTurma($perfil);

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$turma['docid']."'");

$perfis = pegaPerfilGeral();

if($esdid != ESD_EM_CADASTRAMENTO && !$db->testa_superuser() && !in_array(PFL_ADMINISTRADOR, $perfis)) {
	$consulta = true;
}

?>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

function salvarMunicipioAtuacao() {
	if(jQuery('#muncodatuacao').val()=='') {
		alert('Selecione um município');
		return false;
	}

	jQuery('#formulario_municipioatuacao').submit();
}

function salvarRedeAtuacao() {
	if(jQuery('#iusrede').val()=='') {
		alert('Selecione uma rede');
		return false;
	}

	jQuery('#formulario_rede').submit();
}

function exibirMunicipio(iusid, muncod) {

	jQuery('#iusid_atuacao').val(iusid);
	ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncodatuacao&name=muncodatuacao&valuecombo='+muncod+'&estuf=<?=$db->pegaUm("SELECT estuf FROM escolaterra.ufparticipantes WHERE ufpid='".$_SESSION['escolaterra'][$sis]['ufpid']."'") ?>','dv_municipio');

	jQuery("#modalMunicipio").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 500,
	                        height: 300,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function exibirRede(iusid, rede) {

	jQuery('#iusid_redeatuacao').val(iusid);
	ajaxatualizar('requisicao=carregarRede&id=iusrede&name=iusrede&valuecombo='+rede,'dv_rede');

	jQuery("#modalRede").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 500,
	                        height: 300,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function inserirUsuarioPerfil(pflcod) {

	ajaxatualizar('requisicao=carregarCamposPerfil&pflcod='+pflcod,'td_campoperfil');

	jQuery("#modalFormulario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function efetuarInsercaoUsuarioPerfil() {

	if(jQuery('#pflcod_').length==0) {
		alert('Perfil não foi carregado. Feche a tela e abra novamente');
		return false;
	}

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

    if(jQuery('#muncod_endereco').length==0) {
		alert('Selecione uma UF');
		return false;
    }

	if(jQuery('#muncod_endereco').val()=='') {
		alert('Selecione um Munícipio');
		return false;
	}
	jQuery('#salvar_inserir').attr('value','Salvando... Aguarde!');
	jQuery('#salvar_inserir').attr('disabled','disabled');

	document.getElementById('formulario_insercao').submit();

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

function removerIdentificacaoPerfil(iusid, pflcod) {
	var conf = confirm('Deseja realmente remover o registro?');
	
	if(conf) {
		window.location='escolaterra.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=A&aba=cadastro&requisicao=removerIdentificacaoPerfil&iusid='+iusid+'&pflcod='+pflcod;
	}

}

function efetuarDownload(iusid) {
	window.location='escolaterra.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=A&aba=cadastro&requisicao=efetuarDownload&iusid='+iusid;
}
</script>

<div id="modalFormulario" style="display:none;">

<form method="post" name="formulario_insercao" id="formulario_insercao" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="efetuarInsercaoUsuarioPerfil">
<input type="hidden" name="ufpid_" value="<?=$_SESSION['escolaterra'][$perfil['perfil']]['ufpid'] ?>">
<input type="hidden" name="turid_" value="<?=$turma['turid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	[ORIENTAÇÕES]
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
	<td class="SubTituloDireita" width="25%">UF</td>
	<td>
	<?
	$sql = "SELECT estuf as codigo, estuf||' - '||estdescricao as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '200', 'S', 'estuf','', $_REQUEST['estuf']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Município atuação</td>
	<td id="td_municipio3">Selecione uma UF</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Documento comprovando seleção</td>
	<td><input type="file" name="arquivo" id="documentoselecao"> <input type="hidden" name="tipodocumentoselecao" id="tipodocumentoselecao" value="C"></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Perfil</td>
	<td id="td_campoperfil"></td>
</tr>

<tr>
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" id="salvar_inserir" value="Salvar" onclick="efetuarInsercaoUsuarioPerfil();">
	</td>
</tr>
</table>
</form>

</div>

<div id="modalMunicipio" style="display:none;">

<form method="post" name="formulario_municipioatuacao" id="formulario_municipioatuacao">
<input type="hidden" name="requisicao" value="salvarMunicipioAtuacao">
<input type="hidden" name="iusid_atuacao" id="iusid_atuacao" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Município de atuação</td>
	<td id="dv_municipio"></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" name="salvar" onclick="salvarMunicipioAtuacao();"></td>
</tr>
</table>

</form>

</div>

<div id="modalRede" style="display:none;">

<form method="post" name="formulario_rede" id="formulario_rede">
<input type="hidden" name="requisicao" value="salvarRedeAtuacao">
<input type="hidden" name="iusid_atuacao" id="iusid_redeatuacao" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Rede de atuação</td>
	<td id="dv_rede"></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" name="salvar" onclick="salvarRedeAtuacao();"></td>
</tr>
</table>

</form>

</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Cadastramento</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	
	<table width="100%">
	<tr>
		<td colspan="2">
		<? if(!$consulta) : ?>
		<input type="button" name="inserir" value="<?=$perfil['botaoinserir'] ?>" onclick="inserirUsuarioPerfil('<?=$perfil['pflcod'] ?>');">
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td valign="top" width="95%">
		<?
		$sql = "SELECT '<center>".(($consulta)?"":"<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"removerIdentificacaoPerfil('|| i.iusid ||','|| t.pflcod ||')\">")." '|| CASE WHEN iudid IS NOT NULL THEN '<img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" onclick=\"efetuarDownload('|| i.iusid ||')\">' ELSE '' END ||'</center>' as acao,
					   replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf,
					   i.iusnome as nome,
					   i.iusemailprincipal as email,
 					   COALESCE(m.estuf||' - '||m.mundescricao,'Não informado')||' <img src=../imagens/arrow_v.png style=cursor:pointer; align=absmiddle onclick=\"exibirMunicipio('||i.iusid||',\''||COALESCE(i.muncodatuacao,'')||'\');\">' as municipio,
 					   CASE WHEN i.iusrede='E' THEN 'Estadual'
 							WHEN i.iusrede='M' THEN 'Municipal'
 							ELSE 'Não informado' END||' <img src=../imagens/arrow_v.png style=cursor:pointer; align=absmiddle onclick=\"exibirRede('||i.iusid||',\''||COALESCE(i.iusrede,'')||'\');\">' as rede
				FROM escolaterra.identificacaousuario i 
				INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".$perfil['pflcod']." 
				INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
				LEFT JOIN escolaterra.identificacaousuariodocumentos iud ON iud.iusid = i.iusid AND iud.iudtipo='C' 
				LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao  
				WHERE u.turid='".$turma['turid']."'";
		
		$cabecalho = array("&nbsp;","CPF","Nome","Email","Município de atuação","Rede");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
		?>
		</td>
		<td valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $turma['docid'], array('tipo' => $tipo) );
		?>
		</td>
	</tr>
	</table>

	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
		<? if($goto_pro) : ?>
		<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
		<? endif; ?>
		</td>
</tr>
</table>
