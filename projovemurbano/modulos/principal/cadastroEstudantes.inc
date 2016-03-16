<?php


$bloq 		= 'S';
$disabled 	= '';

$bloqStatus 		= 'S';
$disabledStatus 	= '';

if($_REQUEST['caeid']) {
	
	$requisicao = "atualizarEstudantes";
	
	$sql = "SELECT * FROM projovemurbano.cadastroestudante WHERE caeid='".$_REQUEST['caeid']."'";
	$cadastroestudante = $db->pegaLinha($sql);

	if( is_array($cadastroestudante) ){
		extract($cadastroestudante);
	}else{
		echo "<script>
				alert('Estudante não encontrado.');
				window.location = 'projovemurbano.php?modulo=principal/monitoramento&acao=A';
			  </script>";
	}
	$caecpf_sem_masc = $caecpf;
	$caedatanasc = formata_data($caedatanasc);
	$caedataemissaorg = formata_data($caedataemissaorg);
	//$caedataemissaorg = formata_data($caedataemissaorg);
	$caecep = mascaraglobal($caecep,"#####-###");
	$caecpf = mascaraglobal($caecpf,"###.###.###-##");
	
	if( trim($caenispispasep) != '' ){
		$bloq = 'N';
		$disabled = 'disabled="disabled"';
	}
	
} else {
	$requisicao = "inserirEstudantes";
}

$perfis = pegaPerfilGeral();

if(!$db->testa_superuser()) {
	
	if(!in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$bloq 		= 'N';
		$disabled 	= 'disabled="disabled"';
		$bloqStatus 		= 'N';
		$disabledStatus 	= 'disabled="disabled"';
	}elseif (in_array(PFL_CONSULTA, $perfis)){
		$bloq 		= 'N';
		$disabled 	= 'disabled="disabled"';
		$bloqStatus 		= 'N';
		$disabledStatus 	= 'disabled="disabled"';
	}else{
		$bloq 		= 'S';
		$disabled 	= '';
		$bloqStatus 		= 'S';
		$disabledStatus 	= '';
        
	}
}else{
	$bloq 		= 'S';
	$disabled 	= '';
    $bloqStatus 		= 'S';
    $disabledStatus 	= '';
}

if( in_array(PFL_ADMINISTRADOR, $perfis) ){
	$bloq 		= 'S';
	$disabled 	= '';
    $bloqStatus 		= 'S';
    $disabledStatus 	= '';
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
	jQuery('[name="caehistorico"]').click(function(){
		if(jQuery(this).val()=='TRUE'){
			jQuery('[name="caetestepro"][value="FALSE"]').attr('checked',true);
		}else{
			jQuery('[name="caetestepro"][value="TRUE"]').attr('checked',true);
		}
	});
	jQuery('[name="caetestepro"]').click(function(){
		if(jQuery(this).val()=='TRUE'){
			jQuery('[name="caehistorico"][value="FALSE"]').attr('checked',true);
		}else{
			jQuery('[name="caehistorico"][value="TRUE"]').attr('checked',true);
		}
	});
	jQuery('[name="caestatus"]').click(function (){
		if(jQuery(this).val()=='A'){
			jQuery('#caejustificativainativacao').val('');
			jQuery('#tr_caejustificativainativacao').hide();
		}else{
			jQuery('#tr_caejustificativainativacao').show();
		}
	});
	jQuery('[name="nomesocial"]').click(function(){
		if( jQuery(this).val() == 'S' ){
			jQuery('#tr_nomesocial').show();
		}else{
			jQuery('#tr_nomesocial').hide();
			jQuery('#caenomesocial').val('');
		}
	});
	if( jQuery('#caecpf').val() != '' ){
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	   		data: "requisicao=verificaCertificadoTotalBolsa&caenome="+jQuery('#caenome').val()+"&cpf="+jQuery('#caecpf').val()+"&caenomemae="+jQuery('#caenomemae').val()+"&caedatanasc="+jQuery('#caedatanasc').val()+"&caenumrg="+jQuery('#caenumrg').val(),
	   		async: false,
	   		success: function(certificadoBolsa){
	   			var cebo = certificadoBolsa.split(";");
				jQuery('#caeqtddireitobolsa').val(cebo[1]);
	   		}
	 	});
	}
});

function carregarEstudante() {
	
	jQuery('#caecpf').val(mascaraglobal('###.###.###-##',jQuery('#caecpf').val()));
	if(!validar_cpf(replaceAll(replaceAll(jQuery('#caecpf').val(),".",""),"-",""))) {
		alert('CPF inválido');
		jQuery('#caecpf').val('');
		return false;
	}

	var cadastrado = false;
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
   		data: "requisicao=verificaCadastroCPF&cpf="+replaceAll(replaceAll(jQuery('#caecpf').val(),".",""),"-",""),
   		async: false,
   		success: function(msg){
   			if(msg=='true' && jQuery('#caecpf').val() != '<?=$caecpf?>' ){
   	   			alert('Este aluno já possui cadastro.');
   	   			jQuery('#caecpf').val('');
   	   			cadastrado = true;
   	   		}else if( jQuery('#caecpf').val() == '<?=$caecpf?>' ){
   	   			cadastrado = true;
   	   	   	}
   		}
 	});

 	if( cadastrado ){
 	 	return false;
 	}
	
	var comp = new dCPF();
	comp.buscarDados(jQuery('#caecpf').val());
	//alert(comp.dados.no_pessoa_rf);
	
	/*
	//	Como requerido pela Helem, dia 30/08/2012, foram retiradas todas as regras de ajuste de idade antes requeridas pela  mesma.
	//	Agora o Calculo de idade será feito considerando se o aluno teve a idade entre 18 e 29 em algum momento do ano, ou seja,
	//	se ele tem 16 mas vai fazer 18 no ano de matricula, considera 18, e, se ele tem 30 mas em algum momento do ano de matricula ele teve 29, considera 29.
	//		SE Ano Nascimento <= 1994
	//		ENTAO Idade = Data de Nascimento - 31/12/Ano de Matricula
	//		SENAO Idade = Data de Nascimento - 01/01/Ano de Matricula
	//		Ex.:
	//			Aluno Nasceu em 31/12/1994 e a Matricula está sendo realisada hoje, 30/08/2012, logo a idade considerada será:
	//				Idade = Data de Nascimento(31/12/1994) - 31/12/Ano de Matricula(2012) => Idade = 18 Anos ou seja, poderá fazer a matricula.
	//			Aluno Nasceu em 01/01/1994 e a Matricula está sendo realisada hoje, 30/08/2012, logo a idade considerada será:
	//				Idade = Data de Nascimento(01/01/1994) - 31/12/Ano de Matricula(2012) => Idade = 18 Anos ou seja, poderá fazer a matricula.
	//			Aluno Nasceu em 31/12/1995 e a Matricula está sendo realisada hoje, 30/08/2012, logo a idade considerada será:
	//				Idade = Data de Nascimento(31/12/1995) - 31/12/Ano de Matricula(2012) => Idade = 17 Anos ou seja, não poderá fazer a matricula.
	//			Aluno Nasceu em 01/01/1982 e a Matricula está sendo realisada hoje, 30/08/2012, logo a idade considerada será:
	//				Idade = Idade = Data de Nascimento(01/01/1982) - 01/01/Ano de Matricula(2012) => Idade = 30 Anos ou seja, não poderá fazer a matricula.
	//			Aluno Nasceu em 02/01/1982 e a Matricula está sendo realisada hoje, 30/08/2012, logo a idade considerada será:
	//				Idade = Idade = Data de Nascimento(02/01/1982) - 01/01/Ano de Matricula(2012) => Idade = 29 Anos ou seja, poderá fazer a matricula.
	*/
		var idade = <?=date("Y") ?>-parseInt(comp.dados.dt_nascimento_rf.substr(0,4));
		if( idade <= 18 ){
		}else{
			var idadeMes = 1 - <?=date('m') ?>;
			var idadeDia = 1 - <?=date('d') ?>;
			if( idadeMes < 0 || idadeDia < 0 ){
				idade = idade-1;
			}
		}
