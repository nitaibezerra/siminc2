<?

include "_funcoes_coordenadorestadual.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$_SITUACAO = array("A" => "Ativo", "P" => "Pendente", "B" => "Bloqueado");

if(!$_REQUEST['ufpid']) 
	die("<script>
			alert('Não foi possível identificar a REDE participante.');
			window.close();
		 </script>");

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
 
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<?
$coordestadual = carregarDadosIdentificacaoUsuario(array("ufpid"  => $_REQUEST['ufpid'],
													  	 "pflcod" => PFL_COORDENADORESTADUAL));

if($coordestadual) {
	$coordestadual = current($coordestadual);
	extract($coordestadual);
	$consulta = true;
	$suscod = $db->pegaUm("SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$iuscpf."' AND sisid='".SIS_ESCOLATERRA."'");
}

?>
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
	
	var situacao = new Array();
	situacao['NC'] = 'Não cadastrado';
	<? foreach($_SITUACAO as $key => $sit) : ?>
	situacao['<?=$key ?>'] = '<?=$sit ?>';
	<? endforeach; ?>
	
	document.getElementById('iusnome').value=comp.dados.no_pessoa_rf;
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=pegarDadosUsuarioPorCPF&cpf='+usucpf,
   		async: false,
   		success: function(dados){
   			var da = dados.split("||");
			var suscod = da[1];
			document.getElementById('iusemailprincipal').value = da[0];
			document.getElementById('spn_susdsc').innerHTML    = situacao[suscod];
   		}
	});

	
	divCarregado();
}

function inserirCoordenadorEstadualGerenciamento() {

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
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#iusemailprincipal').val())) {
    	alert('Email inválido');
    	return false;
    }

	document.getElementById('formulario').submit();
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

function trocarUsuarioPerfil(pflcod, iusid) {
	jQuery('#iusidantigo').val(iusid);
	jQuery('#pflcod_').val(pflcod);

	jQuery("#modalFormulario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 700,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function carregaUsuario_() {
	var usucpf=document.getElementById('iuscpf_').value;

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
	
	document.getElementById('iusnome_').value=comp.dados.no_pessoa_rf;
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=pegarDadosUsuarioPorCPF&cpf='+usucpf,
   		async: false,
   		success: function(dados){
   			var da = dados.split("||");
			document.getElementById('iusemailprincipal_').value = da[0];
   		}
	});
	
	divCarregado();
}
</script>
<div id="modalFormulario" style="display:none;">
<form method="post" name="formulario_troca" id="formulario_troca" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="efetuarTrocaUsuarioPerfil">
<input type="hidden" name="iusidantigo" id="iusidantigo" value="">
<input type="hidden" name="pflcod_" id="pflcod_" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	[ORIENTAÇÕES]
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?=campo_texto('iuscpf_', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf_"', '', '', 'if(this.value!=\'\'){carregaUsuario_();}'); ?></td>
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
	<td class="SubTituloDireita" width="25%">Documento comprovando seleção</td>
	<td><input type="file" name="arquivo_" id="documentoselecao_"> <input type="hidden" name="tipodocumentoselecao_" id="tipodocumentoselecao_" value="C">
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" value="Salvar" onclick="efetuarTrocaUsuarioPerfil();">
	</td>
</tr>
</table>
</form>
</div>


<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirCoordenadorEstadual">
<input type="hidden" name="ufpid" value="<?=$_REQUEST['ufpid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<p>A Tela permite o gerenciamento de acesso do Coordenador Estadual, verificando se ele esta Ativo, Pendente ou Não cadastrado. É possível trocar o Coordenador Local, Ativar (em caso de bloqueio) e reenviar a senha de acesso.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Coordenador Estadual</td>
	<td><?= $db->pegaUm("SELECT e.estuf || ' / ' || e.estdescricao as descricao 
						 FROM escolaterra.ufparticipantes u 
						 INNER JOIN territorios.estado e ON e.estuf = u.estuf 
						 WHERE ufpid = '".$_REQUEST['ufpid']."'") ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?=campo_texto('iuscpf', "S", (($consulta)?"N":"S"), "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##"), 'carregaUsuario();'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Nome</td>
	<td><?=campo_texto('iusnome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Email</td>
	<td><?=campo_texto('iusemailprincipal', "S", (($consulta)?"N":"S"), "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Documento comprovando seleção</td>
	<td><input type="file" name="arquivo" id="documentoselecao"> <input type="hidden" name="tipodocumentoselecao" id="tipodocumentoselecao" value="C">
	<? 
	
	if($iusid) {
	
		$arrArquivo = $db->pegaLinha("SELECT p.arqid, p.arqnome||'.'||p.arqextensao as arquivo 
									  FROM escolaterra.identificacaousuariodocumentos i 
									  INNER JOIN public.arquivo p ON p.arqid = i.arqid 
									  WHERE i.iusid='".$iusid."' AND i.iudtipo='C'");
		
		if($arrArquivo['arqid']) {
			echo "<br><br> <span style=\"cursor:pointer;\" onclick=\"window.location='escolaterra.php?modulo=principal/coordenadorestadual/gerenciarcoordenadorestadual&acao=A&&requisicao=downloadDocumentoSelecao&arqid=".$arrArquivo['arqid']."';\"><img src=\"../imagens/anexo.gif\" align=\"absmiddle\"> ".$arrArquivo['arquivo']."</span>";
		}
	
	}
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Status Geral do Usuário</td>
	<td>
	<b><span id="spn_susdsc"><?=(($_SITUACAO[$suscod])?$_SITUACAO[$suscod]:"Não cadastrado") ?></span></b> <input type="checkbox" name="suscod" value="A" checked> Ativar usuário no sispacto
	</td>
</tr>
<? if($consulta) : ?>
<tr>
	<td class="SubTituloDireita" width="25%">Reenviar Senha para Usuário, caso esteja Pendente ou Bloqueado</td>
	<td><input type="checkbox" name="reenviarsenha" value="S"> Alterar a senha do usuário para a senha padrão: <b>simecdti</b> e enviar por email</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" value="Salvar" onclick="inserirCoordenadorEstadualGerenciamento();">
	<? if($consulta) : ?>
	<input type="button" name="trocar" value="Trocar Coordenador Estadual" onclick="trocarUsuarioPerfil('<?=PFL_COORDENADORESTADUAL ?>', '<?=$iusid ?>');">
	<? endif; ?>
	</td>
</tr>
</table>
</form>