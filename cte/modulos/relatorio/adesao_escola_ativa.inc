<?php

$refano = $_GET['refano'] ? $_GET['refano'] : '2011';

set_time_limit( 0 );

if($_REQUEST['mostraRelAdesao'] == "true"){
	
	/****************************************************************************************
	*									ESCOLA ATIVA										* 
	****************************************************************************************/	
	
	$sql = "select distinct m.estuf, m.muncod, 
			'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=2&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
			from cte.instrumentounidade i
				inner join territorios.municipio m on m.muncod = i.muncod
				inner join cte.escolaativa ea on i.inuid = ea.inuid
				left join workflow.documento d on d.docid = ea.docid			
			where esasituacaoadesao = 1
			and ea.esaano = '{$refano}'
			and itrid = 2
			and d.esdid in (47,48,49,50,51)
			order by m.estuf, mundescricao ";
	
	$arEscolaAtivaAderiram = $db->carregar( $sql );
	$arEscolaAtivaAderiram = $arEscolaAtivaAderiram ? $arEscolaAtivaAderiram : array();
	
	$sql = "select distinct m.estuf, m.muncod, 
			'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=2&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
			from cte.instrumentounidade i
				inner join territorios.municipio m on m.muncod = i.muncod
				inner join cte.escolaativa ea = i.inuid = ea.inuid			
			where esasituacaoadesao = 2
			and ea.esaano = '{$refano}'
			and itrid = 2
			order by m.estuf, mundescricao ";
	
	$arEscolaAtivaNaoAderiram = $db->carregar( $sql );
	$arEscolaAtivaNaoAderiram = $arEscolaAtivaNaoAderiram ? $arEscolaAtivaNaoAderiram : array();
	
//	$sql = "select distinct m.estuf, m.muncod, 
//			'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=2&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
//			from cte.instrumentounidade i
//				inner join territorios.municipio m on m.muncod = i.muncod
//				inner join cte.escolaativa ea = i.inuid = ea.inuid			
//			where esasituacaoadesao = 3
//			and ea.esaano = '{$refano}'
//			and itrid = 2
//			order by m.estuf, mundescricao ";
//	
//	$arEscolaAtivaMultiseriadas = $db->carregar( $sql );
//	$arEscolaAtivaMultiseriadas = $arEscolaAtivaMultiseriadas ? $arEscolaAtivaMultiseriadas : array();
	
	$sql = "select distinct m.estuf, m.muncod, 
			'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=2&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
			from cte.instrumentounidade i
				inner join territorios.municipio m on m.muncod = i.muncod
				inner join cte.escolaativa ea = i.inuid = ea.inuid			
			where esasituacaoadesao = 3
			and ea.esaano = '{$refano}'
			and itrid = 2
			order by m.estuf, mundescricao ";
	
	$arEscolaAtivaNaoPossuemClasses = $db->carregar( $sql );
	$arEscolaAtivaNaoPossuemClasses = $arEscolaAtivaNaoPossuemClasses ? $arEscolaAtivaNaoPossuemClasses : array();
	
	$sql = "select distinct m.estuf, m.muncod, 
			'<a href=\"cte.php?modulo=lista&acao=M&evento=selecionar&csFiltroArvore=2&muncod='|| m.muncod ||'\">' || m.mundescricao ||'<a>' as mundescricao
			from cte.instrumentounidade i
				inner join territorios.municipio m on m.muncod = i.muncod
				inner join cte.escolaativa ea = i.inuid = ea.inuid			
			where esasituacaoadesao is null
			and ea.esaano = '{$refano}'
			and itrid = 2
			order by m.estuf, mundescricao ";
	
	$arEscolaAtivaNaoManifestados = $db->carregar( $sql );
	$arEscolaAtivaNaoManifestados = $arEscolaAtivaNaoManifestados ? $arEscolaAtivaNaoManifestados : array();
	
	?>
	
		<div style="text-align: center;">
			<a href="javascript: toggleTodos( 'img_divEscolaAtiva[]', 'dadosEscolaAtiva[]', 'abrir' )"><img style="border: none; margin-right: 5px;" src="../imagens/mais.gif" />Abrir Todos</a> 
			<span style="margin: 0 10px;" > |&nbsp;|&nbsp;| </span>  
			<a href="javascript: toggleTodos( 'img_divEscolaAtiva[]', 'dadosEscolaAtiva[]', 'fechar' )"><img style="border: none; margin-right: 5px;" src="../imagens/menos.gif" />Fechar Todos</a>
		</div>
	
		<div id="escolaAtivaAderiram" class="escolaAtiva">
			<div class="labelColulaRelatorio">
				<h3>
					<img src="../imagens/mais.gif" name="img_divEscolaAtiva[]" id="img_escolaAtivaAderiram" onclick="toggleDiv( this.id, 'dadosEscolaAtivaAderiram' );"/>
					&nbsp;&nbsp;&nbsp;
					Municípios que aderiram (<?php echo count( $arEscolaAtivaAderiram ); ?>)
				</h3>
			</div>
			<div id="dadosEscolaAtivaAderiram" name="dadosEscolaAtiva[]" style="display: none;" class="colunasRelatorio" >
				<?php $db->monta_lista_simples( $arEscolaAtivaAderiram, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>				
			</div>
		</div>
	
		<div id="escolaAtivaAderiram" class="escolaAtiva">
			<div class="labelColulaRelatorio">
				<h3>
					<img src="../imagens/mais.gif" name="img_divEscolaAtiva[]" id="img_escolaAtivaNaoAderiram" onclick="toggleDiv( this.id, 'dadosEscolaAtivaNaoAderiram' );"/>
					&nbsp;&nbsp;&nbsp;
					Municípios que não aderiram (<?php echo count( $arEscolaAtivaNaoAderiram ); ?>)
				</h3>
			</div>
			<div id="dadosEscolaAtivaNaoAderiram" name="dadosEscolaAtiva[]" style="display: none;" class="colunasRelatorio">
				<?php $db->monta_lista_simples( $arEscolaAtivaNaoAderiram, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
			</div>
		</div>
	
		<div id="escolaAtivaAderiram" class="escolaAtiva">
			<div class="labelColulaRelatorio">
				<h3>
					<img src="../imagens/mais.gif" name="img_divEscolaAtiva[]" id="img_escolaAtivaNaoPossuemClasses" onclick="toggleDiv( this.id, 'dadosEscolaAtivaNaoPossuemClasses' );"/>
					&nbsp;&nbsp;&nbsp;
					Não possuem classes multisseriadas (<?php echo count( $arEscolaAtivaNaoPossuemClasses ); ?>)
				</h3>
			</div>
			<div id="dadosEscolaAtivaNaoPossuemClasses" name="dadosEscolaAtiva[]" style="display: none;" class="colunasRelatorio">
				<?php $db->monta_lista_simples( $arEscolaAtivaNaoPossuemClasses, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
			</div>
		</div>
	
		<div id="escolaAtivaAderiram" class="escolaAtiva">
			<div class="labelColulaRelatorio">
				<h3>
					<img src="../imagens/mais.gif" name="img_divEscolaAtiva[]" id="img_escolaAtivaNaoManifestados" onclick="toggleDiv( this.id, 'dadosEscolaAtivaNaoManifestados' );"/>
					&nbsp;&nbsp;&nbsp;
					Municípios que não se manifestaram (<?php echo count( $arEscolaAtivaNaoManifestados ); ?>)
				</h3>
			</div>
			<div id="dadosEscolaAtivaNaoManifestados" name="dadosEscolaAtiva[]" style="display: none;" class="colunasRelatorio">
				<?php $db->monta_lista_simples( $arEscolaAtivaNaoManifestados, array('UF', 'Código IBGE', 'Município' ), 6000, 1, 'N', '95%','N' ); ?>
			</div>
		</div>		
	
	<?php
	
	
	die();
	// Acaba relatório escola ativa
}

include_once( APPRAIZ . 'cte/classes/InstrumentoUnidade.class.inc' );
$obInstrumentoUnidade = new InstrumentoUnidade();

if( $_REQUEST["requisicao"] == 'recuperarDados' ){
	
	switch( $_REQUEST["nivel"] ){
		case( 1 ):			
			$stTabela = $obInstrumentoUnidade->recuperarTabelaEstados( $_REQUEST["tipo"], $refano );
			break;
		case( 2 ):
			$stTabela = $obInstrumentoUnidade->recuperarTabelaMunicipios( $_REQUEST["tipo"], $_REQUEST["valor"], $refano );
			break;
		case( 3 ):
			$stTabela = $obInstrumentoUnidade->recuperarTabelaEscolas( $_REQUEST["tipo"], $_REQUEST["valor"], $refano );
			break;
	}
	echo $stTabela;
	
	die();
}
elseif( $_REQUEST["requisicao"] == 'recuperarResumo' ){
	
	switch( $_REQUEST["nivel"] ){
		case( 1 ):
			$stTabela = $obInstrumentoUnidade->recuperarResumo( $_REQUEST["tipo"], '' );
			break;
		case( 2 ):
			$stTabela = $obInstrumentoUnidade->recuperarResumo( $_REQUEST["tipo"], $_REQUEST["valor"] );
			break;
		case( 3 ):
			$stTabela = $obInstrumentoUnidade->recuperarResumo( $_REQUEST["tipo"], "", $_REQUEST["valor"] );
			break;
		case( 4 ):
			$stTabela = $obInstrumentoUnidade->recuperarResumoEscola( $_REQUEST["tipo"], "", "", $_REQUEST["valor"] );
			break;
	}
	echo $stTabela;
	
	die();
}


include_once( APPRAIZ . 'includes/cabecalho.inc' );
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório de Adesão aos Programas", "" );
//monta_titulo_cte( $titulo_modulo );

?>

<script type="text/javascript" src="../includes/prototype.js"></script>

<style type="text/css">

	.filho1{
		text-indent: 20px;
	}

	 .escolaAtiva{
	 	width: 25%;
	 	float: left;
	 	text-align: center;
	 }
	 
	 .proLetramento{
	 	width: 50%;
	 	float: left;
	 	text-align: center;
	 }

	.tituloPrograma{
		text-align: center;
		margin-top: 20px; 
	}
	
	.tituloPrograma img{
		height: 20px;
		width: 20px;
		margin-right: 5px;
	}
	
	.labelColulaRelatorio{		
		height: 45px;
	}
	
	.colunasRelatorio{				 
		height: 300px; 
		overflow: auto;
	}

	.divTabela{
		margin: 20px 0;
	}
	
	.divInterna{
		height: 300px;
		overflow: auto;
	}
	
	#resumo{
		padding: 30px 50px;
		background:#fff;
		color:#000;
		position:absolute;
		border:1px solid #000;
	}
	
</style>

<!--  link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css' -->

<?php

$sql = "select count(muncod)
		from cte.instrumentounidade i		
		where itrid = 2";

//$sql = "select count(ea.inuid) 
//		from cte.escolaativa ea
//		inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
//		left join workflow.documento d on d.docid = ea.docid
//		where esasituacaoadesao is not null
//		and esaano = '{$refano}'
//		and iu.itrid = 2";
		
$nrQuantidadeGeral = $db->pegaUm( $sql );

$sql = "select 
			count(iu.muncod)
		from cte.instrumentounidade iu				
		left join cte.escolaativa ea on ea.inuid = iu.inuid and ea.esaano = '{$refano}'
		where iu.itrid = 2
		and ea.esaid is null";
		
$nrNaoManifestados = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
		inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
		left join workflow.documento d on d.docid = ea.docid
		where esasituacaoadesao = '1'
		and esaano = '{$refano}'
		and iu.itrid = 2
		and d.esdid in (47,48,49,50,51)";

$nrQuantidadeAderiram = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
		inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
		where ea.esasituacaoadesao = '2'
		and iu.itrid = 2
		and ea.esaano = '{$refano}'";

$nrQuantidadeNaoAderiram = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
		inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
		where ea.esasituacaoadesao = '3'
		and ea.esaano = '{$refano}'
		and iu.itrid = 2";

$nrQuantidadeClassesMultiseriadas = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
			inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
			left join workflow.documento d on d.docid = ea.docid
		where d.esdid = 47
		and esasituacaoadesao = '1'
		and iu.itrid = 2
		and ea.esaano = '{$refano}'";

$nrQuantidadeEmPreenchimento = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
			inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
			left join workflow.documento d on d.docid = ea.docid
		where d.esdid = 48		
		and esasituacaoadesao = '1'	
		and iu.itrid = 2	
		and ea.esaano = '{$refano}'";

$nrQuantidadeEmAnalise = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
			inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
			left join workflow.documento d on d.docid = ea.docid
		where d.esdid = 50
		and esasituacaoadesao = '1'	
		and iu.itrid = 2	
		and ea.esaano = '{$refano}'";

$nrQuantidadeEmRevisao = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
			inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
			left join workflow.documento d on d.docid = ea.docid
		where d.esdid = 51
		and esasituacaoadesao = '1'		
		and iu.itrid = 2
		and ea.esaano = '{$refano}'";

$nrQuantidadeEmCorrecao = $db->pegaUm( $sql );

$sql = "select count(ea.inuid) 
		from cte.escolaativa ea
			inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
			left join workflow.documento d on d.docid = ea.docid
		where d.esdid = 49
		and esasituacaoadesao = '1'		
		and ea.esaano = '{$refano}'
		and iu.itrid = 2";

$nrQuantidadeAnalisados = $db->pegaUm( $sql );

$arIndicadores["geral"]["qtd"] = $nrQuantidadeGeral;
$arIndicadores["geral"]["stTitulo"] = "Quantidade Geral";

$arIndicadores["naomanifestados"]["qtd"] = $nrNaoManifestados;
$arIndicadores["naomanifestados"]["stTitulo"] = "Municípios que não se manifestaram";

$arIndicadores["aderiram"]["qtd"] = $nrQuantidadeAderiram;   
$arIndicadores["aderiram"]["stTitulo"] = "Municípíos que aderiram";

$arIndicadores["naoaderiram"]["qtd"] = $nrQuantidadeNaoAderiram;   
$arIndicadores["naoaderiram"]["stTitulo"] = "Municípíos que não aderiram";

$arIndicadores["classesmultiseriadas"]["qtd"] = $nrQuantidadeClassesMultiseriadas;   
$arIndicadores["classesmultiseriadas"]["stTitulo"] = "Não há classes multiseriadas";

$arIndicadores["em_preenchimento"]["qtd"] = $nrQuantidadeEmPreenchimento;   
$arIndicadores["em_preenchimento"]["stTitulo"] = "Em Preenchimento";

$arIndicadores["em_analise"]["qtd"] = $nrQuantidadeEmAnalise;   
$arIndicadores["em_analise"]["stTitulo"] = "Em Análise";

$arIndicadores["em_revisao"]["qtd"] = $nrQuantidadeEmRevisao;   
$arIndicadores["em_revisao"]["stTitulo"] = "Em Revisão";

$arIndicadores["em_correcao"]["qtd"] = $nrQuantidadeEmCorrecao;   
$arIndicadores["em_correcao"]["stTitulo"] = "Em Correção";

$arIndicadores["analisado"]["qtd"] = $nrQuantidadeAnalisados;
$arIndicadores["analisado"]["stTitulo"] = "Analisado";

?>
<center>
	<div id="loadAguarde" style="display:none; background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br>
	</div>
</center>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td><center>
			Ano de referência: 
			<?php 
			$arAnos = array(array('codigo' => '2010',
								  'descricao'=>'2009'),
						    array('codigo' => '2011',
						    	  'descricao' => '2010'));
			
			$db->monta_combo('refano', $arAnos, 'S', '', 'selecionaAno', '');
			?>
			</center>
			<?php foreach( $arIndicadores as $tipo => $arIndicador ){ ?>
			
				<div class="divTabela">
					<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr class="title" style="background: #e3e3e3;">
							<td>
								<img src="../imagens/mais.gif" name="img_<?php echo $tipo ?>[]" id="img_<?php echo $tipo ?>" onclick="toggleDiv( this.id, 'linha_<?php echo $tipo ?>', 'div_<?php echo $tipo ?>', '<?php echo $tipo ?>', '', 1 );"/>
								&nbsp;&nbsp;&nbsp;<span onclick="abrirResumo( 'resumo_<?php echo $tipo ?>', '<?php echo $tipo ?>', '', 1 );"><?php echo $arIndicador["stTitulo"] ?> ( <?php echo $arIndicador["qtd"] ?> )</span>
								<div id="resumo_<?php echo $tipo ?>" class="resumo" style="display: none"></div>
							</td>
						</tr>
						<tr class="title" style="display: none" id="linha_<?php echo $tipo ?>">
							<td>
								<div id="div_<?php echo $tipo ?>"></div>							
							</td>
						</tr>
					</table>
				</div>
				
			<?php } ?>
			
		</td>
	</tr>
</table>

<script type="text/javascript">

	function selecionaAno(ano){
				
		document.location.href = '?modulo=relatorio/adesao_escola_ativa&acao=A&refano='+ano;
	}
	
	function abrirResumo( idComponente, tipo, valor, nivel ){

		var componenet = document.getElementById( idComponente );
			
		if( !componenet.innerHTML ){
			carregarResumo( tipo, nivel, valor, idComponente ); 
		}
		
		if( componenet.style.display == 'none' ){
			componenet.style.display = 'block';
		}
		else{
			componenet.style.display = 'none';
		}
	}
	
	function toggleDiv( idImg, idComponente, idDivInterna, tipo, valor, nivel ){

		var img = document.getElementById( idImg );	
		var componenet = document.getElementById( idComponente );
		var divInterna = document.getElementById( idDivInterna );
				
		if( !divInterna.innerHTML ){
			carregarDiv( tipo, nivel, valor, idDivInterna );
		}
		
		if( componenet.style.display == 'none' ){
			componenet.style.display = '';
			img.src = '../imagens/menos.gif'
		}
		else{
			componenet.style.display = 'none';
			img.src = '../imagens/mais.gif'
		}
	}

	function carregarDiv( tipo, nivel, valor, idDivInterna ){
		
		$('loadAguarde').show();
		var divInterna = document.getElementById( idDivInterna );
		
		var parametros = 'requisicao=recuperarDados&valor='+valor+'&nivel='+nivel+'&tipo='+tipo;

		return new Ajax.Request(window.location.href,{	
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function( resposta ){
				
				divInterna.innerHTML = resposta.responseText;
				$('loadAguarde').hide();	
			}
		});		
		
	}

	function carregarResumo( tipo, nivel, valor, idDivInterna ){
		
		$('loadAguarde').show();
		var divInterna = document.getElementById( idDivInterna );
		
		var parametros = 'requisicao=recuperarResumo&valor='+valor+'&nivel='+nivel+'&tipo='+tipo;
		
		return new Ajax.Request(window.location.href,{	
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function( resposta ){
				
				divInterna.innerHTML = resposta.responseText;
				$('loadAguarde').hide();	
			}
		});		
		
	}

</script>