<?php
monta_titulo( "Diretor(a)", '&nbsp' );
$arrDados = recuperaDadosDiretor($_SESSION['pdeinterativo_vars']['pdeid']);
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

	jQuery(function() {
	
		var UA = navigator.userAgent;
		if (UA.indexOf('MSIE') > -1) {
		    alert('Algumas funcionalidades podem não funcionar corretamente com este navegar.\nRecomendamos que você utilize o Mozilla Firefox!');
		}
		
		if( jQuery('[name=usucpf]').val() ){
			jQuery('[name=usucpf]').val( mascaraglobal('###.###.###-##', jQuery('[name=usucpf]').val() ) );
		}
		if( jQuery('[name=usufonenum]').val() ){
			jQuery('[name=usufonenum]').val( mascaraglobal('####-####', jQuery('[name=usufonenum]').val() ) );
		}
		jQuery('[name=usuemail]').blur( function() {
			validaEmail();
		});
		jQuery('[name=cousuemail]').blur( function() {
			if( jQuery(this).val() != jQuery('[name=usuemail]').val() ){
				alert("Confirmação de E-mail inválida!");
			}
		});
			
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
	
	});
	
	function confirmarDadosDiretor(local)
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
		if( erro == 0 && jQuery('[name=usuemail]').val() != jQuery('[name=cousuemail]').val() ){
			alert("Confirmação de E-mail inválida!");
			erro = 1;
		}

		if(erro == 0){
			jQuery("[name='hdn_redirect']").val(local);
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
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
<form name="form_cad_diretor" id="form_cad_diretor"  method="post" action="" >
	<input type="hidden" name="requisicao" value="confirmaDadosDiretor" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita blue" >Orientações</td>
			<td class="SubTituloEsquerda blue normal" >Preencha todos os campos antes de salvar. Os campos com o símbolo ( <?php echo obrigatorio() ?> ) são obrigatórios e podem ser atualizados.</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >CPF:</td>
			<td><?php echo campo_texto("usucpf","N","N","CPF","16","14","###.###.###-##","","","","","","",$arrDados['usucpf'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome:</td>
			<td><?php echo $arrDados['usunome'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Gênero:</td>
			<td><?php echo $arrDados['genero'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >E-mail:</td>
			<td id="td_usuemail" ><?php echo campo_texto("usuemail","S","S","E-mail","40","40","","","","","","","",$arrDados['usuemail'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Confirme o E-mail:</td>
			<td id="td_cousuemail" ><?php echo campo_texto("cousuemail","S","S","E-mail","40","40","","","","","","","",$arrDados['usuemail'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Telefone:</td>
			<td id="td_usufone" >
				<?php echo campo_texto("usufoneddd","N","S","DDD","2","2","","","","","","","",$arrDados['usufoneddd'])?>&nbsp;
				<?php echo campo_texto("usufonenum","S","S","DDD","10","9","####-####","","","","","","",$arrDados['usufonenum'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Celular:</td>
			<td id="td_usufone" >
				<? $dadocelular = $db->pegaLinha("SELECT * FROM pdeinterativo.usuariocelular WHERE usucpf='".$arrDados['usucpf']."'"); ?>
				<?php echo campo_texto("usucelddd","N","S","DDD","2","2","","","","","","","",$dadocelular['usucelddd'])?>&nbsp;
				<?php echo campo_texto("usucelnum","N","S","DDD","10","9","####-####","","","","","","",$dadocelular['usucelnum'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Função / Cargo:</td>
			<td>Diretor(a)</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Apresentacao'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="confirmarDadosDiretor()">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="confirmarDadosDiretor('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola'" >
			</td>
		</tr>
	</table>
</form>