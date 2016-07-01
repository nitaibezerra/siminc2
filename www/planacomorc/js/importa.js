/**
 * Exibe as opções da função selecionada
 * @param {string} tipo ID da div de opções que deve ser exibida.
 * @returns {undefined}
 */
function mostraCombo(tipo){
  // -- Escondendo as opções de filtro e/ou chamadas
  $('.chamadaWs').each(function() {
    $(this).next().next().hide();
  });
  // -- Esconde todos os filtros
  $('.filtroWs').hide();
  // -- Exibindo opções da função selecionada
  $('#'+tipo).show();
  // -- Exibindo os filtros relacionados à função escolhida
  $('.'+tipo).show();
}

/**
 *  Função mostra combo para o componente de comboPopup().
 *  @param obj Radio button (de função) que recebeu o click.
 */
function mostraCombo2(obj)
{
    $(obj).closest('tr').next().toggle()
                               .children('.SubTituloDireita').click();
}

/**
 * Rola a tela para poder visualizar o campo indicado em referencia.
 * @param {string} referencia Elemento utilizado como referencia para a rolagem da tela.
 */
function rolaTela(referencia) {
  $('html, body').animate({scrollTop: $('#'+referencia)
    .offset().top - 100}, 500);
}

/**
 * Faz a validação básica do formulário e executa a validação de cada página, se
 * estiver tudo ok, exibe a popup de autenticação do WS.
 * @returns {Boolean}
 */
function solicitarExecucao() {
  var docSelecionado = false;
  // -- Verificando se ao menos uma chamada ao webService foi solicitada.
  $('.chamadaWs').each(function(){
    if (this.checked) {
      docSelecionado = true;
      return;
    }
  });

  if (!docSelecionado) { return alert('Selecione uma função.'); }

  // -- Validação particular da chamada para cada pagina. A função "validacaoAdicional"
  // -- deve ser implementada na página que precisa de validações adicionais.
  if ((typeof(validacaoAdicional) === 'function') && (true !== validacaoAdicional())) {
    return false;
  }

  // -- Exibindo div de autenticação no WS
  $('#div_auth').show();

  // -- Rolando a página para o formulário de autenticação
  rolaTela('formulario');
}

/**
 * Verfica os campos obrigatórios do ws (user, pass, momento) e submete o formulário.
 * @returns {unresolved}
 */
function enviaSolicitacao() {
  if (!$('#wsusuario').attr('value')) { return alert('Favor informar o usuário!'); }
  if (!$('#wssenha').attr('value')) { return alert('Favor informar a senha!'); }
  if (('function' != typeof(ignorarMomento)) && $('#codigomomento')[0]
          && !$('#codigomomento').attr('value')) {
    return alert('Favor informar o Código do Momento!');
  }
  selectAll();
  $('#requisicao').val('enviasolicitacao');
  $('#formulario').submit();
}

/**
 * Exibe os filtros relacionados ao id selecionado.
 * @param {string} opcaoID 
 * @returns {undefined}
 */
function mostraFiltros(opcaoID) {
  // -- Esconde todos os filtros
  $('.filtroWs').hide();
  // -- Exibindo opções de filtro conforme id do elem. selecionado
  // -- os filtros associados ao elemento tem em sua lista de classes o id
  // -- deste elemento
  $('.' + opcaoID).show();
}

/**
 * Marca e desmarca um conjunto de checkboxes de acordo com o id de checkbox.
 * @param {htmlObject} input
 * @param {id} tipo
 * @returns {undefined}
 */
function marcarTodos(input, tipo) {
  if ($(input).attr('checked')) {
    $('.check_'+tipo).attr('checked', true);
  } else {
    $('.check_'+tipo).attr('checked', false);
  }
}

/**
 * Seleciona todos os itens conforme o radio de função selecionado.
 */
function selectAll()
{
    var documento = $('input[type=radio]:checked').attr('value');
    $('#acaid_'+documento+' option').attr('selected', 'selected');
}