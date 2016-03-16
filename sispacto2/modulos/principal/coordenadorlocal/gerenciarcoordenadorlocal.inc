<?
include "_funcoes_coordenadorlocal.php";
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$perfis = pegaPerfilGeral();

$_SITUACAO = array("A" => "Ativo", "P" => "Pendente", "B" => "Bloqueado");

if(!$_REQUEST['picid']) die("<script>alert('Problemas de acesso. Tente novamente');window.opener.location='sispacto2.php?modulo=inicio&acao=C';window.close();</script>");

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>


function selecionarCoordenadorLocal(iusd) {
	window.location='sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=<?=$_REQUEST['picid'] ?>&iusd='+iusd;
}

</script>
<?
if($_REQUEST['iusd']!='novo') {

	$coordlocal = carregarDadosIdentificacaoUsuario(array("picid"=>$_REQUEST['picid'],"pflcod"=>PFL_COORDENADORLOCAL));
	
}

if(count($coordlocal) > 1) :
	foreach($coordlocal as $cl) {
		$arrCL[] = array("codigo"=>$cl['iusd'],"descricao"=>$cl['iusnome']);
		if($_REQUEST['iusd']==$cl['iusd']) $selecionado = $cl;
	}
	
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		$arrCL[] = array("codigo"=>"novo","descricao"=>"Inserir Novo Coordenador Local");
	}
	
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Coordenadores Locais</td>
	<td><?

	$db->monta_combo('iusd', $arrCL, 'S', 'Selecione', 'selecionarCoordenadorLocal', '', '', '', 'N', 'iusd','', $_REQUEST['iusd']);
	?></td>
</tr>
</table>
<?

if($_REQUEST['iusd'] && $selecionado) $coordlocal[] = $selecionado;
else exit;

else :

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
	echo "<p align=center><input type=button name=inserirnovo value=\"Inserir Novo Coordenador Local\" onclick=\"selecionarCoordenadorLocal('novo');\"></p>";
}

endif;

if($coordlocal) {
	$coordlocal = current($coordlocal);
	extract($coordlocal);
	$consulta = true;
	$suscod = $db->pegaUm("SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$iuscpf."' AND sisid='".SIS_SISPACTO."'");
	$esdid_pic = $db->pegaUm("SELECT esdid FROM sispacto2.pactoidadecerta p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE picid='".$_REQUEST['picid']."'");
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

function removerCoordenadorLocal(iusd, pflcod) {
	var conf = confirm('Deseja realmente remover este cpf do perfil de coordenador local? Caso tenha mais 1 Coordenador Local, esta vaga será excluída e não poderá ser mais preenchida.');
	
	if(conf) {
		window.location='sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=<?=$_REQUEST['picid'] ?>&requisicao=removerTipoPerfil&iusd='+iusd+'&pflcod='+pflcod;
	}

}

function inserirCoordenadorLocalGerenciamento() {

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

function trocarUsuarioPerfil(pflcod, iusd) {

	jQuery('#iusdantigo').val(iusd);
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
<form method="post" name="formulario_troca" id="formulario_troca">
<input type="hidden" name="requisicao" value="efetuarTrocaUsuarioPerfil">
<input type="hidden" name="iusdantigo" id="iusdantigo" value="">
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
	<td class="SubTituloCentro" colspan="2">
	<input type="button" name="salvar" value="Salvar" onclick="efetuarTrocaUsuarioPerfil();">
	</td>
</tr>
</table>
</form>
</div>


<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="inserirCoordenadorLocalGerenciamento">
<input type="hidden" name="picid" value="<?=$_REQUEST['picid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<p>Para substituir o Coordenador Local do Pacto é necessário remover o atual e inserir os dados do(a) novo(a) Coordenador(a).</p>
	<p>Para acessar o SisPacto, a pessoa indicada que já tem acesso ao SIMEC pode acessar este sistema com a mesma senha utilizada para outros módulos do SIMEC. Caso a pessoa não tenha acesso a nenhum módulo do SIMEC, então a senha inicial de acesso será “simecdti” e deverá ser trocada no primeiro acesso para outra de fácil memorização.</p>
	<p>Lembre-se: os Coordenadores Locais só conseguem acessar o SisPacto quando o seu cadastro foi concluído pela Equipe do PAR – Aprovação. Em caso de dúvida, consulte o Manual de Orientações.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Coordenador Local</td>
	<td><?=$db->pegaUm("SELECT CASE WHEN p.muncod IS NOT NULL THEN m.estuf || ' - ' || m.mundescricao 
									WHEN p.estuf IS NOT NULL THEN e.estuf || ' - ' || e.estdescricao END as descricao 
						FROM sispacto2.pactoidadecerta p 
						LEFT JOIN territorios.municipio m ON m.muncod=p.muncod 
						LEFT JOIN territorios.estado e ON e.estuf = p.estuf  
						WHERE picid='".$_REQUEST['picid']."'") ?></td>
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
	<input type="button" name="salvar" value="Salvar" onclick="inserirCoordenadorLocalGerenciamento();">
	<? if($consulta && $esdid_pic != ESD_VALIDADO_COORDENADOR_LOCAL && !$_REQUEST['iusd']) : ?>
	<input type="button" name="remover" value="Remover Coordenador Local" onclick="removerCoordenadorLocal('<?=$iusd ?>','<?=PFL_COORDENADORLOCAL ?>');">
	<? elseif($consulta) : ?>
	<input type="button" name="trocar" value="Trocar Coordenador Local" onclick="trocarUsuarioPerfil('<?=PFL_COORDENADORLOCAL ?>', '<?=$iusd ?>');">
	<? if($db->testa_superuser() || in_array(PFL_EQUIPEMUNICIPALAP,$perfis) || in_array(PFL_EQUIPEESTADUALAP,$perfis)) : ?>
	<input type="button" name="remover" value="Remover Coordenador Local" onclick="removerCoordenadorLocal('<?=$iusd ?>','<?=PFL_COORDENADORLOCAL ?>');">
	<? endif; ?>
	<? endif; ?>
	</td>
</tr>
</table>
</form>
<?
if($iuscpf) {
	echo "<p align=center><b>Histórico de gerenciamento de usuários</b></p>";
	$sql = "SELECT htudsc, to_char(htudata,'dd/mm/YYYY HH24:MI') as htudata FROM seguranca.historicousuario WHERE usucpf='".$iuscpf."' AND sisid='".SIS_SISPACTO."'";
	$cabecalho = array("Motivo", "Data");
	$db->monta_lista_simples($sql,$cabecalho,150,10,"N","","N");
}
?>