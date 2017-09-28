<?php 
include_once APPRAIZ.'includes/classes/RequestHttp.class.inc';
 
$obMunicipio	= new Municipio();
if( $_REQUEST['geraPDF'] )
{
	$municipio		= $obMunicipio->descricaoMunicipio($_SESSION['par']['muncod'], false);
	
	$sql = "SELECT
			e.entnome
		FROM
			par.entidade e
		INNER JOIN par.instrumentounidade 	iu 	ON iu.inuid = e.inuid
		INNER JOIN territorios.municipio 	m 	ON m.muncod = iu.muncod
		INNER JOIN territorios.estado 		est ON est.estuf = m.estuf
		WHERE
			iu.muncod = '".$_SESSION['par']['muncod']."' AND e.dutid = ".DUTID_PREFEITO;
	/*
	$sql = "SELECT 
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	funprefeito.funid = 2 AND 
            	--funprefeitura.funid = 1 AND 
            	mun.muncod ='".$_SESSION['par']['muncod']."'";
	*/
	$prefeito = $db->pegaUm($sql);
	
	$sql = "SELECT
				tprcampus
			FROM
				par.termopronatec
			WHERE
				muncod = '".trim($_SESSION['par']['muncod'])."'";
	$instituto = $db->pegaUm($sql);
	
	if( $_SESSION['par']['muncod'] != '5300108' )
	{
		$pref =	'<br />
			 	'.$prefeito.'
			 	<br /><br />
			 	Prefeito(a) de '.$municipio.'';
	}
	
	$html = '<center><font size="14px"><b>TERMO DE COMPROMISSO</b></font></center>
			 <br /><br />
			 <p>
			 Com a finalidade de ser credenciado como município sede para implantação do campus '.$municipio.' do Instituto Federal de Educação Profissional, Ciência e Tecnologia '.ucwords(strtolower($instituto)).', na qualidade de representante legal, devidamente autorizado, firmo em nome  do município de '.$municipio.' o compromisso  de promover a transferência dominial, no prazo máximo de 150 dias, a partir da assinatura deste termo,  devidamente legalizado, de área de terra ou infraestrutura  física edificada  em consonância com os requisitos listados  neste documento e aprovado pelo Instituto Federal  com vistas à instalação  do campus.
			 <br /><br />
			 Ciente de que o não cumprimento da transferência do imóvel para propriedade do ente federado até a data prevista, autoriza a instituição responsável pela implantação da unidade a buscar município alternativo  para execução do pleito.
			 <br /><br />
			 <center>
			 '.$municipio.', '.date('d').' de '.ucwords(strtolower(pegaMes())).' de 2011
			 <br /><br /><br /><br />
			 __________________________________________
			 '.$pref.'
			 </center>
			 </p>';
	
	$trans = get_html_translation_table(HTML_ENTITIES);
	$html = strtr($html, $trans);
	$html = str_replace("&lt;", "<", $html);
	$html = str_replace("&gt;", ">", $html);
	
	$http = new RequestHttp();
	$http->toPdfDownload($html, 'termo_compromisso');
	die;
}


