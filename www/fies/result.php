<?php
include("fies.php");
include APPRAIZ . "includes/classes/dateTime.inc";

//$Cdata = new Data();

$d = new CalculoSimulador();

//echo $d->dado['processoseletivo'];
//dbg($d->dado, 1);
//echo $d->processoseletivo;
?>
<html>
<head>
	<title>FIES - Financiamento Estudantil</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body>
<? 
monta_titulo('Simulador Fies - Resultado da Simulação','');
extract($_POST);
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td colspan="2" style="border-bottom: 1px solid #cccccc;" >
			<div class="textoAzul2" align="left" style="font-weight: normal;">
Esta simulação foi realizada pelo estudante no sítio do FIES na Internet e possui um caráter meramente ilustrativo e visa possibilitar ao estudante interessado no Financiamento Estudantil informações aproximadas sobre a sua dívida futura, bem como o montante de recursos que deverá despender mensalmente para quitá-la. O estudante afirma ter compreendido que variáveis como data e valor da prestação, valor da semestralidade e do financiamento, taxa de juros, data da assinatura do contrato, data da realização dos aditamentos, trazem variações nos valores simulados.
<br><br>
O estudante declara ter informado valores da taxa de juros no campo correspondente o valor praticado no seu contrato levando em conta as diferentes taxas, conforme regulamentação:
<br><br>
Para os contratos firmados até 2005, inclusive, a taxa de juros é de 9% ao ano.
<br><br>
Para os contratos firmados a partir do segundo semestre de 2006 a taxa de juros é de:
- exclusivamente para os cursos de licenciatura, pedagogia, normal superior e cursos constantes do Catálogo de Cursos Superiores de Tecnologia = 3,5% ao ano ou,
- para os demais cursos = 6,5% ao ano.
<br><br>
Os valores das prestações foram calculados considerando os dados informados pelo estudante.
<br><br>
O estudante afirma ter compreendido que os resultados obtidos na simulação do financiamento estudantil não são válidos como proposta ou obrigação do Ministério da Educação, do FIES ou da Caixa Econômica Federal na concessão do crédito.
<br><br>
As condições do financiamento podem ser alteradas caso as normas legais que regem o FIES sejam alteradas. 			
			</div>
		</td>
	</tr>
	<tr>
		<td colspan="2" style="border-bottom: 1px solid #cccccc;" class="SubTituloTelaEsquerda">
		Dados Informados
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Processo Seletivo</td>
		<td width="65%">
			<?=$d->dado['processoseletivo']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Quantidade de Semestres do curso </td>
		<td>
			<?=$d->dado['qtdsemcurso']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Quantidade de semestres já concluídos</td>
		<td>
			<?=$d->dado['qtdsemconc']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual de Financiamento</td>
		<td>
			<?=$d->dado['percentfinanc']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de Estudante</td>
		<td>
			<?=$d->dado['tipoestudante']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor da Semestralidade (Mensalidade * 6)</td>
		<td>
			R$ <?=number_format($d->dado['valsemestre'], 2, ',', '.'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo de Carência </td>
		<td>
			<?=$d->dado['prazocarencia']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Taxa de Juros</td>
		<td>
			<?=$d->dado['taxajuros']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data da Assinatura</td>
		<td>
			<?=$d->dado['dtasscontrato']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Dia escolhido para vencimento das prestações</td>
		<td>
			<?=$d->dado['diavenc']; ?>
		</td>
	</tr>
	<tr>
		<td colspan="2" style="border-bottom: 1px solid #cccccc;" class="SubTituloTelaEsquerda">
		Dados Calculados
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Quantidade de Semestres a serem financiados</td>
		<td>
			<?=$d->dado['quantsemfinanciado'];?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo de utilização em meses(meses a serem financiados)</td>
		<td>
			<?=$d->dado['prazoutilizacao'];?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor do Financiamento(pelos primeiros 6 meses)</td>
		<td>
			R$ <?=number_format($d->dado['valfinseis'], 2, ',', '.');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor do financiamento durante todo o prazo de utilização</td>
		<td>
			R$ <?=number_format($d->dado['valfintotal'], 2, ',', '.');?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Data de Inicio do benefício(para efeito da contagem do prazo)</td>
		<td>
			<?=$d->dado['dtinibeneficio']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de concessão(para efeito do início dos cálculos de juros)</td>
		<td>
			<?=$d->dado['dtconcessao']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo da fase de carência (em meses)</td>
		<td>
			<?=$d->dado['prazocarencia']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo da fase de amortização 1 (em meses)</td>
		<td>
			<?=$d->dado['prazoamortizacaoI']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de inicio da fase de amortização 1 </td>
		<td>
			<?=$d->dado['dtiniamortizacaoI']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo da fase de amortização 2 (em meses)</td>
		<td>
			<?=$d->dado['prazoamortizacaoII']?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de inicio da fase de amortização 2 </td>
		<td>
			<?=$d->dado['dtiniamortizacaoII']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo total do contrato (em meses)</td>
		<td>
			<?=$d->dado['prazototal']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data vencimento do contrato </td>
		<td>
			<?=$d->dado['dtvenccontrato'];?>
		</td>
	</tr>
</table>
<br>
<br>
<center>
<div style="width: 95%">
<table cellspacing="1" cellpadding="0" border="0" width="100%" class="listagem">
<tbody>
<tr style="margin-left: 20px;" >
	<td align="center" class="title" width="100%" colspan="4" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Simulador de um financiamento</td>
</tr>

<tr style="margin-left: 20px;" >
	<td align="center" width="50%" colspan="2" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"><?=$d->dado['percentfinanc']; ?> da mensalidade financiada*</td>
	<td align="center" width="30%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"><?=$d->dado['percentfinanc']; ?></td>
	<td align="center" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"/>
</tr>

<tr style="margin-left: 20px;" >
	<td align="center" width="25%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"/>
	<td align="center" width="25%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">mensalidade</td>
	<td align="center" width="24%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" >Mensalidade Financiada pelo FIES</td>
	<td align="center" width="26%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Mensalidade não Financiada</td>
</tr>
<? 
for ($i=1; $i <= ($d->dado['qtdsemcurso'] - $d->dado['qtdsemconc']); $i++):
	$cor = ($cor == '') ? '#f7f7f7' : '';
?>
<tr style="margin-left: 20px;" bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor ?>';" onmouseover="this.bgColor='#ffffcc';">
	<td align="center" width="25%"><?=$i ?> semestre</td>
	<td align="center" width="25%"><?=number_format($d->dado['valmens'], 2, ',', '.'); ?></td>
	<td align="center" width="24%"><?=number_format($d->dado['valfinanciado'], 2, ',', '.'); ?></td>
	<td align="center" width="26%"><?=number_format($d->dado['valnaofinanciado'], 2, ',', '.'); ?></td>
</tr>
<?
endfor;
?>
</tbody></table>
</div>
</center>
<br>
<br>
<?
//// Calcula - Prestação na Fase de Amortização I
//if ($tipoestudante == 'Bolsista Prouni 50%' || $tipoestudante == 'Bolsista Complementar 25%'){
//	$percent 	 = (int) str_replace('%', '', $percentfinanc);
//	$percentTipo = substr($tipoestudante, (strlen($tipoestudante)-3), -1);				
//	if ( ($percent + $percentTipo) >= 100){
//		$valMens = ($mensalidade * 25 / 100);
//	}else{
//		$valMens = ( $mensalidade-($mensalidade * ($percent / 100))-($mensalidade * ($percentTipo / 100)) );
//	}
//}else{
//	$valMens = $naoFinanciado;
//}
//
//// Prepara valores para gerar tabela de fases
//$dt = explode("/", $dtConcessao);
//$dataVenc = date("d/m/Y", mktime(0, 0, 0, ($dt[1] + 1), $dt[0], $dt[2]));
////$compMes = $Cdata->diferencaEntreDatas($dtBen, $dtConcessao, 'tempoEntreDadas', 'array', 'dd/mm/yyyy');
//
//
//$dados = array(
//				"taxa" 	  	=> str_replace(array("%", ","), array("", "."), $taxajuros),
//				"compMes" 	=> $compMes['mes'] + 1,
//				"dataVenc"  => $dataVenc,
//				"valFin"	=> $financiado,
//				"qtdSem"	=> ($qtdsemcurso - $qtdsemconc),
//				"valAmortI" => $valMens,
//				"prazoAmortizacaoII" => $prazoamortizacaoII
//			   );
//// Gera tabela de fases, com outros dados pertinentes			   
////$return = tabelaCalculo($dados);

?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="SubTituloDireita"  width="65%">Prestação na Fase de Amortização I</td>
		<td>
		<? 
			echo number_format($d->dado['valamortizacaoI'], 2, ',', '.');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Saldo Devedor no Início da Fase de Amortização I</td>
		<td>
		<? 
			echo number_format($d->dado['saldodevedorini'], 2, ',', '.');
		?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Saldo Devedor no Fim da Fase de Amortização I</td>
		<td>
		<? 
			echo number_format($d->dado['saldodevedorfim'], 2, ',', '.');
		?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prestação na Fase de Amortização II</td>
		<td>
		<? 
			echo number_format($d->dado['prestacaoamortizacaoII'], 2, ',', '.');
		?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Saldo Devedor no Início da Fase de Amortização II</td>
		<td>
		<? 
			echo number_format($d->dado['saldodevedorfim'], 2, ',', '.');
		?>		
		</td>
	</tr>
</table>
<br>
<center>
<div class="textoAzul2" align="left" style="font-weight: normal; width: 95%">
De acordo com Lei 10.260, o valor máximo a ser cobrado na prestação de juros durante a fase de utilização é de R$ 50,00. O excedente será incorporado ao saldo devedor (base de cálculo do trimestre seguinte) 
</div>
</center>
<?=$d->dado['html'] ?>

</body>
</html>
<? 
class CalculoSimulador{

//	public $return;
	public $dado;
	
	public function CalculoSimulador(){
		$Cdata 		= new Data();
		$this->dado = $_POST;
		
		$taxa 	  			 = str_replace(array("%", ","), array("", "."), $this->dado['taxajuros']);
		$percentfinanc    	 = str_replace("%", "", $this->dado['percentfinanc']);
		$processoseletivosem = substr($this->dado['processoseletivo'], 0, 1);
		$processoseletivoano = substr($this->dado['processoseletivo'], -4); 
		
		$this->dado['valsemestre']	 		= ($this->dado['valmens'] * 6);
		$this->dado['prazocarencia'] 		= '06';
		$this->dado['quantsemfinanciado'] 	= ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']);
		$this->dado['prazoutilizacao'] 		= ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6;
		$this->dado['valfinseis'] 			= ($this->dado['valmens'] * 6) * ($percentfinanc / 100);
		$this->dado['valfintotal'] 			= ($this->dado['valmens'] * 6) * ($percentfinanc / 100) * ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']);
		$this->dado['dtinibeneficio'] 		= $this->dado['diavenc'] . '/' . ($processoseletivosem == 2 ? '07' : '01') . '/' . $processoseletivoano;
		
		$diaVencimento = (integer) $this->dado['diavenc'];
		$diaContrato   = (integer) substr($this->dado['dtasscontrato'], 0, 2);
		if ($diaVencimento < $diaContrato){
			$mesVenc 				   = (integer) substr($this->dado['dtasscontrato'], 3, 5);
			$this->dado['dtconcessao'] = $this->dado['diavenc'] . '/' . sprintf("%02d", $mesVenc +1) . substr($this->dado['dtasscontrato'], -5);
		}else{
			$this->dado['dtconcessao'] = $this->dado['diavenc'] . substr($this->dado['dtasscontrato'], -8);
		}
		
		$this->dado['prazoamortizacaoI'] 	= 12;
		$this->dado['dtiniamortizacaoI'] 	= date('d/m/Y', mktime (0, 0, 0, ( ($processoseletivosem == 2 ? 7 : 1) + ( ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6) + 6)  , $this->dado['diavenc'], $processoseletivoano));
		
		$mult = ($processoseletivoano > 2008 || ($processoseletivosem == 2 && $processoseletivoano == 2008 ) ) ? 2 : 1.5;
		$this->dado['prazoamortizacaoII'] 	= (($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6) * $mult;;
		$this->dado['dtiniamortizacaoII'] 	= date('d/m/Y', mktime (0, 0, 0, ( ($processoseletivosem == 2 ? 7 : 1) + ( ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6) + 6)  , $this->dado['diavenc'], $processoseletivoano + 1));
		
		$this->dado['prazototal'] 			= (($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6) + (6) + (12) + (($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6 * 2);

		$carencia = ($processoseletivoano > 2008 || ($processoseletivosem == 2 && $processoseletivoano == 2008 ) ) ? 6 : 0;
		$dia = $this->dado['diavenc'];
		$mes = ( ($processoseletivosem == 2 ? 7 : 1) + ( ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6)/* + 6*/) + (($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']) * 6 * 2) + ($carencia);
		$ano = $processoseletivoano + 1;
		$this->dado['dtvenccontrato'] = date('d/m/Y', mktime (0, 0, 0, $mes, $dia, $ano));
			
		
		$this->dado['valfinanciado'] 	= $this->dado['valmens'] * ($percentfinanc / 100);
		$this->dado['valnaofinanciado'] = $this->dado['valmens'] - $this->dado['valfinanciado'];
		
		// Calcula - Prestação na Fase de Amortização I
		if ($this->dado['tipoestudante'] == 'Bolsista Prouni 50%' || $this->dado['tipoestudante'] == 'Bolsista Complementar 25%'){
//			$percent 	 = (int) str_replace('%', '', $percentfinanc);
			$percentTipo = substr($this->dado['tipoestudante'], (strlen($this->dado['tipoestudante'])-3), -1);				
			if ( ($percentfinanc + $percentTipo) >= 100){
				$this->dado['valamortizacaoI'] = ($this->dado['valmens'] * 25 / 100);
			}else{
				$this->dado['valamortizacaoI'] = ( $this->dado['valmens']-($this->dado['valmens'] * ($percentfinanc / 100))-($this->dado['valmens'] * ($percentTipo / 100)) );
			}
		}else{
			$this->dado['valamortizacaoI'] = $this->dado['valnaofinanciado'];
		}

		$dt 	  = explode("/", $this->dado['dtconcessao']);
		$dataVenc = date("d/m/Y", mktime(0, 0, 0, ($dt[1] + 1), $dt[0], $dt[2]));
		
		$compMes  = $Cdata->diferencaEntreDatas($this->dado['dtinibeneficio'], $this->dado['dtconcessao'], 'tempoEntreDadas', 'array', 'dd/mm/yyyy');
		$param 	  = array(
							"taxa" 	  			 => $taxa,
							"compMes" 			 => $compMes['mes'] + 1,
							"dataVenc"  		 => $dataVenc,
							"valFin"			 => $this->dado['valfinanciado'],
							"qtdSem"			 => ($this->dado['qtdsemcurso'] - $this->dado['qtdsemconc']),
							"valAmortI" 		 => $this->dado['valamortizacaoI'],
							"prazoamortizacaoII" => $this->dado['prazoamortizacaoII']
						 );		
		$result = $this->tabelaCalculoFases( $param );
		$this->dado['html'] 		   		  = $result['html'];
		$this->dado['saldodevedorini'] 		  = $result['saldoDevedorIni'];
		$this->dado['saldodevedorfim'] 		  = $result['saldoDevedorFim'];
		$this->dado['prestacaoamortizacaoII'] = $result['prestacaoAmortizacaoII'];
		
//		return $this->return;
//		return $this->dado;
	}
	
	private function tabelaCalculoFases(Array $dados){
		$tx 	  = (pow((1+($dados['taxa'] / 100)), 1/12)-1) /* * 100*/;
//		$tx = $tx / 100;
		$parcela  = $dados['valFin'];
		$qtdSem	  = $dados['qtdSem'];
//		die($parcela);
		$htm = '<center>
				<table cellspacing="0" cellpadding="0" border="0" width="100%" class="tabela">
					<tbody><tr style="margin-left: 20px;">
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Fase</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Saldo Anterior(R$)</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Juros(R$)</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Prestacao calculada(R$)</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Prestacao cobrada(R$)</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Liberação de parcela(R$)</td>
						<td align="center" nowrap="nowrap" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Saldo Atual(R$)</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Número prestacao</td>
						<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Data vencimento</td>
					</tr>';
		
		$continua 	 = true;
		$saldoAnt 	 = str_replace(array(".", ","), array("", "."), $parcela * $dados['compMes']);
		$calcPrestII = true;
		while ($continua):
			$row++;
			if ( $row <= (($qtdSem * 6) - $dados['compMes']) ){
				$fase = 'Utilização';
			}elseif ( $row <= (($qtdSem * 6) - $dados['compMes'] + 6) ){
				$fase = 'Carência';
				$parcela  = 0;
			}elseif ( $row <= (($qtdSem * 6) - $dados['compMes'] + 6 + 12) ){
				$fase = 'Amortização I';
			}elseif ( $row < (($qtdSem * 6) - $dados['compMes'] + 6 + 12 + $dados['prazoamortizacaoII']) ){
				$fase = 'Amortização II';
	//			$prestacao
			}else{
				$continua = false;
			}
			
			$color = is_int($row / 2) ? '#f7f7f7' : '';
//			die( round($tx, 5) );
//			$tx = round($tx, 9);
			$juros	  = round(($saldoAnt * $tx) /* / 100*/, 2);
//			$juros	  = round(($saldoAnt * $tx) /* / 100*/, 2);
//			die( $tx );
			
			if ( !isset($dataVenc) ){
				$dataVenc = $dados['dataVenc'];
			}else{
				$dt = explode("/", $dataVenc);
				$dataVenc = date("d/m/Y", mktime(0, 0, 0, ($dt[1] + 1), $dt[0], $dt[2]));
			}
			
			$arrMesPrest = array('03', '06', '09', '12');
			list(, $mesPrest) = explode("/", $dataVenc);
			
			if ($fase == 'Amortização I'){
				$numPrest++;
				$numero 	   			   = $numPrest;	
				$prestCalcExib 			   = $dados['valAmortI']; 
				$prestCobExib  			   = $dados['valAmortI'];
				$return['saldoDevedorIni'] = !isset($return['saldoDevedorIni']) ? $saldoAnt : $return['saldoDevedorIni']; 
			}elseif( $fase == 'Amortização II' ){
				$numPrest++;
				$numero = $numPrest;	
				if ( $calcPrestII ){
					$prestCalcExib = bcmul($saldoAnt, ( bcmul($tx, bcpow(1 + $tx, $dados['prazoamortizacaoII'], 10), 10) / ( bcpow(1 + $tx, $dados['prazoamortizacaoII'], 10) - 1 ) ), 20); 
					$prestCobExib  = $prestCalcExib;
					$calcPrestII   = false;
					$return['prestacaoAmortizacaoII'] = $prestCalcExib; 
//					echo $prestCalcExib;
				}
			}else{
				$prestCalc = $prestCalc + $juros; 
				$prestCob  = ($prestCob + $juros > 50) ? 50 : ($prestCob + $juros); 
		
				if (in_array($mesPrest, $arrMesPrest)){
					$numPrest++;
					$numero 	   = $numPrest;	
					$prestCalcExib = $prestCalc;
					$prestCobExib  = $prestCob;
					$prestCalc 	   = 0; 
					$prestCob  	   = 0; 
				}else{
					$prestCalcExib = 0;
					$prestCobExib  = 0;
					$numero 	   = '-';				
				}
			}
			$saldoAtual = round(($saldoAnt + $juros + $parcela) - $prestCobExib, 2);
			$htm .= '<tr style="margin-left: 20px;" bgcolor="' . $color . '" onmouseout="this.bgColor=\'' . $color . '\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td nowrap="" align="center" width="25%">' . $fase . '</td>
						<td align="right">' . number_format($saldoAnt, 2, ',', '.') . '</td>
					    <td align="right">' . number_format($juros, 2, ',', '.') . '</td>
					    <td align="right">' . number_format($prestCalcExib, 2, ',', '.') . '</td>
					    <td align="right">' . number_format($prestCobExib, 2, ',', '.') . '</td>
					    <td align="right">' . $parcela . '</td>
					    <td align="right">' . number_format($saldoAtual, 2, ',', '.') . '</td>
					    <td align="right">' . $numero . '</td>
					    <td align="right">' . $dataVenc . '</td>
					</tr>';
			if ( $fase == 'Amortização I' ){
				$return['saldoDevedorFim'] = $saldoAtual;
			}
			$saldoAnt = $saldoAtual;
		endwhile;
		$htm .= '</table></center>';
	
		$return['html'] = $htm;
		return $return;	
	}	

}


//function tabelaCalculo(Array $dados){
//	$tx 	  = (pow((1+($dados['taxa'] / 100)), 1/12)-1) * 100;
//	$parcela  = $dados['valFin'];
//	$qtdSem	  = $dados['qtdSem'];
//	
//	$htm = '<center>
//			<table cellspacing="0" cellpadding="0" border="0" width="100%" class="tabela">
//				<tbody><tr style="margin-left: 20px;">
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Fase</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Saldo Anterior(R$)</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Juros(R$)</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Prestacao calculada(R$)</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Prestacao cobrada(R$)</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Liberação de parcela(R$)</td>
//					<td align="center" nowrap="nowrap" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Saldo Atual(R$)</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Número prestacao</td>
//					<td align="center" width="10%" bgcolor="#E3E3E3" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Data vencimento</td>
//				</tr>';
//	
//	$continua 	 = true;
//	$saldoAnt 	 = str_replace(array(".", ","), array("", "."), $parcela * $dados['compMes']);
//	$calcPrestII = true;
//	while ($continua):
//		$row++;
//		if ( $row <= (($qtdSem * 6) - $dados['compMes']) ){
//			$fase = 'Utilização';
//		}elseif ( $row <= (($qtdSem * 6) - $dados['compMes'] + 6) ){
//			$fase = 'Carência';
//			$parcela  = 0;
//		}elseif ( $row <= (($qtdSem * 6) - $dados['compMes'] + 6 + 12) ){
//			$fase = 'Amortização I';
//		}elseif ( $row < (($qtdSem * 6) - $dados['compMes'] + 6 + 12 + $dados['prazoamortizacaoII']) ){
//			$fase = 'Amortização II';
////			$prestacao
//		}else{
//			$continua = false;
//		}
//		
//		$color = is_int($row / 2) ? '#f7f7f7' : '';
//		$juros	  = round(($saldoAnt * $tx) / 100, 2);
//		
//		if ( !isset($dataVenc) ){
//			$dataVenc = $dados['dataVenc'];
//		}else{
//			$dt = explode("/", $dataVenc);
//			$dataVenc = date("d/m/Y", mktime(0, 0, 0, ($dt[1] + 1), $dt[0], $dt[2]));
//		}
//		
//		$arrMesPrest = array('03', '06', '09', '12');
//		list(, $mesPrest) = explode("/", $dataVenc);
//		
//		if ($fase == 'Amortização I'){
//			$numPrest++;
//			$numero 	   			   = $numPrest;	
//			$prestCalcExib 			   = $dados['valAmortI']; 
//			$prestCobExib  			   = $dados['valAmortI'];
//			$return['saldoDevedorIni'] = !isset($return['saldoDevedorIni']) ? $saldoAnt : $return['saldoDevedorIni']; 
//		}elseif( $fase == 'Amortização II' ){
//			$numPrest++;
//			$numero = $numPrest;	
//			if ( $calcPrestII ){
////				$prestCalcExib = $saldoAnt . ( ( $tx . bcpow(1 + $tx, $dados['prazoamortizacaoII']) ) / ( bcpow(1 + $tx, $dados['prazoamortizacaoII']) - 1 ) ) ; 
//				$prestCalcExib = bcmul($saldoAnt, ( bcmul($tx, bcpow(1 + $tx, $dados['prazoamortizacaoII'])) / ( bcpow(1 + $tx, $dados['prazoamortizacaoII']) - 1 ) )); 
//				$prestCobExib  = $prestCalcExib;
//				$calcPrestII   = false;
//			}
//		}else{
//			$prestCalc = $prestCalc + $juros; 
//			$prestCob  = ($prestCob + $juros > 50) ? 50 : ($prestCob + $juros); 
//	
//			if (in_array($mesPrest, $arrMesPrest)){
//				$numPrest++;
//				$numero 	   = $numPrest;	
//				$prestCalcExib = $prestCalc;
//				$prestCobExib  = $prestCob;
//				$prestCalc 	   = 0; 
//				$prestCob  	   = 0; 
//			}else{
//				$prestCalcExib = 0;
//				$prestCobExib  = 0;
//				$numero 	   = '-';				
//			}
//		}
//		$saldoAtual = ($saldoAnt + $juros + $parcela) - $prestCobExib;
//		$htm .= '<tr style="margin-left: 20px;" bgcolor="' . $color . '" onmouseout="this.bgColor=\'' . $color . '\';" onmouseover="this.bgColor=\'#ffffcc\';">
//					<td nowrap="" align="center" width="25%">' . $fase . '</td>
//					<td align="right">' . number_format($saldoAnt, 2, ',', '.') . '</td>
//				    <td align="right">' . number_format($juros, 2, ',', '.') . '</td>
//				    <td align="right">' . number_format($prestCalcExib, 2, ',', '.') . '</td>
//				    <td align="right">' . number_format($prestCobExib, 2, ',', '.') . '</td>
//				    <td align="right">' . $parcela . '</td>
//				    <td align="right">' . number_format($saldoAtual, 2, ',', '.') . '</td>
//				    <td align="right">' . $numero . '</td>
//				    <td align="right">' . $dataVenc . '</td>
//				</tr>';
//		if ( $fase == 'Amortização I' ){
//			$return['saldoDevedorFim'] = $saldoAtual;
//		}
//		$saldoAnt = $saldoAtual;
//	endwhile;
//	$htm .= '</table></center>';
//
//	$return['html'] = $htm;
//		
//	return $return;	
//}
?>