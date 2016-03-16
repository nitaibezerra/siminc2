<?php

cte_verificaSessao();

include_once( APPRAIZ. "cte/classes/Dimensao.class.inc" );
include_once( APPRAIZ. "cte/classes/Diretriz.class.inc" );
include_once( APPRAIZ. "cte/classes/Indicador.class.inc" );
include_once( APPRAIZ. "cte/classes/DiretrizIndicador.class.inc" );

$inuid = $_SESSION['inuid'];
if( $_REQUEST['type'] != "popup"){
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
	$db->cria_aba( '57109', $url, '' );
}
$titulo_modulo = "PAR - Plano de Metas";
monta_titulo( $titulo_modulo,'Cumprimento das Diretrizes');

$itrid = cte_pegarItrid( $inuid );

/****************************************************************************************
*									PESOS DE DIRETRIZ									* 
****************************************************************************************/

$obDiretriz = new Diretriz();
$coDiretriz = $obDiretriz->recuperarTodos( "dirid, dirdsc" );

$obIndicador = new Indicador();
$coDiretrizIndicador = $obIndicador->recuperarPorcentagemDiretrizIndicador( $inuid );

foreach( $coDiretrizIndicador as $arDados ){
	$arDiretrizIndicador[$arDados["dirid"]] = $arDados["porcentagem"]; 
}

$arDadosDiretriz = array();
foreach( $coDiretriz as $indice => $arDiretriz ){
	$nrPorcentagem = array_key_exists( $arDiretriz["dirid"], $arDiretrizIndicador ) ? $arDiretrizIndicador[$arDiretriz["dirid"]] : "0"; 
	
	$arDadosDiretriz[$indice]["dirid"] = $arDiretriz["dirid"];
	$arDadosDiretriz[$indice]["diretriz"] = $arDiretriz["dirdsc"];
	$arDadosDiretriz[$indice]["porcentagem"] = '<span style="width: 100%; display: block; color: '. recuperarCorPorcentagem( $nrPorcentagem ) .'; text-align: right;">'.round( $nrPorcentagem*100, 1 ) ."%<span>";
}

/****************************************************************************************
*									PESOS DE DIMENSÃO									* 
****************************************************************************************/	

$obDimensao = new Dimensao();
$coDimensao = $obDimensao->recuperarTodos( "dimid, dimcod, dimdsc", array( "itrid = 2" ), "dimcod" );

$obIndicador = new Indicador();
$coPorcentagemDimensao = $obIndicador->recuperarPorcentagemDimensao( $inuid );

foreach( $coPorcentagemDimensao as $arDados ){
	$arDimensaoIndicador[$arDados["dimcod"]] = $arDados["porcentagem"]; 
}


$arDadosDimensao = array();
foreach( $coDimensao as $indice => $arDimensao ){
	$nrPorcentagem = array_key_exists( $arDimensao["dimcod"], $arDimensaoIndicador ) ? $arDimensaoIndicador[$arDimensao["dimcod"]] : "0"; 
	
	$arDadosDimensao[$indice]["dimid"] = $arDimensao["dimid"];
	$arDadosDimensao[$indice]["dimensao"] = $arDimensao["dimcod"]." - ".$arDimensao["dimdsc"];
	$arDadosDimensao[$indice]["porcentagem"] = '<span style="width: 100%; display: block; color: '. recuperarCorPorcentagem( $nrPorcentagem ) .'; text-align: right;">'.round( $nrPorcentagem*100, 1 ) ."%<span>";
}

/****************************************************************************************
*									PESOS DE IDEB										* 
****************************************************************************************/	

$obIndicador = new Indicador();
$nrPorcentagem = $obIndicador->recuperarPorcentagemIDEB( $inuid );

