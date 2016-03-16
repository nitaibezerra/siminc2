<?php
/**
 * Listagem e cadastro de produtos.
 */
include_once APPRAIZ .'includes/funcoesspo.php';
require APPRAIZ . 'includes/library/simec/Listagem.php';
?>


<link href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css" />
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<script src='../includes/funcoes.js'></script>
<style>
    .marcado {background-color: #C1FFC1 !important;}
    .remover {display: none;}
</style>
<script>
	$(document).ready(function()
	{
		$('.switch-mini').on('switch-change', function(e, d){
	          d.value;
	          var id = $('input[type=checkbox]', e.target).attr('id');
	          var onclick = $('#' + id).attr('onclick');
	          onclick = onclick.replace('this', '$(\'#\' + id)[0]');
	          eval(onclick);
	       });
		pesquisaRapida('busca');
		apresentaLinhasSelecionadas($('#produtos'),1);

	});


	function pesquisaRapida(id) {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#" + id).keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#" + id).val();
            if (stringPesquisa) {
                $('#listagem table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('#listagem table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    }

    function resultado(id, selecionado, adicionais) {
        var produto = $('tr#' + id + ' td:nth-child(2)').text();

        if (!produto) {
        	alert('Nenhum produto foi selecionado.');
            return false;
        }

        if (selecionado == false) {
            var tabelaorigem = document.getElementById('produtos');
            var colunaProduto = document.getElementById('produto_' + produto);
            if (colunaProduto != null) {
                tabelaorigem.deleteRow(document.getElementById('produto_' + produto).rowIndex);
            }

            // -- Criando a nova linha na tabela da página de origem
            var linha = tabelaorigem.insertRow(1);
            linha.id = "produto_" + id;

            linha.style.height = '30px';

            // -- O número da primeira linha da tabela onde serão inseridos os novos dados
            var linha = 1;

            // setando o produto
            var celCodigo = tabelaorigem.rows[linha].insertCell(0);
            celCodigo.style.textAlign = "center";
            celCodigo.innerHTML = produto + '<input type="hidden" name="prdid[' + id + ']" value="' + id + '" />';

            var celProduto = tabelaorigem.rows[linha].insertCell(1);
            var produto = $('tr#' + id + ' td:nth-child(3)').text();
            celProduto.style.textAlign = "center";
            celProduto.innerHTML = produto;

            var celDescricao = tabelaorigem.rows[linha].insertCell(2);
            celDescricao.style.textAlign = "justify";
            var descricao = $('tr#' + id + ' td:nth-child(4)').text();
            celDescricao.innerHTML = descricao;


            var celMetaFisica = tabelaorigem.rows[linha].insertCell(3);
            celMetaFisica.style.textAlign = "center";

            celMetaFisica.innerHTML = '<input type="text" name="meta_fisica[' + id + ']" value="" class="form-control" size="28" '
            	+ 'style="text-align:right;width:25ex" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" '
            	+ 'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
            	+ 'onkeyup="this.value = mascaraglobal(\'###.###.###.###\', this.value);" ' + '/>';


        } else {
            var tabelaorigem = document.getElementById('produtos');
            tabelaorigem.deleteRow(document.getElementById('produto_' + id).rowIndex);

        }

    }


    function apresentaLinhasSelecionadas(tabelaOrigem,apartirDe){
		var val;
		for (i = apartirDe; i < tabelaOrigem.find('tr').length; i++) {
			val = tabelaOrigem.find('tr')[i].id.split("_");
			if (document.getElementById(val[1])) {
				campo = $('tr#' + val[1] + ' td:nth-child(1)').find('span');
				campo.attr('class','glyphicon glyphicon-remove');
				campo.attr('title','Remover item');
				campo.css('color','gray');
			}
		}
    }

</script>

<?php

?>

<section class="well">
	<form class="form-horizontal" action="" method="post" name="formulario">
		<section class="form-group">
			<label class="control-label col-md-2" for="busca">Buscar:</label>
			<section class="col-md-10">
				<?php inputTexto('buscalivre', $valor, 'busca', 150,false);?>
			</section>
		</section>
	</form>
</section>
<br>
<section id="listagem">
<?php
$sql = "
	SELECT
		--'<input type=\"checkbox\" '
        --   || 'id=\"chk_' || psp.id_produto_spo || '\" onclick=\"selecionar(this, ' || psp.id_produto_spo || ', ''' || psp.codigo || ''');\" />' AS checkbox,
       	--'<input type=\"image\" class=\"editar_produto\" src=\"/imagens/alterar.gif\" value=\"' || psp.id_produto_spo || '\" />' AS edit,
       	psp.id_produto_spo,

       	psp.codigo AS codigo,
       	psp.nome,
       	psp.descricao
  	FROM planacomorc.produto_spo psp
  	WHERE psp.st_ativo = 'A'
  	ORDER BY psp.codigo
";
$cabecalho = array ('Subação', 'Nome', 'Descrição');
$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
$listagem->setQuery($sql);
$listagem->setIdLinha();
$listagem->setCabecalho($cabecalho);
//$listagem->addAcao('edit', 'editarProduto');
$listagem->addAcao('select', array('func' => 'resultado', 'extra-params' => array('codigo')));
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
//$db->monta_lista($sql, $cabecalho, 60, 20, '', '', '');
?>
</section>
<?php
exit();
?>
