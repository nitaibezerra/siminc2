<?php

/****************************************************************************************
*									PRÓ-LETRAMENTO										* 
****************************************************************************************/	

$sql = "select distinct m.estuf, m.muncod,
		'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=3&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
		from cte.instrumentounidade i
			inner join territorios.municipio m on m.muncod = i.muncod
		where usucpfproletramento is not null
		and itrid = 2
		order by m.estuf, mundescricao";

$arProLetramentoAderiram = $db->carregar( $sql );
$arProLetramentoAderiram = $arProLetramentoAderiram ? $arProLetramentoAderiram : array();

$sql = "select distinct m.estuf, m.muncod,
		'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=3&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
		from cte.instrumentounidade i
			inner join territorios.municipio m on m.muncod = i.muncod
		where usucpfproletramento is null
		and itrid = 2
		order by m.estuf, mundescricao";

$arProLetramentoNaoAderiram = $db->carregar( $sql );
$arProLetramentoNaoAderiram = $arProLetramentoNaoAderiram ? $arProLetramentoNaoAderiram : array();

?>

	<div style="text-align: center;">
		<a href="javascript: toggleTodos( 'img_divProLetramento[]', 'dadosProLetramento[]', 'abrir' )"><img style="border: none; margin-right: 5px;" src="../imagens/mais.gif" />Abrir Todos</a> 
		<span style="margin: 0 10px;" > |&nbsp;|&nbsp;| </span>  
		<a href="javascript: toggleTodos( 'img_divProLetramento[]', 'dadosProLetramento[]', 'fechar' )"><img style="border: none; margin-right: 5px;" src="../imagens/menos.gif" />Fechar Todos</a>
	</div>

	<div id="proLetramentoAderiram" class="proLetramento">
		<div>
			<h3>
				<img src="../imagens/mais.gif" name="img_divProLetramento[]" id="img_proLetramentoAderiram" onclick="toggleDiv( this.id, 'dadosProLetramentoAderiram' );"/>
				&nbsp;&nbsp;&nbsp;
				Municípios que aderiram (<?php echo count( $arProLetramentoAderiram ); ?>)
			</h3>
		</div>
		<div id="dadosProLetramentoAderiram" name="dadosProLetramento[]" style="display: none;" class="colunasRelatorio">
			<?php $db->monta_lista_simples( $arProLetramentoAderiram, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
		</div>
	</div>
		
	<div id="proLetramentoNaoAderiram" class="proLetramento">
		<div>
			<h3>
				<img src="../imagens/mais.gif" name="img_divProLetramento[]" id="img_proLetramentoNaoAderiram" onclick="toggleDiv( this.id, 'dadosProLetramentoNaoAderiram' );"/>
				&nbsp;&nbsp;&nbsp;
				Municípios que não aderiram (<?php echo count( $arProLetramentoNaoAderiram ); ?>)
			</h3>
		</div>
		<div id="dadosProLetramentoNaoAderiram" name="dadosProLetramento[]" style="display: none;" class="colunasRelatorio">
			<?php $db->monta_lista_simples( $arProLetramentoNaoAderiram, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
		</div>
	</div>