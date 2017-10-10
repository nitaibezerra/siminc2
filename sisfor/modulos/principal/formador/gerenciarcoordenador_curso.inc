<?php
if($_REQUEST['requisicao'] == "pegarDadosUsuarioPorCPF"){
	pegarDadosUsuarioPorCPF($_REQUEST);
	exit();
}

if($_REQUEST['requisicao'] == "inserirCoordenador"){
	inserirCoordenador($_REQUEST,PFL_COORDENADOR_CURSO);
	exit();
}

if($_REQUEST['requisicao'] == "reenviarSenha"){
	reenviarSenha($_REQUEST,PFL_COORDENADOR_CURSO,'S');
	exit();
}

if($_REQUEST['requisicao'] == "ativarUsuario"){
	reenviarSenha($_REQUEST,PFL_COORDENADOR_CURSO,'N');
	exit();
}

if($_REQUEST['requisicao'] == "removerCoordenador"){
	removerCoordenador($_REQUEST,PFL_COORDENADOR_CURSO);
	exit();
}

$_SITUACAO = array("A" => "Ativo", "P" => "Pendente", "B" => "Bloqueado"); 

$perfil = pegarPerfil($_SESSION['usucpf']);


if($_REQUEST['ieoid']){
	$sifopcao = verificarSituacaoCursoMEC($_REQUEST['ieoid']);
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="js/sisfor.js"></script>
<script language="javascript" type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<?php
$coord_curso = carregarDadosIdentificacao($_REQUEST,PFL_COORDENADOR_CURSO);

if($coord_curso) {
	$coord_curso = current($coord_curso);
	extract($coord_curso);
	$consulta = true;
	$suscod = $db->pegaUm("SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf = '{$iuscpf}' AND sisid = '".SIS_SISFOR."'");
} ?>

<script language="javascript" type="text/javascript">
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
		<?php foreach($_SITUACAO as $key => $sit){ ?>
		situacao['<?php echo $key ?>'] = '<?php echo $sit; ?>';
		<?php } ?>
		
		jQuery('#iusnome').val(comp.dados.no_pessoa_rf);
		
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: 'requisicao=pegarDadosUsuarioPorCPF&cpf='+usucpf,
	   		async: false,
	   		success: function(dados){
	   			var da = dados.split("||");
				var suscod = da[1];
				jQuery('#iusemailprincipal').val(da[0]);
				document.getElementById('spn_susdsc').innerHTML = situacao[suscod];
	   		}
		});
		divCarregado();
	}
	
	function removerCoordenador() {
		var conf = confirm('Deseja realmente remover este CPF do perfil de Coordenador Curso?');
		if(conf){
			jQuery('#requisicao').val('removerCoordenador');
			jQuery('#formulario').submit();
		}
	}
	
	function inserirCoordenador() {
		jQuery("#requisicao").val('inserirCoordenador');	
		jQuery('#iuscpf').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()));
		if(jQuery('#iuscpf').val()==''){
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
		jQuery('#formulario').submit();
	}
	
	function habilitarSenha(){
		if(jQuery('#reenviarsenha').is(':checked')){
			jQuery("#requisicao").val('reenviarSenha');
			jQuery("#senha").removeAttr('disabled');
		} else {
			jQuery("#requisicao").val('inserirCoordenador');
			jQuery("#senha").attr('disabled','disabled');		
		}	
	}
	
	function reenviarSenha(){
		jQuery("#requisicao").val('reenviarSenha');
		jQuery('#formulario').submit();
	}
	
	function habilitarUsuario(){
		if(jQuery('#suscod').is(':checked')){
			jQuery('#requisicao').val('ativarUsuario');
			jQuery('#usuario').removeAttr('disabled');
		} else {
			jQuery('#requisicao').val('inserirCoordenador');
			jQuery('#usuario').attr('disabled','disabled');		
		}	
	}
	
	function ativarUsuario(){
		jQuery('#requisicao').val('ativarUsuario');
		jQuery('#formulario').submit();
	}
	
	jQuery(document).ready(function(){
		<?php if($sifopcao == '2'){?>
		jQuery('#salvar').attr('disabled','disabled');
		jQuery('#iuscpf').attr('disabled','disabled');		
		<?php } ?>
	});
