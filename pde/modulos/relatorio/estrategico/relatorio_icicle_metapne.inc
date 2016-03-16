<script>
function abreLinkEstrategico(id)
{
	window.location='estrategico.php?modulo=relatorio/estrategico/relatorio_icicle_Objetivos&acao=A&objid='+id;
}

function abreLinkEixo(id)
{
	//alert(id);
}

function abreMetaPPA(id)
{
	window.location='estrategico.php?modulo=relatorio/estrategico/relatorio_icicle_ppa&acao=A&obeid='+id;
}
</script>
<?php

ini_set("memory_limit","250M");
set_time_limit(0);

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$titulo_modulo = "Relat�rio Icicle PPA";
monta_titulo( $titulo_modulo, 'Filtros Icicle' );

function montaArrayIcicle( $arrSql ){
	
	global $db;
	
	$sql = $arrSql['sql'];
	if( $sql ){
		if( $arrSql['filtro'] ){
			$sql = str_replace(Array('{id_pai}','{filtro}','{filtroStr}'),Array($arrSql['id_pai'],$arrSql['filtro'].' = '.$arrSql['id_filtro'],$arrSql['filtro'].' = \''.$arrSql['id_filtro'].'\''),$sql);
		}
		$dados = $db->carregar($sql);
	}
	if( is_array($dados) ){
		foreach( $dados as $k => $dado ){
			$id = explode('_', $dado['id']);
			$arrSql['filho']['id_pai'] = $dado['id'];
			$arrSql['filho']['id_filtro'] = $id[0];
			$dados[$k]['data'] = Array('$color'=>$arrSql['cor']);
			$dados[$k]['name'] = utf8_encode($dados[$k]['name']);
			$dados[$k]['tittle'] = utf8_encode($dados[$k]['tittle']);
			$dados[$k]['children'] = montaArrayIcicle( $arrSql['filho'] );
		}
	}
	return $dados;
}

$arrObjetivo = Array(
					'name' => '<div align="center" valign="midlle" ><br><b>'.utf8_encode('PPA 2012-2015').'</b></div>',
					'id' => '_raiz',
					'data'=>Array('$color'=>'white'));
$innerProg = '';
$innerObj = '';
if( $_REQUEST['obeid'] ){
	$innerProg = "INNER JOIN monitora.objetivo  obj ON obj.prgcod = p.prgcod
				 INNER JOIN painel.obj_estrategico_x_monitora opm ON opm.objid = obj.objid AND obeid = ".$_REQUEST['obeid'];
	$innerObj = "INNER JOIN painel.obj_estrategico_x_monitora opm ON opm.objid = obj.objid AND obeid = ".$_REQUEST['obeid'];
}
$sql = "SELECT 
			'<div align=\"center\" valign=\"midlle\" >
			<table cellpadding=\"3\" cellspacing=\"2\" style=\"margin:5px\"><tr>
			<td style=\"background:red;color:white;\" onclick=\"abreLinkEixo(' || p.prgid || ')\">Eixo PE</td>
			</tr></table>
			<b>'||p.prgdsc||'</b></div>' as name,
			p.prgcod||'_prog' as id
		FROM 
			monitora.programa p 
		INNER JOIN monitora.acao a ON a.prgid = p.prgid
		$innerProg 
		WHERE 
			p.prgano = '".$_SESSION['exercicio']."' 
			AND acasnrap = false 
		ORDER BY 
			p.prgcod, prgdsc";
$sql1 = "SELECT 
			'<div align=\"center\" valign=\"midlle\" >
			<table cellpadding=\"3\" cellspacing=\"2\" style=\"margin:5px\"><tr>
			<td style=\"background:red;color:white;\" onclick=\"abreLinkEstrategico(' || obj.objid || ')\">Objetivo Estrat�gico</td>
			</tr></table>			
			<b>'||objdsc||'</b></div>' as name,
			obj.objid||'_obj_{id_pai}' as id
		 FROM monitora.objetivo obj
		 $innerObj
		 WHERE 
			prgano = '2012' 
			AND {filtroStr}
			AND objstatus='A' 
		 ORDER BY objdsc";
$sql2 = "SELECT
			'<div align=\"center\" valign=\"midlle\" ><br><b>'||ictdsc||'</b></div>' as name,
			i.ictid||'_ini_{id_pai}' as id
		FROM monitora.iniciativa i 
		INNER JOIN monitora.objetivoiniciativa o ON o.ictid = i.ictid 
		WHERE 
			ictstatus='A' AND {filtro} 
		ORDER BY ictdsc";
