<?	
	include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
	
	$entid = $_SESSION['pse']["entid"];
	$espid = $_SESSION['pse']["espid"];
	if( $entid == '' || empty($entid)){
		print "<script>"
			. "    alert('Faltam dados para esta tela!');"
			. "    history.back(-1);"
			. "</script>";
		
		die;
	}
	$perguntaPage = "19";
	$ano = 1;
	
	//Salvar Subitem
	if($_REQUEST['salvarSubItens']) {
		GravarSubItens($_POST);
		exit;
	}
	//Listar Subitens
	if($_REQUEST['CarregarLista']) {
		header('content-type: text/html; charset=ISO-8859-1');
		carregaListaSubitens($_REQUEST['pergunta'],$_REQUEST['espid'],$_REQUEST['sub']);
		exit;
	}
	//Excluir Subitem
	if($_REQUEST['ExcluiItem']) {
		header('content-type: text/html; charset=ISO-8859-1');
		excluiSubItem($_REQUEST['asuid']);
		exit;
	}
	
	// monta cabeçalho
	include APPRAIZ . 'includes/cabecalho.inc';
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "Unidade Local Integrada - 3 - Ações de Prevenção e Promoção da Saúde - Componente II";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );
	
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function CarregaLista(item){
	var myajax = new Ajax.Request('pse.php?modulo=principal/compII01&acao=A', {
		        method:     'post',
		        parameters: 'CarregarLista=true&pergunta='+<?=$perguntaPage;?> +'&espid='+<?=$espid;?> + '&sub='+item,
		        asynchronous: false,
		        onComplete: function (res){
					$('lista_'+item).innerHTML = res.responseText;
		        }
	});
}
</script>


<form method="POST" name="formulario" id="formulario">
<?php Cabecalho($entid,1); ?> 
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type='hidden' name='rulid' id='rulid' value='<?=$perguntaPage;?>'></td>
	</tr>
	<?php 
		$idpergunta = explode(",",$perguntaPage);
		foreach($idpergunta as $pergunta){
			perguntasULI($pergunta,$espid); 
		}
	?>	

	<?=navegacao('compInecessidadeCarencia06','compII06');?>

</table>


<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function submeteCombos(item) {
	var subacao			= document.getElementById('subacao_'+item).options[document.getElementById('subacao_'+item).selectedIndex].value;
	var periodicidade	= document.getElementById('periodicidade_'+item).options[document.getElementById('periodicidade_'+item).selectedIndex].value;
	var parceria		= document.getElementById('parceria_'+item).options[document.getElementById('parceria_'+item).selectedIndex].value;
	var participante	= document.getElementById('participante_'+item).options[document.getElementById('participante_'+item).selectedIndex].value;
	var material		= document.getElementById('material_'+item).options[document.getElementById('material_'+item).selectedIndex].value;
		
	var params =	'&subacao=' + subacao + 
					'&periodicidade=' + periodicidade +
					'&parceria='+ parceria + 
					'&participante=' + participante + 
					'&material=' + material +
					'&item=' + item + 
					'&pergunta=' + <?=$perguntaPage;?> + 
					'&espid=' + <?=$espid;?> + 
					'&ano=' + <?=$ano;?>;
	if(subacao==''||periodicidade==''||parceria==''||participante==''||material==''){
		alert('Todos os campos são obrigatórios!');
		return false;
	}
	else {
		var req = new Ajax.Request('pse.php?modulo=principal/compII01&acao=A', {
		        method:     'post',
		        parameters: 'salvarSubItens=true'+params,
		        asynchronous: false,
		        onComplete: function (res){
					alert('operação realizada com sucesso!');
					document.getElementById('subacao_'+item).selectedIndex=0;
					document.getElementById('periodicidade_'+item).selectedIndex=0;
					document.getElementById('parceria_'+item).selectedIndex=0;
					document.getElementById('participante_'+item).selectedIndex=0;
					document.getElementById('material_'+item).selectedIndex=0;
					CarregaLista(item);
		        }
		});
	}
}

function ExcluirSubItem(subitem,acao){
	var myajax = new Ajax.Request('pse.php?modulo=principal/compII01&acao=A', {
		        method:     'post',
		        parameters: 'ExcluiItem=true&asuid='+subitem,
		        asynchronous: false,
		        onComplete: function (res){
					alert('Operação realizada com sucesso!');
					CarregaLista(acao);
		        }
	});	
}

</script>