</script>
<form method="post" name="formulario" id="formulario">
	<input id="requisicao" type="hidden" name="requisicao" value="">
	<input id="unicod" type="hidden" name="unicod" value="<?php echo $_REQUEST['unicod']; ?>">
	<input id="iusd" type="hidden" name="iusd" value="<?php echo $iusd; ?>">
	<input id="tpeid" type="hidden" name="tpeid" value="<?php echo $tpeid; ?>">
	<?php if($_REQUEST['ieoid']){ ?>
	<input id="ieoid" type="hidden" name="ieoid" value="<?php echo $_REQUEST['ieoid']; ?>">
	<?php } ?>
	<?php if($_REQUEST['cnvid']){ ?>
	<input id="cnvid" type="hidden" name="cnvid" value="<?php echo $_REQUEST['cnvid']; ?>">
	<?php } ?>
	<?php if($_REQUEST['ocuid']){ ?>
	<input id="ocuid" type="hidden" name="ocuid" value="<?php echo $_REQUEST['ocuid']; ?>">
	<?php } ?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="25%">Orientações</td>
			<td>
				<p>Para alterar o Coordenador-Geral da IES é necessário remover o atual para que possa ser inserido um novo CPF.</p>
				<p>Caso esta pessoa nunca tenha acessado o <?php echo SIGLA_SISTEMA; ?>, a senha padrão criada será <b>simecdti</b>. Caso este usuário já tenha acessado o <?php echo SIGLA_SISTEMA; ?>, a senha continuará a mesma.</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">Coordenador IES:</td>
			<td>
				<?php 
				$sql = "SELECT	 		iu.iusnome 
						FROM 			sisfor.usuarioresponsabilidade ur
						INNER JOIN		sisfor.identificacaousuario iu ON ur.usucpf = iu.iuscpf
						WHERE 			ur.rpustatus = 'A'";
				$db->pegaUm($sql);?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">CPF:</td>
			<td><?php echo campo_texto('iuscpf', 'S', (($consulta) ? 'N' : 'S'), 'CPF', '15', '14', '###.###.###-##', '', '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##"), 'carregaUsuario();'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">Nome:</td>
			<td><?php echo campo_texto('iusnome', "S", "N", "Nome", 67, 150, '', '', '', '', 0, 'id="iusnome"', ''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">E-mail:</td>
			<td><?php echo campo_texto('iusemailprincipal', "S", (($consulta) ? "N" : "S"), 'Principal', '67', '60', '', '', '', '', 0, 'id="iusemailprincipal"'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">Status Geral do Usuário:</td>
			<td>
				<b><span id="spn_susdsc"><?php echo (($_SITUACAO[$suscod]) ? $_SITUACAO[$suscod] : "Não cadastrado"); ?></span></b> 
				<?php if($suscod <> 'A'){ ?>
				<input id="suscod" type="checkbox" name="suscod" value="A" checked="checked" onclick="habilitarUsuario();"> Ativar usuário no sisfor caso esteja Pendente ou Bloqueado e enviar por email senha padrão: <b>simecdti</b>.
				<?php } ?>
			</td>
		</tr>
		<?php if($consulta && $suscod == 'A'){ ?>
		<tr>
			<td class="SubTituloDireita" width="25%">Reenviar Senha para Usuário, :</td>
			<td>
				<input id="reenviarsenha" type="checkbox" name="reenviarsenha" value="S" onclick="habilitarSenha();"> Alterar a senha do usuário para a senha padrão: <b>simecdti</b> e enviar por email.
			</td>
		</tr>
		<? } ?>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<?php if(!$consulta){ ?>
					<input id="salvar" type="button" name="salvar" value="Salvar" onclick="inserirCoordenador();">
				<?php } else { ?>
					<?php if(($suscod == 'P' || $suscod == 'B') && (in_array(PFL_SUPER_USUARIO,$perfil) || in_array(PFL_EQUIPE_MEC,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil) || in_array(PFL_COORDENADOR_CURSO,$perfil))){ ?>
						<input id="usuario" type="button" name="usuario" value="Ativar Usuário" onclick="ativarUsuario();" <?php echo ($suscod == 'A') ? 'disabled="disabled"' : '' ?>>
					<?php  } ?>
					<?php if($suscod == 'A'){ ?>
						<input id="senha" type="button" name="senha" value="Reenviar Senha" onclick="reenviarSenha();" disabled="disabled">
					<?php } ?>
					<input type="button" name="remover" value="Remover Coordenador Curso" onclick="removerCoordenador();">
				<?php } ?>
				<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>	
			</td>
		</tr>
	</table>
</form>

<?php
if($iuscpf){
	historicoUsuario($iuscpf);
} 
?>