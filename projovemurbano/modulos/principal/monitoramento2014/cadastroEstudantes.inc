<?php

// include_once APPRAIZ."includes/classes/EnderecoCEP.class.inc";

// $endereco = New enderecoCEP( '49052240' );

$bloq = $bloqStatus = 'S';
$disabled = $disabledStatus = '';
if ($_REQUEST['caeid']) {
  $requisicao = "atualizarEstudantes";
  $sql = "SELECT * FROM projovemurbano.cadastroestudante WHERE caeid='{$_REQUEST['caeid']}'";

  $cadastroestudante = $db->pegaLinha($sql);
// ver($cadastroestudante,d);  
  if (is_array($cadastroestudante)) {
    extract($cadastroestudante);
  } else {
    echo "<script>alert('Estudante não encontrado.');window.location = 'projovemurbano.php?modulo=principal/monitoramento2014&acao=A';</script>";
  }
  $caecpf_sem_masc = $caecpf;
  $caedatanasc = formata_data($caedatanasc);
  $caedataemissaorg = formata_data($caedataemissaorg);
  //$caedataemissaorg = formata_data($caedataemissaorg);
  $caecep = mascaraglobal($caecep, "#####-###");
  $caecpf = mascaraglobal($caecpf, "###.###.###-##");

  if (trim($caenispispasep) != ''){
    $bloq = 'N';
    $disabled = 'disabled="disabled"';
  }
} else {
  $requisicao = "inserirEstudantes";
}
$perfis = pegaPerfilGeral();

if (!$db->testa_superuser()) {
  if(!in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } elseif (in_array(PFL_CONSULTA, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } elseif (in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } elseif (in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } else {
    $bloq     = $bloqStatus     = 'S';
    $disabled = $disabledStatus = '';
  }
} else {
  $bloq     = $bloqStatus     = 'S';
  $disabled = $disabledStatus = '';
}

if (in_array(PFL_ADMINISTRADOR, $perfis)) {
  $bloq     = $bloqStatus     = 'S';
  $disabled = $disabledStatus = '';
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/datapicker.pt_Br.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" />
<script>jQuery.noConflict();</script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text'/css" href="../../includes/superTitle.css"/>
<style type="text/css">
label{cursor:pointer}
</style>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
  jQuery('[name="caehistorico"]').click(function() {
    if(jQuery(this).val() == 'TRUE') {
      jQuery('[name="caetestepro"][value="FALSE"]').attr('checked', true);
    } else {
      jQuery('[name="caetestepro"][value="TRUE"]').attr('checked', true);
    }
  });
  jQuery('[name="caetestepro"]').click(function(){
    if (jQuery(this).val() == 'TRUE') {
      jQuery('[name="caehistorico"][value="FALSE"]').attr('checked', true);
    } else {
      jQuery('[name="caehistorico"][value="TRUE"]').attr('checked', true);
    }
  });
  jQuery('[name="caestatus"]').click(function() {
    if (jQuery(this).val() == 'A'){
      jQuery('#caejustificativainativacao').val('');
      jQuery('#tr_caejustificativainativacao').hide();
    } else {
      jQuery('#tr_caejustificativainativacao').show();
    }
  });
  jQuery('[name="nomesocial"]').click(function() {
    if (jQuery(this).val() == 'S') {
      jQuery('#tr_nomesocial').show();
    } else {
      jQuery('#tr_nomesocial').hide();
      jQuery('#caenomesocial').val('');
    }
  });
  if (jQuery('#caecpf').val() != '') {
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "requisicao=verificaCertificadoTotalBolsa&caenome=" + jQuery('#caenome').val()
              + "&cpf=" + jQuery('#caecpf').val()
              + "&caenomemae=" + jQuery('#caenomemae').val()
              + "&caedatanasc=" + jQuery('#caedatanasc').val()
              + "&caenumrg=" + jQuery('#caenumrg').val(),
      async: false,
      success: function(certificadoBolsa) {
        var cebo = certificadoBolsa.split(";");
        jQuery('#caeqtddireitobolsa').val(cebo[1]);
        jQuery('#div_caeqtddireitobolsa').html(cebo[1]);
      }
    });
  }

  // -- Campos que aceitam apenas texto
  jQuery('.alphaonly').bind('keyup blur', function() {
    jQuery(this).val(
            jQuery(this).val().replace(/[^a-záéíóúãõâêîôûç ]/gi, ''));
  });

  // -- Datepicker
  jQuery("#caedatanasc").datepicker({
    maxDate:"+0D", changeMonth: true, changeYear: true, showOtherMonths: true,
    selectOtherMonths: true, createButton: false, onSelect: function(selectedDate) {
      jQuery('#caeidade').val(calculaIdade(selectedDate));
    }
  });
  jQuery('#caedataemissaorg').datepicker({
    maxDate:'+0D', changeMonth: true, changeYear: true, showOtherMonths: true,
    selectOtherMonths: true, createButton: false});
  
  // -- Nome do pai e da mãe mudam para IGNORADA/IGNORADO quando vázios
  jQuery('#caenomemae').blur(function(){
    if ('' == jQuery(this).val().trim()) { jQuery(this).val('IGNORADA'); }
  });
  jQuery('#caenomepai').blur(function(){
    if ('' == jQuery(this).val().trim()) { jQuery(this).val('IGNORADO'); }
  });

  // -- Exibe o campo de digitar o motivo de inativação
  jQuery('#minid').change(function(){
    if (-1 == jQuery(this).val()) { jQuery('#descricaomotivoinativacao_div').show();
    } else { jQuery('#descricaomotivoinativacao_div').hide(); }
  });
});

/**
 * Processa o retorno da consulta do CEP
 * @param {string} retornoCEP Retorno da consulta do CEP com o endereço do indivíduo
 * @returns {undefined}
 * @see projevemurbano.js
 */
function processaRetornoCEP(retornoCEP) {
  var dadosEndereco = retornoCEP.split('||');
  jQuery('#caelogradouro').val(dadosEndereco[0]);
  jQuery('#caebairro').val(dadosEndereco[1]);
  jQuery('#endestuf').val(dadosEndereco[3]).change();
  jQuery('#endmuncod').val(dadosEndereco[4]);
  jQuery('#caelogradouro_label').html(dadosEndereco[0]);
  jQuery('#caebairro_label').html(dadosEndereco[1]);
  jQuery('#endestuf_label').html(dadosEndereco[3]).change();
  jQuery('#td_endmuncod').html(dadosEndereco[2]);
  jQuery('#caenumero').val('');
  jQuery('#caecomp').val('');
}

