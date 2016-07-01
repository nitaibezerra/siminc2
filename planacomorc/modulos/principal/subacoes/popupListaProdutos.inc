<?php
/**
 * Listagem e cadastro de produtos.
 */

$whereFiltroSubacao = '';
if (isset($_POST['requisicao']) && 'salvar' == $_POST['requisicao']) {
    if (isset($_POST['prdid']) && !empty($_POST['prdid'])) { // -- Modificar
        // -- Alterando o produto na base de dados
        $sql = <<<DML
UPDATE planacomorc.produto_spo
  SET codigo = '%s',
      nome = '%s',
      descricao = '%s'
  WHERE id_produto_spo = %d
DML;
        $stmt = sprintf($sql, $_POST['codigo'], $_POST['nome'], $_POST['descricao'], $_POST['prdid']);
    } else { // -- Inserir
        // -- Inserindo o novo produto na base de dados
        $sql = <<<DML
INSERT INTO planacomorc.produto_spo(codigo, nome, descricao, st_ativo)
  VALUES ('%s', '%s', '%s', 'A');
DML;
        $stmt = sprintf($sql, $_POST['codigo'], $_POST['nome'], $_POST['descricao']);
    }

    $db->executar($stmt);
    $msg = '';
    if ($db->commit()) {
        $msg = 'Sua requisição foi executada com sucesso.';
    } else {
        $msg = 'Não foi possível executar sua requisição.';
    }
    ?>
<script type="text/javascript" language="javascript">
    alert('<?php echo $msg; ?>');
    window.location.href = '';
</script>
    <?php
    exit();
} elseif (isset($_POST['buscalivre']) && (!empty($_POST['buscalivre']))) {
    // -- Filtrando os produtos pelo campo de texto livre
    $whereFiltroSubacao = <<<DML
AND (psp.codigo ILIKE ('%%%s%%') OR psp.nome ILIKE('%%%s%%') OR psp.descricao ILIKE('%%%s%%'))
DML;
    $whereFiltroSubacao = sprintf(
            $whereFiltroSubacao,
            $_POST['buscalivre'],
            $_POST['buscalivre'],
            $_POST['buscalivre']
    );
}
?>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="js/planacomorc.js"></script>
    <script type="text/javascript" src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
    <script type="text/javascript" src="/pnbe/js/jquery.easing.min.js"></script>
    <script type="text/javascript" src="/pnbe/jquery-ui-1.9.2.custom/js/jquery-ui-1.9.2.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.9/themes/base/jquery-ui.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<div id="modalNovoProduto" style="display:none;">
    <form method="post" name="formproduto" id="formproduto">
        <input type="hidden" name="requisicao" value="salvar" />
        <input type="hidden" name="prdid" id="prdid" value="" />
	    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubTituloDireita">Código:</td>
                <td>
                    <?php echo campo_texto('codigo', "S", "S", "Código", 12, 10, "", "", '', '', 0, 'id="codigo"'); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Nome:</td>
                <td>
                    <?php echo campo_texto('nome', "S", "S", "Nome", 62, 100, "", "", '', '', 0, 'id="nome"'); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" valign="top">Descrição:</td>
                <td>
                    <?php echo campo_textarea('descricao', 'S', 'S', '', '70', '4', '500', '', '', '', '', ''); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" name="produto" value="Salvar" onclick="inserirProduto();">
                    <input type="button" name="fechar" value="Fechar" onclick="$('#modalNovoProduto').dialog('close');">
                </td>
            </tr>
        </table>
    </form>
</div>
<?php monta_titulo('Produtos de Subação', 'Selecione os produtos para associação'); ?>
<form action="" method="post" name="formulario">
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloDireita" align="right">Buscar:</td>
        <td>
            <?php echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, '' ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top;width:25%">&nbsp;</td>
        <td>
            <input type="submit" name="botao" value="Pesquisar" id="pesquisar" />
            <input type="button" name="botao" value="Todos"
                   onclick="window.location='/planacomorc/planacomorc.php?modulo=principal/subacoes/popupListaProdutos&acao=A';"/>
            <input type="button" name="novo" value="Novo" id="novo_produto" />
            <input type="button" name="close" value="Fechar" onclick="window.close();">
        </td>
    </tr>
