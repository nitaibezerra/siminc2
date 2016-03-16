<?php

if( $_POST['req'] == buscar ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	global $db;

	$sql = "SELECT DISTINCT
				'<img onclick=\"return alterarCategoria('|| cat.catid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
					<img onclick=\"return excluirCategoria('|| cat.catid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' 
						as acoes, 
				cat.cordsc as texto,
				cat.catid as catid 
			FROM cte.categoria cat
			INNER JOIN cte.fasescategoria fcat ON fcat.catid = cat.catid
			WHERE cordsc ILIKE '%".utf8_decode($_REQUEST['categoria'])."%' AND
				  facdsc ILIKE '%".utf8_decode($_REQUEST['fase'])."%'";
	
	$dados = $db->carregar($sql);
	
	$i = 0;
	if($dados){
		foreach( $dados as $cat ){
			
			$sqlF = "SELECT COALESCE(facordem::CHARACTER,'') || 'ª Fase: '|| facdsc as dsc
					FROM cte.fasescategoria
					WHERE catid = ".$cat['catid'];
			
			$fases = $db->carregarColuna($sqlF);
			
			$dados[$i]['fases'] = implode("; <br>",$fases);
			unset($dados[$i]['catid']);
			
			$i++;
		}
	}
	
	if(!$dados){
		$dados = $sql;
	}
	
	echo "
		<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\" widht='100%'>
			<tr>
				<td align=\"center\" gbcolor=\"#dcdcdc\" colspan=\"2\" bgcolor=\"#dcdcdc\" >
					<label class=\"SubTitulo\" style=\"color: rgb(0, 0, 0);\">Lista de Categorias</label>
				</td>
			</tr>
			<tr>
				<td><div widht='100%'>";
	//dbg($sql);
		$db->monta_lista_simples( $dados, array('Ações','Categorias','Fases'), 50, 1000, 'N','100%','','categorias',array('5%','70%','20%'),array('center','left') );
	
	echo "</div></td></tr></table>";
	
	die();	
}

if( $_POST['req'] == 'excluir' ){
	
	global $db;
	
	$sql = "DELETE FROM cte.categoriasubacao
			WHERE catid = ".$_POST['catid'].";
			DELETE FROM cte.fasescategoria
			WHERE catid = ".$_POST['catid'].";
			DELETE FROM cte.categoria
			WHERE catid = ".$_POST['catid'];
	
	$db->executar($sql);
	$db->commit();
	die();
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$res = array( 0 =>
					array ("descricao" => "Pesquisa de Categorias",
							    "id"        => "1",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_categorias&acao=A"
							   )
			);

if( $db->testa_superuser() ){	
		array_push($res,
					array ("descricao" => "Cadastrar Categorias",
							    "id"        => "2",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A"
							   ),
		   			array ("descricao" => "Vincular Categorias nas Subações",
							    "id"		=> "3",
							    "link"		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_categorias&acao=A"
					  		   )
			  );
	}
	
echo montarAbasArray($res, "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_categorias&acao=A");
//cte_montaTitulo( 'Pesquisa de categorias', '&nbsp;' );

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" >
	<tr>
		<td width="100%" align="center" gbcolor="#dcdcdc" colspan="2" bgcolor="#dcdcdc" >
			<label class="TituloTela" style="color: rgb(0, 0, 0);">Pesquisa de categorias</label>
		</td>
	</tr>
	<tr>
		<td style="" align="center" bgcolor="#e9e9e9" colspan="2">
			&nbsp;
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
			Categoria:
		</td>
		<td>
			<?= campo_texto( 'paramCategoria', 'N', 'S', '', 65, 65, '', '' ); ?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
			Fase:
		</td>
		<td>
			<?= campo_texto( 'paramFase', 'N', 'S', '', 65, 100, '', '' ); ?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
		</td>
		<td>
			<input type="button" id="btnBuscar" name="btnBuscar" value="Buscar" onclick="buscarCategorias()"/>
		</td>
	</tr>
	<tr align='left'>
		<td>
		</td>
		<td align='left'>
			<div id='listaCategorias' align='left'></div>
		</td>
	</tr>
</table>
</form>
<script>
function buscarCategorias() {		

	var categoria = document.getElementsByName( "paramCategoria" )[0].value;
	var fase      = document.getElementsByName( "paramFase" )[0].value;
	var div       = document.getElementById( "listaCategorias" );		

	var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: '&req=buscar&categoria=' + categoria + '&fase=' + fase,
				asynchronous: false,
				onComplete: function(resp){
					div.innerHTML = resp.responseText;
				},
				onLoading: function(){
					div.innerHTML = "carregando...";
				}
		});		
}

function excluirCategoria( catid ) {

	if( !confirm('Deseja realmente excluir esta categoria?') ){
        return false;
    }
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=excluir&catid=' + catid,
		onComplete: function(res){
			alert('Categoria Excluida.');
			buscarCategorias();
		}
	});	
}

function alterarCategoria( catid ) {

	window.location.href = 'brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A&req=importar&catid='+catid;
	
}



</script>