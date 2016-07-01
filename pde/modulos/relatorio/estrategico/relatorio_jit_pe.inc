<?php include_once APPRAIZ."pde/modulos/relatorio/estrategico/relatorio_win8_desafios.inc"; die; ?>
<?php 

$arrObjetivo = Array(
		'name' => '<div align="center" valign="midlle" ><br><b>'.utf8_encode('Eixo').'</b></div>',
		'id' => '_raiz',
		'data'=>Array('$color'=>'white'));


// Eixo
$sql_eixo = "SELECT
			e.exoid || '_eixo' as id,
			e.exodsc as name,
			'prg_' || prgid as linkplanilha
		FROM
			jit_pe_eixo e
		LEFT JOIN 
			monitora.jit_ppa_programa p on p.exoid = e.exoid AND p.exoid NOT IN (1)";

// Objetivo Estratégico
$sql_obe = "SELECT
			obe.obedsc as name,
			obe.obeid || '_obe_{id_pai}' as id,
			'obj_' || objid as linkplanilha
		FROM
			monitora.jit_pe_objetivoestrategico	obe
		LEFT JOIN 
			monitora.jit_ppa_objetivo obj ON obj.obeid = obe.obeid AND obj.objid NOT IN (7,8)
		WHERE 
			obestatus = 'A' 
		AND 
			{filtro}";

// Desafio Prioritario
$sql_dse = "SELECT
				dse.dsedsc as name,
				dse.dseid || '_dse_{id_pai}' as id,
				'met_' || metid as linkplanilha
			FROM
				monitora.jit_pe_desafioprioritario dse
			LEFT JOIN
				monitora.jit_pne_meta met ON met.dseid = dse.dseid
			WHERE
				dse.dsestatus = 'A'
			AND
				{filtro}";

// Acao
$sql_aca = "SELECT
				aca.acadsc as name,
				aca.acaid || '_aca_{id_pai}' as id,
				'met_' || metid as linkplanilha,
				acalinkpainelestrategico as linkpainel
			FROM
				monitora.jit_pe_acao aca
			LEFT JOIN
				monitora.jit_pne_meta met ON met.metid = aca.metidpne
			WHERE
				aca.acastatus = 'A'
			AND
				{filtro}";

// Detalhe da Acao
$sql_dtl = "SELECT
				aca.acadetalhe as name,
				aca.acaid || '_dtl_{id_pai}' as id
			FROM
				monitora.jit_pe_acao aca			
			WHERE
				aca.acastatus = 'A'
			AND
				{filtro}";

// Meta Acao
$sql_met = "SELECT
				aca.acameta2014 as name,
				aca.acaid || '_dtl_{id_pai}' as id,
				acalinkindicador as linkindicador
			FROM
				monitora.jit_pe_acao aca
			WHERE
				aca.acastatus = 'A'
			AND
				{filtro}";

// Aplica filtros
$stParams = '';
if($_REQUEST['exoid']){
	$sql_eixo .= " WHERE e.exoid = {$_REQUEST['exoid']} ";
	$stParams = "&exoid={$_REQUEST['exoid']}";
}else 
if($_REQUEST['obeid']){
	$sql_eixo .= " INNER JOIN monitora.jit_pe_objetivoestrategico obe ON e.exoid = obe.exoid AND obe.obeid = {$_REQUEST['obeid']} ";
	$sql_obe .= " AND obe.obeid = {$_REQUEST['obeid']} ";
	$stParams = "&obeid={$_REQUEST['obeid']}";
}

$arrSql = array('sql'	=> $sql_eixo,
				'cor' 	=> '08ff02',
				'filho' => array('sql' 		=>$sql_obe,
								 'cor' 		=> 'ffff00',
								 'filtro' 	=>'exoid',
								 'filho' 	=> Array('sql' 		=>$sql_dse,
													 'cor' 		=> 'ffff00',
													 'filtro' 	=>'obeid',
													 'filho' 	=> Array('sql' 		=>$sql_aca,
																		 'cor' 		=> 'ffff00',
																		 'filtro' 	=>'aca.dseid',
																		 'filho' 	=> Array('sql' 		=>$sql_dtl,
																							 'cor' 		=> 'ffff00',
																							 'filtro' 	=>'acaid',
																							 'filho' 	=> Array('sql' 		=>$sql_met,
																												 'cor' 		=> 'ffff00',
																												 'filtro' 	=>'acaid',
																												 'filho' 	=> Array()))
													 					)
								 					)
								)
				);

$arrObjetivo['children'] = montaArrayJit( $arrSql );