function carregarEstudante() {
  jQuery('#caecpf').val(mascaraglobal('###.###.###-##', jQuery('#caecpf').val()));
  if (!validar_cpf(replaceAll(replaceAll(jQuery('#caecpf').val(), ".", ""), "-", ""))) {
    alert('CPF inválido');
    jQuery('#caecpf').val('');
    return false;
  }

  var cadastrado = false;
  jQuery.ajax({
    type: "POST",
    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
    data: "requisicao=verificaCadastroCPF&cpf=" + replaceAll(replaceAll(jQuery('#caecpf').val(), ".", ""), "-", ""),
    async: false,
    success: function(msg) {
      if (msg == '1' && jQuery('#caecpf').val() != '<?=$caecpf?>') {
        jQuery('#caecpf').val('');
        cadastrado = true;
        alert('Este aluno já possui cadastro.');
      }else if(msg == '2'){
    	  jQuery('#caecpf').val('');
          cadastrado = true;
          alert('Este aluno já possui cadastro ativo no Projovem Campo.');
      } else if(jQuery('#caecpf').val() == '<?=$caecpf?>'){
        cadastrado = true;
      }
    }
  });


  if (cadastrado) {
    return false;
  }
  var comp = new dCPF();
  comp.buscarDados(jQuery('#caecpf').val());
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
  if((jQuery('#caecpf').val() != '120.319.814-04') && (jQuery('#caecpf').val() != '604.846.023-65') && (jQuery('#caecpf').val() != '100.921.206-01') ){
	  var idade = <?=date("Y"); ?> - parseInt(comp.dados.dt_nascimento_rf.substr(0, 4));
	  if (idade <= 18) {
	  } else {
	    var idadeMes = 1 - <?=date('m'); ?>;
	    var idadeDia = 1 - <?=date('d'); ?>;
	    if (idadeMes < 0 || idadeDia < 0) {
	      idade = idade-1;
	    }
	  }
  
//	if( <?=date('m') ?> < comp.dados.dt_nascimento_rf.substr(4,2) || ( <?=date('m') ?> == comp.dados.dt_nascimento_rf.substr(4,2) && <?=date('d') ?> <= comp.dados.dt_nascimento_rf.substr(6,2) ) ){
//		idade = parseInt(idade)-1;
//	}
	//alert(idade+' - '+comp.dados.dt_nascimento_rf+' - '+<?php //date('dmY')?>);

  if (idade < 18 || idade > 29) {
    alert('Estudante com idade diferente de 18 a 29 anos');
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
  //}//else if( (idade==17 && <?=date('m') ?> > comp.dados.dt_nascimento_rf.substr(4,2) ) || 
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
	}
  }
  var possuiCertificado = false;
  jQuery.ajax({
    type: "POST",
    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
    data: "requisicao=verificaCertificadoTotalBolsa&cpf=" + jQuery('#caecpf').val(),
    async: false,
    success: function(certificadoBolsa){
      var cebo = certificadoBolsa.split(";");
      if (cebo[0] == "1") {
        possuiCertificado = true;
      }
      jQuery('#caeqtddireitobolsa').val(cebo[1]);
      jQuery('#div_caeqtddireitobolsa').html(cebo[1]);
    }
  });

  if (possuiCertificado) {
    alert('Estudante já certificado no Ensino Fundamental');
    jQuery('#caecpf').val('');
    return false;
  }
  //carregando dados pessoais
  jQuery('#caenome').val(comp.dados.no_pessoa_rf);
  jQuery('#caenomemae').val(comp.dados.no_mae_rf);
  jQuery('#caedatanasc').val(comp.dados.dt_nascimento_rf.substr(6, 2)
          + '/' + comp.dados.dt_nascimento_rf.substr(4, 2)
          + '/' + comp.dados.dt_nascimento_rf.substr(0, 4));
  jQuery('#caeidade').val(calculaIdade(jQuery('#caedatanasc').val()));
  jQuery('#caenumrg').val(comp.dados.nu_rg);

  jQuery("[name='caesexo'][value='" + comp.dados.sg_sexo_rf + "']").attr('checked', true);

  //carregando endereco
  if (comp.dados.sg_uf) {
    jQuery('#endestuf').val(comp.dados.sg_uf);
    carregarMunicipios(comp.dados.sg_uf);
    jQuery('#endmuncod').val(comp.dados.co_cidade.substr(8));
  }
  jQuery('#caebairro').val(comp.dados.ds_bairro);
  jQuery('#caelogradouro').val(comp.dados.ds_logradouro);
  jQuery('#caecomp').val(comp.dados.ds_logradouro_comp);
  jQuery('#caenumero').val(comp.dados.ds_numero);
  jQuery('#caecep').val(mascaraglobal('#####-###', comp.dados.nu_cep));
  jQuery('#caetelfixo').val(mascaraglobal('##-####-####', replaceAll(comp.dados.ds_contato_pessoa, "-", "")));

}

