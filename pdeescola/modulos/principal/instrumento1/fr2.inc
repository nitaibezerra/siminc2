<?php
pde_verificaSessao();
/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

/*----------------- Cabeçalho da Página ---------------------*/ 
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1 - Ficha Resumo 1', ''); 

echo cabecalhoPDE();
echo subCabecalho('Ficha Resumo - Funcionamento da Escola - Dados do desempenho acadêmico da escola no ensino médio. A) Sistema seriado');

$pdeid = $_SESSION["pdeid"];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag();

?>
<table class="tabela " border="0" align="center" cellpadding="2" cellspacing="2" width="100%" style="text-align: center" width="200" border="1">
  <tr>
    <th>Indicadores</th>
    <th colspan="4">Serie</th>
  </tr>
  <tr>
    <th>&nbsp;</th>
    <th>1º</th>
    <th>2º</th>
    <th>3º</th>
    <th>Geral</th>
  </tr>
  <? 
  $serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
						WHERE tmeid = 6 AND scetipo = 'S' ORDER BY sceid");
		
		$totalMatInicial = 0;
		$totalAdmitido = 0;
		$totalAbandono = 0;
		$totalTransferencia = 0;
		$totalMatFinal = 0;
		$totalAprovados = 0;
		$totalReprovados = 0;
				//dbg($serie);
		for($i=0; $i<count($serie); $i++) {
			//echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
			$dados = $db->carregar("SELECT * FROM pdeescola.aproveitamentoaluno
								  	WHERE pdeid = ".$pdeid." 
									  AND sceid = ".$serie[$i]["sceid"]."
									  AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			
			for($j=0; $j<count($dados); $j++) {
				$divisor = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] );
				if($divisor != 0) {
					$taxa_aprovacao = number_format((($dados[$j]["apaqtdaprovados"] / $divisor) * 100),1, ',','.')."%";
					$taxa_reprovacao = number_format((($dados[$j]["apaqtdreprovados"] / $divisor) * 100),1, ',','.')."%";
					$taxa_abandono = number_format((($dados[$j]["apaqtdafastadosabandono"] /  $divisor) * 100),1, ',','.')."%";
					
					if($taxa_aprovacao > 100){
						$taxa_aprovacao = 100;
					}
					
					if($taxa_reprovacao > 100){
						$taxa_reprovacao = 100;
					}
					
					if($taxa_abandono > 100){
						$taxa_abandono = 100;
					}
					
				}
				else {
					$taxa_aprovacao = "";
					$taxa_reprovacao = "";
					$taxa_abandono = "";
				}
				
				if($serie[$i]["sceid"] == "20"){
					$taxa_aprovacao1 	= $taxa_aprovacao;
					$taxa_reprovacao1 	= $taxa_reprovacao;
					$taxa_abandono1 	= $taxa_abandono;
					$totalAprovados1a3 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a3 += $dados[$j]["apaqtdmatriculainicial"];
					$totalReprovados1a3 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a3 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "21"){
					$taxa_aprovacao2 	= $taxa_aprovacao;
					$taxa_reprovacao2 	= $taxa_reprovacao;
					$taxa_abandono2 	= $taxa_abandono;
					$totalAprovados1a3 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a3 += $dados[$j]["apaqtdmatriculainicial"];
					$totalReprovados1a3 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a3 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "22"){
					$taxa_aprovacao3 	= $taxa_aprovacao;
					$taxa_reprovacao3 	= $taxa_reprovacao;
					$taxa_abandono3 	= $taxa_abandono;
					$totalAprovados1a3 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a3 += $dados[$j]["apaqtdmatriculainicial"];
					$totalReprovados1a3 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a3 += $dados[$j]["apaqtdafastadosabandono"];
				}
			
			}
		}
		if( $totalMatInicial1a3 != 0 ) {
			$totalTaxaAprovacao1a3 = str_replace('.',',',number_format((($totalAprovados1a3 / $totalMatInicial1a3) * 100),2));
			$totalTaxaReprovacao1a3 = str_replace('.',',',number_format((($totalReprovados1a3 / $totalMatInicial1a3) * 100),2));
			$totaTaxaAbandono1a3 = str_replace('.',',',number_format((($totalAbandono1a3 / $totalMatInicial1a3) * 100),2));
		}
			if($totalTaxaAprovacao1a3 > 100){
			   $totalTaxaAprovacao1a3 = 100 ;
			}else {
			$totalTaxaAprovacao = 0;
			$totalTaxaReprovacao = 0;
			$totaTaxaAbandono = 0;
		}
  
  ?>
  <tr>
    <td class="SubTituloDireita" >Taxa de Aprovação </td>
    <td><?=round($taxa_aprovacao1).'%';?></td>
    <td><?=round($taxa_aprovacao2).'%';?></td>
    <td><?=round($taxa_aprovacao3).'%';?></td>
    <td><?=round($totalTaxaAprovacao1a3).'%'; ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Taxa de reprovação </td>
    <td><?=round($taxa_reprovacao1).'%';?></td>
    <td><?=round($taxa_reprovacao2).'%';?></td>
    <td><?=round($taxa_reprovacao3).'%';?></td>
    <td><?=round($totalTaxaReprovacao1a3).'%'; ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Taxa de Abandono </td>
    <td><?=round($taxa_abandono1).'%'; ?></td>
    <td><?=round($taxa_abandono2).'%'; ?></td>
    <td><?=round($taxa_abandono3).'%'; ?></td>
    <td><?=round($totaTaxaAbandono1a3).'%'; ?></td>
  </tr>
</table>
<form action="" method="post">
<table bgcolor="#C0C0C0" class="tabela" cellSpacing="1" cellPadding="3" align="center">
<? $cForm->montarbuttons(); ?>
</table>
</form>