// include APPRAIZ . 'includes/cabecalho.inc';
// echo "<br/>";

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="X-UA-Compatible" content="IE=7" />
		<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
		<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>		
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/Jit/jit.js"></script>
		<link rel="stylesheet" type="text/css" href="css/jit_pe.css"/>
	</head>
	<body> 
		<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
			<tr>
				<td class="titulo_pagina" >
					<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
			            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" alt="" title="" />
			        	<div style="float:left" class="titulo_box" >SIMEC <br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
					</div>
					<div style="float:right;cursor:pointer;"  onclick="window.location='estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A';">
						<img src="../imagens/icones/icons/mais.png" style="vertical-align:middle;" alt="Mostrar Árvore Completa" title="Mostrar Árvore Completa" />
					</div>
				</td>
			</tr>
			<tr>
				<td>
					<?php
					$menu = array(0 => array("id" => 1, "descricao" => "PE",	"link" => "/pde/estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A{$stParams}"),
					1 => array("id" => 2, "descricao" => "PPA",	"link" => "/pde/estrategico.php?modulo=relatorio/estrategico/relatorio_jit_ppa&acao=A"),
					2 => array("id" => 2, "descricao" => "PNE",	"link" => "/pde/estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pne&acao=A"));
					
					echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
					
					$linha1 = 'Relatório PE';
					$linha2 = 'Planilha';
					monta_titulo($linha1, $linha2);
					?>	
					<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<tr class="ocultaLinha">
							<td class="SubTituloDireita" width="45%"></td>
							<td>
								<a id="update" href="#" class="theme button white">Nível Anterior</a>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Orientação:</td>
							<td>
								<?php 
								$sql = Array(Array('codigo'=>'v', 'descricao' => 'Vertical'),
											 Array('codigo'=>'h', 'descricao' => 'Horizontal'));
								?>
								<?=$db->monta_combo('orientation',$sql,'S','','','',$txtdica='',$size='','', 'orientation', $return = false, 'h' ) ?>
								<input type="button" value="Mostrar tudo" onclick="window.location='estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A'" style="float:right" />			
							</td>
						</tr>
						<tr class="ocultaLinha">
							<td class="SubTituloDireita">Numero de Níveis:</td>
							<td>
								<?php 
								$sql = Array(Array('codigo'=>'all', 'descricao' => 'Todos'),
											 Array('codigo'=>'1', 'descricao' => '1'),
											 Array('codigo'=>'2', 'descricao' => '2'),
											 Array('codigo'=>'3', 'descricao' => '3'),
											 Array('codigo'=>'4', 'descricao' => '4'),
											 Array('codigo'=>'5', 'descricao' => '5'))
								?>
								<?=$db->monta_combo('i-levels-to-show',$sql,'S','','','','','','', 'i-levels-to-show', $return = false, 4 ) ?>
							</td>
						</tr>	
						<tr>
							<td colspan="2" >
								<div id="container" >
								<div id="center-container">
								    <div id="infovis"></div>    
								</div>
								</div>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</body>
</html>
<script type="text/javascript">
		
var labelType, useGradients, nativeTextSupport, animate;

(function() {
  	var ua = navigator.userAgent,
      	iStuff = ua.match(/iPhone/i) || ua.match(/iPad/i),
      	typeOfCanvas = typeof HTMLCanvasElement,
      	nativeCanvasSupport = (typeOfCanvas == 'object' || typeOfCanvas == 'function'),
      	textSupport = nativeCanvasSupport 
       		&& (typeof document.createElement('canvas').getContext('2d').fillText == 'function');
	/* 
	 * I'm setting this based on the fact that ExCanvas provides text support for IE
  	 * and that as of today iPhone/iPad current text support is lame
  	 */
  	labelType = 'HTML';
  	nativeTextSupport = labelType == 'HTML';
  	useGradients = nativeCanvasSupport;
  	animate = !(iStuff || !nativeCanvasSupport);
})();

var Log = {
  elem: false,
  write: function(text){
    if (!this.elem) 
      this.elem = document.getElementById('log');
    this.elem.innerHTML = text;
    this.elem.style.left = (500 - this.elem.offsetWidth / 2) + 'px';
  }
};

var icicle;

function init(){
    // left panel controls
  	controls();
	//init data
	var json = <?=simec_json_encode($arrObjetivo) ?>
	// init Icicle
  	icicle = new $jit.Icicle({
    // id of the visualization container
    injectInto: 'infovis',
    // whether to add transition animations
    animate: animate,
    // nodes offset
    offset: 1,
    // whether to add cushion type nodes
    cushion: false,
    // do not show all levels at once
    constrained: true,
    levelsToShow: 4,
    // enable tips
    Tips: {
      enable: true,
      type: 'Native',
      // add positioning offsets
      offsetX: 20,
      offsetY: 20,
      // implement the onShow method to
      // add content to the tooltip when a node
      // is hovered
      onShow: function(tip, node){
        // count children
        var count = 0;
        node.eachSubnode(function(){
          count++;
        });
        // add tooltip info
        tip.innerHTML = "<div class=\"tip-title\"> "
            + node.name + "</div><div class=\"tip-text\">" + count
            + " filhos </div>";
      }
    },
    // Add events to nodes
    Events: {
      enable: true,
      onClick: function(node){
        if (node) {
          //hide tips
          icicle.tips.hide();
          // perform the enter animation
          icicle.enter(node);
        }
      },
      onRightClick: function(){
        //hide tips
        icicle.tips.hide();
        // perform the out animation
        icicle.out();
      }
    },
    // Add canvas label styling
    Label: {
      type: labelType, // "Native" or "HTML"
      color: '#333',
      style: 'bold',
      size: 12
    },
    // Add the name of the node in the corresponding label
    // This method is called once, on label creation and only for DOM and
    // not
    // Native labels.
    onCreateLabel: function(domElement, node){
      domElement.innerHTML = node.name;
      var style = domElement.style;
      style.fontSize = '0.9em';
      style.display = '';
      style.cursor = 'pointer';
      style.color = '#333';
      style.overflow = 'hidden';
    },
    // Change some label dom properties.
    // This method is called each time a label is plotted.
    onPlaceLabel: function(domElement, node){
      var style = domElement.style, width = node.getData('width'), height = node
          .getData('height');
      if (width < 7 || height < 7) {
        style.display = 'none';
      } else {
        style.display = '';
        style.width = width + 'px';
        style.height = height + 'px';
      }
    }
  });
  	// load data
  	icicle.loadJSON(json);
  	// compute positions and plot
  	icicle.refresh();
  	//end
  	icicle.layout.orientation = 'h';
	icicle.refresh();
}