$arDadosIDEB[0]["IDEB"] = "IDEB Esperado";
$arDadosIDEB[0]["porcentagem"] = '<span style="width: 100%; display: block; color: '. recuperarCorPorcentagem( $nrPorcentagem ) .'; text-align: right;">'.(round( $nrPorcentagem*100, 1 ) / 10)."<span>";

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			<a href="?modulo=principal/monitora_estrutura&acao=A">Voltar</a>
			<h3 title="Unidade da Federação">
				<?php  
					// Pegando o nome da UF e municipio
					$muncod = cte_pegarMuncod( $inuid );
					$descricaom = cte_pegarMundescricao($muncod);
					echo $descricaom; 
				?>
			</h3>
			<div style="margin-right: 2%; text-align: right">
				<table width="20%" align="right" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem" >
					<tr  class="title" style="background: #e3e3e3; text-align: center;">
						<td colspan="2">Legenda</td>
					</tr>
					<tr style="color: red">
						<td>Vermelho</td>
						<td>De 0% até 49,9%</td>
					</tr>
					<tr style="color: #cc9900">
						<td>Laranja</td>
						<td>De 50% até 69,9%</td>
					</tr>
					<tr style="color: green">
						<td>Verde</td>
						<td>De 70% até 100%</td>
					</tr>
				</table>
			</div>
	</tr>
	<tr>
		<td>
			<h2 class="tituloPorcentagem"><img src="../../imagens/grafico_pizza.png" />Cumprimento das Diretrizes</h2>
			
			<div style="margin-left: 30px; padding: 10px;">
				<a href="javascript: toggleTodos( 'img_diretriz[]', 'linha_diretriz[]', 'abrir' )"><img style="border: none; margin-right: 5px;" src="../imagens/mais.gif" />Abrir Todos</a> 
				<span style="margin: 0 10px;" > |&nbsp;|&nbsp;| </span>  
				<a href="javascript: toggleTodos( 'img_diretriz[]', 'linha_diretriz[]', 'fechar' )"><img style="border: none; margin-right: 5px;" src="../imagens/menos.gif" />Fechar Todos</a>
			</div>
			
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
				<tr class="title" style="background: #e3e3e3; text-align: center;">
					<td>Diretriz</td>
					<td>% Atingida</td>
				</tr>
				<?php $corPai = '#f7f7f7';
				 
				foreach( $arDadosDiretriz as $diretriz ){
					 
					$corPai = $corPai == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7'; ?>
					
					<tr bgColor="<?php echo $corPai ?>" onmouseover="javascript: this.bgColor='#ffffcc';" onmouseout="javascript: this.bgColor='<?php echo $corPai ?>';">
						<td width="93%"><img src="../imagens/mais.gif" name="img_diretriz[]" id="img_diretriz_<?php echo $diretriz["dirid"] ?>" onclick="toggleIndicador( this.id, 'linha_diretriz_<?php echo $diretriz["dirid"] ?>' );"/>&nbsp;&nbsp;<?php echo $diretriz["diretriz"] ?></td>
						<td width="7%"><?php echo $diretriz["porcentagem"] ?></td>
					</tr>
					<?php $coIndicadores = $obIndicador->recuperarTodosPorDiretriz( $diretriz["dirid"] ); ?>
					<tr name="linha_diretriz[]" id="linha_diretriz_<?php echo $diretriz["dirid"] ?>" style="display: none">
						<td colspan="2">
					<?php if( count( $coIndicadores ) ){ ?>
						<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;">
						
							<tr class="title" style="background: #e3e3e3; text-align: center;">
								<td>Indicador</td>
								<td>Peso/Indicador</td>
								<td>% Atingida</td>
							</tr>
							<?php $corFilho1 = '#f7f7f7';
							 
							foreach( $coIndicadores as $indicador ){
								 
								$corFilho1 = $corFilho1 == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7'; ?>
								<tr class="filho1" bgColor="<?php echo $corFilho1 ?>" onmouseover="javascript: this.bgColor='#fffffc';" onmouseout="javascript: this.bgColor='<?php echo $corFilho1 ?>';" >
									<td width="86%" ><img src="../imagens/seta_filho.gif" >&nbsp;&nbsp;<?php echo $indicador["indicador"]; ?></td>
									<td width="7%"><?php echo '<span style="width: 100%; display: block; text-align: right;">'.round( $indicador["dinpeso"]*100, 2 ) ."%<span>" ?></td>
									<td width="7%"><?php echo '<span style="width: 100%; display: block; text-align: right;">'.round( $indicador["porcentagem"]*100, 2 ) ."%<span>" ?></td>
								</tr>
							<?php } ?>
						</table>
					<?php }
					else{ echo '<span style="width: 100%; display: block; text-align: center; color: red; ">Não há indicadores para essa Diretriz!</span>'; } ?>
						</td>
					</tr>
				<?php } ?>
			</table>	
		</td>
	</tr>
	<tr>
		<td>
			<h2 class="tituloPorcentagem"><img src="../../imagens/grafico_pizza.png" />Cumprimento das Dimensões</h2>
			
			<div style="margin-left: 30px; padding: 10px;">
				<a href="javascript: toggleTodos( 'img_dimensao[]', 'linha_dimensao[]', 'abrir' )"><img style="border: none; margin-right: 5px;" src="../imagens/mais.gif" />Abrir Todos</a> 
				<span style="margin: 0 10px;" > |&nbsp;|&nbsp;| </span>  
				<a href="javascript: toggleTodos( 'img_dimensao[]', 'linha_dimensao[]', 'fechar' )"><img style="border: none; margin-right: 5px;" src="../imagens/menos.gif" />Fechar Todos</a>
			</div>
			
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
				<tr class="title" style="background: #e3e3e3; text-align: center;">
					<td>Dimensão</td>
					<td>% Atingida</td>
				</tr>
				<?php $corPai = '#f7f7f7';
				 
				foreach( $arDadosDimensao as $dimensao ){
					 
					$corPai = $corPai == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7'; ?>
					
					<tr bgColor="<?php echo $corPai ?>" onmouseover="javascript: this.bgColor='#ffffcc';" onmouseout="javascript: this.bgColor='<?php echo $corPai ?>';">
						<td width="93%"><img src="../imagens/mais.gif" name="img_dimensao[]" id="img_dimensao_<?php echo $dimensao["dimid"] ?>" onclick="toggleIndicador( this.id, 'linha_dimensao_<?php echo $dimensao["dimid"] ?>' );"/>&nbsp;&nbsp;<?php echo $dimensao["dimensao"] ?></td>
						<td width="7%"><?php echo $dimensao["porcentagem"] ?></td>
					</tr>
					<?php $coIndicadores = $obIndicador->recuperarTodosPorDimensao( $dimensao["dimid"] ); ?>
					<tr name="linha_dimensao[]" id="linha_dimensao_<?php echo $dimensao["dimid"] ?>" style="display: none">
						<td colspan="2">
					<?php if( count( $coIndicadores ) ){ ?>
						<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;">
						
							<tr class="title" style="background: #e3e3e3; text-align: center;">
								<td>Indicador</td>
								<td>Peso/Indicador</td>
								<td>% Atingida</td>
							</tr>
							<?php $corFilho1 = '#f7f7f7';
							 
							foreach( $coIndicadores as $indicador ){
								 
								$corFilho1 = $corFilho1 == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7'; ?>
								<tr class="filho1" bgColor="<?php echo $corFilho1 ?>" onmouseover="javascript: this.bgColor='#fffffc';" onmouseout="javascript: this.bgColor='<?php echo $corFilho1 ?>';" >
									<td width="86%" ><img src="../imagens/seta_filho.gif" >&nbsp;&nbsp;<?php echo $indicador["indicador"]; ?></td>
									<td width="7%"><?php echo '<span style="width: 100%; display: block; text-align: right;">'.round( $indicador["indpesodimensao"]*100, 2 ) ."%<span>" ?></td>
									<td width="7%"><?php echo '<span style="width: 100%; display: block; text-align: right;">'.round( $indicador["porcentagem"]*100, 2 ) ."%<span>" ?></td>
								</tr>
							<?php } ?>
						</table>
					<?php }
					else{ echo '<span style="width: 100%; display: block; text-align: center; color: red; ">Não há indicadores para essa Dimensão!</span>'; } ?>
						</td>
					</tr>
				<?php } ?>
			</table>				
		</td>
	</tr>
	<tr>
		<td>
			<h2 class="tituloPorcentagem"><img src="../../imagens/grafico_pizza.png" />Cumprimento IDEB</h2>
			<?php $obIndicador->monta_lista_simples( $arDadosIDEB, array( "", "" ), 50, 50 ); ?>	
		</td>
	</tr>