//	if( <?=date('m') ?> < comp.dados.dt_nascimento_rf.substr(4,2) || ( <?=date('m') ?> == comp.dados.dt_nascimento_rf.substr(4,2) && <?=date('d') ?> <= comp.dados.dt_nascimento_rf.substr(6,2) ) ){
//		idade = parseInt(idade)-1;
//	}
	//alert(idade+' - '+comp.dados.dt_nascimento_rf+' - '+<?php //date('dmY')?>);

		if(idade<18 || idade>29) {
			alert('Estudante com idade diferente de 18 a 29 anos de idade');
			jQuery('#caecpf').val('');
			jQuery('#caenome').val('');
			jQuery('#caenomemae').val('');
			jQuery('#caedatanasc').val('');
			jQuery('#caenumrg').val('');
			jQuery('#endestuf').val('');
			jQuery('#endmuncod').val('');
			jQuery('#caebairro').val('');
			jQuery('#caelogradouro').val('');
			jQuery('#caecomp').val('');
			jQuery('#caenumero').val('');
			jQuery('#caecep').val('');
			jQuery('#caetelfixo').val('');
			return false;
		}//else if( (idade==17 && <?=date('m') ?> > comp.dados.dt_nascimento_rf.substr(4,2) ) || 
//			  (idade==30 && <?=date('m') ?> < comp.dados.dt_nascimento_rf.substr(4,2) )  ){
//		alert('Estudante com idade diferente de 18 a 29 anos de idade');
//		jQuery('#caecpf').val('');
//		jQuery('#caenome').val('');
//		jQuery('#caenomemae').val('');
//		jQuery('#caedatanasc').val('');
//		jQuery('#caenumrg').val('');
//		jQuery('#endestuf').val('');
//		jQuery('#endmuncod').val('');
//		jQuery('#caebairro').val('');
//		jQuery('#caelogradouro').val('');
//		jQuery('#caecomp').val('');
//		jQuery('#caenumero').val('');
//		jQuery('#caecep').val('');
//		jQuery('#caetelfixo').val('');
//		return false;
//	}
	var possuiCertificado = false;
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
   		data: "requisicao=verificaCertificadoTotalBolsa&cpf="+jQuery('#caecpf').val(),
   		async: false,
   		success: function(certificadoBolsa){
   			var cebo = certificadoBolsa.split(";");
   			if(cebo[0]=="1") {
   				possuiCertificado = true;
   			}
			jQuery('#caeqtddireitobolsa').val(cebo[1]);
			jQuery('#div_caeqtddireitobolsa').html(cebo[1]);
   		}
 	});
 		
 	if(possuiCertificado) {
 		alert('Estudante já certificado no Ensino Fundamental');
		jQuery('#caecpf').val('');
		return false;
 	}
	
	//carregando dados pessoais
	jQuery('#caenome').val(comp.dados.no_pessoa_rf);
	jQuery('#caenomemae').val(comp.dados.no_mae_rf);
	jQuery('#caedatanasc').val(comp.dados.dt_nascimento_rf.substr(6,2)+'/'+comp.dados.dt_nascimento_rf.substr(4,2)+'/'+comp.dados.dt_nascimento_rf.substr(0,4));
	jQuery('#caenumrg').val(comp.dados.nu_rg);
	
	jQuery("[name='caesexo'][value='"+comp.dados.sg_sexo_rf+"']").attr('checked',true);
	//carregando endereco
	if(comp.dados.sg_uf) {
		jQuery('#endestuf').val(comp.dados.sg_uf);
		carregarMunicipios(comp.dados.sg_uf);
		jQuery('#endmuncod').val(comp.dados.co_cidade.substr(8));
	}
	jQuery('#caebairro').val(comp.dados.ds_bairro);
	jQuery('#caelogradouro').val(comp.dados.ds_logradouro);
	jQuery('#caecomp').val(comp.dados.ds_logradouro_comp);
	jQuery('#caenumero').val(comp.dados.ds_numero);
	jQuery('#caecep').val(mascaraglobal('#####-###',comp.dados.nu_cep));
	jQuery('#caetelfixo').val(replaceAll(comp.dados.ds_contato_pessoa,"-",""));

}