$sql3 = "SELECT 
			'<div align=\"center\" valign=\"midlle\" ><br><b>'||acacod||' - '||acadsc||'</b></div>' as name,
			icaid||'_aca_{id_pai}' as id
		 FROM monitora.iniciativaacao 
		 WHERE 
			icastatus='A' AND {filtro} 
		 ORDER BY acadsc ";
$arrSql = Array('sql'	=>$sql, 
				'cor' 	=> '08ff02', 
				'filho' => Array('sql' 		=> $sql1, 
								 'cor' 		=> 'ffff00', 
								 'filtro' 	=> 'prgcod',
								 'filho' 	=> Array('sql' 		=> $sql2, 
													 'cor' 		=> 'ffd300', 
													 'filtro' 	=> 'o.objid',
													 'filho' 	=> Array('sql' 		=> $sql3, 
																		 'cor' 		=> 'e99200', 
																		 'filtro' 	=> 'ictid')
													)
								)
				);
$arrObjetivo['children'] = montaArrayIcicle( $arrSql );
?>

<?php

$botaoMeta10_2 = '<table cellpadding="3" cellspacing="2" style="margin:5px"><tr><td onclick="abreMetaPPA(5)" style="color:white;background:red;">Metas PPA</td></tr></table>';

$arrObjetivo2 = array(
		'name' => '<div align="center" valign="midlle" >'.$botaoMeta10_2.'<br><b>'.utf8_encode('Meta 10: Oferecer, no m�nimo, vinte e cinco por cento das matr�culas de educa��o de jovens e adultos na forma integrada � educa��o profissional, nos ensinos fundamental e m�dio').'.</b></div>',
		'id' => '_raiz',
		'data' => array('$color' => 'white'),
		'children' => array(
				0 => array(
						'name' => '<div align="center" valign="midlle"><br><b>'.utf8_encode('EJA integrado � educa��o profissional').'</b></div>',
						'id' => '10001_acao',
						'data' => array('$color' => '08ff02'),
						'tittle' => '',
						'children' => array(
								0 => array(
										'name' => '<div align="center" valign="midlle"><br><b>'.utf8_encode('Elevar o n�mero matr�culas de educa��o de jovens e adultos na forma integrada � educa��o profissional').'.</b></div>',
										'id' => '10005_2014',
										'data' => array('$color' => 'ffff00'),
										'tittle' => '',
										'children' => array()
								)
						)
				),
				1 => array(
						'name' => '<div align="center" valign="midlle" ><br><b>'.utf8_encode('Projovem').'</b></div>',
						'id' => '10002_acao',
						'data' => array('$color' => '08ff02'),
						'tittle' => '',
						'children' => array(
								0 => array(
										'name' => '<div align="center" valign="midlle"><br><b>'.utf8_encode('Atender 110 mil jovens').'.</b></div>',
										'id' => '10004_2014',
										'data' => array('$color' => 'ffff00'),
										'tittle' => '',
										'children' => array()
								)
						)
				)
		)
);

switch($_GET['obeid']){
	case 5:
		$arrObjetivo = $arrObjetivo2;
		break;
	default:
		$arrObjetivo = $arrObjetivo2;
		break;
}

