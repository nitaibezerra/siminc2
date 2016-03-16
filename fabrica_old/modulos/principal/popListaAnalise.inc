<?php

/*** Inclue os arquivos necessários ***/
//require_once "config.inc";
//include APPRAIZ . "includes/classes_simec.inc";

/*** Se os dados da lista foram submetidos... ***/
if( $_POST['lista'] )
{
	echo '<script>';
	
	$html = '<input type="hidden" name="dspid['.$_POST['dspid'].']" value="'.$_POST['tpeid'].'" />';
	$listaProdutos = '';
	foreach($_POST['lista'] as $item)
	{
		$html .= '<input type="hidden" name="prdid[]" value="'.$item.'" />';
		
		$dsc = $db->pegaUm("SELECT prddsc FROM fabrica.produto WHERE prdid = $item"); 
		$listaProdutos .= '<img src="../imagens/arrow_simples.gif"> '.$dsc.'<br>';
	}
	
	if($listaProdutos) {
		echo 'window.opener.$("#botaonao_'.$_POST['dspid'].'_'.$_POST['tpeid'].'").hide();';
		echo 'window.opener.$("#botaosim_'.$_POST['dspid'].'_'.$_POST['tpeid'].'").show();';
	}
	else{
		echo 'window.opener.$("#botaosim_'.$_POST['dspid'].'_'.$_POST['tpeid'].'").hide();';
		echo 'window.opener.$("#botaonao_'.$_POST['dspid'].'_'.$_POST['tpeid'].'").show();';
	} 
	
	
	echo '	window.opener.$("#'.$_POST['dspid'].'_'.$_POST['tpeid'].'").html(\''.$html.'\');
			window.opener.$("#produtos_'.$_POST['dspid'].'_'.$_POST['tpeid'].'").html(\''.$listaProdutos.'\');
			alert("Dados gravados com sucesso.");
			self.close();
		  </script>';
	die;
}

if( $_GET['nome'] )
{
	/*** ***/
	$nome = urldecode($_GET['nome']);
	
	$arr = explode('_', $nome);
	$dspid = $arr[0];
	$tpeid = $arr[1];
	
	/*** Se o SQL da lista foi informado... ***/
	if( $_SESSION['sql_lista_analise'][$nome] )
	{
		/*** Cria uma instância do DB ***/
		//$db = new cls_banco();
		/*** Carrega os dados da lista ***/
		$dadosLista = $db->carregar($_SESSION['sql_lista_analise'][$nome]);
		
		/*** Prepara o titulo da página ***/
		$titulo = urldecode($_GET['titulo']);
	}
}

?>

<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div style="width:100%;height:390px;overflow:auto;">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr bgcolor="#cdcdcd">
					<td style="text-align:center;">
						<input type="checkbox" id="checkMarcaTodos" />
					</td>
					<td>
						<strong><?= $titulo ?></strong>
					</td>
				</tr>
				<tr bgcolor="#dcdcdc">
					<td width="1" style="text-align: center;">&nbsp;</td>
					<td style="text-align: center;">Descrição</td>
				</tr>
				<?php if( $dadosLista ): ?>
				<?php $cont = 0; ?>
				<?php foreach($dadosLista as $dadoLista): ?>
				<?php $cor = ($cont%2) ? '#e0e0e0' : '#f4f4f4'; ?>
				<tr bgcolor="<?= $cor; ?>">
					<td>
						<input type="checkbox" id="<?php echo $dadoLista['codigo']; ?>" onchange="checkItemLista(<?php echo $dadoLista['codigo']; ?>,'<?php echo $dadoLista['descricao']; ?>', true);" />
					</td>
					<td>
						<span id="desc_<?php echo $dadoLista['codigo']; ?>"><?php echo $dadoLista['descricao']; ?></span>
					</td>
				</tr>
				<?php $cont++; ?>
				<?php endforeach; ?>
				<?php else: ?>
				<tr>
					<td colspan="3" style="text-align:center;color:red;">Nenhum registro encontrado</td>
				</tr>
				<?php endif; ?>
			</table>
		</div>
		<form method="post" id="formPopLista" action="">
			<input type="hidden" name="dspid" value="<?php echo $dspid; ?>" />
			<input type="hidden" name="tpeid" value="<?php echo $tpeid; ?>" />
			<select id="lista" name="lista[]" multiple="multiple" style="width:100%;height:80px;border-top:2px solid #7f9db9;">
				<option value="">[Selecione algum item da lista]</option>
			</select>
			<div style="text-align:center;width:100%;background-color:#c0c0c0;padding-top:5px;padding-bottom:5px;">
				<input id="btFormLista" type="button" value="Salvar" />
			</div>
		</form>
	</body>
</html>
<script type="text/javascript">
/*** Quando o documento estiver pronto... ***/
$(document).ready(function()
{
	if( window.opener.$('#<?php echo $nome; ?>').html() != '' )
	{
		window.opener.$('#<?php echo $nome; ?> input').each(function()
		{
			if( $(this).attr('name') == 'prdid[]' )
			{
				var prdid = $(this).val();
				/*** Percorre todos os itens da lista ***/
				$('input[type="checkbox"]').each(function()
				{
					if( $(this).attr('id') != 'checkMarcaTodos' )
					{
						if( Number($(this).attr('id')) == Number(prdid) )
						{
							/*** Marca o item da lista ***/
							$(this).attr('checked',true);
							/*** Prepara variáveis com o código e a descricão do item ***/
							codigo		= $(this).attr('id');
							descricao	= $('#desc_' + codigo).html();
							/*** Inclui o item na lista utilizando a função adequada ***/
							checkItemLista(codigo, descricao, true);
						}
					}
				});
			}
		});
	}
	
	/*** No evento 'onclick' do botão de 'Salvar' da lista ***/
	$('#btFormLista').click(function()
	{
		/*** Flag para verificar se algum item foi adicionado na lista ***/
		var flag = false;
		
		/*** Percorre todos os itens do select ***/
		$('#lista').each(function()
		{
			$('option', this).each(function()
			{
				/*** Se o option tiver value(evita a opção padrão: '[Selecione algum item da lista]')... ***/
				if( $(this).val() != "" )
				{
					/*** Marca cada item antes de submeter o formulário ***/
					$(this).attr('selected','selected');
					/*** Altera a flag ***/
					flag = true;
				}
			});
		});

		/*** Se existe algum item na lista... ***/
		if( flag )
		{
			$('#formPopLista').submit();
		}
		/*** Se nenhum item tiver sido selecionado... ***/
		else
		{
			alert('Algum item da lista deve ser selecionado.');
		}
	});

	/*** No evento 'onchange' do checkbox para marcar todas as opções da lista ***/
	$('#checkMarcaTodos').change(function()
	{
		/*** Percorre todos os itens da lista ***/
		$('input[type="checkbox"]').each(function()
		{
			/*** Se o item não for opção de marcar todos, e esta estiver marcada... ***/
			if( $(this).attr('id') != 'checkMarcaTodos' && $('#checkMarcaTodos').is(':checked') )
			{
				/*** Se o item não estiver marcado... ***/
				if( $(this).attr('checked') == false )
				{
					/*** Marca o item da lista ***/
					$(this).attr('checked',true);
					/*** Prepara variáveis com o código e a descricão do item ***/
					codigo		= $(this).attr('id');
					descricao	= $('#desc_' + codigo).html();
					/*** Inclui o item na lista utilizando a função adequada ***/
					checkItemLista(codigo, descricao, false);
				}
			}
			/*** Se o item não for opção de marcar todos, e esta não estiver marcada... ***/
			else if( $(this).attr('id') != 'checkMarcaTodos' && $('#checkMarcaTodos').not(':checked') )
			{
				/*** Se o item estiver marcado... ***/
				if( $(this).attr('checked') == true )
				{
					/*** Desmarca o item da lista ***/
					$(this).attr('checked',false);
					/*** Prepara variáveis com o código e a descricão do item ***/
					codigo		= $(this).attr('id');
					descricao	= $('#desc_' + codigo).html();
					/*** Exclui o item na lista utilizando a função adequada ***/
					checkItemLista(codigo, descricao, false);
				}
			}
		});
	}); 
});

/*** Função para incluir/excluir um item na lista ***/
function checkItemLista(codigo, descricao, flag)
{
	/*** Se o item for marcado... ***/
	if( $('#'+codigo).is(':checked') )
	{
		$('#lista').append( $("<option></option>").attr("value", codigo).text(descricao) );
	}
	/*** Se o item for desmarcado... ***/
	else
	{
		/*** Percorre todos os itens do select ***/
		$('#lista').each(function()
		{
			$('option', this).each(function()
			{
				/*** Se o value do option for igual ao item selecionado... ***/
				if( $(this).val() == codigo )
				{
					/*** Exclui o elemento da lista ***/
					$(this).remove();
				}
			});
		});
	}

	/*** Se a flag for true, a chamada da função veio por meio do evento 'onchange' do checkbox da lista, e não pela chamada do evento 'onchange' do #checkMarcaTodos ***/
	if( flag )
	{
		/*** Marca/Desmarca a opção de marcar todos, se necessário ***/
		checkMarcaTodos();
	}
	
	/*** Inclui/Exclui a opção padrão, se necessário ***/
	itemInicial();
}

/*** Marca/Desmarca a opção de marcar todos, dependendo da quantidade de itens marcados/desmarcados na lista ***/
function checkMarcaTodos()
{
	/*** Flag para verificar se todos os itens estão marcados ***/
	var flag = true;
	/*** Percorre todos os itens da lista ***/
	$('input[type="checkbox"]').each(function()
	{
		/*** Se o item não for opção de marcar todos... ***/
		if( $(this).attr('id') != 'checkMarcaTodos' )
		{
			/*** Se o item não estiver marcado... ***/
			if( ! $(this).attr('checked') )
			{
				flag = false;
			}
		}
	});
//alert(flag);
	/*** Se a flag estiver true, todos itens estão marcados... ***/
	if( flag )
	{
		/*** Se o marca todos não estiver marcado... ***/
		if( $('#checkMarcaTodos').not(':checked') )
		{
			/*** Marca a opção de marca todos ***/
			$('#checkMarcaTodos').attr('checked', true);
		}
	}
	/*** Se a flag estiver false, todos itens não estão marcados... ***/
	else
	{
		/*** Se o marca todos estiver marcado... ***/
		if( $('#checkMarcaTodos').is(':checked') )
		{
			/*** Desmarca a opção de marca todos ***/
			$('#checkMarcaTodos').attr('checked', false);
		}
	}
}
/*** Função para incluir/retirar o item inicial da lista ('[Selecione algum item da lista]') ***/
function itemInicial()
{
	/*** Se existe algum elemento na lista... ***/
	if( $('#lista option').size() > 0 )
	{
		/*** Percorre todos os itens do select ***/
		$('#lista').each(function()
		{
			$('option', this).each(function()
			{
				/*** Se o option não tiver value(opção padrão: '[Selecione algum item da lista]')... ***/
				if( $(this).val() == "" )
				{
					$(this).remove();
				}
			});
		});
	}
	/*** Se a lista está vazia... ***/
	else
	{
		$('#lista').append( $("<option></option>").attr("value", "").text("[Selecione algum item da lista]") );
	}
}
</script>