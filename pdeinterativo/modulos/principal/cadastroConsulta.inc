<?php
if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );


$arrPerfil = pegaPerfilGeral();

$habilitado = "S";

if(!$db->testa_superuser()) {

	if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL, $arrPerfil)) {
		
		$estuf = $db->pegaUm("SELECT estuf FROM pdeinterativo.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND estuf IS NOT NULL and rpustatus = 'A' order by rpuid desc");
		$tpocod = "2";
		$habilitado = "N";

	}
	
	if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil)) {
		$arr = $db->pegaLinha("SELECT m.muncod, m.estuf FROM pdeinterativo.usuarioresponsabilidade ur inner join territorios.municipio m ON m.muncod = ur.muncod WHERE usucpf='".$_SESSION['usucpf']."' AND ur.muncod IS NOT NULL and rpustatus = 'A' order by rpuid desc");		
		$muncod = $arr['muncod'];
		$estuf  = $arr['estuf'];
		$tpocod = "3";
		$habilitado = "N";
		
	}

}

?>
<script
	type="text/javascript" src="/includes/prototype.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script
	type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/webservice/cpf.js" /></script>
<script>

	jQuery(function() {
	
		var UA = navigator.userAgent;
		if (UA.indexOf('MSIE') > -1) {
		    alert('Algumas funcionalidades podem não funcionar corretamente com este navegar.\nRecomendamos que você utilize o Mozilla Firefox!');
		}
			
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
	
		jQuery('[name=usucpf]').blur( function() {
			limparCampos();
			carregaUsuario();
		});
		
		jQuery('[name=usuemail]').blur( function() {
			validaEmail();
			if( jQuery(this).val() && jQuery(this).val() == jQuery('[name=cousuemail]').val() ){
				verificaReenvioEmail();
			}
		});
				
		jQuery('[name=cousuemail]').blur( function() {
			if( jQuery(this).val() != jQuery('[name=usuemail]').val() ){
				alert("Confirmação de E-mail inválida!");
			}else{
				verificaReenvioEmail();
			}
		});
		
		if(jQuery('[name=usucpf]').val()){
			var cpf = jQuery('[name=usucpf]').val();
			jQuery('[name=usucpf]').val( mascaraglobal('###.###.###-##',cpf) );
			carregaUsuario();
		}
		
	});

	function enviarEmail()
	{
		var usucpf = jQuery('[name=usucpf]').val();
		if(usucpf){
			usucpf = usucpf.replace("-","");
			usucpf = usucpf.replace(".","");
			usucpf = usucpf.replace(".","");
			url = "../geral/envia_email_usuario.php?usuID=" + usucpf;
	    	janela(url,700,550,"E-mail Usuário")
		}else{
			alert('Favor informar o CPF!');
		}
	}
	
	function carregaUsuario()
	{
		var usucpf = jQuery('[name=usucpf]').val();
		if(usucpf){
			usucpf = usucpf.replace("-","");
			usucpf = usucpf.replace(".","");
			usucpf = usucpf.replace(".","");
			
			jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=recuperaMembroPorCPF&usucpf=" + usucpf,
			   dataType: 'json',
			   success: function(arrDados){
			   
			   		if(arrDados == null || arrDados == false){
			   			var comp = new dCPF();
						comp.buscarDados(usucpf);
						if(!comp.dados.no_pessoa_rf){
							alert('CPF Inválido');
							return false;
						}
						var arrDados = new Object();
						arrDados.usunome = comp.dados.no_pessoa_rf;
						arrDados.ususexo = comp.dados.sg_sexo_rf;
			   		}
			   		
			   		if(arrDados.ususexo == "M"){
						arrDados.ususexo = "Masculino";
					}
					if(arrDados.ususexo == "F"){
						arrDados.ususexo = "Feminino";
					}
			   		jQuery('[id=td_usunome]').html( arrDados.usunome );
			   		jQuery('[name=usunome]').val( arrDados.usunome );
			   		jQuery('[id=td_ususexo]').html( arrDados.ususexo );
			   		jQuery('[name=ususexo]').val( arrDados.ususexo );
			   		<? if($habilitado=="S") : ?>
			   		jQuery('[name=estuf]').val( arrDados.regcod );
			   		ajax("filtraMunicipio&estuf=" + arrDados.regcod + "&muncod=" + arrDados.muncod,"td_muncod");
			   		jQuery('[name=tpocod]').val( arrDados.tpocod );
			   		<? endif; ?>
			   		ajax("filtraOrgao&tpocod=" + jQuery('[name=tpocod]').val() + "&orgid=" + arrDados.orgid,"td_orgid");
			   		
			   		jQuery('[name=usuemail],[name=cousuemail]').val( arrDados.usuemail );
			   		jQuery('[name=usufoneddd]').val( arrDados.usufoneddd );
			   		jQuery('[name=usufonenum]').val( mascaraglobal('####-####',arrDados.usufonenum) );
			   		jQuery('[name=carid]').val( arrDados.carid );
			   		jQuery('#rdo_status_' + arrDados.status ).attr("checked","checked");
			   		jQuery('[name=justificativa]').val( arrDados.justificativa );
			   }
			 });
			
		}else{
			limparCampos();
		}
	}
	
	function limparCampos()
	{
		jQuery('[id=td_usunome],[id=td_ususexo]').html( "" );
		<? if($habilitado=="S") : ?>
		jQuery('[name=estuf],[name=muncod],[name=tpocod]').val( "" );
		<? endif; ?>
		jQuery('[name=carid],[name=orgid],[name=usuemail],[name=cousuemail],[name=usufoneddd],[name=usufonenum],[name=carid],[name=justificativa]').val( "" );
		jQuery('[name=rdo_status]').attr("checked","");
	}
	
	function salvarMembroConsulta()
	{
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		if( erro == 0){
			if( !validaEmail() ){
				erro = 1;
			}
		}
		if( erro == 0){
			if( !jQuery("#td_usunome").html() || !jQuery("#td_ususexo").html() ){
				alert('CPF Inválido');
				erro = 1;
				return false;
			}
		}
		if( erro == 0 && jQuery('[name=usuemail]').val() != jQuery('[name=cousuemail]').val() ){
			alert("Confirmação de E-mail inválida!");
			erro = 1;
		}

		if(erro == 0){
			jQuery("#form_cad_consulta").submit();
		}
	}
	
	function verificaReenvioEmail()
	{
		var email = jQuery('[name=usuemail]').val();
		var usucpf = jQuery('[name=usucpf]').val();
		
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=verificaReenvioEmail&usucpf=" + usucpf + "&usuemail=" + email,
			   success: function(resp){
			   		if(resp){
			   			if(confirm("A alteração de e-mail gera uma nova senha de acesso, deseja enviá-la para o novo e-mail?")){
			   				jQuery('[id=reenvio_email]').html("<input type=\"hidden\" name=\"hdn_reenvia_email\" value=\"1\" >A senha de acesso ao SIMEC será enviada para o novo e-mail.");
			   				return true;
			   			}
			   		}else{
			   			jQuery('[id=reenvio_email]').html("");
			   			return true;
			   		}
			   }
			 });
		
	}
	
	function validaEmail()
	{
		var sEmail	= jQuery('[name=usuemail]').val();
		var emailFilter = /^.+@.+\..{2,}$/;
		var illegalChars = /[\(\)\<\>\,\;\:\\\/\"\[\]]/
		if( !(emailFilter.test(sEmail)) || sEmail.match(illegalChars) ){
			alert('Por favor, informe um email válido.');
			jQuery('[name=usuemail]').focus();
			return false;
		}else{
			return true;
		}
	}

</script>
<link
	rel="stylesheet" type="text/css"
	href="../pdeinterativo/css/pdeinterativo.css" />
<form name="form_cad_consulta" id="form_cad_consulta" method="post" action="">
	<input type="hidden" name="requisicao" value="salvarMembroConsulta" />
	<input type="hidden" name="usunome" value="" />
	<input type="hidden" name="ususexo" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td width="25%" class="SubTituloDireita">CPF:</td>
		<td><?php echo campo_texto("usucpf","S", ($arrDados['usucpf'] ? "N" : "S"),"CPF","16","14","###.###.###-##","","","","","","",$arrDados['usucpf'])?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td id="td_usunome"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Gênero:</td>
		<td id="td_ususexo"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estado:</td>
		<td id="td_estuf">
		<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?> 
		<?php $db->monta_combo("estuf",$sql,$habilitado,"Selecione...","filtraMunicipio","","","","S","","") ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município:</td>
		<td id="td_muncod"><?php if($estuf): ?> <?php $sql = "	select
										muncod as codigo,
										mundescricao as descricao
									from
										territorios.municipio
									where
										estuf = '$estuf'
									order by
										mundescricao" ?> <?php $db->monta_combo("muncod",$sql,$habilitado,"Selecione...","","","","","N","","") ?>
		<?php else: ?> <?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
		<?php endif; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de Órgão:</td>
		<td id="td_tpocod"><?php $sql = "	select
									tpocod as codigo,
									tpodsc as descricao
								from
									public.tipoorgao
								where
									tpostatus = 'A'
								and
									tpocod::integer in (2,3)
								order by
									tpodsc" ?> <?php $db->monta_combo("tpocod",$sql,$habilitado,"Selecione...","filtraOrgao","","","","S","","") ?>
		</td>
	</tr>
	<!-- 
		<tr>
			<td class="SubTituloDireita" >Órgão:</td>
			<td id="td_orgid">
				<?php if($tpocod): ?>
					<?php $sql = "	select
										orgid as codigo,
										orgcod || ' - ' || orgdsc as descricao
									from
										public.orgao
									where
										tpocod = '$tpocod'
									order by
										orgdsc" ?>
					<?php $db->monta_combo("orgid",$sql,"S","Selecione...","","","","","S","","",$orgid) ?>
				<?php else: ?>
					<?php $db->monta_combo("orgid",array(),"N","Selecione o Tipo de Órgão","","","","","N") ?>
				<?php endif; ?>
			</td>
		</tr>
		 -->
	<tr>
		<td class="SubTituloDireita">E-mail:</td>
		<td id="td_usuemail"><?php echo campo_texto("usuemail","S","S","E-mail","40","40","","")?>
		<img
			onmouseover="return escape('Atenção, este e-mail será utilizado para enviar a senha de acesso ao sistema para o usuário!')"
			class="link img_middle" src="../imagens/icon_campus_2.png" /> <span
			id="reenvio_email" class="msg_erro bold"></span></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Confirme o E-mail:</td>
		<td id="td_cousuemail"><?php echo campo_texto("cousuemail","S","S","E-mail","40","40","","")?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Telefone:</td>
		<td id="td_usufone"><?php echo campo_texto("usufoneddd","N","S","DDD","2","2","","")?>&nbsp;
		<?php echo campo_texto("usufonenum","S","S","DDD","10","9","####-####","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Função / Cargo:</td>
		<td id="td_carid"><?php $sql = "select carid as codigo, cardsc as descricao from public.cargo order by cardsc"; ?>
		<?php $db->monta_combo("carid",$sql,"S","Selecione...","","","","","S","","",$carid) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Status Geral do Usuário:</td>
		<td id="td_usustatus"><input type="radio" name="rdo_status"
			id="rdo_status_A" value="A" /> Ativo <input style="margin-left: 10px"
			type="radio" name="rdo_status" id="rdo_status_P" value="P" />
		Pendente <input style="margin-left: 10px" type="radio"
			name="rdo_status" id="rdo_status_B" value="B" /> Bloqueado</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Enviar E-mail para Usuário:</td>
		<td><span class="link" onclick="enviarEmail()"><img
			src="../imagens/email.gif" /> Clique aqui para preencher o e-mail</span></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Reenviar Senha para Usuário:</td>
		<td>
			<input type="radio" name="rdo_reeviar_senha" id="rdo_reeviar_senha_s" value="S" />Sim 
			<input style="margin-left: 10px" type="radio" name="rdo_reeviar_senha" checked="checked" id="rdo_reeviar_senha_n" value="B" /> Não <br />
			<input type="checkbox" name="chk_senha_padrao" id="chk_senha_padrao" value="1" /> Alterar a senha do usuário para a senha padrão: <b>simecdti</b>.
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Justificativa:</td>
		<td id="td_justificativa"><?php echo campo_textarea("justificativa","N","S","",80,5,250) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td>
			<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMembroConsulta()" />
			<input type="button" name="btn_voltar" value="Voltar" onclick="window.location.href='pdeinterativo.php?modulo=principal/listaConsulta&acao=A'" />
		</td>
	</tr>
</table>
</form>