function gravarCadastroEstudantes() {
  var formOk = true;
  var cpfEstudante = jQuery('#caecpf').val().replace(/[^0-9]/g, '');

  jQuery('#form [required]').each(function() {
    var nomeCampo = jQuery(this).parent().prev().text().replace(/[^a-záéíóúãõâêîôûç \/]/gi,'');
	if( nomeCampo == '' ){
		/**/
		nomeCampo = jQuery(this).parent().parent().prev().children().first().text().replace(/[^a-záéíóúãõâêîôûç \/]/gi,'');
    }
    var idCampo = jQuery(this).attr('id');
    if ('' == jQuery(this).val()) {
      alert("O campo '"+ nomeCampo +"' não pode ser deixado em branco.");
      rolaTela(idCampo);
      jQuery(this).focus();
      formOk = false;
      return false;
    } else {
      var estaInvalido = false;
      switch (idCampo) {
        case 'caecpf': estaInvalido = (!validar_cpf(replaceAll(replaceAll(jQuery('#caecpf').val(), ".", ""), "-", ""))); break;
        case 'caedatanasc': estaInvalido = (!validaData(document.getElementById('caedatanasc'))); break;
        case 'caedataemissaorg': estaInvalido = (!validaData(document.getElementById('caedataemissaorg'))); break;
      }
      if (estaInvalido) {
        alert("O valor inserido no campo '"+ nomeCampo +"' é inválido.");
        rolaTela(idCampo);
        jQuery(this).focus();
        formOk = false;
        return false;
      }
    }
  });
  if(jQuery('#ufemissaorg').val()==''){
	alert('Escolha uma opção para a UF de emissão do RG');
	return false;
  }
	var caetipomoradia = parseInt(jQuery("[name = 'caetipomoradia']:checked").length); 
	if (caetipomoradia == 0) {
    alert('Você deve selecionar uma opção para o campo "Qual Região de Moradia".');
    rolaTela('caetipomoradia');
    return false;
  }
  
  if (!formOk) { return false; }

  var numcaesexo = parseInt(jQuery("[name^='caesexo']:enabled:checked").length);
  if (numcaesexo == 0) {
    alert('Você deve selecionar uma opção para o campo "Sexo".');
    rolaTela('caesexo_m');
    return false;
  }

  var numcaedeficiencia = parseInt(jQuery("[name^='caedeficiencia']:enabled:checked").length);
  if (numcaedeficiencia == 0) {
    alert("Você deve selecionar uma opção para o campo 'Pessoa com deficiência'.");
    rolaTela('caedeficiencia_s');
    return false;
  } else {
    if(jQuery("[name^='caedeficiencia']:enabled:checked").val() == 'TRUE') {
      if(jQuery('#tdeid').val() == '') {
        alert("O campo 'Deficiência' não pode ser deixado em branco.");
        rolaTela('caedeficiencia_s');
        return false;
      }
    }
  }
  
  var motivo = document.getElementById('descricaomotivoinativacao').value;
  if(motivo){
  
	 if( motivo.length >= 199 ){
	  	alert("Campo \"Motivo\" não pode ser maior do que 200 caracteres.");
	    return false;
  	}
  }
  var caealtashabilidades = parseInt(jQuery("[name^='caealtashabilidades']:enabled:checked").length);
  if (caealtashabilidades == 0) {
    alert("Você deve selecionar uma opção para o campo 'Altas Habilidades/Superdotação'.");
    rolaTela('caealtashabilidades_s');
    return false;
  }

  var numcaefilhos = parseInt(jQuery("[name^='caefilhos']:enabled:checked").length);
  if (numcaefilhos == 0) {
    alert("Você deve selecionar uma opção para o campo 'Tem filhos'.");
    rolaTela('caefilhos_s');
    return false;
  } else if ('TRUE' == jQuery("[name^='caefilhos']:enabled:checked").val()
                && '' == jQuery('#qtdfilhos').val()) {
    alert("O campo 'Quantos filhos' não pode ser deixado em branco.");
    rolaTela('qtdfilhos');
    return false;
  }

  var numcaebenoutroprog = parseInt(jQuery("[name^='caebenoutroprog']:enabled:checked").length);
  if (numcaebenoutroprog == 0) {
    alert("Você deve selecionar uma opção para o campo 'Beneficiários de outros programas'.");
    rolaTela('caebenoutroprog_s');
    return false;
  } else {
    if (jQuery("[name^='caebenoutroprog']:enabled:checked").val() == 'TRUE') {
      if (jQuery('#pbeid').val() == '') {
        alert("O campo 'Qual programa' não pode ser deixado em branco.");
        rolaTela('pbeid');
        return false;
      }
    }
  }

  var numcaecumpremedidasocioeduc = parseInt(jQuery("[name^='caecumpremedidasocioeduc']:enabled:checked").length);
  if (0 == numcaecumpremedidasocioeduc) {
    alert("Você deve selecionar uma opção para o campo 'Jovem em cumprimento de medidas socioeducacionais'.");
    rolaTela('caecumpremedidasocioeduc"_s');
    return false;
  }

 var numcaeparticipouprojovemurbano = parseInt(jQuery("[name^='caeparticipouprojovemurbano']:enabled:checked").length);
  if (0 == numcaeparticipouprojovemurbano) {
    alert("Você deve selecionar uma opção para o campo 'Já participou do Projovem Urbano'.");
    rolaTela('caeparticipouprojovemurbano_s');
    return false;
  }

  var numcaeocupacao = parseInt(jQuery("[name^='caeocupacao']:enabled:checked").length);
  if (numcaeocupacao == 0) {
    alert("Você deve selecionar uma opção para o campo 'Tem alguma ocupação'.");
    rolaTela('caeocupacao_s');
    return false;
  } else {
    if (jQuery("[name^='caeocupacao']:enabled:checked").val() == 'TRUE') {
      if (jQuery('#ocuid').val() == '') {
        alert("O campo 'Qual ocupação' não pode ser deixado em branco.");
        rolaTela('ocuid');
        return false;
      }
    }
  }

  var numcaebrasilalfa = parseInt(jQuery("[name^='caebrasilalfa']:enabled:checked").length);
  if (numcaebrasilalfa == 0) {
    alert("Você deve selecionar uma opção para o campo 'É egresso do Programa Brasil Alfabetizado'.");
    rolaTela('caebrasilalfa_s');
    return false;
  }

  var egressoprogalfabetizacao = parseInt(jQuery("[name^='egressoprogalfabetizacao']:enabled:checked").length);
  if (egressoprogalfabetizacao == 0) {
    alert("Você deve selecionar uma opção para o campo 'É egresso de outro programa de alfabetização'.");
    rolaTela('egressoprogalfabetizacao_s');
    return false;
  }

  var numegressoprogalfabetizacao = parseInt(jQuery("[name^='egressoprogalfabetizacao']:enabled:checked").length);
  if (numegressoprogalfabetizacao == 0) {
    alert("Você deve selecionar uma opção para o campo 'Já participou do Projovem Urbano'.");
    rolaTela('caeprojovem_s');
    return false;
  }

  var numcaehistorico = parseInt(jQuery("[name^='caehistorico']:enabled:checked").length);
  if (numcaehistorico == 0) {
    alert("Você deve selecionar uma opção para o campo 'Apresentou histórico escolar'.");
    rolaTela('caehistorico_s');
    return false;
  }

  var numcaetestepro = parseInt(jQuery("[name^='caetestepro']:enabled:checked").length);
  if (numcaetestepro == 0) {
    alert("Você deve selecionar uma opção para o campo 'Realizou teste de proficiência'.");
    rolaTela('caehistorico_s');
    return false;
  }

  // -- Validação do certificado de ensino fundamental
  if ((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "A")) {
    var possuiCertificado = false;
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "requisicao=verificaCertificadoTotalBolsa&caenome=" + jQuery('#caenome').val()
              + "&cpf=" + jQuery('#caecpf').val()
              + "&caenomemae=" + jQuery('#caenomemae').val()
              + "&caedatanasc=" + jQuery('#caedatanasc').val()
              + "&caenumrg=" + jQuery('#caenumrg').val(),
      async: false,
      success: function(certificadoBolsa) {
        var cebo = certificadoBolsa.split(";");
        jQuery('#caeqtddireitobolsa').val(cebo[1]);
        jQuery('#div_caeqtddireitobolsa').html(cebo[1]);
        if (cebo[0] == "1") {
          possuiCertificado = true;
        }
      }
    });

    if (possuiCertificado) {
      alert('Estudante já certificado no Ensino Fundamental');
      return false;
    }
  }

  // -- Lotação do núcleo
  if ((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "A")) {
// 	  alert(jQuery('#nucid').val());
    var nucleoLotado = false;
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "testaDados=testaQtdAlunoNucleo&nucid="+jQuery('#nucid').val()+"&cpfestudante="+cpfEstudante,
      async: false,
      success: function(retorno) {
        if (retorno.replace(/ /g, '') != '') {
          nucleoLotado = true;
        }
      }
    });

    if (nucleoLotado) {
      alert('Não há vagas disponíveis no Núcleo escolhido. Escolha outro.');
      return false;
    }
  }

  // -- Validação de vagas na turma
  if( jQuery('#turid').val() == '' || jQuery('#turid').val() == 'undefined' ){
      alert('Escolha uma turma.');
      return false;
  }

//   if ((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "A")) {
//     var turmaLotada = false;
//     jQuery.ajax({
//       type: "POST",
//       url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
//       data: "testaDados=testaQtdAlunoTurma&turid="+jQuery('#turid').val()+"&cpfestudante="+cpfEstudante,
//       async: false,
//       success: function(retorno) {
//         if (retorno.replace(/ /g, '') != '') {
//           turmaLotada = true;
//         }
//       }
//     });

//     if (turmaLotada) {
//       alert('Não há vagas disponíveis na Turma escolhida. Escolha outra.');
//       return false;
//     }
//   }

  // -- Validação das opções de escolha
  if ((jQuery('[name="caestatus"]').is(':checked')) && (jQuery('[name="caestatus"]:checked').val() == "I")) {
    if ('' == jQuery('#minid').val()) {
      alert("O campo 'Justificativa para inativação' não pode ser deixado em branco.");
      rolaTela('minid');
      return false;
    } else if (-1 == jQuery('#minid').val()
            && '' == jQuery('#descricaomotivoinativacao').val()) {
      alert("O campo 'Digite o motivo' não pode ser deixado em branco.");
      rolaTela('descricaomotivoinativacao');
      return false;
    }
  }

  document.getElementById('endmuncod').removeAttribute('disabled');

  jQuery('[name=polid],[name=nucid],[name=turno],[name=caenucleo],[name=turid]').attr('disabled', false);
  jQuery('#form').submit();
}

