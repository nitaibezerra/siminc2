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
echo subCabecalho('Ficha Resumo - Funcionamento da Escola - Dados do desempenho acadêmico da escola no ensino fundamental. A) Sistema seriado');

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
<table class="tabela" border="0" align="center" cellpadding="2" cellspacing="2" width="100%" style="text-align: center" width="200" border="1">
  <tr>
    <th rowspan="3">Indicadores</th>
    <th colspan="12">Serie/Ano</th>
  </tr>
  <tr>
    <th colspan="7">0/1º a 4º/5º e Multiseriada </th>
    <th colspan="6">5º/6º a 8º/9º </th>
  </tr>
  <tr>
    <th>0/1º</th>
    <th>1º/2º</th>
    <th>2º/3º</th>
    <th>3º/4º</th>
    <th>4º/5º</th>
    <th>Mult</th>
    <th>Geral</th>
    <th>5º/6º</th>
    <th>6º/7º</th>
    <th>7º/8º</th>
    <th>8º/9°</th>
    <th>Geral</th>
  </tr>
  <? 
  $serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
						WHERE tmeid in (2,3) AND scetipo = 'S' ORDER BY sceid");
		
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
			$dados = $db->carregar("SELECT distinct * FROM pdeescola.aproveitamentoaluno
								  	WHERE pdeid = ".$pdeid." 
									  AND sceid = ".$serie[$i]["sceid"]."
									  AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			
			
			for($j=0; $j<count($dados); $j++) {
				$divisor = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] );
				if($divisor != 0) {
					$taxa_aprovacao = number_format((($dados[$j]["apaqtdaprovados"] / $divisor) * 100),1, ',','.')."%";
					$taxa_reprovacao = number_format((($dados[$j]["apaqtdreprovados"] / $divisor) * 100),1,',','.')."%";
					$taxa_abandono = number_format((($dados[$j]["apaqtdafastadosabandono"] / $divisor) * 100),1,',','.')."%";
					
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
				
				if($serie[$i]["sceid"] == "1"){
					$taxa_aprovacao1 	= $taxa_aprovacao;
					$taxa_reprovacao1 	= $taxa_reprovacao;
					$taxa_abandono1 	= $taxa_abandono;
					$totalAprovados1a4 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a4 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal1a4 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados1a4 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a4 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "2"){
					$taxa_aprovacao2 	= $taxa_aprovacao;
					$taxa_reprovacao2 	= $taxa_reprovacao;
					$taxa_abandono2 	= $taxa_abandono;
					$totalAprovados1a4 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a4 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal1a4 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados1a4 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a4 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "3"){
					$taxa_aprovacao3 	= $taxa_aprovacao;
					$taxa_reprovacao3 	= $taxa_reprovacao;
					$taxa_abandono3 	= $taxa_abandono;
					$totalAprovados1a4 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a4 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal1a4 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados1a4 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a4 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "4"){
					$taxa_aprovacao4 	= $taxa_aprovacao;
					$taxa_reprovacao4 	= $taxa_reprovacao;
					$taxa_abandono4 	= $taxa_abandono;
					$totalAprovados1a4 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a4 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal1a4 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados1a4 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a4 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "5"){
					$taxa_aprovacao5 	= $taxa_aprovacao;
					$taxa_reprovacao5 	= $taxa_reprovacao;
					$taxa_abandono5 	= $taxa_abandono;
					$totalAprovados1a4 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a4 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal1a4 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados1a4 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a4 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "10"){
					$taxa_aprovacao10 	= $taxa_aprovacao;
					$taxa_reprovacao10 	= $taxa_reprovacao;
					$taxa_abandono10 	= $taxa_abandono;
					$totalAprovados1a4 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial1a4 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal1a4 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados1a4 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono1a4 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "6"){
					$taxa_aprovacao6 	= $taxa_aprovacao;
					$taxa_reprovacao6 	= $taxa_reprovacao;
					$taxa_abandono6 	= $taxa_abandono;
					$totalAprovados5a9 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial5a9 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal5a9 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados5a9 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono5a9 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "7"){
					$taxa_aprovacao7 	= $taxa_aprovacao;
					$taxa_reprovacao7 	= $taxa_reprovacao;
					$taxa_abandono7 	= $taxa_abandono;
					$totalAprovados5a9 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial5a9 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal5a9 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados5a9 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono5a9 += $dados[$j]["apaqtdafastadosabandono"];
				}
				if($serie[$i]["sceid"] == "8"){
					$taxa_aprovacao8 	= $taxa_aprovacao;
					$taxa_reprovacao8 	= $taxa_reprovacao;
					$taxa_abandono8 	= $taxa_abandono;
					$totalAprovados5a9 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial5a9 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal5a9 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados5a9 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono5a9 += $dados[$j]["apaqtdafastadosabandono"];
				}
			if($serie[$i]["sceid"] == "9"){
					$taxa_aprovacao9 	= $taxa_aprovacao;
					$taxa_reprovacao9 	= $taxa_reprovacao;
					$taxa_abandono9 	= $taxa_abandono;
					$totalAprovados5a9 += $dados[$j]["apaqtdaprovados"];
					$totalMatInicial5a9 += $dados[$j]["apaqtdmatriculainicial"];
					$totalMatFinal5a9 += $dados[$j]["apaqtdmatriculafinal"];
					$totalReprovados5a9 += $dados[$j]["apaqtdreprovados"];
					$totalAbandono5a9 += $dados[$j]["apaqtdafastadosabandono"];
				}
				
			}
		}
		if($totalMatInicial1a4 != 0) {
			$totalTaxaAprovacao1a4 = str_replace('.',',',number_format((($totalAprovados1a4 / $totalMatInicial1a4) * 100),1));
			$totalTaxaReprovacao1a4 = str_replace('.',',',number_format((($totalReprovados1a4 / $totalMatInicial1a4) * 100),1));
			$totaTaxaAbandono1a4 = str_replace('.',',',number_format((($totalAbandono1a4 / $totalMatInicial1a4) * 100),1));
			
			if($totalTaxaAprovacao1a4 > 100){
				$totalTaxaAprovacao1a4 = 100 ;
			}else{
				$totalTaxaAprovacao = 0;
				$totalTaxaReprovacao = 0;
				$totaTaxaAbandono = 0;
			}
		}
		
		if($totalMatInicial5a9 != 0) {
				$totalTaxaAprovacao5a9 = str_replace('.',',',number_format((($totalAprovados5a9 / $totalMatInicial5a9) * 100),1));
				$totalTaxaReprovacao5a9 = str_replace('.',',',number_format((($totalReprovados5a9 / $totalMatInicial5a9) * 100),1));
				$totaTaxaAbandono5a9 = str_replace('.',',',number_format((($totalAbandono5a9 / $totalMatInicial5a9) * 100),1));
	
				if($totalTaxaAprovacao5a9 > 100){
					$totalTaxaAprovacao5a9 = 100;
				}else{
					$totalTaxaAprovacao = 0;
					$totalTaxaReprovacao = 0;
					$totaTaxaAbandono = 0;
				}
		}
  
  ?>
  <tr>
    <td class="SubTituloDireita" >Taxa de Aprovação </td>
    <td><?=round($taxa_aprovacao1).'%';?></td>
    <td><?=round($taxa_aprovacao2).'%';?></td>
    <td><?=round($taxa_aprovacao3).'%';?></td>
    <td><?=round($taxa_aprovacao4).'%';?></td>
    <td><?=round($taxa_aprovacao5).'%';?></td>
    <td><?=round($taxa_aprovacao10).'%';?></td>
    <td><?=round($totalTaxaAprovacao1a4).'%'; ?></td>
    <td><?=round($taxa_aprovacao6).'%'; ?></td>
    <td><?=round($taxa_aprovacao7).'%'; ?></td>
    <td><?=round($taxa_aprovacao8).'%'; ?></td>
    <td><?=round($taxa_aprovacao9).'%'; ?></td>
    <td><?=round($totalTaxaAprovacao5a9).'%'; ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Taxa de reprovação </td>
    <td><?=round($taxa_reprovacao1).'%';?></td>
    <td><?=round($taxa_reprovacao2).'%';?></td>
    <td><?=round($taxa_reprovacao3).'%';?></td>
    <td><?=round($taxa_reprovacao4).'%';?></td>
    <td><?=round($taxa_reprovacao5).'%';?></td>
    <td><?=round($taxa_reprovacao10).'%';?></td>
    <td><?=round($totalTaxaReprovacao1a4).'%'; ?></td>
    <td><?=round($taxa_reprovacao6).'%';?></td>
    <td><?=round($taxa_reprovacao7).'%';?></td>
    <td><?=round($taxa_reprovacao8).'%';?></td>
    <td><?=round($taxa_reprovacao9).'%';?></td>
    <td><?=round($totalTaxaReprovacao5a9).'%';?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Taxa de Abandono </td>
    <td><?=round($taxa_abandono1).'%'; ?></td>
    <td><?=round($taxa_abandono2).'%'; ?></td>
    <td><?=round($taxa_abandono3).'%'; ?></td>
    <td><?=round($taxa_abandono4).'%'; ?></td>
    <td><?=round($taxa_abandono5).'%'; ?></td>
    <td><?=round($taxa_abandono10).'%'; ?></td>
    <td><?=round($totaTaxaAbandono1a4).'%'; ?></td>
    <td><?=round($taxa_abandono6).'%'; ?></td>
    <td><?=round($taxa_abandono7).'%'; ?></td>
    <td><?=round($taxa_abandono8).'%'; ?></td>
    <td><?=round($taxa_abandono9).'%'; ?></td>
    <td><?=round($totaTaxaAbandono5a9).'%'; ?></td>
  </tr>
</table>
<form action="" method="post">
<table bgcolor="#C0C0C0" class="tabela" cellSpacing="1" cellPadding="3" align="center">
<? $cForm->montarbuttons(); ?>
</table>
</form>