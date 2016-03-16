<script language='javascript' type='text/javascript'>

	/**
	 * Recupera opção selecionada
	 * @name selecionar
	 * @return void
	 */	
	function selecionar(clscodclasse){

		if(clscodclasse == ''){

			alert('Selecione uma Classe.');
			
		}else{

			executarScriptPai("carregarClasse('"+clscodclasse+"')");
			self.close();
			
		}

	}

</script>

<div class="tituloLista"> Lista das Classes </div>

<?php 

// Se for solicitado a filtragem dos dados
if($_POST['requisicao'] == 'filtrar')
	// filtra
	$oClasse->filtrarClasse($_POST['clscodclasse'], $_POST['clsdescclasse']);
else
	// exibe todos
	$oClasse->filtrarClasse('', '');
	
?>

<div class="tituloLista"> 
	<input type='button' class="botao" name='btnSelecionar' id='btnSelecionar' value='Selecionar' onclick='javascript:selecionar();' /> 
</div>