</table>

<?php function recuperarCorPorcentagem( $nrPorcentagem ){
	
	if( $nrPorcentagem < 0.5 ){ return  "red"; }
	elseif( $nrPorcentagem < 0.9 ){ return "#cc9900"; }
	return "green";	
}
?>

<style type="text/css">

	.tituloPorcentagem{
		text-align: center;
		margin-top: 20px; 
	}
	
	.tituloPorcentagem img{
		height: 20px;
		width: 20px;
		margin-right: 5px;
	}
	
	.filho1{
		text-indent: 20px;
	}
	
</style>

<script type="text/javascript">

	function toggleIndicador( idImg, idLinha ){

		var img = document.getElementById( idImg );	
		var linha = document.getElementById( idLinha );
		
		if( linha.style.display == 'none' ){
			linha.style.display = '';
			img.src = '../imagens/menos.gif'
		}
		else{
			linha.style.display = 'none';
			img.src = '../imagens/mais.gif'
		}
	}
	
	function toggleTodos( nameImg, nameLinha, acao ){

		var arImg   = getElementsByName_iefix( 'img', nameImg );	
		var arLinha = getElementsByName_iefix( 'tr', nameLinha );
		
		for( var i = 0; i<arLinha.length; i++ ){
			
		
			if( acao == 'abrir' ){
				arLinha[i].style.display = '';
				arImg[i].src = '../imagens/menos.gif'
			}
			else{
				arLinha[i].style.display = 'none';
				arImg[i].src = '../imagens/mais.gif'
			}
		}
	}

function getElementsByName_iefix(tag, name) {  
       
	var elem = document.getElementsByTagName(tag);  
	var arr = new Array();  
    for(i = 0,iarr = 0; i < elem.length; i++) {  
		att = elem[i].getAttribute("name");  
		if(att == name) {  
			arr[iarr] = elem[i];  
			iarr++;  
		}  
	}  
	return arr;  
}  

</script>