<?php
if($_REQUEST['requisicao']){
	$arrDados = $_REQUEST['requisicao']();
	if($arrDados){
		extract($arrDados);
	}
}
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit;
}

//include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );
?>
<html>
<head>
<title>Dados do Diretor</title>
</head>
<body>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../pdeinterativo2013/js/pdeinterativo2013.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script>

	jQuery(function() {
	
		var UA = navigator.userAgent;
		if (UA.indexOf('MSIE') > -1) {
		    alert('Algumas funcionalidades podem não funcionar corretamente com este navegar.\nRecomendamos que você utilize o Mozilla Firefox!');
		}
	
		<?php if($_SESSION['pdeinterativo2013']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo2013']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo2013']['msg']) ?>
		<?php endif; ?>



		jQuery('[name=usucpf]').blur( function() {
			limparCampos();
			carregaUsuario();
		});
		//jQuery('[name=pdicodinep]').blur( function() {
		//	limparCamposEscola();
		//	carregaEscola();
		//});
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
		
		//if(jQuery('[name=usucpf]').val()){
			//jQuery('[name=usucpf]').val( mascaraglobal('###.###.###-##',jQuery('[name=usucpf]').val() ) );
			//carregaUsuario();
		//}
		
		//if(jQuery('[name=pdicodinep]').val()){
		//	carregaEscola();
		//}
		
	});
	
	
	function editarDiretor(usucpf,inep)
	{
		jQuery("[name=pdicodinep]").val(inep);
		jQuery("[name=usucpf]").val(usucpf);
		carregaUsuario();
	}
	
	function excluirDiretor(usucpf,pdeid)
	{
		if(confirm("Deseja realmente excluir o diretor?")){
			jQuery("[name=usucpf]").val(usucpf);
			jQuery("[name=pdeid]").val(pdeid);
			jQuery("[name=requisicao]").val("excluirDiretor");
			jQuery("[name=form_cad_diretor]").submit();
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

	function enviarEmail()
	{
		var usucpf = jQuery('[name=usucpf]').val();
		if(usucpf){
			usucpf = usucpf.replace("-","");
			usucpf = usucpf.replace(".","");
			usucpf = usucpf.replace(".","");
			url = "../geral/envia_email_usuario.php?usuID=" + usucpf;
	    	janela(url,700,550,"Email_Usuario")
		}else{
			alert('Favor informar o CPF!');
		}
	}
	
	
	function carregaDadosDiretor(pdeid)
	{
	
			jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=AjaxrecuperaDadosDiretor&pdeid="+pdeid,
			   dataType: 'json',
			   success: function(arrDados){
			  
			   		if(!arrDados){
			   			return false;
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
			   		jQuery('[name=estuf]').val( arrDados.regcod );
			   		ajax("filtraMunicipio&estuf=" + arrDados.regcod + "&muncod=" + arrDados.muncod,"td_muncod");
			   		jQuery('[name=usuemail],[name=cousuemail]').val( arrDados.usuemail );
			   		jQuery('[name=usufoneddd]').val( arrDados.usufoneddd );
			   		jQuery('[name=usufonenum]').val( mascaraglobal('####-####',arrDados.usufonenum) );
			   		jQuery('#rdo_status_' + arrDados.status ).attr("checked","checked");
			   		jQuery('[name=justificativa]').val( arrDados.justificativa );
			   }
			 });
			
		
	}
	
	function carregaUsuario()
	{
		
		var usucpf = jQuery('[name=usucpf]').val();
		var pdicodinep = jQuery('[name=pdicodinep]').val();
		
		if(usucpf){
			usucpf = usucpf.replace("-","");
			usucpf = usucpf.replace(".","");
			usucpf = usucpf.replace(".","");
			
			jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=recuperaDiretorPorCPF&usucpf=" + usucpf + "&pdicodinep=" + pdicodinep,
			   dataType: 'json',
			   success: function(arrDados){
			   
			   		if(arrDados != null && arrDados.naopode){
			   			alert(arrDados.naopode);
			   			jQuery('[name=naopode]').val( arrDados.naopode );
			   			jQuery('[name=usucpf]').val("");
			   			return false;
			   		}else{
			   			jQuery('[name=naopode]').val("");
			   		}
			   		
			   		if(arrDados != null && arrDados.confirm){
			   			if(!confirm(arrDados.confirm)){
			   				return false;
			   			}
			   		else{
			   				arrDados = null;
			   				removeDiretoresAntigos(pdicodinep,usucpf);
			   			}
			   		}
			   
			   		if(arrDados == null || arrDados == false){
			   			
			   			var comp = new dCPF();
						comp.buscarDados(usucpf);
						var arrDados = new Object();
						if(!comp.dados.no_pessoa_rf){
							alert('CPF Inválido');
							return false;
						}
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
			   		jQuery('[name=estuf]').val( arrDados.regcod );
			   		ajax("filtraMunicipio&estuf=" + arrDados.regcod + "&muncod=" + arrDados.muncod,"td_muncod");
			   		jQuery('[name=usuemail],[name=cousuemail]').val( arrDados.usuemail );
			   		jQuery('[name=usufoneddd]').val( arrDados.usufoneddd );
			   		jQuery('[name=usufonenum]').val( mascaraglobal('####-####',arrDados.usufonenum) );
			   		jQuery('#rdo_status_' + arrDados.status ).attr("checked","checked");
			   		jQuery('[name=justificativa]').val( arrDados.justificativa );
			   }
			 });
			
		}else{
			limparCampos();
		}
	}
	
	function carregaEscola()
	{
		var pdicodinep = jQuery('[name=pdicodinep]').val();
		
		<?php if($_REQUEST['carregarDiretor'] && !$_REQUEST['usucpf']): ?>
			limparCampos();
		<?php endif; ?>
		
		if(pdicodinep){
				
			jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   dataType: 'json',
			   data: "requisicaoAjax=recuperaEscolaPorCodigoINEP&pdicodinep=" + pdicodinep,
			   success: function(arrDados){
			   		limparCamposEscola();
			   		
			   		
			   		if(arrDados == null){
			   			alert('O código INEP informado não foi priorizado!');
			   		}else{
			   			if(arrDados.naopode){
			   				alert(arrDados.naopode);
			   				jQuery('[name=naopode]').val( arrDados.naopode );
			   				return false;
			   			}
			   				
			   			jQuery('[name=entid]').val(arrDados.entid);
			   			jQuery('[name=pdeid]').val(arrDados.pdeid);
			   			jQuery('[id=td_pdenome]').html( arrDados.pdenome );
			   			jQuery('[id=td_pdiesfera]').html( arrDados.pdiesfera );
			   			jQuery('[id=td_pdilocalizacao]').html( arrDados.pdilocalizacao );
			   			jQuery('[id=td_pdelogradouro]').html( arrDados.pdelogradouro );
			   			jQuery('[id=td_pdinumero]').html( arrDados.pdinumero );
			   			jQuery('[id=td_pdecomplemento]').html( arrDados.pdecomplemento );
			   			jQuery('[id=td_pdebairro]').html( arrDados.pdebairro );
			   			jQuery('[id=td_pdecep]').html( mascaraglobal('###.###-###',arrDados.pdecep) );
			   			jQuery('[id=td_pdeestado]').html( arrDados.estuf );
			   			jQuery('[id=td_pdemunicipio]').html( arrDados.mundescricao );
			   		
			   		}
			   		
			   }
			 });
			
		}else{
			limparCamposEscola();
		}
		
	}
	
	function limparCamposEscola()
	{
		jQuery('[name=entid]').val("");
		jQuery('[id^="td_pd"]').html( "" );
		if(!jQuery('[name=usucpf]').val()){
			limparCampos();
		}
	}
	
	function limparCampos()
	{
		jQuery('[id^="td_u"]').html( "" );
		jQuery('[name=estuf],[name=muncod],[name=usuemail],[name=cousuemail],[name=usufoneddd],[name=usufonenum],[name=justificativa]').val( "" );
		jQuery('[name=rdo_status]').attr("checked","");
	}
	
	function salvarDiretor()
	{
		var erro = 0;
		
		if( erro == 0 && !jQuery('[name=entid]').val() ){
			alert("Escola não encontrada!");
			erro = 1;
			return false;
		}
		
		jQuery("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		if( erro == 0){
			if( !jQuery("#td_usunome").html() || !jQuery("#td_ususexo").html() ){
				alert('CPF Inválido');
				erro = 1;
				return false;
			}
		}
		
		if( erro == 0){
			if( !validaEmail() ){
				erro = 1;
			}
		}
		if( erro == 0 && jQuery('[name=usuemail]').val() != jQuery('[name=cousuemail]').val() ){
			alert("Confirmação de E-mail inválida!");
			erro = 1;
		}
		if( erro == 0 && jQuery('[name=naopode]').val() ){
			alert( jQuery('[name=naopode]').val() );
			erro = 1;
		}
		if(erro == 0){
			jQuery("#form_cad_diretor").submit();
		}
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
	
	function pesquisarEscola()
	{
		janela("pdeinterativo2013.php?modulo=principal/popupPesquisarEscola&acao=A",400,400,"Pesquisar_Escola");
	}
	
	function removeDiretoresAntigos(pdicodinep,usucpf)
	{
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=removeDiretoresAntigos&pdicodinep=" + pdicodinep + "&usucpf=" + usucpf,
			   success: function(resp){
			   }
			 });
	}
	
	jQuery(document).ready(function(){
		carregaEscola();
		<? if ($_REQUEST['carregaDiretorEscola']): ?>
			//carregaDadosDiretor(<?=$_REQUEST['pdeid']?>);
		<?php endif; ?>
	});
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo2013/css/pdeinterativo2013.css"/>
<form name="form_cad_diretor" id="form_cad_diretor"  method="post" action="" >
	<input type="hidden" name="requisicao" value="salvarDiretor" />
	
	<input type="hidden" name="pdeid"   value="<?php echo $arrDados['pdeid'] ?>" />
	<input type="hidden" name="usunome" value="<?php echo $arrDados['usunome'] ?>" />
	<input type="hidden" name="ususexo" value="<?php echo $arrDados['ususexo'] ?>" />
	
	<input type="hidden" name="entid" value="" />
	
	<input type="hidden" name="naopode" value="" />
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloTabela center bold" colspan="2" >Dados da Escola</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Código INEP:</td>
			<td>
				<?php echo campo_texto("pdicodinep","S",($arrDados['pdicodinep'] ? "N" : "S"),"Código INEP","16","14","[#]","","","","","","",$arrDados['pdicodinep'])?>
				 <?php if(!$arrDados['pdicodinep']): ?>
				 	<a href="javascript:pesquisarEscola()" >Pesquisar</a>
				 <?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome da Escola:</td>
			<td id="td_pdenome"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Tipo de Órgão:</td>
			<td id="td_pdiesfera"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Localização:</td>
			<td id="td_pdilocalizacao"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Endereço:</td>
			<td id="td_pdelogradouro"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Número:</td>
			<td id="td_pdinumero"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Complemento:</td>
			<td id="td_pdecomplemento"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Bairro:</td>
			<td id="td_pdebairro"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >CEP:</td>
			<td id="td_pdecep"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_pdemunicipio"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td id="td_pdeestado"></td>
		</tr>
		<tr>
			<td class="SubTituloTabela center bold" colspan="2" >Dados do Diretor</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >CPF:</td>
			<td><?php echo campo_texto("usucpf","S","S","CPF","16","14","###.###.###-##","","","","","","",$arrDados['usucpf'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome:</td>
			<td id="td_usunome"><?php echo $arrDados['usunome'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Gênero:</td>
			<td id="td_ususexo">
				<?php 
				if($arrDados['ususexo']){
					if($arrDados['ususexo'] == "M"){
						echo "Masculino"; 
					}else{
						echo "Feminino";
					}
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >E-mail:</td>
			<td><?php echo campo_texto("usuemail","S","S","E-mail","40","40","","")?> <img onmouseover="return escape('Atenção, este e-mail será utilizado para enviar a senha de acesso ao sistema para o usuário!')" class="link img_middle" src="../imagens/icon_campus_2.png" /> <span id="reenvio_email" class="msg_erro bold" ></span></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Confirme o E-mail:</td>
			<td>
				<?php $cousuemail = $usuemail ?>
				<?php echo campo_texto("cousuemail","S","S","E-mail","40","40","","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Telefone:</td>
			<td>
				<?php echo campo_texto("usufoneddd","N","S","DDD","2","2","","")?>&nbsp;
				<?php echo campo_texto("usufonenum","S","S","DDD","10","9","####-####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Status Geral do Usuário:</td>
			<td>
				<input 						    type="radio" name="rdo_status" <?php echo $arrDados['usucpf'] ? "checked='checked'" : "" ?> id="rdo_status_A" value="A" /> Ativo
				<input style="margin-left:10px" type="radio" name="rdo_status" id="rdo_status_P" value="P" /> Pendente
				<input style="margin-left:10px" type="radio" name="rdo_status" id="rdo_status_B" value="B" /> Bloqueado
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Enviar E-mail para Usuário:</td>
			<td><span class="link" onclick="enviarEmail()" ><img src="../imagens/email.gif"  /> Clique aqui para preencher o e-mail</span></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Reenviar Senha para Usuário:</td>
			<td>
				<input 							type="radio" name="rdo_reeviar_senha" id="rdo_reeviar_senha_s" value="S" /> Sim
				<input style="margin-left:10px" type="radio" name="rdo_reeviar_senha" checked="checked" id="rdo_reeviar_senha_n" value="B" /> Não
				<input type="checkbox" name="chk_senha_padrao" id="chk_senha_padrao" value="1" /> Alterar a senha do usuário para a senha padrão: <b>simecdti</b>.
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Justificativa:</td>
			<td>
				<?php echo campo_textarea("justificativa","N","S","",80,5,250) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" name="btn_novo" value="Novo" onclick="limparCampos(); jQuery('[name=usucpf]').val('');" />
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDiretor();" />
				<input type="button" name="btn_fechar" value="Fechar" onclick="window.close()" />
			</td>
		</tr>
	</table>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
</body>
</html>