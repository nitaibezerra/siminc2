<?php
$bloq = $bloqStatus = 'S';
$disabled = $disabledStatus = '';
// echo "<script>alert('Em manutenção.');window.location = 'projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A';</script>";
// die;
if ($_REQUEST['estid']) {
	
  $requisicao = "atualizarEstudantes";
  $sql = "SELECT * FROM projovemcampo.estudante WHERE estid='{$_REQUEST['estid']}'";

  $estudante = $db->pegaLinha($sql);
// ver($estudante,d);  
  if (is_array($estudante)) {
    extract($estudante);
  } else {
    echo "<script>alert('Estudante não encontrado.');window.location = 'projovemcampo.php?modulo=principal/monitoramento&acao=A';</script>";
  }
  $estcpf_sem_masc = $estcpf;
  $estdatanascimento = formata_data($estdatanascimento);
  $estdataemissaorg = formata_data($estdataemissaorg);
  //$estdataemissaorg = formata_data($estdataemissaorg);
  $estendcep = mascaraglobal($estendcep, "#####-###");
  $estcpf = mascaraglobal($estcpf, "###.###.###-##");

  if (trim($estnis) != ''){
    $bloq = 'N';
    $disabled = 'disabled="disabled"';
  }
} else {
  $requisicao = "inserirEstudantes";
}
$perfis = pegaPerfilGeral();
$meta = pegameta();
$sql = "SELECT
			count(estid)
		FROM 
			projovemcampo.estudante c
		INNER JOIN projovemcampo.turma tur ON tur.turid = c.turid
		INNER JOIN	projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid 
		WHERE
			apc.apcid = {$_SESSION['projovemcampo']['apcid']}
			AND eststatus = 'A'";
$qtdestudantes = $db->pegaUm($sql);