function pegaMes()
{
	$mes = date('m');
	
	switch ($mes)
	{
		case 1: $mes = "JANEIRO"; break;
		case 2: $mes = "FEVEREIRO"; break;
		case 3: $mes = "MARÇO"; break;
		case 4: $mes = "ABRIL"; break;
		case 5: $mes = "MAIO"; break;
		case 6: $mes = "JUNHO"; break;
		case 7: $mes = "JULHO"; break;
		case 8: $mes = "AGOSTO"; break;
		case 9: $mes = "SETEMBRO"; break;
		case 10: $mes = "OUTUBRO"; break;
		case 11: $mes = "NOVEMBRO"; break;
		case 12: $mes = "DEZEMBRO"; break;
	}
	
	return $mes;
}
?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<link rel="stylesheet" href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/entidadesn.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom.min.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
		<style>
		.ui-widget
		{
			font-size: 10px;
		}
		</style>
		<script type="text/javascript">
		jQuery(document).ready(function()
		{
			jQuery('#btOk').click(function()
			{
				jQuery( "#dialog-message" ).dialog( "close" );
			});
			
		});
		
		jQuery(function() {
			jQuery( "#dialog:ui-dialog" ).dialog( "destroy" );
			
			jQuery( "#dialog-message" ).dialog({
				modal: true,
				height: 600,
				width: 550
			});
		});
		</script>
	</head>
	<body>
	<?php if($_REQUEST['tipoAba']=='TermoCompromisso'||$_REQUEST['tipoAba']==''){ ?>
		<div id="dialog-message" title="TERMO DE ADESÃO">
			<p style="font-weight:bold;text-align:center;">
				MINISTÉRIO DA EDUCAÇÃO<br />
SECRETARIA DE EDUCAÇÃO PROFISSIONAL E TECNOLÓGICA<br />
DIRETORIA DE DESENVOLVIMENTO DA REDE FEDERAL DE EPCT
			</p>
			<br />
			<p style="font-weight:bold;text-align:center;">
				PLANO DE EXPANSÃO DA REDE FEDERAL DE EDUCAÇÃO<br />
				PROFISSIONAL, CIENTÍFICA E TECNOLÓGICA - 2011/2014
			</p>
			<br />
			<p style="font-weight:bold;text-align:center;">
				TERMO DE ADESÃO 
			</p>
			<br />
			<p style="font-weight:bold;text-align:left;">
				 CONTEXTUALIZAÇÃO
			</p>
			<p style="text-align:left;">
			O Plano de Expansão da Rede Federal de Educação Profissional, Científica e Tecnológica com unidades a serem instaladas no quadriênio 2011/2014, constitui-se na iniciativa do Governo Federal, por intermédio do Ministério da Educação, de implantar, nos próximos quatro anos, 201 (duzentas e uma) novas unidades da Rede, sendo 81 já em processo de implantação e 120 objeto do atual termo de adesão. Podendo assim oferecer ao país condições favoráveis à formação e qualificação profissional nos diversos níveis e modalidades de ensino, suporte ao desenvolvimento da atividade produtiva, oportunidades de geração e disseminação de conhecimentos científicos e tecnológicos e estímulo ao desenvolvimento socioeconômico em níveis local e regional.
			<br /><br />
			As novas unidades de ensino estarão localizadas nos municípios, distribuídos nos 26 estados e no Distrito Federal, relacionados no Anexo I. A definição das localidades contempladas orientou-se por uma abordagem multicriterial, fundamentada em análise de variáveis econômicas, demográficas, socioambientais, culturais e geográficas abaixo relacionadas:
			<br /><br />
			1) PARA DEFINIÇÃO DE NÚMERO DE UNIDES POR ESTADO
			<br />
			<ul>
				<li>População do estado em relação à população do País; </li>
				<li>Presença de escolas federais e/ou estaduais de educação Profissional em atividade; </li>
				<li>Relação inversa do Índice de Desenvolvimento da Educação Básica - IDEB por estado;</li>
				<li>Percentual de jovens entre 14 e 18 anos nas séries finais do ensino fundamental.</li>
			</ul>
			2) PARA DEFINIÇÃO DE MUNICÍPIOS
			<ul>
				<li>Universalização de atendimento aos Territórios da Cidadania; </li>
				<li>Atendimento aos municípios populosos e com baixa receita per capita; </li>
				<li>Municípios com percentual elevado de extrema pobreza;  </li>
				<li>Atendimento prioritário aos municípios ou microrregiões com população acima de 50.000 habitantes;   </li>
				<li>Universalização do atendimento às mesorregiões Brasileiras; </li>
				<li>Municípios em microrregiões não atendidas por escolas federais; </li>
				<li>Municípios com Arranjos Produtivos Locais -APLs identificados; </li>
				<li>Entorno de grandes investimentos;</li>
				<li>Interiorização da oferta pública de Educação Profissional e Ensino Superior.</li>	
			</ul>
			</p>
			<p style="font-weight:bold;text-align:left;">DAS CARACTERÍSTICAS DO PLANO</p>
			<p style="text-align:left;">
			O Ministério da Educação, na qualidade de órgão responsável pela implantação das novas unidades de ensino e de mantenedor da Rede Federal de Educação Profissional, Científica e Tecnológica, será o principal agente na realização dos investimentos em obras de construção, ampliação e reforma de espaços físicos; aquisição de equipamentos, mobiliários e acervo bibliográfico para as atividades administrativas e didático-pedagógicas, incluindo as práticas laboratoriais; seleção e contratação de pessoal docente e técnico administrativo para adequado funcionamento das unidades; e alocação de recursos financeiros destinados à gestão e manutenção dos novos estabelecimentos de ensino.
			<br /><br />
			Para confirmação da implantação do novo campus do Instituto Federal, conforme proposto, caberá ao Município (e/ou Distrito Federal) hospedeiro apenas a doação à União/ Instituto Federal correspondente, de área de terra ou infraestrutura pré-existente compatível com a instalação da referida unidade, que se dará através da assinatura de TERMO DE COMPROMISSO, por parte do município pré-selecionado, se comprometendo a promover a doação de terreno, terra nua e/ou imóvel edificado que atendam os requisitos abaixo relacionados:
			</p>
			<p style="font-weight:bold;text-align:center;">
			Terreno - área nua
			</p>
			<p style="text-align:left;">
			<ul>
				<li>Terreno no perímetro urbano destinado à implantação de unidade de ensino que atuará, prioritariamente, no setor de indústria, comércio e/ou serviços. A área disponibilizada deverá ser de no mínimo 20.000 m² (metros quadrados), com área ideal acima de 50.000 m².</li>
				<li>Terreno em área rural, com afastamento limite máximo de 08 Km do perímetro urbano, destinado à implantação de unidade de ensino que atuará, prioritariamente, no setor agropecuário. A área deverá ter no mínimo 60 hectares, com área ideal acima dos 150 hectares.</li>
				<li>O imóvel indicado deverá apresentar condições adequadas de interligação às redes públicas de abastecimento de água, eletrificação e telefonia.</li>
				<li>O imóvel indicado deverá apresentar condições favoráveis de acesso aos alunos e servidores, mediante existência de:</li>
				<ul>
					<li>Pavimentação nas vias de tráfego que dão acesso ao imóvel indicado;  linhas regulares de transporte urbano ou rural; ou serviços públicos de transporte escolar.</li>
					<li>Características topográficas favoráveis à realização de obras civis.</li>
					<li>Características geológicas adequadas no caso de imóveis rurais, como a existência de veio de água perene e de um mínimo de 50% de área agricultável.</li>
					<li>Comprovação da inexistência de óbices de natureza ambiental, jurídica e dominial, que possam inviabilizar ou retardar a sua utilização para os fins em questão.</li>
				</ul>
			</ul>
			</p>
			<br />
			<p style="font-weight:bold;text-align:center;">
			Imóvel edificado
			</p>
			<p style="text-align:left;">
			Imóvel edificado poderá substituir a doação de terreno, <b>agilizando o processo de implantação da escola</b>, desde que vistoriado e aprovado pelo Instituto Federal responsável e que atenda os requisitos abaixo:
			<br />
			<ul>
				<li>Possuir área compatível com a proposta e facilidade de adaptação da estrutura existente para fins de constituição de espaços adequados à prática pedagógica e de formação profissional</li>
				<li>Confirmação, mediante parecer técnico expedido por órgão ou entidade competente, sobre a inexistência de patologias ou anomalias de construção que impliquem comprometimento estrutural na edificação indicada pelo proponente</li>
				<li>Promover doação legal e definitiva do bem imóvel a fim de ser incorporado ao patrimônio do Instituto Federal responsável pela instalação da unidade.</li>
			</ul>
			</p>
			<br />
			<p style="font-weight:bold;text-align:left;">OBSERVAÇÕES:</p>
			<p style="text-align:left;">
			&nbsp;&nbsp;&nbsp;A. No caso de a regularização definitiva do imóvel demandar período que  coloque em risco a implantação da unidade  no prazo programado,  o município responsável será substituído por outro município  com as mesmas características.
			<br />
			&nbsp;&nbsp;&nbsp;B. Terrenos e/ou imóveis edificados, a serem  disponibilizados  serão cadastrados  em tela própria deste módulo, e   somente serão aceitos após vistoria e aprovação por parte do corpo dirigente do Instituto Federal responsável pela implantação da unidade.
			</p>
			<br />
			<center><input type="button" value="OK" style="width:80px;" id="btOk" /></center>
			<br />
		</div>
		<?php } ?>
		<?php
		
		$anoref = $_GET['ano'] ? $_GET['ano'] : '2011';
		$_SESSION['par']['preid'] 	= ($_REQUEST['preid']) ? $_REQUEST['preid'] : $_SESSION['par']['preid'];
		
		$_SESSION['par']['prog'] = $_REQUEST['prog'] != $_SESSION['par']['prog'] ? $_REQUEST['prog'] : 'nulo';
		
		if($_REQUEST['preid'] && !$_GET['muncod']){
			$_SESSION['par']['preid'] 	= $_REQUEST['preid'];
			$_SESSION['par']['estuf'] 	= $_SESSION['par']['estuf'] ? $_SESSION['par']['estuf'] : $obMunicipio->recuperarUF($_SESSION['par']['muncod']);
		}
		
		if($_GET['muncod']){
			$_SESSION['par']['preid'] 	= $_GET['preid'];
			$_SESSION['par']['muncod'] 	= $_GET['muncod'];
			$_SESSION['par']['estuf'] 	= $obMunicipio->recuperarUF($_SESSION['par']['muncod']);
		}
		
		if(!$_REQUEST['preid']){
			unset($_SESSION['par']['preid']);
		}
		
		//includes
		
		require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
		require_once APPRAIZ . 'includes/workflow.php';
		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		//Fim includes
		$_SESSION['sisdiretorio'] = 'par';
		
		$tipoAba = ($_REQUEST['tipoAba']) ? $_REQUEST['tipoAba'] : 'TermoCompromisso';
		$chamada = 'pronatec'.ucwords($tipoAba);
		
		$arParametros = array('visao' => $chamada);
		$obItensComposicaoControle = new ItensComposicaoControle();
		
		$obItensComposicaoControle->visualizacaoItensComposicaoPronatec($arParametros);
		?>
	</body>
</html>