//init controls
function controls() {
  	var jit = $jit;
  	var gotoparent = jit.id('update');
  	jit.util.addEvent(gotoparent, 'click', function() {
    	icicle.out();
  	});
	var select = jit.id('orientation');
  	jit.util.addEvent(select, 'change', function () {
    	icicle.layout.orientation = select[select.selectedIndex].value;
    	icicle.refresh();
  	});
  	var levelsToShowSelect = jit.id('i-levels-to-show');
  	jit.util.addEvent(levelsToShowSelect, 'change', function () {
   	 	var index = levelsToShowSelect.selectedIndex;
    	if(index == 0) {
      		icicle.config.constrained = false;
    	} else {        	
      		icicle.config.constrained = true;
      		icicle.config.levelsToShow = index+1;
    	}
    icicle.refresh();
  	});
}
//end
init();
</script>
<?php 
function montaArrayJit( $arrSql ){

	global $db;

	$sql = $arrSql['sql'];
	if( $sql ){
		if( $arrSql['filtro'] ){
			$sql = str_replace(Array('{id_pai}','{filtro}'),Array($arrSql['id_pai'],$arrSql['filtro'].' = '.$arrSql['id_filtro']),$sql);
		}
		$dados = $db->carregar($sql);
	}else{
		$dados = $sql;
	}
	if( is_array($dados) ){
		foreach( $dados as $k => $dado ){
			$id = explode('_', $dado['id']);
			$arrSql['filho']['id_pai'] = $dado['id'];
			$arrSql['filho']['id_filtro'] = $id[0];
			$dados[$k]['data'] = Array('$color'=>$arrSql['cor']);
			
			$htmlLinks = '<table cellpadding="3" cellspacing="2" style="margin:5px"><tr>';
			if($dado['linkplanilha']){
				$arLinkPlanilha = explode('_', $dado['linkplanilha']);
				if(!empty($arLinkPlanilha[0]) && !empty($arLinkPlanilha[1])){
					switch ($arLinkPlanilha[0]){
						case 'prg':
							$htmlLinksTemp .= '<td align="center" style="background:red;"><a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_ppa&acao=A&prgid='.$arLinkPlanilha[1].'" style="color:white;">Programa PPA</a></td>';
							break;
						case 'obj':
							$htmlLinksTemp .= '<td align="center" style="background:red;"><a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_ppa&acao=A&objid='.$arLinkPlanilha[1].'" style="color:white;">Objetivo PPA</a></td>';
							break;
						case 'met':
							$htmlLinksTemp .= '<td align="center" style="background:red;"><a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pne&acao=A&metid='.$arLinkPlanilha[1].'" style="color:white;">Meta PNE</a></td>';
							break;
					}
					
				}
			}
			if($dado['linkpainel']){
				$htmlLinksTemp .= '<td align="center" style="background:blue;"><a href="'.$dado['linkpainel'].'" target="_blank" style="color:white;">'.utf8_encode('Painel Estratégico').'</a></td>';
			}
			if($dado['linkindicador']){
				$htmlLinksTemp .= '<td align="center" style="background:blue;"><a href="'.$dado['linkindicador'].'" target="_blank" style="color:white;">Indicador</a></td>';
			}
			$htmlLinks .= $htmlLinksTemp ? $htmlLinksTemp : '<td></td>'; 
			$htmlLinks .= '</tr></table>';
			
			$dados[$k]['name'] = '<div align="center" valign="midlle" style="margin:5px;height:100%;" >'.$htmlLinks.'<b>'.utf8_encode($dados[$k]['name']).'</b></div>';
			$dados[$k]['tittle'] = utf8_encode($dados[$k]['tittle']);
			$dados[$k]['children'] = montaArrayJit( $arrSql['filho'] );
			unset($htmlLinksTemp);
		}
	}
	return $dados;
}
?>