function pessoaDeficiencia(opcao) {
  if (opcao == 'S') {
    document.getElementById('pessoaDeficiencia_1').style.display = '';
  } else if (opcao == 'N') {
    document.getElementById('pessoaDeficiencia_1').style.display = 'none';
  }
}

function outrosProgramas(opcao) {
  if (opcao == 'S') {
    document.getElementById('outrosProgramas').style.display = '';
  } else if(opcao == 'N') {
    document.getElementById('outrosProgramas').style.display = 'none';
  }
}

function temFilhos(opcao) {
  if (opcao == 'S') {
    document.getElementById('temFilhos').style.display = '';
  } else if(opcao == 'N') {
    document.getElementById('temFilhos').style.display = 'none';
  }
}

function temOcupacao(opcao) {
  if (opcao == 'S') {
    document.getElementById('temOcupacao').style.display = '';
  } else if (opcao == 'N') {
    document.getElementById('temOcupacao').style.display = 'none';
  }
}

function carregarMunicipiosNaturalidade(natuf, bloqueio) {
  var bloq = '';
  if (typeof bloqueio == 'undefined') { bloq = "S";
  } else { bloq = bloqueio; }

  jQuery('#td_natmuncod').html('Carregando...');
  jQuery.ajax({
    type: "POST",
    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
    data: "requisicao=carregarMunicipios&nat=true&estuf=" + natuf + "&bloq=" + bloq,
    async: false, success: function(msg) {
      jQuery('#td_natmuncod').html(msg);
    }
  });
}

function carregarMunicipios2(estuf) {
  document.getElementById('td_muncod').innerHTML = 'Carregando...';
  jQuery.ajax({
    type: "POST",
    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
    data: "requisicao=carregarMunicipios2&estuf=" + estuf,
    async: false,
    success: function(msg){document.getElementById('td_muncod').innerHTML = msg;}
  });
}

function carregarMunicipios(estuf,muncod, bloqueio) {
  var bloq = '';
  if (typeof bloqueio == 'undefined') {
    bloq = "S";
  } else {
    bloq = bloqueio;  
  }
  document.getElementById('td_endmuncod').innerHTML = 'Carregando...';
  jQuery.ajax({
    type: "POST",
    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
    data: "requisicao=carregarMunicipios&estuf=" + estuf + "&muncod=" + muncod + "&bloq=" + bloq,
    async: false,
    success: function(msg) {
      document.getElementById('td_endmuncod').innerHTML = msg;
    }
  });
}

function buscarTurmas2(nucid, bloqueio) {
  var bloq = bloqueio;

  if (bloq == ""){
      bloq = "N";
  }
  if (nucid != ''){
    document.getElementById('td_turma2').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "requisicao=buscarTurmas2&nucid=" + nucid + "&bloq=" + bloq,
      async: false,
      success: function(msg){	
        document.getElementById('td_turma2').innerHTML = msg;
      }
    });
  }
}

function buscarNucleos2(polid, bloq) {
  if (polid != '') {
    document.getElementById('td_nucleo2').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "requisicao=buscarNucleos2&polid=" + polid + "&bloq=<?=$bloq ?>",
      async: false,
      success: function(msg) {
        document.getElementById('td_nucleo2').innerHTML = msg;
        document.getElementById('td_turma2').innerHTML
                = '<select id="endestuf" style="width: auto" class="CampoEstilo obrigatorio" name="endestuf"><option value="">Selecione</option></select>';
      }
    });
  }
}

function buscarTurmas(nucid, bloq) {
  if (nucid != '') {
    document.getElementById('td_turma').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "requisicao=buscarTurmas&nucid=" + nucid + "&bloq=<?=$bloq ?>",
      async: false,
      success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
    });
  }
}

function buscarNucleos(polid, bloq) {
  if (polid != '') {
    document.getElementById('td_nucleo').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
      data: "requisicao=buscarNucleos&polid=" + polid + "&bloq=<?=$bloq ?>",
      async: false,
      success: function(msg) {
        document.getElementById('td_nucleo').innerHTML = msg;
      }
    });
  }
}

function excluirEstudantes(caeid) {
  var conf = confirm('Deserealmente excluir o estudante?');
  if (conf) {
    window.location
            = 'projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&requisicao=excluirEstudante&caeid=' + caeid;
  }
}

function exibir_ocultar_historico (id) {
  div = document.getElementById(id);
  if (div.style.display == 'none') {
    div.style.display = 'block';
  } else {
    div.style.display = 'none';
  }
}