function gravarCadastroEstudantes() {

	jQuery('#caecpf').val(mascaraglobal('###.###.###-##',jQuery('#caecpf').val()));

	if(jQuery('#caecpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	if(!validar_cpf(replaceAll(replaceAll(jQuery('#caecpf').val(),".",""),"-",""))) {
		alert('CPF Inválido');
		return false;
	}
        
        var cpfEstudante = replaceAll(replaceAll(jQuery('#caecpf').val(),".",""),"-","");
	
	if(jQuery('#caenome').val()=='') {
		alert('Nome em branco');
		return false;
	}

	if( jQuery('[name="nomesocial"][value="S"]').attr('checked') ){
		if( jQuery('#caenomesocial').val() == '' ){
			alert('Nome social em branco.');
			jQuery('#caenomesocial').focus();
			return false;
		}
	}
	
	jQuery('#caedatanasc').val(mascaraglobal('##/##/####',jQuery('#caedatanasc').val()));
	if(jQuery('#caedatanasc').val()=='') {
		alert('Data de Nascimento em branco');
		return false;
	}
	if(!validaData(document.getElementById('caedatanasc'))) {
		alert('Data de Nascimento inválida');
		return false;
	}
	
	var numcaesexo = parseInt(jQuery("[name^='caesexo']:enabled:checked").length);
	
	if(numcaesexo==0) {
		alert('Selecione o sexo');
		return false;
	}
	
	if(jQuery('#caenomemae').val()=='') {
		alert('Nome da mãe em branco');
		return false;
	}

	if(jQuery('#caenaturalidade').val()=='') {
		alert('Naturalidade em branco');
		return false;
	}
	if(jQuery('#craid').val()=='') {
		alert('Cor/Raça em branco');
		return false;
	}
	if(jQuery('#caenumrg').val()=='') {
		alert('Carteira de identidade em branco');
		return false;
	}
	if(jQuery('#caeorgaoexpedidorrg').val()=='') {
		alert('Orgão expedidor em branco');
		return false;
	}
	jQuery('#caedataemissaorg').val(mascaraglobal('##/##/####',jQuery('#caedataemissaorg').val()));
	if(jQuery('#caedataemissaorg').val()=='') {
		alert('Data emissão em branco');
		return false;
	}
	if(!validaData(document.getElementById('caedataemissaorg'))) {
		alert('Data emissão inválida');
		return false;
	}
	
	var numcaedeficiencia = parseInt(jQuery("[name^='caedeficiencia']:enabled:checked").length);
	if(numcaedeficiencia==0) {
		alert('Selecione se possui com defiência');
		return false;
	} else {
		if(jQuery("[name^='caedeficiencia']:enabled:checked").val()=='TRUE') {
			if(jQuery('#tdeid').val()=='') {
				alert('Selecione Deficiência');
				return false;
			}
		}
	}
	var caealtashabilidades = parseInt(jQuery("[name^='caealtashabilidades']:enabled:checked").length);
	if(caealtashabilidades==0) {
		alert('Selecione se possui altas habilidades.');
		return false;
	}
	
	
	if(jQuery('#escid').val()=='') {
		alert('Estado Civil em branco');
		return false;
	}
	
	var numcaefilhos = parseInt(jQuery("[name^='caefilhos']:enabled:checked").length);
	if(numcaefilhos==0) {
		alert('Selecione se possui filhos');
		return false;
	}
	
	var numcaebenoutroprog = parseInt(jQuery("[name^='caebenoutroprog']:enabled:checked").length);
	if(numcaebenoutroprog==0) {
		alert('Selecione se possui beneficiários de outros programas');
		return false;
	} else {
		if(jQuery("[name^='caebenoutroprog']:enabled:checked").val()=='TRUE') {
			if(jQuery('#pbeid').val()=='') {
				alert('Selecione Qual transtorno desintegrativo da infância');
				return false;
			}
		}
	}
	
	var numcaeocupacao = parseInt(jQuery("[name^='caeocupacao']:enabled:checked").length);
	if(numcaeocupacao==0) {
		alert('Selecione se possui tem alguma ocupação');
		return false;
	} else {
		if(jQuery("[name^='caeocupacao']:enabled:checked").val()=='TRUE') {
			if(jQuery('#ocuid').val()=='') {
				alert('Selecione Qual ocupação');
				return false;
			}
		}
	}
	
	if(jQuery('#ssoid').val()=='') {
		alert('Selecione o Segmento Social');
		return false;
	}
	
	var numcaebrasilalfa = parseInt(jQuery("[name^='caebrasilalfa']:enabled:checked").length);
	if(numcaebrasilalfa==0) {
		alert('Selecione se é egresso do Programa Brasil Alfabetizado.');
		return false;
	}
	
	var egressoprogalfabetizacao = parseInt(jQuery("[name^='egressoprogalfabetizacao']:enabled:checked").length);
	if(egressoprogalfabetizacao==0) {
		alert('Selecione se é egresso de outro programa de alfabetização.');
		return false;
	}
	
	var numegressoprogalfabetizacao = parseInt(jQuery("[name^='egressoprogalfabetizacao']:enabled:checked").length);
	if(numegressoprogalfabetizacao==0) {
		alert('Selecione se é egresso de outro programa de alfabetização.');
		return false;
	}
	
	jQuery('#caecep').val(mascaraglobal('#####-###',jQuery('#caecep').val()));
	if(jQuery('#caecep').val()=='') {
		alert('CEP em branco');
		return false;
	}
	
	if(jQuery('#caelogradouro').val()=='') {
		alert('Logradouro em branco');
		return false;
	}
	if(jQuery('#caenumero').val()=='') {
		alert('Número em branco');
		return false;
	}
	
	if(jQuery('#caebairro').val()=='') {
		alert('Bairro em branco');
		return false;
	}
	
	if(jQuery('#endestuf').val()=='') {
		alert('UF em branco');
		return false;
	}
	
	if(jQuery('#endmuncod').val()=='') {
		alert('Munícipio em branco');
		return false;
	}
	
	jQuery('#caetelfixo').val(mascaraglobal('##########',jQuery('#caetelfixo').val()));
	if(jQuery('#caetelfixo').val()=='') {
		alert('Telefone fixo em branco');
		return false;
	}
	jQuery('#caetelcel').val(mascaraglobal('##########',jQuery('#caetelcel').val()));
	
	var numcaehistorico = parseInt(jQuery("[name^='caehistorico']:enabled:checked").length);
	if(numcaehistorico==0) {
		alert('Selecione se apresentou histórico escola');
		return false;
	}
	
	var numcaetestepro = parseInt(jQuery("[name^='caetestepro']:enabled:checked").length);
	if(numcaetestepro==0) {
		alert('Selecione se realizou teste de proficiência');
		return false;
	}
	
	if(document.getElementById('polid')) {
		if(jQuery('#polid').val()=='') {
			alert('Polo em branco');
			return false;
		}
	}
	
	if(jQuery('#nucid').val()=='') {
		alert('Núcleo / Escola em branco');
		return false;
	}
	
	if(jQuery('#turid').val()=='') {
		alert('Turma em branco');
		return false;
	}

	if(jQuery('#turno').val()=='') {
		alert('Turno em branco');
		return false;
	}	
	
   if((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "A")){
	
		var possuiCertificado = false;
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	   		data: "requisicao=verificaCertificadoTotalBolsa&caenome="+jQuery('#caenome').val()+"&cpf="+jQuery('#caecpf').val()+"&caenomemae="+jQuery('#caenomemae').val()+"&caedatanasc="+jQuery('#caedatanasc').val()+"&caenumrg="+jQuery('#caenumrg').val(),
	   		async: false,
	   		success: function(certificadoBolsa){
	   			var cebo = certificadoBolsa.split(";");
				jQuery('#caeqtddireitobolsa').val(cebo[1]);
				jQuery('#div_caeqtddireitobolsa').html(cebo[1]);
	   			if(cebo[0]=="1") {
	   				possuiCertificado = true;
	   			}
	   		}
	 		});
	                
	 	if(possuiCertificado) {
	 		alert('Estudante já certificado no Ensino Fundamental');
			return false;
	 	}
 	
 	}
	if((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "A")){
	   	var nucleoLotado = false;
       	jQuery.ajax({
			type: "POST",
            url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
            data: "testaDados=testaQtdAlunoNucleo&nucid="+jQuery('#nucid').val()+"&cpfestudante="+cpfEstudante,
            async: false,
            success: function(retorno){
            	if( retorno != '' ) {
                	nucleoLotado = true;
                }
            }
        });

        if( nucleoLotado ) {
        	alert('Núcleo lotado. Escolha outra.');
            return false;
		}
	}
       
	if((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "A")){
		var turmaLotada = false;
		jQuery.ajax({
			type: "POST",
			url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
			data: "testaDados=testaQtdAlunoTurma&turid="+jQuery('#turid').val()+"&cpfestudante="+cpfEstudante,
			async: false,
			success: function(retorno){
				if( retorno != '' ) {
					turmaLotada = true;
				}
			}
		});
	
		if( turmaLotada ) {
			alert('Turma lotada. Escolha outra.');
			return false;
		}
	}
// 	document.getElementById('endmuncod').removeAttribute('disabled');

	jQuery('[name=polid],[name=nucid],[name=turno],[name=caenucleo],[name=turid]').attr('disabled',false);
	jQuery('#form').submit();
}

function pessoaDeficiencia(opcao) {
	if(opcao=='S') {
		document.getElementById('pessoaDeficiencia_1').style.display='';
	} else if(opcao=='N') {
		document.getElementById('pessoaDeficiencia_1').style.display='none';
	}
}

function outrosProgramas(opcao) {
	if(opcao=='S') {
		document.getElementById('outrosProgramas').style.display='';
	} else if(opcao=='N') {
		document.getElementById('outrosProgramas').style.display='none';
	}
}

function temFilhos(opcao) {
	if(opcao=='S') {
		document.getElementById('temFilhos').style.display='';
	} else if(opcao=='N') {
		document.getElementById('temFilhos').style.display='none';
	}
}

function temOcupacao(opcao) {
	if(opcao=='S') {
		document.getElementById('temOcupacao').style.display='';
	} else if(opcao=='N') {
		document.getElementById('temOcupacao').style.display='none';
	}
}

