<?php 

$arrObjetivo = Array(
		'name' => '<div align="center" valign="midlle" ><br><b>'.utf8_encode('Programa').'</b></div>',
		'id' => '_raiz',
		'data'=>Array('$color'=>'white'));

// Programa
$sql_prg = "SELECT
				prg.prgid || '_prg' as id,
				prg.prgdsc as name,
				'exo_' || exo.exoid as linkplanilha
			FROM
				monitora.jit_ppa_programa prg
			LEFT JOIN
				monitora.jit_pe_eixo exo ON exo.exoid = prg.exoid";

// Objetivo
$sql_obj = "SELECT
			objdsc as name,
			objid || '_obj_{id_pai}' as id,
			'obe_' || obe.obeid as linkplanilha
		FROM
			monitora.jit_ppa_objetivo obj
		LEFT JOIN
			monitora.jit_pe_objetivoestrategico obe ON obe.obeid = obj.obeid
		WHERE 
			objstatus = 'A' 
		AND 
			{filtro}";

// Meta
$sql_met = "SELECT
				met.metdsc as name,
				met.metid || '_met_{id_pai}' as id,
				'mtp_' || mtp.metid as linkplanilha
			FROM
				monitora.jit_ppa_meta met
			LEFT JOIN 
				monitora.jit_pne_meta mtp ON mtp.metid = met.metidpne
			WHERE
				met.metstatus = 'A'
			AND
				{filtro}";


// Acoes PDE2
$sql_pde = "SELECT
				acpdsc as name,
				acpid || '_acp_{id_pai}' as id,
				acplinkpainelestrategico as linkpainel
			FROM
				monitora.jit_ppa_acoes_pde2
			WHERE
				acpstatus = 'A'
			AND
				{filtro}";


// Aplica filtros 
$stParams = '';
if($_REQUEST['prgid']){
	$sql_prg .= " WHERE prgid = {$_REQUEST['prgid']} ";
	$stParams = "&prgid={$_REQUEST['prgid']}";
}else
if($_REQUEST['objid']){
	$sql_prg .= " INNER JOIN monitora.jit_ppa_objetivo obj on obj.prgid = prg.prgid AND obj.objid = {$_REQUEST['objid']} ";
	$sql_obj .= " AND objid = {$_REQUEST['objid']} ";
	$stParams = "&objid={$_REQUEST['objid']}";
}

$arrSql = array('sql'	=> $sql_prg,
				'cor' 	=> '08ff02',
				'filho' => array('sql' 		=>$sql_obj,
								 'cor' 		=> 'ffff00',
								 'filtro' 	=>'prgid',
								 'filho' 	=> Array('sql' 		=>$sql_met,
													 'cor' 		=> 'ffff00',
													 'filtro' 	=>'objid',
													 'filho' 	=> array('sql' 		=>$sql_pde,
																		 'cor' 		=> 'ffff00',
																		 'filtro' 	=>'metid',
																		 'filho' 	=> array()
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
			        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
					</div>
					<div style="float:right;cursor:pointer;"  onclick="window.location='estrategico.php?modulo=relatorio/estrategico/relatorio_jit_ppa&acao=A';">
						<img src="../imagens/icones/icons/mais.png" style="vertical-align:middle;" alt="Mostrar Árvore Completa" title="Mostrar Árvore Completa" />
					</div>
				</td>
			</tr>
			<tr>
				<td>
					<?php
					$menu = array(0 => array("id" => 1, "descricao" => "PE",	"link" => "/pde/estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A"),
							1 => array("id" => 2, "descricao" => "PPA",	"link" => "/pde/estrategico.php?modulo=relatorio/estrategico/relatorio_jit_ppa&acao=A{$stParams}"),
							2 => array("id" => 2, "descricao" => "PNE",	"link" => "/pde/estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pne&acao=A"));
					
					echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
					
					$linha1 = 'Relatório PPA';
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
								<?=$db->monta_combo('orientation',$sql,'S','','','',$txtdica='',$size='','', 'orientation', $return = false, 'h') ?>
								<input type="button" value="Mostrar tudo" onclick="window.location='estrategico.php?modulo=relatorio/estrategico/relatorio_jit_ppa&acao=A'" style="float:right" />
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
								<?=$db->monta_combo('i-levels-to-show',$sql,'S','','','','','','', 'i-levels-to-show', $return = false, 3 ) ?>
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
						case 'exo':
							$htmlLinksTemp .= '<td align="center" style="background:red;"><a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A&exoid='.$arLinkPlanilha[1].'" style="color:white;">Eixo PE</a></td>';
							break;
						case 'obe':
							$htmlLinksTemp .= '<td align="center" style="background:red;"><a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A&obeid='.$arLinkPlanilha[1].'" style="color:white;">'.utf8_encode('Objetivo Estratégico').'</a></td>';
							break;
						case 'mtp':
							$htmlLinksTemp .= '<td align="center" style="background:red;"><a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pne&acao=A&metid='.$arLinkPlanilha[1].'" style="color:white;">Meta PNE</a></td>';
							break;
					}
					
				}
			}
			if($dado['linkpainel']){
				$htmlLinksTemp .= '<td align="center" style="background:blue;"><a href="'.$dado['linkpainel'].'" target="_blank" style="color:white;">'.utf8_encode('Painel Estratégico').'</a></td>';
			}
			$htmlLinks .= $htmlLinksTemp ? $htmlLinksTemp : '<td></td>'; 
			$htmlLinks .= '</tr></table>';
				
			$dados[$k]['name'] = '<div align="center" valign="midlle" style="margin:5px;" >'.$htmlLinks.'<b>'.utf8_encode($dados[$k]['name']).'</b></div>';
			$dados[$k]['tittle'] = utf8_encode($dados[$k]['tittle']);
			$dados[$k]['children'] = montaArrayJit( $arrSql['filho'] );
			unset($htmlLinksTemp);
		}
	}
	return $dados;
}
?>