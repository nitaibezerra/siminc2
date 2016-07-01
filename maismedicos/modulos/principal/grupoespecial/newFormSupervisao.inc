<?php 

include_once APPRAIZ.'maismedicos/classes/Form.class.inc';
$form = new Form();

if($_REQUEST['requisicao']){
	$form->$_REQUEST['requisicao']($_POST);
	die;
}

include APPRAIZ.'includes/cabecalho.inc';
echo '</br>';

$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo('Relatório de Supervisão Periódica','Grupo Especial de Supervisão');

$rsPlano = $form->recuperaTutorPorPlanoTrabalho($_SESSION['usucpf']);
?>
<link rel="stylesheet" type="text/css" href="/includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
 
<!-- <script type="text/javascript" src="js/jquery.livequery.js"></script>  -->
<script type="text/javascript" src="/includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="/includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="/includes/jquery-validate/lib/jquery.metadata.js"></script>

<script type="text/javascript" src="/includes/jquery-autocomplete/jquery.autocomplete.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/jquery-autocomplete/jquery.autocomplete.css" />
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
$j = jQuery.noConflict();

$j(function(){

	<?php if($rsPlano): ?>
		$j('.tutor').val('<?php echo $rsPlano['cpftutor'].' - '.$rsPlano['nometutor']; ?>');
		$j('.tutor').next().val('<?php echo $rsPlano['cpftutor']; ?>');
	<?php endif; ?>

	$j('#formulario2, #formulario3').hide();
	
	$j('.campoPraticaDsei, .campoPraticaMun, .semContatoPrevio, .modalidadeInLoco, .absenteismoMedico, .modalidadeInLoco, .mudancaEstrutura').parent().parent().hide();
	$j('.subtitulodireita').css('width','50%');
	$j('textarea').css({'width':'100%','height':'80px'});

	$j('.houveMudancaEstrutura').change(function(){
		texto = trim($j(this).find('option:selected').text());
		if(texto == 'Sim'){
			$j('.mudancaEstrutura').parent().parent().show();
			$j('.mudancaEstrutura').val('');
		}else{
			$j('.mudancaEstrutura').parent().parent().hide();
			$j('.mudancaEstrutura').val('');
		}
	});
	
	$j('.medicoCompareceu').change(function(){
		texto = trim($j(this).find('option:selected').text());
		if(texto == 'Sim'){
			$j('.absenteismoMedico').parent().parent().hide();
			$j('.absenteismoMedico').val('');
		}else{
			$j('.absenteismoMedico').parent().parent().show();
			$j('.absenteismoMedico').val('');
		}
	});
	
	$j('.tipoModalidadeSupervisao').change(function(){
		texto = trim($j(this).find('option:selected').text());
		if(texto == 'Visita In Loco'){
			$j('.modalidadeInLoco').parent().parent().show();
			$j('.modalidadeInLoco').val('');
		}else{
			$j('.modalidadeInLoco').parent().parent().hide();
			$j('.modalidadeInLoco').val('');
		}
	});
	
	$j('.teveContatoPrevio').change(function(){
		texto = trim($j(this).find('option:selected').text());
		if(texto == 'Sim'){
			$j('.semContatoPrevio').parent().parent().hide();
			$j('.semContatoPrevio').val('');
		}else{
			$j('.semContatoPrevio').parent().parent().show();
			$j('.semContatoPrevio').val('');
		}
	});
	
	$j('.tipoCampoPratica').change(function(){
		texto = trim($j(this).find('option:selected').text());
		if(texto == 'DSEI'){
			$j('.campoPraticaDsei').parent().parent().show();
			$j('.campoPraticaMun').parent().parent().hide();
			$j('.campoPraticaMun').val('');
		}else{
			$j('.campoPraticaDsei').parent().parent().hide();
			$j('.campoPraticaMun').parent().parent().show();
			$j('.campoPraticaDsei').val('');
		}
	});
	
	/*
	$j('[name=supcpf]').blur(function(){
		nuCpf = replaceAll(replaceAll( $j(this).val(),'.',''),'-','');
		if( !validar_cpf( nuCpf  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			$j(this).val('');
			$j('[name=supcpf]').val('');
			return false;	
		}
		var comp  = new dCPF();
		comp.buscarDados( nuCpf );
		if (comp.dados.no_pessoa_rf != ''){
			$j('[name=supnome]').val(comp.dados.no_pessoa_rf);				
			$j('[name=supnome]').attr("readonly","readonly");
		}
	});
	*/

	thisForm = '#formulario1';
	var validator = $j( thisForm ).validate({
		errorClass: "warning",
		onkeyup: false,
		onfocusout: false,
		submitHandler: function() {
			//alert("Submitted, thanks!");
		}
	});

	$j('.mesreferencia').change(function(){
		idCampo = this.id;
		idPergunta = idCampo.replace('mes_','').replace('ano_','');
		mes = $j('#mes_'+idPergunta).val();
		ano = $j('#ano_'+idPergunta).val();
		valor = mes+'/'+ano;
		$j('#'+idPergunta).val(valor);			
	});
	
	$j(".tutor").autocomplete(document.location.href+'&requisicao=carregaTutores', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var cpf = data[1];
		id = this.id.replace('nome_', '');
		$j('#'+id).val(cpf);
		//preencheDadosTutor(cpf);
	});
	
	$j(".supervisor").autocomplete(document.location.href+'&requisicao=carregaSupervisores', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var cpf = data[1];
		id = this.id.replace('nome_', '');
		$j('#'+id).val(cpf);
		preencheDadosSupervisor(cpf);
	});

	$j(".medico").autocomplete(document.location.href+'&requisicao=carregaMedicos', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var cpf = data[1];
		id = this.id.replace('nome_', '');
		$j('#'+id).val(cpf);
		preencheDadosMedico(cpf);
	});

	$j(".instituicao").autocomplete(document.location.href+'&requisicao=carregaInstituicoes', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var valor = data[1];
		id = this.id.replace('nome_', '');
		$j('#'+id).val(valor);
		//preencheDadosTutor(cpf);
	});
	
	$j('.btnEnviar').click(function(){
		
		camposObrigatoriosClass = ['mesref','tutor','supervisor','medico',
		                   		'tiporelatorio', 'tipoCampoPratica', 'teveContatoPrevio',
		                   		'impedimentoPactuacao',
		                   		'tipoModalidadeSupervisao', 'medicoCompareceu',
		                   		'absenteismoMedico', 
		                   		'houveMudancaEstrutura', 
		                   		'mudancaEstrutura'];
   		
		thisForm = $j(this).parent().parent().parent().parent().parent().parent();

		divCarregando();
		for(x=0;x<camposObrigatoriosClass.length;x++){
			
			label = $j('.'+camposObrigatoriosClass[x]).parent().prev().text();
			
			if( $j('.'+camposObrigatoriosClass[x]).length>1 ){
				
				//alert($j('.'+camposObrigatoriosClass[x]+':checked').length+'_'+camposObrigatoriosClass[x]);
				
				if(camposObrigatoriosClass[x]=='absenteismoMedico'){
					if(trim($j('.medicoCompareceu option:selected').text()) == 'Sim'){
						continue;
					}
				}

				if($j('.'+camposObrigatoriosClass[x]+':checked').length==0){
					alert('O campo '+label+' é obrigatório!');
					formErro = $j('.'+camposObrigatoriosClass[x]).parent().parent().parent().parent().parent();				
					$j(thisForm).hide();
					$j(formErro).show();
					$j('.'+camposObrigatoriosClass[x]).focus();
					divCarregado();
					return false;
				}
				
			}else
			if(!$j('.'+camposObrigatoriosClass[x]).val()){

				if(camposObrigatoriosClass[x]=='tipoCampoPratica'){
					if(trim($j('.tiporelatorio option:selected').text()) == 'Supervisão Periódica'){
						continue;
					}
				}else
				if(camposObrigatoriosClass[x]=='impedimentoPactuacao'){
					if(trim($j('.teveContatoPrevio option:selected').text()) == 'Sim'){
						continue;
					}
				}else
				if(camposObrigatoriosClass[x]=='medicoCompareceu'){
					if(trim($j('.tipoModalidadeSupervisao option:selected').text()) == 'Acompanhamento Longitudinal'){
						continue;
					}
				}else
				if(camposObrigatoriosClass[x]=='mudancaEstrutura'){
					if(trim($j('.houveMudancaEstrutura option:selected').text()) == 'Não'){
						continue;
					}
				}
				
				
				alert('O campo '+label+' é obrigatório!');
				formErro = $j('.'+camposObrigatoriosClass[x]).parent().parent().parent().parent().parent();				
				$j(thisForm).hide();
				$j(formErro).show();
				$j('.'+camposObrigatoriosClass[x]).focus();
				divCarregado();
				return false;
			}
		}

		thisForm = $j(this).parent().parent().parent().parent().parent().parent();
		idThisForm = $j(thisForm).attr('id');
		$j.ajax({
			url 	: document.location.href,
			type	: 'post',
			data	: '&requisicao=salvaFormulario&envia=true&idForm='+idThisForm+'&'+$j('#'+idThisForm).serialize(),
			success	: function(e){
				//alert(e);
				//divCarregado();
				//return false;
				document.location.href = 'maismedicos.php?modulo=principal/grupoespecial/listRelatorioSupervisao&acao=A';
				divCarregado();
			}
		});

		
	});

	$j('.btnSalvar').click(function(){
		
		thisForm = $j(this).parent().parent().parent().parent().parent().parent();
		idThisForm = $j(thisForm).attr('id');
		idProxForm = trim($j('.tiporelatorio option:selected').text()) == 'Supervisão Periódica' ? 3 : 2;
		idAnteForm = trim($j('.tiporelatorio option:selected').text()) == 'Supervisão Periódica' ? 1 : 2;
		classe = $j(this).attr('class');

			if(idThisForm == 'formulario1'){
				idProxForm = '#formulario'+idProxForm;
			}else{
				idProxForm = '#formulario3';
			}

			if(idThisForm == 'formulario3'){		
				idAnteForm = '#formulario'+idAnteForm;
			}else{
				idAnteForm = '#formulario1';
			}
			
			divCarregando();
			$j.ajax({
				url 	: document.location.href,
				type	: 'post',
				data	: '&requisicao=salvaFormulario&idForm='+idThisForm+'&'+$j('#'+idThisForm).serialize(),
				success	: function(e){
					alert(e);
					//divCarregado();
					//return false;
					if(classe.indexOf("btnProximo") >= 0){
						$j( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
						$j(thisForm).hide();				
						$j(idProxForm).show();
					}else if(classe.indexOf("btnAnterior") >= 0){
						$j( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
						$j(thisForm).hide();				
						$j(idAnteForm).show();
					}
					divCarregado();
				}
			});

			$j(".ufcampopratica").change(function(){
				carregaMunicipios(this.value, this.name, '.municipiocampopratica');
			});
		
	});
	
	/*
	
	

	$j('[name=tipopratica]').change(function(){
		if(this.value == '1'){
			$j('#dadosCadastraisMunicipio').show();
			$j('#dadosCadastraisDsei').hide();
		}else{
			$j('#dadosCadastraisDsei').show();
			$j('#dadosCadastraisMunicipio').hide();
		}
	});


	//$j('#dadosCadastraisMunicipio, #dadosCadastraisDsei').hide();
	*/
	
});