/*
$arrObjetivo = array(
		'name' => 'Meta 10: Oferecer, no m�nimo, vinte e cinco por cento das matr�culas de educa��o de jovens e adultos na forma integrada � educa��o profissional, nos ensinos fundamental e m�dio.',
		'id' => '_raiz',
		'data' => array('$color' => 'white'),
		'children' => array(
				0 => array(
						'name' => 'EJA integrado � educa��o profissional',
						'id' => '10001_acao',
						'data' => array('$color' => '08ff02'),
						'tittle' => '',
						'children' => array(
								0 => array(
										'name' => 'Objetivo Estratégico Expandir, interiorizar, democratizar e qualificar a oferta de cursos de educação profissional e tecnológica, considerando os arranjos produtivos, sociais, culturais, locais e regionais, a necessidade de ampliação das oportunidades educacionais dos trabalhadores e os interesses e necessidades das populações do campo, indígenas, quilombolas, afrodescendentes, das mulheres de baixa renda e das pessoas com deficiência.',
										'id' => '23_obj_2031_prog',
										'data' => array('$color' => 'ffff00'),
										'tittle' => '',
										'children' => array(
												0 => Array(
														'name' => 'Ampliação da oferta de vagas em cursos de formação profissional a distância nas redes de educação profissional e tecnológica.',
														'id' => '61_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => array(
																0 => array(
																		'name' => '8252 - Educação Profissional e Tecnológica a Distância - E-TEC Brasil',
																		'id' => '77_aca_61_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																)

														)

												),
												1 => array(
														'name' => 'Expansão e reestruturação da Rede Federal de Educação Profissional e Tecnológica para ampliação do acesso, interiorização e diversificação da oferta, com condições de funcionamento, acessibilidade e permanência do estudante, considerando a otimização da capacidade instalada da estrutura física e de recursos humanos e assegurando condições de inclusão e equidade',
														'id' => '62_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => array(
																0 => array(
																		'name' => '20RG - Expansão e Reestruturação da Rede Federal de Educação Profissional e Tecnológica',
																		'id' => '78_aca_62_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																),
																1 => array(
																		'name' => '20RL - Funcionamento das Instituições Federais de Educação Profissional e Tecnológica',
																		'id' => '79_aca_62_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																)
														)
												),
												2 => array(
														'name' => 'Fomento �  expansão e ao desenvolvimento das redes de educação profissional e tecnológica, ao desenvolvimento de tecnologias educacionais, �  modernização do processo didático-pedagógico, �  elaboração e desenvolvimento de material didático, incluindo capacitação de docentes e técnicos administrativos, concessão de bolsas e cooperação internacional, além de apoio �  pesquisa, inovação e extensão.',
														'id' => '63_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => array(
																0 => array(
																		'name' => '6380 - Fomento ao Desenvolvimento da Educação Profissional',
																		'id' => '80_aca_63_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																)
														)
												),
												3 => array(
														'name' => 'Fomento �  expansão e qualificação das redes estaduais de educação profissional e tecnológica.',
														'id' => '64_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => array(
																0 => array(
																		'name' => '8652 - Modernização da Rede Pública Não Federal de Educação Profissional e Tecnológica',
																		'id' => '81_aca_64_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																)
														)
												),
												4 => array(
														'name' => 'Implantação de novos campi do Instituto Federal de Educação, Ciência e Tecnologia no Estado da Paraíba',
														'id' => '67_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => ''
												),
												5 => array(
														'name' => 'Promoção de condições de permanência e ampliação do acesso em instituições públicas e privadas, também por meio de vagas gratuitas e oferta de financiamento estudantil a alunos do ensino médio público, trabalhadores, populações do campo, indígenas, quilombolas, afrodescendentes, mulheres de baixa renda, pessoas com deficiência e beneficiários de programas sociais.',
														'id' => '65_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => array(
																0 => array(
																		'name' => '20RW - Apoio �  Formação Profissional e Tecnológica',
																		'id' => '82_aca_65_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																),
																1 => array(
																		'name' => '2994 - Assistência ao Educando da Educação Profissional',
																		'id' => '83_aca_65_ini_23_obj_2031_prog',
																		'data' => array('$color' => 'e99200'),
																		'tittle' => '',
																		'children' => ''
																)
														)
												),
												6 => array(
														'name' => 'Realização de estudos, avaliaçôes de impacto e análise do papel do Estado na promoção da educação profissional e tecnológica, levantamentos estatístico-educacionais e habilitação de instituições privadas para fins de expansão da oferta gratuita de cursos de educação profissional e acesso a programa de financiamento estudantil.',
														'id' => '66_ini_23_obj_2031_prog',
														'data' => array('$color' => 'ffd300'),
														'tittle' => '',
														'children' => ''
												)
										)
								)
						)
				)
		),
		1 => array(
				'name' => 'Projovem',
				'id' => '10002_acao',
				'data' => array('$color' => '08ff02'),
				'tittle' => '',
				'children' => array()
		)
);
 */
