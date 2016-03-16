<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class InvestimentoView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

	function listar(){
		$this->model->ListarInvestimento();
		$this->output.='<BR>';
		$this->output.= '<div id="listar">';
		$l = $this->model->getInvestimento();
		$totalizador = $this->model->getTotalInvestimento();
		$totalcreditos = $this->model->getTotalCreditos();
		$construcao = $this->model->getConstrucao();
		$this->output.= '
		<form method="post" action="">
		<table class="lista" width="1%" align=center>
		<tr>
		<th class="left" colspan="6" width="1%">Os dados desta tabela não são cumulativos, portanto você deve informar os valores de investimento em cada ano.</th>
		</tr>
		<tr>
		<th class="center" width="1%">Investimentos (R$)</th>
		<th class="center" width="1%">2008</th>
		<th class="center" width="1%">2009</th>
		<th class="center" width="1%">2010</th>
		<th class="center" width="1%">2011</th>
		<th class="center" width="1%">Total</th>
		</tr>';
		foreach ($l as $investimento) {
			$cod = $investimento['co_orcamento'];
			$this->output.='<tr>';
			$this->output.= '<th class="valoresitens" nowrap>'.$investimento['ds_orcamento'].'</th>';
			$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Inv2008['.$cod.']" value='.number_format($investimento['vl_total_2008'],2,',','.').'></td>';
			$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Inv2009['.$cod.']" value='.number_format($investimento['vl_total_2009'],2,',','.').'></td>';
			$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Inv2010['.$cod.']" value='.number_format($investimento['vl_total_2010'],2,',','.').'></td>';
			$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Inv2011['.$cod.']" value='.number_format($investimento['vl_total_2011'],2,',','.').'></td>';
			$this->output.= '<th class="valoresitenstotalr" align="right">'.number_format($investimento['vl_total_2008']+$investimento['vl_total_2009']+$investimento['vl_total_2010']+$investimento['vl_total_2011'],2,',','.').'</th>';
			$this->output.='</tr>';
		}
		$tinv = $totalizador['2008']+$totalizador['2009']+$totalizador['2010']+$totalizador['2011'];
		$tcre = $totalcreditos['vl_total_2008']+$totalcreditos['vl_total_2009']+$totalcreditos['vl_total_2010']+$totalcreditos['vl_total_2011'];
		$D2008 = $totalcreditos['vl_total_2008']-$totalizador['2008'];
		$D2009 = $totalcreditos['vl_total_2009']-$totalizador['2009'];
		$D2010 = $totalcreditos['vl_total_2010']-$totalizador['2010'];
		$D2011 = $totalcreditos['vl_total_2011']-$totalizador['2011'];
		$DTOTAL = $tcre-$tinv;
		if ($D2008<0) $class2008 = 'valoresitenstotalred'; else $class2008 = 'valoresitenstotalr';
		if ($D2009<0) $class2009 = 'valoresitenstotalred'; else $class2009 = 'valoresitenstotalr';
		if ($D2010<0) $class2010 = 'valoresitenstotalred'; else $class2010 = 'valoresitenstotalr';
		if ($D2011<0) $class2011 = 'valoresitenstotalred'; else $class2011 = 'valoresitenstotalr';
		if ($DTOTAL<0) $classtotal = 'valoresitenstotalred'; else $classtotal = 'valoresitenstotalr';
		$this->output.= '
		<tr>
		<th class="valoresitenstotal" nowrap>Total em Investimentos</th>
		<th class="valoresitenstotalr">'.number_format($totalizador['2008'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($totalizador['2009'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($totalizador['2010'],2,',','.').'</th>
		<th class="valoresitenstotalr"">'.number_format($totalizador['2011'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($tinv,2,',','.').'</th>
		</tr>
		<tr>
		<th class="valoresitenstotal" nowrap>Total de Créditos em Investimentos</th>
		<th class="valoresitenstotalr">'.number_format($totalcreditos['vl_total_2008'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($totalcreditos['vl_total_2009'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($totalcreditos['vl_total_2010'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($totalcreditos['vl_total_2011'],2,',','.').'</th>
		<th class="valoresitenstotalr">'.number_format($tcre,2,',','.').'</th>
		</tr>
		<tr>
		<th class="valoresitenstotal" nowrap>Diferença</th>
		<th nowrap class="'.$class2008.'">'.number_format($D2008,2,',','.').'</th>
		<th nowrap class="'.$class2009.'">'.number_format($D2009,2,',','.').'</th>
		<th nowrap class="'.$class2010.'">'.number_format($D2010,2,',','.').'</th>
		<th nowrap class="'.$class2011.'">'.number_format($D2011,2,',','.').'</th>
		<th nowrap class="'.$classtotal.'">'.number_format($DTOTAL,2,',','.').'</th>
		</tr>
		<tr><th class="esp">&nbsp;</th></tr>
		<tr><th class="esp">&nbsp;</th></tr>
		<tr>
		<th class="center" width="1%">Construções (m&#178)</th>
		<th class="center" width="1%">2008</th>
		<th class="center" width="1%">2009</th>
		<th class="center" width="1%">2010</th>
		<th class="center" width="1%">2011</th>
		<th class="center" width="1%">Total</th>
		</tr>
		<tr>';
		$this->output.= '<th class="valoresitens" nowrap>Previsão de área a ser construída/Edificações</th>';
		$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Construcao[2008]" value='.number_format($construcao['vl_total_2008'],2,',','.').'></td>';
		$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Construcao[2009]" value='.number_format($construcao['vl_total_2009'],2,',','.').'></td>';
		$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Construcao[2010]" value='.number_format($construcao['vl_total_2010'],2,',','.').'></td>';
		$this->output.= '<td class="valores" width="1%" onclick="MouseClick3(this);"><input autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###,##\',this.value);" class="valores" onfocus="MouseClick2(this);" onblur="MouseBlur2(this);" type="text" size=12  name="Construcao[2011]" value='.number_format($construcao['vl_total_2011'],2,',','.').'></td>';
		$this->output.= '<th class="valoresitenstotalr" align="right">'.number_format($construcao['vl_total_2008']+$construcao['vl_total_2009']+$construcao['vl_total_2010']+$construcao['vl_total_2011'],2,',','.').'</th>';
		$this->output.='</tr>
		</table>
		<BR>
		<center><input type="submit" class="formbutton" name="Salvar" value="Recalcular/Salvar"/></center>
		<BR>
		</form>';
		$this->output.= '</div>';
		$this->output.='<BR>';
	}

    function display() {
    	$this->menu='investimento';
        $this->listar();
        parent::display();
    }

}

?>