</table>
</form>
<br>
<?php
$sql = <<<DML
SELECT '<input type="checkbox" '
           || 'id="chk_' || psp.id_produto_spo || '" onclick="selecionar(this, ' || psp.id_produto_spo || ', ''' || psp.codigo || ''');" />' AS checkbox,
       '<input type="image" class="editar_produto" src="/imagens/alterar.gif" value="' || psp.id_produto_spo || '" />' AS edit,
       psp.codigo,
       psp.nome,
       psp.descricao
  FROM planacomorc.produto_spo psp
  WHERE psp.st_ativo = 'A' {$whereFiltroSubacao}
  ORDER BY psp.codigo
DML;
$cabecalho = array ('', '', 'Subação', 'Nome', 'Descrição');
$db->monta_lista($sql, $cabecalho, 60, 20, '', '', '');
?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PRODUTO */
function Trim(str)
{
    return str.replace(/^\s+|\s+$/g, '');
}

var tabelaorigem = window.opener.document.getElementById('produtos');
var idck;
for (i = 1; i < tabelaorigem.rows.length - 1; i++) {
	idck = tabelaorigem.rows[i].id.split("_");
	if (document.getElementById("chk_" + idck[1])) {
		document.getElementById("chk_" + idck[1]).checked = true;
	}
}
/* FIM CARREGANDO OS DADOS DE PRODUTO */

function selecionar(check, prdid, prdcod)
{
	if (!prdid) {
		alert('Nenhum produto foi selecionado.');
		return false;
	}

	if (check.checked) {

        var linhaTbl = check.parentNode.parentNode;
        var tabelaorigem = window.opener.document.getElementById('produtos');
        if (eval(tabelaorigem.rows.length % 2)) {
            var cor = "";
        } else {
    		var cor = "#dcdcdc";
    	}

        // -- Definindo o ID
        var linha = tabelaorigem.insertRow(1);
        linha.id = "prdid_" + prdid;
        linha.style.backgroundColor = cor;
        linha.style.height = '30px';

        // -- Carregando os dados do produto
        var celula1 = tabelaorigem.rows[1].insertCell(0);
        celula1.style.textAlign = "center";
        celula1.innerHTML = prdcod + '<input type="hidden" name="prdid[' + prdid + ']" value="' + prdid + '" />';

        var celula2 = tabelaorigem.rows[1].insertCell(1);
        celula2.style.textAlign = "left";
        celula2.innerHTML = linhaTbl.cells[2].innerHTML;

        var celula3 = tabelaorigem.rows[1].insertCell(2);
        celula3.style.textAlign = "left";
        celula3.innerHTML = linhaTbl.cells[3].innerHTML;

        var celula4 = tabelaorigem.rows[1].insertCell(3);
        celula4.style.textAlign = "center";
        celula4.innerHTML = '<input type="text" name="meta_fisica[' + prdid + ']" value="" class="normal" size="28" '
                                 + 'style="text-align:right;width:25ex" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" '
                                 + 'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
                                 + 'onkeyup="this.value = mascaraglobal(\'###.###.###.###\', this.value);" ' + '/>';
	} else {
        var tabelaorigem = window.opener.document.getElementById('produtos');
        tabelaorigem.deleteRow(window.opener.document.getElementById('prdid_'+prdid).rowIndex);
	}
}

function abrirPopupProduto(popupTitle)
{
    $('#modalNovoProduto').dialog({
        draggable:true,
        resizable:true,
        width:600,
        height:300,
        position:'top',
        modal:true,
        title:popupTitle,
        open:function(){
            $('#formproduto')[0].reset();
        }
    });
}

function inserirProduto()
{
	if ('' == jQuery('#codigo').val()) {
		alert('O campo "Código" não pode ser deixado em branco.');
		return false;
	}
	if ('' == jQuery('#nome').val()) {
		alert('O campo "Nome" não pode ser deixado em branco.');
		return false;
	}
	if ('' == jQuery('#descricao').val()) {
		alert('O campo "Descrição" não pode ser deixado em branco.');
		return false;
	}
    $('#formproduto').submit();
}

function editarProduto(prdid)
{
    $('#prdid').attr('value', prdid);
}


$(document).ready(function(){
    $('#novo_produto').click(function(){
        abrirPopupProduto('Adicionar novo produto');
    });
    $('.editar_produto').click(function(){
        var prdid = $(this).val();
        var prdcod = $(this).parent().next('td').text();
        var prdnom = $(this).parent().next('td').next('td').text();
        var prddsc = $(this).parent().next('td').next('td').next('td').text();

        abrirPopupProduto('Editar produto');
        $('#prdid').val(prdid);
        $('#codigo').val(prdcod);
        $('#nome').val(prdnom);
        $('#descricao').val(prddsc);
    });
});
</script>
<?php exit();?>
