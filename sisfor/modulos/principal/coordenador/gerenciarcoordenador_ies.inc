<?php
if($_REQUEST['requisicao'] == "pegarDadosUsuarioPorCPF"){
	pegarDadosUsuarioPorCPF($_REQUEST);
	exit();
}

if($_REQUEST['requisicao'] == "inserirCoordenador"){
	inserirCoordenador($_REQUEST,PFL_COORDENADOR_INST);
	exit();
}

if($_REQUEST['requisicao'] == "efetuarTrocaUsuarioPerfil"){
	efetuarTrocaUsuarioPerfil($_REQUEST);
	exit();
}


if($_REQUEST['requisicao'] == "reenviarSenha"){
	reenviarSenha($_REQUEST,PFL_COORDENADOR_INST,'S');
	exit();
}

if($_REQUEST['requisicao'] == "ativarUsuario"){
	reenviarSenha($_REQUEST,PFL_COORDENADOR_INST,'N');
	exit();
}

if($_REQUEST['requisicao'] == "removerCoordenador"){
	removerCoordenador($_REQUEST,PFL_COORDENADOR_INST);
	exit();
}

$_SITUACAO = array("A" => "Ativo", "P" => "Pendente", "B" => "Bloqueado"); 

$perfil = pegarPerfil($_SESSION['usucpf']);
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script language="javascript" type="text/javascript" src="js/sisfor.js"></script>
<script language="javascript" type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<?php
$coord_ies = carregarDadosIdentificacao($_REQUEST,PFL_COORDENADOR_INST);

if($coord_ies){
	$coord_ies = current($coord_ies);
	extract($coord_ies);
	$consulta = true;
	$suscod = $db->pegaUm("SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf = '{$iuscpf}' AND sisid = '".SIS_SISFOR."'");
	$e_perfil = $db->pegaUm("SELECT count(*) as n FROM seguranca.perfilusuario WHERE usucpf = '{$iuscpf}' AND pflcod = '".PFL_COORDENADOR_INST."'");
} ?>

<script language="javascript" type="text/javascript">
function carregaUsuario_(esp) {
    var usucpf = document.getElementById('iuscpf' + esp).value;

    usucpf = usucpf.replace('-', '');
    usucpf = usucpf.replace('.', '');
    usucpf = usucpf.replace('.', '');

    var comp = new dCPF();
    comp.buscarDados(usucpf);
    var arrDados = new Object();

    if (!comp.dados.no_pessoa_rf) {
        alert('CPF Inválido');
        return false;
    }

    document.getElementById('iusnome' + esp).value = comp.dados.no_pessoa_rf;

    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: 'requisicao=pegarDadosUsuarioPorCPF&cpf=' + usucpf,
        async: false,
        success: function(dados) {
            var da = dados.split("||");
            document.getElementById('iusemailprincipal' + esp).value = da[0];
        }
    });

    divCarregado();
}

function efetuarTrocaUsuarioPerfil() {

    jQuery('#iuscpf_').val(mascaraglobal('###.###.###-##', jQuery('#iuscpf_').val()));

    if (jQuery('#iuscpf_').val() == '') {
        alert('CPF em branco');
        return false;
    }

    if (!validar_cpf(jQuery('#iuscpf_').val())) {
        alert('CPF inválido');
        return false;
    }

    if (jQuery('#iusnome_').val() == '') {
        alert('Nome em branco');
        return false;
    }

    if (jQuery('#iusemailprincipal_').val() == '') {
        alert('Email em branco');
        return false;
    }

    if (!validaEmail(jQuery('#iusemailprincipal_').val())) {
        alert('Email inválido');
        return false;
    }

    document.getElementById('formulario_troca').submit();

}

function removerCoordenador(iusd, pflcod) {
	var conf = confirm('Deseja realmente remover este CPF do perfil de Coordenador IES?');
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
		jQuery("#requisicao").val('ativarUsuario');
		jQuery("#usuario").removeAttr('disabled');
	} else {
		jQuery("#requisicao").val('inserirCoordenador');
		jQuery("#usuario").attr('disabled','disabled');		
	}	
}