if (!$db->testa_superuser()) {
  if(in_array(PFL_DIRETOR_ESCOLA, $perfis)) {
    $bloq     = $bloqStatus     = 'S';
    $disabled = $disabledStatus = '';
  } elseif (in_array(PFL_CONSULTA, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } elseif (in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } elseif (in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
    $bloq     = $bloqStatus     = 'N';
    $disabled = $disabledStatus = 'disabled="disabled"';
  } elseif (in_array(PFL_COORDENADOR_TURMA, $perfis)) {
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
<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/datapicker.pt_Br.js"></script>
<link rel="stylesheet" type="text/css"
	href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" />
<script>jQuery.noConflict();</script>
<script type="text/javascript" src="./js/projovemcampo.js"></script>
<script language="javascript" type="text/javascript"
	src="../includes/webservice/cpf.js" /></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text'/css"
	href="../../includes/superTitle.css" />
<style type="text/css">
label {
	cursor: pointer
}
</style>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
  jQuery('#turid').attr('disabled','disabled');
  jQuery('[name="esthistoricoescolar"]').click(function() {
    if(jQuery(this).val() == 'TRUE') {
      jQuery('[name="esttesteproficiencia"][value="FALSE"]').attr('checked', true);
    } else {
      jQuery('[name="esttesteproficiencia"][value="TRUE"]').attr('checked', true);
    }
  });
  jQuery('[name="esttesteproficiencia"]').click(function(){
    if (jQuery(this).val() == 'TRUE') {
      jQuery('[name="esthistoricoescolar"][value="FALSE"]').attr('checked', true);
    } else {
      jQuery('[name="esthistoricoescolar"][value="TRUE"]').attr('checked', true);
    }
  });
  jQuery('[name="eststatus"]').click(function() {
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
      jQuery('#estnomesocial').val('');
    }
  });
//   if (jQuery('#estcpf').val() != '') {
// 	  alert
//     jQuery.ajax({
//       type: "POST",
//       url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
//       data: "requisicao=verificaCertificadoTotalBolsa&estnome=" + jQuery('#estnome').val()
//               + "&cpf=" + jQuery('#estcpf').val()
//               + "&estnomemae=" + jQuery('#estnomemae').val()
//               + "&estdatanascimento=" + jQuery('#estdatanascimento').val()
//               + "&estnumrg=" + jQuery('#estnumrg').val(),
//       async: false,
//       success: function(certificadoBolsa) {
//         var cebo = certificadoBolsa.split(";");
//         jQuery('#estauxiliosareceber').val(cebo[1]);
//         jQuery('#div_estauxiliosareceber').html(cebo[1]);
//       }
//     });
//   }

  // -- Campos que aceitam apenas texto
  jQuery('.alphaonly').bind('keyup blur', function() {
    jQuery(this).val(
            jQuery(this).val().replace(/[^a-záéíóúãõâêîôûç ]/gi, ''));
  });

  // -- Datepicker
  jQuery("#estdatanascimento").datepicker({
    maxDate:"+0D", changeMonth: true, changeYear: true, showOtherMonths: true,
    selectOtherMonths: true, createButton: false, onSelect: function(selectedDate) {
      jQuery('#estidade').val(calculaIdade(selectedDate));
    }
  });
  jQuery('#estdataemissaorg').datepicker({
    maxDate:'+0D', changeMonth: true, changeYear: true, showOtherMonths: true,
    selectOtherMonths: true, createButton: false});
  
  // -- Nome do pai e da mãe mudam para IGNORADA/IGNORADO quando vázios
  jQuery('#estnomemae').blur(function(){
    if ('' == jQuery(this).val().trim()) { jQuery(this).val('IGNORADA'); }
  });
  jQuery('#estnomepai').blur(function(){
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
  jQuery('#estendlogradouro').val(dadosEndereco[0]);
  jQuery('#estendlogradouro_label').html(dadosEndereco[0]);
  jQuery('#estendbairro').val(dadosEndereco[1]);
  jQuery('#estendbairro_label').html(dadosEndereco[1]);
  jQuery('#estendestuf').val(dadosEndereco[3]).change();
  jQuery('#estendestuf_label').html(dadosEndereco[3]);
  jQuery('#estendmuncod').val(dadosEndereco[4]);
  jQuery('#td_estendmuncod').html(dadosEndereco[2]);
  jQuery('#estendnumero').val('');
  jQuery('#estendcomplemento').val('');
}

function carregarEstudante() {

  jQuery('#estcpf').val(mascaraglobal('###.###.###-##', jQuery('#estcpf').val()));
  if (!validar_cpf(replaceAll(replaceAll(jQuery('#estcpf').val(), ".", ""), "-", ""))) {
    alert('CPF inválido');
    jQuery('#estcpf').val('');
    return false;
  }

  var cadastrado = false;
  var msg = false;
  
  jQuery.ajax({
    type: "POST",
    url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
    data: "requisicao=verificaCadastroCPF&cpf=" + replaceAll(replaceAll(jQuery('#estcpf').val(), ".", ""), "-", ""),
    async: false,
    success: function(msg) {

      if (msg == '1' &&jQuery('#estcpf').val() != '<?=$estcpf?>') {
        jQuery('#estcpf').val('');
        cadastrado = true;
        alert('Este aluno já possui cadastro.');
      } else if(msg=='2'){
    	  jQuery('#estcpf').val('');
          cadastrado = true;
          alert('Este aluno já possui cadastro ativo no Projovem Urbano.');
      }else if(msg == 2){
    	  jQuery('#estcpf').val('');
          cadastrado = true;
          alert('Este aluno possui cadastro ativo no Projovem Urbano.');	
      }
    }
  });

  if (cadastrado) {
      return false;
  }
  var comp = new dCPF();
  comp.buscarDados(jQuery('#estcpf').val());
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
	  var idade = <?=date("Y"); ?> - parseInt(comp.dados.dt_nascimento_rf.substr(0, 4));
// 	  alert(idade);
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
    jQuery('#estcpf').val('');
    jQuery('#estnome').val('');
    jQuery('#estnomemae').val('');
    jQuery('#estdatanascimento').val('');
    jQuery('#estnumrg').val('');
    jQuery('#estendestuf').val('');
    jQuery('#estendmuncod').val('');
    jQuery('#estendbairro').val('');
    jQuery('#estendlogradouro').val('');
    jQuery('#estendcomplemento').val('');
    jQuery('#estendnumero').val('');
    jQuery('#estendcep').val('');
    jQuery('#esttelefone').val('');
    return false;
  }//else if( (idade==17 && <?=date('m') ?> > comp.dados.dt_nascimento_rf.substr(4,2) ) || 
//			  (idade==30 && <?=date('m') ?> < comp.dados.dt_nascimento_rf.substr(4,2) )  ){
//		alert('Estudante com idade diferente de 18 a 29 anos de idade');
//		jQuery('#estcpf').val('');
//		jQuery('#estnome').val('');
//		jQuery('#estnomemae').val('');
//		jQuery('#estdatanascimento').val('');
//		jQuery('#estnumrg').val('');
//		jQuery('#estendestuf').val('');
//		jQuery('#estendmuncod').val('');
//		jQuery('#estendbairro').val('');
//		jQuery('#estendlogradouro').val('');
//		jQuery('#estendcomplemento').val('');
//		jQuery('#estendnumero').val('');
//		jQuery('#estendcep').val('');
//		jQuery('#esttelefone').val('');
//		return false;
//	}
//   var possuiCertificado = false;
//   jQuery.ajax({
//     type: "POST",
//     url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
//     data: "requisicao=verificaCertificadoTotalBolsa&cpf=" + jQuery('#estcpf').val(),
//     async: false,
//     success: function(certificadoBolsa){
//       var cebo = certificadoBolsa.split(";");
//       if (cebo[0] == "1") {
//         possuiCertificado = true;
//       }
//       jQuery('#estauxiliosareceber').val(cebo[1]);
//       jQuery('#div_estauxiliosareceber').html(cebo[1]);
//     }
//   });

//   if (possuiCertificado) {
//     alert('Estudante já certificado no Ensino Fundamental');
//     jQuery('#estcpf').val('');
//     return false;
//   }
  //carregando dados pessoais
  jQuery('#estnome').val(comp.dados.no_pessoa_rf);
  jQuery('#estnomemae').val(comp.dados.no_mae_rf);
  jQuery('#estdatanascimento').val(comp.dados.dt_nascimento_rf.substr(6, 2)
          + '/' + comp.dados.dt_nascimento_rf.substr(4, 2)
          + '/' + comp.dados.dt_nascimento_rf.substr(0, 4));
  jQuery('#estidade').val(calculaIdade(jQuery('#estdatanascimento').val()));
  jQuery('#estnumrg').val(comp.dados.nu_rg);

  jQuery("[name='estsexo'][value='" + comp.dados.sg_sexo_rf + "']").attr('checked', true);

  //carregando endereco
//   if (comp.dados.sg_uf) {
//     jQuery('#estendestuf_label').html(comp.dados.sg_uf);
//     carregarMunicipios(comp.dados.sg_uf);
//     jQuery('#estendmuncod').val(comp.dados.co_cidade.substr(8));
//   }
//   jQuery('#estendbairro_label').html(comp.dados.ds_bairro);001
//   jQuery('#estendlogradouro_label').html(comp.dados.ds_logradouro);
//   jQuery('#estendlogradouro_label').html(comp.dados.ds_logradouro);
//   jQuery('#estendcomplemento').val(comp.dados.ds_logradouro_comp);
//   jQuery('#estendnumero').val(comp.dados.ds_numero);
//   jQuery('#estendcep').val(mascaraglobal('#####-###', comp.dados.nu_cep));
//   jQuery('#esttelefone').val(mascaraglobal('##-####-####', replaceAll(comp.dados.ds_contato_pessoa, "-", "")));

}

function gravarestudantes() {

  jQuery('[name=salvar]').attr('disable','true');
  
  var formOk = true;
  var cpfEstudante = jQuery('#estcpf').val().replace(/[^0-9]/g, '');

  jQuery('#form [required]').each(function() {
    var nomeCampo = jQuery(this).parent().prev().text().replace(/[^a-záéíóúãõâêîôûç \/]/gi,'');
	if( nomeCampo == '' ){
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
        case 'estcpf': estaInvalido = (!validar_cpf(replaceAll(replaceAll(jQuery('#estcpf').val(), ".", ""), "-", ""))); break;
        case 'estdatanascimento': estaInvalido = (!validaData(document.getElementById('estdatanascimento'))); break;
        case 'estdataemissaorg': estaInvalido = (!validaData(document.getElementById('estdataemissaorg'))); break;
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
  if(jQuery('#estdataemissaorg').val()==''){
		alert('Preencha o campo Data emissão RG');
		rolaTela(estdataemissaorg);
		return false;
  }
  if(jQuery('#escid').val()==''){
		alert('Escolha uma opção para o campo Estado Civil');
		rolaTela(escid);
		return false;
  }

  if(isNaN(parseInt(jQuery('#sesid').val()))){
  		alert('Escolha uma opção para o Segmento Social');
		rolaTela(sesid);
		return false;
  }
  var estendregiaomoradia = parseInt(jQuery("[name = 'estendregiaomoradia']:checked").length); 
	if (estendregiaomoradia == 0) {
    alert('Você deve selecionar uma opção para o campo "Qual Região de Moradia".');
    rolaTela('estendregiaomoradia');
    return false;
  }
  
  if (!formOk) { return false; }

  var numestsexo = parseInt(jQuery("[name^='estsexo']:enabled:checked").length);
  if (numestsexo == 0) {
    alert('Você deve selecionar uma opção para o campo "Sexo".');
    rolaTela('estsexo_m');
    return false;
  }

  var numesttemdeficiencia = parseInt(jQuery("[name^='esttemdeficiencia']:enabled:checked").length);
  if (numesttemdeficiencia == 0 ||numesttemdeficiencia=='') {
    alert("Você deve selecionar uma opção para o campo 'Pessoa com deficiência'.");
    rolaTela('esttemdeficiencia_s');
    return false;
  } else {
    if(jQuery("[name^='esttemdeficiencia']:enabled:checked").val() == 'TRUE') {
      if(jQuery('#defid').val() == '') {
        alert("O campo 'Deficiência' não pode ser deixado em branco.");
        rolaTela('esttemdeficiencia_s');
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
   
  var estaltashabilidades = parseInt(jQuery("[name^='estaltashabilidades']:enabled:checked").length);
  if (estaltashabilidades == 0) {
    alert("Você deve selecionar uma opção para o campo 'Altas Habilidades/Superdotação'.");
    rolaTela('estaltashabilidades_s');
    return false;
  }
	
  var numestfilhos = parseInt(jQuery("[name^='estfilhos']:enabled:checked").length);
  if (numestfilhos == 0) {
    alert("Você deve selecionar uma opção para o campo 'Tem filhos'.");
    rolaTela('estfilhos_s');
    return false;
  } else if ('TRUE' == jQuery("[name^='estfilhos']:enabled:checked").val()
                && '' == jQuery('#qtdfilhos').val()) {
    alert("O campo 'Quantos filhos' não pode ser deixado em branco.");
    rolaTela('qtdfilhos');
    return false;
  }
	
  var numestbeneficiariooutroprograma = parseInt(jQuery("[name^='estbeneficiariooutroprograma']:enabled:checked").length);
  if (numestbeneficiariooutroprograma == 0) {
    alert("Você deve selecionar uma opção para o campo 'Beneficiários de outros programas'.");
    rolaTela('estbeneficiariooutroprograma_s');
    return false;
  } else {
    if (jQuery("[name^='estbeneficiariooutroprograma']:enabled:checked").val() == 'TRUE') {
      if (jQuery('#pbeid').val() == '') {
        alert("O campo 'Qual programa' não pode ser deixado em branco.");
        rolaTela('pbeid');
        return false;
      }
    }
  }

  var numestcumpremedidassocioeducativas = parseInt(jQuery("[name^='estcumpremedidassocioeducativas']:enabled:checked").length);
  if (0 == numestcumpremedidassocioeducativas) {
    alert("Você deve selecionar uma opção para o campo 'Jovem em cumprimento de medidas socioeducacionais'.");
    rolaTela('estcumpremedidassocioeducativas"_s');
    return false;
  }

 var numestparticipouprojovemcampo = parseInt(jQuery("[name^='estparticipouprojovemcampo']:enabled:checked").length);
  if (0 == numestparticipouprojovemcampo) {
    alert("Você deve selecionar uma opção para o campo 'Já participou do Projovem Urbano'.");
    rolaTela('estparticipouprojovemcampo_s');
    return false;
  }

  var numestocupacao = parseInt(jQuery("[name^='estocupacao']:enabled:checked").length);
  if (numestocupacao == 0) {
    alert("Você deve selecionar uma opção para o campo 'Tem alguma ocupação'.");
    rolaTela('estocupacao_s');
    return false;
  } else {
    if (jQuery("[name^='estocupacao']:enabled:checked").val() == 'TRUE') {
      if (jQuery('#ocuid').val() == '') {
        alert("O campo 'Qual ocupação' não pode ser deixado em branco.");
        rolaTela('ocuid');
        return false;
      }
    }
  }

  var numestegressobrasilalfabetizado = parseInt(jQuery("[name^='estegressobrasilalfabetizado']:enabled:checked").length);
  if (numestegressobrasilalfabetizado == 0) {
    alert("Você deve selecionar uma opção para o campo 'É egresso do Programa Brasil Alfabetizado'.");
    rolaTela('estegressobrasilalfabetizado_s');
    return false;
  }
  var egressooutroprogramaalfabetizac = parseInt(jQuery("[name^='egressooutroprogramaalfabetizac']:enabled:checked").length);
  if (egressooutroprogramaalfabetizac == 0) {
    alert("Você deve selecionar uma opção para o campo 'É egresso de outro programa de alfabetização'.");
    rolaTela('egressooutroprogramaalfabetizac_s');
    return false;
  }
 
  var numegressooutroprogramaalfabetizac = parseInt(jQuery("[name^='egressooutroprogramaalfabetizac']:enabled:checked").length);
  if (numegressooutroprogramaalfabetizac == 0) {
    alert("Você deve selecionar uma opção para o campo 'Já participou do Projovem Urbano'.");
    rolaTela('caeprojovem_s');
    return false;
  }

  var numesthistoricoescolar = parseInt(jQuery("[name^='esthistoricoescolar']:enabled:checked").length);
  if (numesthistoricoescolar == 0) {
    alert("Você deve selecionar uma opção para o campo 'Apresentou histórico escolar'.");
    rolaTela('esthistoricoescolar_s');
    return false;
  }
	
  var numesttesteproficiencia = parseInt(jQuery("[name^='esttesteproficiencia']:enabled:checked").length);
  if (numesttesteproficiencia == 0) {
    alert("Você deve selecionar uma opção para o campo 'Realizou teste de proficiência'.");
    rolaTela('esthistoricoescolar_s');
    return false;
  }

//   -- Validação do certificado de ensino fundamental
//   if ((jQuery('[name="eststatus"]').is(':checked')) && (jQuery('[name="eststatus"]:checked').val() == "A")) {
//     var possuiCertificado = false;
//     jQuery.ajax({
//       type: "POST",
//       url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
//       data: "requisicao=verificaCertificadoTotalBolsa&estnome=" + jQuery('#estnome').val()
//               + "&cpf=" + jQuery('#estcpf').val()
//               + "&estnomemae=" + jQuery('#estnomemae').val()
//               + "&estdatanascimento=" + jQuery('#estdatanascimento').val()
//               + "&estnumrg=" + jQuery('#estnumrg').val(),
//       async: false,
//       success: function(certificadoBolsa) {
//         var cebo = certificadoBolsa.split(";");
//         jQuery('#estauxiliosareceber').val(cebo[1]);
//         jQuery('#div_estauxiliosareceber').html(cebo[1]);
//         if (cebo[0] == "1") {
//           possuiCertificado = true;
//         }
//       }
//     });

//     if (possuiCertificado) {
//       alert('Estudante já certificado no Ensino Fundamental');
//       return false;
//     }
//   }

  // -- Validação de vagas na turma
  if( jQuery('#turid').val() == '' || jQuery('#turid').val() == 'undefined' ){
      alert('Escolha uma turma.');
      return false;
  }
  
//   if ((jQuery('[name="eststatus"]').is(':checked')) && (jQuery('[name="eststatus"]:checked').val() == "A")) {
//     var turmaLotada = false;
//     jQuery.ajax({
//       type: "POST",
//       url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
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
  if ((jQuery('[name="eststatus"]').is(':checked')) && (jQuery('[name="eststatus"]:checked').val() == "I")) {
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
  
//   document.getElementById('estendmuncod').removeAttribute('disabled');
  if(jQuery('#turid').val()==''){
		alert('Escolha uma opção para o campo Turma');
		rolaTela('turid');
  }
  jQuery('[name=turid]').attr('disabled', false);
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
    url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
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
    url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
    data: "requisicao=carregarMunicipios2&estuf=" + estuf,
    async: false,
    success: function(msg){document.getElementById('td_muncod').innerHTML = msg;}
  });
}

function carregarMunicipios(estuf, muncod, bloqueio) {
  var bloq = '';
  if (typeof bloqueio == 'undefined') {
    bloq = "S";
  } else {
    bloq = bloqueio;  
  }
  document.getElementById('td_estendmuncod').innerHTML = 'Carregando...';
  jQuery.ajax({
    type: "POST",
    url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
    data: "requisicao=carregarMunicipios&estuf=" + estuf + "&muncod=" + muncod + "&bloq=" + bloq,
    async: false,
    success: function(msg) {
      document.getElementById('td_estendmuncod').innerHTML = msg;
    }
  });
}

function buscarTurmas(entid,bloq) {
	 if (entid != '') {	
	    document.getElementById('td_turma').innerHTML = 'Carregando...';
	    jQuery.ajax({
	      type: "POST",
	      url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
	      data: "requisicao=buscarTurmas&entid="+entid+"&bloq=<?=$bloq ?>",
	      async: false,
	      success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
	    });
	 }
}

function excluirEstudantes(estid) {
  var conf = confirm('Deserealmente excluir o estudante?');
  if (conf) {
    window.location
            = 'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&requisicao=excluirEstudante&estid=' + estid;
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

function verificaFaltas(estid, eststatus){
  var turid = jQuery('#turid').val();
  var nucid = jQuery('#nucid').val();

  jQuery.ajax({
    type: "POST",
    url: "projovemcampo.php?modulo=principal/monitoramento&acao=A",
    data: "requisicao=verificarFaltasAluno&estid=" + estid
            + "&eststatus=" + eststatus
            + "&turid=" + turid
            + "&nucid=" + nucid,
    async: false,
    success: function(status) {
      if (status == 'inativo') {
        alert('Aluno não poderá ser ativado por exceder 360 horas de faltas.');	
        jQuery('input:radio[name=eststatus]').attr('checked', false);
        //jQuery('#eststatus').val(eststatus);
      }
    }
  });
}
function mascaratel(campo){

	
	var campo = jQuery('#'+campo);
	alert(campo);
	if(campo.value.length>12){
         campo.mask("(999) 999-9999");
    }
}
	    

</script>
<form id="form" name="form" method="POST">
	<input type="hidden" name="requisicao" value="<?=$requisicao ?>"> <input
		type="hidden" name="estid" value="<?=$estid ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<td><? echo campo_texto('estcpf', "S", ($estcpf != '' ? 'N' : $bloq), "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="estcpf" required', '', '', 'carregarEstudante();' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('estnome', "S", ((!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis))?"N":$bloq), "Nome", 67, 70, "", "", '', '', 0, 'id="estnome" required class="obrigatorio normal alphaonly"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Nascimento:</td>
			<td><? echo campo_texto('estdatanascimento', "S", ((!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis))?"N":$bloq), "Data de Nascimento", 12, 10, "##/##/####", "", '', '', 0, 'id="estdatanascimento" required' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Idade:</td>
			<td><?=campo_texto('estidade', 'S', ((!$db->testa_superuser()&&!in_array(PFL_ADMINISTRADOR, $perfis)) ? "N" : $bloq ), 'Idade', 5, 2, '###', '', '', '', 0, 'id="estidade" required disabled'); ?></td>
    <?php if (isset($estdatanascimento) && !empty($estdatanascimento)): ?>
      <script type="text/javascript">
        jQuery(document).ready(function(){
          jQuery('#estidade').val(calculaIdade('<?php echo $estdatanascimento; ?>'));
        });
      </script>
    <?php endif; ?>
  </tr>
		<tr>
			<td class="SubTituloDireita">Sexo:</td>
			<td><input type="radio" name="estsexo" id="estsexo_m" value="M"
				<?=(($estsexo=="M")?"checked":"");?> <?=$disabled;?>> <label
				for="estsexo_m">Masculino</label> <input type="radio" name="estsexo"
				id="estsexo_f" value="F" <?=(($estsexo=="F")?"checked":"");?>
				<?=$disabled;?>> <label for="estsexo_f">Feminino</label></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome da mãe:</td>
			<td><? echo campo_texto('estnomemae', "S", $bloq, "Nome da mãe", 67, 70, "", "", '', '', 0, 'id="estnomemae" class="obrigatorio normal alphaonly"'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome do pai:</td>
			<td><? echo campo_texto('estnomepai', "N", $bloq, "Nome do pai", 67, 70, "", "", '', '', 0, 'id="estnomepai" class="normal alphaonly"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nacionalidade:</td>
			<td>
    <?php
    $naturalidade = array(
        0 => array("codigo" => "B", "descricao" => "Brasileira"),
        1 => array("codigo" => "E", "descricao" => "Estrangeira"));
    $db->monta_combo('estnaturalidade', $naturalidade, $bloq, 'Selecione', '', '', '', '', 'S', 'estnaturalidade', null, null, null, 'required'); 

    if (in_array(PFL_CONSULTA, $perfis)): ?>
      <img width="13px" border="0" style="cursor: pointer;"
				title="Nacionalidade" src="../imagens/AtencaoP.gif"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaNacionalidade');">
    <?php endif; ?>
    </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" rowspan="2">Naturalidade:</td>
			<td>
      <?php
      //$estendestuf = $estuf;
      $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
      $db->monta_combo('estestufnasc', $sql, $bloq, 'Selecione a UF', 'carregarMunicipiosNaturalidade', '', '', '', 'N', 'estestufnasc', null, null, null, 'required');
      if($estestufnasc): ?>
      <script>
      jQuery(document).ready(function() {
          carregarMunicipiosNaturalidade('<?=$estestufnasc; ?>', '<?=$bloq; ?>');
          jQuery('#estmuncodnasc').val('<?=$estmuncodnasc; ?>');
      });
      </script>
      <? endif; ?>
      <?php if (in_array(PFL_CONSULTA, $perfis)): ?>
          <img width="13px" border="0" style="cursor: pointer;"
				title="UF" src="../imagens/AtencaoP.gif"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaUF');">
      <?php endif; ?>
    </td>
		</tr>
		<tr>
			<td id="td_natmuncod"><span style="display: none">Naturalidade</span><span>
      <? $db->monta_combo('estmuncodnasc', array(), $bloq, 'Selecione o Município', '', '', '', '', 'S', 'estmuncodnasc'); ?>
      <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
        <img width="13px" border="0" style="cursor: pointer;"
					title="Município" src="../imagens/AtencaoP.gif"
					onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carrega_Municipio');">
      <?php endif; ?>
      </span></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Cor / Raça:</td>
			<td>
    <?php
    $sql = "SELECT corid as codigo, cordescricao as descricao FROM projovemcampo.corraca WHERE corstatus='A' ORDER BY cordescricao";
    $db->monta_combo('corid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'corid', null, null, null, 'required'); 
    if (in_array(PFL_CONSULTA, $perfis)): ?>
      <img width="13px" border="0" style="cursor: pointer;"
				title="Cor / Raça" src="../imagens/AtencaoP.gif"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaRaca');">
    <?php endif; ?>
    </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Carteira de identidade:</td>
			<td><? echo campo_texto('estnumrg', "S", $bloq, "Carteira de identidade", 17, 15, "", "", '', '', 0, 'id="estnumrg" required' ); ?> 
      <span>Orgão Emissor:</span> <span><? echo campo_texto('estorgaoexpedidorg', "S", $bloq, "Orgão Emissor", 12, 10, "", "", '', '', 0, 'id="estorgaoexpedidorg" required' ); ?></span>
				<span>UF de Emissão:</span> <span><?  $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
          echo $db->monta_combo('estestufemissao', $sql, $bloq, 'Selecione a UF', '', '', '', '', 'S', 'estestufemissao', null, null, null, 'required'); ?></span>
				<span>Data de Emissão:</span> <span><? echo campo_texto('estdataemissaorg', "S", $bloq, "Data de emissão", 12, 10, "##/##/####", "", '', '', 0, 'id="estdataemissaorg" required' ); ?></span></td>
		</tr>
		<tr>
  	<?$estnis = $estnis == 0 ? '' : $estnis;?>
    <td class="SubTituloDireita">Número de Identificação Social (NIS):</td>
			<td><? echo campo_texto('estnis', "N", "N", "NIS / PIS / PASEP", 13, 11, "", "", '', '', 0, 'id="estnis"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Pessoa com deficiência:</td>
			<td><input type="radio" name="esttemdeficiencia" value="TRUE"
				<?=(($esttemdeficiencia == "t")?"checked":"") ?> <?=$disabled ?>
				onclick="pessoaDeficiencia('S');" id="esttemdeficiencia_s" required = "required"><label
				for="esttemdeficiencia_s"> Sim </label> <input type="radio"
				name="esttemdeficiencia"
				<?=(($esttemdeficiencia=="f")?"checked":"") ?> <?=$disabled ?>
				value="FALSE" onclick="pessoaDeficiencia('N');"
				id="esttemdeficiencia_n" required><label for="esttemdeficiencia_n">
					Não </label> <img border="0" title="Indica campo obrigatório."
				src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)){?>
            <img width="13px" border="0"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaDeficiencia')"
				style="cursor: pointer;"
				title="Transtorno global do desenvolvimento"
				src="../imagens/AtencaoP.gif">
        <?php }?>
      </td>
		</tr>
		<tr id="pessoaDeficiencia_1"
			<?=(($esttemdeficiencia=="t")?"":"style=\"display:none;\"") ?>>
			<td class="SubTituloDireita"><b>Deficiência:</b></td>
			<td>
			<? 
			$sql = "SELECT defid as codigo, defdescricao as descricao FROM projovemcampo.deficiencia WHERE defstatus='A' ORDER BY defdescricao";
			$db->monta_combo('defid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'defid');
			?> 
		</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Transtorno global do desenvolvimento:</td>
			<td>
			<? 
			$sql = "SELECT trdid as codigo, trddescricao as descricao FROM projovemcampo.transtornodesenvolvimento WHERE trdstatus='A' AND trdid not in (5) ORDER BY trddescricao";
			$db->monta_combo('trdid', $sql, $bloq, 'Selecione', '', '', '', '', 'N', 'trdid');
			if (in_array(PFL_CONSULTA, $perfis)){?>
                            <img width="13px" border="0"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaTranstorno')"
				style="cursor: pointer;"
				title="Transtorno global do desenvolvimento"
				src="../imagens/AtencaoP.gif">
                        <?php }?>    
                </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Altas Habilidades/Superdotação:</td>
			<td><input type="radio" name="estaltashabilidades"
				<?=(($estaltashabilidades == 't')?'checked':'') ?> <?=$disabled ?>
				value="TRUE" id="estaltashabilidades_s" /><label
				for="estaltashabilidades_s"> Sim </label> <input type="radio"
				name="estaltashabilidades"
				<?=(($estaltashabilidades == 'f')?'checked':'') ?> <?=$disabled ?>
				value="FALSE" id="estaltashabilidades_n" /><label
				for="estaltashabilidades_n"> Não </label> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de recurso de acessibilidade que
				utiliza:</td>
			<td>
        <?php 
        $sql = "SELECT traid as codigo, tradescricao as descricao FROM projovemcampo.recursoacessibilidade WHERE trastatus = 'A'";
        if ($_REQUEST['estid']) {
          $marcados = $db->carregarColuna('SELECT traid FROM projovemcampo.estudanterecursoacessibilidade WHERE
          estid = '.$_REQUEST['estid']);
        }
        $db->monta_checkbox('traid[]', $sql, $marcados, '<br>', $comLabel = true);
        ?>
      </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Estado Civil:</td>
			<td>
        <?php
        $sql = "SELECT escid as codigo, escdescricao as descricao FROM projovemcampo.estadocivil WHERE escstatus='A' ORDER BY escdescricao";
        $db->monta_combo('escid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'escid', null, null, null, 'required');
        if (in_array(PFL_CONSULTA, $perfis)): ?> 
          <img width="13px" border="0" style="cursor: pointer;"
				title="Estado Civil" src="../imagens/AtencaoP.gif"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaEstadoCivil');">
        <?php endif; ?>
      </td>
		</tr>
		<tr>
		
		
		<tr>
			<td class="SubTituloDireita">Tem filhos ?</td>
			<td><input type="radio" name="estfilhos"
				<?=(($estfilhos=="t")?"checked":"") ?> <?=$disabled ?> value="TRUE"
				onclick="temFilhos('S');"> Sim <input type="radio" name="estfilhos"
				<?=(($estfilhos=="f")?"checked":"") ?> <?=$disabled ?> value="FALSE"
				onclick="temFilhos('N');"> Não <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
		</tr>
		<tr id="temFilhos"
			<?=(($estfilhos=="t")?"":"style=\"display:none;\"") ?>>
			<td class="SubTituloDireita">&nbsp;</td>
			<td colspan="2">
			<?
			
			if( $estid ){
				$estqtdfilhos = ",
						(SELECT qtdfilhos::character(7) 
	                	FROM projovemcampo.estudantefaixaetariafilhos e 
	                	WHERE i.fefid=e.fefid AND e.estid='".$estid."'  --AND esfstatus = 'A'
						) as qtdfilhos";
			}
			$sql = "SELECT 
						fefdescricao, 
						fefid
						$estqtdfilhos
                	FROM 
                		projovemcampo.faixaetariafilhos i 
                	WHERE 
                		fefstatus='A' 
                	ORDER BY fefid";
			$linhas = $db->carregar($sql);
			?>
			<table width="100%" cellspacing="0" cellpadding="2" border="0"
					align="center" class="listagem" style="color: 333333;">
					<thead>
						<tr>
							<td valign="top" align="center"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								class="title">Idade</td>
							<td valign="top" align="center"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								class="title">Quantidade de filhos</td>
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
					<tr bgcolor="" onmouseout="this.bgColor='';"
							onmouseover="this.bgColor='#ffffcc';">
							<td valign="top" title="Idade"><?=$linha['fefdescricao'] ?></td>
							<td valign="top" title="Quantidade de filhos"><?=$db->monta_combo("estqtdfilhos[{$linha['fefid']}]", $arr, $bloq, 'Selecione', '', '', '', '', 'S', "estqtdfilhos[{$linha['fefid']}]", null, $linha['qtdfilhos'], null, 'required'); ?></td>
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
			<td><input type="radio" name="estbeneficiariooutroprograma"
				<?=(($estbeneficiariooutroprograma=="t")?"checked":"") ?>
				<?=$disabled ?> value="TRUE" onclick="outrosProgramas('S');"
				id="estbeneficiariooutroprograma_s"><label
				for="estbeneficiariooutroprograma_s"> Sim </label> <input
				type="radio" name="estbeneficiariooutroprograma"
				<?=(($estbeneficiariooutroprograma=="f")?"checked":"") ?>
				<?=$disabled ?> value="FALSE" onclick="outrosProgramas('N');"
				id="estbeneficiariooutroprograma_n"><label
				for="estbeneficiariooutroprograma_n"> Não </label> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)): ?>
          <img width="13px" border="0" style="cursor: pointer;"
				title="Estado Civil" src="../imagens/AtencaoP.gif"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaProgramasBeneficiarios');" />
        <?php endif; ?>
      </td>
		</tr>
		<tr id="outrosProgramas"
			<?=(($estbeneficiariooutroprograma=="t")?"":"style=\"display:none;\"") ?>>
			<td class="SubTituloDireita"><b>Qual programa?</b></td>
			<td>
        <?php
        $sql = "SELECT bprid as codigo, bprdesc as descricao FROM projovemcampo.beneficiarioprograma WHERE bprstatus='A' ORDER BY bprdesc";
        $db->monta_combo('bprid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'bprid');
        ?>
      </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Jovem em cumprimento de medidas
				socioeducacionais:</td>
			<td><input type="radio" name="estcumpremedidassocioeducativas"
				<?=(($estcumpremedidassocioeducativas == "t")?"checked":"") ?>
				<?=$disabled ?> value="TRUE" id="estcumpremedidassocioeducativas_s"><label
				for="estcumpremedidassocioeducativas_s"> Sim </label> <input
				type="radio" name="estcumpremedidassocioeducativas"
				<?=(($estcumpremedidassocioeducativas  == "f")?"checked":"") ?>
				<?=$disabled ?> value="FALSE" id="estcumpremedidassocioeducativas_n"><label
				for="estcumpremedidassocioeducativas_n"> Não </label> <img
				border="0" title="Indica campo obrigatório."
				src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)): ?>
          <img width="13px" border="0" style="cursor: pointer;"
				title="Estado Civil" src="../imagens/AtencaoP.gif"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaProgramasBeneficiarios');" />
        <?php endif; ?>
      </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tem alguma ocupação?</td>
			<td><input type="radio" name="estocupacao"
				<?=(($estocupacao=="t")?"checked":"") ?> <?=$disabled ?>
				value="TRUE" onclick="temOcupacao('S');" id="estocupacao_s"><label
				for="estocupacao_s"> Sim </label> <input type="radio"
				name="estocupacao" <?=(($estocupacao=="f")?"checked":"") ?>
				<?=$disabled ?> value="FALSE" onclick="temOcupacao('N');"
				id="estocupacao_n"><label for="estocupacao_n"> Não </label> <img
				border="0" title="Indica campo obrigatório."
				src="../imagens/obrig.gif">
        <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
        <img width="13px" border="0"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaOcupacao')"
				style="cursor: pointer;" title="Estado Civil"
				src="../imagens/AtencaoP.gif">
        <?php endif; ?>
      </td>
		</tr>
		<tr id="temOcupacao"
			<?=(($estocupacao=="t")?"":"style=\"display:none;\"") ?>>
			<td class="SubTituloDireita"><b>Qual ocupação?</b></td>
			<td>
        <? 
        $sql = "SELECT ocuid as codigo, ocudescricao as descricao FROM projovemcampo.ocupacao WHERE ocustatus='A' ORDER BY ocudescricao";
        $db->monta_combo('ocuid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'ocuid');
        ?>
      </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Segmento Social:</td>
			<td>
			<? 
			$sql = "SELECT sesid as codigo, sesdescricao as descricao FROM projovemcampo.segmentosocial WHERE sesstatus='A' ORDER BY sesdescricao";
			$db->monta_combo('sesid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'sesid', null, null, null, '');
			?> 
                        <?php if (in_array(PFL_CONSULTA, $perfis)){?> 
                            <img width="13px" border="0"
				onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaSegmentoSocial')"
				style="cursor: pointer;" title="Segmento Social"
				src="../imagens/AtencaoP.gif">
                        <?php }?>
                </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">É egresso do Programa Brasil
				Alfabetizado?</td>
			<td><input type="radio" name="estegressobrasilalfabetizado"
				<?=(($estegressobrasilalfabetizado=="t")?"checked":"") ?>
				<?=$disabled ?> value="TRUE" id="estegressobrasilalfabetizado_s"><label
				for="estegressobrasilalfabetizado_s" /> Sim </label> <input
				type="radio" name="estegressobrasilalfabetizado"
				<?=(($estegressobrasilalfabetizado=="f")?"checked":"") ?>
				<?=$disabled ?> value="FALSE" id="estegressobrasilalfabetizado_n"><label
				for="estegressobrasilalfabetizado_n" /> Não </label> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
		</tr>
		<td class="SubTituloDireita">É egresso de outro programa de
			alfabetização?</td>
		<td><input type="radio" name="egressooutroprogramaalfabetizac"
			id="egressooutroprogramaalfabetizac_s" value="TRUE"
			<?=(($egressooutroprogramaalfabetizac=="t")?"checked":"") ?>
			<?=$disabled ?> /> <label for="egressooutroprogramaalfabetizac_s">
				Sim </label> <input type="radio"
			name="egressooutroprogramaalfabetizac"
			id="egressooutroprogramaalfabetizac_n" value="FALSE"
			<?=(($egressooutroprogramaalfabetizac=="f")?"checked":"") ?>
			<?=$disabled ?> /> <label for="egressooutroprogramaalfabetizac_n">
				Não </label> <img border="0" title="Indica campo obrigatório."
			src="../imagens/obrig.gif"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Já participou do Projovem Urbano:</td>
			<td><input type="radio" name="estparticipouprojovemcampo"
				<?=(($estparticipouprojovemcampo=="t")?"checked":"") ?>
				<?=$disabled ?> value="TRUE" id="estparticipouprojovemcampo_s"><label
				for="estparticipouprojovemcampo_s" /> Sim </label> <input
				type="radio" name="estparticipouprojovemcampo"
				<?=(($estparticipouprojovemcampo == "f")?"checked":"") ?>
				<?=$disabled ?> value="FALSE" id="estparticipouprojovemcampo_n"><label
				for="estparticipouprojovemcampo_n" /> Não </label> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
		</tr>
		<tr>
		
		
		<tr>
			<td class="SubTituloDireita">Endereço</td>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
					cellPadding="3" align="center">
					<tr>
						<td class="SubTituloDireita" width="20%">CEP</td>
						<td><? echo campo_texto('estendcep', "S", $bloq, "CEP", 11, 9, "#####-###", "", '', '', 0, 'id="estendcep" required', '', '', 'getEnderecoPeloCEP(this.value);'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Logradouro</td>
						<td>
							<input type="hidden" id="estendlogradouro" name="estendlogradouro" value="<?=$estendlogradouro?>"/>
							<label id="estendlogradouro_label"><?=$estendlogradouro?></label>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Número</td>
						<td><? echo campo_texto('estendnumero', "S", $bloq, "Número", 12, 10, "", "", '', '', 0, 'id="estendnumero" required' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Complemento</td>
						<td><? echo campo_texto('estendcomplemento', "N", $bloq, "Complemento", 67, 150, "", "", '', '', 0, 'id="estendcomplemento"' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Bairro</td>
						<td>
							<input type="hidden" id="estendbairro" name="estendbairro" value="<?=$estendbairro?>"/>
							<label id="estendbairro_label"><?=$estendbairro?></label>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">UF</td>
						<td>
			              	<input type="hidden" id="estendestuf" name="estendestuf" value="<?=$estendestuf?>"/>
							<label id="estendestuf_label"><?=$estendestuf?></label>
              			  <? if($estendestuf): ?>
				              <script>
				              jQuery(document).ready(function() {
				                      carregarMunicipios('<?=$estendestuf ?>','<?=$estendmuncod?>','<?=$bloq?>');
				                      jQuery('#estendmuncod').val('<?=$estendmuncod ?>');
				              });
				              </script>
              			  <? endif; ?>
			              <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
				              <img width="13px" border="0" style="cursor: pointer;"
											title="UF" src="../imagens/AtencaoP.gif"
											onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carregaUF');">
			              <?php endif; ?>
            			</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Município</td>
						<td>
						<input type="hidden" id="estendmuncod" name="estendmuncod" value="<?=$municipio['descricao']?>"/>
						<label id="td_estendmuncod"><?=$municipio['descricao']?></label>
             			 <?php if (in_array(PFL_CONSULTA, $perfis)): ?> 
                  			<img width="13px" border="0" style="cursor: pointer;"
								title="Município" src="../imagens/AtencaoP.gif"
								onmouseover="SuperTitleAjax('projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&reqTittle=carrega_Municipio');">
              			<?php endif; ?>
            			</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Qual Região de Moradia?</td>
						<td><input type="radio" name="estendregiaomoradia"
							<?=(	($estendregiaomoradia == 'r')
                                                                ?'checked'
                                                                :
	                                                                ( ($estid != '') ? 
		                                                                (	($estendregiaomoradia == 'u')
		                                                                    ? 'checked'
		                                                                    : ''
		                                                            	)
	                                                                : 
																		''				                                                                
	                                                                )
															); ?>
							<?=$disabled ?> value="u" id="estendregiaomoradia_u"><label
							for="estendregiaomoradia_u" /> Urbano </label> <input
							type="radio" name="estendregiaomoradia"
							<?=(	($estendregiaomoradia == 'r')
                                                                ?'checked'
                                                                :
	                                                                ( ($estid != '') ? 
		                                                                (	($estendregiaomoradia == 'u')
		                                                                    ? ''
		                                                                    :'checked'
		                                                            	)
	                                                                : 
																		''				                                                                
	                                                                )
															); ?>
							<?=$disabled ?> value="r" id="estendregiaomoradia_r"><label
							for="estendregiaomoradia_r" /> Rural </label> <img border="0"
							title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">E-mail</td>
						<td><? echo campo_texto('estemail', "N", $bloq, "E-mail", 67, 100, "", "", '', '', 0, 'id="estemail"' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Telefone fixo</td>
						<td><? echo campo_texto('esttelefone', "S", $bloq, "Telefone fixo", 14, 14, "", "", '', '', 0, 'id="esttelefone" required','if(this.value.length>12){ this.value=mascaraglobal(\'##-#####-####\',this.value)}else{ this.value=mascaraglobal(\'##-####-####\',this.value)}'); ?> Formato: DD-####-####</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Telefone celular</td>
						<td><? echo campo_texto('estcelular', "N", $bloq, "Telefone celular", 14, 14, "", "", '', '', 0, 'id="estcelular"','if(this.value.length>12){ this.value=mascaraglobal(\'##-#####-####\',this.value)}else{ this.value=mascaraglobal(\'##-####-####\',this.value)}'); ?> Formato: DD-####-####</td>
					</tr>
				</table>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">Apresentou histórico escolar ?</td>
			<td><input type="radio" name="esthistoricoescolar"
				<?=(($esthistoricoescolar == 't')?'checked':'') ?> <?=$disabled ?>
				value="TRUE" id="esthistoricoescolar_s"><label
				for="esthistoricoescolar_s"> Sim </label> <input type="radio"
				name="esthistoricoescolar"
				<?=(($esthistoricoescolar == 'f')?'checked':'') ?> <?=$disabled ?>
				value="FALSE" id="esthistoricoescolar_n"><label
				for="esthistoricoescolar_n"> Não </label> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Realizou teste de proficiência ?</td>
			<td><input type="radio" name="esttesteproficiencia"
				<?=(($esttesteproficiencia=="t")?"checked":"") ?> <?=$disabled ?>
				value="TRUE" id="esttesteproficiencia_s"><label
				for="esttesteproficiencia_s"> Sim </label> <input type="radio"
				name="esttesteproficiencia"
				<?=(($esttesteproficiencia=="f")?"checked":"") ?> <?=$disabled ?>
				value="FALSE" id="esttesteproficiencia_n"><label
				for="esttesteproficiencia_n"> Não </label> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Auxílio financeiro a receber:</td>
      
     <?php
      if($_REQUEST['estid']){
      	 $sql = "SELECT 
      						CASE 
					    		WHEN (estauxiliosareceber - estauxiliosrecebidos) > 0
					    		THEN (estauxiliosareceber - estauxiliosrecebidos)
					    		ELSE '1'
					    		END  as auxilios
					   FROM projovemcampo.estudante WHERE estid='{$_REQUEST['estid']}'"; 
			   $auxilios = $db->pegaUm($sql);
		}	
		?>
<!-- 		<td>campo_texto('estauxiliosareceber', "S", "N", "Quantidade de bolsas", 12, 10, "##########", "", '', '', 0, 'id="estauxiliosareceber"' );</td> -->
			<td><input type="hidden" name="estauxiliosareceber"
				id="estauxiliosareceber"
				value="<?php  if($auxilios==''){ echo '0';}else{$auxilios;} ?>" />
				<div id="div_estauxiliosareceber"><?php  if($auxilios==''){ echo '0';}else{echo $auxilios;} ?></div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Vincular a escola</b></td>
			<td><input type="hidden" name="turid_bkp" value="<?=$turid ?>" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
					cellPadding="3" align="center">
					<tr>
						<td class="SubTituloDireita"><b>Escolas</b></td>
						<td><? 
				if($turid !=''){
					$sql = "SELECT
								entid
							FROM
								projovemcampo.turma
							WHERE
								turid = {$turid}";
					$entid = $db->pegaUm($sql);
				}
            	$escolas = pegarEscolas('');
            	$habilita = $turid?'N':'S';
	            if($escolas[0]['codigo']!=''){

	            	$db->monta_combo('entid', $escolas, $habilita, 'Selecione', 'buscarTurmas', '', '', '', 'S', 'entid');?>
	            <script>
	            	jQuery(document).ready(function() {
		                    buscarTurmas('<?=$entid ?>','S');
		                    jQuery('#turid').val('<?=$turid ?>','<?=$bloq ?>');
		            	});
	            </script>
            	<?}else{
					$db->monta_combo('entid', array(), $bloq, 'Não foram criadas turmas.', 'buscarTurmas', '', '', '', 'S', 'entid');
				}
	            ?>
            </td>
					</tr>
					<tr>
						<td class="SubTituloDireita"><b>Turma</b></td>
						<td><label id="td_turma"><? $db->monta_combo('turid', array(), $bloq, 'Selecione uma Escola', '', '', '', '', 'N', 'turid'); ?><label></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Status:</b></td>
			<td><input type="radio" name="eststatus" value="A"
				<?=(($eststatus == 'A'||!$eststatus)?'checked':'') ?>
				<?php echo $disabledStatus ?>
				onchange="return verificaFaltas('<?php echo $estudante['estid']; ?>', '<?php echo $eststatus; ?>');"
				id="eststatus_a" /> <label for="eststatus_a"> Ativo </label> <input
				type="radio" name="eststatus" value="I"
				<?=(($eststatus == 'I')?'checked':'') ?>
				<?php echo $disabledStatus; ?> id="eststatus_i" /> <label
				for="eststatus_i"> Inativo - outros motivos </label> <input
				type="radio" name="eststatus" value="D"
				<?=(($eststatus=="D")?"checked":"") ?> <?php echo 'disabled' ?>
				id="eststatus_d"> <label for="eststatus_d"> Inativo - desistência
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </label>
        <?php if ($estid): ?>
          <a
				href="javascript: exibir_ocultar_historico('historico_geral');"> <img
					src="/imagens/mais.gif" style="border: 0" /> histórico
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
                      FROM projovemcampo.historicocadastro hc
                        JOIN seguranca.usuario sg ON sg.usucpf = hc.usucpf
                        JOIN projovemcampo.estudante cd ON cd.estid = hc.estid
                      WHERE hc.estid = $estid OR cd.estcpf = '$estcpf_sem_masc'
                      ORDER BY hicdataacao", 
              $estid);
              $db->monta_lista_simples($sql, $cabecalho, 25, 0);
            ?>
            </p>
				</div>
        <?php endif; ?>
      </td>
		</tr>
		<tr id="tr_caejustificativainativacao"
			<?=(($eststatus!="I")?"style=\"display:none\"":"") ?>>
			<td class="SubTituloDireita"><b>Justificativa para inativação:</b></td>
			<td>
        <?php
        $sql = "
				SELECT minid AS codigo, mindesc AS descricao
				  FROM projovemcampo.motivoinativacao
				  ORDER BY mindesc
				";
        $db->monta_combo('minid', $sql, $bloq, 'Selecione o Motivo da Inativação', '', '', '', '', 'S', 'minid');
        ?>
        <span id="descricaomotivoinativacao_div"
				<?php if (-1 != $minid): ?> style="display: none" <?php endif; ?>>
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
				<label style="color: red"><?=$msgMeta ?></label><br>
			<?php }
//            /*
//             */
           $dataAtual      = mktime( date('H'), date('i'), date('s'), date('m'), date('d'), date('Y') );
           $dataBloqueio   = mktime( 23, 59, 00, 28,2, 2014 );
           $bloqueioHorario = (bool) ( $dataAtual > $dataBloqueio );
//          ver($dataAtual,$dataBloqueio,d);  
           if( /*!$bloqueioHorario ||*/ !empty($_REQUEST['estid']) || in_array(PFL_EQUIPE_MEC, $perfis)|| in_array(PFL_SUPER_USUARIO, $perfis)||in_array(PFL_DIRETOR_ESCOLA, $perfis)) {  
                echo '<input type="button" name="salvar" value="Salvar" '. $disabled .' onclick="gravarestudantes();">';
           }
            ?>
				

			<?/* if($estid) : ?>
				<input type="button" name="novo" value="Novo" <?=$disabled ?> onclick="window.location='projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes';">
			<? endif;*/ ?>
		</td>
		</tr>
	</table>
</form>
<form id="form_busca" name="form_busca" method="POST">
	<input type="hidden" name="pesquisar" value="1" /> <input type="hidden"
		name="estuf" value="<?=$_SESSION['projovemcampo']['estuf'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2"><b>Filtros de pesquisa</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Turma</td>
			<td><?buscarTurmas2($bloq);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Matricula:</td>
			<td><? echo campo_texto('matricula', "N", "S", "Matricula", 12, 8, "########", "", '', '', 0, 'id="matricula"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<td><? echo campo_texto('estcpf', "N", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="estcpf"', '', '', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('estnome', "N", "S", "Nome", 50, 255, "", "", '', '', 0, 'id="estnome" class="alphaonly normal"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="button"
				name="pesquisar" value="Pesquisar"
				onclick="document.getElementById('form_busca').submit();"></td>
		</tr>
	</table>
</form>

<?

if($nucleos[0]) {
	foreach($nucleos as $nucleo) {
		$filtro_nucleo[] = $nucleo['codigo'];
	}
}

$where = Array();
if( $_POST['pesquisar'] == '1' ){
	if( $_POST['muncod'] ){
		$where[] = "(ende.muncod = '".$_POST['muncod']."')";
	}
// 	if( $_POST['polid'] ){
// 		$where[] = "cae.polid = ".$_POST['polid'];
// 	}
// 	if( $_POST['nucid'] ){
// 		$where[] = "cae.nucid = ".$_POST['nucid'];
// 	}
	if( $_POST['caeturma'] ){
		$where[] = "est.turid = ".$_POST['turid'];
	}
	if( $_POST['matricula'] ){
		$where[] = "est.estid = {$_POST['matricula']}";
	}
	if( $_POST['estnome'] ){
		$where[] = "est.estnome ilike '%'||UPPER('".$_POST['estnome']."')||'%'";
	}
	if( $_POST['estcpf'] ){
		$where[] = "est.estcpf = '".str_replace(Array('.','-'),'',$_POST['estcpf'])."'";
	}
}
if(!$db->testa_superuser()) {
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_DIRETOR_POLO, $perfis)) {
		$inner_polo = "inner join projovemcampo.usuarioresponsabilidade ur1 on ur1.usucpf='".$_SESSION['usucpf']."' and ur1.polid=cae.polid AND ur1.rpustatus='A'";
	}
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$inner_nucleo = "left join projovemcampo.usuarioresponsabilidade ur2 on ur2.usucpf='".$_SESSION['usucpf']."' and ur2.entid = nes.entid AND ur2.rpustatus='A'";
		$where_nucleo = " AND ( ur2.entid IS NOT NULL OR cae.turid IS NULL )";
	}
	
}

//<img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirEstudantes(\''||cae.estid||'\')\" > 
$sql = "SELECT DISTINCT
			'<img src=../imagens/alterar.gif title=\"Editar\" border=0 style=cursor:pointer; onclick=\"window.location=\'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes&estid='||est.estid||'\';\"> 
			<img src=../imagens/report.gif border=0 width=\"13px\"  title=\"Imprimir Comprovante de Inscrição\" style=cursor:pointer; onclick=\"window.open(\'projovemcampo.php?modulo=principal/popComprovante&acao=A&estid='||est.estid||'\',\'Comprovante\',\'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">
			<img src=../imagens/lista_verde.gif border=0 width=\"13px\"  title=\"Imprimir Ficha de Matricula\" style=cursor:pointer; onclick=\"window.open(\'projovemcampo.php?modulo=principal/popFichaMatricula$anoM&acao=A&estid='||est.estid||'\',\'Comprovante\',\'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">' as acao,
			'<center><img src=../imagens/'||
			CASE WHEN eststatus = 'A'
				THEN '0_ativo.png title=\"Aluno Ativo\"'
				ELSE '0_inativo.png title=\"Aluno Inativo\"'
			END ||' border=0 width=\"13px\" style=cursor:pointer;/></center>' as status,
			2014||lpad(estid::varchar,6,'0') as Matricula,
			estcpf, 
			estnome, 
			estemail,
			ent.entnome, 
			turdescricao 
		FROM 
			projovemcampo.estudante est
		INNER  JOIN projovemcampo.turma          tur ON tur.turid  = est.turid
		INNER  JOIN entidade.entidade 	         ent ON ent.entid  = tur.entid
		INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid  = tur.secaid
		left  JOIN entidade.entidadeendereco     etd ON etd.entid  = ent.entid
		left  JOIN entidade.endereco 	        ende ON ende.endid = etd.endid
		WHERE
			apc.apcid = ".$_SESSION['projovemcampo']['apcid']."
			".(($where)?" AND ".implode(" AND ",$where) :"")."
		ORDER BY estnome";
// ver($sql,d);
$cabecalho = array("","Status","Matrícula","CPF","Nome","E-mail","Escola","Turma");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>

<? 
// registarUltimoAcesso(); 
?>

<?php 
if($_REQUEST['estid']){ ?>
<script>
		jQuery(function(){
			jQuery('[name=turid]').attr('disabled',true);
		});
	</script>
<?php  } 
registarUltimoAcesso();
?>
