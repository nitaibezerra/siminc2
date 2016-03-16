<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(function(){
		$('.alterar').click(function(){
			id = this.id.split('_');
			document.location = 'elabrev.php?modulo=sistema/tabapoio/termoCooperacao/formRepresentanteLegal&acao=A&cpf='+id[1]+'&ug='+id[0];
		});
		$('.pesquisar').click(function(){
			if($('[name=ungcod]').val() == '' && $('[name=ungdsc]').val() == '' && $('[name=cpf]').val() == '' && $('[name=nome]').val() == ''){
				alert('Preencha pelo menos um dos campos da pesquisa!');
				return false;
			}
			$('#formulario').submit();
		});
		$('.limpar').click(function(){			
			window.location = 'elabrev.php?modulo=sistema/tabapoio/termoCooperacao/listaRepresentanteLegal&acao=A';
		});
	});
</script>
<?php
 
require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
monta_titulo( "Lista de Representantes Legais", 'Tabela de Apoio' );

if($_POST) extract($_POST); 
?>
<form id="formulario" name="formulario" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita">Código da UG</td>
			<td><?php echo campo_texto('ungcod', 'N', 'S', '', 15, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome da UG</td>
			<td><?php echo campo_texto('ungdsc', 'N', 'S', '', 45, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">CPF do Responsável</td>
			<td><?php echo campo_texto('cpf', 'N', 'S', '', 25, '', '###.###.###-##', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome do Responsável</td>
			<td><?php echo campo_texto('nome', 'N', 'S', '', 45, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td>
				<input type="button" value="Pesquisar" class="pesquisar"/>
				<input type="button" value="Limpar" class="limpar"/>
			</td>
		</tr>
	</table>
</form>
<?php 

if($_POST) extract($_POST);

if($ungcod)
	$where[] = "ung.ungcod = '{$ungcod}'";

if($ungdsc)
	$where[] = "(ung.ungdsc ILIKE '%{$ungdsc}%' OR removeacento(ung.ungdsc) ILIKE removeacento('%{$ungdsc}%'))";

if($cpf)
	$where[] = "rpl.cpf = '".str_replace(array('.','-'), '',$cpf)."'";

if($nome)
	$where[] = "(rpl.nome ILIKE '%{$nome}%' OR removeacento(rpl.nome) ILIKE removeacento('%{$nome}%'))";

$sql = "
SELECT 
	'
	<img src=\"../imagens/alterar.gif\" border=\"0\" id=\"' || rpl.ug || '_' || rpl.cpf || '\" class=\"alterar\" style=\"cursor:pointer\" />
	' as acao,
	ung.ungcod,
	ung.ungdsc,
	rpl.cpf,	
	rpl.nome,
	rpl.email
FROM elabrev.representantelegal rpl
JOIN public.unidadegestora ung ON ung.ungcod = rpl.ug
".($where ? " WHERE ".implode(" AND ", $where) : "")."
ORDER BY ung.ungdsc
";

$arCabecalho = array('Ação','Código da UG','Unidade Gestora','CPF do Representante Legal','Nome do Representante Legal','E-mail do Representante Legal');
$db->monta_lista($sql, $arCabecalho, 50, 10, 'N', '', '', '', '');
?>