function carregarMunicipios2(estuf) {
	document.getElementById('td_muncod').innerHTML = 'Carregando...';
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
   		data: "requisicao=carregarMunicipios2&estuf="+estuf,
   		async: false,
   		success: function(msg){document.getElementById('td_muncod').innerHTML = msg;}
 		});
}

function carregarMunicipios(estuf, bloqueio) {
    
    var bloq = '';
    if(typeof bloqueio == 'undefined'){  
       bloq = "S";
    } else {  
       bloq = bloqueio;  
    }  

    document.getElementById('td_endmuncod').innerHTML = 'Carregando...';
    jQuery.ajax({
            type: "POST",
            url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
            data: "requisicao=carregarMunicipios&estuf="+estuf+"&bloq="+bloq,
            async: false,
            success: function(msg){
                document.getElementById('td_endmuncod').innerHTML = msg;
            }
            });
}

function buscarTurmas2(nucid, bloqueio) {
    var bloq = bloqueio;
    
    if (bloq == ""){
        bloq = "N";
    }
	if( nucid != '' ){
		document.getElementById('td_turma2').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	   		data: "requisicao=buscarTurmas2&nucid="+nucid+"&bloq="+bloq,
	   		async: false,
	   		success: function(msg){	
	   			document.getElementById('td_turma2').innerHTML = msg;
	   		}
	 		});
	}
}

function buscarNucleos2(polid, bloq) {
	if( polid != '' ){
		document.getElementById('td_nucleo2').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	   		data: "requisicao=buscarNucleos2&polid="+polid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('td_nucleo2').innerHTML = msg;
	   			document.getElementById('td_turma2').innerHTML = '<select id="endestuf" style="width: auto" class="CampoEstilo obrigatorio" name="endestuf"><option value="">Selecione</option></select>';
	   		}
	 		});
	}
}

function buscarTurmas(nucid, bloq) {
	if( nucid != '' ){
		document.getElementById('td_turma').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	   		data: "requisicao=buscarTurmas&nucid="+nucid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
	 		});
	}
}

function buscarNucleos(polid, bloq) {
	if( polid != '' ){
		document.getElementById('td_nucleo').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	   		data: "requisicao=buscarNucleos&polid="+polid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_nucleo').innerHTML = msg;}
	 		});
	}
}

function excluirEstudantes(caeid) {
	var conf = confirm('Deseja realmente excluir o estudante?');
	
	if(conf) {
		window.location='projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&requisicao=excluirEstudante&caeid='+caeid;
	}

}

function exibir_ocultar_historico( id ){
	div = document.getElementById( id );
	if ( div.style.display == 'none' ) {
		div.style.display = 'block';
	} else {
		div.style.display = 'none'
	}
}

function verificaFaltas(caeid, caecpf, caestatus){
	  var turid = jQuery('#turid').val();
	  var nucid = jQuery('#nucid').val();

	  jQuery.ajax({
	    type: "POST",
	    url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
	    data: "requisicao=verificarFaltasAluno&caeid=" + caeid
	            + "&caestatus=" + caestatus
	            + "&turid=" + turid
	            + "&nucid=" + nucid,
	    async: false,
	    success: function(status) {
	      if (status == 'inativo') {
	        alert('Aluno não poderá ser ativado por exceder 360 horas de faltas.');	
	        jQuery('input:radio[name=caestatus]').attr('checked', false);
	        //jQuery('#caestatus').val(caestatus);
	      }
	    }
	  });
	  
	  jQuery.ajax({
		    type: "POST",
		    url: "projovemurbano.php?modulo=principal/monitoramento&acao=A",
		    data: "requisicao=verificaProjovemcampo&caecpf=" + caecpf
		            + "&caestatus=" + caestatus,
		    async: false,
		    success: function(msg) {
		      if (msg == 1) {
		        alert('Aluno não poderá ser ativado por ter matrícula ativa no Projovem Campo.');	
// 		        jQuery('input:radio[name=caestatus]').attr('checked', false);
				jQuery('#caestatus_i').attr('checked', 'checked');
		        //jQuery('#caestatus').val(caestatus);
		      }
		    }
	  });
	}