function preencheDadosMedico(cpf)
{
	$j.ajax({
		url		 : document.location.href,
		type	 : 'post',
		dataType : "json",
		data	 : 'requisicao=carregaDadosMedico&cpf='+cpf,
		success	 : function(e){
			if(typeof e.mdcnome == 'undefined'){
				alert('O Médico não foi localizado em nossa base de dados!');
			}else if(e.mdcnome == '' || e.mdcnome == 'null' || e.mdcnome == null){
				alert('Nulo');
			}else{
				$j('.cpfMedico').val(e.mdccpf);
				$j('.nomeMedico').val(e.mdcnome);
				$j('.nomeMedico').attr("readonly","readonly");
				$j('.emailMedico').val(e.mdcemail);
				//$j('.cpfMedico').val(e.mdcnomeinstituicao);
			}
		}
	});
}

function preencheDadosSupervisor(cpf)
{
	$j.ajax({
		url		 : document.location.href,
		type	 : 'post',
		dataType : "json",
		data	 : 'requisicao=carregaDadosSupervisor&cpf='+cpf,
		success	 : function(e){
			if(typeof e.nome == 'undefined'){
				alert('O Supervisor não foi localizado em nossa base de dados!');
			}else if(e.nome == '' || e.nome == 'null' || e.nome == null){
				alert('Nulo');
			}else{
				$j('.cpfSupervisor').val(e.cpf);
				$j('.nomeSupervisor').val(e.nome);
				$j('.titulacaoSupervisor').val(e.titulacao);
				
				idInstituicao = $j('.instituicaoSupervisor').attr('id').replace('nome_','');
				$j('.instituicaoSupervisor').val(e.uninome);
				$j('#'+idInstituicao).val(e.uniid);

				if(e.cpf) $j('.cpfSupervisor').attr("readonly","readonly");
				if(e.nome) $j('.nomeSupervisor').attr("readonly","readonly");
				if(e.titulacao) $j('.titulacaoSupervisor').attr("readonly","readonly");
				if(e.uniid) $j('.instituicaoSupervisor').attr("readonly","readonly");
			}
		}
	});
}