function verificaFaltas(caeid, caecpf, caestatus){
	  var turid = jQuery('#turid').val();
	  var nucid = jQuery('#nucid').val();

	  jQuery.ajax({
	    type: "POST",
	    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
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
		    url: "projovemurbano.php?modulo=principal/monitoramento2014&acao=A",
		    data: "requisicao=verificaProjovemcampo&caecpf=" + caecpf
		            + "&caestatus=" + caestatus,
		    async: false,
		    success: function(msg) {
		      if (msg == 1) {
		        alert('Aluno não poderá ser ativado por ter matrícula ativa no Projovem Campo.');	
		        jQuery('#caestatus_i').attr('checked', 'checked');
// 		        jQuery('input:radio[name=caestatus]').attr('checked', false);
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
    <td><? echo campo_texto('caecpf', "S", ($caecpf != '' ? 'N' : $bloq), "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="caecpf" required', '', '', 'carregarEstudante();' ); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Nome:</td>
    <td><? echo campo_texto('caenome', "S", ((!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis))?"N":$bloq), "Nome", 67, 70, "", "", '', '', 0, 'id="caenome" required class="obrigatorio normal alphaonly"' ); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Data de Nascimento:</td>
    <td><? echo campo_texto('caedatanasc', "S", ((!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis))?"N":$bloq), "Data de Nascimento", 12, 10, "##/##/####", "", '', '', 0, 'id="caedatanasc" required' ); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Idade:</td>
    <td><?=campo_texto('caeidade', 'S', ((!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis)) ? "N" : $bloq ), 'Idade', 5, 2, '###', '', '', '', 0, 'id="caeidade" required disabled'); ?></td>
    <?php if (isset($caedatanasc) && !empty($caedatanasc)): ?>
      <script type="text/javascript">
        jQuery(document).ready(function(){
          jQuery('#caeidade').val(calculaIdade('<?php echo $caedatanasc; ?>'));
        });
      </script>
    <?php endif; ?>
  </tr>
  <tr>
    <td class="SubTituloDireita">Sexo:</td>
    <td>
      <input type="radio" name="caesexo" id="caesexo_m" value="M" <?=(($caesexo=="M")?"checked":"");?> <?=$disabled;?>>
        <label for="caesexo_m">Masculino</label>
      <input type="radio" name="caesexo" id="caesexo_f" value="F" <?=(($caesexo=="F")?"checked":"");?> <?=$disabled;?>>
        <label for="caesexo_f">Feminino</label>
    </td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Nome da mãe:</td>
    <td><? echo campo_texto('caenomemae', "S", $bloq, "Nome da mãe", 67, 70, "", "", '', '', 0, 'id="caenomemae" required class="obrigatorio normal alphaonly"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Nome do pai:</td>
    <td><? echo campo_texto('caenomepai', "N", $bloq, "Nome do pai", 67, 70, "", "", '', '', 0, 'id="caenomepai" class="normal alphaonly"' ); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Nacionalidade:</td>
    <td>
    <?php
    $naturalidade = array(
        0 => array("codigo" => "B", "descricao" => "Brasileira"),
        1 => array("codigo" => "E", "descricao" => "Estrangeira"));
    $db->monta_combo('caenaturalidade', $naturalidade, $bloq, 'Selecione', '', '', '', '', 'S', 'caenaturalidade', null, null, null, 'required'); 

    if (in_array(PFL_CONSULTA, $perfis)): ?>
      <img width="13px" border="0" style="cursor:pointer;" title="Nacionalidade" src="../imagens/AtencaoP.gif"
           onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaNacionalidade');">
    <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td class="SubTituloDireita"  rowspan="2">Naturalidade:</td>
    <td>
      <?php
      //$endestuf = $estuf;
      $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
      $db->monta_combo('caeufnaturalidade', $sql, $bloq, 'Selecione a UF', 'carregarMunicipiosNaturalidade', '', '', '', 'N', 'caeufnaturalidade', null, null, null, 'required');
      if($caeufnaturalidade): ?>
      <script>
      jQuery(document).ready(function() {
          carregarMunicipiosNaturalidade('<?=$caeufnaturalidade; ?>', '<?=$bloq; ?>');
          jQuery('#caemuncodnaturalidade').val('<?=$caemuncodnaturalidade; ?>');
      });
      </script>
      <? endif; ?>
      <?php if (in_array(PFL_CONSULTA, $perfis)): ?>
          <img width="13px" border="0" style="cursor:pointer;" title="UF" src="../imagens/AtencaoP.gif"
               onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaUF');">
      <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td id="td_natmuncod"><span style="display:none">Naturalidade</span><span>
      <? $db->monta_combo('caemuncodnaturalidade', array(), $bloq, 'Selecione o Município', '', '', '', '', 'S', 'caemuncodnaturalidade'); ?>
      <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
        <img width="13px" border="0" style="cursor:pointer;" title="Município" src="../imagens/AtencaoP.gif"
             onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carrega_Municipio');">
      <?php endif; ?>
      </span>
    </td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Cor / Raça:</td>
    <td>
    <?php
    $sql = "SELECT craid as codigo, cradesc as descricao FROM projovemurbano.corraca WHERE crastatus='A' ORDER BY cradesc";
    $db->monta_combo('craid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'craid', null, null, null, 'required'); 
    if (in_array(PFL_CONSULTA, $perfis)): ?>
      <img width="13px" border="0" style="cursor:pointer;" title="Cor / Raça" src="../imagens/AtencaoP.gif"
           onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaRaca');">
    <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Carteira de identidade:</td>
    <td><? echo campo_texto('caenumrg', "S", $bloq, "Carteira de identidade", 17, 15, "", "", '', '', 0, 'id="caenumrg" required' ); ?> 
      <span>Orgão Emissor:</span> <span><? echo campo_texto('caeorgaoexpedidorrg', "S", $bloq, "Orgão Emissor", 12, 10, "", "", '', '', 0, 'id="caeorgaoexpedidorrg" required' ); ?></span>
       <span>UF de Emissão:</span> <span><?  $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
          echo $db->monta_combo('ufemissaorg', $sql, $bloq, 'Selecione a UF', '', '', '', '', 'S', 'ufemissaorg'); ?></span>
      <span>Data de Emissão:</span> <span><? echo campo_texto('caedataemissaorg', "S", $bloq, "Data de emissão", 12, 10, "##/##/####", "", '', '', 0, 'id="caedataemissaorg" required' ); ?></span></td>
  </tr>
  <? if(in_array(PFL_ADMINISTRADOR, $perfis)||in_array(PFL_EQUIPEMEC, $perfis)||$db->testa_superuser()){?>
	  <tr>
	  	<?$caenispispasep = $caenispispasep == 0 ? '' : $caenispispasep;?>
	    <td class="SubTituloDireita">Número de Identificação Social (NIS):</td>
	    <td><? echo campo_texto('caenispispasep', "N", "N", "NIS / PIS / PASEP", 13, 11, "", "", '', '', 0, 'id="caenispispasep"' ); ?></td>
	  </tr>
  <?}?>
    <tr>
      <td class="SubTituloDireita">Pessoa com deficiência:</td>
      <td>
        <input type="radio" name="caedeficiencia" value="TRUE" <?=(($caedeficiencia == "t")?"checked":"") ?> <?=$disabled ?>
               onclick="pessoaDeficiencia('S');" id="caedeficiencia_s" required><label for="caedeficiencia_s"> Sim </label>
        <input type="radio" name="caedeficiencia" <?=(($caedeficiencia=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE"
               onclick="pessoaDeficiencia('N');" id="caedeficiencia_n" required><label for="caedeficiencia_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)){?>
            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaDeficiencia')" style="cursor:pointer;" title="Transtorno global do desenvolvimento" src="../imagens/AtencaoP.gif">
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
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaTranstorno')" style="cursor:pointer;" title="Transtorno global do desenvolvimento" src="../imagens/AtencaoP.gif">
                        <?php }?>    
                </td>
	</tr>
    <tr>
      <td class="SubTituloDireita">Altas Habilidades/Superdotação:</td>
      <td>
        <input type="radio" name="caealtashabilidades" <?=(($caealtashabilidades == 't')?'checked':'') ?> <?=$disabled ?>
               value="TRUE" id="caealtashabilidades_s" /><label for="caealtashabilidades_s"> Sim </label>
        <input type="radio" name="caealtashabilidades" <?=(($caealtashabilidades == 'f')?'checked':'') ?> <?=$disabled ?>
               value="FALSE" id="caealtashabilidades_n" /><label for="caealtashabilidades_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif" />
      </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Tipo de recurso de acessibilidade que utiliza:</td>
      <td>
        <?php 
        $sql = "SELECT racid as codigo, racdesc as descricao FROM projovemurbano.recursosacessibilidade WHERE racstatus = 'A'";
        if ($_REQUEST['caeid']) {
          $marcados = $db->carregarColuna('SELECT racid FROM projovemurbano.cadastroestudante_recursosacessibilidade WHERE
          caeid = '.$_REQUEST['caeid']);
        }
        $db->monta_checkbox('racid[]', $sql, $marcados, '<br>', $comLabel = true);
        ?>
      </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Estado Civil:</td>
      <td>
        <?php
        $sql = "SELECT escid as codigo, escdesc as descricao FROM projovemurbano.estadocivil WHERE escstatus='A' ORDER BY escdesc";
        $db->monta_combo('escid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'escid', null, null, null, 'required');
        if (in_array(PFL_CONSULTA, $perfis)): ?> 
          <img width="13px" border="0" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif"
               onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaEstadoCivil');">
        <?php endif; ?>
      </td>
    </tr>
    <tr>
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
			
			if( $caeid ){
				$estqtdfilhos = ",
						(SELECT estqtdfilhos::character(7) 
	                	FROM projovemurbano.estudantefilhos e 
	                	WHERE i.idfid=e.idfid AND e.caeid='".$caeid."'  AND esfstatus = 'A') as estqtdfilhos";
			}
			$sql = "SELECT 
						idfdesc, 
						idfid
						$estqtdfilhos
                	FROM 
                		projovemurbano.idadefilhos i 
                	WHERE 
                		idfstatus='A' 
                	ORDER BY idfid";
			
			$linhas = $db->carregar($sql);
//			ver(simec_htmlentities($sql),d);
			?>
			<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
				<thead>
					<tr>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Idade</td>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Quantidade de filhos</td>
					</tr> 
				</thead>
				<tbody>
			<? 
			$arr = Array();
			for( $x=0; $x < 10; $x++ ){
				$arr[] = Array( 'codigo' => $x, 'descricao' => $x );
			}
			if( is_array($linhas) ){
				foreach($linhas as $linha){
				
				
			?>
					<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
						<td valign="top" title="Idade"><?=$linha['idfdesc'] ?></td>
						<td valign="top" title="Quantidade de filhos"><?=$db->monta_combo("estqtdfilhos[{$linha['idfid']}]", $arr, $bloq, 'Selecione', '', '', '', '', 'S', "estqtdfilhos[{$linha['idfid']}]", null, $linha['estqtdfilhos'], null, 'required'); ?></td>
					</tr>
			<? 
				}
			}
			?>
				</tbody>
			</table>
			
		</td>
	</tr>
    <tr>
      <td class="SubTituloDireita">Beneficiários de outros programas:</td>
      <td>
        <input type="radio" name="caebenoutroprog" <?=(($caebenoutroprog=="t")?"checked":"") ?> <?=$disabled ?>
               value="TRUE" onclick="outrosProgramas('S');" id="caebenoutroprog_s"><label for="caebenoutroprog_s"> Sim </label>
        <input type="radio" name="caebenoutroprog" <?=(($caebenoutroprog=="f")?"checked":"") ?> <?=$disabled ?>
               value="FALSE" onclick="outrosProgramas('N');" id="caebenoutroprog_n"><label for="caebenoutroprog_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)): ?>
          <img width="13px" border="0" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif"
               onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaProgramasBeneficiarios');" />
        <?php endif; ?>
      </td>
    </tr>
    <tr id="outrosProgramas" <?=(($caebenoutroprog=="t")?"":"style=\"display:none;\"") ?> >
      <td class="SubTituloDireita"><b>Qual programa?</b></td>
      <td>
        <?php
        $sql = "SELECT pbeid as codigo, pbedesc as descricao FROM projovemurbano.programabeneficiario WHERE pbestatus='A' ORDER BY pbedesc";
        $db->monta_combo('pbeid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'pbeid');
        ?>
      </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Jovem em cumprimento de medidas socioeducacionais:</td>
      <td>
        <input type="radio" name="caecumpremedidasocioeduc" <?=(($caecumpremedidasocioeduc == "t")?"checked":"") ?> <?=$disabled ?>
               value="TRUE" id="caecumpremedidasocioeduc_s"><label for="caecumpremedidasocioeduc_s"> Sim </label>
        <input type="radio" name="caecumpremedidasocioeduc" <?=(($caecumpremedidasocioeduc  == "f")?"checked":"") ?> <?=$disabled ?>
               value="FALSE" id="caecumpremedidasocioeduc_n"><label for="caecumpremedidasocioeduc_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)): ?>
          <img width="13px" border="0" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif"
               onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaProgramasBeneficiarios');" />
        <?php endif; ?>
      </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Tem alguma ocupação?</td>
      <td>
        <input type="radio" name="caeocupacao" <?=(($caeocupacao=="t")?"checked":"") ?> <?=$disabled ?>
               value="TRUE" onclick="temOcupacao('S');" id="caeocupacao_s"><label for="caeocupacao_s"> Sim </label>
        <input type="radio" name="caeocupacao" <?=(($caeocupacao=="f")?"checked":"") ?> <?=$disabled ?>
               value="FALSE" onclick="temOcupacao('N');" id="caeocupacao_n"><label for="caeocupacao_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
        <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaOcupacao')" style="cursor:pointer;" title="Estado Civil" src="../imagens/AtencaoP.gif">
        <?php endif; ?>
      </td>
    </tr>
    <tr id="temOcupacao" <?=(($caeocupacao=="t")?"":"style=\"display:none;\"") ?> >
      <td class="SubTituloDireita"><b>Qual ocupação?</b></td>
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
			$sql = "SELECT ssoid as codigo, ssodesc as descricao FROM projovemurbano.segmentosocial WHERE ssostatus='A' AND ppuid = 2 ORDER BY ssodesc";
			$db->monta_combo('ssoid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'ssoid', null, null, null, 'required');
			?> 
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaSegmentoSocial')" style="cursor:pointer;" title="Segmento Social" src="../imagens/AtencaoP.gif">
                        <?php }?>
                </td>
	</tr>
    <tr>
      <td class="SubTituloDireita">É egresso do Programa Brasil Alfabetizado?</td>
      <td>
        <input type="radio" name="caebrasilalfa" <?=(($caebrasilalfa=="t")?"checked":"") ?> <?=$disabled ?>
               value="TRUE" id="caebrasilalfa_s"><label for="caebrasilalfa_s" /> Sim </label>
        <input type="radio" name="caebrasilalfa"  <?=(($caebrasilalfa=="f")?"checked":"") ?> <?=$disabled ?>
               value="FALSE" id="caebrasilalfa_n"><label for="caebrasilalfa_n" /> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
      </td>
    </tr>
      <td class="SubTituloDireita">É egresso de outro programa de alfabetização?</td>
      <td>
        <input type="radio" name="egressoprogalfabetizacao" id="egressoprogalfabetizacao_s"
               value="TRUE" <?=(($egressoprogalfabetizacao=="t")?"checked":"") ?> <?=$disabled ?> />
        <label for="egressoprogalfabetizacao_s"> Sim </label>
        <input type="radio" name="egressoprogalfabetizacao" id="egressoprogalfabetizacao_n"
               value="FALSE" <?=(($egressoprogalfabetizacao=="f")?"checked":"") ?> <?=$disabled ?> />
        <label for="egressoprogalfabetizacao_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
      </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Já participou do Projovem Urbano:</td>
      <td>
        <input type="radio" name="caeparticipouprojovemurbano" <?=(($caeparticipouprojovemurbano=="t")?"checked":"") ?> <?=$disabled ?>
               value="TRUE" id="caeparticipouprojovemurbano_s"><label for="caeparticipouprojovemurbano_s" /> Sim </label>
        <input type="radio" name="caeparticipouprojovemurbano"  <?=(($caeparticipouprojovemurbano == "f")?"checked":"") ?> <?=$disabled ?>
               value="FALSE" id="caeparticipouprojovemurbano_n"><label for="caeparticipouprojovemurbano_n" /> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
      </td>
    </tr>
    <tr>
    <tr>
      <td class="SubTituloDireita">Endereço</td>
      <td>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
          <tr>
            <td class="SubTituloDireita" width="20%">CEP</td>
            <td><? echo campo_texto('caecep', "S", $bloq, "CEP", 11, 9, "#####-###", "", '', '', 0, 'id="caecep" required', '', '', 'getEnderecoPeloCEP(this.value);'); ?></td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Logradouro</td>
            <td>
            	<input type="hidden" id="caelogradouro" name="caelogradouro" value="<?=$caelogradouro?>"/>
				<label id="caelogradouro_label"><?=$caelogradouro?></label>
			</td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Número</td>
            <td><? echo campo_texto('caenumero', "S", $bloq, "Número", 12, 10, "", "", '', '', 0, 'id="caenumero" required' ); ?></td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Complemento</td>
            <td><? echo campo_texto('caecomp', "N", $bloq, "Complemento", 67, 150, "", "", '', '', 0, 'id="caecomp"' ); ?></td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Bairro</td>
            <td>
            	<input type="hidden" id="caebairro" name="caebairro" value="<?=$caebairro?>"/>
				<label id="caebairro_label"><?=$caebairro?></label>
			</td>
          </tr>
          <tr>
            <td class="SubTituloDireita">UF</td>
            <td>
              <? 
              $endestuf = $estuf;
              ?>
            	<input type="hidden" id="endestuf" name="endestuf" value="<?=$endestuf?>"/>
				<label id="endestuf_label"><?=$endestuf?></label>
              <? if($endestuf): ?>
              <script>
              jQuery(document).ready(function() {
                      carregarMunicipios('<?=$endestuf ?>','<?=$muncod ?>','<?=$bloq?>');
                      jQuery('#endmuncod').val('<?=$muncod ?>');
              });
              </script>
              <? endif; ?>
              <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
              <img width="13px" border="0" style="cursor:pointer;" title="UF" src="../imagens/AtencaoP.gif"
                   onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaUF');">
              <?php endif; ?>
            </td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Município</td>
            <td>
             <input type="hidden" id="endmuncod" name="endmuncod" value="<?=$municipio['descricao']?>"/>
			 <label id="td_endmuncod"><?=$municipio['descricao']?></label>
              <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
                  <img width="13px" border="0" style="cursor:pointer;" title="Município" src="../imagens/AtencaoP.gif"
                       onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carrega_Municipio');">
              <?php endif; ?>
            </td>
          </tr>
          <tr><?if($caetipomoradia == 'U'){
          			$caetipomoradia = 'u';
         		}elseif($caetipomoradia == 'R'){
         			$caetipomoradia = 'r';
         		} ?>
            <td class="SubTituloDireita">Qual Região de Moradia?</td>
            <td>
              <input type="radio" name="caetipomoradia" <?=(	($caetipomoradia == 'u')
                                                                ?'checked'
                                                                : 
	                                                                ( ($caeid != '') ? 
		                                                                (	($caetipomoradia == 'u')
		                                                                    ? 'checked'
		                                                                    : ''
		                                                            	)
	                                                                : 
																		''				                                                                
	                                                                )
															); ?> 
															<?=$disabled ?>
                     value="u" id="caetipomoradia_u"><label for="caetipomoradia_u" /> Urbano </label>
              <input type="radio" name="caetipomoradia"  <?=(	($caetipomoradia == 'r')
                                                                ?'checked'
                                                                :
	                                                                ( ($caeid != '') ? 
		                                                                (	($caetipomoradia == 'u')
		                                                                    ? ''
		                                                                    :'checked'
		                                                            	)
	                                                                : 
																		''				                                                                
	                                                                )
															); ?> 
															<?=$disabled ?>
                     value="r" id="caetipomoradia_r"><label for="caetipomoradia_r" /> Rural </label>
              <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
          </tr>
          <tr>
            <td class="SubTituloDireita">E-mail</td>
            <td><? echo campo_texto('caeemail', "N", $bloq, "E-mail", 67, 100, "", "", '', '', 0, 'id="caeemail"' ); ?></td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Telefone fixo</td>
            <td><? echo campo_texto('caetelfixo', "S", $bloq, "Telefone fixo", 13, 13, "##-#####-####", "", '', '', 0, 'id="caetelfixo" required','if(this.value.length>12){ this.value=mascaraglobal(\'##-#####.####\',this.value);}else{ this.value=mascaraglobal(\'##-####.####\',this.value);}' ); ?> Formato: DD-####-####</td>
          </tr>
          <tr>
            <td class="SubTituloDireita">Telefone celular</td>
            <td><? echo campo_texto('caetelcel', "N", $bloq, "Telefone celular", 13, 13, "##-#####-####", "", '', '', 0, 'id="caetelcel"','if(this.value.length>12){ this.value=mascaraglobal(\'##-#####.####\',this.value);}else{ this.value=mascaraglobal(\'##-####.####\',this.value);}' ); ?> Formato: DD-####-####</td>
          </tr>
        </table>
      </td>
    </tr>

    <tr>
      <td class="SubTituloDireita">Apresentou histórico escolar ?</td>
      <td>
        <input type="radio" name="caehistorico" <?=(($caehistorico == 't')?'checked':'') ?> <?=$disabled ?>
               value="TRUE" id="caehistorico_s"><label for="caehistorico_s"> Sim </label>
        <input type="radio" name="caehistorico" <?=(($caehistorico == 'f')?'checked':'') ?> <?=$disabled ?>
               value="FALSE" id="caehistorico_n"><label for="caehistorico_n"> Não </label>
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
      </td>
    </tr>
    <tr>
      <td class="SubTituloDireita">Realizou teste de proficiência ?</td>
      <td>
        <input type="radio" name="caetestepro" <?=(($caetestepro=="t")?"checked":"") ?> <?=$disabled ?>
               value="TRUE" id="caetestepro_s"><label for="caetestepro_s"> Sim </label>
        <input type="radio" name="caetestepro" <?=(($caetestepro=="f")?"checked":"") ?> <?=$disabled ?>
               value="FALSE" id="caetestepro_n"><label for="caetestepro_n"> Não </label>
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
					    		ELSE '1'
					    		END  as auxilios
					   FROM projovemurbano.cadastroestudante WHERE caeid='{$_REQUEST['caeid']}'"; 
			   $auxilios = $db->pegaUm($sql);
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
          if($_SESSION['projovemurbano']['ppuid']==3){
          	$filtrotprid = "AND tprid='{$_SESSION['projovemurbano']['tprid']}'";
          }
            $sql = "SELECT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A' $filtrotprid";
            $pmupossuipolo = $db->pegaUm($sql);
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

            if($pmupossuipolo=="t") :
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
                    $db->monta_combo('polid', $sql, $bloq, 'Selecione', 'buscarNucleos', '', '', '', 'S', 'polid', null, null, null, 'required');

                    if ($polid): ?>
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
              <? $db->monta_combo('nucid', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'nucid', null, null, null, 'required'); ?>
              <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
                  <img width="13px" border="0" style="cursor:pointer;" title="Município" src="../imagens/AtencaoP.gif"
                       onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carrega_Municipio&endestuf=MG');" />
              <?php endif; ?>
            </td>
          </tr>
          <tr>
            <td class="SubTituloDireita"><b>Turma</b></td>
            <td><label id="td_turma"><? $db->monta_combo('turid', array(), $bloq, 'Selecione', '', '', '', '150', 'S', 'turid', null, null, null, 'required'); ?><label></td>
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
            <? if($pmupossuipolo=="f") : ?>
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
                <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaNucleoEscola&possuipolo=<?php echo $polomunicipio['pmupossuipolo'] ?>')" style="cursor:pointer;" title="Núcleo / Escolas" src="../imagens/AtencaoP.gif">
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
                <img width="13px" border="0" onmouseover="SuperTitleAjax('projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes&reqTittle=carregaNucleoTurma&nucid=<?=$nucid ?>')" style="cursor:pointer;" title="Município" src="../imagens/AtencaoP.gif">
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
        <input type="radio" name="caestatus" value="A" <?=(($caestatus == 'A'||!$caestatus)?'checked':'') ?> <?php echo $disabledStatus ?>
               onchange="return verificaFaltas('<?php echo $cadastroestudante['caeid']; ?>','<?php echo $cadastroestudante['caecpf']; ?>', '<?php echo $caestatus; ?>');"
               id="caestatus_a" />
        <label for="caestatus_a"> Ativo </label>
        <input type="radio" name="caestatus" value="I" <?=(($caestatus == 'I')?'checked':'') ?> <?php echo $disabledStatus; ?>
               id="caestatus_i" />
        <label for="caestatus_i"> Inativo - outros motivos </label>
        <input type="radio" name="caestatus" value="D" <?=(($caestatus=="D")?"checked":"") ?> <?php echo 'disabled' ?>
               id="caestatus_d">
        <label for="caestatus_d"> Inativo - desistência &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </label>
        <?php if ($caeid): ?>
          <a href="javascript: exibir_ocultar_historico('historico_geral');">
            <img src="/imagens/mais.gif" style="border: 0" /> histórico
          </a>
          <div id="historico_geral" style="width: 500px; display: none">
            <p>
            <?php
              $cabecalho = array("Responsável pelo Cadastramento", "Data", "Status", "Ação");
              $sql = sprintf("
                      SELECT sg.usunome,
                             TO_CHAR(hicdataacao, 'DD/MM/YYYY') AS  hicdataacao,
                             CASE WHEN hictipo = 'A' THEN 'Ativo' ELSE 'Inativo' END AS hictipo, 
                             CASE WHEN hicacao = 'A' THEN 'Alteração' ELSE 
                                CASE WHEN hicacao = 'I' THEN 'Inserção' ELSE 
                                   CASE WHEN hicacao = 'D' THEN 'Excluido' END
                                END
                             END AS hicacao
                      FROM projovemurbano.historicocadastro hc
                        JOIN seguranca.usuario sg ON sg.usucpf = hc.usucpf
                        JOIN projovemurbano.cadastroestudante cd ON cd.caeid = hc.caeid
                      WHERE hc.caeid = $caeid OR cd.caecpf = '$caecpf_sem_masc'
                      ORDER BY hicdataacao", 
              $caeid);
              $db->monta_lista_simples($sql, $cabecalho, 25, 0);
            ?>
            </p>
          </div>
        <?php endif; ?>
      </td>
    </tr>
    <tr id="tr_caejustificativainativacao" <?=(($caestatus!="I")?"style=\"display:none\"":"") ?>>
      <td class="SubTituloDireita"><b>Justificativa para inativação:</b></td>
      <td>
        <?php
        $sql = "
				SELECT minid AS codigo, descricao AS descricao
				  FROM projovemurbano.motivoinativacao
				  ORDER BY descricao
				";
        $db->monta_combo('minid', $sql, $bloq, 'Selecione o Motivo da Inativação', '', '', '', '', 'S', 'minid');
        ?>
        <span id="descricaomotivoinativacao_div"<?php if (-1 != $minid): ?> style="display:none"<?php endif; ?>>
        	<? $descricaomotivoinativacao = ltrim($descricaomotivoinativacao) ?>
        	<? $descricaomotivoinativacao = rtrim($descricaomotivoinativacao) ?>
          Digite o motivo: <?php //echo campo_texto('descricaomotivoinativacao', 'S', '', 'Digite o outro motivo', 20, 20, '', '', '', '', 0, 'id="descricaomotivoinativacao"' ); ?>
          <?=campo_textarea("descricaomotivoinativacao","N","S","Digite o outro motivo", 60, 3, 200,"","","","","",$descricaomotivoinativacao)?>
        </span>          
      </td>
    </tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<?php if($msgMeta!=''){?>
				<label style="color:red"><?=$msgMeta ?></label><br>
			<?php }
//            /*
//             */
           $dataAtual      = mktime( date('H'), date('i'), date('s'), date('m'), date('d'), date('Y') );
           $dataBloqueio   = mktime( 23, 59, 00, 28,2, 2014 );
           $bloqueioHorario = (bool) ( $dataAtual > $dataBloqueio );
//          ver($dataAtual,$dataBloqueio,d);  
//            if( /*!$bloqueioHorario ||*/ !empty($_REQUEST['caeid']) || in_array(PFL_EQUIPE_MEC, $perfis)|| (in_array(PFL_SUPER_USUARIO, $perfis))) {  
                	echo '<input type="button" name="salvar" value="Salvar" '. $disabled .' onclick="gravarCadastroEstudantes();">';
            ?>
				

			<?/* if($caeid) : ?>
				<input type="button" name="novo" value="Novo" <?=$disabled ?> onclick="window.location='projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba=cadastroEstudantes';">
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
			<td class="SubTituloDireita">Município</td>
			<td id="td_muncod">
				<? 
					$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_SESSION['projovemurbano']['estuf'] ."' ORDER BY mundescricao";
					$db->monta_combo('muncod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'muncod');
				?>
			</td>
		</tr>
		--><?
		if($_SESSION['projovemurbano']['ppuid']==3){
			$filtrotprid = "AND tprid='{$_SESSION['projovemurbano']['tprid']}'";
		}
		$sql = "SELECT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A' $filtrotprid";
		$pmupossuipolo = $db->pegaUm($sql);
		if($pmupossuipolo =="t") :
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
		<? if($pmupossuipolo=="f") : ?>
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
			<td><? echo campo_texto('caenome', "N", "S", "Nome", 50, 255, "", "", '', '', 0, 'id="caenome" class="alphaonly normal"' ); ?></td>
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
if($_SESSION['projovemurbano']['ppuid']= 3){
	$anoM = 2014;
}

//<img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirEstudantes(\''||cae.caeid||'\')\" > 
$sql = "SELECT DISTINCT
			'<img src=../imagens/alterar.gif title=\"Editar\" border=0 style=cursor:pointer; onclick=\"window.location=\'projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba2=cadastroEstudantes&caeid='||cae.caeid||'\';\"> 
			<img src=../imagens/report.gif border=0 width=\"13px\"  title=\"Imprimir Comprovante de Inscrição\" style=cursor:pointer; onclick=\"window.open(\'projovemurbano.php?modulo=principal/popComprovante&acao=A&caeid='||cae.caeid||'\',\'Comprovante\',\'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">
			<img src=../imagens/lista_verde.gif border=0 width=\"13px\"  title=\"Imprimir Ficha de Matricula\" style=cursor:pointer; onclick=\"window.open(\'projovemurbano.php?modulo=principal/popFichaMatricula2014&acao=A&caeid='||cae.caeid||'\',\'Comprovante\',\'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">' as acao,
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
      		AND nuc.tprid = ".$_SESSION['projovemurbano']['tprid']."
			$where_nucleo 
			".(($filtro_nucleo)?" AND nuc.nucid IN('".implode("','",$filtro_nucleo)."')":"")
			.(($where)?" AND ".implode(" AND ",$where) :"")."
		ORDER BY caenome";
// ver($sql,d);
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