function ativarUsuario(){
	jQuery("#requisicao").val('ativarUsuario');
	jQuery('#formulario').submit();
}

function trocarUsuarioPerfil(pflcod, iusd) {

    jQuery('#iusdantigo').val(iusd);
    jQuery('#pflcod_').val(pflcod);

    jQuery("#modalFormulario").dialog({
        draggable: true,
        resizable: true,
        width: 800,
        height: 600,
        modal: true,
        close: function() {
        }
    });


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
                    <p>Esta é uma ferramenta de troca de usuários direta, sem aprovações ou fluxos. Todas alterações são registradas no projeto, logando as informações de quem será substituido, do novo membro e do responsável pela mudança.</p>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">CPF</td>
                <td><?= campo_texto('iuscpf_', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf_"', '', '', 'if(this.value!=\'\'){carregaUsuario_(\'_\');}'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Nome</td>
                <td><?= campo_texto('iusnome_', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome_"', ''); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Email</td>
                <td><?= campo_texto('iusemailprincipal_', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal_"'); ?></td>
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
	<input id="requisicao" type="hidden" name="requisicao" value="">
	<input id="unicod" type="hidden" name="unicod" value="<?php echo $_REQUEST['unicod']; ?>">
	<input id="iusd" type="hidden" name="iusd" value="<?php echo $iusd; ?>">
	<input id="tpeid" type="hidden" name="tpeid" value="<?php echo $tpeid; ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="25%">Orientações</td>
			<td>
				<p>Para alterar o Coordenador-Geral da IES é necessário remover o atual para que possa ser inserido um novo CPF.</p>
				<p>Caso esta pessoa nunca tenha acessado o SIMEC, a senha padrão criada será <b>simecdti</b>. Caso este usuário já tenha acessado o SIMEC, a senha continuará a mesma.</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">Coordenador Institucional:</td>
			<td>
				<?=$coord_ies ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">CPF:</td>
			<td><?php echo campo_texto('iuscpf', 'S', (($consulta) ? 'N' : 'S'), 'CPF', '15', '14', '###.###.###-##', '', '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##"), 'carregaUsuario(\'\');'); ?></td>
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
			<td class="SubTituloDireita" width="25%">Reenviar Senha para Usuário, caso esteja Pendente ou Bloqueado:</td>
			<td>
				<input id="reenviarsenha" type="checkbox" name="reenviarsenha" value="S" onclick="habilitarSenha();"> Alterar a senha do usuário para a senha padrão: <b>simecdti</b> e enviar por email.
			</td>
		</tr>
		<? } ?>
		
		<?
		
		if($iusd)
			$tpeid = $db->pegaUm("select p.tpeid from sisfor.pagamentobolsista p 
								  inner join sisfor.tipoperfil t on t.tpeid = p.tpeid 
								  where t.iusd='".$iusd."' and t.pflcod='".PFL_COORDENADOR_INST."'");
		
		?>
		
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<?php if(!$consulta){ ?>
					<input id="salvar" type="button" name="salvar" value="Salvar" onclick="inserirCoordenador();">
				<?php } else { ?>
					<?php if(($suscod == 'P' || $suscod == 'B' || !$e_perfil) && (in_array(PFL_SUPER_USUARIO,$perfil) || in_array(PFL_EQUIPE_MEC,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil))){ ?>
						<input id="usuario" type="button" name="usuario" value="Ativar Usuário" onclick="ativarUsuario();" <?php echo ($suscod == 'A' && $e_perfil) ? 'disabled="disabled"' : '' ?>>
					<?php  } ?>
					<?php if($suscod == 'A'){ ?>
						<input id="senha" type="button" name="senha" value="Reenviar Senha" onclick="reenviarSenha();" disabled="disabled">
					<?php } ?>
					<? if($tpeid) : ?>
					<input type="button" name="trocar" value="Trocar Coordenador Instucional" onclick="trocarUsuarioPerfil(<?=PFL_COORDENADOR_INST ?>, <?=$iusd ?>);">
					<? else :?>
					<input type="button" name="remover" value="Remover Coordenador Instucional" onclick="removerCoordenador();">
					<? endif; ?>
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