function carregaMunicipios(uf, id, classe)
{
	td = $j(classe).parent();
	idMun = $j(classe).attr('id');
	$j(td).html('Carregando...');
	$j.ajax({
		url		: document.location.href,
		type	: 'post',
		data	: 'requisicao=carregaMunicipios&uf='+uf+'&class='+classe+'&id='+idMun,
		success	: function (e) {
			$j(td).html(e);
		}
	});
}

</script>
<style>
.txtajuda{
	color: gray;
	font-style: italic;
	font-size: 11px;
}
</style>
<?php 
$form->montaForm();
?>
<script>
$j(function(){
	idPerSup = $j('.supervisor').attr('id').replace('nome_','');
	supervisor = $j('#'+idPerSup).val();
	if(supervisor){
		preencheDadosSupervisor(supervisor);
	}

	idPerMed = $j('.medico').attr('id').replace('nome_','');
	medico = $j('#'+idPerMed).val();
	if(medico){
		preencheDadosMedico(medico);
	}

	campoPratica = trim($j('.tipoCampoPratica option:selected').text());
	if(campoPratica == 'DSEI'){
		$j('.campoPraticaDsei').parent().parent().show();
		$j('.campoPraticaMun').parent().parent().hide();
	}else if(campoPratica == 'Município'){
		$j('.campoPraticaDsei').parent().parent().hide();
		$j('.campoPraticaMun').parent().parent().show();
	}
	
	campoContatoPrevio = trim($j('.teveContatoPrevio option:selected').text());
	if(campoContatoPrevio == 'Sim'){
		$j('.semContatoPrevio').parent().parent().hide();
	}else if(campoContatoPrevio == 'Não'){
		$j('.semContatoPrevio').parent().parent().show();
	}
	
	houveMudancaEstrutura = trim($j('.houveMudancaEstrutura option:selected').text());
	if(houveMudancaEstrutura == 'Sim'){
		$j('.mudancaEstrutura').parent().parent().show();
	}else if(houveMudancaEstrutura == 'Não'){
		$j('.mudancaEstrutura').parent().parent().hide();
	}
	
	medicoCompareceu = trim($j('.medicoCompareceu option:selected').text());
	if(medicoCompareceu == 'Sim'){
		$j('.absenteismoMedico').parent().parent().hide();
	}else if(medicoCompareceu == 'Não'){
		$j('.absenteismoMedico').parent().parent().show();
	}
	
	tipoModalidadeSupervisao = trim($j('.tipoModalidadeSupervisao option:selected').text());
	if(tipoModalidadeSupervisao == 'Visita In Loco'){
		$j('.modalidadeInLoco').parent().parent().show();
	}else if(tipoModalidadeSupervisao == 'Acompanhamento Longitudinal'){
		$j('.modalidadeInLoco').parent().parent().hide();
	}

});

</script>