<script language="javascript" type="text/javascript">
	jQuery.noConflict();

	function salvarDados() {
		jQuery('#iuscpf').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()));
		
		if(jQuery('#iuscpf').val()=='') {
			alert('Informe o CPF!');
			jQuery('#iuscpf').focus();
			return false;
		}

		if(!validar_cpf(jQuery('#iuscpf').val())){
			alert('CPF inválido!');
			jQuery('#iuscpf').focus();
			return false;
		}
		
		if(jQuery('#iusnome').val()==''){
			alert('Informe o Nome!');
			jQuery('#iusnome').focus();
			return false;
		}
		
		if(jQuery('#iusdatanascimento').val()==''){
			alert('Informe a Data de Nascimento!');
			jQuery('#iusdatanascimento').focus();
			return false;
		}
		
		if(!validaData(document.getElementById('iusdatanascimento'))){
			alert('Data de Nascimento inválida!');
			jQuery('#iusdatanascimento').focus();
			return false;
		}
		
		var iussexo_r = jQuery("[name^='iussexo']:enabled:checked").length;
		
		if(iussexo_r==0) {
			alert('Informe o Sexo!');
			return false;
		}
		
		if(jQuery('#iusnomemae').val()==''){
			alert('Informe o Nome da mãe!');
			jQuery('#iusnomemae').focus();
			return false;
		}

		if(jQuery('#estuf_nascimento').val()==''){
			alert('Informe o Estado - Local Nascimento!');
			jQuery('#estuf_nascimento').focus();
			return false;
		}
		
		if(document.getElementById('muncod_nascimento')){
			if(jQuery('#muncod_nascimento').val()=='') {
				alert('Informe o Município - Local Nascimento!');
				jQuery('#muncod_nascimento').focus();
				return false;
			}
		} else {
			alert('Informe o Município - Local Nascimento!');
			jQuery('#muncod_nascimento').focus();
			return false;
		}
		
		if(jQuery('#nacid').val()==''){
			alert('Informe a Nacionalidade!');
			jQuery('#nacid').focus();
			return false;
		}

		if(jQuery('#eciid').val()==''){
			alert('Informe o Estado Civil!');
			jQuery('#eciid').focus();
			return false;
		}

		if(jQuery('#eciid').val()=='<?php echo ECI_CASADO; ?>' || jQuery('#eciid').val()=='<?php echo ECI_UNIAO_ESTAVEL; ?>') {
			if(jQuery('#iusnomeconjuge').val()=='') {
				alert('Informe o Nome do Conjuge!');
				jQuery('#iusnomeconjuge').focus();
				return false;
			}
		}
		
		if(jQuery('#itdufdoc').val()==''){
			alert('Informe o Estado - Documento!');
			jQuery('#itdufdoc').focus();
			return false;
		}
		
		if(jQuery('#tdoid').val()==''){
			alert('Informe o Tipo - Documento!');
			jQuery('#tdoid').focus();
			return false;
		}
		
		if(jQuery('#itdnumdoc').val()==''){
			alert('Informe o Número do Documento!');
			jQuery('#itdnumdoc').focus();
			return false;
		}
		
		if(jQuery('#itddataexp').val()==''){
			alert('Informe a Data Expedição!');
			jQuery('#itddataexp').focus();
			return false;
		}
		
		if(!validaData(document.getElementById('itddataexp'))){
			alert('Data Expedição inválida!');
			jQuery('#itddataexp').focus();
			return false;
		}
		
		if(jQuery('#itdnoorgaoexp').val()==''){
			alert('Informe o Orgão Expedidor!');
			jQuery('#itdnoorgaoexp').focus();
			return false;
		}
		
		if(jQuery('#tvpid').val()=='') {
			alert('Informe o Vínculo!');
			jQuery('#tvpid').focus();
			return false;
		}
		
		if(jQuery('#funid').val()==''){
			alert('Informe a Função!');
			jQuery('#funid').focus();
			return false;
		}
		
		if(jQuery('#foeid').val()==''){
			alert('Informe a Formação (Escolaridade)!');
			jQuery('#foeid').focus();
			return false;
		}
		
		if(jQuery('#foeid').val()=='<?=FOE_ESPECIALIZACAO ?>' || jQuery('#foeid').val()=='<?=FOE_MESTRADO ?>' || jQuery('#foeid').val()=='<?=FOE_DOUTORADO ?>' || jQuery('#foeid').val()=='<?=FOE_SUPERIOR_COMPLETO_PEDAGOGIA ?>' || jQuery('#foeid').val()=='<?=FOE_SUPERIOR_COMPLETO_LICENCIATURA ?>' || jQuery('#foeid').val()=='<?=FOE_SUPERIOR_COMPLETO_OUTRO ?>') {	   

			if(document.getElementById('cufid')) {
				if(jQuery('#cufid').val()=='') {
					alert('Informe a Área de Formação!');
					jQuery('#cufid').focus();
					return false;
				}
			} else {
				alert('Selecione a Área');
				return false;
			}
		
		}
		
		var iufsituacaoformacao_r = jQuery("[name^='iufsituacaoformacao']:enabled:checked").length;
		
		if(iufsituacaoformacao_r==0) {
			alert('Informe a Situação - Formação!');
			return false;
		}
		
		if(jQuery('#iufdatainiformacao').val()==''){
			alert('Informe o Início - Formação!');
			jQuery('#iufdatainiformacao').focus();
			return false;
		}
		
		if(!validaData(document.getElementById('iufdatainiformacao'))){
			alert('Início - Formação inválida!');
			jQuery('#iufdatainiformacao').focus();
			return false;
		}
		
		if(document.getElementById('iufsituacaoformacao_C').checked){
			if(jQuery('#iufdatafimformacao').val()=='') {
				alert('Informe o Fim - Formação!');
				jQuery('#iufdatafimformacao').focus();
				return false;
			}
			
			if(!validaData(document.getElementById('iufdatafimformacao'))){
				alert('Fim - Formação inválida!');
				jQuery('#iufdatafimformacao').focus();
				return false;
			}
			
			var data1    = jQuery('#iufdatainiformacao').val();
			data1 		 = data1.split('/');
			var data2    = jQuery('#iufdatafimformacao').val();
			data2 		 = data2.split('/');
		
			if(data2[2]+data2[1]+data2[0] < data1[2]+data1[1]+data1[0]){
				alert('Data de Inicio de Formação é maior do que Data Fim!');
				jQuery('#iufdatainiformacao').focus();
				return false;
			}
		}
		
		jQuery('#iencep').val(mascaraglobal('#####-###',jQuery('#iencep').val()));
		
		if(jQuery('#iencep').val()=='') {
			alert('Informe o CEP!');
			jQuery('#iencep').focus();
			return false;
		}
		
		if(document.getElementById('muncod_endereco')){
			if(jQuery('#estuf_endereco').val()=='') {
				alert('Informe o Estado - Endereço!');
				jQuery('#estuf_endereco').focus();
				return false;
			}
		} else {
			alert('Informe o Estado - Endereço!');
			jQuery('#estuf_endereco').focus();
			return false;
		}
		
		if(document.getElementById('muncod_endereco')){
			if(jQuery('#muncod_endereco').val()=='') {
				alert('Informe o Município - Endereço!');
				jQuery('#muncod_endereco').focus();
				return false;
			}
		} else {
			alert('Informe o Município - Endereço!');
			jQuery('#muncod_endereco').focus();
			return false;
		}
		
		var ientipo_r = jQuery("[name^='ientipo']:enabled:checked").length;
		
		if(ientipo_r==0) {
			alert('Informe o Tipo - Endereço!');
			return false;
		}

		if(jQuery('#ienlogradouro').val()==''){
			alert('Informe o Logradouro!');
			jQuery('#ienlogradouro').focus();
			return false;
		}
		
		if(jQuery('#ienbairro').val()==''){
			alert('Informe o Bairro!');
			jQuery('#ienbairro').focus();
			return false;
		}
		
		jQuery('#iennumero').val(mascaraglobal('#####',jQuery('#iennumero').val()));
		
		if(jQuery('#iennumero').val()==''){
			alert('Informe o Número!');
			jQuery('#iennumero').focus();
			return false;
		}
		
		if(jQuery('#itedddtel_T').val()==''){
			alert('Informe o DDD Trabalho!');
			jQuery('#itedddtel_T').focus();
			return false;
		}
		
		if(jQuery('#itedddtel_T').val().length!=2){
			alert('DDD Trabalho deve possuir 2 dígitos!');
			jQuery('#itedddtel_T').focus();
			return false;
		}
		
		if(jQuery('#itenumtel_T').val()==''){
			alert('Informe o Telefone Trabalho!');
			jQuery('#itenumtel_T').focus();
			return false;
		}
		
		if(jQuery('#itenumtel_T').val().length!=9){
			alert('Telefone Trabalho deve possuir 8 dígitos!');
			jQuery('#itenumtel_T').focus();
			return false;
		}
		
		if(jQuery('#itedddtel_R').val()!=''){
			jQuery('#itedddtel_R').val(mascaraglobal('##',jQuery('#itedddtel_R').val()));
		}
		
		if(jQuery('#itenumtel_R').val()!=''){
			jQuery('#itenumtel_R').val(mascaraglobal('####-####',jQuery('#itenumtel_R').val()));
		}

		
		if(jQuery('#iusemailprincipal').val()==''){
			alert('Informe o E-mail Principal!');
			jQuery('#iusemailprincipal').focus();
			return false;
		}
		
	    if(!validaEmail(jQuery('#iusemailprincipal').val())){
	    	alert('E-mail Principal inválido!');
	    	jQuery('#iusemailprincipal').focus();
	    	return false;
	    }

		if(jQuery('#iusemailopcional').val()!=''){
		    if(!validaEmail(jQuery('#iusemailopcional').val())) {
		    	alert('E-mail Opcional inválido');
		    	jQuery('#iusemailopcional').focus();
		    	return false;
		    }
	    }
	    
	    var email_unico = true;
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: 'requisicao=verificarEmailUnico&iusemailprincipal='+jQuery('#iusemailprincipal').val()+'&iusd='+jQuery('#iusd').val(),
	   		async: false,
	   		success: function(html){
	   			if(html) {
	   				alert('Por questões de segurança, o e-mail deve ser único por pessoa. Constatamos que o email utilizado ('+jQuery('#iusemailprincipal').val()+') se encontra cadastrado em outros usuários:\n\n'+html);
					email_unico = false;
	   			}
	   		}
		});
		
		if(email_unico==false){
			return false;
		}
	    
		if(jQuery('#iusagenciasugerida').val()==''){
			alert('Informe a Agência!');
			jQuery('#iusagenciasugerida').focus();
			return false;
		}
		
		if(jQuery('#iusagenciaend').val()==''){
			alert('Informe o Endereço!');
			jQuery('#iusagenciaend').focus();
			return false;
		}
	    
	    if(jQuery('#iustermocompromisso').attr('checked')==false){
	    	alert('É necessário aceitar o Termo de Compromisso!');
	    	jQuery('#iustermocompromisso').focus();
	    	return false;
	    }
	    
	    if(document.getElementById('tpeatuacaoinicio_mes')){
			if(jQuery('#tpeatuacaoinicio_mes').val()=='') {
				alert('Informe o Inicío - Mês - Período de Atuação!');
				jQuery('#tpeatuacaoinicio_mes').focus();
				return false;
			}
		}

	    if(document.getElementById('tpeatuacaoinicio_ano')){
			if(jQuery('#tpeatuacaoinicio_ano').val()=='') {
				alert('Informe o Inicío - Ano - Período de Atuação!');
				jQuery('#tpeatuacaoinicio_ano').focus();
				return false;
			}
			
			//Regra fixa - solicitada
			if(jQuery('#tpeatuacaoinicio_ano').val()=='2012'){
				if(parseInt(jQuery('#tpeatuacaoinicio_mes').val())< 10) {
					alert('Não aceita data inicial anterior a Outubro/2012!');
					jQuery('#tpeatuacaoinicio_mes').focus();
					return false;
				}
			}

		}
		
	    if(document.getElementById('tpeatuacaofim_mes')){
			if(jQuery('#tpeatuacaofim_mes').val()=='') {
				alert('Informe o Fim - Mês - Período de Atuação!');
				jQuery('#tpeatuacaofim_mes').focus();
				return false;
			}
		}

	    if(document.getElementById('tpeatuacaofim_mes')){
			if(jQuery('#tpeatuacaofim_ano').val()=='') {
				alert('Informe o Fim - Ano - Período de Atuação!');
				jQuery('#tpeatuacaofim_ano').focus();
				return false;
			}
			
			if(jQuery('#tpeatuacaoinicio_ano').val()+jQuery('#tpeatuacaoinicio_mes').val()>jQuery('#tpeatuacaofim_ano').val()+jQuery('#tpeatuacaofim_mes').val()) {
				alert('Período de atuação - Término deve ser maior do que Período de atuação - Inicío!');
				jQuery('#tpeatuacaoinicio_ano').focus();
				return false;
			}
		}

	    divCarregando();
		jQuery('#formulario').submit();
	}

	function verificaEstadoCivil(eciid){
		if(eciid=='<?php echo ECI_CASADO; ?>' || eciid=='<?php echo ECI_UNIAO_ESTAVEL; ?>') {
			jQuery('#tr_iusnomeconjuge').css('display','');	
		} else {
			jQuery('#iusnomeconjuge').val('');
			jQuery('#tr_iusnomeconjuge').css('display','none');
		}
	}

	function verificaFormacaoEscolar(foeid){
		if(foeid) {
		   	jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: 'requisicao=verificarFormacaoCompleta&foeid='+foeid,
		   		async: false,
		   		success: function(res){
					jQuery("[name^='iufsituacaoformacao'][value=C]").attr('disabled',false);
					jQuery("[name^='iufsituacaoformacao'][value=I]").attr('disabled',false);
					jQuery("[name^='iufsituacaoformacao'][value=E]").attr('disabled',false);
	
					jQuery("[name^='iufsituacaoformacao'][value=C]").attr('checked',false);
					jQuery("[name^='iufsituacaoformacao'][value=I]").attr('checked',false);
					jQuery("[name^='iufsituacaoformacao'][value=E]").attr('checked',false);
					if(res=='T') {
						jQuery("[name^='iufsituacaoformacao'][value=C]").attr('checked',true);
						jQuery("[name^='iufsituacaoformacao'][value=I]").attr('disabled',true);
						jQuery("[name^='iufsituacaoformacao'][value=E]").attr('disabled',true);
					}
					if(res=='F') {
						jQuery("[name^='iufsituacaoformacao'][value=I]").attr('checked',true);
						jQuery("[name^='iufsituacaoformacao'][value=C]").attr('disabled',true);
						jQuery("[name^='iufsituacaoformacao'][value=E]").attr('disabled',true);
					}
		   		}
			});
		}

		if(foeid=='<?php echo FOE_ESPECIALIZACAO; ?>' || foeid=='<?php echo FOE_MESTRADO; ?>' || 
		   foeid=='<?php echo FOE_DOUTORADO; ?>' || foeid=='<?php echo FOE_SUPERIOR_COMPLETO_PEDAGOGIA; ?>' || 
		   foeid=='<?php echo FOE_SUPERIOR_COMPLETO_LICENCIATURA; ?>' || foeid=='<?php echo FOE_SUPERIOR_COMPLETO_OUTRO; ?>' ||
		   foeid=='<?php echo FOE_SUPERIOR_INCOMPLETO; ?>') {
		   
		   	jQuery('#cufcodareageral').val('');
			jQuery('#cufcodareageral').attr('disabled','');
			jQuery('#td_listaCursosFormacao').html('Selecione uma Área');

		} else {	
		   	jQuery('#cufcodareageral').val('');
			jQuery('#cufcodareageral').attr('disabled','disabled');
			jQuery('#td_listaCursosFormacao').html('Selecione uma Área');
		}

	}
	
	function listarAgencias(){
		jQuery("#modalAgencia").dialog({
			draggable:true,
			resizable:true,
			width: 800,
			height: 600,
			modal: true,
			close: function(){} 
		});
	}

	function buscarAgencias(muncod){
		if(document.getElementById('estuf_agencias')) {
			if(jQuery('#muncod_agencias').val()!='') {
				ajaxatualizar('requisicao=listarAgencias&muncod='+muncod,'listaAgencias');
				jQuery('#tr_agencia').css('display','');
			} else {
				alert('Selecione um município');
				return false;
			}
		} else {
			alert('Selecione um estado');
			return false;
		}
	}

	function carregarCursosFormacao(cod){
		if(cod) {
			ajaxatualizar('requisicao=listarCursosFormacao&cufcodareageral='+cod,'td_listaCursosFormacao');
		} else {
			jQuery('#td_listaCursosFormacao').html('Selecione uma Área');
		}
	}

	function selecionarAgencia(){
		if(jQuery('#dados_agencia').val()!='') {
			var dadosagencia = jQuery('#dados_agencia').val();
			dadosagencia = dadosagencia.split('_');
			var iusagenciasugerida = dadosagencia[0];
			var cep = dadosagencia[1];
			var iusagenciaend = 'Endereço não encontrado';
			
			jQuery.ajax({
		   		type: "POST",
		   		url: '/geral/consultadadosentidade.php',
		   		data: 'requisicao=pegarenderecoPorCEP&endcep='+cep,
		   		async: false,
		   		success: function(resposta){
					var dadosendereco = resposta.split("||");
					iusagenciaend = 'CEP: '+mascaraglobal('#####-###',cep)+', Logradouro: '+dadosendereco[0]+', Bairro: '+dadosendereco[1]+', Cidade: '+dadosendereco[2]+', UF: '+dadosendereco[3];
				}
			});
			
			jQuery('#iusagenciasugerida').val(iusagenciasugerida);
			jQuery('#iusagenciaend').val(iusagenciaend);
			jQuery('#modalAgencia').dialog('close');
		} else {
			alert('Selecione uma agência');
			return false;
		}
	}

	function anexarDocumentoDesignacao(){
		if(jQuery('#documentodesignacao').val()=='') {
			alert('Selecione um arquivo!');
			return false;	
		}
		jQuery('#requisicao').val('anexarDocumentoDesignacao');
		jQuery('#formulario').submit();
	}

	function removerDocumentoDesignacao(iuaid){
		if(confirm("Deseja realmente Documento de Designação?")){
			jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=dadosies',
			data: { requisicao: 'removerDocumentoDesignacao', iuaid: iuaid, iusd: jQuery('#iusd').val()},
			async: false,
			success: function(data) {
					alert('Documento de Designação excluído com sucesso!');
					jQuery("#divDocumentoDesignacao").html(data);
		    	}
			});
		}
	}	

	function atualizarNomeUsuario(){
		window.location = 'sisfor.php?modulo=principal/coordenador/coordenador&acao=A&aba=dados&requisicao=atualizarNomeUsuario&iuscpf=<?php echo $iuscpf; ?>';
	}
