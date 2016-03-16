<?
include "_funcoes_escola.php";

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

$escolasensinomedio = carregarDadosGestorEscola(array('lemcodigoinep' => $_REQUEST['lemcodigoinep']));

if($escolasensinomedio) {
	extract($escolasensinomedio);
	if($usucpf) {

		$consulta = true;
		$suscod = $db->pegaUm("SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$usucpf."' AND sisid='".SIS_MEDIO."'");
		
	}
}

?>
<script>


function carregaUsuario() {
	divCarregando();
	var usucpf=document.getElementById('usucpf').value;
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
	
	document.getElementById('usunome').value=comp.dados.no_pessoa_rf;
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=pegarDadosUsuarioPorCPF&cpf='+usucpf,
   		async: false,
   		success: function(dados){
   			var da = dados.split("||");
			var suscod = da[1];
			document.getElementById('usuemail').value = da[0];
			document.getElementById('spn_susdsc').innerHTML    = situacao[suscod];
   		}
	});

	
	divCarregado();
}

function removerGestorEscola(usucpf) {
	var conf = confirm('Deseja realmente remover este cpf do perfil de Gestor da Escola?');
	
	if(conf) {
		window.location='sismedio.php?modulo=principal/escola/gerenciargestorescola&acao=A&requisicao=removerGestorEscola&lemcodigoinep=<?=$_REQUEST['lemcodigoinep'] ?>&usucpf='+usucpf;
	}

}

function inserirGestorEscolaGerenciamento() {

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
	
	if(jQuery('#usuemail').val()=='') {
		alert('Email em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#usuemail').val())) {
    	alert('Email inválido');
    	return false;
    }

	document.getElementById('formulario').submit();
}
</script>
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="inserirGestorEscolaGerenciamento">
<input type="hidden" name="lemcodigoinep" value="<?=$_REQUEST['lemcodigoinep'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<p>Para alterar o gestor da Escola é necessário remover o atual para que possa ser inserido um novo CPF.</p>
	<p>Caso esta pessoa nunca tenha acessado o SIMEC, a senha padrão criada será <b>simecdti</b>. Caso este usuário já tenha acessado o SIMEC, a senha continuará a mesma.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Escola</td>
	<td><?=$lemnomeescola ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?=campo_texto('usucpf', "S", (($consulta)?"N":"S"), "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="usucpf"', '', mascaraglobal($usucpf,"###.###.###-##"), 'carregaUsuario();'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Nome</td>
	<td><?=campo_texto('usunome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="usunome"', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Email</td>
	<td><?=campo_texto('usuemail', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="usuemail"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Status Geral do Usuário</td>
	<td>
	<b><span id="spn_susdsc"><?=(($_SITUACAO[$suscod])?$_SITUACAO[$suscod]:"Não cadastrado") ?></span></b> <input type="checkbox" name="suscod" value="A" checked> Ativar usuário no SISMédio
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
	<input type="button" name="salvar" value="Salvar" onclick="inserirGestorEscolaGerenciamento();">
	<? if($consulta && $esdid_ies != ESD_VALIDADO_COORDENADOR_IES) : ?>
	<input type="button" name="remover" value="Remover Gestor Escola" onclick="removerGestorEscola('<?=$usucpf ?>','<?=PFL_COORDENADORIES ?>');">
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