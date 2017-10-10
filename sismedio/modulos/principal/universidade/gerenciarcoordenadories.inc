<?
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$_SITUACAO = array("A" => "Ativo", "P" => "Pendente", "B" => "Bloqueado");

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<?
$coordies = carregarDadosIdentificacaoUsuario(array("uncid"=>$_REQUEST['uncid'],"pflcod"=>PFL_COORDENADORIES));

if($coordies) {
	$coordies = current($coordies);
	extract($coordies);
	$consulta = true;
	$suscod = $db->pegaUm("SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$iuscpf."' AND sisid='".SIS_MEDIO."'");
	$esdid_ies = $db->pegaUm("SELECT esdid FROM sismedio.universidadecadastro p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE uncid='".$_REQUEST['uncid']."'");
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

function removerCoordenadorIES(iusd, pflcod) {
	var conf = confirm('Deseja realmente remover este cpf do perfil de coordenador IES?');
	
	if(conf) {
		window.location='sismedio.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=<?=$_REQUEST['uncid'] ?>&requisicao=removerTipoPerfil&iusd='+iusd+'&pflcod='+pflcod;
	}

}

function inserirCoordenadorIESGerenciamento() {

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
</script>
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="inserirCoordenadorIESGerenciamento">
<input type="hidden" name=uncid value="<?=$_REQUEST['uncid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<p>Para alterar o Coordenador-Geral da IES é necessário remover o atual para que possa ser inserido um novo CPF.</p>
	<p>Caso esta pessoa nunca tenha acessado o <?php echo SIGLA_SISTEMA; ?>, a senha padrão criada será <b>simecdti</b>. Caso este usuário já tenha acessado o <?php echo SIGLA_SISTEMA; ?>, a senha continuará a mesma.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Coordenador IES</td>
	<td><?=$db->pegaUm("SELECT su.uniuf || ' - ' || su.uninome as descricao FROM sismedio.universidadecadastro u 
						INNER JOIN sismedio.universidade su ON su.uniid = u.uniid 
						WHERE u.uncid='".$_REQUEST['uncid']."'") ?></td>
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
	<b><span id="spn_susdsc"><?=(($_SITUACAO[$suscod])?$_SITUACAO[$suscod]:"Não cadastrado") ?></span></b> <input type="checkbox" name="suscod" value="A" checked> Ativar usuário no sismedio
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
	<input type="button" name="salvar" value="Salvar" onclick="inserirCoordenadorIESGerenciamento();">
	<? if($consulta && $esdid_ies != ESD_VALIDADO_COORDENADOR_IES) : ?>
	<input type="button" name="remover" value="Remover Coordenador IES" onclick="removerCoordenadorIES('<?=$iusd ?>','<?=PFL_COORDENADORIES ?>');">
	<? endif; ?>
	</td>
</tr>
</table>
</form>
<?
if($iuscpf) {
	echo "<p align=center><b>Histórico de gerenciamento de usuários</b></p>";
	$sql = "SELECT htudsc, to_char(htudata,'dd/mm/YYYY HH24:MI') as htudata FROM seguranca.historicousuario WHERE usucpf='".$iuscpf."' AND sisid='".SIS_MEDIO."'";
	$cabecalho = array("Motivo", "Data");
	$db->monta_lista_simples($sql,$cabecalho,150,10,"N","","N");
}
?>