?>
<style>
/* white */
.white {
color: #606060;
border: solid 1px #b7b7b7;
background: #fff;
background: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#ededed));
background: -moz-linear-gradient(top, #fff, #ededed);
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffff', endColorstr='#ededed');
}
.white:hover {
background: #ededed;
background: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#dcdcdc));
background: -moz-linear-gradient(top, #fff, #dcdcdc);
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffff', endColorstr='#dcdcdc');
}
.white:active {
color: #999;
background: -webkit-gradient(linear, left top, left bottom, from(#ededed), to(#fff));
background: -moz-linear-gradient(top, #ededed, #fff);
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ededed', endColorstr='#ffffff');
}
.tip {
text-align: left;
width:auto;
max-width:500px;
}
.tip-title {
font-size: 11px;
text-align:center;
margin-bottom:2px;
}
#container {
width: 100%;
height: 300px;
margin:0 auto;
position:relative;
}
#center-container {
width:100%;
height:300px;
position:absolute;
top:0;
float:center;
background-color:#1a1a1a;
color:#ccc;
}
#infovis {
position:relative;
width:100%;
height:300px;
margin:auto;
overflow:hidden;
}
.text {
margin: 7px;
}
/*TOOLTIPS*/
.tip {
color: #111;
width: 139px;
background-color: white;
border:1px solid #ccc;
-moz-box-shadow:#555 2px 2px 8px;
-webkit-box-shadow:#555 2px 2px 8px;
-o-box-shadow:#555 2px 2px 8px;
box-shadow:#555 2px 2px 8px;
opacity:0.9;
filter:alpha(opacity=90);
font-size:10px;
font-family:Verdana, Geneva, Arial, Helvetica, sans-serif;
padding:7px;
} 

/*WHite*/
.white {
color: #606060;
border: solid 1px #b7b7b7;
background: #fff;
background: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#ededed));
background: -moz-linear-gradient(top, #fff, #ededed);
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffff', endColorstr='#ededed');
}
.white:hover {
background: #ededed;
background: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#dcdcdc));
background: -moz-linear-gradient(top, #fff, #dcdcdc);
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffff', endColorstr='#dcdcdc');
}
.white:active {
color: #999;
background: -webkit-gradient(linear, left top, left bottom, from(#ededed), to(#fff));
background: -moz-linear-gradient(top, #ededed, #fff);
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ededed', endColorstr='#ffffff');
} 

.button {
display: inline-block;
outline: none;
cursor: pointer;
text-align: center;
text-decoration: none;
font: 10px / 100% Arial, Helvetica, sans-serif;
padding: 0.5em 1em 0.55em;
text-shadow: 0px 1px 1px rgba(0, 0, 0, 0.3);
-webkit-border-radius: 0.5em;
-moz-border-radius: 0.5em;
border-radius: 0.5em;
-webkit-box-shadow: 0px 1px 2px rgba(0, 0, 0, 0.2);
-moz-box-shadow: 0px 1px 2px rgba(0, 0, 0, 0.2);
box-shadow: 0px 1px 2px rgba(0, 0, 0, 0.2);
}
.button:hover {
text-decoration: none;
}
.button:active {
position: relative;
top: 1px;
} 

</style>
<script language="javascript" type="text/javascript" src="../includes/Jit/jit.js"></script>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<?php if( $_REQUEST['obeid'] ){?>
	<tr>
		<td class="SubTituloDireita" width="45%"></td>
		<td>
			<a href="#" class="theme button white" onclick="window.location = 'estrategico.php?modulo=relatorio/estrategico/relatorio_icicle_Objetivos&acao=A'">Mostrar tudo</a>
		</td>
	</tr>
	<?php }?>
	<tr>
		<td class="SubTituloDireita" width="45%"></td>
		<td>
			<a id="update" href="#" class="theme button white">N�vel Anterior</a>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Orienta��o:</td>
		<td>
			<?php 
			$sql = Array(Array('codigo'=>'v', 'descricao' => 'Vertical'),
						 Array('codigo'=>'h', 'descricao' => 'Horizontal'));
			?>
			<?=$db->monta_combo('orientation',$sql,'S','','','',$txtdica='',$size='','', 'orientation', $return = false ) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Numero de N�veis:</td>
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
<script type="text/javascript">
var labelType, useGradients, nativeTextSupport, animate;

(function() {
  	var ua = navigator.userAgent,
      	iStuff = ua.match(/iPhone/i) || ua.match(/iPad/i),
      	typeOfCanvas = typeof HTMLCanvasElement,
      	nativeCanvasSupport = (typeOfCanvas == 'object' || typeOfCanvas == 'function'),
      	textSupport = nativeCanvasSupport 
       		&& (typeof document.createElement('canvas').getContext('2d').fillText == 'function');
	//I'm setting this based on the fact that ExCanvas provides text support for IE
  	//and that as of today iPhone/iPad current text support is lame
//	labelType = (!nativeCanvasSupport || (textSupport && !iStuff))? 'Native' : 'HTML';
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
//  $jit.id('max-levels').style.display = 'none';
	//init data
	var json = <?=simec_json_encode($arrObjetivo) ?>
	//init Icicle
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
  	icicle.layout.orientation = 'v';
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