<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class CusteioView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

	function listar(){
		$this->model->ListarCusteio();
		$this->output.='<BR>';
		$this->output.= '<div id="listar">';
		$l = $this->model->getCusteio();
		$totalizador = $this->model->getTotalCusteio();
		$totalcreditos = $this->model->getTotalCreditos();
		$totalpeqexpansao = $this->model->getTotalPeqExpansao();
		$this->output.= '
		<form method="post" action="">
		<table class="lista" width="800" align=center>
		<tr>
		<th class="left" colspan="12" width="1%">Os dados desta tabela são cumulativos, portanto se você vai necessitar, por exemplo, de 10 professores a partir de 2009, você deve inclui-los também em 2010, 2011 e 2012.</th>
		</tr>
		<tr>
		<th class="center" colspan="2" width="1%">Item Custeio</th>
		<th class="center" colspan="2">2008</th>
		<th class="center" colspan="2">2009</th>
		<th class="center" colspan="2">2010</th>
		<th class="center" colspan="2">2011</th>
		<th class="center" colspan="2">2012</th>
		</tr>
		<tr>
		<th class="center">Descrição</th>
		<th class="center">R$</th>
		<th class="center">Qtde</th>
		<th class="center">R$</th>
		<th class="center">Qtde</th>
		<th class="center">R$</th>
		<th class="center">Qtde</th>
		<th class="center">R$</th>
		<th class="center">Qtde</th>
		<th class="center">R$</th>
		<th class="center">Qtde</th>
		<th class="center">R$</th>
		</tr>';
		if ($l) {
			foreach ($l as $custeio) {
				$cod = $custeio['co_orcamento'];
				if ($cod<>9) {
					$mascara = "'[###.]###'";
					$decimais = 0;
				} else {
					$mascara = "'[###.]###,##'";
					$decimais = 2;
				}
				$this->output.='<tr>';
				$this->output.= '<th class="valoresitens" nowrap>'.$custeio['ds_orcamento'].'</th>';
				$this->output.= '<td align="right">'.number_format($custeio['vl_unidade'],2,',','.').'</td>';
				$this->output.= '<td class="valores" onclick="MouseClick3(this);" width="1%"><input autocomplete="off" onkeyup="this.value = mascaraglobal('.$mascara.',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size="4" name="Inv2008['.$cod.']" value='.number_format($custeio['unidades_2008'],$decimais,',','.').'></td>';
				$this->output.= '<td align="right">'.number_format($custeio['vl_total_2008'],2,',','.').'</td>';
				$this->output.= '<td class="valores" onclick="MouseClick3(this);" width="1%"><input autocomplete="off" onkeyup="this.value = mascaraglobal('.$mascara.',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size="4" name="Inv2009['.$cod.']" value='.number_format($custeio['unidades_2009'],$decimais,',','.').'></td>';
				$this->output.= '<td align="right">'.number_format($custeio['vl_total_2009'],2,',','.').'</td>';
				$this->output.= '<td class="valores" onclick="MouseClick3(this);" width="1%"><input autocomplete="off" onkeyup="this.value = mascaraglobal('.$mascara.',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size="4" name="Inv2010['.$cod.']" value='.number_format($custeio['unidades_2010'],$decimais,',','.').'></td>';
				$this->output.= '<td align="right">'.number_format($custeio['vl_total_2010'],2,',','.').'</td>';
				$this->output.= '<td class="valores" onclick="MouseClick3(this);" width="1%"><input autocomplete="off" onkeyup="this.value = mascaraglobal('.$mascara.',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size="4" name="Inv2011['.$cod.']" value='.number_format($custeio['unidades_2011'],$decimais,',','.').'></td>';
				$this->output.= '<td align="right">'.number_format($custeio['vl_total_2011'],2,',','.').'</td>';
				$this->output.= '<td class="valores" onclick="MouseClick3(this);" width="1%"><input autocomplete="off" onkeyup="this.value = mascaraglobal('.$mascara.',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size="4" name="Inv2012['.$cod.']" value='.number_format($custeio['unidades_2012'],$decimais,',','.').'></td>';
				$this->output.= '<td align="right">'.number_format($custeio['vl_total_2012'],2,',','.').'</td>';
				$this->output.='</tr>';
			}
		}
		$tcus = $totalizador['2008']+$totalizador['2009']+$totalizador['2010']+$totalizador['2011']+$totalizador['2012'];
		$tcre = $totalcreditos['vl_total_2008']+$totalcreditos['vl_total_2009']+$totalcreditos['vl_total_2010']+$totalcreditos['vl_total_2011']+$totalcreditos['vl_total_2012'];
		$tpee = $totalpeqexpansao['vl_total_2008']+$totalpeqexpansao['vl_total_2009']+$totalpeqexpansao['vl_total_2010']+$totalpeqexpansao['vl_total_2011']+$totalpeqexpansao['vl_total_2012'];
		$this->output.= '
		<tr>
		<th class="valoresitenstotal" nowrap colspan="2">Total em Custeio (R$)</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalizador['2008'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalizador['2009'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalizador['2010'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalizador['2011'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalizador['2012'],2,',','.').'</th>
		</tr>
		<tr>
		<th class="valoresitenstotal" nowrap colspan="2">Total de Créditos em Custeio (R$)</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalcreditos['vl_total_2008'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalcreditos['vl_total_2009'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalcreditos['vl_total_2010'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalcreditos['vl_total_2011'],2,',','.').'</th>
		<th class="valoresitenstotalr" colspan="2">'.number_format($totalcreditos['vl_total_2012'],2,',','.').'</th>
		</tr>
		<tr>
		<th class="valoresitenstotal" nowrap colspan="2">Diferença (R$)</th>';
		$D2008 = $totalcreditos['vl_total_2008']-$totalizador['2008'];
		$D2009 = $totalcreditos['vl_total_2009']-$totalizador['2009'];
		$D2010 = $totalcreditos['vl_total_2010']-$totalizador['2010'];
		$D2011 = $totalcreditos['vl_total_2011']-$totalizador['2011'];
		$D2012 = $totalcreditos['vl_total_2012']-$totalizador['2012'];
		$DTOTAL = $tcre-$tcus;
		if ($D2008<0) $class2008 = 'valoresitenstotalred'; else $class2008 = 'valoresitenstotalr';
		if ($D2009<0) $class2009 = 'valoresitenstotalred'; else $class2009 = 'valoresitenstotalr';
		if ($D2010<0) $class2010 = 'valoresitenstotalred'; else $class2010 = 'valoresitenstotalr';
		if ($D2011<0) $class2011 = 'valoresitenstotalred'; else $class2011 = 'valoresitenstotalr';
		if ($D2012<0) $class2012 = 'valoresitenstotalred'; else $class2012 = 'valoresitenstotalr';
		if ($DTOTAL<0) $classtotal = 'valoresitenstotalred'; else $classtotal = 'valoresitenstotalr';
		$this->output.= '
		<th nowrap class="'.$class2008.'" colspan="2">'.number_format($D2008,2,',','.').'</th>
		<th nowrap class="'.$class2009.'" colspan="2">'.number_format($D2009,2,',','.').'</th>
		<th nowrap class="'.$class2010.'" colspan="2">'.number_format($D2010,2,',','.').'</th>
		<th nowrap class="'.$class2011.'" colspan="2">'.number_format($D2011,2,',','.').'</th>
		<th nowrap class="'.$class2012.'" colspan="2">'.number_format($D2012,2,',','.').'</th>
		</tr>
		<tr><th colspan ="12" class="esp">&nbsp;</th></tr>
		<tr><th colspan ="12" class="esp">&nbsp;</th></tr>
		<tr>
		<th class="center" colspan="2" width="1%">Total = Atual + Expansão + REUNI</th>
		<th class="center" colspan="2" width="1%">2008</th>
		<th class="center" colspan="2" width="1%">2009</th>
		<th class="center" colspan="2" width="1%">2010</th>
		<th class="center" colspan="2" width="1%">2011</th>
		<th class="center" colspan="2" width="1%">2012</th>
		</tr>
		<tr>';
		$this->output.= '<th class="valoresitens" colspan="2" nowrap>Professores-Equivalente</th>';
		$this->output.= '<td align="right" colspan="2">'.number_format($totalpeqexpansao['vl_peq_2008'],2,',','.').'</td>';
		$this->output.= '<td align="right" colspan="2">'.number_format($totalpeqexpansao['vl_peq_2009'],2,',','.').'</td>';
		$this->output.= '<td align="right" colspan="2">'.number_format($totalpeqexpansao['vl_peq_2010'],2,',','.').'</td>';
		$this->output.= '<td align="right" colspan="2">'.number_format($totalpeqexpansao['vl_peq_2011'],2,',','.').'</td>';
		$this->output.= '<td align="right" colspan="2">'.number_format($totalpeqexpansao['vl_peq_2012'],2,',','.').'</td>';
		$this->output.='</tr>
		</table>
		<BR>
		<center><input type="submit" class="formbutton" name="Salvar" value="Recalcular/Salvar"/></center>
		<BR>
		</form>';
		$this->output.= '</div>
		<BR>';
	}

    function display() {
        $this->menu='custeio';
        $this->listar();
        parent::display();
    }

}

?>
