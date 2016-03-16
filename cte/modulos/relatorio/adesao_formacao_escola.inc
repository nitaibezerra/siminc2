<?php

/****************************************************************************************
*									FORMAÇÃO PELA ESCOLA								* 
****************************************************************************************/	

$sqlAd = "select distinct m.estuf, m.muncod,
		'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=7&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao		
		from cte.instrumentounidade i
			inner join territorios.municipio m on m.muncod = i.muncod
		where usucpfformacaoescola is not null
		and itrid = 2
		order by m.estuf, mundescricao";

$arformacaoEscolaAderiram = $db->carregar( $sqlAd );
$arformacaoEscolaAderiram = $arformacaoEscolaAderiram ? $arformacaoEscolaAderiram : array();


$sql = "select distinct m.estuf, m.muncod,
		'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=7&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
		from cte.instrumentounidade i
			inner join territorios.municipio m on m.muncod = i.muncod
		where usucpfformacaoescola is null
		and itrid = 2
		order by m.estuf, mundescricao";

$arformacaoEscolaNaoAderiram = $db->carregar( $sql );
$arformacaoEscolaNaoAderiram = $arformacaoEscolaNaoAderiram ? $arformacaoEscolaNaoAderiram : array();
?>

<div style="text-align: center;">
	<a href="javascript: toggleTodos( 'img_divformacaoEscola[]', 'dadosformacaoEscola[]', 'abrir' )"><img style="border: none; margin-right: 5px;" src="../imagens/mais.gif" />Abrir Todos</a> 
	<span style="margin: 0 10px;" > |&nbsp;|&nbsp;| </span>  
	<a href="javascript: toggleTodos( 'img_divformacaoEscola[]', 'dadosformacaoEscola[]', 'fechar' )"><img style="border: none; margin-right: 5px;" src="../imagens/menos.gif" />Fechar Todos</a>
</div>

<div id="formacaoEscolaAderiram" class="formacaoEscola">
	<div>
		<h3>
			<img src="../imagens/mais.gif" name="img_divformacaoEscola[]" id="img_formacaoEscolaAderiram" onclick="toggleDiv( this.id, 'dadosformacaoEscolaAderiram' );"/>
			&nbsp;&nbsp;&nbsp;
			Municípios que aderiram (<?php echo count( $arformacaoEscolaAderiram ); ?>)
		</h3>
	</div>
	<div id="dadosformacaoEscolaAderiram" name="dadosformacaoEscola[]" style="display: none;" class="colunasRelatorio">
		<?php $db->monta_lista_simples( $arformacaoEscolaAderiram, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
	</div>
</div>
	
<div id="formacaoEscolaNaoAderiram" class="formacaoEscola">
	<div>
		<h3>
			<img src="../imagens/mais.gif" name="img_divformacaoEscola[]" id="img_formacaoEscolaNaoAderiram" onclick="toggleDiv( this.id, 'dadosformacaoEscolaNaoAderiram' );"/>
			&nbsp;&nbsp;&nbsp;
			Municípios que não aderiram (<?php echo count( $arformacaoEscolaNaoAderiram ); ?>)
		</h3>
	</div>
	<div id="dadosformacaoEscolaNaoAderiram" name="dadosformacaoEscola[]" style="display: none;" class="colunasRelatorio">
		<?php $db->monta_lista_simples( $arformacaoEscolaNaoAderiram, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
	</div>
</div>