</script>

<?php 
if($_SESSION['sisfor']){
	$usuario = carregarDadosIdentificacao($_SESSION['sisfor'],PFL_COORDENADOR_INST);
	if($usuario){
		extract($usuario[0]); 
	}
} 

if($_SESSION['REQUEST']){
	extract($_SESSION['REQUEST']);	
	$iuscpf = corrige_cpf($iuscpf);
	
	$telefones['R']['itedddtel'] = $itedddtel[R];
	$telefones['T']['itedddtel'] = $itedddtel[T];
	$telefones['C']['itedddtel'] = $itedddtel[C];
	$telefones['F']['itedddtel'] = $itedddtel[F];

	$telefones['R']['itenumtel'] = $itenumtel[R];
	$telefones['T']['itenumtel'] = $itenumtel[T];
	$telefones['C']['itenumtel'] = $itenumtel[C];
	$telefones['F']['itenumtel'] = $itenumtel[F];
	$iencep = str_replace("-","",$iencep);
}

monta_titulo('Dados Coordenador Institucional','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

if($_SESSION['sisfor']['unicod']){
	$docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
	$esdid = pegarEstadoDocumento($docid);
} 

if($esdid == WF_PLAN_FECHADO){
	$planejamentofechado = true;
} else {
	$planejamentofechado = false;
}
?>

<form method="post" id="formulario" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="atualizarDadosIdentificacaoUsuario"> 
	<input type="hidden" name="iusd" id="iusd" value="<?php echo $iusd; ?>"> 
	<input type="hidden" name="tpeid" id="tpeid" value="<?php echo $tpeid; ?>"> 
	<input type="hidden" name="location" id="location" value="sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=dadosies">
	<input type="hidden" name="msgalert" id="msgalert" value="Coordenador da IES salvo com sucesso!">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
		<tr>
			<td class="subtituloDireita" width="25%">Orientações:</td>
			<td>
				<p align="justify">
					Este formulário é integrado ao SGB (Sistema Geral de Bolsas). Preencha TODOS os campos, lembrando que estas informações são obrigatórias para o recebimento da bolsa de estudo. Caso haja inconsistências, a bolsa não poderá ser paga. 
					Ao preencher os dados no SisFor não é será necessário fazer nenhum cadastro adicional no SGB.
				</p>
				<p align="justify">
					O formulário permanece editável a qualquer tempo de modo que, havendo alterações nos dados, os respectivos campos deverão ser imediatamente atualizados. Observe atentamente as seguintes questões:
					- Os dados indicados no campo "Documento de identificação" referem-se ao bolsista (não ao cônjuge do bolsista);
					- No campo "Área de formação", se a situação for "Completo" é obrigatório indicar as datas de início e término. Nos dois outros casos, é obrigatória apenas a data de início;
					- No campo "Agência do Banco do Brasil sugerida", pode ser escolhida uma agência onde bolsista já seja correntista, mas uma nova conta será aberta pelo FNDE e o banco escolhido só pode ser o Banco do Brasil;
					- Se você já é bolsista do FNDE/MEC e não deseja substituir sua bolsa atual pela bolsa de Coordenador Institucional, não esqueça de assinalar o penúltimo botão antes do salvamento, caso contrário, o SGB processa a primeira ordem de pagamento enviada (e não realiza duplo pagamento para o mesmo CPF).
				</p>
				<p align="justify">
					Em caso de dúvida, envie um e-mail para <?php echo $_SESSION['email_sistema']; ?>. 
				</p>			
			</td>
		</tr>			
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Dados Pessoais</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">CPF</td>
			<td><?php echo campo_texto('iuscpf', 'S', 'N', 'CPF', '15', '14', '###.###.###-##', '', '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##")); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Nome</td>
			<td>
				<?php echo campo_texto('iusnome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome"', ''); ?>
				<?php if($iustermocompromisso != 't'){ ?>
				<span style="cursor: pointer;" onclick="atualizarNomeUsuario();">
					<img src="../imagens/refresh2.gif">Caso o nome não esteja condizente com CPF. Clique para atualizar.</span>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Data de Nascimento</td>
			<td><?php echo campo_data2('iusdatanascimento','S', 'S', 'Data nascimento', 'S', '', '', '', '', '', 'iusdatanascimento'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Local Nascimento</td>
			<td>
				<table>
					<tr>
						<td>Estado:</td>
						<td>
							<?php 
							$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
							$db->monta_combo('estuf_nascimento', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF2', '', '', '', 'S', 'estuf_nascimento', ''); ?>
						</td>
						<td>Município:</td>
						<td id="td_municipio2">
							<?php if($estuf_nascimento){
								$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf = '{$estuf_nascimento}' ORDER BY mundescricao";
								$db->monta_combo('muncod_nascimento', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_nascimento', ''); 
							} else {
								echo "Selecione uma UF";
							} ?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Sexo</td>
			<td>
				<input type="radio" name="iussexo" value="M" <?php echo(($iussexo=="M") ? "checked" : ""); ?>> Masculino 
				<input type="radio" name="iussexo" value="F" <?php echo(($iussexo=="F") ? "checked" : ""); ?>> Feminino
				&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Nome da mãe</td>
			<td><?php echo campo_texto('iusnomemae', "S", "S", "Nome da mãe", 60, 60, "", "", '', '', 0, 'id="iusnomemae"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Nacionalidade</td>
			<td>
				<?php if(!$nacid) $nacid = NAC_BRASIL;
				$sql = "SELECT 		nacid AS codigo, 
									nacdesc AS descricao 
						FROM 		sisfor.nacionalidade 
						ORDER BY 	naccodpais";
				$db->monta_combo('nacid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'nacid', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Estado Civil</td>
			<td>
				<?php
				$sql = "SELECT 		eciid AS codigo, 
									ecidesc AS descricao 
						FROM 		sisfor.estadocivil 
						ORDER BY 	ecidesc";
				
				$db->monta_combo('eciid', $sql, 'S', 'Selecione', 'verificaEstadoCivil', '', '', '200', 'S', 'eciid', '');?>
			</td>
		</tr>
		<tr id="tr_iusnomeconjuge" <?php echo(($eciid==ECI_CASADO || $eciid==ECI_UNIAO_ESTAVEL)?"":"style=\"display:none;\"") ?>>
			<td class="SubTituloDireita" width="20%">Nome do Cônjuge</td>
			<td><?php echo campo_texto('iusnomeconjuge', "S", "S", "Nome do Conjuge", 67, 150, "", "", '', '', 0, 'id="iusnomeconjuge"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Documento de identificação</td>
			<td>
				<table>
					<tr>
						<td>Estado</td>
						<td>
							<?php $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
							$db->monta_combo('itdufdoc', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'itdufdoc', ''); ?>
						</td>
					</tr>
					<tr>
						<td>Tipo</td>
						<td>
							<?php 
							if(!$tdoid) $tdoid = TDO_RG;
							$sql = "SELECT tdoid as codigo, tdodesc as descricao FROM sisfor.tipodocumento WHERE tdostatus='A' ORDER BY tdodesc";
							$db->monta_combo('tdoid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tdoid', ''); ?>
						</td>
					</tr>
					<tr>
						<td>Número do Documento</td>
						<td><?php echo campo_texto('itdnumdoc', "S", "S", "Número do Documento", 30, 30, "", "", '', '', 0, 'id="itdnumdoc"' ); ?></td>
					</tr>
					<tr>
						<td>Data Expedição</td>
						<td><?php echo campo_data2('itddataexp','S', 'S', 'Data Expedição', 'S', '', '', '', '', '', 'itddataexp'); ?></td>
					</tr>
					<tr>
						<td>Orgão Expedidor</td>
						<td><?php echo campo_texto('itdnoorgaoexp', "S", "S", "Orgão Expedidor", 50, 50, "", "", '', '', 0, 'id="itdnoorgaoexp"' ); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Dados Profissionais</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Vínculo</td>
			<td>
				<?php
				$sql = "SELECT tvpid as codigo, tvpdsc as descricao FROM public.tipovinculoprofissional WHERE tvpstatus='A'";
				$db->monta_combo('tvpid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'tvpid', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Função atual ou similar</td>
			<td>
				<?php
				$sql = "SELECT funid as codigo, funnome as descricao FROM sisfor.funcao WHERE funstatus='A'";
				$db->monta_combo('funid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'funid', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Formação (Escolaridade)</td>
			<td>
				<?php
				$sql = "SELECT foeid as codigo, foecod||' - '||foedesc as descricao FROM sisfor.formacaoescolaridade WHERE foestatus='A' ORDER BY foecod";
				$db->monta_combo('foeid', $sql, 'S', 'Selecione', 'verificaFormacaoEscolar', '', '', '', 'S', 'foeid', '');
	
				$disabled_iufsituacaoformacao = array();
				$habil_areaformacao = "S";
	
				if($foeid) {
					$foecompleto = $db->pegaUm("SELECT foecompleto FROM sisfor.formacaoescolaridade WHERE foeid='".$foeid."'");
					if($foecompleto=="t") {
						$iufsituacaoformacao = "C";
						$disabled_iufsituacaoformacao = array("I","A");
					}
					if($foecompleto=="f") {
						$iufsituacaoformacao = "I";
						$disabled_iufsituacaoformacao = array("C","A");
					}
					if($foeid==FOE_ESPECIALIZACAO || $foeid==FOE_MESTRADO ||
					$foeid==FOE_DOUTORADO || $foeid==FOE_SUPERIOR_COMPLETO_PEDAGOGIA ||
					$foeid==FOE_SUPERIOR_COMPLETO_LICENCIATURA || $foeid==FOE_SUPERIOR_COMPLETO_OUTRO || $foeid==FOE_SUPERIOR_INCOMPLETO) {
						 
						$habil_areaformacao = "S";
						 
					} else {
						$habil_areaformacao = "N";
					}
				}?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Área de formação</td>
			<td>
				<table>
					<tr>
						<td>Área</td>
						<td>
							<?php 
							$sql = "SELECT 		cufcodareageral AS codigo, 
												cufareageraldesc AS descricao 
									FROM 		sisfor.cursoformacao 
									WHERE 		cufstatus='A' 
									GROUP BY 	cufcodareageral, cufareageraldesc 
									ORDER BY 	cufareageraldesc";
							$db->monta_combo('cufcodareageral', $sql, $habil_areaformacao, 'Selecione', 'carregarCursosFormacao', '', '', '', 'S', 'cufcodareageral', ''); ?>
						</td>
					</tr>
					<tr>
						<td>Área de Formação (Selecione a mais relevante, se houver mais de uma)</td>
						<td id="td_listaCursosFormacao">
							<?php
							if($cufcodareageral){
								$sql = "SELECT 		cufid AS codigo, 
													cufcursodesc AS descricao 
										FROM 		sisfor.cursoformacao 
										WHERE 		cufstatus = 'A' 
										AND 		cufcodareageral = '{$cufcodareageral}' 
										ORDER BY 	cufcursodesc";
								$db->monta_combo('cufid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'cufid', ''); 
							} ?>
						</td>
					</tr>
					<tr>
						<td>Situação formação</td>
						<td>
							<input type="radio" name="iufsituacaoformacao" id="iufsituacaoformacao_C" value="C" <?php echo(($iufsituacaoformacao=="C") ? "checked" : "") ?><?php echo((in_array("C",$disabled_iufsituacaoformacao)) ? "disabled" : "") ?>> Completo 
							<input type="radio" name="iufsituacaoformacao" value="I" <?php echo(($iufsituacaoformacao=="I") ? "checked" : "") ?> <?php echo((in_array("I",$disabled_iufsituacaoformacao)) ? "disabled" : "") ?>> Incompleto 
							<input type="radio" name="iufsituacaoformacao" value="E" <?php echo(($iufsituacaoformacao=="E") ? "checked" : "") ?> <?php echo((in_array("A",$disabled_iufsituacaoformacao)) ? "disabled" : "") ?>> Em andamento
							&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
						</td>
					</tr>
					<tr>
						<td>Início</td>
						<td><?php echo campo_data2('iufdatainiformacao','S', 'S', 'Início', 'S', '', '', '', '', '', 'iufdatainiformacao'); ?></td>
					</tr>
					<tr>
						<td>Fim</td>
						<td><?php echo campo_data2('iufdatafimformacao','S', 'S', 'Fim', 'S', '', '', '', '', '', 'iufdatafimformacao'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Contatos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Endereço</td>
			<td>
				<table>
					<tr>
						<td>Tipo</td>
						<td>
							<input type="radio" name="ientipo" value="R" <?php echo(($ientipo=="R") ? "checked" : ""); ?>> Residencial 
							<input type="radio" name="ientipo" value="P" <?php echo(($ientipo=="P") ? "checked" : ""); ?>> Profissional 
							<input type="radio" name="ientipo" value="D" <?php echo(($ientipo=="D") ? "checked" : ""); ?>> Descanso
							&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
						</td>
					</tr>
					<tr>
						<td>CEP</td>
						<td><?php echo campo_texto('iencep', "S", "S", "CEP", 9, 10, "#####-###", "", '', '', 0, 'id="iencep"', '', mascaraglobal($iencep,"#####-###"), 'if(this.value.length==9){carregarEnderecoPorCEP_coordenadorLocal(this.value);}else{limparEnderecoPorCEP_coordenadorLocal();}' ); ?></td>
					</tr>
					<tr>
						<td>UF</td>
						<td>
							<?php
							$sql = "SELECT 		estuf AS codigo, 
												estuf AS descricao 
									FROM 		territorios.estado 
									ORDER BY 	estuf";
							$db->monta_combo('estuf_endereco', $sql, 'N', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '');?>
						</td>
					</tr>
					<tr>
						<td>Município</td>
						<td id="td_municipio3">
							<?php 
							if($muncod_endereco){
								$sql = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod = '{$muncod_endereco}'";
								$mundescricao = $db->pegaUm($sql);
								echo $mundescricao;
								echo '<input type="hidden" id="muncod_endereco" name="muncod_endereco" value="'.$muncod_endereco.'">';
							} else {
								echo "Digite CEP";
							}?>
						</td>
					</tr>
					<tr>
						<td>Logradouro</td>
						<td><?php echo campo_texto('ienlogradouro', "S", "S", "Logradouro", 60, 150, "", "", '', '', 0, 'id="ienlogradouro"' ); ?></td>
					</tr>
					<tr>
						<td>Bairro</td>
						<td><?php echo campo_texto('ienbairro', "S", "S", "Bairro", 60, 60, "", "", '', '', 0, 'id="ienbairro"' ); ?></td>
					</tr>
					<tr>
						<td>Complemento</td>
						<td><?php echo campo_texto('iencomplemento', "N", "S", "Complemento", 50, 50, "", "", '', '', 0, 'id="iencomplemento"' ); ?></td>
					</tr>
					<tr>
						<td>Número</td>
						<td><?php echo campo_texto('iennumero', "S", "S", "Número", 6, 5, "#####", "", '', '', 0, 'id="iennumero"' ); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Telefone</td>
			<td>
			<table>
				<tr>
					<td>Residencial</td>
					<td><?php echo campo_texto('itedddtel[R]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_R"', '', $telefones['R']['itedddtel'] ); ?>
					<?php echo campo_texto('itenumtel[R]', "N", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_R"', '', $telefones['R']['itenumtel'] ); ?></td>
				</tr>
				<tr>
					<td>Trabalho</td>
					<td><?php echo campo_texto('itedddtel[T]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_T"', '', $telefones['T']['itedddtel'] ); ?>
					<?php echo campo_texto('itenumtel[T]', "S", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_T"', '', $telefones['T']['itenumtel'] ); ?></td>
				</tr>
				<tr>
					<td>Celular</td>
					<td><?php echo campo_texto('itedddtel[C]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_C"', '', $telefones['C']['itedddtel'] ); ?>
					<?php echo campo_texto('itenumtel[C]', "N", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_C"', '', $telefones['C']['itenumtel'] ); ?></td>
				</tr>
				<tr>
					<td>Outro</td>
					<td><?php echo campo_texto('itedddtel[F]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_F"', '', $telefones['F']['itedddtel'] ); ?>
					<?php echo campo_texto('itenumtel[F]', "N", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_F"', '', $telefones['F']['itenumtel'] ); ?></td>
				</tr>
			</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">E-mail</td>
			<td>
				<table>
					<tr>
						<td>Principal</td>
						<td><?php echo campo_texto('iusemailprincipal', "S", "S", "Principal", 60, 60, "", "", '', '', 0, 'id="iusemailprincipal"'); ?></td>
					</tr>
					<tr>
						<td>Opcional</td>
						<td><?php echo campo_texto('iusemailopcional', "N", "S", "Opcional", 60, 60, "", "", '', '', 0, 'id="iusemailopcional"'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Dados para recebimento da bolsa de estudo</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Agência do Banco do Brasil	sugerida</td>
			<td>
				<table>
					<tr>
						<td>Agência</td>
						<td>
							<?php echo campo_texto('iusagenciasugerida', 'S', 'N', 'Agência Sugerida', 5, 4, '####', '', '', '', 0, 'id="iusagenciasugerida"' ); ?>
							<img align="absmiddle" src="../imagens/consultar.gif" border="0" onclick="listarAgencias();" id="btnagencias" style="cursor: pointer;">
						</td>
					</tr>
					<tr>
						<td>Endereço</td>
						<td><?php echo campo_textarea('iusagenciaend', 'S', 'S', '', '70', '4', '250'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">TERMO DE COMPROMISSO</td>
			<td>
				<table width="100%">
					<tr>
						<td>
							<p style="text-align: justify; margin-left: 5px;margin-right: 5px;">Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Coordenador Geral de Comitê Gestor Institucional de Formação Inicial e Continuada de Profissionais da Educação Básica (COMFOR) e COMPROMETO-ME a respeitar as cláusulas descritas na Portaria MEC Nº 1.105, de 08 de Novembro de 2013 e na Resolução CD/FNDE Nº 53, de 11 de Dezembro de 2013.</p><br>
							<p style="text-align: justify; margin-left: 5px;margin-right: 5px;">Declaro ainda, estar ciente de que é vedada a acumulação de mais de uma bolsa de estudo ou pesquisa nos programas regidos pela Lei Nº 11.273, de 06 de Fevereiro de 2006. Desta forma, estou ciente de que, caso eu já seja bolsista, terei o direito de receber apenas uma bolsa.</p><br>
							<p style="text-align: justify; margin-left: 5px;margin-right: 5px;">Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei Nº 11.273, de 06 de Fevereiro de 2006, na Portaria Nº 1.243, de 30 de Dezembro de 2009, na Resolução CD/FNDE Nº 53, de 11 de Dezembro de 2013, que estabelece diretrizes 
							e procedimentos para o pagamento de bolsas de estudo e pesquisa aos coordenadores gerais dos COMFORs.</p><br>
							<p style="text-align: justify; margin-left: 5px;margin-right: 5px;">Estou ciente de que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução CD/FNDE Nº 53, de 11 de Dezembro de 2013, que prevê ainda que incorreções na emissão do cartão-benefício ou em pagamento de bolsa causadas por informações incorretas, prestadas pelo 
							bolsista quando de seu cadastramento, implicarão seu imediato desligamento e impedimento de sua participação, pelo prazo de cinco anos, em qualquer outro programa que conceda bolsas e seja executado pelo FNDE, independentemente de sua responsabilização civil e penal.</p>
						</td>
					</tr>
					<tr>
						<td>
							<input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?php echo(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>>
							<b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br />
							<br>
							<div style="width: 100%;padding: 10px;border: 5px solid gray;margin: 0px;">
							<?php
							
							selecionarBolsaPagamento(array('iusd' => $iusd));
							
							?>
							<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;">
							
							<tbody>
							<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<td valign="top" title="&nbsp;" style="padding-left: 15px;"><input type="radio" name="tpebolsa[]" id="iusnaodesejosubstituirbolsa" value="iusnaodesejosubstituirbolsa" <?php echo(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>></td>
								<td valign="top" title="Perfil">Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa por nenhuma bolsa do SISFOR</td>
							</tr>
							</tbody>
							</table>
							
							</div>
							
							
						</td>
					</tr>
					<tr>
						<td>
							<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
								<tr>
									<td class="SubTituloDireita">Documento de Designação:</td>
									<td>
										<input type="file" name="arquivo" id="documentodesignacao"> 
										<input type="button" value="Anexar" onclick="anexarDocumentoDesignacao();">
									</td>
								</tr>
								<tr>
									<td id="divDocumentoDesignacao" colspan="2">
									<?php $iusd ? listarDocumentoDesignacao($iusd) : ''; ?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" name="salvar" value="Salvar" onclick="salvarDados();"> 
			</td>
		</tr>
	</table>
</form>

<div id="modalAgencia" style="display:none;">
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloCentro" colspan="2">Localizar Agências</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%"><b>Estado:</b></td>
			<td><?php
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$db->monta_combo('estuf_agencias', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF4', '', '', '', 'S', 'estuf_agencias', ''); ?>
			</td>
		</tr>
		<tr>	
			<td class="SubTituloDireita" width="20%"><b>Município:</b></td>
			<td id="td_municipio4">Selecione uma UF</td>
		</tr>
		<tr id="tr_agencia" style="display:none;">
			<td class="SubTituloDireita" width="20%"><b>Agência:</b></td>
			<td id="listaAgencias"></td>
		</tr>
		<tr>	
			<td class="SubTituloCentro" colspan="2"><input type="button" value="Selecionar Agência" onclick="selecionarAgencia();"></td>
		</tr>
	</table>
</div>

<script language="javascript" type="text/javascript">
	<?php if(in_array(PFL_CONSULTAGERAL,$perfil) || in_array(PFL_EQUIPE_MEC,$perfil) || $planejamentofechado){ ?>
		jQuery('button, input, select, textarea').attr('disabled', true);
	<?php } ?>	
	<?php if($iencep){ ?>
		carregarEnderecoPorCEP_coordenadorLocal(<?php echo $iencep; ?>);
	<?php } ?>
</script>

<?php unset($_SESSION['REQUEST']); ?>