jQuery.noConflict();


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function excluirTurmaEscola(endereco) {
	var conf = confirm('Deseja realmente excluir esta turma? A página de profissionais será alterada!');
	if(conf) {
		window.location=endereco;
	}
}

/**
 * Calcula quantos anos se passaram de uma data até hoje.
 * @param {string} nascimento Data de nascimento no formato: "dd/mm/yyyy".
 * @return {int} Idade calculada com base em "nascimento".
 */
function calculaIdade(nascimento) {
	
  var hoje = new Date();
  
  var ano_atual = hoje.getFullYear();
  var mes_atual = hoje.getMonth();
  var dia_atual = hoje.getDay();

  var ano_nasc = parseInt(nascimento.substr(-4));
  var mes_nasc = parseInt(nascimento.substr(-7, 2)) - 1;
  var dia_nasc = parseInt(nascimento.substr(0, 2));

  var idade = ano_atual - ano_nasc;
  var idade_mes = mes_atual - mes_nasc;
  var idade_dia = dia_atual - dia_nasc;
 
  if ((idade_mes < 0) || (idade_mes == 0 && idade_dia < 0)) {
    idade = parseInt(idade) - 1;
    if (idade < 0) { idade = 0; }
  }
  
  return idade;
}

//if ('undefined' == typeof('getEnderecoPeloCEP')) {
    function getEnderecoPeloCEP(cep) {
        jQuery.ajax({
            type: "POST",
            url: "/geral/consultadadosentidade.php",
            data: "requisicao=pegarenderecoPorCEP&endcep=" + cep,
            async: false,
            success: function(dados) {
                if ('undefined' == typeof(processaRetornoCEP)) {
                    alert("Implemente 'processaRetornoCEP' para tratar o retorno da consulta do CEP.");
                    return false;
                }
                processaRetornoCEP(dados);
            }
        });
    }  
//}

/**
 * Rola a tela para poder visualizar o campo indicado em referencia.
 * @param {string} referencia Elemento utilizado como referencia para a rolagem da tela.
 */
function rolaTela(referencia) {
  jQuery('html, body').animate({scrollTop: jQuery('#'+referencia)
    .offset().top - 100}, 500);
}