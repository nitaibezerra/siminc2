<?php

// monta cabeçalho


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );



if(isset($_REQUEST['nota'])){
	foreach($_REQUEST['nota'] as $id=>$p){
		$sql = "update reuni.pergunta set prgpeso = $p where prgcod = $id ";
		$db->executar($sql);
	}
	$db->commit();
	$db->sucesso('principal/peso&acao=C');
}



$sql = "
select
	codPai,
	dscPai,
	ordemPai,
	codFilho,
	dscFilho,
	ordemFilho,
	codPergunta,
	dscPergunta,
	ordemPergunta,
	obrPergunta,
	qtdCaracteres,
	pesoPergunta
from
	(
		select
			gp.grpcod as codPai,
			gp.grpdsc as dscPai,
			gp.grpord as ordemPai,
			gp2.grpcod as codFilho,
			gp2.grpdsc as dscFilho,
			gp2.grpord as ordemFilho,
			p.prgcod as codPergunta,
			p.prgdsc as dscPergunta,
			p.prgord as ordemPergunta,
			p.prgobr as obrPergunta,
			p.prgqtdcar as qtdCaracteres,
			p.prgpeso as pesoPergunta
		from reuni.grupopergunta gp
			inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
			inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
	union all
		select
			gp.grpcod as codPai,
			gp.grpdsc as dscPai,
			gp.grpord as ordemPai,
			null as codFilho,
			null as dscFilho,
			null as ordemFilho,
			p.prgcod as codPergunta,
			p.prgdsc as dscPergunta,
			p.prgord as ordemPergunta,
			p.prgobr as obrPergunta,
			p.prgqtdcar as qtdCaracteres,
			p.prgpeso as pesoPergunta
		from reuni.grupopergunta gp
			inner join reuni.pergunta p ON p.grpcod = gp.grpcod
		where gp.grpcodfil is null
	) as foo
order by
	ordemPai,
	ordemFilho,
	ordemPergunta
";

$result = $db->carregar($sql);


?>
<form method="post" action="" name="formulario">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">

<tr>
	<td class="SubTituloEsquerda" align="right" width="40%" >
		Questões
	</td>
	
	<td class="SubTituloEsquerda" colspan="6">
		Pontuação
	</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" align="right" colspan="7">
		<b>1. As dimenções do plano de reestruturação</b>
	</td>
	

</tr>


	<? $i=0;
	while ($i < count($result))
	{
		?>
			<tr>
				<td  colspan="7" class="SubTituloEsquerda" align="right" style="padding-left: <?php if($result[$i]['codpai'] >= 28) echo ''; else echo'20px'?>">
				
					<b> 
						<?php 
						
							echo $result[$i]['dscpai'];

						 ?>
					</b>
				</td>
				
			
			
			<?php if($result[$i]['codpai'] >= 28){?>
			<tr>
						<td class="SubTituloDireita">
					 		<b>Peso:</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>1</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>2</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>3</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>4</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>5</b>
						</td>
						<td class="SubTituloEsquerda" width="100%"> 
						&nbsp;
						</td>
			<?php }?>
			</tr>
		<?		

		$codPai     = $result[$i]['codpai'];
		$codPaiNovo = $result[$i]['codpai'];


		while($codPai == $codPaiNovo )
		{
			$codFilho = $result[$i]['codfilho'];
			$codFilhoNovo = $codFilho;

			if($codFilho != '')
			{
				?>
					<tr  style="cursor: pointer ;">
						<td colspan="7" class="SubTituloEsquerda" align="right" style="padding-left: 30px" >
							<b>
								<?php echo $result[$i]['dscfilho']; ?>
							</b>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">
					 	<b>Peso:</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>1</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>2</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>3</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>4</b>
						</td>
						<td class="SubTituloEsquerda" style="text-align:center">
						<b>5</b>
						</td>
						<td class="SubTituloEsquerda" width="100%"> 
						&nbsp;
						</td>
					</tr>	
				<?
				$j = 1 ;
				while($codFilhoNovo == $codFilho  )
				{
					$img = '';

					?>
						<tr>
							<td class="SubTituloDireita" align="right" style="padding-left: 40px">
								<?= $img ?>
								<?= $j ?>
								   <?= $result[$i]['dscpergunta'] ?>
							</td>
							
								
								<?php echo montaNotas( "nota[".$result[$i]['codpergunta']. "]" , $result[$i]['pesopergunta'] );?>
							
						</tr>
					<?
					$j++;
					$i++;
					$codFilhoNovo = $result[$i]['codfilho'];
				}
				$codPaiNovo = $result[$i]['codpai'];
			}else{
				$img = '';

				?>
					<tr  >
						<td class="SubTituloDireita" align="right" style="padding-left: 40px">
							<?= $img ?>
								<?= $result[$i]['dscpergunta'] ?>
						</td>	
				
							<?php echo montaNotas( "nota[".$result[$i]['codpergunta']. "]",$result[$i]['pesopergunta'] );?>
						
						
					</tr>
					
				<?
				$t++;
				$i++;
				$codPaiNovo = $result[$i]['codpai'];
			}
		}
	}
?>

	<tr bgcolor="#cococo">
			<td align="right"></td>
			<td align="left" colspan="6"><input class="botao" type="button" value="Voltar">
				<input class="botao" type="button" value="Gravar" onclick="document.formulario.submit()">
			</td>
	</tr>

</table>
</form>


<?php

function montaNotas($nome,$nota=0){
	//$nota = int($nota);

	$saida = '';
	$check = '';
	for ($i = 1 ; $i<6;$i++){

		if($nota == $i){
			$check = 'checked="checked"';
		}
		$saida .= '<td width="2%"><input type="radio" name="'.$nome.'" value="'.$i.'" '.$check.'></td>' ;
		$check = '';
	}

	return $saida;
}

?>