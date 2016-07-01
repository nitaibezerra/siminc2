<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class InvestimentoController extends Controller {

    public function __construct($dao) {
 		parent::__construct(new InvestimentoModel($dao));
 		if ($_POST['Salvar']) {
			if ($_POST["Construcao"]) {
				while($a = each($_POST["Construcao"]))
	  			{
	  				$valor = str_replace(".", "", $a['value']);
	  				$valor = str_replace(",", ".",$valor);
					if (empty($valor))  $valor=0;
	  				$this->model->AtualizarConstrucao($a['key'],$valor);
	  			}
			}
 			if ($_POST["Inv2008"]) {
				while($a = each($_POST["Inv2008"]))
	  			{
	  				$valor = str_replace(".", "", $a['value']);
	  				$valor = str_replace(",", ".",$valor);
					if (empty($valor))  $valor=0;
	  				$this->model->AtualizarInvestimento($a['key'],2008,$valor);
	  			}
 			}
 			if ($_POST["Inv2009"]) {
				while($a = each($_POST["Inv2009"]))
	  			{
	  				$valor = str_replace(".", "", $a['value']);
	  				$valor = str_replace(",", ".",$valor);
					if (empty($valor))  $valor=0;
	  				$this->model->AtualizarInvestimento($a['key'],2009,$valor);
	  			}
 			}
 			if ($_POST["Inv2010"]) {
				while($a = each($_POST["Inv2010"]))
	  			{
	  				$valor = str_replace(".", "", $a['value']);
	  				$valor = str_replace(",", ".",$valor);
					if (empty($valor))  $valor=0;
	  				$this->model->AtualizarInvestimento($a['key'],2010,$valor);
	  			}
    		}
 			if ($_POST["Inv2011"]) {
				while($a = each($_POST["Inv2011"]))
	  			{
	  				$valor = str_replace(".", "", $a['value']);
	  				$valor = str_replace(",", ".",$valor);
					if (empty($valor))  $valor=0;
	  				$this->model->AtualizarInvestimento($a['key'],2011,$valor);
	  			}
			}
		}
   		$this->view = new InvestimentoView($this->model);
    }

}
?>
