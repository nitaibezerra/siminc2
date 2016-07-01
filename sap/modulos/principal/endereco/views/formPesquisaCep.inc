<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>

<script type="text/javascript">

	/**
	 * Aciona o filtro
	 * @name filtrar
	 * @param requisicao - Requisição que será executada
	 * @return boolean
	 */	
	function filtrar(requisicao){

		$('requisicao').setValue(requisicao);

		// uf sempre obrigatorio
		if($('estado').getValue() == ''){

			alert('Selecione a UF');
			return false;
			
		}

		// se preencher o logradouro, o bairro fica obrigatorio
		if($('logradouro').getValue() != '' && $('bairro').getValue() == ''){

			alert('Preencha o Bairro');
			return false;
			
		}

		// se preencher o bairro, ca cidade fica obrigatorio
		if($('bairro').getValue() != '' && $('cidade').getValue() == ''){

			alert('Selecione a cidade');
			return false;
			
		}

		$('formularioPesquisa').submit();
		
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo $oEndereco->montaComboUf($estado); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<div id='cidades'>
					<?php echo $oEndereco->montaComboCidade($estado, $cidade);?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Bairro: </td>
			<td class="campo">
				<?php echo campo_texto('bairro', 'N', 'S', 'Bairro', 100, 100, '', '', '', '', '', 'id="bairro"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Logradouro: </td>
			<td class="campo">
				<?php echo campo_texto('logradouro', 'N', 'S', 'Logradouro', 100, 100, '', '', '', '', '', 'id="logradouro"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="filtrar('filtrar')" />
				<input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' />
		    </td>
		</tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/endereco/views/partialsViews/lista.inc'; ?>