</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
<input type="hidden" name="caeid" value="<?=$caeid ?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">CPF:</td>
		<td><? echo campo_texto('caecpf', "S", ($caecpf != '' ? 'N' : $bloq), "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="caecpf"', '', '', 'carregarEstudante();' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><? echo campo_texto('caenome', "S", ( (!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis)) ? "N" : $bloq ), "Nome", 67, 150, "", "", '', '', 0, 'id="caenome"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Nascimento:</td>
		<td><? echo campo_texto('caedatanasc', "S", ( (!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis)) ? "N" : $bloq ), "Data de Nascimento", 12, 10, "##/##/####", "", '', '', 0, 'id="caedatanasc"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sexo:</td>
		<td>
			<input type="radio" name="caesexo" value="M" <?=(($caesexo=="M")?"checked":"") ?> <?=$disabled ?> > Masculino 
			<input type="radio" name="caesexo" value="F" <?=(($caesexo=="F")?"checked":"") ?> <?=$disabled ?> > Feminino
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome da mãe:</td>
		<td>
		<? echo campo_texto('caenomemae', "S", $bloq, "Nome da mãe", 67, 150, "", "", '', '', 0, 'id="caenomemae"' );	?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do pai:</td>
		<td><? echo campo_texto('caenomepai', "N", $bloq, "Nome do pai", 67, 150, "", "", '', '', 0, 'id="caenomepai"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nacionalidade:</td>
		<td>
		<?
		$naturalidade = array(0=>array("codigo"=>"B","descricao"=>"Brasileira"),1=>array("codigo"=>"E","descricao"=>"Estrangeira"));
		$db->monta_combo('caenaturalidade', $naturalidade, $bloq, 'Selecione', '', '', '', '', 'S', 'caenaturalidade'); 
		
                if (in_array(PFL_CONSULTA, $perfis)){?>
                    <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaNacionalidade')" style="cursor:pointer;" title="Nacionalidade" src="../imagens/AtencaoP.gif">
                <?php } ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cor / Raça:</td>
		<td>
		<?
		$sql = "SELECT craid as codigo, cradesc as descricao FROM projovemurbano.corraca WHERE crastatus='A' ORDER BY cradesc";
		$db->monta_combo('craid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'craid'); 
		if (in_array(PFL_CONSULTA, $perfis)){?>
                    <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaRaca')" style="cursor:pointer;" title="Cor / Raça" src="../imagens/AtencaoP.gif">
		<?php } ?>
                </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Carteira de identidade:</td>
		<td><? echo campo_texto('caenumrg', "S", $bloq, "Carteira de identidade", 40, 42, "", "", '', '', 0, 'id="caenumrg"' ); ?> 
			Orgão expedidor: <? echo campo_texto('caeorgaoexpedidorrg', "S", $bloq, "Orgão expedidor", 12, 10, "", "", '', '', 0, 'id="caeorgaoexpedidorrg"' ); ?> 
			Data de emissão: <? echo campo_texto('caedataemissaorg', "S", $bloq, "Data de emissão", 12, 10, "##/##/####", "", '', '', 0, 'id="caedataemissaorg"' ); ?></td>
	</tr>
	<tr>
		<?$caenispispasep = $caenispispasep == 0 ? '' : $caenispispasep;?>
		<td class="SubTituloDireita">NIS / PIS / PASEP / NIT:</td>
		<td><? echo campo_texto('caenispispasep', "N", "N", "NIS / PIS / PASEP", 13, 11, "", "", '', '', 0, 'id="caenispispasep"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pessoa com deficiência:</td>
		<td>
			<input type="radio" name="caedeficiencia" value="TRUE" <?=(($caedeficiencia=="t")?"checked":"") ?> <?=$disabled ?> onclick="pessoaDeficiencia('S');"> Sim 
			<input type="radio" name="caedeficiencia" <?=(($caedeficiencia=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE" onclick="pessoaDeficiencia('N');"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?>
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaDeficiencia')" style="cursor:pointer;" title="Transtorno global do desenvolvimento" src="../imagens/AtencaoP.gif">
                        <?php }?>
                </td>
	</tr>
	<tr id="pessoaDeficiencia_1" <?=(($caedeficiencia=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Deficiência:</b></td>
		<td>
			<? 
			$sql = "SELECT tdeid as codigo, tdedesc as descricao FROM projovemurbano.tipodeficiencia WHERE tdestatus='A' ORDER BY tdedesc";
			$db->monta_combo('tdeid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'tdeid');
			?> 
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Transtorno global do desenvolvimento:</td>
		<td>
			<? 
			$sql = "SELECT tdiid as codigo, tdidesc as descricao FROM projovemurbano.transdesininfancia WHERE tdistatus='A' AND tdiid not in (5) ORDER BY tdidesc";
			$db->monta_combo('tdiid', $sql, $bloq, 'Selecione', '', '', '', '', 'N', 'tdiid');
			if (in_array(PFL_CONSULTA, $perfis)){?>
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaTranstorno')" style="cursor:pointer;" title="Transtorno global do desenvolvimento" src="../imagens/AtencaoP.gif">
                        <?php }?>    
                </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Altas Habilidades/Superdotação:</td>
		<td>
			<input type="radio" name="caealtashabilidades" <?=(($caealtashabilidades=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE" /> Sim 
			<input type="radio" name="caealtashabilidades" <?=(($caealtashabilidades=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE" /> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif" />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de recurso de acessibilidade que utiliza:</td>
		<td>
			<?php 
			$sql = "SELECT
						racid as codigo,
						racdesc as descricao
					FROM
						projovemurbano.recursosacessibilidade
					WHERE
						racstatus = 'A'";
			
			if( $_REQUEST['caeid'] ){
				$marcados = $db->carregarColuna('SELECT racid FROM projovemurbano.cadastroestudante_recursosacessibilidade WHERE
				caeid = '.$_REQUEST['caeid']);
			}
			
			$db->monta_checkbox('racid[]', $sql, $marcados, '<br>')
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estado Civil:</td>
		<td>
			<? 
			$sql = "SELECT escid as codigo, escdesc as descricao FROM projovemurbano.estadocivil WHERE escstatus='A' ORDER BY escdesc";
			$db->monta_combo('escid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'escid');
			if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaEstadoCivil')" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif">
                        <?php }?>    
                </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tem filhos  ?</td>
		<td>
			<input type="radio" name="caefilhos" <?=(($caefilhos=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE" onclick="temFilhos('S');"> Sim 
			<input type="radio" name="caefilhos" <?=(($caefilhos=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE" onclick="temFilhos('N');"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr id="temFilhos" <?=(($caefilhos=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita">&nbsp;</td>
		<td colspan="2">
			<?
			$sql = "SELECT idfdesc, '<input type=\"text\" class=\"normal\" name=\"estqtdfilhos['||idfid||']\" ".(($caeid)?"value=\"'||COALESCE((SELECT estqtdfilhos::character(7) 
                FROM projovemurbano.estudantefilhos e WHERE i.idfid=e.idfid AND e.caeid='".$caeid."'),'')||'\"":"")." size=\"1\" onkeyup=\"this.value=mascaraglobal(\'#\',this.value);\">' FROM projovemurbano.idadefilhos i WHERE idfstatus='A' ORDER BY idfid";
//			ver($sql);
			$cabecalho = array("Idade","Quantidade de filhos");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Beneficiários de outros programas:</td>
		<td>
			<input type="radio" name="caebenoutroprog" <?=(($caebenoutroprog=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE" onclick="outrosProgramas('S');"> Sim 
			<input type="radio" name="caebenoutroprog" <?=(($caebenoutroprog=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE" onclick="outrosProgramas('N');"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaProgramasBeneficiarios')" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif">
                        <?php }?>    
                </td>
	</tr>
	<tr id="outrosProgramas" <?=(($caebenoutroprog=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Qual ?</b></td>
		<td>
			<? 
			$sql = "SELECT pbeid as codigo, pbedesc as descricao FROM projovemurbano.programabeneficiario WHERE pbestatus='A' ORDER BY pbedesc";
			$db->monta_combo('pbeid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'pbeid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tem alguma ocupação?</td>
		<td>
			<input type="radio" name="caeocupacao" <?=(($caeocupacao=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE" onclick="temOcupacao('S');"> Sim 
			<input type="radio" name="caeocupacao" <?=(($caeocupacao=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE" onclick="temOcupacao('N');"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaOcupacao')" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif">
                        <?php }?>    
                </td>
	</tr>
	<tr id="temOcupacao" <?=(($caeocupacao=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Qual ?</b></td>
		<td>
			<? 
			$sql = "SELECT ocuid as codigo, ocudesc as descricao FROM projovemurbano.ocupacao WHERE ocustatus='A' ORDER BY ocudesc";
			$db->monta_combo('ocuid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'ocuid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Segmento Social:</td>
		<td>
			<? 
			$sql = "SELECT ssoid as codigo, ssodesc as descricao FROM projovemurbano.segmentosocial WHERE ssostatus='A' ORDER BY ssodesc";
			$db->monta_combo('ssoid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'ssoid');
			?> 
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaSegmentoSocial')" style="cursor:pointer;" title="Segmento Social" src="../imagens/AtencaoP.gif">
                        <?php }?>
                </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É egresso do Programa Brasil Alfabetizado?</td>
		<td>
			<input type="radio" name="caebrasilalfa" <?=(($caebrasilalfa=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE"> Sim 
			<input type="radio" name="caebrasilalfa"  <?=(($caebrasilalfa=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É egresso de outro programa de alfabetização?</td>
		<td>
			<input type="radio" name="egressoprogalfabetizacao" value="TRUE" <?=(($egressoprogalfabetizacao=="t")?"checked":"") ?> <?=$disabled ?> > Sim 
			<input type="radio" name="egressoprogalfabetizacao" value="FALSE" <?=(($egressoprogalfabetizacao=="f")?"checked":"") ?> <?=$disabled ?>> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Endereço</td>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="20%">CEP</td>
			<td><? echo campo_texto('caecep', "S", $bloq, "CEP", 11, 9, "#####-###", "", '', '', 0, 'id="caecep"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Logradouro</td>
			<td><? echo campo_texto('caelogradouro', "S", $bloq, "Logradouro", 67, 300, "", "", '', '', 0, 'id="caelogradouro"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número</td>
			<td><? echo campo_texto('caenumero', "S", $bloq, "Número", 12, 10, "", "", '', '', 0, 'id="caenumero"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento</td>
			<td><? echo campo_texto('caecomp', "N", $bloq, "Complemento", 67, 150, "", "", '', '', 0, 'id="caecomp"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro</td>
			<td><? echo campo_texto('caebairro', "S", $bloq, "Bairro", 67, 150, "", "", '', '', 0, 'id="caebairro"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
				<? 
				$endestuf = $estuf;
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$db->monta_combo('endestuf', $sql, $bloq, 'Selecione', 'carregarMunicipios', '', '', '', 'S', 'endestuf');
				?>
				<? if($endestuf) : ?>
				<script>
				jQuery(document).ready(function() {
					carregarMunicipios('<?=$endestuf ?>','<?=$bloq?>');
					jQuery('#endmuncod').val('<?=$muncod ?>');
				});
				</script>
				<? endif; ?>
                                <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                                    <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaUF')" style="cursor:pointer;" title="UF" src="../imagens/AtencaoP.gif">
                                <?php }?>
                        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Munícipio</td>
			<td>
                            <label id="td_endmuncod"><? $db->monta_combo('endmuncod', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'endmuncod'); ?></label>
                            <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                                <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carrega_Municipio')" style="cursor:pointer;" title="Munícipio" src="../imagens/AtencaoP.gif">
                            <?php }?>
                        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail</td>
			<td><? echo campo_texto('caeemail', "N", $bloq, "E-mail", 67, 100, "", "", '', '', 0, 'id="caeemail"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone fixo</td>
			<td><? echo campo_texto('caetelfixo', "S", $bloq, "Telefone fixo", 12, 12, "##-####-####", "", '', '', 0, 'id="caetelfixo"' ); ?> Formato: DD-####-####</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone celular</td>
			<td><? echo campo_texto('caetelcel', "N", $bloq, "Telefone celular", 12, 12, "##-####-####", "", '', '', 0, 'id="caetelcel"' ); ?> Formato: DD-####-####</td>
		</tr>
		</table>
		
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Apresentou histórico escolar ?</td>
		<td>
			<input type="radio" name="caehistorico" <?=(($caehistorico=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE"> Sim 
			<input type="radio" name="caehistorico" <?=(($caehistorico=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Realizou teste de proficiência ?</td>
		<td>
			<input type="radio" name="caetestepro" <?=(($caetestepro=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE"> Sim 
			<input type="radio" name="caetestepro" <?=(($caetestepro=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE"> Não
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Auxílio financeiro a receber:</td>
		
			<?php 
			if($_REQUEST['caeid']){
				$sql = "SELECT 
      						CASE 
					    		WHEN (caeqtddireitobolsa - caeqtdbolsaprojovem) > 0
					    		THEN (caeqtddireitobolsa - caeqtdbolsaprojovem)
					    		ELSE '0'
					    		END  as auxilios
					   FROM projovemurbano.cadastroestudante WHERE caeid='{$_REQUEST['caeid']}'"; 
			   $auxilios = $db->pegaUm($sql);
// 			   ver($sql,d);
		   }
			?>
			
<!-- 		<td>campo_texto('caeqtddireitobolsa', "S", "N", "Quantidade de bolsas", 12, 10, "##########", "", '', '', 0, 'id="caeqtddireitobolsa"' );</td> -->
			<td>
				<input type="hidden" name="caeqtddireitobolsa" id="caeqtddireitobolsa" value="<?php  if($auxilios==''){ echo '0';}else{$auxilios;} ?>"/>
				<div id="div_caeqtddireitobolsa"><?php  if($auxilios==''){ echo '0';}else{echo $auxilios;} ?></div>
			</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Vincular a escola</b></td>
		<td>
		<input type="hidden" name="turid_bkp" value="<?=$turid ?>" />
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		
		<?
		$sql = "SELECT * FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A'";
		$polomunicipio = $db->pegaLinha($sql);
		
		if(!$db->testa_superuser()) {
			$perfis = pegaPerfilGeral();
			
			if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
				$sql = "SELECT
							entid
						FROM
							projovemurbano.usuarioresponsabilidade
						WHERE
							usucpf='".$_SESSION['usucpf']."'
							AND entid  IS NOT NULL
							AND rpustatus = 'A'";
				$_SESSION['projovemurbano']['entid'] = $db->pegaUm($sql);
			}
		}
		
		if($polomunicipio['pmupossuipolo']=="t") :
		?>
		<tr>
			<td class="SubTituloDireita" width="20%"><b>Polo</b></td>
			<td><? 
			// Adaptação para o perfil Diretor do Pólo
			if(!$db->testa_superuser()) {
				
				if(in_array(PFL_DIRETOR_POLO, $perfis)) {
					$inner_polo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.polid=pol.polid AND rpustatus='A'";
					$inner_polo_filtro = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.polid=cae.polid AND rpustatus='A'";
				}elseif(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
					$inner_nucleo= "INNER JOIN projovemurbano.associamucipiopolo 	 asm ON asm.polid = pol.polid 
									INNER JOIN projovemurbano.nucleo 				 nuc ON nuc.munid = asm.munid
									INNER JOIN projovemurbano.nucleoescola 			 nes ON nes.nucid = nuc.nucid
									INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf = '".$_SESSION['usucpf']."' and ur.entid=nes.entid AND rpustatus='A'";
					}
			}
			$sql = "SELECT DISTINCT pol.polid as codigo, 'POLO '||pol.polid as descricao 
					FROM projovemurbano.polo pol 
					LEFT JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid 
					{$inner_polo} 
					{$inner_nucleo}
					WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A' AND polstatus='A' 
					ORDER BY pol.polid";
//			ver($sql);
			$db->monta_combo('polid', $sql, $bloq, 'Selecione', 'buscarNucleos', '', '', '', 'S', 'polid');
			
			if($polid) : ?>
			<script>
			jQuery(document).ready(function() {
				buscarNucleos('<?=$polid ?>','<?=$bloq ?>');
				jQuery('#nucid').val('<?=$nucid ?>');
				buscarTurmas('<?=$nucid ?>','<?=$bloq ?>');
				jQuery('#turid').val('<?=$turid ?>');
			});
			</script>
			<? endif; ?>

			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Núcleo / Escolas</b></td>
			<td id="td_nucleo">
                            <? $db->monta_combo('nucid', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'nucid'); ?>
                            <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                                <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carrega_Municipio&endestuf=MG')" style="cursor:pointer;" title="Munícipio" src="../imagens/AtencaoP.gif">
                            <?php } ?>
                        </td>
                        
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turma</b></td>
			<td><label id="td_turma"><? $db->monta_combo('turid', array(), $bloq, 'Selecione', '', '', '', '150', 'S', 'turid'); ?><label></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turno</b></td>
			<td>
				<label id="td_turno">
					<?php
						 $arrTurno = array( 
							 			array("codigo" => "M", "descricao" => "Matutino"), 
							 			array("codigo" => "V", "descricao" => "Vespertino"),
							 			array("codigo" => "N", "descricao" => "Noturno")
						 			); 
						$db->monta_combo('turno', $arrTurno, $bloq, 'Selecione o Turno', '', '', '', '150', 'S', 'turno'); 
					?>
				</label>
			</td>
		</tr>			
		<? endif; ?>
		<? if($polomunicipio['pmupossuipolo']=="f") : ?>
		<tr>
			<td class="SubTituloDireita"><b>Núcleo / Escolas</b></td>
			<td><? 
			$nucleos = pegarNucleos($polomunicipio['pmupossuipolo']);
			if($nucleos[0]['codigo']!=''){ 
			$db->monta_combo('nucid', $nucleos, $bloq, 'Selecione', 'buscarTurmas', '', '', '', 'S', 'nucid');
			?>
			<script>
			jQuery(document).ready(function() {
				buscarTurmas('<?=$nucid ?>');
				jQuery('#turid').val('<?=$turid ?>','<?=$bloq ?>');
			});
			</script>
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaNucleoEscola&possuipolo=<?php echo $polomunicipio['pmupossuipolo'] ?>')" style="cursor:pointer;" title="Núcleo / Escolas" src="../imagens/AtencaoP.gif">
                        <?php } ?>
			<? }elseif(contaEstudantesNucleos($polomunicipio['pmupossuipolo'])){ 
			$db->monta_combo('caenucleo', array(), $bloq, 'Numero máximo de estudantes atingido.', '', '', '', '', 'N', 'caeturma');
			}else{ 
			$db->monta_combo('caenucleo', array(), $bloq, 'Sem nucleos', '', '', '', '', 'N', 'caeturma');
			} ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turma</b></td>
			<td><label id="td_turma"><? $db->monta_combo('turid', array(), $bloq, 'Selecione um nucleo', '', '', '', '150', 'S', 'turid'); ?></label>
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&reqTittle=carregaNucleoTurma&nucid=<?=$nucid ?>')" style="cursor:pointer;" title="Munícipio" src="../imagens/AtencaoP.gif">
                        <?php } ?>
                        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turno</b></td>
			<td>
				<label id="td_turno">
					<?php
						 $arrTurno = array( 
							 			array("codigo" => "M", "descricao" => "Matutino"), 
							 			array("codigo" => "V", "descricao" => "Vespertino"),
							 			array("codigo" => "N", "descricao" => "Noturno")
							 		);
						$db->monta_combo('turno', $arrTurno, $bloq, 'Selecione o Turno', '', '', '', '150', 'S', 'turno'); 
					?>
				</label>
			</td>
		</tr>	
		<? endif; ?>

		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Status:</b></td>
		<td>
			<input type="radio" name="caestatus" value="A" <?=(($caestatus=="A"||!$caestatus)?"checked":"") ?> <?php echo $disabledStatus ?>  onchange="return verificaFaltas('<?php echo $cadastroestudante['caeid']; ?>','<?php echo $cadastroestudante['caecpf']; ?>', '<?php echo $caestatus; ?>');"> Ativo 
			<input type="radio" name="caestatus" value="I" <?=(($caestatus=="I")?"checked":"") ?> <?php echo $disabledStatus ?>> Inativo - outros motivos
			<input type="radio" name="caestatus" value="D" <?=(($caestatus=="D")?"checked":"") ?> <?php echo 'disabled' ?>> Inativo - desistência
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
			<?php if ($caeid){?>
			<a href="javascript: exibir_ocultar_historico('historico_geral');">
				<img src="/imagens/mais.gif" style="border: 0" /> histórico
			</a>
			<div id="historico_geral" style="width: 500px; display: none">
				<p>
					<?php
						$cabecalho = array("Responsável pelo Cadastramento", "Data", "Status", "Ação");
						$sql = sprintf("
							Select 	sg.usunome,
									to_char(hicdataacao, 'DD/MM/YYYY') as  hicdataacao,
									case when hictipo = 'A' then 'Ativo' else 'Inativo' end as hictipo, 
									case when hicacao = 'A' then 'Alteração' else 
									   case when hicacao = 'I' then 'Inserção'else 
									      case when hicacao = 'D' then 'Excluido' end
									   end
									end as hicacao
							From projovemurbano.historicocadastro hc
							join seguranca.usuario sg on sg.usucpf = hc.usucpf
							join projovemurbano.cadastroestudante cd on cd.caeid = hc.caeid
							Where hc.caeid = $caeid or cd.caecpf = '$caecpf_sem_masc'
							order by hicdataacao", 
						$caeid);
						$db->monta_lista_simples($sql, $cabecalho, 25, 0);
					?>
				</p>
			</div>
			<?php }?>
		</td>
	</tr>		
	
	<tr id="tr_caejustificativainativacao" <?=(($caestatus!="I")?"style=\"display:none\"":"") ?>>
		<td class="SubTituloDireita"><b>Justificativa para inativação:</b></td>
		<td>
			<?=campo_textarea( 'caejustificativainativacao', 'S', $bloq, '', 71, 3, 5000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<?php if($msgMeta!=''){?>
				<label style="color:red"><?=$msgMeta ?></label><br>
			<?php }
            /*
             * Bloqueio: dia 02/10 às 12 horas (Adicionado por Rondomar)             * 
             */
            $dataAtual      = mktime( date('H'), date('i'), date('s'), date('m'), date('d'), date('Y') );
            $dataBloqueio   = mktime( 12, 0, 0, 10, 2, 2012 );
            $bloqueioHorario = (bool) ( $dataAtual > $dataBloqueio );
            
            /*A pedido do Analista Julio Leitão 12/08/2013. Acrescentada a regra que permite a quem tem o perfil Administrador, 
			cadastrar novo estudante mesmo após o prazo.(((in_array(PFL_ADMINISTRADOR, $perfis)&&($_REQUEST['caeid']=='' )))*/
            
           
	       if((!$bloqueioHorario || !empty($_REQUEST['caeid']))|| (in_array(PFL_ADMINISTRADOR, $perfis)&&($_REQUEST['caeid']==''))||in_array(PFL_SUPER_USUARIO, $perfis)) {  
	            
	             echo '<input type="button" name="salvar" value="Salvar" '. $disabled .' onclick="gravarCadastroEstudantes();">';
	       }
           
            ?>
				

			<?/* if($caeid) : ?>
				<input type="button" name="novo" value="Novo" <?=$disabled ?> onclick="window.location='projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes';">
			<? endif;*/ ?>
		</td>
	</tr>
</table>
</form>
<form id="form_busca" name="form_busca" method="POST">
	<input type="hidden" name="pesquisar" value="1" />
	<input type="hidden" name="estuf" value="<?=$_SESSION['projovemurbano']['estuf'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<b>Filtros de pesquisa</b>
			</td>
		</tr>
		<!--<tr>
			<td class="SubTituloDireita">Munícipio</td>
			<td id="td_muncod">
				<? 
					$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_SESSION['projovemurbano']['estuf'] ."' ORDER BY mundescricao";
					$db->monta_combo('muncod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'muncod');
				?>
			</td>
		</tr>
		--><?
		$sql = "SELECT * FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A'";
		$polomunicipio = $db->pegaLinha($sql);
		
		if($polomunicipio['pmupossuipolo']=="t") :
		?>
		<tr>
			<td class="SubTituloDireita" width="35%"><b>Polo</b></td>
			<td><? 
			// Adaptação para o perfil Diretor do Pólo
			if(!$db->testa_superuser()) {
				$perfis = pegaPerfilGeral();
				
				if(in_array(PFL_DIRETOR_POLO, $perfis)) {
					$inner_polo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.polid=pol.polid AND rpustatus='A'";
					$inner_polo_filtro = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.polid=cae.polid AND rpustatus='A'";
				}elseif(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
					$inner_nucleo = "inner join projovemurbano.associamucipiopolo asm on asm.polid = pol.polid 
								     inner join projovemurbano.nucleo nuc on nuc.munid = asm.munid
								     inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.nucid=nuc.nucid AND rpustatus='A'";
				}
			}
			$polid = $_POST['polid'];
			$nucid = $_POST['nucid'];
			$caeturma = $_POST['caeturma'];  
			$sql = "SELECT pol.polid as codigo, 'POLO '||pol.polid as descricao 
					FROM projovemurbano.polo pol 
					LEFT JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid 
					{$inner_polo} 
					{$inner_nucleo}
					WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A' AND polstatus='A' 
					ORDER BY pol.polid";
			$db->monta_combo('polid', $sql, 'S', 'Selecione', 'buscarNucleos2', '', '', '', 'N', 'polid');
			if($polid) : ?>
			<script>
			jQuery(document).ready(function() {
				buscarNucleos2('<?=$polid ?>');
				jQuery('#nucid').val('<?=$nucid ?>');
				buscarTurmas2('<?=$nucid ?>');
				jQuery('#caeturma').val('<?=$caeturma ?>');
			});
			</script>
			<? endif; ?>
	
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Núcleo / Escolas</b></td>
			<td id="td_nucleo2"><? $db->monta_combo('nucid', array(), 'S', 'Selecione um polo', '', '', '', '', 'N', 'nucid'); ?>
                        </td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turma</b></td>
			<td id="td_turma2"><? $db->monta_combo('endestuf', array(), 'S', 'Selecione um nucleo', '', '', '', '', 'N', 'endestuf'); ?>
                        </td>
		</tr>
		<? endif; ?>
		<? if($polomunicipio['pmupossuipolo']=="f") : ?>
		<tr>
			<td class="SubTituloDireita" width="35%">Núcleo / Escolas</td>
			<td>
				<? 
				$nucleos = pegarNucleos($polomunicipio['pmupossuipolo']);
				if($nucleos[0]['codigo']!=''){ 
				$db->monta_combo('nucid', $nucleos, 'S', 'Selecione', 'buscarTurmas2', '', '', '', 'N', 'nucid');
				?>
				<script>
				jQuery(document).ready(function() {
					buscarTurmas2('<?=$nucid ?>','S');
					jQuery('#caeturma').val('<?=$caeturma ?>');
				});
				</script>
				<? }elseif(contaEstudantesNucleos($polomunicipio['pmupossuipolo'])){ 
				$db->monta_combo('caenucleo', array(), 'S', 'Numero máximo de estudantes atingido.', '', '', '', '', 'N', 'caeturma');
				}else{ 
				$db->monta_combo('caenucleo', array(), 'S', 'Sem nucleos', '', '', '', '', 'N', 'caeturma');
				} ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Turma</td>
			<td id="td_turma2"><? $db->monta_combo('caeturma', array(), 'S', 'Selecione um nucleo', '', '', '', '', 'N', 'caeturma'); ?>
                        </td>
		</tr>
	
		<? endif; ?>
		<tr>
			<td class="SubTituloDireita">Matricula:</td>
			<td><? echo campo_texto('matricula', "N", "S", "Matricula", 12, 8, "", "", '', '', 0, 'id="matricula"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<td><? echo campo_texto('caecpf', "N", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="caecpf"', '', '', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('caenome', "N", "S", "Nome", 50, 255, "", "", '', '', 0, 'id="caenome"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" name="pesquisar" value="Pesquisar" onclick="document.getElementById('form_busca').submit();">
			</td>
		</tr>
	</table>
</form>

<?

//if($nucleos[0]) {
//	foreach($nucleos as $nucleo) {
//		$filtro_nucleo[] = $nucleo['codigo'];
//	}
//}

$where = Array();
if( $_POST['pesquisar'] == '1' ){
	if( $_POST['muncod'] ){
		$where[] = "(ende.muncod = '".$_POST['muncod']."')";
	}
	if( $_POST['polid'] ){
		$where[] = "cae.polid = ".$_POST['polid'];
	}
	if( $_POST['nucid'] ){
		$where[] = "cae.nucid = ".$_POST['nucid'];
	}
	if( $_POST['caeturma'] ){
		$where[] = "cae.caeturma = ".$_POST['caeturma'];
	}
	if( $_POST['matricula'] ){
		$where[] = "caeano||lpad(cae.caeid::varchar,6,'0') ilike '%".$_POST['matricula']."%'";
	}
	if( $_POST['caenome'] ){
		$where[] = "cae.caenome ilike '%'||UPPER('".$_POST['caenome']."')||'%'";
	}
	if( $_POST['caecpf'] ){
		$where[] = "cae.caecpf = '".str_replace(Array('.','-'),'',$_POST['caecpf'])."'";
	}
}
if(!$db->testa_superuser()) {
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_DIRETOR_POLO, $perfis)) {
		$inner_polo = "inner join projovemurbano.usuarioresponsabilidade ur1 on ur1.usucpf='".$_SESSION['usucpf']."' and ur1.polid=cae.polid AND ur1.rpustatus='A'";
	}
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$inner_nucleo = "left join projovemurbano.usuarioresponsabilidade ur2 on ur2.usucpf='".$_SESSION['usucpf']."' and ur2.entid = nes.entid AND ur2.rpustatus='A'";
		$where_nucleo = " AND ( ur2.entid IS NOT NULL OR cae.turid IS NULL )";
	}
	
} 
//<img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirEstudantes(\''||cae.caeid||'\')\" > 
$sql = "SELECT DISTINCT
			'<img src=../imagens/alterar.gif title=\"Editar\" border=0 style=cursor:pointer; onclick=\"window.location=\'projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&caeid='||cae.caeid||'\';\"> 
			
			<img src=../imagens/report.gif border=0 width=\"13px\"  title=\"Imprimir Comprovante de Inscrição\" style=cursor:pointer; onclick=\"window.open(\'projovemurbano.php?modulo=principal/popComprovante&acao=A&caeid='||cae.caeid||'\',\'Comprovante\',\'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">
			<img src=../imagens/lista_verde.gif border=0 width=\"13px\"  title=\"Imprimir Ficha de Matricula\" style=cursor:pointer; onclick=\"window.open(\'projovemurbano.php?modulo=principal/popFichaMatricula&acao=A&caeid='||cae.caeid||'\',\'Comprovante\',\'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">' as acao,
			'<center><img src=../imagens/'||
			CASE WHEN caestatus = 'A'
				THEN '0_ativo.png title=\"Aluno Ativo\"'
				ELSE '0_inativo.png title=\"Aluno Inativo\"'
			END ||' border=0 width=\"13px\" style=cursor:pointer;/></center>' as status,
			caeano||lpad(cae.caeid::varchar,6,'0') as ninscricao,   
			caecpf, 
			caenome, 
			caeemail, 
			'NÚCLEO '||nuc.nucid||
			CASE WHEN nuetipo = 'S' THEN ', SEDE: ' ELSE ', ANEXO:' END||ent.entnome as entnome,
			turdesc 
		FROM 
			projovemurbano.cadastroestudante cae
		LEFT  JOIN projovemurbano.turma          tur ON tur.turid  = cae.turid
		LEFT  JOIN projovemurbano.nucleo         nuc ON nuc.nucid  = tur.nucid --AND nuc.nucstatus = 'A'  
		LEFT  JOIN projovemurbano.nucleoescola   nes ON nes.nucid  = nuc.nucid AND nes.entid = tur.entid --AND nuestatus = 'A'
		LEFT  JOIN entidade.entidade 	         ent ON ent.entid  = tur.entid
		INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid  = cae.pjuid
		LEFT  JOIN entidade.entidadeendereco     etd ON etd.entid  = ent.entid
		LEFT  JOIN entidade.endereco 	        ende ON ende.endid = etd.endid
		{$inner_polo_filtro}
		$inner_polo
		$inner_nucleo
		WHERE
			pju.pjuid = ".$_SESSION['projovemurbano']['pjuid']."
			$where_nucleo 
			".(($filtro_nucleo)?" AND nuc.nucid IN('".implode("','",$filtro_nucleo)."')":"")
			.(($where)?" AND ".implode(" AND ",$where) :"")."
		ORDER BY caenome";
//ver($sql);
$cabecalho = array("","Status","Matrícula","CPF","Nome","E-mail","Núcleo","Turma");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>

<? registarUltimoAcesso(); ?>

<?php if($_REQUEST['caeid']): ?>
	<script>
		jQuery(function(){
			jQuery('[name=polid],[name=nucid],[name=turno],[name=caenucleo],[name=turid]').attr('disabled',true);
		});
	</script>
<?php endif; ?>