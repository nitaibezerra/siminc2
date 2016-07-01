<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class View {
	protected $menu;
    protected $model;
    protected $output;

    public function __construct($model) {
        $this->model = $model;
    }

    private function header() {
        $output = '
		<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
		<html lang="pt_br">
		<head>
			<link rel="stylesheet" href="reuni.css">
			<script language="JavaScript" src="functions.js"></script>
			<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"><title></title>
		</head>
		<body>
		<div id="header">
			<table width="100%" height="25px" border="0" cellpadding="0" cellspacing="0">
  				<tr bgcolor="#ffcc00">
    				<td align="left"><img src="imagens/logo_mec_br.gif"></td>
  				</tr>
			</table>
			<table width="100%" height="25px" border="0" cellpadding="0" cellspacing="0">
  				<tr>
   					<td class="sistema">Programa de Apoio a Planos de Reestruturação e Expansão das Universidades Federais - REUNI</td>
  				</tr>
			</table>
		</div>
		';
		return $output;
    }

    private function footer() {
    	if ($this->model->instituicao) {
        	$output= '<div id="footer"><table width="100%"><tr><td width="50%" align="left">Usuário: '.$this->model->nome.'</td><td width="50%" align="right">MEC/SESu/DeDES - REUNI</td></tr></table></div>';
    	} else {
        	$output= '<div id="footer"><table width="100%"><tr><td width="50%" align="left"></td><td width="50%" align="right">MEC/SESu/DeDES - REUNI</td></tr></table></div>';
    	}
  		$output.= '</body></html>';
  		return $output;
    }

    private function displayMenu ($item) {
		$menu[0] = '';
		$menu[1] = '';
		$menu[2] = '';
		$menu[3] = '';
		$menu[4] = '';
		$menu[5] = '';
		$menu[6] = '';
    	switch ($item) {
    		case "unidades":
        		$menu[0] = 'class=selected';
        		break;
    		case "graduacao":
	        	$menu[1] = 'class=selected';
    	    	break;
    		case "pos_graduacao":
	        	$menu[2] = 'class=selected';
    	    	break;
    		case "custeio":
		        $menu[3] = 'class=selected';
    	    	break;
    		case "investimento":
		        $menu[4] = 'class=selected';
    	    	break;
	    	case "planilhas":
		        $menu[5] = 'class=selected';
        		break;
    		}
        $output =
		'<div id="menu">
			<ul id="nav">
				<li><a '.$menu[0].' href="index.php?view=unidades">Unidades Acadêmicas</a></li>
				<li><a '.$menu[1].' href="index.php?view=graduacao">Graduação</a></li>
				<li><a '.$menu[2].' href="index.php?view=pos_graduacao">Pós-Graduação</a></li>
				<li><a '.$menu[3].' href="index.php?view=custeio">Custeio</a></li>
				<li><a '.$menu[4].' href="index.php?view=investimento">Investimento</a></li>
				<li><a '.$menu[5].' href="index.php?view=planilhas">Planilhas</a></li>
				<li><a href="javascript:self.close();">Sair</a></li>
			</ul>
		</div>';
		return $output;
    }

	protected function display() {
//		if (substr_count($this->model->accept_encoding,'gzip'))
//			ob_start("ob_gzhandler");
//		else
//			ob_start();
		header("Pragma: no-cache");
		echo $this->header();
		if ($this->menu) echo $this->displayMenu($this->menu);
		echo $this->output;
		echo $this->footer();
//		ob_end_flush();
	}

}

?>