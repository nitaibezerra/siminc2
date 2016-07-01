<?php
//require_once APPRAIZ . 'includes/classes/consultaReceitaFederal.class.inc';

// -- Processando a requisição feita pelo usuário
if ($_POST['requisicao']) {
  switch ($_POST['requisicao']) { // -- Requisições válidas
    case 'pesquisarDirigente': break;
    case 'cadastrarDirigente':
    case 'atualizarDirigente':
    case 'excluirDirigente':
      $flagSucesso = $_POST['requisicao']();
    default:
      if (isset($flagSucesso)) {
        if (-1 === $flagSucesso) { $msg = 'Já existe um dirigente ativo cadastrado para esta Instituição de Ensino.\nPor favor, exclua o dirigente atual e tente novamente.';
        } elseif ($flagSucesso) { $msg = 'Sua requisição foi realizada com sucesso.';
        } else { $msg = 'Não foi possível completar sua requisição.'; }
        echo("<script>alert('{$msg}');</script>");
      }
      echo ("<script>window.location='snf.php?modulo=principal/cadastrarDirigente&acao=A'</script>");
      exit;
  }
}
/**
 * Cabeçalho do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ.'includes/cabecalho.inc';
?>
<br />
<?php monta_titulo('Cadastro De Dirigentes', ''); ?>
<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
function salvarDirigente() {
  if ($('#dirid').attr('value')) { // -- Atualização
    var msg = 'Tem certeza que quer alterar o dirigente selecionado?';
    if (validarCPF($('#dircpf').attr('value')) && $('#dirnome').attr('value') && confirm(msg)) {
      $('#CadastrarDirigente').submit();
    } else {
      return alert('Preencha o Nome e CPF para atualizar o dirigente selecionado.');
    }
  } else { // -- Inserção
    var msg = 'Tem certeza que quer incluir o novo dirigente?';
    if (!validarCPF($('#dircpf').attr('value'))) {
      return alert('O CPF informado é inválido.');
    }
    if ($('#dircpf').attr('value') && $('#dirnome').attr('value') && $('#insid').attr('value') && confirm(msg)) {
      $('#requisicao').attr('value', 'cadastrarDirigente');
      $('#CadastrarDirigente').submit();
    } else {
      return alert('Preencha todos os campos para incluir um novo dirigente.');
    }
  }
}
function excluirDirigente(input, dirid) {
  var msg = "Tem certeza que quer remover o dirigente '"
          +$(input).parent().next().text() + "'?";
  if ((dirid) && (confirm(msg))) {
    $("#dirid").attr('value', dirid);
    $('#requisicao').attr('value', 'excluirDirigente');
    $('#CadastrarDirigente').submit();
  }
}
function alterarDirigente(input, dirid, insid) {
  // -- Alterando o botão incluir
  $('#btnIncluir').attr('value', 'Salvar');
  // -- Preenchendo o formulário com os dados do dirigente
  $('#dirid').attr('value', dirid);
  $('#insid').attr('value', insid).attr('disabled', 'disabled');
  $('#dirnome').attr('value', $(input).parent().next().text());
  var cpf = $(input).parent().next().next().text();
  // -- Inserindo o CPF formatado
  $('#dircpf').attr('value',
      cpf.substr(0, 3)+'.'+cpf.substr(3, 3)+'.'+cpf.substr(6, 3)+'-'+cpf.substr(9, 2));
  // -- Rolando a página para o formulário
  $('html, body').animate({scrollTop: $("#insid").offset().top - 100}, 500);
  // -- Action invocada
  $('#requisicao').attr('value', 'atualizarDirigente');
}
function pesquisarDirigente() {
  $('#requisicao').attr('value', '');
  $('#dirid').attr('value', '');
  // -- Ao menos um campo deve ser informado na pesquisa
  if (!$('#insid').attr('value') && !$('#dircpf').attr('value') && !$('#dirnome').attr('value')) {
    alert('Preencha um dos campos para realizar a pesquisa.');
    return;
  }
  $('#CadastrarDirigente').submit();
}
function limparFormulario() {
  $('#btnIncluir').attr('value', 'Incluir');
  $('#requisicao').attr('value', '');
  $('#dirid').attr('value', '');
}
function onChangeCPF(e){
  if (!validarCPF($(e.target).attr('value'), true)) {
    alert('O CPF informado é inválido.');
  }
}

function validarCPF(cpf, preencheNome) {
  var ehValido = false;
  $.ajax({
    type:'POST', url:'/includes/webservice/cpf.php',
    data:{ajaxCPF:cpf},
    success:function(data) {
      if ('' != data) {
        ehValido = true;
        if (preencheNome) {
          $('#dirnome').attr('value', data.substr(0, data.indexOf('|')).split('#')[1]);
        }
      }
    },
    async:false
  });
  return ehValido;
}
function selecionaInstituicao() {}
</script>
<?php
$sql = <<<QUERY
SELECT insid AS codigo,
       COALESCE(insnome || ' - ' || inssigla, insnome) AS descricao
  FROM snf.instituicaoensino
  WHERE insstatus = 'A'
  ORDER BY descricao
QUERY;
$htmlComboIES = $db->monta_combo('insid', $sql, 'S', 'Selecione uma Instituição...',
        "selecionaInstituicao", '', '', '', 'N', 'insid', true);
?>
<form name="formulario" id="CadastrarDirigente" method="post">
  <input type="hidden" name="requisicao" id="requisicao" />
  <input type="hidden" name="dirid" id="dirid" />
  <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
    <tr>
      <td colspan="2" class="SubtituloTabela bold">Dados do Dirigente</td>
    </tr>
    <tr>
      <td width="220" class="subtituloDireita">Instituição de Ensino</td>
      <td><?php echo $htmlComboIES; ?></td>
    </tr>
    <tr>
      <td width="220" class="subtituloDireita">CPF do Dirigente</td>
      <td>
        <?php echo campo_texto('dircpf', 'N', 'S', '', 40, 14, '###.###.###-##', '', '', '', '', 'id="dircpf"', '', $dircpf); ?>
      </td>
    </tr>
    <tr>
      <td width="220" class="subtituloDireita">Nome do Dirigente</td>
      <td>
        <?php echo campo_texto('dirnome', 'N', 'S', '', 60, 100, '', '', '', '', '', 'id="dirnome"', '', $dirnome); ?>
      </td>
    </tr>
    <tr>
      <td class="SubtituloTabela center" colspan="2"  >
        <input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarDirigente();" />
        <input type="reset" value="Limpar" onclick="limparFormulario();" />
        <input type="button" name="btn_incluir" id="btnIncluir" value="Incluir" onclick="salvarDirigente();" />
      </td>
    </tr>
  </table>
</form>
<?php
// -- Faz a listagem de dirigentes cadastrados. @see www/snf/_funcoes.php
listarDirigentes();

// -- Mensagem da requisição
if (isset($msg)): ?>
<script type="javascript/text">alert('<?php echo $msg; ?>');</script>
<?php endif; ?>
<script>
$(document).ready(function(){
  $('#dircpf').change(onChangeCPF);
});
</script>
